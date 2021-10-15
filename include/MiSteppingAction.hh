#include "G4UserSteppingAction.hh"
#include "G4Step.hh"
#include "MiEventAction.hh"

class MiSteppingAction : public G4UserSteppingAction 
{
	public:
  
        	MiSteppingAction(MiEventAction* EventAction);
        	~MiSteppingAction();
     
        	void UserSteppingAction(const G4Step* stp);


	private:
		
		MiEventAction* fEvAct;
};

