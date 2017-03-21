
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#include "CSPDetectorConstruction.hh"
#include "CSPPhysicsList.hh"
#include "CSPPrimaryGeneratorAction.hh"
#include "CSPRunAction.hh"
#include "CSPEventAction.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

int main()
{
  // Construct the default run manager
  //
  G4RunManager* runManager = new G4RunManager;

  // set mandatory initialization classes
  //
  CSPDetectorConstruction* detector = new CSPDetectorConstruction;
  runManager->SetUserInitialization(detector);
  //
  CSPPhysicsList* physics = new CSPPhysicsList;
  runManager->SetUserInitialization(physics);

  // set mandatory user action class
  //
  CSPPrimaryGeneratorAction* gen_action = new CSPPrimaryGeneratorAction(detector);
  runManager->SetUserAction(gen_action);


  //runAction
  CSPRunAction* runaction = new CSPRunAction(detector, gen_action);
  runManager->SetUserAction(runaction);


  //eventAction
  CSPEventAction* eventaction = new CSPEventAction(runaction);
  runManager->SetUserAction(eventaction);


  // Initialize G4 kernel
  //
  runManager->Initialize();

  // Get the pointer to the UI manager and set verbosities
  //
  G4UImanager* UI = G4UImanager::GetUIpointer();

#ifdef G4VIS_USE
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif    

  G4UIsession * session = 0;
#ifdef G4UI_USE_TCSH
  session = new G4UIterminal(new G4UItcsh);      
#else
  session = new G4UIterminal();
#endif
  //  UI->ApplyCommand("/control/execute vis.mac");     
  session->SessionStart();
  delete session;


  // Job termination
  //
  // Free the store: user actions, physics_list and detector_description are
  //                 owned and deleted by the run manager, so they should not
  //                 be deleted in the main() program !
  //
  delete runManager;

#ifdef G4VIS_USE
      delete visManager;
#endif     

  return 0;
}


