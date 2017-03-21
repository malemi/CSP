#ifndef SensorSD_h
#define SensorSD_h 1

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;

class SensorSD : public G4VSensitiveDetector
{
public:
  SensorSD(G4String);
  ~SensorSD();
  
  void Initialize(G4HCofThisEvent*);
  G4bool ProcessHits(G4Step*, G4TouchableHistory*);
  void EndOfEvent(G4HCofThisEvent*);
  
private:
  //  CSPSensorHitsCollection* hitsCollection;
  G4int SENSOR_ID;


  
};

#endif

