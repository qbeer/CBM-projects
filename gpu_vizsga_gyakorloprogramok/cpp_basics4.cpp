#include <iostream>
#include <memory> // to be able to use shared pointers
#include <array>
#include <vector>
#include <initializer_list>
#include <algorithm>
#include <numeric>

template <typename IT1, typename IT2, typename F>
void zipassign2(IT1 it1begin, IT1 it1end, IT2 it2begin, F func){

    auto it1 = it1begin;
    auto it2 = it2begin;

    while(it1 != it1end){

        *it1 = func(*it1, *it2);
        it1++;
        it2++;

    }

}

template <typename T, int n>
struct Vector{

    std::array<T, n> cont;

    T& operator[](int i){ return cont[i]; }

    const T& operator[](int i)const{ return cont[i]; }

    Vector();

    Vector( std::initializer_list<T> li ){

        std::copy(li.begin(), li.end(), cont.begin());

    } // with this it is possible to initilaize a vector with
      // an initializer list

    Vector( std::vector<T>  vec ){

        std::copy(vec.begin(), vec.end(), cont.begin());

    } // it is possible to initilaize our vector with a standard
      // c++ vector type

    Vector<T, n>& operator*=( const T& c ){

        std::for_each(cont.begin(), cont.end(), [&](T& elem){ elem*=c; });

        return *this;

    }

    Vector<T, n> operator+=( const Vector<T, n>& other ){

        zipassign2(cont.begin(), cont.end(),
                   other.cont.begin(),
                   std::plus<T>());
                // add to vectors together

        return *this;

    }

    Vector<T, n>& operator=( const Vector<T, n>& other ){

        std::copy(other.cont.begin(), other.cont.end(), cont.begin());

        return *this;

    }

};

template <typename T, int n>
Vector<T, n> operator+( Vector<T, n> const& u,
                            Vector<T, n> const& v )
    {

        Vector<T, n> res;

        std::transform(u.cont.begin(), u.cont.end(),
                       v.cont.begin(), res.cont.begin(),
                       std::plus<T>());

        return res;

    }

int main()
{

    std::vector<double> v(10);

    std::iota(v.begin(), v.end(), 1);

    Vector<double, 10> vec1(v);

    for(int i=0; i<10; i++){

        std::cout << vec1[i] << "\t";

    }

    std::cout << std::endl;

    Vector<double, 10> vec2{ 1.1, 2.1, 3.2, 0.2, 0.3, 3.2, 1.2, 3.0, 9.7, 10.0 };

    for(int i=0; i<10; i++){

        std::cout << vec2[i] << "\t";

    }

    std::cout << std::endl;

    Vector<double, 10> vec3 = vec1;

    vec3+=vec2;

    for(int i=0; i<10; i++){

        std::cout << vec3[i] << "\t";

    }

    std::cout << std::endl;

    return 0;

}
