#include "Materials.hh"

#include "G4NistManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

Materials::Materials() { ConstructMaterials(); }

Materials::~Materials() {}

void Materials::ConstructMaterials() {

	// Implement materials which are not in the Geant4 materials database
	// http://geant4.cern.ch/UserDocumentation/UsersGuides/ForApplicationDeveloper/html/apas08.html
	// for example isotopically enriched materials or materials with a
	// non-standard density.

	G4NistManager *nist = G4NistManager::Instance();

	G4double z, a, n;
	G4double density, abundance;
	G4String name, symbol;
	G4int ncomponents, natoms;

	// BGO (old implementation by J. Hasper, density does not agree with
	// standard density of BGO material... why?)
	/*  density=9.2*g/cm3;
	  BGO=new G4Material(name="BGO", density, ncomponents=3);
	  BGO->AddElement(elBi, 12);
	  BGO->AddElement(elGe, 1);
	  BGO->AddElement(elO, 20);*/

	// Enriched Ti target from 2014/2015 NRF experiments
	G4Isotope *Ti50 =
	    new G4Isotope(name = "50Ti", z = 22, n = 50, a = 49.945791 * g / mole);
	G4Isotope *Ti49 =
	    new G4Isotope(name = "49Ti", z = 22, n = 49, a = 48.947870 * g / mole);
	G4Isotope *Ti48 =
	    new G4Isotope(name = "48Ti", z = 22, n = 48, a = 47.947946 * g / mole);
	G4Isotope *Ti47 =
	    new G4Isotope(name = "47Ti", z = 22, n = 47, a = 46.951763 * g / mole);
	G4Isotope *Ti46 =
	    new G4Isotope(name = "46Ti", z = 22, n = 46, a = 45.952631 * g / mole);

	G4Element *enriched_Ti =
	    new G4Element(name = "enriched Ti", symbol = "Ti", ncomponents = 5);

	enriched_Ti->AddIsotope(Ti50, abundance = 67.62 * perCent);
	enriched_Ti->AddIsotope(Ti49, abundance = 3.01 * perCent);
	enriched_Ti->AddIsotope(Ti48, abundance = 24.06 * perCent);
	enriched_Ti->AddIsotope(Ti47, abundance = 2.53 * perCent);
	enriched_Ti->AddIsotope(Ti46, abundance = 2.78 * perCent);

	G4Element *nat_O = nist->FindOrBuildElement("O");

	target_TiO2 = new G4Material(name = "enriched Ti", density = 4. * g / cm3,
	                             ncomponents = 2);
	// Density is a mean value of different TiO2 crystal lattice configurations.
	target_TiO2->AddElement(enriched_Ti, natoms = 1);
	target_TiO2->AddElement(nat_O, natoms = 2);

	// P10 gas for fission chamber

	G4Element *nat_C = nist->FindOrBuildElement("C");
	G4Element *nat_H = nist->FindOrBuildElement("H");
	G4Element *nat_Ar = nist->FindOrBuildElement("Ar");

	P10 =
	    new G4Material(name = "P10", density = 1.57 * kg / m3, ncomponents = 3);
	P10->AddElement(nat_Ar, natoms = 9);
	P10->AddElement(nat_H, natoms = 4);
	P10->AddElement(nat_C, natoms = 1);

	//***************************************
	// Target material for 116Sn experiment
	//***************************************

	// Chromium-Oxide, enriched in 52Cr
	// The target was enriched to 99.8%, estimated contributions of other
	// isotopes from the natural abundance

	G4Isotope *Cr50 =
	    new G4Isotope(name = "50Cr", z = 24, n = 50, a = 49.946041 * g / mole);
	G4Isotope *Cr52 =
	    new G4Isotope(name = "52Cr", z = 24, n = 52, a = 51.940504 * g / mole);
	G4Isotope *Cr53 =
	    new G4Isotope(name = "53Cr", z = 24, n = 53, a = 52.940646 * g / mole);
	G4Isotope *Cr54 =
	    new G4Isotope(name = "54Cr", z = 24, n = 54, a = 53.938878 * g / mole);

	G4Element *enriched_Cr =
	    new G4Element(name = "enriched Cr", symbol = "Cr", ncomponents = 4);

	G4double nat_abundance_50Cr =
	    0.04345; // All natural abundances from Wikipedia
	// G4double nat_abundance_52Cr = 0.83789;
	G4double nat_abundance_53Cr = 0.09501;
	G4double nat_abundance_54Cr = 0.02365;

	enriched_Cr->AddIsotope(Cr50, abundance =
	                                  0.002 * nat_abundance_50Cr /
	                                  (nat_abundance_50Cr + nat_abundance_53Cr +
	                                   nat_abundance_54Cr) *
	                                  perCent);
	enriched_Cr->AddIsotope(Cr52, abundance = 0.998 * perCent);
	enriched_Cr->AddIsotope(Cr53, abundance =
	                                  0.002 * nat_abundance_53Cr /
	                                  (nat_abundance_50Cr + nat_abundance_53Cr +
	                                   nat_abundance_54Cr) *
	                                  perCent);
	enriched_Cr->AddIsotope(Cr54, abundance =
	                                  0.002 * nat_abundance_54Cr /
	                                  (nat_abundance_50Cr + nat_abundance_53Cr +
	                                   nat_abundance_54Cr) *
	                                  perCent);

	Cr2O3_density = 5.22 * g / cm3; // Density from Wikipedia

	target_Cr2O3 = new G4Material(name = "enriched_Cr2O3",
	                              density = Cr2O3_density, ncomponents = 2);
	target_Cr2O3->AddElement(enriched_Cr, natoms = 2);
	target_Cr2O3->AddElement(nat_O, natoms = 3);

	// Tin target, enriched in 116Sn
	G4Isotope *Sn114 = new G4Isotope(name = "114Sn", z = 50, n = 114,
	                                 a = 113.902780 * g / mole);
	G4Isotope *Sn115 = new G4Isotope(name = "115Sn", z = 50, n =115,
	                                 a = 114.903345 * g / mole);
	G4Isotope *Sn116 = new G4Isotope(name = "116Sn", z = 50, n =116,
	                                 a = 115.901743 * g / mole);
	G4Isotope *Sn117 = new G4Isotope(name = "117Sn", z = 50, n = 117,
	                                 a = 116.902954 * g / mole);
	G4Isotope *Sn118 = new G4Isotope(name = "118Sn", z = 50, n = 118,
	                                 a = 117.901607 * g / mole);
	G4Isotope *Sn119 = new G4Isotope(name = "119Sn", z = 50, n =119,
	                                 a = 118.903311 * g / mole);
	G4Isotope *Sn120 = new G4Isotope(name = "120Sn", z = 50, n = 120,
	                                 a = 119.902202 * g / mole);
	G4Isotope *Sn122 = new G4Isotope(name = "122Sn", z = 50, n = 122,
	                                 a = 121.903444 * g / mole);
	G4Isotope *Sn124 = new G4Isotope(name = "124Sn", z = 50, n = 124,
	                                 a = 123.905277 * g / mole);

	G4Element *enriched_Sn =
	    new G4Element(name = "enriched Sn", symbol = "Sn", ncomponents = 9);

	// Isotopic abundances from data sheet from Trace Science. 112Sn was
	// neglected, since they only give an upper limit for the abundance. The
	// abundance of 116Sn was modified from 97.80% to 97.77% so that the numbers
	// add up to 100%
	enriched_Sn->AddIsotope(Sn114, abundance = 0.03 * perCent);
	enriched_Sn->AddIsotope(Sn115, abundance = 0.06 * perCent);
	enriched_Sn->AddIsotope(Sn116, abundance = 97.77 * perCent);
	enriched_Sn->AddIsotope(Sn117, abundance = 0.90 * perCent);
	enriched_Sn->AddIsotope(Sn118, abundance = 0.67 * perCent);
	enriched_Sn->AddIsotope(Sn119, abundance = 0.11 * perCent);
	enriched_Sn->AddIsotope(Sn120, abundance = 0.41 * perCent);
	enriched_Sn->AddIsotope(Sn122, abundance = 0.03 * perCent);
	enriched_Sn->AddIsotope(Sn124, abundance = 0.02 * perCent);

	Sn_density = 7.265 * g / cm3; // Density from Wikipedia, beta-tin, which is
	                              // the stable physical form of tin at room
	                              // temperature

	target_Sn = new G4Material(name = "enriched_Sn", density = Sn_density,
	                           ncomponents = 1);
	target_Sn->AddElement(enriched_Sn, natoms = 1);
}

