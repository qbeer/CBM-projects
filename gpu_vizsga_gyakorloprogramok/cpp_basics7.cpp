#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include <memory>
#include <sstream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <chrono>

/*
    This program reads a file containing a data structure like this:

    pi+ 1.5686 1.5 2.3 4.5  1.6 2.2 4.6 1.7 2.1 4.7
    pi- 1.5686 -1.5 2.3 -0.5  -1.6 2.2 0.6  -1.7 2.1 0.7
    proton 5.225 0.0 1.1 0.55  0.01 1.1 0.65  0.02 1.1 0.75  0.03 1.1 0.85  0.04 1.1 0.95
    K+ 3.953 3.2 2.1 0.05  3.45 2.3 0.15

*/

struct Point3D{

    double x, y, z;

};

std::istream& operator>>( std::istream& is, Point3D& P ){

    is >> P.x >> P.y >> P.z;

    return is;

}

std::ostream& operator<<( std::ostream& os, Point3D const& P ){

    os << P.x << " " << P.y << " " << P.z;

    return os;

}

struct Particle{

    std::string name;
    double energy;
    std::vector<Point3D> path;

};

std::istream& operator>>( std::istream& is, Particle& part ){

    std::getline(is, part.name, ' '); // get string until first ' '
    if(part.name.size() == 0){ return is; } // error checking

    std::string tmp;
    std::getline(is, tmp, ' ');
    if(tmp.size() == 0){ return is; }

    part.energy = std::stod(tmp);

    std::getline(is, tmp, '\n');
    if(tmp.size() == 0){ return is; }

    std::istringstream ss(tmp);
    part.path.clear();
    std::copy(std::istream_iterator<Point3D>(ss),
              std::istream_iterator<Point3D>(),
              std::back_inserter(part.path));

    return is;

}

std::ostream& operator<<( std::ostream& os, Particle const& part ){

    os << part.name << " " << part.energy << " ";
    for( auto const& i : part.path ){

        os << i << " ";

    }

    return os;

}

int main()
{

    std::vector<Particle> particles;

    std::ifstream fin("particles.dat");

    auto t1 = std::chrono::high_resolution_clock::now();

    std::copy( std::istream_iterator<Particle>(fin),
               std::istream_iterator<Particle>(),
               std::back_inserter(particles));

    auto t2 = std::chrono::high_resolution_clock::now();

    long long duration =
        std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count();

    std::for_each( particles.begin(), particles.end(),
                   [&](Particle const& p){ std::cout << p << std::endl; } );

    std::cout << "\nReading file took " << duration << " microseconds.\n";

    return 0;

}
