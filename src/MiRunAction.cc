#include "MiRunAction.hh"

#include "G4RootAnalysisManager.hh"

MiRunAction::MiRunAction()
{
	G4RootAnalysisManager* analysisManager = G4RootAnalysisManager::Instance();
		analysisManager->SetVerboseLevel(0);
  		analysisManager->SetFirstHistoId(0);
}

MiRunAction::~MiRunAction()
{
	delete G4RootAnalysisManager::Instance();
}
    
void MiRunAction::BeginOfRunAction(const G4Run*)
{
	G4cout<<"START-RUN!!!!!!!" << G4endl << G4endl;
}

void MiRunAction::EndOfRunAction(const G4Run*)
{
	G4cout << "END-RUN!!!!!!!" << G4endl << G4endl;
}
