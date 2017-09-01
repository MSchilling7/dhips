/// \file BGO.hh
/// \brief Header file of BGO class without parents

#ifndef BGO_HH
#define BGO_HH

#include "G4UnionSolid.hh"
#include "G4LogicalVolume.hh"

class BGO
{
public:
  BGO();
  ~BGO();

  G4LogicalVolume* Get_Logical() { return bgo_Mother_Logical; }
  G4LogicalVolume* Get_Crystal_Logical() { return bgo_Crystal_Logical; }
  G4UnionSolid* Get_Al_Solid() { return al_Solid; }

  static G4double bgo_Length;
  static G4double bgo_Collimator_Length;

private:
  G4LogicalVolume* bgo_Mother_Logical;
  G4LogicalVolume* bgo_Crystal_Logical;
  G4UnionSolid* al_Solid;
};

#endif
