//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************

#include "CSPEventAction.hh"

#include "CSPRunAction.hh"
#include "CSPHistoManager.hh"
// #include "EventMessenger.hh"

#include "G4Event.hh"
#include "G4SDManager.hh"
// #include "G4TrajectoryContainer.hh"
// #include "G4Trajectory.hh"
// #include "G4VVisManager.hh"


CSPEventAction::CSPEventAction(CSPRunAction* RA)
  :runaction(RA), drawFlag("none"),printModulo(1000)
{
  //  eventMessenger = new EventMessenger(this);
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  histoManager = CSPHistoManager::getInstance(); 
}

CSPEventAction::~CSPEventAction()
{
  //  delete eventMessenger;
}

void CSPEventAction::BeginOfEventAction(const G4Event* evt)
{
 G4int evtNb = evt->GetEventID();

 //printing survey
 if (evtNb%printModulo == 0) 
    G4cout << "---> Begin of Event: " << evtNb << G4endl;

 // initialisation per event
 EnergyDeposit  = EnergySecondary = 0.;
}

void CSPEventAction::EndOfEventAction(const G4Event* evt)
{
 runaction->AddEnergyDeposit(EnergyDeposit);

 //  histoManager->FillHisto(1, 7.*deg);//EnergyDeposit);
//  histoManager->FillHisto(2, EnergySecondary);
//  histoManager->FillHisto(3, EnergyDeposit+EnergySecondary);

//  if (G4VVisManager::GetConcreteInstance())
//   {
//    G4TrajectoryContainer* trajectoryContainer = evt->GetTrajectoryContainer();
//    G4int n_trajectories = 0;
//    if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();  
//    for (G4int i=0; i<n_trajectories; i++) 
//       { G4Trajectory* trj = (G4Trajectory*)
//                                       ((*(evt->GetTrajectoryContainer()))[i]);
//         if (drawFlag == "all") trj->DrawTrajectory(1000);
//         else if ((drawFlag == "charged")&&(trj->GetCharge() != 0.))
//                                trj->DrawTrajectory(1000);
//         else if ((drawFlag == "neutral")&&(trj->GetCharge() == 0.))
//                                trj->DrawTrajectory(1000);
			        
//       }
//   }  
}


