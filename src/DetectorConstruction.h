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
  G4VPhysicalVolume* Construct() override;
private:
  void ConstructWorld(G4VPhysicalVolume&);
  void ConstructWLSPlate(G4VPhysicalVolume*) const;
  void ConstructPhotosensors(G4VPhysicalVolume*) const;
  void ConstructReflectiveFoils(G4VPhysicalVolume*) const;

  void Assert(G4VPhysicalVolume*, const G4String&) const;

private:
  const G4double world_size_;
  const G4double plate_width_, plate_thickn_, plate_length_, foil_thickn_;
  const G4int num_phsensors;
};

#endif
