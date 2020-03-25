// -----------------------------------------------------------------------------
//  G4Basic | TrackingSD.cpp
//
//  TODO: Class description
//   * Author: Justo Martin-Albo
//   * Creation date: 13 Feb 2019
// -----------------------------------------------------------------------------

#include "TrackingSD.h"
#include "TrackingHit.h"

#include <G4SDManager.hh>


TrackingSD::TrackingSD(const G4String& sd_name, const G4String& hc_name):
  G4VSensitiveDetector(sd_name),
  hc_(nullptr)
{
  collectionName.insert(hc_name);
}


TrackingSD::~TrackingSD()
{
}


void TrackingSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  hc_ = new TrackingHitsCollection(SensitiveDetectorName, collectionName[0]);

  // Add this collection in hce
  G4int hcid = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection(hcid, hc_);
}


G4bool TrackingSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  G4double edep = aStep->GetTotalEnergyDeposit();

  if (edep==0.) return false;

  TrackingHit* newHit = new TrackingHit();

  newHit->SetEdep(edep);
  newHit->SetPosition(aStep->GetPostStepPoint()->GetPosition());

  hc_->insert(newHit);

  return true;
}


void TrackingSD::EndOfEvent(G4HCofThisEvent*)
{
  G4int nofHits = hc_->entries();
  G4cout
    << G4endl
    << "-------->Hits Collection: in this event there are " << nofHits
    << " hits in the tracker." << G4endl;
    //for ( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->Print();
}
