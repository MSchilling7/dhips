#include "BGO.hh"

#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4RotationMatrix.hh"
#include "G4PVPlacement.hh"

#include "G4Color.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4VisAttributes.hh"

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
	G4Material* AIR = man->FindOrBuildMaterial("G4_AIR");

	bgo_Mother_Length = 202.*mm;
	bgo_Mother_Radius = 115.*mm;

	//*************************************************
	// BGO mother volume: a tub filled with air that encloses the whole BGO
	//*************************************************
	
	G4Tubs *bgo_Mother_Solid = new G4Tubs("bgo_Mother_Solid", 0., bgo_Mother_Radius, bgo_Mother_Length*0.5, 0.*deg, 360.*deg);

	bgo_Mother_Logical = new G4LogicalVolume(bgo_Mother_Solid, AIR, "bgo_Mother_Logical");
	bgo_Mother_Logical->SetVisAttributes(yellow);

	//*************************************************
	// BGO aluminium case
	// Consists of seven parts with labels 1-7 from the target-facing side (1) to the backside (7)
	//*************************************************
	
	// Part 1
	
G4double	al_Case1_Length = 8.*mm;
G4double	al_Case1_inner_angle = 16.5*deg;
G4double	al_Case1_outer_angle = 21.*deg;
G4double	al_Case1_Rmin1 = 25.*mm;
G4double	al_Case1_Rmax1 = 62.*mm;
G4double	al_Case1_Rmin2 = al_Case1_Rmin1 + al_Case1_Length*tan(al_Case1_inner_angle);
G4double	al_Case1_Rmax2 = al_Case1_Rmax1 + al_Case1_Length*tan(al_Case1_outer_angle);
	
	G4Cons* al_Case1_Solid = new G4Cons("al_Case1_Solid", al_Case1_Rmin1, al_Case1_Rmax1, al_Case1_Rmin2, al_Case1_Rmax2, al_Case1_Length*0.5, 0.*deg, 360.*deg);
	G4LogicalVolume* al_Case1_Logical = new G4LogicalVolume(al_Case1_Solid, Al, "al_Case1_Logical");
	G4RotationMatrix *rot = new G4RotationMatrix();
	rot->rotateX(180.*deg);
	new G4PVPlacement(rot, G4ThreeVector(0., 0., bgo_Mother_Length*0.5 - al_Case1_Length*0.5), al_Case1_Logical, "al_Case1", bgo_Mother_Logical, false, 0);
	al_Case1_Logical->SetVisAttributes(cyan);

	// Part 2
	
G4double	al_Case2_Length = 78.*mm;
G4double	al_Case2_inner_angle = 21.*deg;
G4double	al_Case2_outer_angle = 21.*deg;
G4double	al_Case2_Rmin1 = al_Case1_Rmax2 - 3.*mm;
G4double	al_Case2_Rmax1 = al_Case1_Rmax2;
G4double	al_Case2_Rmin2 = al_Case2_Rmin1 + al_Case2_Length*tan(al_Case2_inner_angle);
G4double	al_Case2_Rmax2 = al_Case2_Rmax1 + al_Case2_Length*tan(al_Case2_outer_angle);
	
	G4Cons* al_Case2_Solid = new G4Cons("al_Case2_Solid", al_Case2_Rmin1, al_Case2_Rmax1, al_Case2_Rmin2, al_Case2_Rmax2, al_Case2_Length*0.5, 0.*deg, 360.*deg);
	G4LogicalVolume* al_Case2_Logical = new G4LogicalVolume(al_Case2_Solid, Al, "al_Case2_Logical");
	new G4PVPlacement(rot, G4ThreeVector(0., 0., bgo_Mother_Length*0.5 - al_Case1_Length - al_Case2_Length*0.5), al_Case2_Logical, "al_Case2", bgo_Mother_Logical, false, 0);
	al_Case2_Logical->SetVisAttributes(cyan);
	
	// Part 3
	
G4double	al_Case3_Length = 86.*mm;
G4double	al_Case3_Rmin1 = al_Case2_Rmin2;
G4double	al_Case3_Rmax1 = al_Case2_Rmax2;
G4double	al_Case3_Rmin2 = al_Case3_Rmin1; 
G4double	al_Case3_Rmax2 = al_Case3_Rmax1;	
	G4Cons* al_Case3_Solid = new G4Cons("al_Case3_Solid", al_Case3_Rmin1, al_Case3_Rmax1, al_Case3_Rmin2, al_Case3_Rmax2, al_Case3_Length*0.5, 0.*deg, 360.*deg);
	G4LogicalVolume* al_Case3_Logical = new G4LogicalVolume(al_Case3_Solid, Al, "al_Case3_Logical");
	new G4PVPlacement(rot, G4ThreeVector(0., 0., bgo_Mother_Length*0.5 - al_Case1_Length - al_Case2_Length - al_Case3_Length*0.5), al_Case3_Logical, "al_Case3", bgo_Mother_Logical, false, 0);

	al_Case3_Logical->SetVisAttributes(cyan);
	
	// Part 4
	
G4double	al_Case4_Length = 20.*mm;
G4double	al_Case4_Rmin1 = al_Case3_Rmin2;
G4double	al_Case4_Rmax1 = al_Case4_Rmin1 + 23.*mm;
G4double	al_Case4_Rmin2 = al_Case4_Rmin1; 
G4double	al_Case4_Rmax2 = al_Case4_Rmax1;	
	G4Cons* al_Case4_Solid = new G4Cons("al_Case4_Solid", al_Case4_Rmin1, al_Case4_Rmax1, al_Case4_Rmin2, al_Case4_Rmax2, al_Case4_Length*0.5, 0.*deg, 360.*deg);
	G4LogicalVolume* al_Case4_Logical = new G4LogicalVolume(al_Case4_Solid, Al, "al_Case4_Logical");
	new G4PVPlacement(rot, G4ThreeVector(0., 0., bgo_Mother_Length*0.5 - al_Case1_Length - al_Case2_Length - al_Case3_Length - al_Case4_Length*0.5), al_Case4_Logical, "al_Case4", bgo_Mother_Logical, false, 0);

	al_Case4_Logical->SetVisAttributes(cyan);
	
	// Part 5
	
G4double	al_Case5_Length = 11.*mm;
G4double	al_Case5_Rmin1 = al_Case4_Rmin2;
G4double	al_Case5_Rmax1 = al_Case4_Rmin2;
G4double	al_Case5_Rmin2 = 51.5*mm; 
G4double	al_Case5_Rmax2 = al_Case5_Rmax1;	
	G4Cons* al_Case5_Solid = new G4Cons("al_Case5_Solid", al_Case5_Rmin1, al_Case5_Rmax1, al_Case5_Rmin2, al_Case5_Rmax2, al_Case5_Length*0.5, 0.*deg, 360.*deg);
	G4LogicalVolume* al_Case5_Logical = new G4LogicalVolume(al_Case5_Solid, Al, "al_Case5_Logical");
	new G4PVPlacement(rot, G4ThreeVector(0., 0., bgo_Mother_Length*0.5 - al_Case1_Length - al_Case2_Length - al_Case3_Length - 7.*mm - al_Case5_Length*0.5), al_Case5_Logical, "al_Case5", bgo_Mother_Logical, false, 0);

	al_Case5_Logical->SetVisAttributes(cyan);
	
	// Part 6
	
G4double	al_Case6_Length = 3.*mm;
G4double	al_Case6_Rmin1 = al_Case5_Rmin2;
G4double	al_Case6_Rmax1 = al_Case5_Rmax2;
G4double	al_Case6_Rmin2 = al_Case6_Rmin1; 
G4double	al_Case6_Rmax2 = al_Case6_Rmax1;	
	G4Cons* al_Case6_Solid = new G4Cons("al_Case6_Solid", al_Case6_Rmin1, al_Case6_Rmax1, al_Case6_Rmin2, al_Case6_Rmax2, al_Case6_Length*0.5, 0.*deg, 360.*deg);
	G4LogicalVolume* al_Case6_Logical = new G4LogicalVolume(al_Case6_Solid, Al, "al_Case6_Logical");
	new G4PVPlacement(rot, G4ThreeVector(0., 0., bgo_Mother_Length*0.5 - al_Case1_Length - al_Case2_Length - al_Case3_Length - 7.*mm  - al_Case5_Length - al_Case6_Length*0.5), al_Case6_Logical, "al_Case6", bgo_Mother_Logical, false, 0);

	al_Case6_Logical->SetVisAttributes(cyan);
	
	// Part 7
	
G4double	al_Case7_Length = 9.*mm;
G4double	al_Case7_Rmin1 = al_Case6_Rmin2;
G4double	al_Case7_Rmax1 = al_Case6_Rmax2;
G4double	al_Case7_Rmin2 = al_Case6_Rmin2; 
G4double	al_Case7_Rmax2 = al_Case6_Rmin2 + 7.*mm;

	G4Cons* al_Case7_Solid = new G4Cons("al_Case7_Solid", al_Case7_Rmin1, al_Case7_Rmax1, al_Case7_Rmin2, al_Case7_Rmax2, al_Case7_Length*0.5, 0.*deg, 360.*deg);
	G4LogicalVolume* al_Case7_Logical = new G4LogicalVolume(al_Case7_Solid, Al, "al_Case7_Logical");
	new G4PVPlacement(rot, G4ThreeVector(0., 0., bgo_Mother_Length*0.5 - al_Case1_Length - al_Case2_Length - al_Case3_Length - 7.*mm  - al_Case5_Length - al_Case6_Length - al_Case7_Length*0.5), al_Case7_Logical, "al_Case7", bgo_Mother_Logical, false, 0);

	al_Case7_Logical->SetVisAttributes(cyan);
}

BGO::~BGO(){}
