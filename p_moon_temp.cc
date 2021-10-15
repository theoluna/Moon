#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "MiDetectorConstruction.hh"
#include "MiActionInitialization.hh"

#include "MiSteppingAction.hh"
#include "MiRunAction.hh"
#include "MiEventAction.hh"

#include "G4VisExecutive.hh"

#include "G4PhysListFactory.hh"
#include "G4VUserPhysicsList.hh"

#include "G4RootAnalysisManager.hh"

int main()
{
	int seed = %SEED;
	G4cout << "Seed used in simulation was set to: " << seed << G4endl;

	G4Random::setTheSeed(seed);

	// *** RUN MANAGER ***
	G4cout << "*** RUN MANAGER ***" << G4endl << G4endl;
	// construct the default run manager
	G4RunManager* runManager = new G4RunManager;
	// set mandatory initialization classes

	G4PhysListFactory* physListFactory = new G4PhysListFactory(); 
	G4VUserPhysicsList* physicsList = physListFactory->GetReferencePhysList("QGSP_BIC_HP"); 

		//runManager->SetUserInitialization(physicsList);
		runManager->SetUserInitialization(new MiDetectorConstruction);
		runManager->SetUserInitialization(physicsList);
		runManager->SetUserInitialization(new MiActionInitialization);

	// initialize G4 kernel
		runManager->Initialize();

	// *** VIS MANAGER ***
	G4cout << "*** VIS MANAGER ***" << G4endl << G4endl;
	G4VisManager* visManager = new G4VisExecutive;
		visManager->Initialize();
	
	// *** UI MANAGER ***
	G4cout << "*** UI MANAGER ***" << G4endl << G4endl;
	// get the pointer to the UI manager and set verbosities
	G4UImanager* UI = G4UImanager::GetUIpointer();
		UI->ApplyCommand("/vis/scene/create");
		UI->ApplyCommand("/run/verbose 0");
		UI->ApplyCommand("/event/verbose 0");
		UI->ApplyCommand("/tracking/verbose 0");
		UI->ApplyCommand("/vis/open HepRepFile");
		UI->ApplyCommand("/vis/scene/endOfEventAction accumulate");
		UI->ApplyCommand("/vis/viewer/flush");

		UI->ApplyCommand("/vis/scene/add/trajectories smooth");
		UI->ApplyCommand("/vis/scene/add/hits");

	G4RootAnalysisManager* analysisManager = G4RootAnalysisManager::Instance();
		//analysisManager->OpenFile("%PROJECTDIR/RESULTS/p_50MeV_%FILENO.root");
		//analysisManager->OpenFile("%PROJECTDIR/RESULTS/p_50MeV_wH2O_1p_%FILENO.root");
		analysisManager->OpenFile("%PROJECTDIR/RESULTS/p_50MeV_wH2O_10p_%FILENO.root");
		//analysisManager->OpenFile("%PROJECTDIR/RESULTS/p_100MeV_%FILENO.root");
		//analysisManager->OpenFile("%PROJECTDIR/RESULTS/p_100MeV_wH2O_1p_%FILENO.root");
		//analysisManager->OpenFile("%PROJECTDIR/RESULTS/p_100MeV_wH2O_10p_%FILENO.root");
		//analysisManager->OpenFile("%PROJECTDIR/RESULTS/p_1GeV_%FILENO.root");
		//analysisManager->OpenFile("%PROJECTDIR/RESULTS/p_1GeV_wH2O_1p_%FILENO.root");
		//analysisManager->OpenFile("%PROJECTDIR/RESULTS/p_1GeV_wH2O_10p_%FILENO.root");
		analysisManager->CreateH2("D: all gammas, creation depth vs. initial energy",           "D: all gammas, creation depth vs. initial energy",           1500, 0.0, 15000.0, 360, -3000.0, 600);
		analysisManager->CreateH2("E: all neutrons, creation depth vs. initial energy",         "E: all neutrons, creation depth vs. initial energy",         1500, 0.0, 15000.0, 360, -3000.0, 600);
		analysisManager->CreateH1("F: energies of gammas over surface",                         "F: energies of gammas over surface",                         1500, 0.0, 15000.0);
		analysisManager->CreateH1("G: energies of neutrons over surface",                       "G: energies of neutrons over surface",                       1500, 0.0, 15000.0);
		analysisManager->CreateH2("H: gammas above surface, creation depth vs. final energy",   "H: gammas above surface, creation depth vs. final energy",   1500, 0.0, 15000.0, 360, -3000.0, 600);
		analysisManager->CreateH2("I: neutrons above surface, creation depth vs. final energy", "I: neutrons above surface, creation depth vs. final energy", 1500, 0.0, 15000.0, 360, -3000.0, 600);

	// start a run
	int numberOfEvent = 1;
	runManager->BeamOn(numberOfEvent);

  		analysisManager->Write();
		analysisManager->CloseFile();
	// job termination
	delete runManager;

	return 0;
}
