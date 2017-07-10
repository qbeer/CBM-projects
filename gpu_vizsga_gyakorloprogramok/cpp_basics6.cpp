#include <iostream>
#include <tuple>

/*
    With this program it is possible to add numbers together as
    ratios.
*/

std::tuple<int, int, double> ratio(int a,int b){

    return std::make_tuple( a, b, (double)a/(double)b );

}

std::tuple<int, int, double> add( std::tuple<int, int, double>& a,
                                  std::tuple<int, int, double>& b)
{

    return std::make_tuple( std::get<0>(a)+std::get<0>(b),
                            std::get<1>(a)+std::get<1>(b),
                            std::get<2>(a)+std::get<2>(b));

}

int main()
{

    auto x = ratio(5,7);
    auto y = ratio(3,4);

    auto z = add(x,y);

    std::cout << "(" << std::get<0>(x) << "," << std::get<1>(x) << ","
              << std::get<2>(x) << ")\n";

    std::cout << "(" << std::get<0>(y) << "," << std::get<1>(y) << ","
              << std::get<2>(y) << ")\n";

    std::cout << "(" << std::get<0>(z) << "," << std::get<1>(z) << ","
              << std::get<2>(z) << ")\n";

    return 0;

}
