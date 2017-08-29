/// \file BGO.cc
/// \brief Implementation of BGO class

#include "BGO.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "globals.hh"
#include "G4VisAttributes.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"

// Initialization of static member variables of BGO

G4double BGO::bgo_Length = 0.*cm;
G4double BGO::bgo_Collimator_Length = 0.*cm;

BGO::BGO(){
       
     G4Colour  white   (1.0, 1.0, 1.0) ;
     G4Colour  grey    (0.5, 0.5, 0.5) ;
     G4Colour  black   (0.0, 0.0, 0.0) ;
     G4Colour  red     (1.0, 0.0, 0.0) ;
     G4Colour  green   (0.0, 1.0, 0.0) ;
     G4Colour  blue    (0.0, 0.0, 1.0) ;
     G4Colour  cyan    (0.0, 1.0, 1.0) ;
     G4Colour  magenta (1.0, 0.0, 1.0) ;
     G4Colour  yellow  (1.0, 1.0, 0.0) ;
     G4Colour  orange  (1.0, 0.5, 0.0) ;
     G4Colour  light_orange  (1.0, 0.82, 0.36) ;
     
  G4NistManager* man = G4NistManager::Instance();
  
  G4Material* Al = man->FindOrBuildMaterial("G4_Al");
  G4Material* bgo = man->FindOrBuildMaterial("G4_BGO");

  // BGO Cylinder which encloses the detector crystal  
  G4double bgo_Cylinder_innerRadius = 51.*mm;
  G4double bgo_Cylinder_outerRadius = 92.*mm;
  G4double bgo_Cylinder_Length = 116.*mm;
  
  G4Tubs* bgo_Cylinder_Solid = new G4Tubs("bgo_Cylinder_Solid", bgo_Cylinder_innerRadius, bgo_Cylinder_outerRadius,
				    bgo_Cylinder_Length/2, 0., twopi);
  
  // First BGO Cone on top of the BGO Cylinder which also encloses the detector crystal. Therefore,
  // it needs a similar cylindric hole as bgo_Cylinder. This is accomplished by subtracting a cylindric
  // body.
  
  G4double bgo_Cone1_fouterRadius = 74.6*mm;
  
  G4double bgo_Cone1_bouterRadius = bgo_Cylinder_outerRadius;
  G4double bgo_Cone1_binnerRadius = bgo_Cylinder_innerRadius;
  
  G4double bgo_Cone1_Length = 44.*mm;

  G4Cons* bgo_Cone1_Solid = new G4Cons("bgo_Cone1_Solid", 0., bgo_Cone1_bouterRadius, 0., bgo_Cone1_fouterRadius,
				       bgo_Cone1_Length/2, 0, twopi);
  G4Tubs* bgo_Cone1_Hole = new G4Tubs("bgo_Cone1_Hole", 0., bgo_Cone1_binnerRadius, 
				      bgo_Cone1_Length /* When setting the exact hole length as bgo_Cone1_Length/2, the visualization looks awkward. Therefore, subtract a little more to prevent numerical instabilites.*/, 
				      0., twopi);

  G4SubtractionSolid* bgo_Cone1 = new G4SubtractionSolid("bgo_Cone1", bgo_Cone1_Solid, bgo_Cone1_Hole);

  // Second BGO Cone on top of Cone1. The detector can be pushed into the BGO up to this Cone, because it
  // has a smaller conical hole in it.
  
  G4double bgo_Cone2_fouterRadius = 62.*mm;
  G4double bgo_Cone2_finnerRadius = 28*mm;
  
  G4double bgo_Cone2_bouterRadius = bgo_Cone1_fouterRadius;
  G4double bgo_Cone2_binnerRadius = 37.5*mm;
  
  G4double bgo_Cone2_Length = 32.*mm;
  
  G4Cons* bgo_Cone2_Solid = new G4Cons("bgo_Cone2_Solid", bgo_Cone2_binnerRadius, bgo_Cone2_bouterRadius,
				       bgo_Cone2_finnerRadius, bgo_Cone2_fouterRadius, bgo_Cone2_Length/2, 0, twopi);
  
  
  // Merge the three parts, Cylinder, Cone1 and Cone2

  G4RotationMatrix* rmy = new G4RotationMatrix();
  rmy->rotateY(180.*deg);
  
  G4UnionSolid* bgo_1 = new G4UnionSolid("bgo_1", bgo_Cylinder_Solid, bgo_Cone1, 0, G4ThreeVector(0., 0., bgo_Cylinder_Length/2 + bgo_Cone1_Length/2));
  
  G4UnionSolid* bgo_2 = new G4UnionSolid("bgo_2", bgo_1, bgo_Cone2_Solid, 0, G4ThreeVector(0., 0., bgo_Cylinder_Length/2 + bgo_Cone1_Length + bgo_Cone2_Length/2));

  G4LogicalVolume* bgo_Logical = new G4LogicalVolume(bgo_2, bgo, "bgo_Logical", 0, 0, 0);
  
  bgo_Logical->SetVisAttributes(blue);
  
  bgo_Crystal_Logical = bgo_Logical;

  // Construct Al housing of BGO crystal
  // The Al housing construction is very similar to the BGO crystal. It is constructed as a solid,
  // and later the BGO volume is subtracted. The Al housing acts as the mother volume of BGO since
  // it encloses all the geometry.
  
  // al_Cylinder
  G4double al_Cylinder_innerRadius = 50.*mm;
  G4double al_Cylinder_outerRadius = 95.*mm;
  G4double al_Cylinder_Length = 116.*mm;
  
  G4Tubs* al_Cylinder_Solid = new G4Tubs("al_Cylinder_Solid", al_Cylinder_innerRadius, al_Cylinder_outerRadius,
				    al_Cylinder_Length/2, 0., twopi);
 
  G4SubtractionSolid* al_Cylinder_SubSolid = new G4SubtractionSolid("al_Cylinder_SubSolid", al_Cylinder_Solid, bgo_Cylinder_Solid);

  // al_Cone1
  
  G4double al_Cone1_fouterRadius = bgo_Cone1_fouterRadius + 3.*mm;
  
  G4double al_Cone1_bouterRadius = al_Cylinder_outerRadius;
  G4double al_Cone1_binnerRadius = al_Cylinder_innerRadius;

  G4double al_Cone1_Length = 44.*mm + 1.*mm; //Add 1mm to avoid numerical instabilities when calculating the G4SubtractionSolid

  G4Cons* al_Cone1_Solid = new G4Cons("al_Cone1_Solid", 0., al_Cone1_bouterRadius, 0., al_Cone1_fouterRadius,
				       al_Cone1_Length/2, 0, twopi);
  G4Tubs* al_Cone1_Hole = new G4Tubs("al_Cone1_Hole", 0., al_Cone1_binnerRadius, 
				      al_Cone1_Length, 
				      0., twopi);

  G4SubtractionSolid* al_Cone1 = new G4SubtractionSolid("al_Cone1", al_Cone1_Solid, al_Cone1_Hole);

  G4SubtractionSolid* al_Cone1_SubSolid = new G4SubtractionSolid("al_Cone1_SubSolid", al_Cone1, bgo_Cone1, 0, G4ThreeVector(0., 0., 0.));

  // al Cone2
  
  G4double al_Cone2_fouterRadius = bgo_Cone2_fouterRadius + 3.*mm;
  G4double al_Cone2_finnerRadius = bgo_Cone2_finnerRadius - 3.*mm;
  
  G4double al_Cone2_bouterRadius = al_Cone1_fouterRadius;
  G4double al_Cone2_binnerRadius = bgo_Cone2_binnerRadius - 3.*mm;
  
  G4double al_Cone2_Length = 42.*mm;
  bgo_Collimator_Length = al_Cone2_Length;
  
  G4Cons* al_Cone2_Solid = new G4Cons("al_Cone2_Solid", al_Cone2_binnerRadius, al_Cone2_bouterRadius,
				       al_Cone2_finnerRadius, al_Cone2_fouterRadius, al_Cone2_Length/2, 0, twopi);

  G4SubtractionSolid* al_Cone2_SubSolid = new G4SubtractionSolid("al_Cone2_SubSolid", al_Cone2_Solid, bgo_Cone2_Solid, 0, G4ThreeVector(0., 0., - al_Cone2_Length/2 + bgo_Cone2_Length/2));
  
  // Merge the three parts, Cylinder, Cone1 and Cone2
  
  G4UnionSolid* al_1 = new G4UnionSolid("al_1", al_Cylinder_SubSolid, al_Cone1_SubSolid, 0, G4ThreeVector(0., 0., al_Cylinder_Length/2 + al_Cone1_Length/2));
  
  G4UnionSolid* al_2 = new G4UnionSolid("al_2", al_1, al_Cone2_SubSolid, 0, G4ThreeVector(0., 0., al_Cylinder_Length/2 + al_Cone1_Length + al_Cone2_Length/2));
  
  G4LogicalVolume* al_Logical = new G4LogicalVolume(al_2, Al, "al_Logical", 0, 0, 0);
  
  al_Logical->SetVisAttributes(grey);
  
  bgo_Mother_Logical = al_Logical;
  bgo_Length = al_Cone1_Length + al_Cone2_Length + al_Cylinder_Length;
  
  new G4PVPlacement(0, G4ThreeVector(0., 0., -al_Cylinder_Length/2 + bgo_Cylinder_Length/2), bgo_Logical, "bgo_Crystal", bgo_Mother_Logical, false, 0);
  
}

BGO::~BGO(){
}