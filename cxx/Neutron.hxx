#ifndef _NEUTRON_
#define _NEUTRON_

#include "misc.hxx"
#include "Material.hxx"     // added because a Neutron object needs to know the Material where it's slowing-down (cf. Neutron's first private attribute, below)

class Neutron
{
  public:
 	
    Neutron(string n_name, string d_name);          // constructor (several versions are possible, this one asks for both neutron and data directory names)
	  ~Neutron();                                     // destructor (to free memory when the object is removed, using "delete")

    void SetMaterial(Material* material){Neutron_Material = material;};     // inline definition of Neutron_Material (no need to define this method in Neutron.cxx)
    void InitEnergies(double init, double last);                            // definition of initial and final kinetic energies
    
    void WriteCurrentPosition();                                            // writes in the trajectory file the current time and the Neutron's positions at this time
    double SampleLength();                                                  // samples of a new segment length by using cstdlib's rand() method

    // DEFINITION OF NEW (PUBLIC) METHODS TO ADD BELOW

    void SetPositions(double x, double y);
    void SetCumulatedAngle(double theta);
    void SetDiffuNb();
    void ResetParameters();
    int GetDiffuNumber();
    void BuildTrajectory(double SartEnergy, double FinalEnergy);




  private:
  
    Material* Neutron_Material;             // pointer to the object of the Material class which the Neutron object needs to know (for access to its data)

    double Neutron_Emax;                    // kinetic energy the neutron has when it appears (2 MeV if produced by fission)
    double Neutron_Emin;                    // kinetic energy set as goal (for estimating how many collisions are necessary on average to reach such a low energy)
    
    int Neutron_DiffusionNumber;            // current number of diffusions already performed by the neutron
    double Neutron_CumulatedAngle;          // cumulated angle in LAB after all diffusions already performed (to be used for the new position)
    
    double Neutron_PosX;                    // current x position of the Neutron
    double Neutron_PosY;

    string Neutron_Name;                    // "SlowingDownNeutron" for instance
	  string Neutron_DataDirName;             // "../dat" (for the Neutron to know where to write each new data file, at least one for each trajectory)
    	
	  string Neutron_TrajectoryFileName;      // name of the current trajectory file (composed of a few segments, until its energy goes under Neutron_Emin)
	  ofstream *Neutron_TrajectoryOFStream;   // Output File Stream towards Neutron's current trajectory data file (for writing the successive positions x and y)

    // DEFINITION OF NEW (PRIVATE) ATTRIBUTES TO ADD BELOW



};

#endif
