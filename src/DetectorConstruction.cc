#include "DetectorConstruction.hh"

// Sensitive Detectors
#include "EnergyDepositionSD.hh"
#include "ParticleSD.hh"
#include "SecondarySD.hh"
//Detectors
#include "Germanium1_TUD.hh"
#include "Germanium2_TUD.hh"
#include "Polarimeter_TUD.hh"
#include "BGO.hh"
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


// These boxes are declared here for later use. They are used to make tubs out of them.
	G4Box *blockwithouthole = new G4Box("blockwithouthole", block_x / 2, block_y / 2, block_z / 2);
	G4Box *tubeblock1 = new G4Box("tubeblock1",15.5*cm,15.5*cm,1*cm);
    G4Box *tubeblock2 = new G4Box("tubeblock2",15.5*cm,15.5*cm,8.5*cm);
    G4Box *tubeblock3 = new G4Box("tubeblock3",15.5*cm,15.5*cm,5.25*cm);
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


/************************* Lead Shielding *****************/

//Right Leadwall (From the perspective of the wall)
// The Wall stars at the same z-coordinate as the collimator and is 20cm long for 1m
// in y-direction it starts 10cm below the collimator and ends 10cm above it.
//Beginning Block 31-------------------------------------
G4LogicalVolume *block31_Logical;
G4Box *block31; 
G4VisAttributes *block31vis;

block31 =
	    new G4Box("block31",10*cm,40*cm, 50*cm);

  block31_Logical =
	    new G4LogicalVolume(block31, Pb, "block31_Logical", 0, 0, 0);
	block31vis = new G4VisAttributes(leadcolor);
	block31_Logical->SetVisAttributes(block31vis);
	
		new G4PVPlacement(0, G4ThreeVector(-45*cm, -15*cm,-distcollimatortotarget-
	95*cm+50*cm), block31_Logical, "block", world_log, 0, 0);
//End Block 31---------------------------------------

//Second part of the wall is only 5cm thick and continues for 81cm until the back wall.
//Beginning Block 32-------------------------------------
G4LogicalVolume *block32_Logical;
G4Box *block32; 
G4VisAttributes *block32vis;

block32 =
	    new G4Box("block32",25*mm,40*cm, 40.5*cm);

  block32_Logical =
	    new G4LogicalVolume(block32, Pb, "block32_Logical", 0, 0, 0);
	block32vis = new G4VisAttributes(leadcolor);
	block32_Logical->SetVisAttributes(block32vis);
	
		new G4PVPlacement(0, G4ThreeVector(-37.5*cm, -15*cm,-distcollimatortotarget-
	95*cm+50*cm+90.5*cm), block32_Logical, "block", world_log, 0, 0);
//End Block 32---------------------------------------

//Above the iron shielding sits a layer of lead that is 30cm thick
//Beginning Block 41-------------------------------------
G4LogicalVolume *block41_Logical;
G4Box *block41; 
G4VisAttributes *block41vis;

block41 =
	    new G4Box("block41",55*cm+45*cm,15*cm, 90.5*cm);

  block41_Logical =
	    new G4LogicalVolume(block41, Pb, "block41_Logical", 0, 0, 0);
	block41vis = new G4VisAttributes(grey);
	block41_Logical->SetVisAttributes(block41vis);
	
		new G4PVPlacement(0, G4ThreeVector(45*cm, 40.*cm,0-distcollimatortotarget-95*cm+90.5*cm), block41_Logical, "block", world_log, 0, 0);
//End Block 41---------------------------------------


//All the ground below the level of the lowest iron shielding is assumed to be lead.
//Beginning Block 51-------------------------------------
G4LogicalVolume *block51_Logical;
G4Box *block51; 
G4VisAttributes *block51vis;

block51 =
	    new G4Box("block51",180*cm,10*cm, 90.5*cm);

  block51_Logical =
	    new G4LogicalVolume(block51, Pb, "block51_Logical", 0, 0, 0);
	block51vis = new G4VisAttributes(grey);
	block51_Logical->SetVisAttributes(block51vis);
	
		new G4PVPlacement(0, G4ThreeVector(0, -65.*cm,-distcollimatortotarget-95*cm+90.5*cm), block51_Logical, "block", world_log, 0, 0);
//End Block 51---------------------------------------

//Chamber on the right side
// The first wall. It is 6cm in front of the Cone, 10cm deep and 25cm high. It stars
// directly at the thinner wall Element from Block32 
//Beginning Block 61-------------------------------------
G4LogicalVolume *block61_Logical;
G4Box *block61; 
G4VisAttributes *block61vis;

block61 =
	    new G4Box("block61",40*cm,12.5*cm, 5*cm);

  block61_Logical =
	    new G4LogicalVolume(block61, Pb, "block61_Logical", 0, 0, 0);
	block61vis = new G4VisAttributes(leadcolor);
	block61_Logical->SetVisAttributes(block61vis);
	
		new G4PVPlacement(0, G4ThreeVector(-40*cm-40*cm, -15*cm-40*cm+12.5*cm,-distcollimatortotarget-
	95*cm+5*cm+90.5*cm+27*cm), block61_Logical, "block", world_log, 0, 0);
//End Block 61---------------------------------------

// The second wall. It is 6 behind the Cone, 10cm deep and 25cm high. It stars
// directly at the thinner wall Element from Block32 
//Beginning Block 62-------------------------------------
G4LogicalVolume *block62_Logical;
G4Box *block62; 
G4VisAttributes *block62vis;

block62 =
	    new G4Box("block62",40*cm,12.5*cm, 5*cm);

  block62_Logical =
	    new G4LogicalVolume(block62, Pb, "block62_Logical", 0, 0, 0);
	block62vis = new G4VisAttributes(leadcolor);
	block62_Logical->SetVisAttributes(block62vis);
	
		new G4PVPlacement(0, G4ThreeVector(-40*cm-40*cm, -15*cm-40*cm+12.5*cm,-distcollimatortotarget-
	95*cm+5*cm+90.5*cm+27*cm+43*cm), block62_Logical, "block", world_log, 0, 0);
//End Block 62---------------------------------------


//Chamber on the left side
// Right Wall leading into the chamber. It is 40cm high, 10cm thick 
//and as long as the collimator (95cm)
//Beginning Block 71-------------------------------------
G4LogicalVolume *block71_Logical;
G4Box *block71; 
G4VisAttributes *block71vis;

block71 =
	    new G4Box("block71",10*cm,40*cm, 47.5*cm);

  block71_Logical =
	    new G4LogicalVolume(block71, Pb, "block71_Logical", 0, 0, 0);
	block71vis = new G4VisAttributes(leadcolor);
	block71_Logical->SetVisAttributes(block71vis);
	
		new G4PVPlacement(0, G4ThreeVector(45.*cm, -15.*cm,-distcollimatortotarget-
	95*cm+47.5*cm), block71_Logical, "block", world_log, 0, 0);
//End Block 71---------------------------------------

//Left Wall closing the chamber off. It starts 37cm behind the right wall and runs
// at a constant thickness of 10cm towards the end of the chamber.

//Beginning Block 72-------------------------------------
G4LogicalVolume *block72_Logical;
G4Box *block72; 
G4VisAttributes *block72vis;

block72 =
	    new G4Box("block72",10*cm,40*cm, 72*cm);

  block72_Logical =
	    new G4LogicalVolume(block72, Pb, "block72_Logical", 0, 0, 0);
	block72vis = new G4VisAttributes(leadcolor);
	block72_Logical->SetVisAttributes(block72vis);
	
		new G4PVPlacement(0, G4ThreeVector(165.*cm, -15.*cm,-distcollimatortotarget-
	95*cm+37*cm+72*cm), block72_Logical, "block", world_log, 0, 0);
//End Block 72---------------------------------------

// A Block auf Lead sitting in front of the entrance of the chamber.
// It is a meter Wide, 30cm deep and 5cm high
//Beginning Block 73-------------------------------------
G4LogicalVolume *block73_Logical;
G4Box *block73; 
G4VisAttributes *block73vis;

block73 =
	    new G4Box("block73",50*cm,2.5*cm, 15*cm);

  block73_Logical =
	    new G4LogicalVolume(block73, Pb, "block73_Logical", 0, 0, 0);
	block73vis = new G4VisAttributes(leadcolor);
	block73_Logical->SetVisAttributes(block73vis);
	
		new G4PVPlacement(0, G4ThreeVector(55.*cm+50*cm, -55.*cm+2.5*cm,-distcollimatortotarget-
	95*cm+37*cm+15*cm), block73_Logical, "block", world_log, 0, 0);
//End Block 73---------------------------------------

// Lead behind the cones.It is is 19cm deep and has a 90 degree angle with the wall behind it.


//Beginning Block 74-------------------------------------
G4LogicalVolume *block74_Logical;
G4Box *block74; 
G4VisAttributes *block74vis;

block74 =
	    new G4Box("block74",30*cm,40*cm, 9.5*cm); //30cm Dicke nur geschätzt...Ideen?

  block74_Logical =
	    new G4LogicalVolume(block74, Pb, "block74_Logical", 0, 0, 0);
	block74vis = new G4VisAttributes(leadcolor);
	block74_Logical->SetVisAttributes(block74vis);
	
		new G4PVPlacement(0, G4ThreeVector(155*cm-130*cm,-15*cm, 181*cm-distcollimatortotarget-
	95*cm-9.5*cm), block74_Logical, "block", world_log, 0, 0);
//End Block 73---------------------------------------


//Wall behind target
//Beginning Block 81-------------------------------------
G4LogicalVolume *block81_Logical;
G4Box *block81; 
G4VisAttributes *block81vis;

block81 =
	    new G4Box("block81",160*cm,100*cm,15*cm);

  block81_Logical =
	    new G4LogicalVolume(block81, Pb, "block81_Logical", 0, 0, 0);
	block81vis = new G4VisAttributes(grey);
	block81_Logical->SetVisAttributes(block81vis);
	
		new G4PVPlacement(0, G4ThreeVector(0, 0,181*cm-distcollimatortotarget-
	95*cm+15*cm ), block81_Logical, "block", world_log, 0, 0);
//End Block 81---------------------------------------



//Beginning tube1-------------------------------------
//G4LogicalVolume *tube1_Logical;
//G4Box *tube1; 
//G4VisAttributes *tube1vis;

//tube1 =
	    //new G4Box("tube1",10*cm,10*cm,10*cm);

  //tube1_Logical =
	    //new G4LogicalVolume(tube1, Pb, "tube1_Logical", 0, 0, 0);
	//tube1vis = new G4VisAttributes(light_orange);
	//tube1_Logical->SetVisAttributes(tube1vis);
	
		//new G4PVPlacement(0, G4ThreeVector(2*m, 2*m,2*m), tube1_Logical, "block", world_log, 0, 0);
//End Block tube1---------------------------------------

 G4double detectordistance = 265.*mm;
 
   Germanium1_TUD* germaniumDetector1=new Germanium1_TUD("Germanium1_TUD");

  G4double g1_Distance = -(detectordistance + germaniumDetector1->Get_Length());
  
  G4double g1_theta=90.*deg;
  G4double g1_phi=175.*deg;
  G4double g1_Angle=g1_phi-90.*deg;

  G4RotationMatrix* rm1=new G4RotationMatrix();
  rm1->rotateY(g1_Angle);

  G4ThreeVector germaniumDetector1_Position=G4ThreeVector(g1_Distance*sin(g1_theta)*cos(g1_phi), g1_Distance*cos(g1_theta), g1_Distance*sin(g1_theta)*sin(g1_phi));

  G4LogicalVolume* germaniumDetector1_Logical=germaniumDetector1->Get_Logical();

 // crystal1_Logical = germaniumDetector1->Get_Logical();

  new G4PVPlacement(rm1, germaniumDetector1_Position, germaniumDetector1_Logical, "Germanium1_TUD", world_log, false, 0);
G4double collimator_Radius=2.5*cm;

  //G4double cu_Thickness=1.*cm;
  //G4double pb_Thickness=0.85*3.*cm;

  //G4double g1_Filter_Distance;
  
  //g1_Filter_Distance=-(detectordistance - BGO::bgo_Collimator_Length - cu_Thickness/2);
  //G4ThreeVector germaniumDetector1_Filter_Position=G4ThreeVector(g1_Filter_Distance*sin(g1_theta)*cos(g1_phi), g1_Filter_Distance*cos(g1_theta), g1_Filter_Distance*sin(g1_theta)*sin(g1_phi));
  
  //G4Tubs* filter1_Cu_Solid=new G4Tubs("Filter_Copper_Solid", 0.*cm, collimator_Radius, cu_Thickness/2, 0.*deg, 360.*deg);

  //G4LogicalVolume* filter1_Cu_Logical=new G4LogicalVolume(filter1_Cu_Solid, Cu,"filter1_Cu_Logical", 0, 0, 0);

  //filter1_Cu_Logical->SetVisAttributes(orange);
  
  //new G4PVPlacement(rm1,germaniumDetector1_Filter_Position, filter1_Cu_Logical, "Filter1_Copper", world_log , false,0);
  
  //g1_Filter_Distance=-(detectordistance - BGO::bgo_Collimator_Length - cu_Thickness - pb_Thickness/2);
  //germaniumDetector1_Filter_Position=G4ThreeVector(g1_Filter_Distance*sin(g1_theta)*cos(g1_phi), g1_Filter_Distance*cos(g1_theta), g1_Filter_Distance*sin(g1_theta)*sin(g1_phi));

  //G4Tubs* filter1_Pb_Solid=new G4Tubs("filter1_Pb_Solid", 0.*cm, collimator_Radius, pb_Thickness/2, 0.*deg, 360.*deg);

  //G4LogicalVolume* filter1_Pb_Logical=new G4LogicalVolume(filter1_Pb_Solid, Pb,"filter1_Pb_Logical", 0, 0, 0);
  
  //filter1_Pb_Logical->SetVisAttributes(grey);

  //new G4PVPlacement(rm1, germaniumDetector1_Filter_Position, filter1_Pb_Logical, "Filter1_Lead", world_log , false, 0);









// /control/execute init_vis.mac

	return world_phys;
}

void DetectorConstruction::ConstructSDandField() {

}
