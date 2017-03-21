#include "SensorSD.hh"
#include "CSPHistoManager.hh"

#include <G4HCofThisEvent.hh>
#include <G4Step.hh>
#include <G4ThreeVector.hh>
#include <G4SDManager.hh>
#include <G4ios.hh>
#include <G4TrackStatus.hh>

SensorSD::SensorSD(G4String name)
:G4VSensitiveDetector(name)
{
}

SensorSD::~SensorSD(){ }


void SensorSD::Initialize(G4HCofThisEvent* HCE)
{
}


G4bool SensorSD::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{

  CSPHistoManager* histo = CSPHistoManager::getInstance(); 

   G4Track* t = aStep->GetTrack();
   G4String PosVol=aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();
//    G4cout << "NAME " << PosVol << G4endl;
   
//    t->SetTrackStatus(fStopAndKill);

//  count particles hitting probe volume
//     if(aStep->GetTrack()->GetNextVolume()) {
//       static int i=0;
//       G4String PosVol=aStep->GetPreStepPoint()
//         ->GetPhysicalVolume()->GetName();
//       G4cout << "NAME " << PosVol << G4endl;
//       if(PosVol=="receiver") {
//         aStep->GetTrack()->SetTrackStatus(fStopAndKill);
//         G4cout << i++ << G4endl;
//       }
//     }



  G4ThreeVector currentPosition = t->GetPosition();
  
  // G4cout <<  currentPosition.x() << " " << currentPosition.y()  << " " << currentPosition.z() << G4endl; //that's OK


  G4double phi = currentPosition.getPhi() ;
  G4double theta = currentPosition.getTheta() ;

  if(abs(phi)> 170*deg)    theta = (180*deg - theta) + 180*deg; 
  
  histo->FillHisto(1, theta, 1.);

  return true;
}

void SensorSD::EndOfEvent(G4HCofThisEvent*)
{
}

