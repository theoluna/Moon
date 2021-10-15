#ifndef MiDetectorConstruction_h
#define MiDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"

class MiDetectorConstruction : public G4VUserDetectorConstruction
{
	public:
		MiDetectorConstruction();
		~MiDetectorConstruction();

		G4VPhysicalVolume* Construct();
};

#endif
