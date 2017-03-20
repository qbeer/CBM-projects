#include "TH1.h"
#include "TCanvas.h"
#include "TTree.h"
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

void Analyze(string file)
{
	ifstream fin(file);
	Double_t numOfEvents = 0.0;
	Double_t energy = 0.0;
	Double_t transMom = 0.0;
	TTree* t = new TTree("part_tree","Particle tree");
	t->Branch("numOfEvents",&numOfEvents,"numOfEvents/D");
	t->Branch("energy",&energy,"energy/D");
	t->Branch("transMom",&transMom, "transMom/D");
	while(!fin.eof()){
		string a;
		getline(fin,a);
		vector<double> v;
		v.reserve(3);
		istringstream data(a);
		double x;
		while(data>>x) v.push_back(x);
		numOfEvents = v[0];
		energy = v[1];
		transMom = v[2];
		t->Fill();
	}
	fin.close();
	TH1D* h1 = new TH1D("h1","My 1st histo",1000,0,1000.);
	TH1D* h2 = new TH1D("h2","My 2nd histo",100,0.0,5.0);
	TH1D* h3 = new TH1D("h3","My 3rd histo",100,0.0,3.0); 
	for(int i=0;i<t->GetEntries();i++){
		t->GetEntry(i);
		h1->Fill(numOfEvents);
		h2->Fill(energy,numOfEvents);
		h3->Fill(transMom,numOfEvents);
	}
	TCanvas* c1 = new TCanvas("c1","My canvas",900,400);
	c1->Divide(3,1);
	c1->cd(1);
	h1->SetTitle("Number of events");
	h1->Draw();
	c1->cd(2);
	h2->SetTitle("Energy of events");
	h2->Draw();
	c1->cd(3);
	h3->SetTitle("Momenta of events");
	h3->Draw();
}
