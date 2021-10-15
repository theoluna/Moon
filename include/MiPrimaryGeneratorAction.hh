#ifndef MiPrimaryGeneratorAction_h
#define MiPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"

class MiPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
	MiPrimaryGeneratorAction();    
	~MiPrimaryGeneratorAction();

	void GeneratePrimaries(G4Event*);

	private:
		G4ParticleGun* ProtonGun;
};

#endif
