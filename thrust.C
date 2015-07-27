
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <TH1F.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TFile.h>
#include <TTree.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TFile.h>
#include <TStyle.h>
#include <TStopwatch.h>
#include <TRandom3.h>
#include <TChain.h>
#include <TProfile.h>
#include <TStopwatch.h>
#include <TCut.h>
#include <cstdlib>
#include <cmath>
#include "TLegend.h"
#include "TLatex.h"
#include "TMath.h"
#include "TLine.h"
#include "TVector3.h"
#include <TROOT.h>
#include <string>
#include <sstream>
//TODO 
/*
  IMPLEMENT goodJet loop
  IMPLEMENT FILE LOOP
  IMPLEMENT pthatweights
  IMPLEMENT TTREES


*/

TVector3 Norm(TVector3 v){
  if ( v(0) == 0 && v(1) == 0 && v(2) == 0) return v; 
  Double_t mag = TMath::Sqrt(v(0)*v(0) + v(1)*v(1) + v(2)*v(2)); 
  return TVector3(v(0)/mag,v(1)/mag,v(2)/mag);
}

void thrust(const int startfile=0,int endfile=-1,int radius=3,
	    bool DEBUG_SHORT=false,
	    bool DEBUG=false
	    ){



  TH1::SetDefaultSumw2();


  Float_t ptCut=30;


  Double_t px[1000];  Float_t jtpt[1000]; 
  Double_t py[1000];  Float_t eta[1000];
  Double_t pz[1000];  Float_t phi[1000];
  TVector3  p[1000];
  TVector3 nT[1000];

  Float_t numerator;
  Float_t denominator;
  Float_t maxNumerator;
  int nref;
  int pcollisionEventSelection;
  Double_t pthatweight;
  Float_t varpthat;
  float vz;
  int evt;
  int run;
  int lumi;
  //SET UP FILE LIST==========================================
  std::string infile;
  infile = "jetRAA_pp_mc_forest.txt";
  cout<<"\n\nReading from: "+infile<<endl;
  std::ifstream instr(infile.c_str(), std::ifstream::in);

  if(endfile==-1){
    std::ifstream fi(infile.c_str());
    std::string line;
    int number_of_lines=0;
    while (std::getline(fi, line)) ++number_of_lines;
    endfile+=number_of_lines+1;
    cout<<"You did not specify number of files will run on all: "<<endfile<<" lines\n";
  }  // If not starting from file 0 discard earlier lines
  std::string filename;
  cout<<"reading from "<<startfile<<" to "<<endfile<<endl;  
  for(int ifile=0;ifile<startfile;ifile++){ 
    instr >> filename;
  }
  cout<<"Running on "<<endfile-startfile<<" forest files"<<endl;
  std::string outname="";
  if(DEBUG_SHORT) outname=Form("ROOT/thrust_DEBUG_R%d_%d.root",radius,endfile);
  else outname=Form("ROOT/thrust_weight_R%d_%d.root",radius,endfile);
  cout<<"Output: "<<outname<<endl;
  TFile fOUT(outname.c_str(),"RECREATE");
  fOUT.cd();

  //==========+++DEFINE HISTOGRAMS============================
  TH1F * hTHRUST = new TH1F("THRUST","",50,0,1);
  


  stringstream weightName;//stringstream allows easy concat of str and int
  for(int ifile=startfile;ifile<endfile;ifile++){
    instr >> filename;
    weightName.str(""); weightName<<"weights/weights_pp_"<<ifile+1<<".root";
 
    cout<<"ifile: "<<ifile<<"\nFile: "<<filename<<endl<<"weightFile: "<<weightName.str()<<"\n\n";
    //OPEN INPUT FILES======================================
    TFile *fin = TFile::Open(filename.c_str());//data file
    if(DEBUG) cout<<"OPENED FILE\n";
    TFile *wFile= TFile::Open(weightName.str().c_str());//corresponding pthat weight
    //GET TREES==================================================
    TTree* jetTree = (TTree*)fin->Get(Form("ak%dPFJetAnalyzer/t",radius));
    TTree* skimTree_in = (TTree*)fin->Get("skimanalysis/HltTree");
    TTree* evtTree_in = (TTree*)fin->Get("hiEvtAnalyzer/HiTree");
    TTree* weights = (TTree*)wFile->Get("weights");

    //ADD FRIENDS================================================
    jetTree->AddFriend(weights);
    //  jetTree->AddFriend(evtTree_in);
    jetTree->AddFriend(skimTree_in);
    
    //SET BRANCH ADDRESSES=======================================
    evtTree_in->SetBranchAddress("evt",&evt);
    evtTree_in->SetBranchAddress("run",&run);
    evtTree_in->SetBranchAddress("lumi",&lumi);
    evtTree_in->SetBranchAddress("vz",&vz);
    
    jetTree->SetBranchAddress("nref",&nref);
    jetTree->SetBranchAddress("jtpt",&jtpt);
    jetTree->SetBranchAddress("pthat",&varpthat);
    jetTree->SetBranchAddress("jteta",&eta);
    
    skimTree_in->SetBranchAddress("pPAcollisionEventSelectionPA",&pcollisionEventSelection);
 
    weights->SetBranchAddress("pthatweight",&pthatweight);

    //LOOP OVER EVENTS============================================

    Long64_t nentries = jetTree->GetEntries();
    if(DEBUG) cout<<"nentries: "<<nentries<<endl;
    if(DEBUG_SHORT)nentries = 100;

    for(int ie=0;ie<nentries;++ie){//EVENT LOOP
      //GEt ENTRIES HERE
      //
      //
      //=================
      if(ie%1000==0) cout<<"Event: "<<ie<<endl;
      if(DEBUG) cout<<"START OF EVENT LOOP"<<endl;
      jetTree->GetEntry(ie);
      weights->GetEntry(ie);
      if(DEBUG) cout<<"GOT ENTRIES\nnref: "<<nref<<endl;

      //============goodJet Loop
      std::vector <float> goodJet;
      for(int i=0;i<nref;++i){
	if(jtpt[i]>=ptCut && eta[i]<2.0){
	  goodJet.push_back(jtpt[i]);
	}
      }
      if(DEBUG) cout<<"goodJet size: "<<goodJet.size()<<endl;
      if(goodJet.size()==0){
	if(DEBUG)  cout<<"skipping due to goodJet size\n";
	continue;
      }
      //=============THRUST AXIS - assume on jet axis
      denominator=0;
      for(int i=0;i<goodJet.size();++i){
      
	px[i] = goodJet[i]*TMath::Cos(phi[i]);
	py[i] = goodJet[i]*TMath::Sin(phi[i]);
	pz[i] = goodJet[i]*TMath::SinH(eta[i]);
	p[i] = TVector3(px[i],py[i],pz[i]);
	nT[i] = Norm(TVector3(px[i],py[i],pz[i]));  
	denominator+=p[i].Mag();
	if(DEBUG) cout<<"Magnitude: "<<p[i].Mag()<<endl;
	if(DEBUG) cout<<"nT Mag: "<<nT[i].Mag()<<endl;
      }
      maxNumerator=0;
      for(int i=0;i<goodJet.size();++i){
	numerator=0;
	if(DEBUG) cout<<"OUTLOOP==========================: "<<i<<endl;
	for(int il=0;il<goodJet.size();++il){//il for inner loop
	  numerator += nT[i].Dot(p[il]);
        }
	if(ie==55){
	  p[i].Print("base");
	  nT[i].Print("base");
	  cout<<"Numerator: "<<numerator<<endl;
	  cout<<"Max Numerator: "<<maxNumerator<<endl;
	}
	if(numerator>maxNumerator){
	  maxNumerator=numerator;
	}
      }
      Float_t T =maxNumerator/denominator;  
      if(DEBUG){
	cout<<"maxNumerator: "<<maxNumerator<<endl;
	cout<<"denominator: "<<denominator<<endl;
	cout<<"Thrust: "<<T<<endl;
      }
      if(T<.5){
	cout<<"event: "<<ie<<endl;
	cout<<"nref: "<<nref<<endl;
	cout<<"goodJet Size: "<<goodJet.size()<<endl<<endl;
	/*	for(int i=0;i<goodJet.size();++i){
	  
	  for(int il=0;il<goodJet.size();++i){
	    cout<<"Magnitude: "<<p[il].Mag()<<endl;
	    cout<<"Numerator: "<< nT[i].Dot(p[il])<<endl<<endl;
	  }

	  }*/
	cout<<"maxNumerator: "<<maxNumerator<<endl;
	cout<<"denominator: "<<denominator<<endl;
	cout<<"Thrust: "<<T<<endl;
      }
      hTHRUST->Fill(T,pthatweight);


      //THRUST MINOR
      /*
//	Thoughts on best way to find an orthogonal vecotor. Based on this: http://math.stackexchange.com/questions/137362/how-to-find-perpendicular-vector-to-another-vector
//	get two vectors each orthogonal to original vector, each will have one component =0. then linear combos of these vectors as new thrust minor axes. I think you can assume that thrust will vary in a continuous fashion as we rotate the vector so quesiton becomes how to find maximum,optimization problem!!. Once this is figured out it shouldn't be too hard to apply to 3D case.

//	I think that coordinate cycling is a good solution here. see: 
//	http://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=3&ved=0CDAQFjACahUKEwilyPfVzfXGAhUL0IAKHdxIDF4&url=http%3A%2F%2Fmathforcollege.com%2Fnm%2Fmws%2Fgen%2F09opt%2Fmws_gen_opt_txt_multidirect.doc&ei=RSazVeWRLouggwTckbHwBQ&usg=AFQjCNEVHne0jfLEnbKvcuFuITc_BPjufw&sig2=7JDSCN8a-jh2tSk1eOjQqQ&bvm=bv.98717601,d.eXY
//	i found above as third link entitled: Multidimensional Direct Search Method - Math For College when i googled: two dimensional golder section search

      */
    
      for(int i=0;i<1000;i++){//CLEARING LOOP
	px[i]=0; py[i]=0; pz[i]=0;
	jtpt[i]=0; eta[i]=0; phi[i]=0;
      }
      goodJet.clear();
    }
  }
  fOUT.Write();
  fOUT.Close();

}
