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
#include "RadiatorTarget.hh"

// Materials
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "Materials.hh"

// Geometry
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SubtractionSolid.hh"
#include "G4TwoVector.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4GenericTrap.hh"
#include "G4UnionSolid.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"
#include "G4Sphere.hh"

// Units
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include <sstream>
#include <vector>

using std::stringstream;
using std::vector;

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
	G4Material *Al= man->FindOrBuildMaterial("G4_Al");

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
//These Variables are used to correct the position of all boxes, 
//that are not defined in spherical coordinates
G4double trans_x = 0. * mm; 
G4double trans_y = 0. * mm; 
G4double trans_z = -400. * mm; 

// These boxes are declared here for later use. They are used to make tubs out of them.
	G4Box *blockwithouthole = new G4Box("blockwithouthole", block_x / 2, block_y / 2, block_z / 2);
	 
	 G4double disttoendblock32= -40*cm+trans_x ;
	 
	 //detectordistance1 should be divided by cosine. For 5 deg the difference is less than 1 in 100000
	 //G4double detectordistance1 = abs(disttoendblock32)-(192)*mm;//this is the length of bgo
	 G4double detectordistance1 = 265.*mm;//this is the length of bgo
	 G4double detectordistance2 = 265.*mm;
	 G4double poldistance = 265.*mm;
	 

	
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

	new G4PVPlacement(0, G4ThreeVector(trans_x , trans_y, -distcollimatortotarget-(i+0.5) * block_z+trans_z), blocks_Logical[i], "block",
	                  world_log, 0, 0);}

/************************* Radiator targets *****************/

	// Distance from the entrance of the collimator to the side of the radiator target holder that faces the collimator
	
	G4double radiator_holder1_to_collimator = 150.*mm;
	G4double radiator_holder2_to_collimator = 120.*mm;

//	G4double collimator_entrance_z = -distcollimatortotarget + trans_z - 11.*block_z;
//
//	G4Box *box_Solid = new G4Box("box_Solid", 100.*mm, 100.*mm, 100.*mm);
//	G4LogicalVolume *box_Logical = new G4LogicalVolume(box_Solid, AIR, "box_Logical");
//	box_Logical->SetVisAttributes(yellow);
//
//	new G4PVPlacement(0, G4ThreeVector(0., 0., -distcollimatortotarget + trans_z -10.*mm - 11.*block_z), box_Logical, "box", world_log, false, 0);

	RadiatorTarget *radiatorTarget1 = new RadiatorTarget(3.*mm, "Cu", "Target_1");
	G4LogicalVolume *radiator_Holder1_Logical = radiatorTarget1->Get_Logical();

	new G4PVPlacement(0, G4ThreeVector(0., radiatorTarget1->Get_Window_Position(), -distcollimatortotarget + trans_z -11.*block_z - radiator_holder1_to_collimator - radiatorTarget1->Get_Z()), radiator_Holder1_Logical, "radiator_Holder1", world_log, false, 0);

/************************* Red Boxes for Orientation *****************/

// These boxes are purely for orientation and should be commented out
// before running a simulation
//Beginning Block 025-------------------------------------
G4LogicalVolume *block25_Logical;
G4Box *block25; 
G4VisAttributes *block25vis;

block25 =
	    new G4Box("block25",5*cm,2*cm, 2*cm);

  block25_Logical =
	    new G4LogicalVolume(block25, Fe, "block25_Logical", 0, 0, 0);
	block25vis = new G4VisAttributes(red);
	block25_Logical->SetVisAttributes(block25vis);

		new G4PVPlacement(0, G4ThreeVector(0 ,0, 0), block25_Logical, "block", world_log, 0, 0);
//End Block 025---------------------------------------

//Block around target
//Beginning Block 026-------------------------------------
G4LogicalVolume *block26_Logical;
G4Box *block26; 
G4VisAttributes *block26vis;

block26 =
	    new G4Box("block26",1*cm,1*cm, 1*cm);

  block26_Logical =
	    new G4LogicalVolume(block26, Fe, "block26_Logical", 0, 0, 0);
	block26vis = new G4VisAttributes(red);
	block26_Logical->SetVisAttributes(block26vis);

	new G4PVPlacement(0, G4ThreeVector(0, 0,0 ), block26_Logical, "block", world_log, 0, 0);
//End Block 026---------------------------------------

//Orientation Sphere
G4LogicalVolume *Sphere_Logical;
G4Sphere *Sphere; 
G4VisAttributes *Spherevis;

Sphere =
new G4Sphere("Sphere",
              0.*cm,
             40*cm,
              0*deg,
             360*deg,
             0*deg,
       180*deg ) ;
             
             Sphere_Logical =
	    new G4LogicalVolume(Sphere, Fe, "Sphere_Logical", 0, 0, 0);
	Spherevis = new G4VisAttributes(red);
	Sphere_Logical->SetVisAttributes(Spherevis);

	new G4PVPlacement(0, G4ThreeVector(0, 0,0 ), Sphere_Logical, 
	"Sphere", world_log, 0, 0);

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

	new G4PVPlacement(0, G4ThreeVector(25.*cm+trans_x , -15.*cm+trans_y,-distcollimatortotarget-
	(10 * block_z+10.*cm)*0.5+trans_z ), block21_Logical, "block", world_log, 0, 0);
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
	
		new G4PVPlacement(0, G4ThreeVector(trans_x , 20.*cm+trans_y,-distcollimatortotarget-
	(10 * block_z+10.*cm)*0.5+trans_z ), block22_Logical, "block", world_log, 0, 0);
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
	
		new G4PVPlacement(0, G4ThreeVector(-25.*cm+trans_x , -15.*cm+trans_y,-distcollimatortotarget-
	(10 * block_z+10.*cm)*0.5+trans_z ), block23_Logical, "block", world_log, 0, 0);
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
	
		new G4PVPlacement(0, G4ThreeVector(trans_x , -35.*cm+trans_y,-distcollimatortotarget-
	(10 * block_z+10.*cm)*0.5 +trans_z), block24_Logical, "block", world_log, 0, 0);
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
	
		new G4PVPlacement(0, G4ThreeVector(-45*cm+trans_x , -15*cm+trans_y,-distcollimatortotarget-
	95*cm+50*cm+trans_z), block31_Logical, "block", world_log, 0, 0);
//End Block 31---------------------------------------


BGO* bgo1 = new BGO(); //From here on we need the BGO for the SubtractionSolid
 
//Second part of the wall is only 5cm thick and continues for 81cm until the back wall.

//Beginning Block 32-------------------------------------
G4LogicalVolume *block32_Logical;
G4Box *block32; 
G4VisAttributes *block32vis;
G4UnionSolid *subcase_Solid11;
G4Tubs *subcase_Solid12;

G4RotationMatrix yRot45degblock32;
yRot45degblock32.rotateY(M_PI*95/180.*rad);
G4ThreeVector translationblock32(0,0,0);
//G4RotationMatrix yRot45degblock32;
//yRot45degblock32.rotateY(M_PI*95/180.*rad);

G4Tubs *subcase_Solid1;
G4SubtractionSolid *block32minusDetector1;	
G4SubtractionSolid *block32minusDetector1andhole;	
//The distance from the target to other side of block32 is used here and 
// in the defintion of the detector1 



G4UnionSolid* Al_Solid1 = bgo1->Get_Al_Case_Solid();
subcase_Solid11= Al_Solid1;
block32 =
	    new G4Box("block32",175*mm,40*cm, 40.5*cm);
block32minusDetector1=
//new G4SubtractionSolid("block32-subcase_Solid11",block32,subcase_Solid11,
//&yRot45degblock32,translationblock32);
new G4SubtractionSolid("block32-subcase_Solid11",block32,subcase_Solid11, &yRot45degblock32, translationblock32);

subcase_Solid12= new G4Tubs("subcase_Solid12",
 0, 50*mm, 115*mm/2, 0. * deg, 360. * deg);


block32minusDetector1andhole=
new G4SubtractionSolid("block32minusDetector1-subcase_Solid12",block32minusDetector1,subcase_Solid12,
&yRot45degblock32,translationblock32);



//  block32_Logical =
//	    new G4LogicalVolume(block32minusDetector1andhole, Pb, "block32_Logical", 0, 0, 0);
  block32_Logical =
	    new G4LogicalVolume(block32minusDetector1, Pb, "block32_Logical", 0, 0, 0);
	block32vis = new G4VisAttributes(leadcolor);
	block32_Logical->SetVisAttributes(block32vis);
	
		new G4PVPlacement(0, G4ThreeVector(disttoendblock32+17.5*cm , -15*cm+trans_y,-distcollimatortotarget-
	95*cm+50*cm+90.5*cm+trans_z), block32_Logical, "block", world_log, 0, 0);


//End Block 32---------------------------------------

//Small intermediate Block to fill a Gap between block32 and block31
//Beginning Block 33-------------------------------------
G4LogicalVolume *block33_Logical;
G4Box *block33; 
G4VisAttributes *block33vis;

block33 =
	    new G4Box("block33",15*cm,40*cm, 2.5*cm);

  block33_Logical =
	    new G4LogicalVolume(block33, Pb, "block33_Logical", 0, 0, 0);
	block33vis = new G4VisAttributes(leadcolor);
	block33_Logical->SetVisAttributes(block33vis);
	
		new G4PVPlacement(0, G4ThreeVector(-20*cm+trans_x , -15*cm+trans_y,
		2.5*cm-distcollimatortotarget+trans_z), block33_Logical, "block", world_log, 0, 0);
//End Block 33---------------------------------------


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
	
		new G4PVPlacement(0, G4ThreeVector(45*cm+trans_x , 40.*cm+trans_y,0-distcollimatortotarget-95*cm+90.5*cm+trans_z), block41_Logical, "block", world_log, 0, 0);
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
	
		new G4PVPlacement(0, G4ThreeVector(trans_x , -65.*cm+trans_y,-distcollimatortotarget-95*cm+90.5*cm+trans_z), block51_Logical, "block", world_log, 0, 0);
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
	
		new G4PVPlacement(0, G4ThreeVector(-40*cm-40*cm+trans_x , -15*cm-40*cm+12.5*cm+trans_y,-distcollimatortotarget-
	95*cm+5*cm+90.5*cm+27*cm+trans_z), block61_Logical, "block", world_log, 0, 0);
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
	
		new G4PVPlacement(0, G4ThreeVector(-40*cm-40*cm+trans_x , -15*cm-40*cm+12.5*cm+trans_y,-distcollimatortotarget-
	95*cm+5*cm+90.5*cm+27*cm+43*cm+trans_z), block62_Logical, "block", world_log, 0, 0);
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
	
		new G4PVPlacement(0, G4ThreeVector(45.*cm+trans_x , -15.*cm+trans_y,-distcollimatortotarget-
	95*cm+47.5*cm+trans_z), block71_Logical, "block", world_log, 0, 0);
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
	
		new G4PVPlacement(0, G4ThreeVector(165.*cm+trans_x , -15.*cm+trans_y,-distcollimatortotarget-
	95*cm+37*cm+72*cm+trans_z), block72_Logical, "block", world_log, 0, 0);
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
	
		new G4PVPlacement(0, G4ThreeVector(55.*cm+50*cm+trans_x , -55.*cm+2.5*cm+trans_y,-distcollimatortotarget-
	95*cm+37*cm+15*cm+trans_z), block73_Logical, "block", world_log, 0, 0);
//End Block 73---------------------------------------

// Lead behind the cones.It is is 19cm deep and has a 90 degree angle with the wall behind it.


//Beginning Block 74-------------------------------------
G4LogicalVolume *block74_Logical;
G4Box *block74; 
G4VisAttributes *block74vis;

block74 =
	    new G4Box("block74",30*cm,40*cm, 9.5*cm); 

  block74_Logical =
	    new G4LogicalVolume(block74, Pb, "block74_Logical", 0, 0, 0);
	block74vis = new G4VisAttributes(leadcolor);
	block74_Logical->SetVisAttributes(block74vis);
	
		new G4PVPlacement(0, G4ThreeVector(155*cm-130*cm+trans_x ,-15*cm+trans_y, 181*cm-distcollimatortotarget-
	95*cm-9.5*cm+trans_z), block74_Logical, "block", world_log, 0, 0);
//End Block 74---------------------------------------



// Block in front of and enclosing the Detector and Polarimeter
//Beginning Block 75-------------------------------------
G4LogicalVolume *block75_Logical;
G4Box *block751; 
G4Box *block752; 
G4UnionSolid *block75;
// DREIECK! G4Box *block753; 
G4VisAttributes *block75vis;

G4UnionSolid* blocksum_751_752;
G4GenericTrap* itsatrap;
block751 =
	    new G4Box("block751",10.1*cm,40*cm, (14.55/2)*cm); 
block752 =
	    new G4Box("block752",19*cm,40*cm, 15.6*cm); //1mm Overlap between the blocks
	    
blocksum_751_752= new G4UnionSolid("blocksum_751_752", block751, block752,0, G4ThreeVector
(9*cm, 0.,-(14.55/2)*cm+15.5*cm+36*cm-0.1*cm));


vector <G4TwoVector> vertices;

vertices.push_back (G4TwoVector(10*cm,40*cm));
vertices.push_back (G4TwoVector(10*cm,-40*cm));
vertices.push_back (G4TwoVector(-10*cm,-40*cm));
vertices.push_back (G4TwoVector(-10*cm,40*cm));
vertices.push_back (G4TwoVector(28*cm,40*cm));
vertices.push_back (G4TwoVector(28*cm,-40*cm));
vertices.push_back (G4TwoVector(-10*cm,-40*cm));
vertices.push_back (G4TwoVector(-10*cm,40*cm));


itsatrap = new G4GenericTrap("itsatrap",   (21.55/2)*cm,
         vertices);



block75= new G4UnionSolid("blocksum_751_752", blocksum_751_752, itsatrap,0, G4ThreeVector
(0, 0.,(36/2)*cm));





G4UnionSolid *subcase_Solid21;
G4Tubs *subcase_Solid22;

G4RotationMatrix yRot45degblock75;
yRot45degblock75.rotateY(M_PI*275/180.*rad);
G4ThreeVector translationblock75(0,0,0);

G4SubtractionSolid *block75minusDetector2;	
G4SubtractionSolid *block75minusDetector2andhole;	


//subcase_Solid21= Al_Solid1;
//
//block75minusDetector2=
//new G4SubtractionSolid("block75-subcase_Solid21",block75,subcase_Solid21,
//&yRot45degblock75,translationblock75);
//
//subcase_Solid22= new G4Tubs("subcase_Solid22",
// 0, 50*mm, 115*mm/2, 0. * deg, 360. * deg);
//
//
//block75minusDetector2andhole=
//new G4SubtractionSolid("block75minusDetector2-subcase_Solid22",
//block75minusDetector2,subcase_Solid22,
//&yRot45degblock75,translationblock75);
//
//  block75_Logical =
//	    new G4LogicalVolume(block75minusDetector2andhole, Pb, "block75_Logical", 0, 0, 0);
//	block75vis = new G4VisAttributes(red);
//	block75_Logical->SetVisAttributes(block75vis);
//	
//		new G4PVPlacement(0, G4ThreeVector(15*cm+trans_x ,-15*cm+trans_y, (14.55/2)*cm-distcollimatortotarget+trans_z), block75_Logical, "block", world_log, 0, 0);
//
//End Block 75---------------------------------------





////Beginning Block 76-------------------------------------
//G4LogicalVolume *block76_Logical;
//G4Box *block76; 
//G4VisAttributes *block76vis;
//G4UnionSolid *subcase_Solid21;
//G4Tubs *subcase_Solid22;

//G4RotationMatrix yRot45degblock76;
//yRot45degblock76.rotateY(M_PI*275/180.*rad);
//G4ThreeVector translationblock76(0,0,0);

//G4SubtractionSolid *block76minusDetector2;	
//G4SubtractionSolid *block76minusDetector2andhole;	


//subcase_Solid21= Al_Solid1;
//block76 =
	    //new G4Box("block76",15*cm,40*cm, 15*cm);
//block76minusDetector2=
//new G4SubtractionSolid("block76-subcase_Solid21",block76,subcase_Solid21,
//&yRot45degblock76,translationblock76);

//subcase_Solid22= new G4Tubs("subcase_Solid22",
 //0, 50*mm, 115*mm/2, 0. * deg, 360. * deg);


//block76minusDetector2andhole=
//new G4SubtractionSolid("block76minusDetector2-subcase_Solid22",
//block76minusDetector2,subcase_Solid22,
//&yRot45degblock76,translationblock76);





  //block76_Logical =
	    //new G4LogicalVolume(block76minusDetector2andhole, Pb, "block76_Logical", 0, 0, 0);
	//block76vis = new G4VisAttributes(leadcolor);
	//block76_Logical->SetVisAttributes(block76vis);
	
//new G4PVPlacement(0, G4ThreeVector(80*cm+trans_x ,-15*cm+trans_y,
//+25*cm-distcollimatortotarget+trans_z), block76_Logical, "block", world_log, 0, 0);
////End Block 76---------------------------------------





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
	
		new G4PVPlacement(0, G4ThreeVector(trans_x , trans_y,181*cm-distcollimatortotarget-
	95*cm+15*cm+trans_z ), block81_Logical, "block", world_log, 0, 0);
//End Block 81---------------------------------------




//First Detector. Sole one on the right side. Angle is 95 degrees relativ to the beam.

 
   Germanium1_TUD* germaniumDetector1=new Germanium1_TUD("Germanium1_TUD");

  G4double g1_Distance = -(detectordistance1 + germaniumDetector1->Get_Length());
  
  G4double g1_theta=90.*deg;
  G4double g1_phi=5.*deg;
  G4double g1_Angle=g1_phi-90.*deg;

  G4RotationMatrix* rm1=new G4RotationMatrix();
  rm1->rotateY(g1_Angle);

  G4ThreeVector germaniumDetector1_Position=G4ThreeVector(g1_Distance*sin(g1_theta)*cos(g1_phi), g1_Distance*cos(g1_theta), g1_Distance*sin(g1_theta)*sin(g1_phi));

  G4LogicalVolume* germaniumDetector1_Logical=germaniumDetector1->Get_Logical();
//Unterscheidet sich vom Source Code aber kompeliert nur so...
//(G4LogicalVolume* vorangestellt)  und Get_Crystal2_Logical musste durch
//Get_Logical ersetzt werden.
 //G4LogicalVolume* crystal1_Logical = germaniumDetector1->Get_Logical();

  new G4PVPlacement(rm1, germaniumDetector1_Position, germaniumDetector1_Logical, "Germanium1_TUD", world_log, false, 0);




 //BGO of Germanium1_TUD
  
  // Is already used in Block 32
  //BGO* bgo1 = new BGO();
  
  G4double bgo1_Distance = -(detectordistance1 + bgo1->Get_Length()/2);
  G4ThreeVector bgo1_Position = G4ThreeVector(bgo1_Distance*sin(g1_theta)*cos(g1_phi), bgo1_Distance*cos(g1_theta), bgo1_Distance*sin(g1_theta)*sin(g1_phi));
  
  G4LogicalVolume* bgo1_Logical = bgo1->Get_Logical();
  
 // G4LogicalVolume* bgoCrystal1_Logical = bgo1->Get_Crystal_Logical();
  
  new G4PVPlacement(rm1, bgo1_Position, bgo1_Logical, "bgo1", world_log, false, 0);

// Filter of Germanium1

  //G4double collimator_Radius=2.5*cm;

  //G4double cu_Thickness=1.*cm;
  //G4double pb_Thickness=0.85*3.*cm;

  //G4double g1_Filter_Distance;
  
  //g1_Filter_Distance=-(detectordistance1 - BGO::bgo_Collimator_Length - cu_Thickness/2);
  //G4ThreeVector germaniumDetector1_Filter_Position=G4ThreeVector(g1_Filter_Distance*sin(g1_theta)*cos(g1_phi), g1_Filter_Distance*cos(g1_theta), g1_Filter_Distance*sin(g1_theta)*sin(g1_phi));
  
  //G4Tubs* filter1_Cu_Solid=new G4Tubs("Filter_Copper_Solid", 0.*cm, collimator_Radius, cu_Thickness/2, 0.*deg, 360.*deg);

  //G4LogicalVolume* filter1_Cu_Logical=new G4LogicalVolume(filter1_Cu_Solid, Cu,"filter1_Cu_Logical", 0, 0, 0);

  //filter1_Cu_Logical->SetVisAttributes(orange);
  
  //new G4PVPlacement(rm1,germaniumDetector1_Filter_Position, filter1_Cu_Logical, "Filter1_Copper", world_log , false,0);
  
  //g1_Filter_Distance=-(detectordistance1 - BGO::bgo_Collimator_Length - cu_Thickness - pb_Thickness/2);
  //germaniumDetector1_Filter_Position=G4ThreeVector(g1_Filter_Distance*sin(g1_theta)*cos(g1_phi), g1_Filter_Distance*cos(g1_theta), g1_Filter_Distance*sin(g1_theta)*sin(g1_phi));

  //G4Tubs* filter1_Pb_Solid=new G4Tubs("filter1_Pb_Solid", 0.*cm, collimator_Radius, pb_Thickness/2, 0.*deg, 360.*deg);

  //G4LogicalVolume* filter1_Pb_Logical=new G4LogicalVolume(filter1_Pb_Solid, Pb,"filter1_Pb_Logical", 0, 0, 0);
  
  //filter1_Pb_Logical->SetVisAttributes(grey);

  //new G4PVPlacement(rm1, germaniumDetector1_Filter_Position, filter1_Pb_Logical, "Filter1_Lead", world_log , false, 0);






/***************************** Detector 2 **************************/
  // contains Germanium2 (Serial Number 72902)
  /*******************************************************************/

//
// Second Detector 130 Degree Angle
//
 Germanium2_TUD* germaniumDetector2=new Germanium2_TUD("Germanium2_TUD");
  
  G4double g2_Distance = -(detectordistance2 + germaniumDetector2->Get_Length());

  G4double g2_theta=90.*deg;
  G4double g2_phi= 140.*deg;
  G4double g2_Angle= g2_phi-90*deg;

  G4RotationMatrix* rm2=new G4RotationMatrix();
  rm2->rotateY(g2_Angle);

  G4ThreeVector germaniumDetector2_Position=G4ThreeVector(g2_Distance*sin(g2_theta)*cos(g2_phi), g2_Distance*cos(g2_theta), g2_Distance*sin(g2_theta)*sin(g2_phi));
  
  G4LogicalVolume* germaniumDetector2_Logical=germaniumDetector2->Get_Logical();
// Auch hier musste crystal2_Logical mit Pointer  gemacht werden.
   //G4LogicalVolume* crystal2_Logical = germaniumDetector2->Get_Logical();
  
  new G4PVPlacement(rm2, germaniumDetector2_Position, germaniumDetector2_Logical, "Germanium2", world_log, false, 0);

 // BGO of Germanium2
  
  BGO* bgo2 = new BGO();
  
  G4double bgo2_Distance = -(detectordistance2 + bgo2->Get_Length()/2);
  G4ThreeVector bgo2_Position = G4ThreeVector(bgo2_Distance*sin(g2_theta)*cos(g2_phi), bgo2_Distance*cos(g2_theta), bgo2_Distance*sin(g2_theta)*sin(g2_phi));
  
  G4LogicalVolume* bgo2_Logical = bgo2->Get_Logical();
  
  //G4LogicalVolume* bgoCrystal2_Logical = bgo2->Get_Crystal_Logical();
  
  new G4PVPlacement(rm2, bgo2_Position, bgo2_Logical, "bgo2", world_log, false, 0);

 // Filter of Germanium2

  //cu_Thickness=1.*cm;
  //pb_Thickness=1.73*cm;

  //G4double g2_Filter_Distance =-(detectordistance2 - BGO::bgo_Collimator_Length - cu_Thickness/2);

  //G4ThreeVector germaniumDetector2_Filter_Position=G4ThreeVector(g2_Filter_Distance*sin(g2_theta)*cos(g2_phi), g2_Filter_Distance*cos(g2_theta), g2_Filter_Distance*sin(g2_theta)*sin(g2_phi));
  //G4Tubs* filter2_Copper_Solid=new G4Tubs("filter2_Copper_Solid", 0.*cm, collimator_Radius, cu_Thickness/2, 0.*deg, 360.*deg);

  //G4LogicalVolume* filter2_Copper_Logical=new G4LogicalVolume(filter2_Copper_Solid, Cu,"filter2_Copper_Logical", 0, 0, 0);

  //filter2_Copper_Logical->SetVisAttributes(orange);
  
  //new G4PVPlacement(rm2,germaniumDetector2_Filter_Position, filter2_Copper_Logical, "filter2_Copper",world_log , false,0);
  
  //g2_Filter_Distance=-(detectordistance2 - BGO::bgo_Collimator_Length - cu_Thickness - pb_Thickness/2);

  //germaniumDetector2_Filter_Position=G4ThreeVector(g2_Filter_Distance*sin(g2_theta)*cos(g2_phi), g2_Filter_Distance*cos(g2_theta), g2_Filter_Distance*sin(g2_theta)*sin(g2_phi));

  //G4Tubs* filter2_Lead_Solid=new G4Tubs("filter2_Lead_Solid", 0.*cm, collimator_Radius, pb_Thickness/2, 0.*deg, 360.*deg);

  //G4LogicalVolume* filter2_Lead_Logical=new G4LogicalVolume(filter2_Lead_Solid, Pb,"filter2_Lead_Logical", 0, 0, 0);
  
  //filter2_Lead_Logical->SetVisAttributes(grey);
  //// Crystal musste wieder aus Get_Logical() gelöscht werden.
  //crystal2_Logical = germaniumDetector2->Get_Logical();

  //new G4PVPlacement(rm2,germaniumDetector2_Filter_Position,filter2_Lead_Logical, "filter2_Lead", world_log , false,0);
  
  
   /***************************** Polarimeter **************************/
  // contains Polarimeter Crystal (Serial Number 72930)
  /********************************************************************/
// At 90 Degree angle relativ to the beam.
  Polarimeter_TUD* polarimeterDetector=new Polarimeter_TUD("Polarimeter_TUD");

  G4double pol_Distance = -(poldistance + polarimeterDetector->Get_Length());
  
  G4double pol_theta=90.*deg;
  G4double pol_phi=180.*deg;
  G4double pol_Angle=pol_phi-90.*deg;

  G4RotationMatrix* rmPol=new G4RotationMatrix();
  rmPol->rotateY(pol_Angle);

  G4ThreeVector polarimeterDetector_Position=G4ThreeVector(pol_Distance*sin(pol_theta)*cos(pol_phi), pol_Distance*cos(pol_theta), pol_Distance*sin(pol_theta)*sin(pol_phi));
  
  G4LogicalVolume* polarimeterDetector_Logical=polarimeterDetector->Get_Logical();
//Pointer benutzt
  //G4LogicalVolume* crystalp_Logical = polarimeterDetector->Get_Logical();

  new G4PVPlacement(rmPol, polarimeterDetector_Position, polarimeterDetector_Logical, "Polarimeter", world_log, false, 0);

  // BGO of Polarimeter
  
  BGO* bgop = new BGO();
  
  G4double bgop_Distance = -(poldistance + bgop->Get_Length()/2);
  G4ThreeVector bgop_Position = G4ThreeVector(bgop_Distance*sin(pol_theta)*cos(pol_phi), bgop_Distance*cos(pol_theta), bgop_Distance*sin(pol_theta)*sin(pol_phi));
  
  G4LogicalVolume* bgop_Logical = bgop->Get_Logical();
  //Auch hier wurde G4LogicalVolume* hinzugefügt
  //G4LogicalVolume* bgoCrystalp_Logical = bgop->Get_Crystal_Logical();
  
  new G4PVPlacement(rmPol, bgop_Position, bgop_Logical, "bgop", world_log, false, 0);

// Filter of Polarimeter

  //cu_Thickness=1.*cm;
  //pb_Thickness=2.60*cm;

  //G4double pol_Filter_Distance = -(poldistance - BGO::bgo_Collimator_Length - cu_Thickness/2);   

  //G4ThreeVector pol_Filter_Position=G4ThreeVector(pol_Filter_Distance*sin(pol_theta)*cos(pol_phi), pol_Filter_Distance*cos(pol_theta), pol_Filter_Distance*sin(pol_theta)*sin(pol_phi));
  //G4Tubs* pol_Filter_Cu_Solid=new G4Tubs("pol_Filter_Cu_Solid", 0.*cm, collimator_Radius, cu_Thickness/2, 0.*deg, 360.*deg);

  //G4LogicalVolume* pol_Filter_Cu_Logical=new G4LogicalVolume(pol_Filter_Cu_Solid, Cu,"pol_Filter_Cu_Logical", 0, 0, 0);

  //pol_Filter_Cu_Logical->SetVisAttributes(orange);
  
  //new G4PVPlacement(rmPol, pol_Filter_Position, pol_Filter_Cu_Logical, "FilterPol_Copper", world_log , false, 0);
  
  //pol_Filter_Distance = -(poldistance - BGO::bgo_Collimator_Length - cu_Thickness - pb_Thickness/2);    

  //pol_Filter_Position=G4ThreeVector(pol_Filter_Distance*sin(pol_theta)*cos(pol_phi), pol_Filter_Distance*cos(pol_theta), pol_Filter_Distance*sin(pol_theta)*sin(pol_phi));

  //G4Tubs* pol_Filter_Pb_Solid=new G4Tubs("pol_Filter_Pb_Solid", 0.*cm, collimator_Radius, pb_Thickness/2, 0.*deg, 360.*deg);

  //G4LogicalVolume* pol_Filter_Pb_Logical=new G4LogicalVolume(pol_Filter_Pb_Solid, Pb,"pol_Filter_Pb_Logical", 0, 0, 0);

  //pol_Filter_Pb_Logical->SetVisAttributes(grey);
  
  //new G4PVPlacement(rmPol,pol_Filter_Position,pol_Filter_Pb_Logical, "FilterPol_Lead", world_log, false, 0);

// /control/execute init_vis.mac

	return world_phys;
}

void DetectorConstruction::ConstructSDandField() {

}
