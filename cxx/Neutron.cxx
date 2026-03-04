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
    Neutron_E=0;
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
    
	*Neutron_TrajectoryOFStream << "#n" <<"," <<"x" <<"," << "y" << endl; 
    
    
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
    Neutron_E = Neutron_Emax;
    double el = SampleLength();
    
    double psi = 0;
    double x = el*cos(psi);
    double y = el*sin(psi);


    double EpsilonTheta = double(rand()) / RAND_MAX;

    double CosTheta = 2*EpsilonTheta - 1;
    
    this->SetCumulatedAngle(psi);

    double A = 0;

    Material* NeutronMaterial = this->GetMaterial();
    double* MassNumber = NeutronMaterial->GetMassNumber();
    double* Sigma = NeutronMaterial->GetDiffusionCrossSection();

    double EpsilonMaterial = double(rand()) / RAND_MAX;

    if( EpsilonMaterial < (Sigma[0]/Sigma[1]))
    {
        A = MassNumber[0];
        NeutronMaterial->SetSlowingDownParameter(Sigma[0]);

    }
    else
    {
        A = MassNumber[1];
        NeutronMaterial->SetSlowingDownParameter(Sigma[1]);
    }

    this->SetMaterial(NeutronMaterial);
      
    Neutron_E = (Neutron_E * ( ( A*A + 2*A*CosTheta + 1)/((A+1)*(A+1))));                  

    double Positive = (double(rand()) / RAND_MAX) * 100;

    el = this->SampleLength();
    
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

    this->SetPositions(x,y);

    this->SetDiffuNb();




    while(Neutron_E > Neutron_Emin)
    
    {

        this->SetPositions(x,y);

        this->SetDiffuNb();


        EpsilonTheta = double(rand()) / RAND_MAX;

        CosTheta = 2*EpsilonTheta - 1;

        EpsilonMaterial = double(rand()) / RAND_MAX;

        if( EpsilonMaterial < (Sigma[0]/Sigma[1]))
        {
            A = MassNumber[0];
            NeutronMaterial->SetSlowingDownParameter(Sigma[0]);

        }
        else
        {
            A = MassNumber[1];
            NeutronMaterial->SetSlowingDownParameter(Sigma[1]);
        }

        this->SetMaterial(NeutronMaterial);

        
         
        Neutron_E = (Neutron_E * ( ( A*A + 2*A*CosTheta + 1)/((A+1)*(A+1))));                          


        Positive = (double(rand()) / RAND_MAX) * 100;

        el = this->SampleLength();
    
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
    auto end = chrono::high_resolution_clock::now();
    this->WriteHistoireTime(start, end);
    this->WriteCurrentPosition();
}
//____________________________________________________________________________________________
Material* Neutron::GetMaterial()
{
    return Neutron_Material;
}
