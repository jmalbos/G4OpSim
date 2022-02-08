// -----------------------------------------------------------------------------
//  G4Basic | OpticalSD.cpp
//
//  Sensitive detector for optical sensors (e.g. SiPMs).
//   * Author: <justo.martin-albo@ific.uv.es>
//   * Creation date: 5 Nov 2020
// -----------------------------------------------------------------------------

#include "OpticalSD.h"
#include "OpticalHit.h"

#include <G4SDManager.hh>



OpticalSD::OpticalSD(const G4String& sdname):
  G4VSensitiveDetector(sdname)
{
  collectionName.insert("Optical");
}


OpticalSD::~OpticalSD()
{
}


void OpticalSD::Initialize(G4HCofThisEvent* hce)
{
}


G4bool OpticalSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
}


void OpticalSD::EndOfEvent(G4HCofThisEvent*)
{
}
