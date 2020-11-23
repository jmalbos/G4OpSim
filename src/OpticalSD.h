// -----------------------------------------------------------------------------
//  G4Basic | OpticalSD.h
//
//  Sensitive detector for optical sensors (e.g. SiPMs).
//   * Author: <justo.martin-albo@ific.uv.es>
//   * Creation date: 5 Nov 2020
// -----------------------------------------------------------------------------

#ifndef OPTICAL_SD_H
#define OPTICAL_SD_H

#include <G4VSensitiveDetector.hh>
#include "OpticalHit.h"


class OpticalSD: public G4VSensitiveDetector
{
public:
  OpticalSD(const G4String&);
  ~OpticalSD();

  void Initialize(G4HCofThisEvent*) override;
  G4bool ProcessHits(G4Step*, G4TouchableHistory*) override;
  void EndOfEvent(G4HCofThisEvent*) override;

private:
  OpticalHitCollection* hc_;
};

#endif
