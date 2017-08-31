#ifndef BGO_HH
#define BGO_HH

#include "G4LogicalVolume.hh"

class BGO
{
public:
  BGO();
  ~BGO();

  G4LogicalVolume* Get_Logical() { return bgo_Mother_Logical; }
  G4LogicalVolume* Get_Crystal_Logical() { return bgo_Crystal_Logical; }

  G4double Get_Length(){ return bgo_Mother_Length; };
  G4double Get_Radius(){ return bgo_Mother_Radius; };


private:
  G4LogicalVolume* bgo_Mother_Logical;
  G4LogicalVolume* bgo_Crystal_Logical;

  G4double bgo_Mother_Length;
  G4double bgo_Mother_Radius;
};

#endif
