	#include "MiEventAction.hh"

#include "G4RootAnalysisManager.hh"
#include "G4SystemOfUnits.hh"

MiEventAction::MiEventAction()
{
}

MiEventAction::~MiEventAction()
{
}
    
void MiEventAction::BeginOfEventAction(const G4Event* ev)
{
	//G4cout << "*********************Event No. " << ev->GetEventID() << " **********************" << G4endl;
}

void MiEventAction::EndOfEventAction(const G4Event* ev)
{

//************************************************* SECTION 1: Sanity check whether vertices were not saved twice (obsolete but might be useful in future)
	vector<double> verX, verY, verZ;

	for (unsigned int i = 0; i < particle.size(); i++)	// Cycle through particles 
	{
		// COLLECT ALL THE VERTICES

		bool ver_saved = false;

		for (unsigned int l = 0; l < verX.size(); l++)
		{
			if (verX.at(l) == x.at(i) &&
			    verY.at(l) == y.at(i) &&
			    verZ.at(l) == z.at(i))
			{
				ver_saved = true;
				break;
			}
		}

		if (!ver_saved)
		{
			verX.push_back(x.at(i));
			verY.push_back(y.at(i));
			verZ.push_back(z.at(i));

			//cout << "Vertex " << verX.back() << " mm, " << verY.back() << " mm, " << verZ.back() << " mm added!" << endl;  
			//cout << x.at(i) << ", " << y.at(i) << ", " << z.at(i) << " " << particle.at(i) << " " << trackID.at(i) << " parent " << parentID.at(i) << endl;
		}
	} 

//************************************************* SECTION 2

	G4RootAnalysisManager* analysisManager = G4RootAnalysisManager::Instance();

	for (unsigned int i = 0; i < verX.size(); i++)
	{
		int nparent = -1;
		vector<int> nchild;
		
		int neutrons   = 0;
		int protons    = 0;
		int alphas     = 0;
		int gammas     = 0;
		int electrons  = 0;
		int deuterons  = 0;
		int others     = 0;

		// FIND CHILDREN OF VERTEX
		for (unsigned int j = 0; j < particle.size(); j++) // Cycle through all particle to find what comes in and out of vertex
		{

			if (verX.at(i) == x.at(j) &&
			    verY.at(i) == y.at(j) &&
			    verZ.at(i) == z.at(j))
			{
				nchild.push_back(j);

				if (particle.at(j)  == "neutron")
				{
					neutrons++;
				}
				else if (particle.at(j)  == "proton")
				{
					protons++;
				}
				else if (particle.at(j)  == "alpha")
				{
					alphas++;
				}
				else if (particle.at(j)  == "gamma")
				{
					gammas++;
				}
				else if (particle.at(j)  == "e-")
				{
					electrons++;
				}
				else if (particle.at(j)  == "deuteron")
				{
					deuterons++;
				}
				else
				{
					others++;
				}
			}
		}

		// FIND PARENT OF VERTEX
		for (unsigned int k = 0; k < particle.size(); k++) // Cycle through all particles again to find a parent
		{
			if (parentID.at(nchild.at(0)) == trackID.at(k))
			{
				nparent = k;
			}
		}


// ********* ANALYSIS OF NUCLEAR PROCESSES AS A FUINCTION OF DEPTH
		if (nparent != -1) // Write only if it is not primary vertex
		{		
			stringstream ssname;
			stringstream ss_ncap_name;
			bool is_ncap = false;

			if (particle.at(nparent) == "neutron" ||
			    particle.at(nparent) == "proton"  ||
			    particle.at(nparent) == "alpha"   ||
			    particle.at(nparent) == "gamma"   ||
			    particle.at(nparent) == "e-"      ||
			    particle.at(nparent) == "deuteron") // Compose the name of histogram
			{
				ssname << "A: " << particle.at(nparent) 	<< "->"
								<< "n" << neutrons 
  								<< "p" << protons 
								<< "a" << alphas 
								<< "g" << gammas 
								<< "e" << electrons 
								<< "d" << deuterons 								
								<< "o" << others;

				// Takes care of special cases which are of interest also separately ("others" type particle is specified)
				if     (filter(particle.at(nparent), "neutron", neutrons, 0, protons, 0, alphas, 0, gammas, 0, electrons, 0, deuterons, 0, others, 1))
				{
					ss_ncap_name << "B: n->" << particle.at(nchild.at(0));
					is_ncap = true;
				}
				else if(filter(particle.at(nparent), "neutron", neutrons, 0, protons, 0, alphas, 0, gammas, 1, electrons, 0, deuterons, 0, others, 1))
				{
					int chi = 0;

					while (particle.at(nchild.at(chi)) == "gamma")
					{
						chi++;
					}					

					ss_ncap_name << "B: n->g+" << particle.at(nchild.at(chi));
					is_ncap = true;
				}
				else if(filter(particle.at(nparent), "neutron", neutrons, 0, protons, 0, alphas, 0, gammas, 2, electrons, 0, deuterons, 0, others, 1))
				{
					int chi = 0;

					while (particle.at(nchild.at(chi)) == "gamma")
					{
						chi++;
					}

					ss_ncap_name << "B: n->2g+" << particle.at(nchild.at(chi));
					is_ncap = true;
				}
				else if(filter(particle.at(nparent), "neutron", neutrons, 0, protons, 0, alphas, 0, gammas, 3, electrons, 0, deuterons, 0, others, 1))
				{
					int chi = 0;

					while (particle.at(nchild.at(chi)) == "gamma")
					{
						chi++;
					}
					ss_ncap_name << "B: n->3g+" << particle.at(nchild.at(chi));
					is_ncap = true;
				}
				else if(filter(particle.at(nparent), "neutron", neutrons, 0, protons, 0, alphas, 0, gammas, 4, electrons, 0, deuterons, 0, others, 1))
				{
					int chi = 0;

					while (particle.at(nchild.at(chi)) == "gamma")
					{
						chi++;
					}
					ss_ncap_name << "B: n->4g+" << particle.at(nchild.at(chi));
					is_ncap = true;
				}
				else if(filter(particle.at(nparent), "neutron", neutrons, 0, protons, 0, alphas, 0, gammas, 5, electrons, 0, deuterons, 0, others, 1))
				{
					int chi = 0;

					while (particle.at(nchild.at(chi)) == "gamma")
					{
						chi++;
					}
					ss_ncap_name << "B: n->5g+" << particle.at(nchild.at(chi));
					is_ncap = true;
				}
			}
			else
			{
				ssname << "A: other->"	<< "n" << neutrons 
  							<< "p" << protons 
							<< "a" << alphas 
							<< "g" << gammas 
							<< "e" << electrons 
							<< "d" << deuterons 								
							<< "o" << others;
			}

			string strname  = ssname.str();

			int histo;
			int mmm = 0;
			bool exists = false;
			//cout << "Type of process: " << strname << " " << verZ.at(i) << endl;

			while (analysisManager->GetH1Title(mmm) != "\0")
			{
				if (analysisManager->GetH1Title(mmm) == strname.c_str())
				{
					exists = true;
					break;	
				}

				mmm++;
			}

			histo = mmm;
			if (!exists)
			{
				analysisManager->CreateH1(strname.c_str(), strname.c_str(),    6600, -60000.0, 6000.0);
				G4cout << "Created histo No.: " << histo << " for vertex: " << strname.c_str() << G4endl;
			}
	
			analysisManager->FillH1(histo, verZ.at(i)/mm);
			
			// The same process again for n capture

			if (is_ncap)
			{
				string str_ncap_name  = ss_ncap_name.str();
				
				mmm = 0;
				exists = false;
				while (analysisManager->GetH1Title(mmm) != "\0")
				{
					if (analysisManager->GetH1Title(mmm) == str_ncap_name.c_str())
					{
						exists = true;
						break;	
					}
	
					mmm++;
				}

				histo = mmm;
				if (!exists)
				{
					analysisManager->CreateH1(str_ncap_name.c_str(), str_ncap_name.c_str(),   6600, -60000.0, 6000.0);
					G4cout << "Created histo No.: " << histo << " for vertex: " << str_ncap_name.c_str() << G4endl;
				}
		
				analysisManager->FillH1(histo, verZ.at(i)/mm);
			}
		}

		// PRINT THE VERTEX INFO

		/*if (nparent != -1)
		{
			//if ((particle.at(nparent) == "neutron") && neutrons == 0 && protons == 0 && alphas == 0 && gammas == 0 && electrons == 0 && deuterons == 0)
			//{	
				
				G4cout << "*********************Event No. " << ev->GetEventID() << " **********************" << G4endl;
				
				cout << "No " << i << ":" << endl;
				cout << "     IN:" << endl;
				G4cout << "         " << x.at(nparent) << ", " 
			 		      	      << y.at(nparent) << ", " 
                                	              << z.at(nparent) << " " 
					              << particle.at(nparent) << " " 
					              << trackID.at(nparent) << " parent " 
					              << parentID.at(nparent) << G4endl;
	
				cout << "     OUT:" << endl;
		
				for (unsigned int l = 0; l < nchild.size(); l++) // Cycle through all particle to find what comes in and out of vertex
				{
					G4cout << "         " << x.at(nchild.at(l)) << ", " 
				 	      	    	      << y.at(nchild.at(l)) << ", " 
                        	                   	      << z.at(nchild.at(l)) << " " 
				                              << particle.at(nchild.at(l)) << " " 
					                      << trackID.at(nchild.at(l)) << " parent " 
					                      << parentID.at(nchild.at(l)) << G4endl;
				}

				G4cout << G4endl; 
			//}
		}
		else
		{
			//cout << "        NO PARENT - PRIMARY!" << endl;
		}*/
	}

// ********* ANALYSIS OF SPECTRA IN VARIOUS DEPTHS

	const double D_REGION = 100.0; 
	const double MIN_D    = -1500.0; 

	for (unsigned int i = 0; i < particle.size(); i++)	// Cycle through particles 
	{	
		if ( particle.at(i) == "gamma"   ||
		     particle.at(i) == "neutron" )
		{
			// Part dedicated to 1D spectra of initial energy

			int segment = int(z.at(i)/(100.0*mm));

			int histo;
			int it = 0;
			bool exists = false;
			stringstream ssnm;
		
		
			if (z.at(i) > MIN_D)
			{	
				ssnm << "C: " << particle.at(i) << " spect: " << double(segment - 1)*D_REGION << " mm < z < " << double(segment)*D_REGION << " mm";	
			}
			else
			{
				ssnm << "C: " << particle.at(i) << " spect: z <= " << MIN_D << " mm";
			}
		
			string strnm  = ssnm.str();

			while (analysisManager->GetH1Title(it) != "\0")
			{
				if (analysisManager->GetH1Title(it) == strnm.c_str())
				{
					exists = true;
					break;	
				}	

				it++;
			}
	
			histo = it;
			if (!exists)
			{
				analysisManager->CreateH1(strnm.c_str(), strnm.c_str(),     1500.0, 0.0, 15000.0);
				G4cout << "Created histo No.: " << histo << " for spectrum: " << strnm.c_str() << G4endl;
			}

			analysisManager->FillH1(histo, E_init.at(i)/keV);
					
			// Part dedicated to 2D plots of vertex depth vs. initial energy
			int mmm = 0;

			if      (particle.at(i) == "gamma")
			{	
				while (analysisManager->GetH2Title(mmm) != "\0")
				{
					if (analysisManager->GetH2Title(mmm) == "D: all gammas, creation depth vs. initial energy")
					{
						break;	
					}

					mmm++;
				}
			}			
			else if (particle.at(i) == "neutron") 			
			{
				while (analysisManager->GetH2Title(mmm) != "\0")
				{
					if (analysisManager->GetH2Title(mmm) == "E: all neutrons, creation depth vs. initial energy")
					{
						break;	
					}

					mmm++;
				}
			}
			
			analysisManager->FillH2(mmm, E_init.at(i)/keV, z.at(i)/mm);
		}
	}	

	 particle.clear();
	        x.clear();
	        y.clear();
	        z.clear();
	   E_init.clear();
	  trackID.clear();
	 parentID.clear();

	if (ev->GetEventID() % 1000 == 0)
	{	
		G4cout << "Event No. " << ev->GetEventID() << " processed!" << G4endl;
	}
}

void   MiEventAction::Add_particle(string _name, double _x, double _y, double _z, double _E_init, int _ID, int _parentID)
{
	if (_ID != 1)
	{
		particle.push_back(_name);
		       x.push_back(_x);
		       y.push_back(_y);
		       z.push_back(_z - 27.0*m);
		  E_init.push_back(_E_init);
		 trackID.push_back(_ID);
		parentID.push_back(_parentID);
	
		/*cout << "ADDED: "      << _name
		     << ", vertex = (" << _x/mm << " mm, " << _y/mm << " mm, " << _z - 27.0*m << " mm)"
		     << ", E = "       << _E_init / keV << " keV"
		     << ", ID = "      << _ID << ", parentID = " << _parentID <<  endl;*/
	}
}

bool   MiEventAction::filter(string _npar, string _npar_comp, int _n, int _n_comp, int _p, int _p_comp, int _a, int _a_comp, int _g, int _g_comp, int _e, int _e_comp, int _d, int _d_comp, int _o, int _o_comp)
{
	if(	_npar == _npar_comp &&
		_n    ==    _n_comp &&
		_p    ==    _p_comp &&
		_a    ==    _a_comp &&
		_g    ==    _g_comp &&
		_e    ==    _e_comp &&
		_d    ==    _d_comp &&
		_o    ==    _o_comp )
	{
		return true;       
	}
	else
	{
		return false;
	}
}
