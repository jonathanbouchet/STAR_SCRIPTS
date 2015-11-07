#include "Riostream.h" 
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TStyle.h"

using namespace std;

int countHotChip(TH2D* myhisto,float cut)
{ 
  int add_counter =0;
  for(int i=0;i<myhisto->GetNbinsX();++i){
    for(int j=0;j<myhisto->GetNbinsY();++j){
      if(myhisto->GetBinContent(i+1,j+1)>cut){
	++add_counter;
      }
    }
  } 
  return add_counter;
}

TH2D *maskChip(TH2D* myhisto,float cut)
{ 
  TH2D *tempo = new TH2D("tempo","",myhisto->GetNbinsX(),0,myhisto->GetNbinsX(),myhisto->GetNbinsY(),0,myhisto->GetNbinsY());
  int add_counter =0;
  for(int i=0;i<myhisto->GetNbinsX();++i){
    for(int j=0;j<myhisto->GetNbinsY();++j){
      if(myhisto->GetBinContent(i+1,j+1)>cut){
	tempo->SetBinContent(i+1,j+1,myhisto->GetBinContent(i+1,j+1));
      }
    }
  } 
  return tempo;
}
 
void read ()
{
  //ROOT stuff
  gStyle->SetPalette(55);
  gStyle->SetOptStat(000000);

  //get the number of lines in the list 
  std::ifstream ff("current_v3.lis");
  std::size_t lines_count =0;
   std::string ll;
   while (std::getline(ff , ll))
        ++lines_count;
   std::cout <<" num of files : " << lines_count << std::endl;

   //histograms definition
  Char_t * namePDF = new Char_t[200];
  TH1S *h2P  = new TH1S("h2P","side P masked chips",lines_count,0,lines_count);
  TH1S *h2N  = new TH1S("h2N","side N masked chips",lines_count,0,lines_count);
  h2P->SetXTitle("run number");
  h2N->SetXTitle("run number");
  h2P->SetYTitle("# of masked chips");
  h2N->SetYTitle("# of masked chips");
  h2P->SetMarkerStyle(20);
  h2N->SetMarkerStyle(20);
  h2P->SetTitle("side P");
  h2N->SetTitle("side N");

  TH1S *h2PP = new TH1S("h2PP","side P masked chips",lines_count,0,lines_count);
  TH1S *h2NN = new TH1S("h2NN","side N masked chips",lines_count,0,lines_count);
  h2PP->SetXTitle("# of masked chips");
  h2NN->SetXTitle("# of masked chips");

  TH2D *hChipRunSideP[20];
  TH2D *hChipRunSideN[20];
  for(int ladder=0;ladder<20;++ladder)
    {
      char *name;
      name = Form("%s%d","chip sideP occupancy vs. run for ladder :",ladder+1);
      hChipRunSideP[ladder] = new TH2D(Form("%s%d","hChipRunSideP",ladder),name,lines_count,0,lines_count,96,0,96);
      hChipRunSideP[ladder]->SetXTitle("run number");
      hChipRunSideP[ladder]->SetYTitle("chip #");
      name = Form("%s%d","chip side N occupancy vs. run for ladder :",ladder+1);
      hChipRunSideN[ladder] = new TH2D(Form("%s%d","hChipRunSideN",ladder),name,lines_count,0,lines_count,96,0,96);
      hChipRunSideN[ladder]->SetXTitle("run number");
      hChipRunSideN[ladder]->SetYTitle("chip #");
    }

  //loop over files
  string line;
  ifstream myfile ("current_v3.lis");
  int counter=0;
  if (myfile.is_open())
    {
      while ( myfile.good() && counter<(int)lines_count)
	{
	  counter++;
	  getline(myfile,line);
	  //std::cout << line << std::endl;
	  //std::istringstream str(line);
	  //std::cout << counter << "  : " << line << std::endl;
	  TString curdir = line.substr(7,3);
	  TString currun = line.substr(11,8);
	  TString curseq = line.substr(44,7);
	  const char *test  = curdir;
	  const char *test2 = currun;
	  const char *test3 = curseq;
	  std::cout << "dir/run/seq  : " << curdir << " "<< currun <<" "<< curseq << std::endl;

	  Char_t *currfile  = new Char_t[200];
	  sprintf(currfile,"%s%s%s%s%s%s%s%s%s","output/",test,"/",test2,"/st_physics_",test2,"_raw_",test3,".sstQA.root");
	  std::cout <<" currfile : " << currfile << std::endl;
	  
	  TFile *file = new TFile(currfile);
	  file->cd();
	  //get histograms from the current file
	  TH2D *AvgSideP  = (TH2D*)file->Get("AvgSideP");
	  TH2D *AvgSideN  = (TH2D*)file->Get("AvgSideN");
	  
	  for(int lad=0;lad<20;lad++)
	    {
	      for(int chip=0;chip<96;chip++)
		{
		  hChipRunSideP[lad]->SetBinContent(counter,chip+1,AvgSideP->GetBinContent(lad+1,chip+1));
		  hChipRunSideN[lad]->SetBinContent(counter,chip+1,AvgSideN->GetBinContent(lad+1,chip+1));
		}
	    }
	  int totCountP = countHotChip(AvgSideP,0.01);
	  int totCountN = countHotChip(AvgSideN,0.01);

	  TH2D *testP;
	  testP = maskChip(AvgSideP,0.01);
	  TH2D *testN;
	  testN = maskChip(AvgSideN,0.01);

	  testP->SetXTitle("ladder Id");
	  testP->SetYTitle("Chip Id");
	  testP->SetTitle(Form("%s%s","side P run number : ",test2));
	  
	  testN->SetXTitle("ladder Id");
	  testN->SetYTitle("Chip Id");
	  testN->SetTitle(Form("%s%s","side N run number : ",test2));
	  
	  std::cout <<" side P masked chips (cut : 0.01)  masked : " << totCountP << std::endl;
	  std::cout <<" side N masked chips (cut : 0.01)  masked : " << totCountN << std::endl;
	  
	  h2PP->Fill(totCountP);
	  h2NN->Fill(totCountN);
	  h2P->Fill(counter,totCountP);
	  h2N->Fill(counter,totCountN);
	  Char_t *canvasTitle = new Char_t[200];
	  sprintf(canvasTitle,"%s%s","run number : ",test2);
	  
	  AvgSideP->SetXTitle("ladder Id");
	  AvgSideP->SetYTitle("Chip Id");
	  AvgSideP->SetTitle(Form("%s%s","side P run number : ",test2));
	  
	  AvgSideN->SetXTitle("ladder Id");
	  AvgSideN->SetYTitle("Chip Id");
	  AvgSideN->SetTitle(Form("%s%s","side N run number : ",test2));

	  TCanvas *c1  = new TCanvas("c1","",800,800);
	  c1->Divide(2,2);
	  c1->cd(1);
	  AvgSideP->Draw("colz");
	  c1->cd(2);
	  AvgSideN->Draw("colz");
	  c1->cd(3);
	  testP->Draw("colz");
	  c1->cd(4);
	  testN->Draw("colz");
	  sprintf(namePDF,"%s","test.pdf(");
	  c1->Print(namePDF,"pdf");
	}
    }
  
  TCanvas *c0  = new TCanvas("c0","",800,800);
  c0->Divide(2,2);
  c0->cd(1);
  h2P->Draw("P");
  c0->cd(2);
  h2N->Draw("P");
  c0->cd(3);
  h2PP->Draw();
  c0->cd(4);
  h2NN->Draw();
  
  sprintf(namePDF,"%s","test.pdf)");
  c0->Print(namePDF,"pdf");

  //write root file

  Char_t *pdfOut = new Char_t[200];
  TFile *fileOut = new TFile("fileOut.root","RECREATE");
  fileOut->cd();
  for(int i=0;i<20;i++)
    {
      hChipRunSideP[i]->Write();
      hChipRunSideN[i]->Write();
      TCanvas *c11  = new TCanvas("c11","",800,400);
      c11->Divide(2,1);
      c11->cd(1);
      hChipRunSideP[i]->Draw("colz");
      c11->cd(2);
      hChipRunSideN[i]->Draw("colz");
      if(i==19)
	{
	  sprintf(pdfOut,"%s","testLadder.pdf)");
	  c11->Print(pdfOut,"pdf");
	}
      else
	{
	  sprintf(pdfOut,"%s","testLadder.pdf(");
	  c11->Print(pdfOut,"pdf");
	}
    }
}


