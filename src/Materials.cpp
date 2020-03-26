// -----------------------------------------------------------------------------
//  G4OpSim | Materials.cpp
//
//  Definitions of materials used in the geometry.
//  * Author:
//  * Creation date: 26 March 2020
// -----------------------------------------------------------------------------

#include "Materials.h"

#include <G4SystemOfUnits.hh>
#include <G4NistManager.hh>
#include <G4Material.hh>


G4Material* Materials::FR4()
{
  // FR-4 is a composite material widely used for printed circuits boards.
  // It consists of woven fiberglass cloth with an epoxy resin binder that is
  // flame resistant. Typical proportions are 60% fused silica and 40% epoxy.

  const G4String name = "FR4";
  G4Material* mat = G4Material::GetMaterial(name, false);

  if (!mat) {
    mat = new G4Material(name, 1.850*g/cm3, 2, kStateSolid);
    mat->AddMaterial(Materials::FusedSilica(), 0.60);
    mat->AddMaterial(Materials::Epoxy(),       0.40);
  }

  return mat;
}


G4Material* Materials::OpticalSilicone()
{
  // Silicone resin with a methyl group
  // (https://en.wikipedia.org/wiki/Silicone_resin)

  const G4String name = "OPTICAL_SILICONE";

  G4Material* mat = G4Material::GetMaterial(name, false);

  if (!mat) {
    G4NistManager* nist = G4NistManager::Instance();

    G4Element* H  = nist->FindOrBuildElement("H");
    G4Element* C  = nist->FindOrBuildElement("C");
    G4Element* O  = nist->FindOrBuildElement("O");
    G4Element* Si = nist->FindOrBuildElement("Si");

    mat = new G4Material(name, 1.05*g/cm3, 4, kStateSolid);
    mat->AddElement(H,  3);
    mat->AddElement(C,  1);
    mat->AddElement(Si, 1);
    mat->AddElement(O,  1);
  }

  return mat;
}


G4Material* Materials::Epoxy()
{
  // Definition taken from the Geant4 advanced example "Composite Calorimeter"
  // (Geant4/examples/advanced/composite_calorimeter/dataglobal/material.cms).

  const G4String name = "EPOXY";

  G4Material* mat = G4Material::GetMaterial(name, false);

  if (!mat) {
    G4NistManager* nist = G4NistManager::Instance();

    G4Element* H = nist->FindOrBuildElement("H");
    G4Element* C = nist->FindOrBuildElement("C");
    G4Element* O = nist->FindOrBuildElement("O");

    mat = new G4Material(name, 1.3*g/cm3, 3);
    mat->AddElement(H, 44);
    mat->AddElement(C, 15);
    mat->AddElement(O, 7);
  }

  return mat;
}


G4Material* Materials::FusedSilica()
{
  G4Material* mat =
    G4NistManager::Instance()->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
  return mat;
}
