#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>

using namespace std;

int main()
{
    std::vector<int> v = {3,2,1,5,4,3,2,1,2,2};
    /*
        With this lambda function I can square all the elements of my vector/array
        of type int. By the followind for_each algorithm and lambda I simply write
        the containement of v to the console output.
    */
    std::transform(v.begin(),v.end(),v.begin(),[](int i)->int{return i*i;});
    std::for_each(v.begin(),v.end(),[](int n)->void{std::cout << n << " ";});

    std::cout << std::endl << std::endl;

    std::vector<int> u(10);
    /*
        The iota function starts the vector from the third parameter and increments
        every following element by one, this way filling the container efficently.
        It is operating on INTEGERS.
    */
    std::iota(u.begin(),u.end(),-1);
    std::for_each(u.begin(),u.end(),[](int n)->void{std::cout << n << " ";});

    std::cout << std::endl << std::endl;

    /*
        The shuffle function takes container iterators and randomly shuffles the
        values between them using a random device provided by the standard library.
    */
    std::shuffle(v.begin(),v.end(),std::mt19937{std::random_device{}()});
    std::shuffle(u.begin(),u.end(),std::mt19937{std::random_device{}()});
    /*
        Here's the most efficient way I've yet found to display the values of
        a container with least possible code in C++.
    */
    for(auto n: v) std::cout << n << ' ';
    std::cout << std::endl;
    for(auto n: u) std::cout << n << ' ';

    std::cout << std::endl << std::endl;

    std::vector<double> a{0.,1.,2.,3.,4.};
    std::vector<double> b{5.,4.,3.,2.,1.};
    /*
        The inner_product algorithm as its name refers returns the so called
        dot product of two vectors.
    */

    double dot_prd = std::inner_product(a.begin(),a.end(),b.begin(),0);
    for(auto n: a) std::cout << n << ' ';
    std::cout << std::endl;
    for(auto n: b) std::cout << n << ' ';
    std::cout << std::endl;
    std::cout << "The inner product of these vectors is: " << dot_prd;

    std::cout << std::endl << std::endl;

    std::vector<double> z(10);
    std::random_device rnd;
    std::mt19937 gen(rnd());
    std::uniform_real_distribution<> dist(0.,1.);
    std::generate(z.begin(),z.end(),[&gen, &dist]{return dist(gen);});
    std::cout.precision(3);
    for(auto n: z) std::cout << n << ' ';
    std::cout << std::endl;
    return 0;
}
