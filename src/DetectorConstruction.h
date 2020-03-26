// -----------------------------------------------------------------------------
//  G4OpSim | DetectorConstruction.h
//
//  * Author: <justo.martin-albo@ific.uv.es>
//  * Creation date: 10 February 2020
// -----------------------------------------------------------------------------

#ifndef DETECTOR_CONSTRUCTION_H
#define DETECTOR_CONSTRUCTION_H

#include <G4VUserDetectorConstruction.hh>

class G4Material;
class G4LogicalVolume;


class DetectorConstruction: public G4VUserDetectorConstruction
{
public:
  DetectorConstruction();
  ~DetectorConstruction();
  G4VPhysicalVolume* Construct();
private:
  G4LogicalVolume* GenericPhotosensor();
};

#endif
