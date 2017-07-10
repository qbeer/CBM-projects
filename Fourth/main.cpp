#include <chrono>
#include <iostream>
#include <regex>
#include <random>
#include <vector>
#include <algorithm>

// Again. This is a practice code based on my lecture in modern C++

int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();

    long long sum = 0;

    for(int i=0;i<1000;i++){
        sum += i;
    }

    std::cerr << std::endl << sum << std::endl;

    auto t2 = std::chrono::high_resolution_clock::now();

    long long duration = 
            std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count();

    std::cerr << duration << " micorseconds" << std::endl;

    std::cerr << std::endl;

    std::string text = "Alex loves Laura more than anyting";
    std::regex vowels("a|e|i|o|u");
    std::cerr << std::regex_replace(text, vowels," ") << std::endl;
    std::cerr << text << std::endl;
    std::cerr << std::regex_replace(text, vowels,"[$&]") << std::endl;

    std::cerr << std::endl;

    // Random generator, written by a physicist RULES.
    std::vector<double> data(10);

    std::random_device rnd_device;
    std::mt19937 mersenne_engine(rnd_device());
    std::normal_distribution<double> dist(1.0,0.5);

    std::generate(data.begin(),data.end(),[&]{return dist(mersenne_engine);});

    for(auto i : data) std::cerr << data[i] << " ";

    std::cerr << std::endl;

}
