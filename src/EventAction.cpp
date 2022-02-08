// -----------------------------------------------------------------------------
//  G4Basic | EventAction.cpp
//
//  User run action class.
// -----------------------------------------------------------------------------

//#include "params.h"
#include "EventAction.h"
#include "Analysis.h"

#include <G4Step.hh>
#include <G4Run.hh>
#include "G4Event.hh"
#include "G4UnitsTable.hh"
#include <G4SystemOfUnits.hh>
#include <G4PhysicalConstants.hh>

using CLHEP::h_Planck;
using CLHEP::c_light;


EventAction::EventAction()
	: G4UserEventAction(),
	fInitEnerg(6 * eV)
	
{}

EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction(const G4Event*)
{
}

void EventAction::EndOfEventAction(const G4Event* anEvent)
{
	//G4PrimaryVertex* primaryVertex = anEvent->GetPrimaryVertex();
	//G4double initEnerg = primaryVertex->GetPrimary()->GetTotalEnergy();
	//G4cout << G4BestUnit((h_Planck * c_light) / (initEnerg), "Length") << G4endl;
}
