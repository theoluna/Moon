#include "MiDetectorConstruction.hh"

#include "G4Material.hh"
#include "G4Box.hh"

#include "G4NistManager.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4SystemOfUnits.hh"

MiDetectorConstruction::MiDetectorConstruction()
{
}
 
MiDetectorConstruction::~MiDetectorConstruction()
{
}
 
G4VPhysicalVolume* MiDetectorConstruction::Construct()
{
	// THE DETECTOR AND WORLD CONSTANTS

	const G4double DetX = 10*m; 
	const G4double DetY = DetX;
	const G4double DetZ = 60*m;

	const G4double extraZ = 6.0*m;

	// MATERIAL

	G4NistManager* nistManager = G4NistManager::Instance();
	nistManager->FindOrBuildMaterial("G4_Galactic");

	G4Material* O = nistManager->FindOrBuildMaterial("G4_O");
	G4Material* Si = nistManager->FindOrBuildMaterial("G4_Si");
	G4Material* Fe = nistManager->FindOrBuildMaterial("G4_Fe");
	G4Material* Ca = nistManager->FindOrBuildMaterial("G4_Ca");
	G4Material* Al = nistManager->FindOrBuildMaterial("G4_Al");
	G4Material* Mg = nistManager->FindOrBuildMaterial("G4_Mg");

	G4Material* vacuumMat  = G4Material::GetMaterial("G4_Galactic");

	// Composition source: https://en.wikipedia.org/wiki/Lunar_soil and http://meteorites.wustl.edu/lunar/lunar_soil/lunar_soil_composions.htm
	// Without water	
	/*G4Material* moonMat = new G4Material("moonMAT", 1.5*g/cm3, 6); //Density, source: https://en.wikipedia.org/wiki/Lunar_soil
		    moonMat->AddMaterial(O,  0.44); 
	            moonMat->AddMaterial(Si, 0.22);
	            moonMat->AddMaterial(Fe, 0.12);
	            moonMat->AddMaterial(Ca, 0.09);
	            moonMat->AddMaterial(Al, 0.07);
	            moonMat->AddMaterial(Mg, 0.06);*/

	// Same composition but With 1% water	
	/*G4Material* H2O = nistManager->FindOrBuildMaterial("G4_WATER");
	G4Material* moonMat = new G4Material("moonMAT", 1.5*g/cm3, 7); //Density, source: https://en.wikipedia.org/wiki/Lunar_soil
		    moonMat->AddMaterial(O,   0.99*0.44); 
	            moonMat->AddMaterial(Si,  0.99*0.22);
	            moonMat->AddMaterial(Fe,  0.99*0.12);
	            moonMat->AddMaterial(Ca,  0.99*0.09);
	            moonMat->AddMaterial(Al,  0.99*0.07);
	            moonMat->AddMaterial(Mg,  0.99*0.06);
	            moonMat->AddMaterial(H2O, 0.01);*/



	// Same composition but With 10% water	
	G4Material* H2O = nistManager->FindOrBuildMaterial("G4_WATER");
	G4Material* moonMat = new G4Material("moonMAT", 1.5*g/cm3, 7); //Density, source: https://en.wikipedia.org/wiki/Lunar_soil
		    moonMat->AddMaterial(O,   0.9*0.44); 
	            moonMat->AddMaterial(Si,  0.9*0.22);
	            moonMat->AddMaterial(Fe,  0.9*0.12);
	            moonMat->AddMaterial(Ca,  0.9*0.09);
	            moonMat->AddMaterial(Al,  0.9*0.07);
	            moonMat->AddMaterial(Mg,  0.9*0.06);
	            moonMat->AddMaterial(H2O, 0.1);

	// Composition but With 100% water	
	/*nistManager->FindOrBuildMaterial("G4_WATER");
	G4Material* moonMat = G4Material::GetMaterial("G4_WATER");*/

	G4bool fCheckOverlaps = true;

//WORLD

	G4Box* worldS
	  = new G4Box("world",                                    //its name
                      DetX/2.0, DetX/2.0, (DetZ + extraZ)/2.0); //its size

	G4LogicalVolume* worldLV
	  = new G4LogicalVolume(worldS,   //its solid
               			vacuumMat,      //its material
               			"World"); //its name

	G4VPhysicalVolume* worldPV
	  = new G4PVPlacement(0,         // no rotation
               		      G4ThreeVector(), // at (0,0,0)
               		      worldLV,         // its logical volume
               		      "World",         // its name
               		      0,               // its mother  volume
               		      false,           // no boolean operations
                 	      0,               // copy number
                 	      fCheckOverlaps); // checking overlaps

//DETEKTOR

	G4Box* moonS
	  = new G4Box("moonS",                                    //its name
                      DetX/2, DetY/2, DetZ/2); //its size

	G4LogicalVolume* moonLV
	  = new G4LogicalVolume(moonS,   //its solid
               			moonMat,      //its material
				"moonLV");

	G4VPhysicalVolume* plaPV = new G4PVPlacement(0, 
					     	G4ThreeVector(0.0, 0.0, -extraZ/2.0), 
					     	moonLV, 
					     	"moonPV", 
					     	worldLV, 
						false, 
						0, 
						false);

	return worldPV;
}


