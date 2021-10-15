#include "MiSteppingAction.hh"

#include "G4RootAnalysisManager.hh"
#include "Randomize.hh"

#include "G4EventManager.hh"
#include "G4Event.hh"

#include "G4SystemOfUnits.hh"	

MiSteppingAction::MiSteppingAction(MiEventAction* EventAction)
{
	fEvAct = EventAction;
}

MiSteppingAction::~MiSteppingAction()
{
}
  
void MiSteppingAction::UserSteppingAction(const G4Step* stp)
{ 
	double vX       = stp->GetTrack()->GetVertexPosition().getX();
	double vY       = stp->GetTrack()->GetVertexPosition().getY();
	double vZ       = stp->GetTrack()->GetVertexPosition().getZ();

// *** SENDS INFO TO MiEvent WHICH PERFORMS ANALYSIS 
	if( vX == stp->GetPreStepPoint()->GetPosition().getX() &&
	    vY == stp->GetPreStepPoint()->GetPosition().getY() &&
	    vZ == stp->GetPreStepPoint()->GetPosition().getZ())		// Choose only if it is first step of each particle track
	{
		string pname    = stp->GetTrack()->GetDynamicParticle()->GetParticleDefinition()->GetParticleName();
		int    trackID  = stp->GetTrack()->GetTrackID();
		int    parentID = stp->GetTrack()->GetParentID();
		double InitE    = stp->GetPreStepPoint()->GetKineticEnergy();
		
		/*cout << "ENE     : " << InitE / keV << " keV" << endl;
		cout << "VERPOS  : " << pname << ", " << vX << ", " << vY << ", " << vZ << std::endl;
		cout << "PRESTPOS: " << pname << ", " << stp->GetPreStepPoint()->GetPosition().getX() << ", " << stp->GetPreStepPoint()->GetPosition().getY() << ", " << stp->GetPreStepPoint()->GetPosition().getZ() << std::endl;
		cout << "ID      : " << trackID << std::endl;
		cout << "parentID: " << parentID << std::endl;*/

		fEvAct->Add_particle(pname, vX, vY, vZ, InitE, trackID, parentID);
	}

// *** ANALYSIS OF SPECTRA COMING OUT FROM BELOW GROUND + 2D SPECTRA DECOMPOSITION ACCORDING TO DEPTH WHERE PARTICLE WAS BORN
	if (stp->GetPostStepPoint()->GetPosition().getZ() - 27.0*m > 0.0 && stp->GetTrack()->GetParentID() != 0) // If particle is over the ground but it is not an initial proton
	{
		double partE  = stp->GetPreStepPoint()->GetKineticEnergy();
		string partnm = stp->GetTrack()->GetDynamicParticle()->GetParticleDefinition()->GetParticleName();

		G4RootAnalysisManager* analysisManager = G4RootAnalysisManager::Instance();
		
		int mmm = 0;
		int nnn = 0;

		if      (partnm == "gamma")
		{
			while (analysisManager->GetH1Title(mmm) != "\0")
			{
				if (analysisManager->GetH1Title(mmm) == "F: energies of gammas over surface")
				{
					break;	
				}

				mmm++;
			}
			
			while (analysisManager->GetH2Title(nnn) != "\0")
			{
				if (analysisManager->GetH2Title(nnn) == "H: gammas above surface, creation depth vs. final energy")
				{
					break;	
				}

				nnn++;
			}

			analysisManager->FillH1(mmm, partE/keV);
			analysisManager->FillH2(nnn, partE/keV, (vZ-27*m)/mm);	
		}
		else if (partnm == "neutron")	
		{
			while (analysisManager->GetH1Title(mmm) != "\0")
			{
				if (analysisManager->GetH1Title(mmm) == "G: energies of neutrons over surface")
				{
					break;	
				}

				mmm++;
			}
			
			while (analysisManager->GetH2Title(nnn) != "\0")
			{
				if (analysisManager->GetH2Title(nnn) == "I: neutrons above surface, creation depth vs. final energy")
				{
					break;	
				}

				nnn++;
			}

			analysisManager->FillH1(mmm, partE/keV);
			analysisManager->FillH2(nnn, partE/keV, (vZ-27*m)/mm);	
		}
	}
}
