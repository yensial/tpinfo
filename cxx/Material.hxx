#ifndef _MATERIAL_
#define _MATERIAL_

#include "misc.hxx"

class Material
{
  public:
 	
	Material();             // constructor (several versions are possible)
	~Material();            // destructor (to free memory when the object is removed, using "delete" if needed)

    void SetMainParameters(double A, double Sigma_s);
    double GetDiffusionCrossSection(){return Material_DiffusionCrossSection;};
    
    // DEFINITION OF NEW (PUBLIC) METHODS TO ADD BELOW



  private:
   
    double Material_MassNumber;           
    double Material_SlowingDownParameter;       // the so-called alpha parameter
    double Material_DiffusionCrossSection;      // in cm-1 (1.3 for H, 0.5 for C)

    // DEFINITION OF NEW (PRIVATE) ATTRIBUTES TO ADD BELOW

    double MeanNbChoc;


};

#endif
