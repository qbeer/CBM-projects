#include "TCanvas.h"
#include "TRandom3.h"
#include "TF2.h"
#include "TH2.h"
#include "TGraph2DErrors.h"
#include "TStyle.h"

void plot2d()
{

	gStyle->SetPalette(kBird); // for better colors

	const double eps = 0.3;

	const int dp = 500;

	TRandom3 gen;

	TF2 func("func","100*(sin(x)/([0]*x) + sin(y)/([1]*y)) + 300",-6.,6.,-6.,6.);

	func.SetParameters(0.19,0.3);

	TGraph2DErrors gr(dp); // graph with errors

	double temp, x, y, z, ex, ey, ez;

	for(Int_t i = 0; i<500; i++){

		func.GetRandom2(x,y);

		temp = gen.Uniform(-eps, eps);

		z = func.Eval(x,y)*(1+temp); // to modify the z point

		gr.SetPoint(i,x,y,z);

		ex = 0.05*gen.Uniform();

		ey = 0.05*gen.Uniform();

		ez = fabs(temp*z);

		gr.SetPointError(i,ex,ey,ez);

	}

	func.SetParameters(0.056, 0.23);

	func.SetTitle("Fitted 2D func");

	gr.Fit(&func);

	auto c1 = new TCanvas("c1","My canvas : c1", 800,600);

	c1->Divide(1,2);

	c1->cd(1);

	func.SetLineWidth(1);

	func.SetLineColor(kBlue);

	func.GetXaxis()->SetTitle("X axis");

	func.GetYaxis()->SetTitle("Y axis");

	func.GetZaxis()->SetTitle("Z axis");

	func.Draw("surf1");

	c1->cd(2);

	gr.Draw("surfA");

	c1->Print("surfplot.pdf");

	return;

}
