// g++ readHe4.cpp `root-config --libs --cflags` -o readHe4 -O3


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
//#include "function.h"

#include <TChain.h>
#include <TString.h>
#include <TF1.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TFile.h>
#include <TString.h>

using namespace std;

int main(int argc, char **argv)
{
  int start_s=clock();
  
  //check to see if an arguement was provided 
  if(argc<2)
  {
    cout<<"You must provide the name of the file you want to analyze.\n";
    return 1;
  }
  
  TString inFileName = argv[1];
  
  ifstream inFile;
  inFile.open(inFileName);
  bool endOfFile = false;
  
  if (inFile.fail())
  {
    cout << "Error: Unable to open input file!" << endl;
  }
  
  else
  {
    cout << inFileName << " is opened." << endl;
  }
  
  const int NUM_SEGMENTS = 3;
  const int NUM_CHANNELS = 256;
  
  //simple array to store histo values
  double data[NUM_SEGMENTS][NUM_CHANNELS] = {0.};
  
  //make ROOT file for histos
  TFile *ROOTFile;
  string OutFileName = "out.root";
  
	ROOTFile = new TFile(OutFileName.c_str(),"RECREATE");
	ROOTFile->SetCompressionSettings(3);
  
  TH1F* segment0Histo = new TH1F("segment0Histo",";channel;counts",256,0,256);
  TH1F* segment1Histo = new TH1F("segment1Histo",";channel;counts",256,0,256);
  TH1F* segment2Histo = new TH1F("segment2Histo",";channel;counts",256,0,256);
  TH1F* totalHisto = new TH1F("totalHisto",";channel;counts",256,0,256);
  
  //start reading data from csv file ------------------------------------------
  stringstream ss;
  string linestr;
  string substr;
  
  for(int j = 0; j < NUM_SEGMENTS; j++)
  {
    //grab descriptor line and throw away
    getline(inFile,linestr,'\n');
    cout << linestr << endl;
    
    getline(inFile,linestr,'\n');
    
    //now read segment data
    ss.str(linestr);
    
    for(int i = 0; i < NUM_CHANNELS; i++)
    {
      getline(ss,substr,','); //grab each comma delimited item
      
      data[j][i] = stod(substr); //convert item to double and store in array
    }
    
    /*
    for(int i = 0; i < NUM_CHANNELS; i++)
    {
      cout << data[j][i] << " ";
    }
  
    cout << endl;
    */
    
    ss.clear();
  }
  
  for(int i = 0; i < NUM_CHANNELS; i++)
  {
    segment0Histo->SetBinContent(i,data[0][i]);
    segment1Histo->SetBinContent(i,data[1][i]);
    segment2Histo->SetBinContent(i,data[2][i]);
    totalHisto->SetBinContent(i,data[0][i]+data[1][i]+data[2][i]);
  }
  
  cout << "Writing ROOT file..." << endl;
 
	ROOTFile->Write();
	ROOTFile->Close();
 
 return 0; 
}