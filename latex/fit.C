void fit(){
    
    TFile* srcFile = TFile::Open("KFParticleFinder_phsdwocsr_auau_10gev_centr_sis100_electron_5M_ToF.root");
    TDirectory* phi = (TDirectory*)srcFile->Get("KFTopoReconstructor/KFParticlesFinder/Particles/phi_{KK}/Parameters");
    TDirectory* phi_signal = (TDirectory*)srcFile->Get("KFTopoReconstructor/KFParticlesFinder/Particles/phi_{KK}/Parameters/Signal");
    
    TH1F* M = (TH1F*)phi->Get("M");
    TH1F* Msignal = (TH1F*)phi_signal->Get("M");
    Msignal->SetName("Msignal");
    
    TFile* myFile = new TFile("phi_fit.root","recreate");
    TH1F* myHisto = (TH1F*)M->Clone();
    myHisto->SetName("myHisto");
    TH1F* myBackground = (TH1F*)M->Clone();
    myBackground->SetName("myBackground");
    TH1F* mySignal = (TH1F*)Msignal->Clone();
    mySignal->SetName("mySignal");
    myFile->cd();
    
    srcFile->Close();
    
    TCanvas* canv = new TCanvas("canv","Total fit",640,480);
    
    TF1* background = new TF1("background","pol2",1.004,1.1);
    TF1* signal = new TF1("signal","gaus",1.011,1.033);
    
    TF1* total = new TF1("total","gaus(0) + pol2(3)",1.005,1.1);
    
    background->SetParameters(0.2,1.01,108000.);
    background->SetParNames("landau_1","landau_2","landau_3");
    myBackground->Fit("background","R+");
    
    Double_t params[6];
    background->GetParameters(&params[3]);
    
    signal->SetParameters(3200., 1.021, 3000.);
    signal->SetParNames("scale","mean","sigma");
    mySignal->Fit("signal","R+");
    signal->GetParameters(&params[0]);
    
    params[0]*=250.; // rescale
    total->SetParameters(params);
    total->SetParNames("scale","mean","sigma",
		       "la","b","c");
    myHisto->Draw();
    total->Draw("same");
    
    signal->Write();
    background->Write();
    total->Write();
    myHisto->Write();
    canv->Write();
    
    myFile->Close();
    
}