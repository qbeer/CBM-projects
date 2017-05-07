#include <iostream>
#include <fstream>
#include <thread>
#include <cmath>
#include <algorithm>
#include <string>
#include <deque>
#include <iterator>
#include <future>

const double eps = 10e-32;

double p( const std::vector< double >& poly, double x ){

    return std::accumulate(poly.begin(), poly.end(), 0.0, [&](double acc, double k){ return acc*x + k; });

}

std::vector< double > deriv( const std::vector< double >& poly ){

    std::vector< double > dpoly(poly.size()-1);

    for( int i=0; i<poly.size()-1 ; i++ ){

        dpoly[i] = poly[i]*(poly.size()-1-i); // generate derivative

    }

    return dpoly;

}

int main( int argc, char* argv[] )
{

    std::ifstream fin(argv[1]); // argv[1] should be the filename

    double from = atof(argv[2]); // argv[2] should be the start of the
                                 // domain on which we want to find the roots

    double to = atof(argv[3]); // argv[3] should be the end of the domain

    if(!fin.is_open()){

        std::cout << "Could not open: " << argv[1] << "\n";
        exit(1);

    }

    std::vector< double > poly;

    std::copy(std::istream_iterator<double>(fin),
              std::istream_iterator<double>(),
              std::back_inserter(poly));

    std::vector< double > dpoly = deriv(poly);

    std::vector< double > ddpoly = deriv(dpoly);

    // I have got the polynomial and generated its derivatives that I needed, here comes the hard part

    auto n = std::thread::hardware_concurrency(); // n holds the number of
                                                  // concurrent nodes

    std::vector< std::future< double > > x_n(n);

    double range = (to-from)/n;

    auto root_finder = [&]( double start, double finish ){

        double x = finish - eps;

        double f, df, ddf;

        double temp;

        while( std::fabs(p(poly, x)) > eps && (x>=start && x<=finish)){

            f = p(poly, x);

            df = p(dpoly, x);

            ddf = p(ddpoly, x);

            temp = 2*df*df - f*ddf;

            x -= 2*f*df*(1/temp);

        }

        return x;

    };

    for( int k=0; k<n; k++ ){

        x_n[k] = std::async( std::launch::async, root_finder, from+k*range, to-(n-k-1)*range );

    }

    std::for_each(x_n.begin(),x_n.end(),[&]( std::future<double>& f ){ std::cout << f.get() << "\n"; });

    return 0;
}
