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

    double EpsilonTheta = double(rand()) / RAND_MAX;

    double CosTheta = 2*EpsilonTheta - 1;
    
    SlowingDownNeutron->SetCumulatedAngle(acos(CosTheta));

    double EnergyStart = StartEnergy;      
    double EnergyFin = (EnergyStart * 0.5 * (1 + CosTheta));                      
    
    SlowingDownNeutron->InitEnergies(EnergyStart, EnergyFin);

    double Positive = (double(rand()) / RAND_MAX) * 100;

    el = SlowingDownNeutron->SampleLength();
    
    if(Positive <= 50)
    {
        psi = acos((1 + CosTheta) / (sqrt(1 + (2 * CosTheta) + 1)));
    }
    else
    {
        psi = -acos((1 + CosTheta) / (sqrt(1 + (2 * CosTheta) + 1)));
    }

    x = el*cos(psi);
    y = el*sin(psi);

    SlowingDownNeutron->SetPositions(x,y);

    SlowingDownNeutron->SetDiffuNb();
    
    SlowingDownNeutron->WriteCurrentPosition();

    // step2 (TO DO BELOW) : loop on the step1 for a complete trajectory

    while(EnergyFin > FinalEnergy)
    {

        SlowingDownNeutron->SetPositions(x,y);

        SlowingDownNeutron->SetDiffuNb();

        SlowingDownNeutron->WriteCurrentPosition();

        EpsilonTheta = double(rand()) / RAND_MAX;

        CosTheta = 2*EpsilonTheta - 1;
    
        EnergyStart = EnergyFin;      
        EnergyFin = (EnergyStart * 0.5 * (1 + CosTheta));                         
    
        SlowingDownNeutron->InitEnergies(StartEnergy, FinalEnergy);

        Positive = (double(rand()) / RAND_MAX) * 100;

        el = SlowingDownNeutron->SampleLength();
    
        if(Positive <= 50)
        {
            psi = acos((1 + CosTheta) / (sqrt(1 + (2 * CosTheta) + 1)));
        }
        else
        {
            psi = -acos((1 + CosTheta) / (sqrt(1 + (2 * CosTheta) + 1)));
        }

        x = el*cos(psi);
        y = el*sin(psi);
    
    }


    // step3 : loop on the step2 and mean on the diffusion number

    double mean = 0;

    for(int i = 0; i < NombreDeTrajectoire; i++)
    {
        SlowingDownNeutron->ResetParameters();

        el = SlowingDownNeutron->SampleLength();
        psi = 0;
        x = el*cos(psi);
        y = el*sin(psi);

        while(EnergyFin > FinalEnergy)
        {

            SlowingDownNeutron->SetPositions(x,y);

            SlowingDownNeutron->SetDiffuNb();

            SlowingDownNeutron->WriteCurrentPosition();

            EpsilonTheta = double(rand()) / RAND_MAX;

            CosTheta = 2*EpsilonTheta - 1;
    
            EnergyStart = EnergyFin;      
            EnergyFin = (EnergyStart * 0.5 * (1 + CosTheta));
            
            cout << "energie " << EnergyFin << endl;
    
            SlowingDownNeutron->InitEnergies(StartEnergy, FinalEnergy);

            Positive = (double(rand()) / RAND_MAX) * 100;

            el = SlowingDownNeutron->SampleLength();
    
            if(Positive <= 50)
            {
             psi = acos((1 + CosTheta) / (sqrt(1 + (2 * CosTheta) + 1)));
            }
            else
            {
               psi = -acos((1 + CosTheta) / (sqrt(1 + (2 * CosTheta) + 1)));
            }

            x = el*cos(psi);
            y = el*sin(psi);
    
        }

        mean += ( SlowingDownNeutron->GetDiffuNumber() );
    }

    mean /= NombreDeTrajectoire;

    cout << "Le nombre moyen d'impact est " << mean << endl;

}

