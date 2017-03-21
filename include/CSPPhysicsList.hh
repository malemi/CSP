#ifndef CSPPhysicsList_h
#define CSPPhysicsList_h 1

#include <G4OpBoundaryProcess.hh>
#include <G4OpAbsorption.hh>
#include <G4VUserPhysicsList.hh>
#include "globals.hh"

class CSPPhysicsList: public G4VUserPhysicsList
{
  public:
    CSPPhysicsList();
    ~CSPPhysicsList();

  protected:
    // Construct particle and physics process
    void ConstructParticle();
    void ConstructProcess();
    void SetCuts();

private:
  G4OpBoundaryProcess* theBoundaryProcess;
  G4OpAbsorption* theAbsorptionProcess;

};

#endif







