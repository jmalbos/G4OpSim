// -----------------------------------------------------------------------------
//  G4OpSim | OpticalMaterialProperties.h
//
//  * Author:
//  * Creation date: 26 March 2020
// -----------------------------------------------------------------------------

#ifndef OPTICAL_MATERIAL_PROPERTIES_H
#define OPTICAL_MATERIAL_PROPERTIES_H

#include <G4Types.hh>
#include <G4SystemOfUnits.hh>

class G4MaterialPropertiesTable;


namespace OpticalMaterialProperties {

  inline const G4double energy_min =  2.0 * eV; // 620 nm
  inline const G4double energy_max = 11.3 * eV; // 110 nm

  inline const G4double abslength_min = 1. * nm;
  inline const G4double abslength_max = 1.E4 * m;

  G4MaterialPropertiesTable* Vacuum();

  G4MaterialPropertiesTable* LAr();

  G4MaterialPropertiesTable* PVT();

  G4MaterialPropertiesTable* PTP();

  G4MaterialPropertiesTable* VIKUITI();

  G4MaterialPropertiesTable* FusedSilica();

  G4MaterialPropertiesTable* GlassEpoxy();

} // end namespace

#endif
