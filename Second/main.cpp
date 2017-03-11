#include <iostream>
#include <algorithm>
#include <vector>

/*
    We want to generalize the parameters of a function
    to an indefinite amount. Doing so we might loose type-
    checking. This issue is solved by templates and C++11
    ... syntax.
*/

// This line of code needed due to the fact that the one below with
// the argument pack does not know what to return when only one 
// parameter is passed to it.
template<typename T>
auto sum(T x){
    return x;
}

template<typename T1,
         typename T2,
         typename... Ts>
// auto return type is C++14 
auto sum(T1 x, T2 y, Ts... xs)
{
    return sum(x+y,xs...);
} 
// Recursive function. This way it sums 
// all arguments.

int main()
{
    auto val = sum(1,2,3,4,5,6,7,8,9,10,45,4,45,4,5,45,4,54,545423,423,4,3,42,312,32,34);
    std::cout << val << std::endl;
    val = sum(1,2,3,4,5,6,7,8,9,10,423,4,3,42,312,32,34);
    std::cout << val << std::endl;
    val = sum(1,2,35,4,45,4,5,45,4,54,0,423,4,3,42,312,32,34);
    std::cout << val << std::endl;
    // It does not return floating point numbers even though there are some amongst these.
    // However, it works pretty well.
    val = sum(1.,2.,3,4,5,6,7,8,9,10,45,4,45,4,5,45,4,54,2.3,423,4,3,42,312,32,34);
    std::cout << val << std::endl;
    /*
     This obviously generates an error, but I needed to try it.
    val = sum("hey",2,3,4,5,6,7,8,9,10,45,4,45,4,5,45,4,54,545423,423,4,3,42,312,32,34);
    std::cout << val << std::endl;
    */
    return 0;
}
