#include "Neutron.hxx"

//________________________________________________________________________
Neutron::Neutron(string n_name, string d_name)
{
    Neutron_Material = NULL;        // no address for this pointer yet (the Neutron does not know its Material yet)
    
    Neutron_Emax = 0;
    Neutron_Emin = 0;
    
    Neutron_DiffusionNumber = 0;
    Neutron_CumulatedAngle = 0;
    
    Neutron_PosX = 0;               // each neutron is supposed to be produced (by fission) in 0,0 (before drifting away by successive diffusions)
    Neutron_PosY = 0;
    
    Neutron_Name = n_name;      
    Neutron_DataDirName = d_name;
        
    // Output File Stream towards Neutron's trajectory data file is created (with a few format options)
    
    Neutron_TrajectoryFileName = Neutron_DataDirName + "/" + Neutron_Name + "_trajectory.dat";

	Neutron_TrajectoryOFStream = new ofstream(Neutron_TrajectoryFileName.c_str());
	Neutron_TrajectoryOFStream->setf(ios::scientific);
	Neutron_TrajectoryOFStream->setf(ios::left);
    
    // First "header" line of Neutron's trajectory data file is written (with n number of collisions already made and x,y the place where the nth collision happens)
    
	*Neutron_TrajectoryOFStream << setw(9) << "# n" << setw(20) << "x" << setw(20) << "y" << endl;        
}
//________________________________________________________________________
Neutron::~Neutron()
{
    delete Neutron_Material;
}
//________________________________________________________________________
void Neutron::InitEnergies(double init, double last)
{
    Neutron_Emax = init;
    Neutron_Emin = last;
}
//________________________________________________________________________
void Neutron::WriteCurrentPosition()
{
	*Neutron_TrajectoryOFStream << Neutron_DiffusionNumber << "\t\t" << Neutron_PosX << "\t\t" << Neutron_PosY << endl;  
}
//________________________________________________________________________
double Neutron::SampleLength()
{
	double epsilon = double(rand())/double(RAND_MAX);
    double Sigma_s = Neutron_Material->GetDiffusionCrossSection();
    
	double length = (-1.0/Sigma_s)*log(1-epsilon);
	return length;
}
//________________________________________________________________________
void Neutron::SetPositions(double x, double y)
{
    Neutron_PosX = x;
    Neutron_PosY = y;
}
//________________________________________________________________________
void Neutron::SetCumulatedAngle(double theta)
{
    Neutron_CumulatedAngle += theta;
}
//________________________________________________________________________
void Neutron::SetDiffuNb()
{
    Neutron_DiffusionNumber ++;
}
//________________________________________________________________________
void Neutron::ResetParameters()
{
    Neutron_Emax = 0;
    Neutron_Emin = 0;
    
    Neutron_DiffusionNumber = 0;
    Neutron_CumulatedAngle = 0;
    
    Neutron_PosX = 0;  
    Neutron_PosY = 0;
}
//_______________________________________________________________________
int Neutron::GetDiffuNumber()
{
    return Neutron_DiffusionNumber;
}
//_______________________________________________________________________
void Neutron::BuildTrajectory(Neutron* SlowingDownNeutron, double StartEnergy, double FinalEnergy)
{
    double el = SlowingDownNeutron->SampleLength();
    //cout << "the length of the very first segment has been sampled equal to " << el << " cm" << endl;
    
    double psi = 0;
    double x = el*cos(psi);
    double y = el*sin(psi);

    //cout << "the neutron is now in x = " << x << " and y = " << y << " (position of the first diffusion)" << endl;

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

    while(EnergyFin > FinalEnergy)
    {

        SlowingDownNeutron->SetPositions(x,y);

        SlowingDownNeutron->SetDiffuNb();

        SlowingDownNeutron->WriteCurrentPosition();

        EpsilonTheta = double(rand()) / RAND_MAX;

        CosTheta = 2*EpsilonTheta - 1;
    
        EnergyStart = EnergyFin;      
        EnergyFin = (EnergyStart * 0.5 * (1 + CosTheta));                         
    
        SlowingDownNeutron->InitEnergies(EnergyStart, EnergyFin);

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
}
//____________________________________________________________________________________________



