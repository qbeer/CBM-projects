#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <tuple>
#include <numeric>
#include <string>
#include <iterator>
#include <map>
#include <cmath>

int main(int argc, char* argv[])
{
    if(argc<2){
        std::cout << "Too few arguments, you need to provide a file in the argument list." << std::endl;
    }
    
    std::ifstream fin(argv[1],std::ios_base::in);
    if(!fin.is_open()){
        std:: cout << "Error loading " << argv[1] << "\n";
        exit(1);
    }

    // get the data
    std::vector<double> data;

    std::copy(std::istream_iterator<double>(fin),
              std::istream_iterator<double>(),
              std::back_inserter(data));

    fin.clear();
    fin.seekg(0);

    std::string line;

    std::getline(fin,line);

    std::stringstream is(line);

    // find out how many columns there are
    int n_of_cols = 0;
    double n;
    while(is >> n){
        n_of_cols++;
    }

    int n_of_rows = data.size()/n_of_cols - 1;

    std::map<double, int> freq;
    std::for_each(data.begin(),data.end(),[&](double x){freq[x]++;});
    
    std::vector<double> pr;
    for(auto i: freq){
        pr.push_back((double)i.second/(n_of_rows*n_of_cols));
    }

    std::vector<double> cdf(freq.size());
    std::partial_sum(pr.begin(),pr.end(),cdf.begin(),std::plus<double>());

    std::for_each(cdf.begin(),cdf.end(),[=](double x){std::cout << x << "\n";});
    std::cout << "\n";

    auto min = *std::min_element(cdf.begin(),cdf.end());

    std::map<double, int> norm_freq;

    int j = 0;
    for(auto i : freq){
        norm_freq[i.first] = (int)std::floor((cdf[j]-min)*255);
        j++; 
    }

    for(auto i : norm_freq){
        std::cout << i.first << " : " << i.second << "\n";
    }

    std::for_each(data.begin(),data.end(),[&](double x){ x = norm_freq[x];});

    std::ofstream fout("normalized.txt");

    int counter = 0;
    for(auto i : data){
        fout << (int)i << " ";
        counter++;
        if(counter==n_of_cols && !fout.eof()){
            fout << "\n";
            counter = 0;
        }
    }
}
