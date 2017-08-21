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

	G4double world_x = 0.5 * m;
	G4double world_y = 0.5 * m;
	G4double world_z = 1.5 * m;

	G4Box *world_dim = new G4Box("world_dim", world_x, world_y, world_z);

	G4LogicalVolume *world_log =
	    new G4LogicalVolume(world_dim, AIR, "world_log");

	world_log->SetVisAttributes(G4VisAttributes::GetInvisible());

	G4VPhysicalVolume *world_phys =
	    new G4PVPlacement(0, G4ThreeVector(), world_log, "world", 0, false, 0);

	/************************* Copper Collimator *****************/

	// Consists of ten 300mm * 300mm * 100mm Cu blocks arranged to form a
	// Collimator of 955mm length. The total length is not
	// 1000mm because the front part of nine out of ten Cu blocks has been
	// carved off so that they can be fit into each other (Like a LEGO toy brick).
	// The collimator diameter is chosen to increase step-wise from 12mm at
	// radiator target position to 20 mm at target position.

	G4double block_x = 300. * mm;
	G4double block_y = 300. * mm;
	G4double block_z = 95. * mm;

	G4double carving_depth = 5. * mm;
	G4double carving_radius = 150. * mm;

	G4Box *block = new G4Box("block", block_x / 2, block_y / 2, block_z / 2);
	G4Tubs *carving = new G4Tubs("carving", 0. * mm, carving_radius,
	                             carving_depth / 2, 0., twopi);

	G4UnionSolid *carving1 = new G4UnionSolid(
	    "carving1", block, carving, 0,
	    G4ThreeVector(0. * mm, 0. * mm, block_z / 2 + carving_depth / 2));
	G4SubtractionSolid *carving2 = new G4SubtractionSolid(
	    "carving2", carving1, carving, 0,
	    G4ThreeVector(0. * mm, 0. * mm, -block_z / 2 + carving_depth / 2));
	G4SubtractionSolid *carving3 = new G4SubtractionSolid(
	    "carving3", block, carving, 0,
	    G4ThreeVector(0. * mm, 0. * mm, -block_z / 2 + carving_depth / 2));

	// _________________________ Block 1 _________________________

	G4double hole1_radius = 12. * mm;

	G4Tubs *hole1 =
	    new G4Tubs("hole1", 0. * mm, hole1_radius, 200. * mm, 0., twopi);

	G4SubtractionSolid *block1 =
	    new G4SubtractionSolid("block2", carving1, hole1);

	G4LogicalVolume *block1_Logical =
	    new G4LogicalVolume(block1, Cu, "block1_Logical", 0, 0, 0);
	G4VisAttributes *block1vis = new G4VisAttributes(light_orange);
	block1_Logical->SetVisAttributes(block1vis);

	new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), block1_Logical, "block1",
	                  world_log, 0, 0);

	// _________________________ Block 2 _________________________

	G4double hole2_radius = 12.88 * mm;

	G4Tubs *hole2 =
	    new G4Tubs("hole2", 0. * mm, hole2_radius, 200. * mm, 0., twopi);

	G4SubtractionSolid *block2 =
	    new G4SubtractionSolid("block2", carving2, hole2);

	G4LogicalVolume *block2_Logical =
	    new G4LogicalVolume(block2, Cu, "block1_Logical", 0, 0, 0);

	G4VisAttributes *block2vis = new G4VisAttributes(orange);
	block2_Logical->SetVisAttributes(block2vis);

	new G4PVPlacement(0, G4ThreeVector(0., 0., 2 * block_z / 2), block2_Logical,
	                  "block2", world_log, 0, 0);

	// _________________________ Block 3 _________________________

	G4double hole3_radius = 13.76 * mm;

	G4Tubs *hole3 =
	    new G4Tubs("hole3", 0. * mm, hole3_radius, 200. * mm, 0., twopi);

	G4SubtractionSolid *block3 =
	    new G4SubtractionSolid("block3", carving2, hole3);

	G4LogicalVolume *block3_Logical =
	    new G4LogicalVolume(block3, Cu, "block3_Logical", 0, 0, 0);

	G4VisAttributes *block3vis = new G4VisAttributes(light_orange);
	block3_Logical->SetVisAttributes(block3vis);

	new G4PVPlacement(0, G4ThreeVector(0., 0., 4 * block_z / 2), block3_Logical,
	                  "block3", world_log, 0, 0);

	// _________________________ Block 4 _________________________

	G4double hole4_radius = 14.64 * mm;

	G4Tubs *hole4 =
	    new G4Tubs("hole4", 0. * mm, hole4_radius, 200. * mm, 0., twopi);

	G4SubtractionSolid *block4 =
	    new G4SubtractionSolid("block4", carving2, hole4);

	G4LogicalVolume *block4_Logical =
	    new G4LogicalVolume(block4, Cu, "block4_Logical", 0, 0, 0);

	G4VisAttributes *block4vis = new G4VisAttributes(orange);
	block4_Logical->SetVisAttributes(block4vis);

	new G4PVPlacement(0, G4ThreeVector(0., 0., 6 * block_z / 2), block4_Logical,
	                  "block4", world_log, 0, 0);

	// _________________________ Block 5 _________________________

	G4double hole5_radius = 15.52 * mm;

	G4Tubs *hole5 =
	    new G4Tubs("hole5", 0. * mm, hole5_radius, 200. * mm, 0., twopi);

	G4SubtractionSolid *block5 =
	    new G4SubtractionSolid("block5", carving2, hole5);

	G4LogicalVolume *block5_Logical =
	    new G4LogicalVolume(block5, Cu, "block5_Logical", 0, 0, 0);

	G4VisAttributes *block5vis = new G4VisAttributes(light_orange);
	block5_Logical->SetVisAttributes(block5vis);

	new G4PVPlacement(0, G4ThreeVector(0., 0., 8 * block_z / 2), block5_Logical,
	                  "block5", world_log, 0, 0);

	// _________________________ Block 6 _________________________

	G4double hole6_radius = 16.40 * mm;

	G4Tubs *hole6 =
	    new G4Tubs("hole6", 0. * mm, hole6_radius, 200. * mm, 0., twopi);

	G4SubtractionSolid *block6 =
	    new G4SubtractionSolid("block6", carving2, hole6);

	G4LogicalVolume *block6_Logical =
	    new G4LogicalVolume(block6, Cu, "block6_Logical", 0, 0, 0);

	G4VisAttributes *block6vis = new G4VisAttributes(orange);
	block6_Logical->SetVisAttributes(block6vis);

	new G4PVPlacement(0, G4ThreeVector(0., 0., 10 * block_z / 2),
	                  block6_Logical, "block6", world_log, 0, 0);

	// _________________________ Block 7 _________________________

	G4double hole7_radius = 17.28 * mm;

	G4Tubs *hole7 =
	    new G4Tubs("hole7", 0. * mm, hole7_radius, 200. * mm, 0., twopi);

	G4SubtractionSolid *block7 =
	    new G4SubtractionSolid("block7", carving2, hole7);

	G4LogicalVolume *block7_Logical =
	    new G4LogicalVolume(block7, Cu, "block7_Logical", 0, 0, 0);

	G4VisAttributes *block7vis = new G4VisAttributes(light_orange);
	block7_Logical->SetVisAttributes(block7vis);

	new G4PVPlacement(0, G4ThreeVector(0., 0., 12 * block_z / 2),
	                  block7_Logical, "block7", world_log, 0, 0);

	// _________________________ Block 8 _________________________

	G4double hole8_radius = 18.16 * mm;

	G4Tubs *hole8 =
	    new G4Tubs("hole8", 0. * mm, hole8_radius, 200. * mm, 0., twopi);

	G4SubtractionSolid *block8 =
	    new G4SubtractionSolid("block8", carving2, hole8);

	G4LogicalVolume *block8_Logical =
	    new G4LogicalVolume(block8, Cu, "block8_Logical", 0, 0, 0);

	G4VisAttributes *block8vis = new G4VisAttributes(orange);
	block8_Logical->SetVisAttributes(block8vis);

	new G4PVPlacement(0, G4ThreeVector(0., 0., 14 * block_z / 2),
	                  block8_Logical, "block8", world_log, 0, 0);

	// _________________________ Block 9 _________________________

	G4double hole9_radius = 19.04 * mm;

	G4Tubs *hole9 =
	    new G4Tubs("hole9", 0. * mm, hole9_radius, 200. * mm, 0., twopi);

	G4SubtractionSolid *block9 =
	    new G4SubtractionSolid("block9", carving2, hole9);

	G4LogicalVolume *block9_Logical =
	    new G4LogicalVolume(block9, Cu, "block9_Logical", 0, 0, 0);

	G4VisAttributes *block9vis = new G4VisAttributes(light_orange);
	block9_Logical->SetVisAttributes(block9vis);

	new G4PVPlacement(0, G4ThreeVector(0., 0., 16 * block_z / 2),
	                  block9_Logical, "block9", world_log, 0, 0);

	// _________________________ Block 10 _________________________

	G4double hole10_radius = 20. * mm;

	G4Tubs *hole10 =
	    new G4Tubs("hole10", 0. * mm, hole10_radius, 200. * mm, 0., twopi);

	G4SubtractionSolid *block10 =
	    new G4SubtractionSolid("block10", carving3, hole10);

	G4LogicalVolume *block10_Logical =
	    new G4LogicalVolume(block10, Cu, "block10_Logical", 0, 0, 0);

	G4VisAttributes *block10vis = new G4VisAttributes(orange);
	block10_Logical->SetVisAttributes(block10vis);

	new G4PVPlacement(0, G4ThreeVector(0., 0., 18 * block_z / 2),
	                  block10_Logical, "block10", world_log, 0, 0);

	return world_phys;
}

void DetectorConstruction::ConstructSDandField() {

}
