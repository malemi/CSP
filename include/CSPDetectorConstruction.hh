#ifndef CSPDetectorConstruction_H
#define CSPDetectorConstruction_H 1

class G4LogicalVolume;
class G4VPhysicalVolume;

#include "G4VUserDetectorConstruction.hh"
#include "CSPDetectorMessenger.hh"
#include <G4Material.hh>
#include <G4OpticalSurface.hh>

#include "G4VSensitiveDetector.hh"

class CSPDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  
  CSPDetectorConstruction();
  ~CSPDetectorConstruction();
  
  G4VPhysicalVolume* Construct();
  void DefineMaterials();


  void UpdateGeometry();
  
  inline void setSecondTrough_r1(G4double secTr1){
    secondTrough_r1 = secTr1;  }  
  inline void setSecondTrough_r2(G4double secTr2){
    secondTrough_r2 = secTr2;  }
  inline void setRim_angle(G4double a){
    rim_angle = a;  }

  inline G4double getSecondTrough_r1(){
    return secondTrough_r1;  }  
  inline G4double getSecondTrough_r2(){
    return secondTrough_r2;  }
  inline G4double getRim_angle(){
    return rim_angle;  }
 
  
private:
  
  // All managed (deleted) by SDManager
  G4VSensitiveDetector* sensor;
  G4VSensitiveDetector* receiver;

  void  ComputeTroughsParameters();

  CSPDetectorMessenger* detectorMessenger;  //pointer to the Messenger

  //materials
  G4Material* Air;
  G4Material* Al;
  G4Material* Vacuum;
  G4Material* Glass;
  G4Material* Steel;

  G4OpticalSurface* receiver_opsurf;
  G4OpticalSurface *trough_opsurf;

  G4double firstTrough_r1;
  G4double firstTrough_r2;
  G4double firstTrough_dz;
  G4double   firstTroughPos_x;
  G4double   firstTroughPos_y;
  G4double   firstTroughPos_z;
  
  G4double secondTrough_r1;
  G4double secondTrough_r2;
  G4double secondTrough_dz;
  G4double   secondTroughLeftPos_x;
  G4double   secondTroughRightPos_x;
  G4double   secondTroughPos_y;
  G4double   secondTroughPos_z;
  
  G4double rim_angle;


  // Logical volumes
  G4LogicalVolume* experimentalHall_log;
  G4LogicalVolume* receiver_log;
  G4LogicalVolume* vacuum_log;
  G4LogicalVolume* vacuum_receiver_log;
  G4LogicalVolume* firstTrough_log;
  G4LogicalVolume* secondTroughLeft_log;
  G4LogicalVolume* secondTroughRight_log;
  G4LogicalVolume* glass_log;
  
  // Physical volumes
  G4VPhysicalVolume* experimentalHall_phys;
  G4VPhysicalVolume* receiver_phys;
  G4VPhysicalVolume* vacuum_phys;
  G4VPhysicalVolume* vacuum_receiver_phys;
  G4VPhysicalVolume* firstTrough_phys;
  G4VPhysicalVolume* secondTroughLeft_phys;
  G4VPhysicalVolume* secondTroughRight_phys;
  G4VPhysicalVolume* glass_phys;
};

#endif

