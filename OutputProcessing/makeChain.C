#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <TChain.h>
#include <TFile.h>

void makeChain(int numberOfChains, int simNumber)
{
	string buffer="dhips";
	buffer+=std::to_string(simNumber);
	buffer+="_sum.root";
	TFile* output_File = new TFile(buffer.c_str(),"recreate");

	TChain* c = new TChain("dhips");
	for(int i=0;i<numberOfChains;i++)
	{
		buffer="dhips";
		buffer+=std::to_string(simNumber);
		buffer+="_t";
		buffer+=std::to_string(i);
		buffer+=".root";

		c->Add(buffer.c_str());
		std::cout<<"Adding "<<buffer<<" to "<< output_File->GetName()<<endl;
		
	}
	c->Write();
}
