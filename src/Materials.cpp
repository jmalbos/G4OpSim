// -----------------------------------------------------------------------------
//  G4OpSim | Materials.cpp
//
//  Definitions of materials used in the geometry.
//  * Author:
//  * Creation date: 26 March 2020
// -----------------------------------------------------------------------------

#include "Materials.h"

#include <G4Material.hh>

using namespace Materials;


G4Material* FR4()
{
  // FR-4 is a composite material widely used for printed circuits boards.
  // It consists of woven fiberglass cloth with an epoxy resin binder that is
  // flame resistant. Typical proportions are 60% fused silica and 40% epoxy.

  const G4String name = "FR4";
  G4Material* mat = G4Material::GetMaterial(name, false);

  if (!mat) {
    mat = new G4Material(name, 1.850*g/cm3, 2, kStateSolid);
    mat->AddMaterial(MaterialsList::FusedSilica(), 0.60);
    mat->AddMaterial(MaterialsList::Epoxy(),       0.40);
  }

  return mat;
}


G4Material* OpticalSilicone()
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
