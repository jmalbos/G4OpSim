// -----------------------------------------------------------------------------
//  G4Basic | SteppingAction.cpp
//
//  User stepping action class.
// -----------------------------------------------------------------------------

#include <vector>

#include "SteppingAction.h"
#include "EventAction.h"
#include "Analysis.h"

#include <G4Step.hh>
#include <G4RunManager.hh>
//#include <G4SteppingManager.hh>
#include <G4ProcessManager.hh>
#include <G4OpticalPhoton.hh>
#include <G4OpBoundaryProcess.hh>
#include <G4VPhysicalVolume.hh>
#include <G4SystemOfUnits.hh>
#include "G4UnitsTable.hh"
#include <CLHEP/Units/PhysicalConstants.h>


using CLHEP::h_Planck;
using CLHEP::c_light;
using CLHEP::twopi;

SteppingAction::SteppingAction(
                    EventAction* eventAction):
    G4UserSteppingAction(), 
    fEventAction(eventAction),
    counter(0), 
    ypos(-99999. * cm)
{
  G4cout << "SteppingAction::SteppingAction" << G4endl;
  wl30_counters = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  wl30_counters_passed = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  wl45_counters = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  wl45_counters_passed = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
}


SteppingAction::~SteppingAction()
{
  //G4double valuetoprint;
  G4cout << "SteppingAction::~SteppingAction" << G4endl;
  G4cout << "counter: " << counter << G4endl;
  G4cout << "final y position: " << ypos << G4endl;
  G4cout << "Final Transmittance fraction 30 deg: [";
  for (int i = 0; i < (int)wl30_counters.size(); i++) {
      //valuetoprint = 0.;
      if (i==28) {
          G4cout << ((wl30_counters[i]!=0) ? (double)wl30_counters_passed[i] / (double)wl30_counters[i] * 100. : 0.) << "]"; //wl30_counters[i] << "]";
          continue;
      }
      G4cout << ((wl30_counters[i]!=0) ? (double)wl30_counters_passed[i] / (double)wl30_counters[i] * 100. : 0.) << ", "; //wl30_counters[i] << ", ";
  }
  G4cout << G4endl;

  G4cout << "Final Transmittance fraction 45 deg: [";
  for (int i = 0; i < (int)wl30_counters.size(); i++) {
      if (i==28) {
          G4cout << ((wl30_counters[i] != 0) ? (double)wl45_counters_passed[i] / (double)wl45_counters[i] * 100. : 0.) << "]"; //wl45_counters[i] << "] ";
          continue;
      }
      G4cout << ((wl30_counters[i] != 0) ? (double)wl45_counters_passed[i] / (double)wl45_counters[i] * 100. : 0.) << ", "; //wl45_counters[i] << ", ";
  }
  G4cout << G4endl;

}


void SteppingAction::UserSteppingAction(const G4Step* step)
{ 
  //if (step->GetTrack()->GetParentID() == 0) return;

  auto analysisManager = G4AnalysisManager::Instance();
  G4ParticleDefinition* pdef = step->GetTrack()->GetDefinition();

  //Check whether the track is an optical photon
  if (pdef != G4OpticalPhoton::Definition()) return;

  auto step_number = step->GetTrack()->GetCurrentStepNumber();
  auto volume_name = step->GetTrack()->GetVolume()->GetName();

  std::vector<G4double> theWavelengths = { 250, 295, 300, 305, 310, 315, 320, 325, 330, 335, 340, 345, 350, 355, 360, 365, 370, 375, 380, 385, 390, 395, 400, 405, 410, 415, 420, 425, 500 };
  G4double ekin_init = step->GetTrack()->GetVertexKineticEnergy();
  G4double wl_init = (h_Planck * c_light) / (ekin_init);
  G4double momx = (step->GetTrack()->GetVertexMomentumDirection())[0];
  G4double momy = (step->GetTrack()->GetVertexMomentumDirection())[1];
  G4double momz = (step->GetTrack()->GetVertexMomentumDirection())[2];
  
  G4StepPoint* postStep = step->GetPostStepPoint();

  ypos = (postStep->GetPosition())[1];

  
  G4double theAngle = std::abs(std::atan(momy / (std::sqrt(momx * momx + momz * momz))));
  theAngle = theAngle / twopi * 360;
  G4cout << "step_number: " << step_number << ", volume_name: " << volume_name << ", incident angle: " << (int)theAngle << ", current count for 250 nm: " << wl30_counters[0] << G4endl;

  G4cout << "Initial photon energy: " << G4BestUnit(wl_init, "Length") << G4endl;
  G4cout << "ypos: " << ypos << G4endl;


  G4double min_ang = 30.5;
  G4double max_ang = 44.5;
  G4cout << "incident wavelength without units: " << (int)((h_Planck * c_light) / (ekin_init) / nm) << G4endl;
  if (step_number == 3) {
      for (int i = 0; i < (int)wl30_counters.size(); i++) {
          if ((int)((h_Planck * c_light) / (ekin_init) / nm) == theWavelengths[i]) {
              //G4cout << "wave length entry: " << theWavelengths[i] << G4endl;
              if (theAngle < min_ang) wl30_counters[i]++;
              if (theAngle > max_ang) wl45_counters[i]++;
              //G4cout << "current wl_counter for wavelength " << theWavelengths[i] << " nm: " << wl30_counters[i] << G4endl;
              if (ypos < 0.) {
                  if (theAngle < min_ang) wl30_counters_passed[i]++;
                  if (theAngle > max_ang) wl45_counters_passed[i]++;
              }
          }
      }
      G4cout << "Temp counts 30 deg: ";
      for (int i = 0; i < (int)wl30_counters.size(); i++) {
          G4cout << wl30_counters[i] << " ";
      }
      G4cout << G4endl;

      G4cout << "Temp Transmittance counts 30 deg: ";
      for (int i = 0; i < (int)wl30_counters.size(); i++) {
          G4cout << wl30_counters_passed[i] << " ";
      }
      G4cout << G4endl;

      G4cout << "Temp Transmittance fraction 30 deg: [";
      for (int i = 0; i < (int)wl30_counters.size(); i++) {
          G4cout << (double)wl30_counters_passed[i]/(double)wl30_counters[i]*100. << ", ";
      }
      G4cout << "]" << G4endl;

      G4cout << "Temp counts 45 deg: ";
      for (int i = 0; i < (int)wl30_counters.size(); i++) {
          G4cout << wl45_counters[i] << " ";
      }
      G4cout << G4endl;

      G4cout << "Temp Transmittance counts 45 deg: ";
      for (int i = 0; i < (int)wl30_counters.size(); i++) {
          G4cout << wl45_counters_passed[i] << " ";
      }
      G4cout << G4endl;

      G4cout << "Temp Transmittance fraction 45 deg: [";
      for (int i = 0; i < (int)wl30_counters.size(); i++) {
          G4cout << (double)wl45_counters_passed[i] / (double)wl45_counters[i] * 100. << ", ";
      }
      G4cout << "]" << G4endl;
  }


  if (step_number==3 && volume_name=="WLS_PLATE") ++counter;

  if (step_number == 3) {
      if (ypos < 0.) {
          if (theAngle < min_ang) {
              analysisManager->FillH1(0, wl_init); // transmittance
              analysisManager->FillNtupleDColumn(0, wl_init);
          }
          if (theAngle > max_ang) {
              analysisManager->FillH1(1, wl_init); // transmittance
              analysisManager->FillNtupleDColumn(1, wl_init);
          }
          analysisManager->FillH1(2, ypos); // position distribution
          analysisManager->FillH2(0, wl_init, theAngle);
      } 
      analysisManager->FillNtupleDColumn(2, ypos);
      analysisManager->FillNtupleDColumn(3, theAngle);
      analysisManager->FillNtupleDColumn(4, wl_init);
      analysisManager->AddNtupleRow();
  }

  
  // example of information one can access about optical photons
  /*
  G4Track* track = step->GetTrack();
  G4int pid = track->GetParentID();
  G4int tid = track->GetTrackID();
  G4StepPoint* point1 = step->GetPreStepPoint();
  G4StepPoint* point2 = step->GetPostStepPoint();
  G4TouchableHandle touch1 = point1->GetTouchableHandle();
  G4TouchableHandle touch2 = point2->GetTouchableHandle();
  G4String vol1name = touch1->GetVolume()->GetName();
  G4String vol2name = touch2->GetVolume()->GetName();

  G4String proc_name = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
  G4int copy_no = step->GetPostStepPoint()->GetTouchable()->GetReplicaNumber(1);*/
  

  /*
  // Retrieve the pointer to the optical boundary process.
  // We do this only once per run defining our local pointer as static.
  static G4OpBoundaryProcess* boundary = 0;

  if (!boundary) { // the pointer is not defined yet
    // Get the list of processes defined for the optical photon
    // and loop through it to find the optical boundary process.
    G4ProcessVector* pv = pdef->GetProcessManager()->GetProcessList();
    for (G4int i=0; i<pv->size(); i++) {
      if ((*pv)[i]->GetProcessName() == "OpBoundary") {
        boundary = (G4OpBoundaryProcess*) (*pv)[i];
        break;
      }
    }
  }

  if (step->GetPostStepPoint()->GetStepStatus() == fGeomBoundary) {
    if (boundary->GetStatus() == Detection ){
      G4String detector_name = step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetName();
      G4cout << "##### Sensitive Volume: " << detector_name << G4endl;

      // detectorCounts::iterator it = my_counts_.find(detector_name);
      // if (it != my_counts_.end()) my_counts_[it->first] += 1;
      // else my_counts_[detector_name] = 1;
    }
  }
  */
  
  return;

}
