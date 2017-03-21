#include "CSPPhysicsList.hh"
#include <G4ParticleTypes.hh>
#include <G4OpBoundaryProcess.hh>
#include <G4OpAbsorption.hh>
#include <G4ProcessManager.hh>

CSPPhysicsList::CSPPhysicsList()
{;}

CSPPhysicsList::~CSPPhysicsList()
{;}

void CSPPhysicsList::ConstructParticle()
{
  // In this method, static member functions should be called
  // for all particles which you want to use.
  // This ensures that objects of these particle types will be
  // created in the program. 
  G4Geantino::GeantinoDefinition(); 
  G4OpticalPhoton::OpticalPhotonDefinition();
}

void CSPPhysicsList::ConstructProcess()
{
  // Define transportation process

  AddTransportation();

  theBoundaryProcess   = new G4OpBoundaryProcess();
  theAbsorptionProcess = new G4OpAbsorption();

  G4ProcessManager * pManager = 0;
  
  pManager = G4OpticalPhoton::OpticalPhoton()->GetProcessManager();
  theBoundaryProcess->SetVerboseLevel(0);
  theBoundaryProcess->SetModel(glisur);

  pManager->AddDiscreteProcess(theBoundaryProcess);
  pManager->AddDiscreteProcess(theAbsorptionProcess);

}

void CSPPhysicsList::SetCuts()
{
  // uppress error messages even in case e/gamma/proton do not exist            
  G4int temp = GetVerboseLevel();                                               
  SetVerboseLevel(0);                                                           
  //  " G4VUserPhysicsList::SetCutsWithDefault" method sets 
  //   the default cut value for all particle types 
  SetCutsWithDefault();   

  // Retrieve verbose level
  SetVerboseLevel(temp);  
}

