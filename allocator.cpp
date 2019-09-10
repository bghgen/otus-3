#include <iostream>
#include <vector>
#include <map>

template<typename T, size_t reserve_size = 10>
struct logging_allocator {
   using value_type = T;
   size_t allocated_size;
   size_t created_size;
   void* p;

   template<typename U>
   struct rebind {
     using other = logging_allocator<U>;
   };

   logging_allocator():allocated_size(0), created_size(0){}

   T *allocate(std::size_t n){
          std::cout << "call allocate n = " << n << '\n';
     if (created_size + n > reserve_size){
         throw;
     }

     void* ptr;
     if (allocated_size == 0){
       ptr = std::malloc(reserve_size * sizeof(T));
       std::cout << "reserve n = " << reserve_size << '\n';
       if (!ptr)
         throw std::bad_alloc();

       allocated_size = reserve_size;
       p = ptr;
     }
     else {
       std::cout << "call allocate but not allocate " << n << " elements\n";
       ptr = p;
       p += n;
     }
     return reinterpret_cast<T *>(ptr);
   }
   void deallocate(T *p, std::size_t n){
     std::cout << "deallocate n = " << n << std::endl;
     std::free(p);
     allocated_size = 0;
   }
   template<typename U, typename ...Args>
   void construct(U *p, Args &&...args){
     new(p) U(std::forward<Args>(args)...);
     std::cout << "construct\n";
     created_size += sizeof...(Args);
   }
   void destroy(T *p){
     std::cout << "destroy\n";
     p->~T();
     --created_size;
   }
};

template<class T, class Allocator = std::allocator<T>>
  class CustomContainer {
public:
  CustomContainer() = default;
  ~CustomContainer(){
    if (size()){
      T* fist_p = data.at(0);
      const size_t size = data.size();
      for (auto i: data){
        alloc.destroy(i);
      }
      alloc.deallocate(fist_p, size);
    }
  }
  void push_back(T item){
    T* p_item = alloc.allocate(1);
    alloc.construct(p_item, item);
    data.push_back(p_item);
  }

  T get(size_t n){
    if (n >= size())
      throw std::out_of_range("");
    else
      return *data.at(n);
  }

  size_t size(){
    return data.size();
  }

private:
  std::vector<T*> data;
  Allocator alloc;
};



int main(int, char *[]) {
   auto v = std::vector<int, logging_allocator<int>>{};
   v.reserve(5);
   for (int i = 0; i < 5; ++i) {
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
    CustomContainer<int, logging_allocator<int>> l;
    l.push_back(3);
    l.push_back(5);
    std::cout << "oh hi" << std::endl;

    std::cout << "\n**** end of main ****\n" << std::endl;

    return 0;
}
