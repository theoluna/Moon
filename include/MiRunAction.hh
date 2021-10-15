#include "G4UserRunAction.hh"
#include "globals.hh"

#include "G4Run.hh"

class MiRunAction : public G4UserRunAction
{
  public:
    MiRunAction();
    ~MiRunAction();

    //virtual G4Run* GenerateRun();
 
    void BeginOfRunAction(const G4Run*);
    void   EndOfRunAction(const G4Run*);
};

