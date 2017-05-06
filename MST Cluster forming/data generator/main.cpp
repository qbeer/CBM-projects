#include <random>
#include <fstream>
#include <algorithm>

int main()
{

    std::random_device dev;
    std::mt19937 mersenne(dev());
    std::normal_distribution<double> pos1_dist(25.,0.4);

    std::normal_distribution<double> pos2_dist(354.,0.54);

    std::normal_distribution<double> mom_dist(120.,73.);

    std::ofstream fout("data.dat",std::ios_base::out);

    // generate 6000 lines

    std::vector<double> output(3);

    for(int i=0; i<2000; i++){

        // 2 line generated with pos1_dist
        // 1 line genrated with pos2_dist

        // all momenta generated with mom_dist

        std::generate(output.begin(),output.end(),
                    [&]{ return pos1_dist(mersenne); });
        
        std::for_each(output.begin(),output.end(),
                     [&](double& x){ fout << x << " "; });

        std::generate(output.begin(),output.end(),
                    [&]{ return mom_dist(mersenne); });
        
        std::for_each(output.begin(),output.end(),
                     [&](double& x){ fout << x << " "; });

        fout << "\n";

        std::generate(output.begin(),output.end(),
                    [&]{ return pos1_dist(mersenne); });
        
        std::for_each(output.begin(),output.end(),
                     [&](double& x){ fout << x << " "; });

        std::generate(output.begin(),output.end(),
                    [&]{ return mom_dist(mersenne); });
        
        std::for_each(output.begin(),output.end(),
                     [&](double& x){ fout << x << " "; });

        fout << "\n";

        std::generate(output.begin(),output.end(),
                    [&]{ return pos2_dist(mersenne); });
        
        std::for_each(output.begin(),output.end(),
                     [&](double& x){ fout << x << " "; });

        std::generate(output.begin(),output.end(),
                    [&]{ return mom_dist(mersenne); });
        
        std::for_each(output.begin(),output.end(),
                     [&](double& x){ fout << x << " "; });

        fout << "\n";

    }

    fout.close();

    return 0;

}
