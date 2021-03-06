#include "TROOT.h"
#include "TFile.h"
#include "../../style/Style.C"
#include "../../style/Labels.C"
#include "TGraphErrors.h"
#include "TMath.h"


//  for(int iz=1; iz<2; iz++) {
//    TString zm="zm0.5";
//     if(iz==1) zm="zm1.0";
//    if(iz==2) zm="zm1.0";

void PlotsPedestal(){
 
  gROOT->Reset();
  SetIrlesStyle();
  gStyle->SetOptFit(0); 
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  
  gStyle->SetTitleBorderSize(0);
  gStyle->SetTitleX(0.4);
  gStyle->SetTitleY(0.9);
  gStyle->SetTitleStyle(0);
  gStyle->SetTitleFontSize(0.03);
  gStyle->SetMarkerSize(1.2);

  TString grid="grid20";
  TString conf="conf2";
  bool bcidplot=false;
  
  int islabs=6;
  TString nslabs_pedestal=TString::Format("pedestal_nslabs%i",islabs);

  TString bcid="bcidmax2850";

  for(int ilayer=2; ilayer<3; ilayer++) {
    TString energy_string[6];
    energy_string[0]="1GeV";
    energy_string[1]="2GeV";
    energy_string[2]="3GeV";
    energy_string[3]="4GeV";
    energy_string[4]="5GeV";
    energy_string[5]="5.8GeV";
    //energy_string[0]="2GeV";
    // energy_string[1]="4GeV";
    
    TGraph *pedestal_mean[6];
    
    for(int ienergy=0; ienergy<6; ienergy++) {
      
      double x[15];
      double y[15];
      TString s_file=nslabs_pedestal+"_"+bcid+"/"+conf+"_"+grid+"_"+energy_string[ienergy]+"_mipcut0.5_showers.root";
      std::cout<<"Opening file: "<<s_file<<std::endl;
      TFile *file = new TFile(s_file);

      int nbins=0;
      for(int isca=0;isca<15; isca++) {
	TString histostring = TString::Format("bs_pedestal_mean_distribution_isca%i_ilayer%i",isca,ilayer);
	if(bcidplot==true) histostring = TString::Format("bs_pedestal_mean_distribution_ibcid%i_ilayer%i",isca,ilayer);
	TH1F *temp= (TH1F*)file->Get(histostring);
	x[isca]=isca;
	if(bcidplot==true) x[isca]=isca*100.+1200;
	if (temp->GetEntries()==0) continue;
	y[isca]=100*temp->GetMean();
	nbins++;
      }
      
      pedestal_mean[ienergy] = new TGraph(nbins,x,y);
    }
    
    TCanvas *c_energy = new TCanvas(TString::Format("c_energy_ilayer%i",ilayer),TString::Format("c_energy_ilayer%i",ilayer),800,600);
  //c_energy->Divide(2,1);
  c_energy->cd(1);
  pedestal_mean[0]->GetXaxis()->SetTitle("SCA");
  if(bcidplot==true)  pedestal_mean[0]->GetXaxis()->SetTitle("BCID");
  pedestal_mean[0]->GetYaxis()->SetTitle("average pedestal deviation [%MIP]");
  pedestal_mean[0]->GetYaxis()->SetRangeUser(-20,20);
  pedestal_mean[0]->SetLineWidth(1);
  pedestal_mean[0]->SetLineColor(1);
  pedestal_mean[0]->SetMarkerStyle(4);
  pedestal_mean[0]->SetMarkerColor(1);
  pedestal_mean[0]->Draw("al*");

  for(int ienergy=1; ienergy<6; ienergy++) {
    pedestal_mean[ienergy]->SetLineWidth(1);
    pedestal_mean[ienergy]->SetLineColor(ienergy+1);
    pedestal_mean[ienergy]->SetMarkerStyle(ienergy+19);
    pedestal_mean[ienergy]->SetMarkerColor(ienergy+1);
    pedestal_mean[ienergy]->Draw("lp");
  }
    
  TLegend *l_1 = new TLegend(0.45,0.2,0.9,0.4);
  l_1->SetHeader(TString::Format("SiW-ECAL: wafer 3, W-configuration 2, layer %i",ilayer));
  
  l_1->AddEntry(pedestal_mean[0],"e^{+} 1 GeV","lp");
  l_1->AddEntry(pedestal_mean[1],"e^{+} 2 GeV","lp");
  l_1->AddEntry(pedestal_mean[2],"e^{+} 3 GeV","lp");
  l_1->AddEntry(pedestal_mean[3],"e^{+} 4 GeV","lp");
  l_1->AddEntry(pedestal_mean[4],"e^{+} 5 GeV","lp");
  l_1->AddEntry(pedestal_mean[5],"e^{+} 5.8 GeV","lp");

  l_1->SetFillColor(0);
  l_1->SetLineColor(0);
  l_1->SetShadowColor(0);
  l_1->Draw();
  IRLESLabel(0.2,0.88,"",kGray+2);

 if(bcidplot==false) c_energy->Print("pedestal_plots/pedestal_deviation_energy_nslabs6_"+bcid+"_"+conf+"_"+grid+"_mipcut0.5_layer"+TString::Format("%i.eps",ilayer));
 else c_energy->Print("pedestal_plots/pedestal_deviation_bcid_energy_nslabs6_"+bcid+"_"+conf+"_"+grid+"_mipcut0.5_layer"+TString::Format("%i.eps",ilayer));

  }
         
}
