#include "CSPDetectorConstruction.hh"
#include "CSPDetectorMessenger.hh"

#include <G4Material.hh>
#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4EllipticalTube.hh>
#include <G4Paraboloid.hh>
#include <G4Sphere.hh>
#include <G4BREPSolidCylinder.hh>
#include <G4BREPSolidTorus.hh>
#include <G4BREPSolidSphere.hh>

#include <G4OpticalSurface.hh>
#include <G4LogicalBorderSurface.hh>
#include <G4LogicalSkinSurface.hh>
#include <G4MaterialPropertiesTable.hh>
#include <G4SubtractionSolid.hh>
#include <G4VisAttributes.hh>

#include <G4LogicalVolume.hh>
#include <G4ThreeVector.hh>
#include <G4PVPlacement.hh>
#include <globals.hh>
#include <G4SDManager.hh>

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "ReceiverSD.hh"
#include "SensorSD.hh"

CSPDetectorConstruction::CSPDetectorConstruction()
 :  experimentalHall_log(0), receiver_log(0),
    vacuum_log(0), vacuum_receiver_log(0),
    firstTrough_log(0),  
    secondTroughLeft_log(0), 
    secondTroughRight_log(0), 
    experimentalHall_phys(0), receiver_phys(0),
    vacuum_phys(0), vacuum_receiver_phys(0),
    firstTrough_phys(0),  
    secondTroughLeft_phys(0),
    secondTroughRight_phys(0) 
{
  // default parameter values of the calorimeter
  setSecondTrough_r1(5*mm);
  setSecondTrough_r2(10.0*m); //r2 always 5.0
  setRim_angle(90*deg);

  // materials
  DefineMaterials();

  // create commands for interactive definition of the calorimeter
  detectorMessenger = new CSPDetectorMessenger(this);
}

CSPDetectorConstruction::~CSPDetectorConstruction()
{
  delete detectorMessenger;
}

void CSPDetectorConstruction::DefineMaterials()
{

  G4double a;  // atomic mass
  G4double z;  // atomic number
  G4double density;
  G4String name;
  G4String symbol;
  G4int nel;

  // Air
  a = 14.01*g/mole;
  G4Element* elN = new G4Element(name="Nitrogen", symbol="N2", z=7., a);

  a = 16.00*g/mole;
  G4Element* elO = new G4Element(name="Oxygen", symbol="O2", z=8., a);
 
  G4Element* H = new G4Element("H", "H", z=1., a=1.01*g/mole);
  G4Element* C = new G4Element("C", "C", z=6., a=12.01*g/mole);

  G4Element* Fe  = new G4Element("Iron","Fe",z = 26., a=55.85*g/mole);

  a = 52.00*g/mole;
  G4Element* Cr  = new G4Element("Chromium","Cr",z = 24.,a);

  a = 58.70*g/mole;
  G4Element* Ni  = new G4Element("Nickel","Ni",z = 28.,a);

  density = 1.290*mg/cm3;
  Air = new G4Material(name="Air", density, nel=2);
  Air->AddElement(elN, 0.7);
  Air->AddElement(elO, 0.3);

  // Aluminum
  Al = new G4Material("Al",z=13.,a=26.98*g/mole,density=2.7*g/cm3);

  // Vacuum
  Vacuum =
    new G4Material("Vacuum", z=1., a=1.01*g/mole,density= universe_mean_density,
		   kStateGas, 2.73*kelvin, 3.e-18*pascal);

  // Glass
  Glass = new G4Material("Glass", density=1.032*g/cm3,2);
  Glass->AddElement(C,91.533*perCent);
  Glass->AddElement(H,8.467*perCent);

  // Stainless steel
  density = 8.02*g/cm3 ;
  Steel = new G4Material("Steel",density,5);
  Steel -> AddElement(Cr, 0.20);
  Steel -> AddElement(Ni, 0.10);
  Steel -> AddElement(Fe, 0.70);

  const G4int nEntries = 2;
  G4double PhotonEnergy[nEntries] =
            { .1*eV, 10.0*eV };
  G4double vacuumRefractiveIndex[nEntries] =
            { 1.0,   1.0};
  G4double airRefractiveIndex[nEntries] =
            { 1.00,  1.00 };
  G4double glassRefractiveIndex[nEntries] =
            { 1.55,  1.55 };

  G4MaterialPropertiesTable* vacuumMPT = new G4MaterialPropertiesTable();
  vacuumMPT->AddProperty("RINDEX", PhotonEnergy, vacuumRefractiveIndex, nEntries);

  G4MaterialPropertiesTable* airMPT = new G4MaterialPropertiesTable();
  airMPT->AddProperty("RINDEX", PhotonEnergy, airRefractiveIndex, nEntries);

  G4MaterialPropertiesTable* glassMPT = new G4MaterialPropertiesTable();
  glassMPT->AddProperty("RINDEX", PhotonEnergy, glassRefractiveIndex, nEntries);

  Air->SetMaterialPropertiesTable(airMPT);
  Vacuum->SetMaterialPropertiesTable(vacuumMPT);
  Glass->SetMaterialPropertiesTable(glassMPT);

  ///////////////////////////////////
  // Optical Surfaces -- ref. to UltraDetectorConstruction.cc at the moment (4/3/09)

//   G4OpticalSurface* trough_opsurf= 
//     new G4OpticalSurface("trough_opsurf",glisur,polished,
// 			 dielectric_metal);
  
  trough_opsurf = new G4OpticalSurface("trough_opsurf");
  trough_opsurf->SetModel(unified);
  //  trough_opsurf->SetPolish(.9);
  trough_opsurf->SetType(dielectric_dielectric);
  trough_opsurf->SetFinish(polishedfrontpainted);

  //  G4double trough_EFF[nEntries]={0.0,0.0}; 
  G4double trough_REFL[nEntries]={1.,1.};
  //Cfr RichTbMaterial.cc and the strange comment at line 823 "to avoid reflection at the mirror"
  //G4double trough_RIDX[nEntries]={100.,100.};

  G4MaterialPropertiesTable* trough_surf_mt = new G4MaterialPropertiesTable();
  //trough_surf_mt->AddProperty("EFFICIENCY",PhotonEnergy,trough_EFF,nEntries);
  trough_surf_mt->AddProperty("REFLECTIVITY",PhotonEnergy,trough_REFL,nEntries);
  //trough_surf_mt->AddProperty("RINDEX",PhotonEnergy,trough_RIDX,nEntries);

   trough_opsurf->SetMaterialPropertiesTable(trough_surf_mt);


//    G4LogicalBorderSurface *AirMirror ;
//    AirMirror = new G4LogicalBorderSurface("Air/Mirror Surface", experimentalHall_phys, secondTrough_phys, trough_opsurf);



  G4double receiver_EFF[nEntries]={.0,.0}; 
  G4double receiver_REFL[nEntries]={.0,.0};
  G4double receiver_RIDX[nEntries]={1.5,1.5};

  G4MaterialPropertiesTable* receiver_surf_mt = new G4MaterialPropertiesTable();
  receiver_surf_mt->AddProperty("EFFICIENCY",PhotonEnergy,receiver_EFF,nEntries);
  receiver_surf_mt->AddProperty("REFLECTIVITY",PhotonEnergy,receiver_REFL,nEntries);
  receiver_surf_mt->AddProperty("RINDEX",PhotonEnergy,receiver_RIDX,nEntries);

  receiver_opsurf= 
    new G4OpticalSurface("receiver_opsurf",glisur,polished,
			 dielectric_metal);
  receiver_opsurf->SetMaterialPropertiesTable(receiver_surf_mt);
}

G4VPhysicalVolume* CSPDetectorConstruction::Construct()
{


  // Clean old geometry, if any
  //
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  ComputeTroughsParameters();

  //------------------------------------------------------ volumes
  G4double expHall_x = 2*secondTrough_r2;
  G4double expHall_y = 2*m;
  G4double expHall_z = secondTrough_r2;

  G4Box* experimentalHall_box
    = new G4Box("expHall_box",expHall_x,expHall_y,expHall_z);
  experimentalHall_log = new G4LogicalVolume(experimentalHall_box,
                                             Air,"expHall_log",0,0,0);
  experimentalHall_phys = new G4PVPlacement(0,G4ThreeVector(0,0,0),
                                      experimentalHall_log,"expHall",0,false,0);

  // ----------------------------- Common 
  G4double halfLengthOfTheTube = 50.*mm;
  G4double startAngleOfTheTube = 0.;
  G4double spanningAngleOfTheTube = 360*deg;

  //------------------------------ glass tube
  G4double  innerRadiusOfTheGlass = 5.0*cm;
  G4double  outerRadiusOfTheGlass = 5.2*cm;

  //------------------------------ receiver tube
  G4double  innerRadiusOfTheReceiver = 0.0*cm;
  G4double  outerRadiusOfTheReceiver = 3.5*cm;

  //------------------------------ first trough 

  G4Paraboloid * firstTrough_tube = new G4Paraboloid( "firstTrough_tube", 
						      firstTrough_dz,
						      firstTrough_r1,
						      firstTrough_r2 );
  G4Box * firstTrough_box = new G4Box ( "firstTrough_box", 
					firstTrough_r2,
					0.5*m,
					firstTrough_dz+1*cm);//don't want a 0mm thick mirror....

  G4RotationMatrix * zero_rot = new G4RotationMatrix(0,0,0);

  G4SubtractionSolid * firstTrough_surface = 
    new G4SubtractionSolid ("firstTrough_surface",firstTrough_box,firstTrough_tube, zero_rot, 
			    G4ThreeVector(0,0,1*cm));

  firstTrough_log = new G4LogicalVolume(firstTrough_surface,Al,"firstTrough_log",0,0,0);

  G4RotationMatrix * firstTrough_rot = new G4RotationMatrix(0,0,0);

     firstTrough_phys = new G4PVPlacement(firstTrough_rot,
 				  G4ThreeVector(firstTroughPos_x,firstTroughPos_y,firstTroughPos_z-1*cm),
 				  firstTrough_log,"firstTrough",experimentalHall_log,false,0);


//------------------------------ second trough left + right
  G4Paraboloid * secondTrough_tube = new G4Paraboloid( "secondTrough_tube", 
					       secondTrough_dz,
					       secondTrough_r1,
					       secondTrough_r2 );

  G4Box * secondTrough_box = new G4Box ( "secondTrough_box", 
					       secondTrough_r2/2,
					       0.5*m,
					       secondTrough_dz );

  G4RotationMatrix* rotNull = new G4RotationMatrix(0,0,0);

  G4SubtractionSolid * secondTroughLeft_surface = new
    G4SubtractionSolid ("secondTroughLeft_surface",
			secondTrough_box,
			secondTrough_tube, 
			rotNull, 
			G4ThreeVector(secondTrough_r2/2,0,0));

  G4SubtractionSolid * secondTroughRight_surface = new
    G4SubtractionSolid ("secondTroughRight_surface",
			secondTrough_box,
			secondTrough_tube, 
			rotNull, 
			G4ThreeVector(-secondTrough_r2/2,0,0));

  secondTroughLeft_log = new G4LogicalVolume(secondTroughLeft_surface, 
					     Al, 
					     "secondTroughLeft_log",0,0,0);

  secondTroughRight_log = new G4LogicalVolume(secondTroughRight_surface, 
					      Al, 
					      "secondTroughRight_log",0,0,0);

  G4ThreeVector secondTroughLeftPos(secondTroughLeftPos_x,
				secondTroughPos_y,
				secondTroughPos_z);

  G4ThreeVector secondTroughRightPos(secondTroughRightPos_x,
				secondTroughPos_y,
				secondTroughPos_z);

  G4Transform3D secondTroughLeft_pos_transf;
  secondTroughLeft_pos_transf = G4Translate3D(secondTroughLeftPos);

  G4Transform3D secondTroughRight_pos_transf;
  secondTroughRight_pos_transf = G4Translate3D(secondTroughRightPos);

  //I put L&R bc secondTrough is the only trough considered in case I have a single mirror design. 
  //Then if I want to measure the lost for the traditional design I have to rotate both mirrors
  G4Transform3D secondTroughLeft_error_transf;
  secondTroughLeft_error_transf = G4Rotate3D(.05*deg, 
					 G4Point3D(-secondTrough_r2,0,0),
					 G4Point3D(-secondTrough_r2,1,0));

  G4Transform3D secondTroughRight_error_transf;
  secondTroughRight_error_transf = G4Rotate3D(.05*deg, 
					 G4Point3D(-secondTrough_r2,0,0),
					 G4Point3D(-secondTrough_r2,1,0));

  G4Transform3D secondTroughLeft_adjust_transf;
  secondTroughLeft_adjust_transf = G4RotateY3D(.0*deg);

  G4Transform3D secondTroughRight_adjust_transf;
  secondTroughRight_adjust_transf = G4RotateY3D(.0*deg);

  G4Transform3D secondTroughLeft_final_transf = 
    secondTroughLeft_adjust_transf*
    secondTroughLeft_error_transf*
    secondTroughLeft_pos_transf;

  G4Transform3D secondTroughRight_final_transf = 
    secondTroughRight_adjust_transf*
    secondTroughRight_error_transf*
    secondTroughRight_pos_transf;

  secondTroughLeft_phys = new G4PVPlacement(secondTroughLeft_final_transf,
					secondTroughLeft_log,
					"secondTroughLeft",
					experimentalHall_log,
					false,
					0);

  secondTroughRight_phys = new G4PVPlacement(secondTroughRight_final_transf,
					secondTroughRight_log,
					"secondTroughRight",
					experimentalHall_log,
					false,
					0);


  //------------------------------ glass tube
  G4Tubs* glass_tube = new G4Tubs ("glass_tube",
				   innerRadiusOfTheGlass,
				   outerRadiusOfTheGlass,
				   halfLengthOfTheTube,
				   startAngleOfTheTube,
				   spanningAngleOfTheTube);

  glass_log = new G4LogicalVolume(glass_tube,Glass,"glass_log",0,0,0);
  G4double receiverPos_x = 0.*m;
  G4double receiverPos_y = 0.*m;
  G4double receiverPos_z = 0.*m;

  G4RotationMatrix * y_minus_pi_rot = new G4RotationMatrix(0,-M_PI/2,0);

  glass_phys = new G4PVPlacement(y_minus_pi_rot,
				 G4ThreeVector(receiverPos_x,receiverPos_y,receiverPos_z),
				 glass_log,"glass",experimentalHall_log,false,0);

  //------------------------------ vacuum tube
  G4Tubs* vacuum_tube = new G4Tubs ("vacuum_tube",
				    outerRadiusOfTheReceiver,
				    innerRadiusOfTheGlass,
				    halfLengthOfTheTube,
				    startAngleOfTheTube,
				    spanningAngleOfTheTube);

  vacuum_log = new G4LogicalVolume(vacuum_tube,Vacuum,"vacuum_log",0,0,0);

  vacuum_phys = new G4PVPlacement(y_minus_pi_rot,
				  G4ThreeVector(receiverPos_x,receiverPos_y,receiverPos_z),
				  vacuum_log,"vacuum",experimentalHall_log,false,0);

  //------------------------------ vacuum receiver: 
  //dirty solution. If the receiver is not reflective does not fill the histo or whatever. 
  //Then I have to put a thin layer of vacuum as "sensitive receiver"

  G4Tubs* vacuum_receiver = new G4Tubs ("vacuum_receiver",
                                    outerRadiusOfTheReceiver-.01*mm,
                                    outerRadiusOfTheReceiver,
                                    halfLengthOfTheTube,
                                    startAngleOfTheTube,
                                    spanningAngleOfTheTube);

  vacuum_receiver_log = new G4LogicalVolume(vacuum_receiver,Vacuum,"vacuum_receiver_log",0,0,0);

  vacuum_receiver_phys = new G4PVPlacement(y_minus_pi_rot,
                                  G4ThreeVector(receiverPos_x,receiverPos_y,receiverPos_z),
                                  vacuum_receiver_log,"vacuum_receiver",experimentalHall_log,false,0);

  //------------------------------ a receiver tube

  G4Tubs* receiver_tube = new G4Tubs ("receiver_tube",
				      innerRadiusOfTheReceiver,
				      outerRadiusOfTheReceiver-.01*mm,
				      halfLengthOfTheTube,
				      startAngleOfTheTube,
				      spanningAngleOfTheTube);


  receiver_log = new G4LogicalVolume(receiver_tube,Steel,"receiver_log",0,0,0);
  receiver_phys = new G4PVPlacement(y_minus_pi_rot,
             G4ThreeVector(receiverPos_x,receiverPos_y,receiverPos_z),
             receiver_log,"receiver",experimentalHall_log,false,0);

  //------------------------------------------------ 
  // Sensitive detectors
  //------------------------------------------------ 

  G4SDManager* SDman = G4SDManager::GetSDMpointer();

  sensor = new SensorSD("sensor");
  SDman->AddNewDetector(sensor);
  glass_log->SetSensitiveDetector(sensor);

  receiver = new ReceiverSD("receiver");
  SDman->AddNewDetector(receiver);
  vacuum_receiver_log->SetSensitiveDetector(receiver);

//--------- Visualization attributes -------------------------------

  G4VisAttributes* ReceiverVisAtt= new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  receiver_log  ->SetVisAttributes(ReceiverVisAtt);  
  G4VisAttributes* TruoghVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  firstTrough_log->SetVisAttributes(TruoghVisAtt);  
  G4VisAttributes* SecondTruoghVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  secondTroughLeft_log->SetVisAttributes(SecondTruoghVisAtt);  
  secondTroughRight_log->SetVisAttributes(SecondTruoghVisAtt);  

  G4VisAttributes* GlassVisAtt= new G4VisAttributes(G4Colour(0.0,1.0,1.0));
  glass_log->SetVisAttributes(GlassVisAtt);  
  
// Skin surfaces

  new G4LogicalSkinSurface("firstTrough_surf",firstTrough_log,trough_opsurf);
  new G4LogicalSkinSurface("secondTroughLeft_surf",secondTroughLeft_log,trough_opsurf);
  new G4LogicalSkinSurface("secondTroughRight_surf",secondTroughRight_log,trough_opsurf);
  new G4LogicalSkinSurface("receiver_surf",receiver_log,receiver_opsurf);


  return experimentalHall_phys;
}

void CSPDetectorConstruction::ComputeTroughsParameters(){

  //------------------------------ Trough (arcs of parabola) 
  //      dz              z half lenght
  //      r1              radius at -dz
  //      r2              radius at  dz
  // I want to test different paraboles with the same aperture. 
  // The receiver is in the origin
  //          + r2  |       
  //    |           |          z= -r^2/(4*f) + f (eq 1, *f<0*, parab w. focus at origin)
  //    |     z1    | (r2,z1): z1= -r2^2/(4*f)+f 
  //    \     o     /          //compute f: the parabola has to pass through (z1,r2) in eq 1: 
  //     \  focus  /           z1 = -r2^2/4f + f
  //      \       /            solve it:  f = 0.5*(z1-sqrt(z1^2+r2^2)) = z2 if r1==0 
  //        -----              //in case r1!=0:
  //                           z2 = -r1^2/(4*f)+f
         

  // ----------------------------- First Trough (underneath, r1 = 0) and second Trough

  firstTrough_r1 = 0.0; //Always
  firstTrough_r2 = secondTrough_r1; 

  //I want the focus of the first trough (=z2) = average distance of the second
  G4double firstTrough_z2 = -(secondTrough_r1 + secondTrough_r2)/2.;

  G4double firstTrough_z1 = -1.*firstTrough_r2*firstTrough_r2/(4*firstTrough_z2) + firstTrough_z2;

  firstTrough_dz = (firstTrough_z1-firstTrough_z2)*0.5;

  firstTroughPos_x = 0.*m;
  firstTroughPos_y = 0.*m;
  firstTroughPos_z = firstTrough_dz+firstTrough_z2; //origin was in the middle of the parab.
  
  //------------------------------ Second trough (r1!=0)
  G4double secondTrough_z1 = -1.*secondTrough_r2/tan(rim_angle); // tan(rim_angle)= -r2/z1
  //z2 = 0.5*(z1-sqrt(z1^2+r2^2))
  G4double secondTrough_focus = 
    0.5*(secondTrough_z1-sqrt(secondTrough_z1*secondTrough_z1+secondTrough_r2*secondTrough_r2));
  G4double secondTrough_z2 = -secondTrough_r1*secondTrough_r1/(4*secondTrough_focus)+secondTrough_focus; 
  secondTrough_dz = (secondTrough_z1-secondTrough_z2)*0.5;
//   G4cout << "secondTrough_focus/cm  = " << secondTrough_focus/cm << G4endl;
//   G4cout << "secondTrough_z2/cm  = " << secondTrough_z2/cm << G4endl;
   
  secondTroughLeftPos_x = -secondTrough_r2/2;
  secondTroughRightPos_x = secondTrough_r2/2;
  secondTroughPos_y = 0.*m;
  secondTroughPos_z = secondTrough_dz+secondTrough_z2; //origin was in the middle of the parab.

}

#include "G4RunManager.hh"

void CSPDetectorConstruction::UpdateGeometry()
{
  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
}


