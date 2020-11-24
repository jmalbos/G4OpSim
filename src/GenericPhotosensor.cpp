// -----------------------------------------------------------------------------
//  G4OpSim | GenericPhotosensor.cpp
//
//  * Author: <justo.martin-albo@ific.uv.es>
//  * Creation date: 10 February 2020
// -----------------------------------------------------------------------------

#include "GenericPhotosensor.h"
#include "OpticalMaterialProperties.h"

#include "Materials.h"
#include "OpticalSD.h"

#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>
#include <G4OpticalSurface.hh>
#include <G4LogicalSkinSurface.hh>
#include <G4SystemOfUnits.hh>
#include <G4SDManager.hh>


GenericPhotosensor::GenericPhotosensor():
  width_    (6.0*mm),
  height_   (6.0*mm),
  thickness_(2.0*mm),
  logvol_(nullptr)
{
}


GenericPhotosensor::~GenericPhotosensor()
{
}


void GenericPhotosensor::Construct()
{
  // PHOTOSENSOR ENCASING //////////////////////////////////

  G4String name = "PHOTOSENSOR";

  G4Box* encasing_solid_vol =
    new G4Box(name, width_/2., height_/2., thickness_/2.);

  G4LogicalVolume* encasing_logic_vol =
    new G4LogicalVolume(encasing_solid_vol, Materials::FR4(), name);

  logvol_ = encasing_logic_vol;

  // OPTICAL WINDOW ////////////////////////////////////////

  name = "PHOTOSENSOR_WINDOW";

  G4double window_thickness = thickness_/4.;

  G4Material* op_sil = Materials::OpticalSilicone();
  op_sil->SetMaterialPropertiesTable(OpticalMaterialProperties::GlassEpoxy());

  G4Box* window_solid_vol =
    new G4Box(name, width_/2., height_/2., window_thickness/2.);

  G4LogicalVolume* window_logic_vol =
    new G4LogicalVolume(window_solid_vol,
                        op_sil,
                        name);

  G4double zpos = thickness_/2. - window_thickness/2.;

  new G4PVPlacement(nullptr, G4ThreeVector(0., 0., zpos),
                    window_logic_vol, name, encasing_logic_vol,
                    false, 0, true);

  // PHOTOSENSITIVE AREA /////////////////////////////////////////////

  name = "PHOTOSENSOR_SENSAREA";

  G4double sensarea_thickness = 0.1*mm;

  G4Box* sensarea_solid_vol =
    new G4Box(name, width_/2., height_/2., sensarea_thickness/2.);

  G4LogicalVolume* sensarea_logic_vol =
    new G4LogicalVolume(sensarea_solid_vol,
                        G4NistManager::Instance()->FindOrBuildMaterial("G4_Si"),
                        name);

  zpos = thickness_/2. - window_thickness - sensarea_thickness/2.;

  new G4PVPlacement(nullptr, G4ThreeVector(0., 0., zpos),
                    sensarea_logic_vol, name, encasing_logic_vol,
                    false, 0, false);

  // OPTICAL PROPERTIES //////////////////////////////////////////////

  name = "PHOTOSENSOR_OPSURF";

  G4double energy[]       = {0.2*eV, 11.5*eV};
  G4double reflectivity[] = {0.0   ,  0.0};
  G4double efficiency[]   = {1.0   ,  1.0};

  G4MaterialPropertiesTable* photosensor_mpt = new G4MaterialPropertiesTable();
  photosensor_mpt->AddProperty("REFLECTIVITY", energy, reflectivity, 2);
  photosensor_mpt->AddProperty("EFFICIENCY",   energy, efficiency,   2);

  G4OpticalSurface* photosensor_opsurf =
    new G4OpticalSurface(name, unified, polished, dielectric_metal);
  photosensor_opsurf->SetMaterialPropertiesTable(photosensor_mpt);
  new G4LogicalSkinSurface(name, sensarea_logic_vol, photosensor_opsurf);

  // SENSITIVE DETECTOR //////////////////////////////////////////////

  OpticalSD* sensdet = new OpticalSD("/GENERIC_PHOTOSENSOR/SiPM");
  G4SDManager::GetSDMpointer()->AddNewDetector(sensdet);
  sensarea_logic_vol->SetSensitiveDetector(sensdet);

  ////////////////////////////////////////////////////////////////////
}
