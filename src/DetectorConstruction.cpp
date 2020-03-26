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
                        G4NistManager::Instance()->FindOrBuildMaterial("lAr"),
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

  G4double zpos = plate_thickn/2. + filter_thickn/2. + 4.*mm;

  //G4VPhysicalVolume* filter_phys_vol =
    new G4PVPlacement(nullptr, G4ThreeVector(0.,0.,zpos),
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

  zpos = refl_thickn/2. + refl_thickn/2. - 4.*mm;

  //G4VPhysicalVolume* refl_phys_vol =
    new G4PVPlacement(nullptr, G4ThreeVector(0.,0.,zpos),
                      refl_logic_vol, refl_name, world_logic_vol,
                      false, 0, true);

  //////////////////////////////////////////////////////////

  return world_phys_vol;
}
