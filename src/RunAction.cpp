// -----------------------------------------------------------------------------
//  G4Basic | RunAction.cpp
//
//  User run action class.
// -----------------------------------------------------------------------------

#include "RunAction.h"
#include "Analysis.h"

#include <G4Run.hh>
#include <G4RunManager.hh>
#include <G4UnitsTable.hh>
#include <G4SystemOfUnits.hh>
#include <CLHEP/Units/PhysicalConstants.h>


using CLHEP::h_Planck;
using CLHEP::c_light;
using CLHEP::twopi;


RunAction::RunAction()
    : G4UserRunAction()
{
    // set printing event number per each event
    G4RunManager::GetRunManager()->SetPrintProgress(1);

    // Create analysis manager
    // The choice of analysis technology is done via selectin of a namespace
    // in B4Analysis.hh
    auto analysisManager = G4AnalysisManager::Instance();
    G4cout << "Using " << analysisManager->GetType() << G4endl;

    // Create directories 
    //analysisManager->SetHistoDirectoryName("histograms");
    //analysisManager->SetNtupleDirectoryName("ntuple");
    analysisManager->SetVerboseLevel(1);

  // Book histograms, ntuple
  //
  // Creating histograms
    analysisManager->CreateH1("trm30", "Transmittance at 30 deg", 100, 150. * nm, 550. * nm);
    analysisManager->CreateH1("trm45", "Transmittance at 45 deg", 100, 150. * nm, 550. * nm);
    analysisManager->CreateH1("ypos", "y-pos of final particle pos", 100, -1000 * cm, 1000. * cm);
    analysisManager->CreateH2("angvswl", "angle vs wavelength", 200, 150. * nm, 550. * nm, 200, 25, 50);
    
    // Creating ntuple
    //
    analysisManager->CreateNtuple("trm", "dichroic simulation");
    analysisManager->CreateNtupleDColumn("trm30");
    analysisManager->CreateNtupleDColumn("trm45"); // this and trm30 is for quick check of distributions, below is full information that can be used for post simulation analysis
    analysisManager->CreateNtupleDColumn("ypos_end");
    analysisManager->CreateNtupleDColumn("angle");
    analysisManager->CreateNtupleDColumn("wavelength");
    analysisManager->FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
    delete G4AnalysisManager::Instance();
}

void RunAction::BeginOfRunAction(const G4Run* run)
{
  G4cout << "------------------------------------------------------------\n"
         << "Run ID " << run->GetRunID() << G4endl;

  //inform the runManager to save random number seed
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);

  //TODO: initialise counters here so the numbers are reset for each run

  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Open an output file
  //
  G4String fileName = "dichroicTransmittance";
  analysisManager->OpenFile(fileName);

}

void RunAction::EndOfRunAction(const G4Run*)
{

  // print histogram statistics
  //
  auto analysisManager = G4AnalysisManager::Instance();

  G4cout << " Trm at 30 deg : mean = "
      << analysisManager->GetH1(0)->mean() << G4endl;
  
  G4cout << "End of run."
         << "------------------------------------------------------------"
         << G4endl;

  // save histograms & ntuple
  //
  analysisManager->Write();
  analysisManager->CloseFile();
}
