#include "include/Vector.hpp"

int main()
{
   idc::Vector<int> vec;
   vec.allow_warnings();
   int a = 10;
   vec.push_back(a);
   // vec.push_back(20);
   // vec.push_back(30);
   // vec.push_back(40);
   // vec.push_back(50);
   // vec.push_back(60);
   // vec.push_back(70);

   vec.display();

   // vec.clear();
   
   vec.display();

   if (vec.is_empty())
      std::cout << vec.code() << std::endl;

   return 0;
}