#include "BGO.hh"

//#include "G4LogicalVolume.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
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
	bgo_Mother_Radius = 95.*mm;

	//*************************************************
	// BGO mother volume: a tub filled with air that encloses the whole BGO
	//*************************************************
	
	G4Tubs *bgo_Mother_Solid = new G4Tubs("bgo_Mother_Solid", 0., bgo_Mother_Radius, bgo_Mother_Length*0.5, 0.*deg, 360.*deg);

	bgo_Mother_Logical = new G4LogicalVolume(bgo_Mother_Solid, AIR, "bgo_Mother_Logical");
	bgo_Mother_Logical->SetVisAttributes(yellow);

	//*************************************************
	// BGO aluminium case
	// Consists of four parts with labels 1-4 from the target-facing side to the backside
	//*************************************************
	
G4double	al_Case1_Length = 8.*mm;
G4double	al_Case1_inner_angle = 16.5*deg;
G4double	al_Case1_outer_angle = 21.*deg;
G4double	al_Case1_Rmin1 = 25.*mm;
G4double	al_Case1_Rmax1 = 62.*mm;
G4double	al_Case1_Rmin2 = al_Case1_Rmin1 + al_Case1_Length*tan(al_Case1_inner_angle);
G4double	al_Case1_Rmax2 = al_Case1_Rmax1 + al_Case1_Length*tan(al_Case1_outer_angle);
	
	G4Cons* al_Case1_Solid = new G4Cons("al_Case1_Solid", al_Case1_Rmin1, al_Case1_Rmax1, al_Case1_Rmin2, al_Case1_Rmax2, al_Case1_Length*0.5, 0.*deg, 360.*deg);
	G4LogicalVolume* al_Case1_Logical = new G4LogicalVolume(al_Case1_Solid, Al, "al_Case1_Logical");
	new G4PVPlacement(0, G4ThreeVector(0., 0., bgo_Mother_Length*0.5 - 4.*mm), al_Case1_Logical, "al_Case1", bgo_Mother_Logical, false, 0);
	al_Case1_Logical->SetVisAttributes(cyan);
}

BGO::~BGO(){};
