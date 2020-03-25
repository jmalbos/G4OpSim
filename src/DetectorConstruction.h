// -----------------------------------------------------------------------------
//  G4Basic | DetectorConstruction.h
//
//
// -----------------------------------------------------------------------------

#ifndef DETECTOR_CONSTRUCTION_H
#define DETECTOR_CONSTRUCTION_H

#include <G4VUserDetectorConstruction.hh>

class G4Material;


class DetectorConstruction: public G4VUserDetectorConstruction
{
public:
  DetectorConstruction();
  virtual ~DetectorConstruction();
  virtual G4VPhysicalVolume* Construct();
private:
  G4Material* EnrichedXenon() const;
};

#endif
