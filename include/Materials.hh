#ifndef Materials_h
#define Materials_h 1

#include "G4Material.hh"

class Materials {

  public:
	Materials();
	~Materials();

	void ConstructMaterials();

	// Get Materials
	// G4Material* Get_BGO() {return BGO;}
	G4Material *Get_target_TiO2() { return target_TiO2; }
	G4Material *Get_P10() { return P10; }
	G4Material *Get_target_Cr2O3() { return target_Cr2O3; }
	G4double Get_Cr2O3_density() { return Cr2O3_density; }
	G4Material *Get_target_Sn() { return target_Sn; }
	G4double Get_Sn_density() { return Sn_density; }

  private:
	// G4Material* BGO;
	G4Material *target_TiO2;
	G4Material *P10;
	G4Material *target_Cr2O3;
	G4double Cr2O3_density;
	G4Material *target_Sn;
	G4double Sn_density;
};

#endif

