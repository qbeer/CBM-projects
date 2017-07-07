#include <iostream>
#include "TROOT.h"
#include "TH1.h"
#include "TCanvas.h"
#include <random>
#include <chrono>
#include "TDirectory.h"
#include "TFile.h"
#include "TBrowser.h"

void fileSys()
{

	TFile* f = new TFile("test1.root", "recreate");

	TDirectory* d1 = gFile->mkdir("d1","Directory 1");

	TDirectory* d1_1 = d1->mkdir("d1_1","Subdirectory 1 of d1");

	TDirectory* d1_2 = d1->mkdir("d1_2","Subdirectory 2 of d1");

	d1_2->cd();

	TCanvas* c1 = new TCanvas("c1","My canvas : c1", 600, 400);

	// now we are in subdirectory d1_2, here comes the histogram

	TH1D* hist1 = new TH1D("hist1","First histo",10000,10.,70.);

	std::random_device rd;

	std::mt19937 gen(rd());

	std::uniform_real_distribution<double> dist(42,3);

	for(int i=0;i<1000;i++){

		hist1->Fill( dist(gen) );

		hist1->Draw();

		c1->Update();

	}


	d1->Write();

	d1_1->Write();

	d1_2->Write();

	hist1->Write();

	c1->Write();

	new TBrowser;

	f->Close();

	return;

}
