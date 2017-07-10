#include <iostream>
#include <algorithm>
#include <array>

int main()
{

    /*
        Here I test some algorithms.
    */

    std::array<int, 10> arr;

    std::iota(arr.begin(), arr.end(),1); // fill it up from 1

    std::for_each(arr.begin(), arr.end(),
                  [](int a){ std::cout << a << std::endl; });
                  // prints out the values of the array

    std::random_device rd;

    std::cout << std::endl;

    std::mt19937 gen(rd());

    std::shuffle(arr.begin(), arr.end(), gen);

    std::for_each(arr.begin(), arr.end(),
                  [](int a){ std::cout << a << std::endl; });
                  // prints out the values of the array

    std::cout << std::endl;

    int n1 = 5;
    int n2 = -3;

    auto first_loc1 = std::find(arr.begin(),arr.end(),n1);

    auto first_loc2 = std::find(arr.begin(),arr.end(),n2);

    if(first_loc1 != arr.end()){

        std::cout << "The array contains the number " << n1 << ".\n";

    }else{

        std::cout << "The array does not contain " << n1 << ".\n";

    }

    if(first_loc2 != arr.end()){

        std::cout << "The array contains the number " << n2 << ".\n";

    }else{

        std::cout << "The array does not contain " << n2 << ".\n";

    }

    std::cout << std::endl;

    std::sort(arr.begin(), arr.end());

    std::for_each(arr.begin(), arr.end(),
                  [](int a){ std::cout << a << std::endl; });
                  // prints out the values of the array

    return 0;

}
