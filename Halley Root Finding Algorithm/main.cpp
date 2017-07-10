#include <iostream>
#include <iomanip>
#include <fstream>
#include <thread>
#include <cmath>
#include <algorithm>
#include <string>
#include <deque>
#include <iterator>
#include <future>

const double eps = 10e-11;
const double o_eps = 10e-6;

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

std::ostream& operator<<( std::ostream& os, const std::vector<double>& v ){

    for (auto i : v){

        os << i << " ";

    }

    return os;

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

    std::vector< std::future< std::vector<double> > > x_n(n);

    double range = (to-from)/n;

    auto root_finder = [&]( double start, double finish ){

        // try to launch it with several initial guesses and only return the
        // solution of different outcomes

        std::vector<double> initial_guesses((int)(finish-start)*10000);

        std::vector<double> roots;

        double step = (finish-start)/(double)initial_guesses.size();

        double a = start+eps;

        std::for_each(initial_guesses.begin(),initial_guesses.end(),[&](double& i)
                      {
                         i = a;
                         a += step;
                         //std::cout << i << " ";
                      });

        double f, df, ddf, temp;

        std::for_each(initial_guesses.begin(),initial_guesses.end(),
                      [&](double x){

        int iteration = 0;

        do{

            f = p(poly, x);

            df = p(dpoly, x);

            ddf = p(ddpoly, x);

            temp = 2*df*df - f*ddf;

            x -= 2*f*df*(1/temp);

            iteration++;

        }while( std::fabs(f) > eps && (x>=start && x<finish) && iteration < 20000);

        //std::cout << x << " ";

        if(iteration!=20000 && std::fabs(f) <= eps){

            if( std::find(roots.begin(),roots.end(),x) != roots.end() && roots.size() > 0){

                roots.push_back(x);

                //std::cout << "Found root.\n";

            }else{

                roots.push_back(x);

            }

        }

        });

        //std::cout << roots << "\n";

        std::sort( roots.begin(), roots.end() );
        roots.erase(std::unique( roots.begin(), roots.end(),[&](double& a, double& b)
                                { if( std::fabs(b-a) < o_eps){ return true; }else{ return false; } })
                    , roots.end() );

        return roots;

    };

    for( int k=0; k<n; k++ ){

        x_n[k] = std::async( std::launch::async, root_finder, from+k*range, to-(n-k-1)*range );

    }

    std::setprecision(4);

    std::for_each(x_n.begin(),x_n.end(),[&]( std::future< std::vector<double> >& f ){ std::cout << f.get() << "\n"; });

    return 0;
}
