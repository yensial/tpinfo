#include "Neutron.hxx"
#include "chrono"

using namespace std;
using namespace std::chrono;

//________________________________________________________________________
Neutron::Neutron(string n_name, string d_name, string t_name)
{
    Neutron_Material = NULL;        // no address for this pointer yet (the Neutron does not know its Material yet)
    
    Neutron_Emax = 0;
    Neutron_Emin = 0;
    Neutron_Eactu = 0;
    
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
    
	*Neutron_TrajectoryOFStream << "#n" << "," <<"x" <<"," << "y" << endl;   

    Neutron_TimeDirName = t_name;
    Neutron_TimeFileName = Neutron_TimeDirName + "/" + Neutron_Name + "_time.dat";

	Neutron_TimeOFStream = new ofstream(Neutron_TimeFileName.c_str());
	Neutron_TimeOFStream->setf(ios::left);     
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
    Neutron_Eactu = init;
}
//________________________________________________________________________
void Neutron::WriteCurrentPosition()
{
	*Neutron_TrajectoryOFStream << Neutron_DiffusionNumber << "," << Neutron_PosX << "," << Neutron_PosY << endl;  
}
//________________________________________________________________________

void Neutron::WriteHistoireTime(auto start, auto end)
{
    double duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
    *Neutron_TimeOFStream << duration << endl;

    
}

//________________________________________________________________________
double Neutron::SampleLength()
{
	double epsilon = double(rand())/double(RAND_MAX);
    double* Sigma_s = Neutron_Material->GetDiffusionCrossSection();

    double Sigma_tot = Sigma_s[0] + Sigma_s[1];
    
	double length = (-1.0/Sigma_tot)*log(1-epsilon);
	return length;
}
//________________________________________________________________________
void Neutron::SetPositions(double x, double y)
{
    Neutron_PosX = x;
    Neutron_PosY = y;
}
//________________________________________________________________________
void Neutron::CumulateAngle(double psi)
{
    Neutron_CumulatedAngle += psi;
}
//________________________________________________________________________
void Neutron::SetDiffuNb()
{
    Neutron_DiffusionNumber ++;
}
//________________________________________________________________________
void Neutron::ResetParameters()
{
    Neutron_Eactu = Neutron_Emax;
    
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
void Neutron::BuildTrajectory()
{
    auto start = chrono::high_resolution_clock::now();
    double el = 0;
    double psi = 0;
    double EpsilonTheta = 0;
    double CosTheta = 0;
    double A = 0;
    double* MassNumber = NULL;
    double* Sigma = NULL;
    double EpsilonMaterial;
    double Positive;

    while(Neutron_Eactu > Neutron_Emin)
    {
        

        SetDiffuNb();

        EpsilonTheta = double(rand()) / RAND_MAX;

        CosTheta = 2*EpsilonTheta - 1;

        EpsilonMaterial = double(rand()) / RAND_MAX;

        MassNumber = Neutron_Material->GetMassNumber();

        Sigma = Neutron_Material->GetDiffusionCrossSection();

        if( EpsilonMaterial < (Sigma[0]/Sigma[1]))
        {
            A = MassNumber[0];

        }
        else
        {
            A = MassNumber[1];
        }
        Neutron_Material->SetSlowingDownParameter(A);

        Neutron_Eactu = (Neutron_Eactu * ( ( A*A + 2*A*CosTheta + 1)/((A+1)*(A+1))));                          

        Positive =2*  (double(rand()) / RAND_MAX) -1;
        Positive = Positive / abs(Positive);
        psi = Positive * acos((1 + CosTheta) / (sqrt(1 + (2 * CosTheta) + 1)));
        

        CumulateAngle(psi);

        el = SampleLength();

        Neutron_PosX = el*cos(psi);
        Neutron_PosY = el*sin(psi);
    
    }
    auto end = chrono::high_resolution_clock::now();
}
//____________________________________________________________________________________________
Material* Neutron::GetMaterial()
{
    return Neutron_Material;
}
//____________________________________________________________________________________________
void Neutron::GetPositions(double position[2])
{
    position[0] = Neutron_PosX;
    position[1] = Neutron_PosY;
} 
//____________________________________________________________________________________________
double Neutron::GetEnergy()
{
    return Neutron_Eactu;
}
