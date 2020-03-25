// -----------------------------------------------------------------------------
//  G4Basic | G4Basic.cpp
//
//  Main function of the Geant4 simulation.
// -----------------------------------------------------------------------------

#include "PhysicsList.h"
#include "DetectorConstruction.h"
#include "PrimaryGeneration.h"
#include "RunAction.h"
#include "EventAction.h"
#include "TrackingAction.h"
#include "SteppingAction.h"

#include <G4RunManager.hh>
#include <G4UImanager.hh>
#include <G4VisExecutive.hh>
#include <G4UIterminal.hh>
#include <G4UItcsh.hh>


int main(int argc, char const *argv[])
{
  // Construct the run manager and set the initialization classes next
  G4RunManager* runmgr = new G4RunManager();
  runmgr->SetUserInitialization(new PhysicsList());
  runmgr->SetUserInitialization(new DetectorConstruction());
  runmgr->SetUserAction(new PrimaryGeneration());
  runmgr->SetUserAction(new RunAction());
  runmgr->Initialize();

  // If no macro file was provided via command line,
  // start an interactive session.
  if (argc == 1) {
    G4VisManager* vismgr = new G4VisExecutive();
    vismgr->Initialize();
    G4UImanager::GetUIpointer()->
      ApplyCommand("/control/execute mac/vis.mac");
    G4UIsession* session = new G4UIterminal(new G4UItcsh);
    session->SessionStart();
    delete session;
    delete vismgr;
  }
  // We will assume that the first command-line argument is the name
  // of a valid macro file. Any other argument will be ignored.
  else {
    G4String command = "/control/execute " + std::string(argv[1]);
    G4UImanager::GetUIpointer()->ApplyCommand(command);
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program.
  delete runmgr;

  return EXIT_SUCCESS;
}
