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

#include "CSPDetectorMessenger.hh"

#include "CSPDetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

CSPDetectorMessenger::CSPDetectorMessenger(
                                           CSPDetectorConstruction* CSPDet)
:CSPDetector(CSPDet)
{ 
  cspDir = new G4UIdirectory("/csp/");
  cspDir->SetGuidance("UI commands of this application");
  
  detDir = new G4UIdirectory("/csp/det/");
  detDir->SetGuidance("detector control");
       
    
  SecTrR1Cmd = new G4UIcmdWithADoubleAndUnit("/csp/det/setBottomTroughWidth",this);
  SecTrR1Cmd->SetGuidance("Set the width of the bottom mirror/aperture of the top");
  SecTrR1Cmd->SetParameterName("Size",false);
  SecTrR1Cmd->SetRange("Size>=0.");
  SecTrR1Cmd->SetUnitCategory("Length");
  SecTrR1Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  SecTrR2Cmd = new G4UIcmdWithADoubleAndUnit("/csp/det/setTopTroughWidth",this);
  SecTrR2Cmd->SetGuidance("Set the width of the top mirror");
  SecTrR2Cmd->SetParameterName("Size",false);
  SecTrR2Cmd->SetRange("Size>=0.");
  SecTrR2Cmd->SetUnitCategory("Length");
  SecTrR2Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  SecRimCmd = new G4UIcmdWithADoubleAndUnit("/csp/det/setRimAngle",this);
  SecRimCmd->SetGuidance("Set the rim angle");
  SecRimCmd->SetParameterName("Size",false);
  SecRimCmd->SetRange("Size>=0.");
  SecRimCmd->SetUnitCategory("Angle");
  SecRimCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  UpdateCmd = new G4UIcmdWithoutParameter("/csp/det/update",this);
  UpdateCmd->SetGuidance("Update calorimeter geometry.");
  UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  UpdateCmd->SetGuidance("if you changed geometrical value(s).");
  UpdateCmd->AvailableForStates(G4State_Idle);
}

CSPDetectorMessenger::~CSPDetectorMessenger()
{
  delete SecTrR1Cmd;
  delete SecTrR2Cmd;
  delete SecRimCmd;
  delete UpdateCmd;
  delete detDir;
  delete cspDir;  
}

void CSPDetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  
  if(command == SecTrR1Cmd){
    CSPDetector->setSecondTrough_r1(SecTrR1Cmd->GetNewDoubleValue(newValue));
  }
  
  if(command == SecTrR2Cmd){
    CSPDetector->setSecondTrough_r2(SecTrR2Cmd->GetNewDoubleValue(newValue));
  }
  
  if(command == SecRimCmd){
    CSPDetector->setRim_angle(SecRimCmd->GetNewDoubleValue(newValue));
  }
  
  if(command == UpdateCmd )
   { CSPDetector->UpdateGeometry(); }
}

