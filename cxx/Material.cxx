#include "Material.hxx"

//________________________________________________________________________
Material::Material()
{        
    Material_MassNumber = 0;           
    Material_SlowingDownParameter = 0;         
    Material_DiffusionCrossSection = 0;
}
//________________________________________________________________________
Material::~Material()
{

}
//________________________________________________________________________
void Material::SetMainParameters(double A, double Sigma_s)
{
    Material_MassNumber = A;
    
    Material_SlowingDownParameter = (A-1)*(A-1);
    Material_SlowingDownParameter /= (A+1)*(A+1);

    Material_DiffusionCrossSection = Sigma_s;
}
//________________________________________________________________________
double Material::GetMeanNbChoc(double ksi)
{
    MeanNbChoc = 13.8/ksi;
    return MeanNbChoc;
}










