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

#include "CSPHistoManager.hh"
#include "CSPHistoMessenger.hh"
#include "G4UnitsTable.hh"

#ifdef G4ANALYSIS_USE
#include "AIDA/AIDA.h"
#endif

CSPHistoManager* CSPHistoManager::instance = 0;

CSPHistoManager::CSPHistoManager()
:af(0),tree(0),factoryOn(false)
{
#ifdef G4ANALYSIS_USE
  G4cout << "Creating the analysis factory\n";
  af = AIDA_createAnalysisFactory();

  if(!af) {
    G4cout << " CSPHistoManager::CSPHistoManager() :" 
           << " problem creating the AIDA analysis factory."
           << G4endl;
  }	     
#endif 

 
  fileName[0] = "csp";
  fileType    = "hbook";
  fileOption  = "--noErrors uncompress";  
  // histograms
  for (G4int k=0; k<MaxHisto; k++) {
    histo[k] = 0;
    exist[k] = false;
    Unit[k]  = 1.0;
    Width[k] = 1.0;
    ascii[k] = false;       
  }

  histoMessenger = new CSPHistoMessenger(this);
}

CSPHistoManager* CSPHistoManager::getInstance()
{
  if (instance == 0) instance = new CSPHistoManager();
  return instance;
}

void CSPHistoManager::dispose()
{
  if (instance != 0)
  {
    delete instance;
    instance = 0;
  }
}



CSPHistoManager::~CSPHistoManager()
{
   delete histoMessenger;
  
#ifdef G4ANALYSIS_USE  
  delete af;
#endif    
}
 

void CSPHistoManager::book()
{
  
#ifdef G4ANALYSIS_USE
  //G4cout << "G4ANALYSIS_USE = True \n";
  if(!af){
    G4cout << "ERROR WITH AIDAFACTORY\n";
    return;
  }
  
  // Creating a tree mapped to an hbook file.
  fileName[1] = fileName[0] + "." + fileType;  
  G4bool readOnly  = false;
  G4bool createNew = true;
  AIDA::ITreeFactory* tf  = af->createTreeFactory();
  tree = tf->create(fileName[1], fileType, readOnly, createNew, fileOption);
  delete tf;
  if(!tree) {
    G4cout << "CSPHistoManager::book() :" 
           << " problem creating the AIDA tree with "
           << " storeName = " << fileName[1]
           << " storeType = " << fileType
           << " readOnly = "  << readOnly
           << " createNew = " << createNew
           << " options = "   << fileOption
           << G4endl;
    return;
  }

  // Creating a histogram factory, whose histograms will be handled by the tree
  AIDA::IHistogramFactory* hf = af->createHistogramFactory(*tree);
  
  // create selected histograms
  for (G4int k=0; k<MaxHisto; k++) {
    
    if (exist[k]) {
      histo[k] = hf->createHistogram1D( Label[k], Title[k],
   					Nbins[k], Vmin[k], Vmax[k]);
      factoryOn = true;
      G4cout << "FactoryOn = " << factoryOn << G4endl;
      
    }
  }

  delete hf;  
  if(factoryOn) 
    G4cout << "----> Histogram Tree is opened in " << fileName[1] << G4endl;
  
#endif
}

void CSPHistoManager::save()
{

#ifdef G4ANALYSIS_USE
  G4cout << "Called  CSPHistoManager::save()\n";

  if (factoryOn) {
    saveAscii();          // Write ascii file, if any     
    tree->commit();       // Writing the histograms to the file
    tree->close();        // and closing the tree (and the file)
    G4cout << "\n----> Histogram Tree is saved in " << fileName[1] << G4endl;
    
    delete tree;
    tree = 0;
    factoryOn = false;
  }
#endif
}


void CSPHistoManager::FillHisto(G4int ih, G4double e, G4double weight)
{
  if (ih > MaxHisto) {
    G4cout << "---> warning from CSPHistoManager::FillHisto() : histo " << ih
           << "does not exist; e= " << e << " w= " << weight << G4endl;
    return;
  }
#ifdef G4ANALYSIS_USE
  if(exist[ih]) histo[ih]->fill(e/Unit[ih], weight);
#endif
}

void CSPHistoManager::SetHisto(G4int ih,
                 G4int nbins, G4double valmin, G4double valmax, const G4String& unit)
{
  G4cout << "PIPIPIPIPIPI" << ih << "\n";

  if (ih > MaxHisto) {
    G4cout << "---> warning from CSPHistoManager::SetHisto() : histo " << ih
           << "does not exist" << G4endl;
    return;
  }

  const G4String id[] = { "0", "1", "2"};
  const G4String title[] =
                { "dummy",              //0
                  "rays along circumference",      //1
		  "number of photon on the receiver",      //2
                 };

  G4String titl = title[ih];
  G4double vmin = valmin, vmax = valmax;
  Unit[ih] = 1.;

  if (unit != "none") {
    titl = title[ih] + " (" + unit + ")";
    Unit[ih] = G4UnitDefinition::GetValueOf(unit);
    vmin = valmin/Unit[ih]; vmax = valmax/Unit[ih];
  }

  exist[ih] = true;
  Label[ih] = id[ih];
  Title[ih] = titl;
  Nbins[ih] = nbins;
  Vmin[ih]  = vmin;
  Vmax[ih]  = vmax;
  Width[ih] = (valmax-valmin)/nbins;

  G4cout << "----> SetHisto " << ih << ": " << titl << ";  "
         << nbins << " bins from "
         << vmin << " " << unit << " to " << vmax << " " << unit << G4endl;

}

void CSPHistoManager::RemoveHisto(G4int ih)
{
 if (ih > MaxHisto) {
    G4cout << "---> warning from CSPHistoManager::RemoveHisto() : histo " << ih
           << "does not exist" << G4endl;
    return;
  }

  histo[ih] = 0;  exist[ih] = false;
}

void CSPHistoManager::PrintHisto(G4int ih)
{
 if (ih < MaxHisto) ascii[ih] = true;
 else
    G4cout << "---> warning from CSPHistoManager::PrintHisto() : histo " << ih
           << "does not exist" << G4endl;
}


#include <fstream>

void CSPHistoManager::saveAscii()
{
#ifdef G4ANALYSIS_USE
 
 G4String name = fileName[0] + ".ascii";
 std::ofstream File(name, std::ios::out);
 File.setf( std::ios::scientific, std::ios::floatfield );
 
 //write selected histograms
 for (G4int ih=0; ih<MaxHisto; ih++) {
    if (exist[ih] && ascii[ih]) {
      File << "\n  1D histogram " << ih << ": " << Title[ih] 
           << "\n \n \t     X \t\t     Y" << G4endl;
     
      for (G4int iBin=0; iBin<Nbins[ih]; iBin++) {
         File << "  " << iBin << "\t" 
              << histo[ih]->binMean(iBin) << "\t"
	      << histo[ih]->binHeight(iBin) 
	      << G4endl;
      } 
    }
 }
#endif
}



