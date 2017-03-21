#include "CSPPrimaryGeneratorAction.hh"
#include "CSPDetectorConstruction.hh"

#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"

#include "G4GeneralParticleSource.hh"
#include "G4SPSAngDistribution.hh"
#include "G4SPSEneDistribution.hh"
#include "G4SPSPosDistribution.hh"

#include "globals.hh"
#include "G4ios.hh"

#include <math.h>
#include <iostream>
#include <fstream>

using namespace CLHEP;
using namespace std;

CSPPrimaryGeneratorAction::CSPPrimaryGeneratorAction(CSPDetectorConstruction* det):
  CSPDetector(det)
{

        particleGun = new G4GeneralParticleSource();

        G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
        G4String particleName;

        
        particleGun->SetParticleDefinition(particleTable->FindParticle(particleName="opticalphoton"));
	G4ThreeVector Polarization = G4ThreeVector(1.,1.,0.) ;
	particleGun->SetParticlePolarization(Polarization);

	// DEFINE A MONO-ENERGETIC SOURCE
	G4SPSEneDistribution *eneDist = particleGun->GetCurrentSource()->GetEneDist() ;
	eneDist->SetEnergyDisType("Mono");
	eneDist->SetMonoEnergy(2.0*eV);
	


        particleGun->SetParticlePosition(G4ThreeVector(0.0, 0.0, -10.*cm));
}

CSPPrimaryGeneratorAction::~CSPPrimaryGeneratorAction()
{
  delete particleGun;
}

void CSPPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
        //has to be taken from detextoronstruction...
        G4double trough_r2 = CSPDetector-> getSecondTrough_r2();

	
	G4double x_pos = trough_r2*(2.*G4UniformRand()-1.);
	//DEBUG G4int i = anEvent->GetEventID();
	//DEBUG G4double x_pos =  i*trough_r2/200000;


        //Random angle of incoming ray

        G4int npti = 64;
        G4int pdf_dim = npti*2;
        G4double* pdf = new G4double[pdf_dim]; // *2 because I read only half of the distrib
        ifstream fin("rabl-solar-shape.dat", ios::in);
        G4double pdf_bin = 0.4363*mrad;

        for(G4int i=0; i < npti; i++){ 
                char stringa[16];
                fin >> stringa;
                pdf[npti+i]=strtod(stringa, NULL);
		pdf[npti-i-1]=pdf[npti+i];
                //OK but SHOULD BE PRIVATE MEMBER? 
                //G4cout << "pdf i & -i " << i << " " << pdf[npti+i]  << " " << pdf[npti-i] << G4endl;
        }

        //for(G4int j=0;j< pdf_dim; j++) G4cout << "pdf[" << j << "] = " << pdf[j] << ";" << G4endl;

        
        RandGeneral* aRandomAngle = new RandGeneral( pdf, pdf_dim);
        
        G4double ray_angle =  ((aRandomAngle->shoot()*pdf_dim*pdf_bin + pdf_bin/2.) - (pdf_bin*npti)); //shoot*npti because shoot ]0,1[
        

        
        //G4cout  << "angle/mrad " << ray_angle/mrad << G4endl; 
        
        G4double x_momentum = tan(ray_angle);
        
//        G4cout  << "x_mom " << x_momentum << G4endl; 

        

	// SET POSITION DISTRIBUTION 
	G4SPSPosDistribution *posDist = particleGun->GetCurrentSource()->GetPosDist() ;
	G4SPSAngDistribution *angDist = particleGun->GetCurrentSource()->GetAngDist() ;

	angDist->SetParticleMomentumDirection(G4ThreeVector(x_momentum,	0, -1.));
	//DEBUG angDist->SetParticleMomentumDirection(G4ThreeVector(0.1,	0, -1.));
	posDist->SetCentreCoords(G4ThreeVector(x_pos, 0.0, 2.0*m));

        particleGun->GeneratePrimaryVertex(anEvent);




}


