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
	G4Colour leadcolor=green;

	G4NistManager *man = G4NistManager::Instance();
	//Materials *mat = new Materials();

	G4Material *Cu = man->FindOrBuildMaterial("G4_Cu");
	G4Material *AIR= man->FindOrBuildMaterial("G4_AIR");
	G4Material *Fe= man->FindOrBuildMaterial("G4_Fe");
	G4Material *Pb= man->FindOrBuildMaterial("G4_Pb");
	/************************* World volume *****************/

	G4double world_x = 6. * m; //Total length of World Volume
	G4double world_y = 6. * m;
	G4double world_z = 6. * m;

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

	G4double block_x = 300. * mm;  //x increases to the left from the perpective of the beam
	G4double block_y = 300. * mm;  //y increases towards the top
	G4double block_z = 95. * mm;   //z lies in the direction of the beam



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

/************************* Red Boxes for Orientation *****************/

// These boxes are purely for orientation and should be commented out
// before running a simulation
//Beginning Block 025-------------------------------------
G4LogicalVolume *block25_Logical;
G4Box *block25; 
G4VisAttributes *block25vis;

block25 =
	    new G4Box("block25",40*cm,40*cm, 40*cm);

  block25_Logical =
	    new G4LogicalVolume(block25, Fe, "block25_Logical", 0, 0, 0);
	block25vis = new G4VisAttributes(red);
	block25_Logical->SetVisAttributes(block25vis);

	new G4PVPlacement(0, G4ThreeVector(0, -100*cm,-250*cm ), block25_Logical, "block", world_log, 0, 0);
//End Block 025---------------------------------------
//Beginning Block 026-------------------------------------
G4LogicalVolume *block26_Logical;
G4Box *block26; 
G4VisAttributes *block26vis;

block26 =
	    new G4Box("block26",10*cm,10*cm, 10*cm);

  block26_Logical =
	    new G4LogicalVolume(block26, Fe, "block26_Logical", 0, 0, 0);
	block26vis = new G4VisAttributes(red);
	block26_Logical->SetVisAttributes(block26vis);

	new G4PVPlacement(0, G4ThreeVector(0, 0,0 ), block26_Logical, "block", world_log, 0, 0);
//End Block 026---------------------------------------


/************************* Iron Shielding *****************/
// Iron Shielding around the Copper Collimator
// 20 cm thick on the sides. 10cm thick on top. 40 cm thick at the bottom
// Length unclear. Assumption: All iron until it reaches the target chamber.
// Box 21,23 are the Boxes on the left and rigth side and they are reaching 
// all the way to top and bottom.
// Box 22,24 are the Boxes at the top and at the bottom.

// The G4Box needs half the length of a side as an input argument!


// Beginning Block 21-----------------------------------

G4LogicalVolume *block21_Logical;
G4Box *block21; 
G4VisAttributes *block21vis;

block21 =
	    new G4Box("block21",10*cm,40*cm, ((100. * mm)+10 * block_z )*0.5);

  block21_Logical =
	    new G4LogicalVolume(block21, Fe, "block21_Logical", 0, 0, 0);
	block21vis = new G4VisAttributes(blue);
	block21_Logical->SetVisAttributes(block21vis);

	new G4PVPlacement(0, G4ThreeVector(25.*cm, -15.*cm,-distcollimatortotarget-
	(10 * block_z+10.*cm)*0.5 ), block21_Logical, "block", world_log, 0, 0);
//End of Block21--------------------------------------------------------
// /control/execute init_vis.mac
//Beginning Block 22-----------------------------------------
G4LogicalVolume *block22_Logical;
G4Box *block22; 
G4VisAttributes *block22vis;

block22 =
	    new G4Box("block22",15*cm,5*cm, ((100. * mm)+10 * block_z )*0.5);

  block22_Logical =
	    new G4LogicalVolume(block22, Fe, "block22_Logical", 0, 0, 0);
	block22vis = new G4VisAttributes(blue);
	block22_Logical->SetVisAttributes(block22vis);
	
		new G4PVPlacement(0, G4ThreeVector(0, 20.*cm,-distcollimatortotarget-
	(10 * block_z+10.*cm)*0.5 ), block22_Logical, "block", world_log, 0, 0);
//End Block 22----------------------------------------

//Beginning Block 23-------------------------------------
G4LogicalVolume *block23_Logical;
G4Box *block23; 
G4VisAttributes *block23vis;

block23 =
	    new G4Box("block22",10*cm,40*cm, ((100. * mm)+10 * block_z )*0.5);

  block23_Logical =
	    new G4LogicalVolume(block23, Fe, "block23_Logical", 0, 0, 0);
	block23vis = new G4VisAttributes(blue);
	block23_Logical->SetVisAttributes(block23vis);
	
		new G4PVPlacement(0, G4ThreeVector(-25.*cm, -15.*cm,-distcollimatortotarget-
	(10 * block_z+10.*cm)*0.5 ), block23_Logical, "block", world_log, 0, 0);
//End Block 23---------------------------------------

//Beginning Block 24-------------------------------------
G4LogicalVolume *block24_Logical;
G4Box *block24; 
G4VisAttributes *block24vis;

block24 =
	    new G4Box("block24",15*cm,20*cm, ((100. * mm)+10 * block_z )*0.5);

  block24_Logical =
	    new G4LogicalVolume(block24, Fe, "block24_Logical", 0, 0, 0);
	block24vis = new G4VisAttributes(blue);
	block24_Logical->SetVisAttributes(block24vis);
	
		new G4PVPlacement(0, G4ThreeVector(0, -35.*cm,-distcollimatortotarget-
	(10 * block_z+10.*cm)*0.5 ), block24_Logical, "block", world_log, 0, 0);
//End Block 24---------------------------------------




//Right Leadwall (From the perspective of the wall)
// The Wall stars at the same z-coordinate as the collimator and is 20cm long for 1m
// in y-direction it starts 10cm below the collimator and ends 10cm above it.
//Beginning Block 31-------------------------------------
G4LogicalVolume *block31_Logical;
G4Box *block31; 
G4VisAttributes *block31vis;
// DIE WAND 3Xer sind noch net feritg!!!!!!!!!!!
block31 =
	    new G4Box("block31",10*cm,40*cm, ((-50. * mm)+10 * block_z )*0.5);

  block31_Logical =
	    new G4LogicalVolume(block31, Pb, "block31_Logical", 0, 0, 0);
	block31vis = new G4VisAttributes(leadcolor);
	block31_Logical->SetVisAttributes(block31vis);
	
		new G4PVPlacement(0, G4ThreeVector(-45*cm, -15*cm,-distcollimatortotarget-
	(10 * block_z-5.*cm)*0.5 ), block31_Logical, "block", world_log, 0, 0);
//End Block 31---------------------------------------


//Above the iron shielding sits a layer of lead that is 30cm thick
//Beginning Block 41-------------------------------------
G4LogicalVolume *block41_Logical;
G4Box *block41; 
G4VisAttributes *block41vis;

block41 =
	    new G4Box("block41",55*cm,15*cm, 190*cm);

  block41_Logical =
	    new G4LogicalVolume(block41, Pb, "block41_Logical", 0, 0, 0);
	block41vis = new G4VisAttributes(leadcolor);
	block41_Logical->SetVisAttributes(block41vis);
	
		new G4PVPlacement(0, G4ThreeVector(0, 40.*cm,-distcollimatortotarget), block41_Logical, "block", world_log, 0, 0);
//End Block 41---------------------------------------


//All the ground below the level of the lowest iron shielding is assumed to be lead.
//Beginning Block 51-------------------------------------
G4LogicalVolume *block51_Logical;
G4Box *block51; 
G4VisAttributes *block51vis;

block51 =
	    new G4Box("block51",55*cm,10*cm, 190*cm);

  block51_Logical =
	    new G4LogicalVolume(block51, Pb, "block51_Logical", 0, 0, 0);
	block51vis = new G4VisAttributes(leadcolor);
	block51_Logical->SetVisAttributes(block51vis);
	
		new G4PVPlacement(0, G4ThreeVector(0, -65.*cm,-distcollimatortotarget), block51_Logical, "block", world_log, 0, 0);
//End Block 51---------------------------------------


	return world_phys;
}

void DetectorConstruction::ConstructSDandField() {

}
