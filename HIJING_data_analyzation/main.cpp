#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

inline double SQR(double x){return x*x;}

int main()
{

    /*
    pi_pos = 211,
    pi_nul = 111,
    kaon_pos = 321,
    kaon_nul = 311,
    proton = 2212,
    neutron = 2112,
    */

    int pi_pos_cnt = 0;
    double m_pi_nul = 0.0;
    double pt_pi_nul = 0.0;
    int pi_nul_cnt = 0;
    double m_pi_pos = 0.0;
    double pt_pi_pos = 0.0;
    int kaon_pos_cnt = 0;
    double m_kaon_pos = 0.0;
    double pt_kaon_pos = 0.0;
    int proton_cnt = 0;
    double m_proton = 0.0;
    double pt_proton = 0.0;
    int kaon_nul_cnt = 0;
    double m_kaon_nul = 0.0;
    double pt_kaon_nul = 0.0;
    int neutron_cnt = 0;
    double m_neutron = 0.0;
    double pt_neutron = 0.0;

    int numOfEvents = 0;

    // IF A STRINGSTREAM GETS CHAR VALUE IT STOPS WORKING WITHOUT ERROR and does (!) keep on going
    ifstream fin("AuAu.txt", ios_base::in);
    ofstream fout("Results.txt",ios_base::app);
    fout << "pi0_count\tpi+_count\tkaon0_count\tkaon+_count\tproton_count\tneutron_count\n";
    fout << "pi0_mass\tpi+_mass\tkaon0_mass\tkaon+_mass\tproton_mass\tneutron_mass\n";
    fout << "pi0_pt\tpi+_pt\tkaon0_pt\tkaon+_pt\tproton_pt\tneutron_pt\n";
    while(!fin.eof()){
        string a;
        vector<double> v;
        v.reserve(8);
        getline(fin,a);
        istringstream nums(a);
        double x;
        while(nums>>x) v.push_back(x);
        if(v.size()==0){
            numOfEvents++;
            fout << pi_nul_cnt << " ";
            fout << pi_pos_cnt << " ";
            fout << kaon_nul_cnt << " ";
            fout << kaon_pos_cnt << " ";
            fout << proton_cnt << " ";
            fout << neutron_cnt << "\n";
            fout << m_pi_nul/(double)pi_nul_cnt << " ";
            fout << m_pi_pos/(double)pi_pos_cnt << " ";
            fout << 0.0 << " ";
            fout << m_kaon_pos/(double)kaon_pos_cnt << " ";
            fout << m_proton/(double)proton_cnt << " ";
            fout << m_neutron/(double)neutron_cnt << "\n";
            fout << pt_pi_nul/(double)pi_nul_cnt << " ";
            fout << pt_pi_pos/(double)pi_pos_cnt << " ";
            fout << 0.0 << " ";
            fout << pt_kaon_pos/(double)kaon_pos_cnt << " ";
            fout << pt_proton/(double)proton_cnt << " ";
            fout << pt_neutron/(double)neutron_cnt << "\n";
            pi_nul_cnt = 0;
            m_pi_nul = 0.0;
            pi_pos_cnt = 0;
            m_pi_pos = 0.0;
            proton_cnt = 0;
            m_proton = 0.0;
            kaon_nul_cnt = 0;
            m_kaon_nul = 0.0;
            kaon_pos_cnt = 0;
            m_kaon_pos = 0.0;
            neutron_cnt = 0;
            m_neutron = 0.0;
            pt_kaon_nul = 0.0;
            pt_kaon_pos = 0.0;
            pt_pi_nul = 0.0;
            pt_pi_pos = 0.0;
            pt_proton = 0.0;
            pt_neutron = 0.0;
        }else if(v.size()==8){
            switch((int)v[1]){
                case 211:
                    pi_pos_cnt++;
                    m_pi_pos += sqrt(SQR(v[7])-SQR(v[6])-SQR(v[5])-SQR(v[4]));
                    pt_pi_pos += sqrt(SQR(v[4])+SQR(v[5]));
                    break;
                case 111:
                    pi_nul_cnt++;
                    m_pi_nul += sqrt(SQR(v[7])-SQR(v[6])-SQR(v[5])-SQR(v[4]));
                    pt_pi_nul += sqrt(SQR(v[4])+SQR(v[5]));
                    break;
                case 321:
                    kaon_pos_cnt++;
                    m_kaon_pos += sqrt(SQR(v[7])-SQR(v[6])-SQR(v[5])-SQR(v[4]));
                    pt_kaon_pos += sqrt(SQR(v[4])+SQR(v[5]));
                    break;
                case 311:
                    kaon_nul_cnt++;
                    m_kaon_nul += sqrt(SQR(v[7])-SQR(v[6])-SQR(v[5])-SQR(v[4]));
                    pt_kaon_nul += sqrt(SQR(v[4])+SQR(v[5]));
                    break;
                case 2212:
                    proton_cnt++;
                    m_proton += sqrt(SQR(v[7])-SQR(v[6])-SQR(v[5])-SQR(v[4]));
                    pt_proton += sqrt(SQR(v[4])+SQR(v[5]));
                    break;
                case 2112:
                    neutron_cnt++;
                    m_neutron += sqrt(SQR(v[7])-SQR(v[6])-SQR(v[5])-SQR(v[4]));
                    pt_neutron += sqrt(SQR(v[4])+SQR(v[5]));
                    break;
                default:
                    break;
            }
        }
    }

    fin.close();
    fout << "Total number of events: " << numOfEvents;
    fout.close();


    fin.open("Results.txt");

    ofstream pi_nul("pi_nul.txt");
    ofstream pi_pos("pi_pos.txt");
    ofstream kaon_nul("kaon_nul.txt");
    ofstream kaon_pos("kaon_pos.txt");
    ofstream proton("proton.txt");
    ofstream neutron("neutron.txt");

    while(!fin.eof()){
        string a;
        vector<double> part,Enry,mom;
        part.reserve(6);
        Enry.reserve(6);
        mom.reserve(6);
        getline(fin,a);
        istringstream nums(a);
        double x;
        while(nums>>x) part.push_back(x);
        nums.clear();
        getline(fin,a);
        nums.str(a);
        while(nums>>x) Enry.push_back(x);
        nums.clear();
        getline(fin,a);
        nums.str(a);
        while(nums>>x) mom.push_back(x);
        // one event takes up 3 rows
        if(part.size()==6 && Enry.size()==6 && mom.size()==6){
        pi_nul << part[0] << " " << Enry[0] << " " << mom[0] << "\n";
        pi_pos << part[1] << " " << Enry[1] << " " << mom[1] << "\n";
        kaon_nul << part[2] << " " << Enry[2] << " " << mom[2] << "\n";
        kaon_pos << part[3] << " " << Enry[3] << " " << mom[3] << "\n";
        proton << part[4] << " " << Enry[4] << " " << mom[4] << "\n";
        neutron << part[5] << " " << Enry[5] << " " << mom[5] << "\n";
        }
    }

    return 0;
}
