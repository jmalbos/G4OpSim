// -----------------------------------------------------------------------------
//  G4Basic | TrackingSD.h
//
//  TODO: Class description
//   * Author: Justo Martin-Albo
//   * Creation date: 12 Feb 2019
// -----------------------------------------------------------------------------

#ifndef TRACKING_SD_H
#define TRACKING_SD_H

#include <G4VSensitiveDetector.hh>
#include "TrackingHit.h"


class TrackingSD: public G4VSensitiveDetector
{
public:
  TrackingSD(const G4String&, const G4String&);
  virtual ~TrackingSD();

  virtual void   Initialize(G4HCofThisEvent*);
  virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);
  virtual void   EndOfEvent(G4HCofThisEvent*);

private:
  TrackingHitsCollection* hc_;
};

#endif
