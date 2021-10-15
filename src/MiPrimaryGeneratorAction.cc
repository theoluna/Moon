#include "MiPrimaryGeneratorAction.hh"

#include "G4ParticleTable.hh"

#include "G4SystemOfUnits.hh"

MiPrimaryGeneratorAction::MiPrimaryGeneratorAction()
{
}  

MiPrimaryGeneratorAction::~MiPrimaryGeneratorAction()
{
}

void MiPrimaryGeneratorAction::GeneratePrimaries(G4Event* G4E)
{
	ProtonGun = new G4ParticleGun(1); // Number of created particles with constructor of G4ParticleGun

	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  	G4ParticleDefinition* particle = particleTable->FindParticle("proton");
		ProtonGun->SetParticleDefinition(particle);
  		ProtonGun->SetParticleEnergy(1.0*GeV);
  		ProtonGun->SetParticlePosition(G4ThreeVector(0.0,0.0,33.0*m));
  		ProtonGun->SetParticleMomentumDirection(G4ThreeVector(0,0,-1));

	ProtonGun->GeneratePrimaryVertex(G4E);

	delete ProtonGun;
}
