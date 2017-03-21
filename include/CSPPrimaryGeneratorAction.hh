#ifndef CSPPrimaryGeneratorAction_h
#define CSPPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "CSPDetectorConstruction.hh"

class G4GeneralParticleSource;
class G4Event;

class CSPPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    CSPPrimaryGeneratorAction(CSPDetectorConstruction *det);
    ~CSPPrimaryGeneratorAction();

  public:
    void GeneratePrimaries(G4Event* anEvent);

  private:
  G4GeneralParticleSource* particleGun;
  CSPDetectorConstruction* CSPDetector;
};

#endif


