#include "Material.hxx"

//________________________________________________________________________
Material::Material()
{        
    Material_MassNumber[0] = 0.0;
    Material_MassNumber[1] = 0.0;           
    Material_SlowingDownParameter = 0;         
    Material_DiffusionCrossSection[0] = 0.0;
    Material_DiffusionCrossSection[1] = 0.0;
}
//________________________________________________________________________
Material::~Material()
{

}
//________________________________________________________________________
void Material::SetMassNumberCrossSection(double A, double B, double Sigma_0, double Sigma_1)
{
    Material_MassNumber[0] = A;
    Material_MassNumber[1] = B;

    Material_DiffusionCrossSection[0] = Sigma_0;
    Material_DiffusionCrossSection[1] = Sigma_1;
}
//________________________________________________________________________
double Material::GetMeanNbChoc(double ksi)
{
    MeanNbChoc = 13.8/ksi;
    return MeanNbChoc;
}
//________________________________________________________________________
double* Material::GetMassNumber()
{
    return Material_MassNumber;
}
//_________________________________________________________________________
void Material::SetSlowingDownParameter(double A)
{
    Material_SlowingDownParameter = (A-1)*(A-1);
    Material_SlowingDownParameter /= (A+1)*(A+1);
}
//_________________________________________________________________________
