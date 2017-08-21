#ifndef Materials_h
#define Materials_h 1

#include "G4Material.hh"

class Materials {

  public:
	Materials(){ ConstructMaterials(); };
	~Materials(){};

	void ConstructMaterials();

  private:
};

#endif
