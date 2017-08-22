#include "DetectorConstruction.hh"

// Materials
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "Materials.hh"

// Geometry
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SubtractionSolid.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"

// Sensitive Detectors
#include "EnergyDepositionSD.hh"
#include "ParticleSD.hh"
#include "SecondarySD.hh"

// Units
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include <sstream>

using std::stringstream;

DetectorConstruction::DetectorConstruction() {}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume *DetectorConstruction::Construct() {

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
	//Materials *mat = new Materials();

	G4Material *Cu = man->FindOrBuildMaterial("G4_Cu");
	G4Material *AIR= man->FindOrBuildMaterial("G4_AIR");

	/************************* World volume *****************/

	G4double world_x = 4. * m; //Total length of World Volume
	G4double world_y = 4. * m;
	G4double world_z = 4. * m;

	G4Box *world_dim = new G4Box("world_dim", world_x * 0.5, world_y * 0.5 , world_z * 0.5);

	G4LogicalVolume *world_log =
	    new G4LogicalVolume(world_dim, AIR, "world_log");

	// world_log->SetVisAttributes(G4VisAttributes::GetInvisible());

	G4VPhysicalVolume *world_phys =
	    new G4PVPlacement(0, G4ThreeVector(), world_log, "world", 0, false, 0);

	/************************* Copper Collimator *****************/

	// Consists of ten 300mm * 300mm * 95mm Cu blocks arranged to form a
	// Collimator of 955mm length. 
	// The collimator diameter is chosen to increase step-wise from 12mm at
	// radiator target position to 20 mm at target position.

	G4double block_x = 300. * mm;
	G4double block_y = 300. * mm;
	G4double block_z = 95. * mm;



	G4Box *blockwithouthole = new G4Box("blockwithouthole", block_x / 2, block_y / 2, block_z / 2);
	

// Variable for collimator hole radius
G4double colholeradius_min=6. * mm;
G4double colholeradius_max=10. * mm;
G4double colholradiusstep= (colholeradius_max-colholeradius_min)*0.1;
G4double hole_radius;
G4Tubs *hole;
G4SubtractionSolid *block; 
G4LogicalVolume *blocks_Logical[10];
stringstream logical_volume_name;
G4double distcollimatortotarget=162. * mm;
G4VisAttributes *blockvis;
	// _________________________ Block Loop _________________________
for (G4int i=0; i<10;++i){
	 hole_radius = (colholeradius_max-colholradiusstep*i) * mm;

	 hole =
	    new G4Tubs("hole", 0. * mm, hole_radius, 200. * mm, 0., twopi);

	block =
	    new G4SubtractionSolid("block",blockwithouthole, hole);
logical_volume_name << "block" << i << "_Logical";
  blocks_Logical[i] =
	    new G4LogicalVolume(block, Cu, logical_volume_name.str().c_str(), 0, 0, 0);
	blockvis = new G4VisAttributes(light_orange);
	blocks_Logical[i]->SetVisAttributes(blockvis);

	new G4PVPlacement(0, G4ThreeVector(0., 0., -distcollimatortotarget-(i+0.5) * block_z), blocks_Logical[i], "block",
	                  world_log, 0, 0);}



	return world_phys;
}

void DetectorConstruction::ConstructSDandField() {

}
