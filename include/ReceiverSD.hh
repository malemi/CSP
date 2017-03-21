#ifndef ReceiverSD_h
#define ReceiverSD_h 1

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ReceiverSD : public G4VSensitiveDetector
{
public:
  ReceiverSD(G4String);
  ~ReceiverSD();
  
  void Initialize(G4HCofThisEvent*);
  G4bool ProcessHits(G4Step*, G4TouchableHistory*);
  void EndOfEvent(G4HCofThisEvent*);
  
private:
  //  CSPReceiverHitsCollection* hitsCollection;
  G4int SENSOR_ID;


  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

