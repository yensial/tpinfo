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
    
    SlowingDownNeutron->BuildTrajectory(SlowingDownNeutron, StartEnergy, FinalEnergy); //Step 1+2

    // step3 : loop on the step2 and mean on the diffusion number
    double mean = 0;

    for(int i = 0; i < NombreDeTrajectoire; i++)
    {
        SlowingDownNeutron->ResetParameters();

        double StartEnergy = InitialNeutronEnergy;      
        double FinalEnergy = 1.0;                     
    
        SlowingDownNeutron->InitEnergies(StartEnergy, FinalEnergy);

        SlowingDownNeutron->BuildTrajectory(SlowingDownNeutron, StartEnergy, FinalEnergy);
        
        mean += ( SlowingDownNeutron->GetDiffuNumber() );
    }

    mean /= NombreDeTrajectoire;

    cout << "Le nombre moyen d'impact est " << mean << endl;

}


