#include <iostream>
#include <vector>
#include <map>

template<typename T>
struct logging_allocator {
   using value_type = T;
   T *allocate(std::size_t n){
     auto p = std::malloc(n * sizeof(T));
     std::cout << "allocate n = " << n << '\n';
     if (!p)
       throw std::bad_alloc();
     return reinterpret_cast<T *>(p);
   }
   void deallocate(T *p, std::size_t n){
     std::cout << "deallocate n = " << n << std::endl;
     std::free(p);
   }
   template<typename U, typename ...Args>
   void construct(U *p, Args &&...args){
     new(p) U(std::forward<Args>(args)...);
     std::cout << "construct\n";
   }
   void destroy(T *p){
     std::cout << "destroy\n";
     p->~T();
   }
};

template<class T, class Allocator = std::allocator<T>>
  class SimpleList {
    struct Node {
      Node* next;
      T current;
    };
public:
  SimpleList():head(nullptr){}
  ~SimpleList(){
    Node* next = head;
    while (next){
      Node* deleteNode = next;
      next = next->next;
      delete deleteNode;
    }
  }
  void push_back(T l){
    Node *n = new Node();
    n->current = l;
    n->next = head;
    head = n;
  }
   T pop_first(){
     Node *n = head;
     T ret = n->current;
     head = head->next;
     delete n;
     return ret;
   }
private:
  Node *head;
};


// template<typename T>
// struct logging_allocator {
//     using value_type = T;
//
//     using pointer = T*;
//     using const_pointer = const T*;
//     using reference = T&;
//     using const_reference = const T&;
//
//     template<typename U>
//     struct rebind {
//         using other = logging_allocator<U>;
//     };
//
//     logging_allocator() = default;
//     ~logging_allocator() = default;
//
//     template<typename U>
//     logging_allocator(const logging_allocator<U>&) {
//
//     }
//
//     T *allocate(std::size_t n) {
//         std::cout << "allocate: [n = " << n << "]" << std::endl;
//         auto p = std::malloc(n * sizeof(T));
//         if (!p)
//             throw std::bad_alloc();
//         return reinterpret_cast<T *>(p);
//     }
//
//     void deallocate(T *p, std::size_t n) {
//         std::cout << "deallocate: [n  = " << n << "] " << std::endl;
//         std::free(p);
//     }
//
//     template<typename U, typename ...Args>
//     void construct(U *p, Args &&...args) {
//         std::cout << "construct" << std::endl;
//         new(p) U(std::forward<Args>(args)...);
//     }
//
//     void destroy(T *p) {
//         std::cout << "destroy" << std::endl;
//         p->~T();
//     }
// };

int main(int, char *[]) {
   auto v = std::vector<int, logging_allocator<int>>{};
   v.reserve(5);
   for (int i = 0; i < 6; ++i) {
       std::cout << "vector size = " << v.size() << std::endl;
       v.emplace_back(i);
       std::cout << std::endl;
   }

   for (auto i: v) {
       std::cout << i << std::endl;
   }

    auto m1 = std::map<int, int>{};
    for (int i = 0; i < 10; ++i)
      m1[i] = i;

    auto m2 = std::map<int, int, std::less<int>,
        logging_allocator<std::pair<const int, int>>>{};
    for (int i = 0; i < 10; ++i){
        m2[i] = i;
      }

    std::cout << "My list" << std::endl;
     SimpleList<int> l;
    l.push_back(3);
    l.push_back(5);
    std::cout << l.pop_first() << std::endl;

    std::cout << "\n**** end of main ****\n" << std::endl;

    return 0;
}
