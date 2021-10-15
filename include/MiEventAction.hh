#ifndef MiEventAction_h
#define MiEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

#include "G4Event.hh"

using namespace std;

class MiEventAction : public G4UserEventAction
{
  	public:
    		MiEventAction();
    		~MiEventAction();
 
    		void   BeginOfEventAction(const G4Event* ev);
    		void   EndOfEventAction(const G4Event* ev);

		void   Add_particle(string _name, double _x, double _y, double _z, double _E_init, int _ID, int _parentID);
		bool   filter(string _npar, string _npar_comp, int _n, int _n_comp, int _p, int _p_comp, int _a, int _a_comp, int _g, int _g_comp, int _e, int _e_comp, int _d, int _d_comp, int _o, int _o_comp);
    		
	private:
		vector<double> x;
		vector<double> y;
		vector<double> z;
		vector<double> E_init;
		vector<string> particle;
		vector<int>    trackID;
		vector<int>    parentID;
};

#endif

