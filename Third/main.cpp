#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <iterator>

// This is a practice file based on one of my lectures.

int main(int argc, char* argv[])
{
    
    std::ifstream fin(argv[1],std::ios_base::in);
    
    std::vector<std::string> cont;

    // istreambuf_iterator -> gets the whitespaces as well!
    if(fin.is_open()){
        std::copy(std::istream_iterator<std::string>(fin),
                    std::istream_iterator<std::string>(),
                        std::back_inserter(cont));
    }else{
        std::cerr << "An error occured opening the file.\n";
    }

    std::ofstream fout("output.txt",std::ios_base::out);

    if(fout.is_open()){
        std::copy(cont.begin(),cont.end(),
                    std::ostream_iterator<std::string>(fout," "));
    }else{
        std::cerr << "An error occured opening the output file.\n";
    }

    // Clears the EOF bit
    fin.clear();
    // Set the file pointer to it's beginning position.
    fin.seekg(0);

    // Default constructor. 
    std::string test{std::istreambuf_iterator<char>(fin),
                    std::istreambuf_iterator<char>()};

    std::cerr << test << std::endl;

    fin.close();
    fout.close();

}
