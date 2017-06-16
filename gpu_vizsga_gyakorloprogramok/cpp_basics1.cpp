#include <iostream>

auto sq = [](auto val){ return val*val; };
/*
  This is a C++14 feature. We do not need a return value,
  it is completely guessed by the compiler.
*/

int main() {

  std::cout << "The square of the integer 5 is: " << sq(5) << std::endl;
  std::cout << "The square of the float 5.0 is: " << sq(5.0) << std::endl;
  std::cout << "The square of the float 5.1 is: " << sq(5.1) << std::endl;
  std::cout << "The square of the float -5.4 is: " << sq(-5.4) << std::endl;
  return 0;
}

/*
  Needs to be compiled using -std=c++14 flag.
*/
