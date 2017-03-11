#include <iostream>
#include <numeric>
#include <array>
#include <vector>
#include <algorithm>
// Needed to use sharedd_prt
#include <memory>

struct VectorN{
    std::vector<float> v;
    // Default constructor
    //VectorN()
    float& operator[](int i){return v[i];};
    const float& operator[](int i)const{return v[i];};
    // Copy constructor to copy data from
    // one to the other keeping both objects!
    //VectorN( VectorN const& cpy);
    // C++11 move constructor, give the 
    // value by ,,posession'' so the given
    // object ceases to exist.
    //VectorN( VectorN && mv);

    int getSize(){
        return v.size();
    }

    // Operator overloading
    VectorN& operator+=(VectorN const& other){
        if(other.v.size() == this->v.size()){
            std::transform(this->v.begin(),this->v.end(),other.v.begin(),this->v.begin(),std::plus<float>());
        }else{
            std::cout << "Error adding vectors." << std::endl;
        }
        return *this;
    };
    VectorN& operator=(VectorN const& other){
        this->v = other.v;
        return *this;
    }
    VectorN& operator+(VectorN const& other);

    // Destructor
    //~VectorN();
};

int main()
{
    // C++11 
    using Vec = VectorN;
    /*
        This algorithm calculates the sum of squares
        from 1 to a 100. Using std::iota and std::accumulate
        algorithms to provide a more effcient and not error prone
        way to do calculation.
    */
    std::array<int, 100> values;
    std::iota(values.begin(),values.end(),1);
    auto sum = std::accumulate(values.begin(),
    values.end(),
    0,
    [](int a, int b){return a+b*b;});
    std::cout << "Sum is: " << sum << std::endl;

    std::cout << std::endl;

    // Allocate memory safely on the heap with C++11
    std::shared_ptr<Vec> a = std::make_shared<Vec>(Vec{{1.2,2.1,2.3,4.3,4.9}});
    // Allocate memory on the stack
    Vec b{{2.1,1.2,2.3,3.4,4.5}};
    for(int i=0;i<b.getSize();i++){
        std::cout << b[i] << std::endl;
    }

    std::cout << std::endl;

    b+=(*a);
    for(int i=0;i<b.getSize();i++){
        std::cout << b[i] << std::endl;
    }

    std::cout << std::endl;

    Vec c = *a;

    for(int i=0;i<c.getSize();i++){
        std::cout << c[i] << std::endl;
    }

    std::cout << std::endl;

    // Until this point everything is working fine WITHOUT the written
    // constructors and destructors.

    return 0;
}
