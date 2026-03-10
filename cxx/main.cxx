#include "misc.hxx"
#include "Neutron.hxx"
#include "Material.hxx"
#include "chrono"

using namespace std;
using namespace std::chrono;


int main()
{
    // unique initialization of the Random Number Generator method rand() from the cstdlib library (included in misc.hxx)
    
	int seed = time(0);	
	srand(seed);
    
    // construction of NonAbsorbingModerator (pointer to a first object, of the Material class)
    
    Material* NonAbsorbingModerator = new Material();
 
    // init of NonAbsorbingModerator

    double Sigma0 = 0.13;                    // in cm-1 
    double Sigma1 = 0.22;
    double MassNumber0 = 16;
    double MassNumber1 = 2;                  // A (here material is supposed to be pure H of mass number A = 1) 
    
    NonAbsorbingModerator->SetMassNumberCrossSection(MassNumber0, MassNumber1, Sigma0, Sigma1);

    // construction of SlowingDownNeutron (pointer to the second object, of the Neutron class) to be re-initialized and re-used for each new neutron
    
    string DataDirName = "../dat";         // already existing directory (for writing data files in it, while source and executable files are in cxx)
    string TimeDirName = "../dat";        // already existing directory (for writing time files in it, while source and executable files are in cxx)
    Neutron* SlowingDownNeutron = new Neutron("SlowingDownNeutron", DataDirName, TimeDirName);

    // SlowingDownNeutron needs to know the Material where it travels
    
    SlowingDownNeutron->SetMaterial(NonAbsorbingModerator);
    
    // init of SlowingDownNeutron's typical energies (max at start, and min to reach)
    
    double StartEnergy = InitialNeutronEnergy;      // in eV (constant defined in misc.hxx)
    double FinalEnergy = 1.0;                       // final energy set to 1 eV (for which the average number of collisions is wanted)
    
    SlowingDownNeutron->InitEnergies(StartEnergy, FinalEnergy);

    // Stockage ram
    vector<int> store_chocs;
    vector<double> store_x;
    vector<double> store_y;
    vector<double> store_e;
    store_chocs.reserve(NombreDeTrajectoire);
    store_x.reserve(NombreDeTrajectoire);
    store_y.reserve(NombreDeTrajectoire);
    store_e.reserve(NombreDeTrajectoire);

    auto start = chrono::high_resolution_clock::now();
    SlowingDownNeutron->BuildTrajectory(); //initialisation: histoire n°0

    // step3 : loop on the step2 and mean on the diffusion number
    double mean = 0;

    for(int i = 0; i < NombreDeTrajectoire; i++)
    {
        SlowingDownNeutron->ResetParameters();                     
    
        SlowingDownNeutron->InitEnergies(StartEnergy, FinalEnergy);

        SlowingDownNeutron->BuildTrajectory();
        
        mean += ( SlowingDownNeutron->GetDiffuNumber() );

        // Sauvegarde des données en mémoire vive
        store_chocs.push_back(SlowingDownNeutron->GetDiffuNumber());
        double pos[2];
        SlowingDownNeutron->GetPositions(pos);
        store_x.push_back(pos[0]);
        store_y.push_back(pos[1]);
        store_e.push_back(SlowingDownNeutron->GetEnergy());
    }


    mean /= NombreDeTrajectoire;
    

    // Ecriture post calculs
    string OutputFileName = DataDirName + "/SlowingDownNeutron.dat";
    ofstream OutputFile(OutputFileName.c_str());
    OutputFile << "nChocs,x,y,Energie" << "\n";
    
    for(int i = 0; i < NombreDeTrajectoire; i++)
    {
        OutputFile << store_chocs[i] << "," << store_x[i] << "," 
                   << store_y[i] << "," << store_e[i] << "\n";
    }
    OutputFile.close();

    auto end = chrono::high_resolution_clock::now();
    double duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Le temps de calcul est " << duration << " millisecondes" << endl;
    cout << "Le nombre moyen d'impact est " << mean << endl;
}
