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

  G4double energy[]       = {3.875*eV, 3.836*eV, 3.815*eV, 3.794*eV, 3.773*eV, 
                            3.752*eV, 3.732*eV, 3.721*eV, 3.687*eV, 3.701*eV, 3.672*eV, 
                            3.652*eV, 3.672*eV, 3.642*eV, 3.613*eV, 3.623*eV, 3.594*eV, 
                            3.613*eV, 3.585*eV, 3.563*eV, 3.539*eV, 3.521*eV, 3.503*eV, 
                            3.485*eV, 3.441*eV, 3.45*eV, 3.415*eV, 3.382*eV, 3.349*eV, 
                            3.316*eV, 3.284*eV, 3.253*eV, 3.223*eV, 3.185*eV, 3.192*eV, 
                            3.148*eV, 3.106*eV, 3.064*eV, 3.023*eV, 2.997*eV, 2.965*eV, 
                            2.902*eV, 2.83*eV, 2.761*eV, 2.696*eV, 2.619*eV, 2.546*eV, 
                            2.49*eV, 2.446*eV, 2.411*eV, 2.378*eV, 2.345*eV, 2.318*eV, 
                            2.294*eV, 2.268*eV, 2.238*eV, 2.216*eV, 2.192*eV, 2.171*eV, 
                            2.15*eV, 2.137*eV, 2.117*eV, 2.098*eV, 2.079*eV, 2.06*eV, 
                            2.038*eV, 2.02*eV, 2.006*eV, 1.988*eV, 1.971*eV, 1.962*eV, 
                            1.943*eV, 1.929*eV, 1.913*eV, 1.889*eV, 1.866*eV, 1.846*eV, 
                            1.826*eV, 1.797*eV, 1.774*eV, 1.754*eV, 1.734*eV, 1.714*eV, 
                            1.683*eV, 1.66*eV, 1.641*eV, 1.619*eV, 1.602*eV, 1.578*eV, 
                            1.557*eV, 1.536*eV, 1.515*eV, 1.499*eV, 1.475*eV, 1.454*eV, 
                            1.436*eV, 1.418*eV, 1.401*eV, 1.382*eV};

  G4double reflectivity[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
                            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
                            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
                            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
                            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
                            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
                            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
                            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
                            0.0, 0.0, 0.0};

  G4double efficiency[]   = {0.026, 0.037, 0.046, 0.055, 0.068, 0.08, 0.09, 0.099, 0.119, 
                            0.108, 0.13, 0.143, 0.135, 0.154, 0.17, 0.163, 0.182, 0.176, 
                            0.19, 0.2, 0.209, 0.217, 0.225, 0.234, 0.245, 0.239, 0.256, 
                            0.267, 0.278, 0.29, 0.302, 0.312, 0.323, 0.335, 0.329, 0.345, 
                            0.352, 0.36, 0.367, 0.37, 0.377, 0.385, 0.392, 0.398, 0.401, 
                            0.398, 0.393, 0.387, 0.381, 0.373, 0.365, 0.356, 0.348, 0.341, 
                            0.333, 0.323, 0.315, 0.308, 0.3, 0.293, 0.289, 0.28, 0.273, 
                            0.266, 0.259, 0.252, 0.247, 0.241, 0.233, 0.225, 0.219, 0.211, 
                            0.206, 0.2, 0.191, 0.183, 0.176, 0.169, 0.159, 0.152, 0.145, 
                            0.139, 0.132, 0.122, 0.114, 0.108, 0.101, 0.096, 0.088, 0.082, 
                            0.076, 0.071, 0.066, 0.059, 0.053, 0.048, 0.044, 0.039, 0.035};


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
