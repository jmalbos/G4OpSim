// -----------------------------------------------------------------------------
//  G4OpSim | OpticalMaterialProperties.cpp
//
//  * Author:
//  * Creation date: 26 March 2020
// -----------------------------------------------------------------------------

#include "OpticalMaterialProperties.h"

#include <G4MaterialPropertiesTable.hh>


G4MaterialPropertiesTable* OpticalMaterialProperties::Vacuum()
{
  G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();

  G4double energies[] = {OpticalMaterialProperties::min_energy,
                         OpticalMaterialProperties::max_energy};

  // Refractive index (RINDEX)
  G4double rindex[] = {1., 1.};
  mpt->AddProperty("RINDEX", energies, rindex, 2);

  // Absorption length (ABSLENGTH)
  G4double abslength[] = {10000.*m, 10000.*m};
  mpt->AddProperty("ABSLENGTH", energies, abslength, 2);

  return mpt;
}
