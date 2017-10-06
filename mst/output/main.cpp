#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <numeric>
#include <algorithm>

int main( int argc, char* argv[] ){

    std::vector<int> data;

    std::ifstream fin(argv[1]);

    std::string line;

    while(std::getline(fin, line)){

        if(line[0] == '*'){
            break;
        }

        std::istringstream ss(line);

        int x;

        while( ss >> x ){ data.push_back(x); }

    }

    std::cout << "Mean:" << \
    (double)std::accumulate(std::begin(data), std::end(data),0.)/data.size() << "\n";
    auto result = std::max_element(data.begin(), data.end());
    std::cout << "Max:" << data[std::distance(data.begin(),result)] << "\n";

}