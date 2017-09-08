#include <TROOT.h>
#include <TSystemDirectory.h>
#include <TChain.h>
#include <TH1.h>
#include <TFile.h>

#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char* argv[]){
	
	// Command line arguments
	TString treename = argv[1]; // Name of ROOT tree which should exist in each of the files
	TString pattern1 = argv[2]; // File name pattern 1
	TString pattern2 = argv[3]; // File name pattern 2
	TString outputfilename = argv[4]; // Output file name


	

	// Find all files in the current directory that contain pattern1 and pattern1 and connect them to a TChain
	TSystemDirectory dir(".",".");

	TList *files = dir.GetListOfFiles();
	TChain dhips("dhips");

	cout << "Joining files that contain the patterns '" << pattern1 << "' and '" << pattern2 << "':" << endl;
	
	if(files){
		TSystemFile *file;
		TString fname;

		TIter next(files);
		while (file=(TSystemFile*) next()){

			fname = file->GetName();
			if(!file->IsDirectory() && fname.Contains(pattern1) && fname.Contains(pattern2)){
				cout << fname << endl;
				dhips.Add(fname);
			}
		}
	}

	// Create histogram
	TH1* h1 = new TH1F("h1", "ekin of Photons hitting the Target", 500, 0.0005, 10.0005);
	TH1* h2 = new TH1F("h2", "X-coordinate of Photons hitting the Target", 500, -11, 11.);
	TH1* h3 = new TH1F("h3", "Y-coordinate of Photons hitting the Target", 500,-11, 11.);
	TH1* h4 = new TH1F("h4", "Z-coordinate of Photons hitting the Target", 500, 5., 5.);
						

	// Fill histogram from TBranch in TChain with user-defined conditions
	Double_t Ekin, X,Y,Z,Volume, Particle;

	dhips.SetBranchAddress("ekin", &Ekin);
		dhips.SetBranchAddress("x", &X);
	dhips.SetBranchAddress("y", &Y);
	dhips.SetBranchAddress("z", &Z);
		dhips.SetBranchAddress("volume", &Volume);
	dhips.SetBranchAddress("particle", &Particle);


	for(int i = 0; i < dhips.GetEntries(); i++){
		dhips.GetEntry(i);

		if(Ekin > 0. && Particle == 22){
			if(Volume == 0)
				{h1->Fill(Ekin);
				h2->Fill(X);
				h3->Fill(Y);
				h4->Fill(Z);
			}

		}
	}

	

	// Write histogram to a new TFile
	TFile *of = new TFile(outputfilename, "RECREATE");

	h1->Write();
	h2->Write();
	h3->Write();
	h4->Write();
	
	of->Close();

	cout << "Created output file " << outputfilename << endl;
}	
