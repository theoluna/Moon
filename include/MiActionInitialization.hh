#ifndef MiActionInitialization_h
#define MiActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

class MiActionInitialization : public G4VUserActionInitialization
{
	public:

    		MiActionInitialization();
    		virtual ~MiActionInitialization();

    		virtual void Build() const;
};

#endif

    
