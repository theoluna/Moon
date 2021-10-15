#include "MiActionInitialization.hh"
#include "MiPrimaryGeneratorAction.hh"
#include "MiEventAction.hh"
#include "MiSteppingAction.hh"
  
MiActionInitialization::MiActionInitialization()
{
}
 
MiActionInitialization::~MiActionInitialization()
{
}

void MiActionInitialization::Build() const
{
        MiEventAction* evAct = new MiEventAction();
        SetUserAction(evAct);

	SetUserAction(new MiPrimaryGeneratorAction());
      
        SetUserAction(new MiSteppingAction(evAct));
}
