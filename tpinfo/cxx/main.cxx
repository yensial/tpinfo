#include "misc.hxx"
#include "Neutron.hxx"
#include "Material.hxx"

int main()
{
    // unique initialization of the Random Number Generator method rand() from the cstdlib library (included in misc.hxx)
    
	int seed = time(0);	
	srand(seed);
    
    // construction of NonAbsorbingModerator (pointer to a first object, of the Material class)
    
    Material* NonAbsorbingModerator = new Material();
 
    // init of NonAbsorbingModerator

    double MassNumber = 1;                  // A (here material is supposed to be pure H of mass number A = 1)
    double ScatteringCrossSection = 1.3;    // in cm-1  
    
    NonAbsorbingModerator->SetMainParameters(MassNumber, ScatteringCrossSection);

    // construction of SlowingDownNeutron (pointer to the second object, of the Neutron class) to be re-initialized and re-used for each new neutron
    
    string DataDirName = "../dat";          // already existing directory (for writing data files in it, while source and executable files are in cxx)
    Neutron* SlowingDownNeutron = new Neutron("SlowingDownNeutron", DataDirName);

    // SlowingDownNeutron needs to know the Material where it travels
    
    SlowingDownNeutron->SetMaterial(NonAbsorbingModerator);
    
    // init of SlowingDownNeutron's typical energies (max at start, and min to reach)
    
    double StartEnergy = InitialNeutronEnergy;      // in eV (constant defined in misc.hxx)
    double FinalEnergy = 1.0;                       // final energy set to 1 eV (for which the average number of collisions is wanted)
    
    SlowingDownNeutron->InitEnergies(StartEnergy, FinalEnergy);
    
    // CALCULATION : to be completed step by step, by defining and using new methods (public) and attributes (private) in both classes
    
    // step0 (given) : builds the very first segment (starting from 0,0 in the x direction) of the very first neutron trajectory
    
    double el = SlowingDownNeutron->SampleLength();
    cout << "the length of the very first segment has been sampled equal to " << el << " cm" << endl;
    
    double psi = 0;
    double x = el*cos(psi);
    double y = el*sin(psi);
    cout << "the neutron is now in x = " << x << " and y = " << y << " (position of the first diffusion)" << endl;
    
    // step1 (TO DO BELOW) : adds a second standard segment (on which you will loop for a first complete trajectory in step2)
    
    
    




}
