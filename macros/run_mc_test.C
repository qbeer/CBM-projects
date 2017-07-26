// --------------------------------------------------------------------------
//
// Macro for standard transport simulation using UrQMD input and GEANT3
//
// V. Friese   22/02/2007
//
// Version 2016-02-05
//
// For the setup (geometry and field), predefined setups can be chosen
// by the second argument. A list of available setups is given below.
// The input file can be defined explicitly in this macro or by the
// third argument. If none of these options are chosen, a default
// input file distributed with the source code is selected.
//
// --------------------------------------------------------------------------


void run_mc_test(TString inFile="/d/cbm05/vassiliev/urqmd.auau.10gev.centr.00001.root", const char* setupName = "sis100_electron", Int_t nEvents = 1000)
{

  // ========================================================================
  //          Adjust this part according to your requirements

  // -----   Environment   --------------------------------------------------
  TString myName = "run_mc";  // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------


  // -----   In- and output file names   ------------------------------------   
  TString outFile = "sim.root";
  TString parFile = "param.root";
  TString geoFile = "geofile.root";
  // --- Logger settings ----------------------------------------------------
  TString logLevel     = "INFO";
  TString logVerbosity = "LOW";
  // ------------------------------------------------------------------------


  // --- Define the target geometry -----------------------------------------
  //
  // The target is not part of the setup, since one and the same setup can
  // and will be used with different targets.
  // The target is constructed as a tube in z direction with the specified
  // diameter (in x and y) and thickness (in z). It will be placed at the
  // specified position as daughter volume of the volume present there. It is
  // in the responsibility of the user that no overlaps or extrusions are
  // created by the placement of the target.
  //
  TString  targetElement   = "Gold";
  Double_t targetThickness = 0.025;  // full thickness in cm
  Double_t targetDiameter  = 2.5;    // diameter in cm
  Double_t targetPosX      = 0.;     // target x position in global c.s. [cm]
  Double_t targetPosY      = 0.;     // target y position in global c.s. [cm]
  Double_t targetPosZ      = 0.;     // target z position in global c.s. [cm]
  Double_t targetRotY      = 0.;     // target rotation angle around the y axis [deg]
  // ------------------------------------------------------------------------


  // --- Define the creation of the primary vertex   ------------------------
  //
  // By default, the primary vertex point is sampled from a Gaussian
  // distribution in both x and y with the specified beam profile width,
  // and from a flat distribution in z over the extension of the target.
  // By setting the respective flags to kFALSE, the primary vertex will always
  // at the (0., 0.) in x and y and in the z centre of the target, respectively.
  //
//   Bool_t smearVertexXY = kTRUE;
//   Bool_t smearVertexZ  = kTRUE;
//   Double_t beamWidthX   = 1.;  // Gaussian sigma of the beam profile in x [cm]
//   Double_t beamWidthY   = 1.;  // Gaussian sigma of the beam profile in y [cm]
  Bool_t smearVertexXY = 0;
  Bool_t smearVertexZ  = 0;
  Double_t beamWidthX   = 0.;  // Gaussian sigma of the beam profile in x [cm]
  Double_t beamWidthY   = 0.;  // Gaussian sigma of the beam profile in y [cm]
  // ------------------------------------------------------------------------
  

  // In general, the following parts need not be touched
  // ========================================================================


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // ----    Debug option   -------------------------------------------------
  gDebug = 0;
  // ------------------------------------------------------------------------

  
  // -----   Remove old CTest runtime dependency file   ---------------------
//  TString depFile = Remove_CTest_Dependency_File(outDir, "run_mc" , setupName);
  // ------------------------------------------------------------------------



  // -----   Create simulation run   ----------------------------------------
  FairRunSim* run = new FairRunSim();
  run->SetName("TGeant3");              // Transport engine
  run->SetOutputFile(outFile);          // Output file
//  run->SetGenerateRunInfo(kTRUE);       // Create FairRunInfo file
  // ------------------------------------------------------------------------


  // -----   Logger settings   ----------------------------------------------
//  FairLogger::GetLogger()->SetLogScreenLevel(logLevel.Data());
//  FairLogger::GetLogger()->SetLogVerbosityLevel(logVerbosity.Data());
  // ------------------------------------------------------------------------


  // -----   Load the geometry setup   -------------------------------------
  std::cout << std::endl;
  TString setupFile = srcDir + "/geometry/setup/setup_" + setupName + ".C";
  TString setupFunct = "setup_";
  setupFunct = setupFunct + setupName + "()";
  std::cout << "-I- " << myName << ": Loading macro " << setupFile << std::endl;
  gROOT->LoadMacro(setupFile);
  gROOT->ProcessLine(setupFunct);
  // You can modify the pre-defined setup by using
  // CbmSetup::Instance()->RemoveModule(ESystemId) or
  // CbmSetup::Instance()->SetModule(ESystemId, const char*, Bool_t) or
  // CbmSetup::Instance()->SetActive(ESystemId, Bool_t)
  // See the class documentation of CbmSetup.
  // ------------------------------------------------------------------------

  // -----   Create media   -------------------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Setting media file" << std::endl;
  run->SetMaterials("media.geo");       // Materials
  // ------------------------------------------------------------------------


  // -----   Create and register modules   ----------------------------------
  std::cout << std::endl;
  TString macroName = gSystem->Getenv("VMCWORKDIR");
  macroName += "/macro/run/modules/registerSetup.C";
  std::cout << "Loading macro " << macroName << std::endl;
  gROOT->LoadMacro(macroName);
  gROOT->ProcessLine("registerSetup()");
  // ------------------------------------------------------------------------


  // -----   Create and register the target   -------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Registering target" << std::endl;
  CbmTarget* target = new CbmTarget(targetElement.Data(),
  		                              targetThickness,
  		                              targetDiameter);
  target->SetPosition(targetPosX, targetPosY, targetPosZ);
  target->SetRotation(targetRotY);
  target->Print();
  run->AddModule(target);
  // ------------------------------------------------------------------------


  // -----   Create magnetic field   ----------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Registering magnetic field" << std::endl;
  CbmFieldMap* magField = CbmSetup::Instance()->CreateFieldMap();
  if ( ! magField ) {
  	std::cout << "-E- run_sim_new: No valid field!";
  	return;
  }
  run->SetField(magField);
  // ------------------------------------------------------------------------


  // -----   Create PrimaryGenerator   --------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Registering event generators" << std::endl;
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  // --- Uniform distribution of event plane angle
  primGen->SetEventPlane(0., 2. * TMath::Pi());
  // --- Get target parameters
  Double_t tX = 0.;
  Double_t tY = 0.;
  Double_t tZ = 0.;
  Double_t tDz = 0.;
//  if ( target ) {
//  	target->GetPosition(tX, tY, tZ);
//  	tDz = target->GetThickness();
//  }
  primGen->SetTarget(tZ, tDz);
  primGen->SetBeam(0., 0., beamWidthX, beamWidthY);
//  primGen->SmearGausVertexXY(smearVertexXY);
//  primGen->SmearVertexZ(smearVertexZ);
  
  //
  // TODO: Currently, there is no guaranteed consistency of the beam profile
  // and the transversal target dimension, i.e., that the sampled primary
  // vertex falls into the target volume. This would require changes
  // in the FairPrimaryGenerator class.
  // ------------------------------------------------------------------------

  // Use the CbmUnigenGenrator for the input
  CbmUnigenGenerator*  uniGen = new CbmUnigenGenerator(inFile);
  primGen->AddGenerator(uniGen);
  run->SetGenerator(primGen);
    // -----   Run initialisation   -------------------------------------------
  // -Trajectories Visualization (TGeoManager Only )
   run->SetStoreTraj(kTRUE);  //->
 // -----------------------------------------------
 
  KFPartEfficiencies eff;
  for(int jParticle=eff.fFirstStableParticleIndex+10; jParticle<=eff.fLastStableParticleIndex; jParticle++)
  {
    TDatabasePDG* pdgDB = TDatabasePDG::Instance();

    if(!pdgDB->GetParticle(eff.partPDG[jParticle])){
        pdgDB->AddParticle(eff.partTitle[jParticle].data(),eff.partTitle[jParticle].data(), eff.partMass[jParticle], kTRUE,
                           0, eff.partCharge[jParticle]*3,"Ion",eff.partPDG[jParticle]);
    }
  }
  
  const int NIons=16;
  int pdgIon[NIons] = {1000030060, 1000040070, 1000040080, 1000020060, 1000030070, 1000030080, 1000030050, 1000050100, 1000020050, 1000060100, 1000050090, 1000040090, 1010010020, 1000040100, 1000020080, 1000030090};
  TString nameIon[NIons] = {"li6", "be7", "be8", "he6", "li7", "li8", "li5", "b10", "he5", "c10", "b9", "be9", "c12", "be10", "he8", "li9"};
  double massIon[NIons] = {6., 7., 8., 6., 7., 8., 5., 10., 5., 10., 9., 9., 2., 10., 8., 9};
  int chargeIon[NIons] = {3, 4, 4, 2, 3, 3, 3, 5, 2, 6, 5, 4, 1, 4, 2, 3};
  double TimeIon[NIons] = {1.0e20, 1.0e20,6.7e-17, 8.067e-5,1.0e20 , 0.838, 3.7e-26, 1.0e20, 7.e-26, 19.3, 8.e-23, 1.0e20, 1.0e20, 1.0e20, 0.119, 0.178};
  for(int iIon=0; iIon<NIons; iIon++)
  {
    TDatabasePDG* pdgDB = TDatabasePDG::Instance();

    if(!pdgDB->GetParticle(pdgIon[iIon])){
        pdgDB->AddParticle(nameIon[iIon].Data(), nameIon[iIon].Data(), massIon[iIon], kTRUE,
                           0, chargeIon[iIon]*3,"Ion",pdgIon[iIon]);
    }
  }
 
//   gMC->DefineParticle(1000060030, "li6", kPTHadron, 6.0, 3,
//                       1.0e20, "hadron", 0.0, 1, 1, 0, 1, 1, 0, 0, 1, kFALSE);
   
   // -----   Run initialisation   -------------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Initialise run" << std::endl;
  run->Init();
 /*    // Set cuts for storing the trajectpries
  FairTrajFilter* trajFilter = FairTrajFilter::Instance();  //->
  trajFilter->SetStepSizeCut(0.01);   //->// 1 cm
 // trajFilter->SetVertexCut(-200., -200., 4., 2000., 2000., 100.);
//  trajFilter->SetMomentumCutP(10e-2); // p_lab > 100 MeV
 // trajFilter->SetEnergyCut(0., 1.02); // 0 < Etot < 1.04 GeV
  trajFilter->SetStorePrimaries(kTRUE);  //->
  trajFilter->SetStoreSecondaries(kTRUE);  //->

 */ 
  
 // -----   Runtime database   ---------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Set runtime DB" << std::endl;
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  CbmFieldPar* fieldPar = (CbmFieldPar*) rtdb->getContainer("CbmFieldPar");
  fieldPar->SetParameters(magField);
  fieldPar->setChanged();
  fieldPar->setInputVersion(run->GetRunId(),1);
  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open(parFile.Data());
  rtdb->setOutput(parOut);
  rtdb->saveOutput();
  rtdb->print();
  // ------------------------------------------------------------------------
  for(int iIon=0; iIon<NIons; iIon++)
    {
      TVirtualMC::GetMC()->DefineParticle(pdgIon[iIon], nameIon[iIon], kPTHadron, massIon[iIon], chargeIon[iIon],
                       TimeIon[iIon], "hadron", 0.0, 1, 1, 0, 1, 1, 0, 0, 1, kFALSE); 

    }


  // -----   Start run   ----------------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Starting run" << std::endl;
  run->Run(nEvents);
  // ------------------------------------------------------------------------


  // -----   Finish   -------------------------------------------------------
  run->CreateGeometryFile(geoFile);
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Output file is "    << outFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Geometry file is "  << geoFile << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime
            << "s" << std::endl << std::endl;
  // ------------------------------------------------------------------------
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;

  // Function needed for CTest runtime dependency
//  Generate_CTest_Dependency_File(depFile);
//  RemoveGeoManager();
  // ------------------------------------------------------------------------

}

