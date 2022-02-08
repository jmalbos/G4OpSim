// -----------------------------------------------------------------------------
//  G4OpSim | GenericPhotosensor.h
//
//  * Author: <justo.martin-albo@ific.uv.es>
//  * Creation date: 10 February 2020
// -----------------------------------------------------------------------------

#ifndef GENERIC_PHOTOSENSOR_H
#define GENERIC_PHOTOSENSOR_H

#include <globals.hh>

class G4LogicalVolume;


class GenericPhotosensor
{
public:
  GenericPhotosensor();
  ~GenericPhotosensor();
  void Construct();
  G4LogicalVolume* GetLogicalVolume();

  G4double GetWidth() const;
  G4double GetHeight() const;
  G4double GetThickness() const;

private:
  G4double width_, height_, thickness_;
  G4LogicalVolume* logvol_;
};

inline G4LogicalVolume* GenericPhotosensor::GetLogicalVolume() { return logvol_; }
inline G4double GenericPhotosensor::GetWidth() const { return width_; }
inline G4double GenericPhotosensor::GetHeight() const { return height_; }
inline G4double GenericPhotosensor::GetThickness() const { return thickness_; }

#endif
