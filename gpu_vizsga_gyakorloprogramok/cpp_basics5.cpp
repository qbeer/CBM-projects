#include <iostream>

template <typename T>
auto sum(T x){ return x; }

template <typename T1, typename T2, typename... Ts>
auto sum(T1 x1, T2 x2, Ts... xs)
{

    return sum(x1+x2, xs...);

}

/*
    This way the functions can take many parameters.
*/

int main()
{

    auto summed = sum(1,2,3,4,5,6,7,8,9,10);

    std::cout << summed << std::endl;

    return 0;

}
