
#include <iostream>

using namespace std;

#include <util/serializer.hpp>

int main () {

  std::cout << "Hello";

  Serializer sr;
  std::string str (" World\n");
  sr << str;
  std::cout << sr.toByteBuffer ().c_str ();
}
