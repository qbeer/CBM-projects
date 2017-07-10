#include <iostream>
#include <array>  // use instead of conventional arrays
#include <vector> // use instead of conventional vectors
                  // similair to this with more functionality
                  // is valarray
                  // in general these are containers
#include <algorithm> // usefool algorithms written by the
                     // coders of C++ itself
                     // extremely useful library

#include <numeric> // for std::iota, std::accumulate

int main()
{

    /*
        Calculate the sum of squares from a given number to another
        incrementing by one.
    */

    std::array<int, 5> values;

    std::iota(values.begin(), values.end(),1); // start with 1 and fill the
                                               // container incrementing

    auto sum = std::accumulate(values.begin(),values.end(),
                               0, [](int a, int b){ return a + b*b; });

                            // start to accumulate the elements of values
                            // from 0 while multiplicating them be themselves
                            // assigning the temporary value to sum then
                            // repeat. it is a loop in a really short form

    std::cout << "Sum is: " << sum << std::endl;

    return 0;

}
