// -----------------------------------------------------------------------------
//  G4OpSim | DetectorConstruction.cpp
//
//  * Author: <justo.martin-albo@ific.uv.es>
//  * Creation date: 10 February 2020
// -----------------------------------------------------------------------------

#include "DetectorConstruction.h"

#include "TrackingSD.h"

#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4Sphere.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4SystemOfUnits.hh>
#include <G4PhysicalConstants.hh>
#include <G4NistManager.hh>
#include <G4VisAttributes.hh>
#include <G4RotationMatrix.hh>
#include <G4UserLimits.hh>
#include <G4SDManager.hh>
#include <G4OpticalSurface.hh>
#include <G4LogicalSkinSurface.hh>


DetectorConstruction::DetectorConstruction(): G4VUserDetectorConstruction()
{
}


DetectorConstruction::~DetectorConstruction()
{
}


G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // WORLD /////////////////////////////////////////////////

  const G4String world_name = "WORLD";
  const G4double world_size = 10.*m;

  G4Sphere* world_solid_vol =
    new G4Sphere(world_name, 0., world_size/2., 0., 360.*deg, 0., 180.*deg);

  G4LogicalVolume* world_logic_vol =
    new G4LogicalVolume(world_solid_vol,
                        G4NistManager::Instance()->FindOrBuildMaterial("G4_lAr"),
                        world_name);

  G4VPhysicalVolume* world_phys_vol =
    new G4PVPlacement(nullptr, G4ThreeVector(0.,0.,0.),
                      world_logic_vol, world_name, nullptr,
                      false, 0, true);

  world_logic_vol->SetVisAttributes(G4VisAttributes::Invisible);

  // WLS PLATE /////////////////////////////////////////////

  const G4String plate_name = "WLS_PLATE";

  const G4double plate_width  = 112.0*mm; // X
  const G4double plate_thickn =   4.0*mm; // Y
  const G4double plate_length = 491.5*mm; // Z

  G4Box* plate_solid_vol =
    new G4Box(plate_name, plate_width/2., plate_thickn/2., plate_length/2.);

  G4LogicalVolume* plate_logic_vol =
    new G4LogicalVolume(plate_solid_vol,
                        G4NistManager::Instance()->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE"),
                        plate_name);

  //G4VPhysicalVolume* plate_phys_vol =
    new G4PVPlacement(nullptr, G4ThreeVector(0.,0.,0.),
                      plate_logic_vol, plate_name, world_logic_vol,
                      false, 0, true);

  // DICHROIC FILTER ///////////////////////////////////////

  const G4String filter_name = "DICHROIC_FILTER";

  const G4double filter_width  = 112.0*mm; // X
  const G4double filter_thickn =   4.0*mm; // Y
  const G4double filter_length = 491.5*mm; // Z

  G4Box* filter_solid_vol =
    new G4Box(filter_name, filter_width/2., filter_thickn/2., filter_length/2.);

  G4LogicalVolume* filter_logic_vol =
    new G4LogicalVolume(filter_solid_vol,
                        G4NistManager::Instance()->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE"),
                        filter_name);

  G4double ypos = plate_thickn/2. + filter_thickn/2. + 4.*mm;

  //G4VPhysicalVolume* filter_phys_vol =
    new G4PVPlacement(nullptr, G4ThreeVector(0.,ypos,0.),
                      filter_logic_vol, filter_name, world_logic_vol,
                      false, 0, true);

  // BACK REFLECTOR ////////////////////////////////////////

  const G4String refl_name = "BACK_REFLECTOR";

  const G4double refl_width  = 112.0*mm; // X
  const G4double refl_thickn =   4.0*mm; // Y
  const G4double refl_length = 491.5*mm; // Z

  G4Box* refl_solid_vol =
    new G4Box(refl_name, refl_width/2., refl_thickn/2., refl_length/2.);

  G4LogicalVolume* refl_logic_vol =
    new G4LogicalVolume(refl_solid_vol,
                        G4NistManager::Instance()->FindOrBuildMaterial("G4_TEFLON"),
                        refl_name);

  //G4VPhysicalVolume* refl_phys_vol =
    new G4PVPlacement(nullptr, G4ThreeVector(0.,-ypos,0.),
                      refl_logic_vol, refl_name, world_logic_vol,
                      false, 0, true);

  //////////////////////////////////////////////////////////

  return world_phys_vol;
}


G4LogicalVolume* DetectorConstruction::GenericPhotosensor()
{
  // PHOTOSENSOR ENCASING //////////////////////////////////

  const G4double width_     = 6.*mm;
  const G4double height_    = 6.*mm;
  const G4double thickness_ = 2.*mm;

  G4String name = "PHOTOSENSOR";

  G4Box* encasing_solid_vol =
    new G4Box(name, width_/2., height_/2., thickness_/2.);

  G4LogicalVolume* encasing_logic_vol =
    new G4LogicalVolume(encasing_solid_vol,
                        G4NistManager::Instance()->FindOrBuildMaterial("G4_TEFLON"),
                        name);

  // OPTICAL WINDOW ////////////////////////////////////////

  name = "PHOTOSENSOR_WINDOW";

  G4double window_thickness = thickness_/4.;

  G4Box* window_solid_vol =
    new G4Box(name, width_/2., height_/2., window_thickness/2.);


  G4LogicalVolume* window_logic_vol =
    new G4LogicalVolume(window_solid_vol,
                        G4NistManager::Instance()->FindOrBuildMaterial("G4_TEFLON"),
                        name);

  G4double zpos = thickness_/2. - window_thickness/2.;

  new G4PVPlacement(nullptr, G4ThreeVector(0., 0., zpos),
                    window_logic_vol, name, encasing_logic_vol,
                    false, 0, false);

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

  // // SENSITIVE DETECTOR //////////////////////////////////////////////
  //
  // G4String sdname = "/GENERIC_PHOTOSENSOR/SiPM";
  // G4SDManager* sdmgr = G4SDManager::GetSDMpointer();
  //
  // if (!sdmgr->FindSensitiveDetector(sdname, false)) {
  //   PmtSD* sensdet = new PmtSD(sdname);
  //   sensdet->SetDetectorVolumeDepth(1);
  //   sensdet->SetDetectorNamingOrder(1000.);
  //   sensdet->SetTimeBinning(time_binning_);
  //   sensdet->SetMotherVolumeDepth(2);
  //
  //   G4SDManager::GetSDMpointer()->AddNewDetector(sensdet);
  //   window_logic_vol->SetSensitiveDetector(sensdet);
  // }

  return encasing_logic_vol;
}
