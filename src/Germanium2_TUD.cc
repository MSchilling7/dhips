//**************************************************************//
//	Germaniumkristall - DET2 (Serial Number 72902)
//**************************************************************//

#include "Germanium2_TUD.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"

#include "G4NistManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

Germanium2_TUD::Germanium2_TUD(G4String Detector_Name) {

	G4Colour white(1.0, 1.0, 1.0);
	G4Colour grey(0.5, 0.5, 0.5);
	G4Colour black(0.0, 0.0, 0.0);
	G4Colour red(1.0, 0.0, 0.0);
	G4Colour green(0.0, 1.0, 0.0);
	G4Colour blue(0.0, 0.0, 1.0);
	G4Colour cyan(0.0, 1.0, 1.0);
	G4Colour magenta(1.0, 0.0, 1.0);
	G4Colour yellow(1.0, 1.0, 0.0);
	G4Colour orange(1.0, 0.5, 0.0);
	G4Colour light_orange(1.0, 0.82, 0.36);

	G4NistManager *man = G4NistManager::Instance();

	G4Material *Cu = man->FindOrBuildMaterial("G4_Cu");
	G4Material *Al = man->FindOrBuildMaterial("G4_Al");
	G4Material *Ge = man->FindOrBuildMaterial("G4_Ge");
	G4Material *vacuum = man->FindOrBuildMaterial("G4_Galactic");

	/***********************Germanium Mothervolume*********************/

	G4double case_Radius = 5. * cm;
	G4double case_Length = 16. * cm;

	Length = case_Length;
	Radius = case_Radius;

	G4Tubs *case_Solid = new G4Tubs("AluCase2_Solid", 0, case_Radius,
	                                case_Length / 2, 0. * deg, 360. * deg);

	germanium2_Logical =
	    new G4LogicalVolume(case_Solid, Al, "AluCase2_Logical", 0, 0, 0);

	germanium2_Logical->SetVisAttributes(white);

	/************************Vacuumlayer*****************************/

	G4double vacuum_Radius = 49. * mm;
	G4double vacuum_front_Thickness = 5.0 * mm;
	G4double vacuum_Length = 158. * mm;

	G4Tubs *vacuum_Solid = new G4Tubs("Vacuum2_Solid", 0, vacuum_Radius,
	                                  vacuum_Length / 2, 0. * deg, 360. * deg);

	G4LogicalVolume *vacuum_Logical =
	    new G4LogicalVolume(vacuum_Solid, vacuum, "Vacuum2_Logical", 0, 0, 0);

	vacuum_Logical->SetVisAttributes(cyan);

	new G4PVPlacement(0, G4ThreeVector(), vacuum_Logical, "Vacuum2",
	                  germanium2_Logical, false, 0);

	/************************Germanium Crystal*************************/

	G4double crystal_Radius = 38.5 * mm;
	G4double crystal_Length = 80. * mm;

	G4Tubs *crystal_Solid =
	    new G4Tubs("Crystal2_Solid", 0. * cm, crystal_Radius,
	               crystal_Length / 2, 0. * deg, 360. * deg);

	G4LogicalVolume *crystal2_Logical =
	    new G4LogicalVolume(crystal_Solid, Ge, Detector_Name, 0, 0, 0);

	crystal2_Logical->SetVisAttributes(green);

	new G4PVPlacement(0, G4ThreeVector(0., 0., vacuum_Length / 2 -
	                                               vacuum_front_Thickness -
	                                               crystal_Length / 2),
	                  crystal2_Logical, "Crystal2", vacuum_Logical, false, 0);

	/****************************Coldfinger*******************************/

	G4double coldfinger_Radius = 6. * mm;
	G4double coldfinger_Length = 39. * mm;

	G4Tubs *coldfinger_Solid =
	    new G4Tubs("Coldfinger2_Solid", 0. * cm, coldfinger_Radius,
	               coldfinger_Length / 2, 0. * deg, 360. * deg);

	G4LogicalVolume *coldfinger_Logical = new G4LogicalVolume(
	    coldfinger_Solid, Cu, "Coldfinger2_Logical", 0, 0, 0);

	coldfinger_Logical->SetVisAttributes(orange);

	new G4PVPlacement(
	    0, G4ThreeVector(0., 0., -(crystal_Length / 2 - coldfinger_Length / 2)),
	    coldfinger_Logical, "Coldfinger2", crystal2_Logical, false, 0);
}

Germanium2_TUD::~Germanium2_TUD() {}
