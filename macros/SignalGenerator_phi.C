//---------------------------------------------------------------------------------
// @author  M. Zyzak, Iouri
// @version 1.0
// @since   15.08.14
// 
// macro to generate signal events for KFParticleFinder
//_________________________________________________________________________________

void SignalGenerator_phi( int NEvent = 10, Int_t seed = 555 )
{
  /*
  gROOT->LoadMacro("$VMCWORKDIR/gconfig/basiclibs.C");
  basiclibs();
  gSystem->Load("libGeoBase");
  gSystem->Load("libParBase");
  gSystem->Load("libBase");
  gSystem->Load("libCbmBase");
  gSystem->Load("libCbmData");
  gSystem->Load("libField");
  gSystem->Load("libGen");
  gSystem->Load("libPassive");
  gSystem->Load("libMvd");
  gSystem->Load("libSts");
  gSystem->Load("libTrd");
  gSystem->Load("libTof");
  gSystem->Load("libEcal");
  gSystem->Load("libGlobal");
  gSystem->Load("libKF");
  gSystem->Load("libL1");
  gSystem->Load("libMinuit2"); // Needs for rich ellipse fitter
 */ 
  TString signalFile = "Signal_phi_10";
  signalFile += ".txt";
 /* TString script = TString(gSystem->Getenv("LIT_SCRIPT"));
  if (script == "yes") {
    signalFile = TString(gSystem->Getenv("LIT_SIGNAL_FILE"));
  }
  */
  gRandom->SetSeed(seed);

  const double kProtonMass = 0.938272321;    // Proton mass in GeV
  KFPartEfficiencies eff;

  const double kTwoPi      = 2.*TMath::Pi(); // 2*pi
  double fSlope = .154;
  double fRapSigma     = 0.5;
  double eBeam = 10.;
  double pBeam = TMath::Sqrt(eBeam*eBeam - kProtonMass*kProtonMass);
  double fYcm = 0.25*TMath::Log( (eBeam+pBeam) / (eBeam-pBeam) );
  
  const int NParticlesPerEvent = 1;
  const double kSignalMass[NParticlesPerEvent] = {1.019455};    //  mass in GeV
  const int    kSignalID[NParticlesPerEvent] =  {333};  

  TF1 fThermal[1];
  for(int i=0; i<1; i++)
  {
    fThermal[i] = TF1("thermal", "x*exp(-1.*sqrt(x*x+[1]*[1]) / [0])", 0., 10.);
    fThermal[i].SetParameter(0, fSlope);
    fThermal[i].SetParameter(1, kSignalMass[i]);
  }
  
  // Open output file
  ofstream outputfile;
  outputfile.open(signalFile.Data());
  if ( ! outputfile.is_open() ) {
    cout << "-E Signal (generator): Cannot open output file " << endl;
    return;
  }
    
  for (int i=0; i<NEvent; i++){
  // Generate rapidity, pt and azimuth
    outputfile<<NParticlesPerEvent<<"   "<<i + 1<<"  "<<0.<<"  "<<0.<<"  "<<0.<<endl;
    for(int j=0;j<NParticlesPerEvent;++j) {      
      double yD   = gRandom->Gaus(fYcm, fRapSigma);
      double ptD  = fThermal[j].GetRandom();
      double phiD = gRandom->Uniform(0., kTwoPi);
        
      // Calculate momentum, energy, beta and gamma
      double pxD    = ptD * TMath::Cos(phiD);
      double pyD    = ptD * TMath::Sin(phiD);
      double mtD    = TMath::Sqrt(kSignalMass[j]*kSignalMass[j] + ptD*ptD);
      double pzD    = mtD * TMath::SinH(yD);

      outputfile<<kSignalID[j]<<"  "<<pxD<<"  "<<pyD<<"  "<<pzD<<endl;
        
    }
  }
  
  outputfile.close();
  
}
 