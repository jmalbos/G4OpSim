// -----------------------------------------------------------------------------
//  G4OpSim | OpticalMaterialProperties.cpp
//
//  * Author:
//  * Creation date: 26 March 2020
// -----------------------------------------------------------------------------

#include "OpticalMaterialProperties.h"

#include <G4MaterialPropertiesTable.hh>

#include <CLHEP/Units/PhysicalConstants.h>

using CLHEP::h_Planck;
using CLHEP::c_light;


G4MaterialPropertiesTable* OpticalMaterialProperties::Vacuum()
{
  G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();

  G4double energies[] = {OpticalMaterialProperties::energy_min,
                         OpticalMaterialProperties::energy_max};

  // Refractive index (RINDEX)
  G4double rindex[] = {1., 1.};
  mpt->AddProperty("RINDEX", energies, rindex, 2);

  // Absorption length (ABSLENGTH)
  G4double abslength[] = {OpticalMaterialProperties::abslength_max,
                          OpticalMaterialProperties::abslength_max};
  mpt->AddProperty("ABSLENGTH", energies, abslength, 2);

  return mpt;
}

G4MaterialPropertiesTable* OpticalMaterialProperties::LAr()
{
  G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();

  G4double energy_step = 0.05 * eV;
  G4double energy_curr = OpticalMaterialProperties::energy_min;
  std::vector<G4double> energies;

  do {
    energies.push_back(energy_curr);
    energy_curr += energy_step;
  } while(energy_curr < OpticalMaterialProperties::energy_max);

  // Refractive index (RINDEX)
  // Add here reference to LAr paper
  G4double a0  = 0.335;
  G4double aUV = 0.099;
  G4double aIR = 0.008;
  G4double lambdaUV = 106.6 * nm;
  G4double lambdaIR = 908.3 * nm;
  std::vector<G4double> rindex;

  for (auto energy: energies) {

    G4double wavelength = h_Planck * c_light / energy;
    G4double x = a0 + aUV * pow (wavelength, 2) / (pow (wavelength, 2) - pow (lambdaUV, 2)) + aIR * pow (wavelength, 2) / (pow (wavelength, 2) - pow (lambdaIR, 2));
    G4double rindex_element = sqrt (1 + 3 * x / (3 - x));
    rindex.push_back(rindex_element);
  }

  mpt->AddProperty("RINDEX", energies.data(), rindex.data(), energies.size());

  // Absorption length (ABSLENGTH)
  G4double energies_lim[]  = {OpticalMaterialProperties::energy_min,
                              OpticalMaterialProperties::energy_max};
  G4double abslength[]     = {OpticalMaterialProperties::abslength_max,
                              OpticalMaterialProperties::abslength_max};
  mpt->AddProperty("ABSLENGTH", energies_lim, abslength, 2);

  return mpt;
}

G4MaterialPropertiesTable* OpticalMaterialProperties::PVT()
{
  G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();

  G4double energy_step = 0.05 * eV;
  G4double energy_curr = OpticalMaterialProperties::energy_min;
  std::vector<G4double> energies;

  do {
    energies.push_back(energy_curr);
    energy_curr += energy_step;
  } while(energy_curr < 7.5 * eV); // Put 7.5 eV instead of energy_max = 11.3 eV in order to avoid divergence.

  // Refractive index (RINDEX)
  // Add here reference to Sellmeier fit
  G4double A = 1.421;
  G4double B1 = 0.9944;
  G4double C1 = 26250 * pow (nm, 2);
  std::vector<G4double> rindex;

  for (auto energy: energies) {

    G4double wavelength = h_Planck * c_light / energy;
    G4double rindex_element = sqrt (A + B1 * pow (wavelength, 2) / (pow (wavelength, 2) - C1));;
    rindex.push_back(rindex_element);
  }

  mpt->AddProperty("RINDEX", energies.data(), rindex.data(), energies.size());

  // Absorption length (ABSLENGTH)
  G4double energies_lim[]  = {OpticalMaterialProperties::energy_min,
                              OpticalMaterialProperties::energy_max};
  G4double abslength[]     = {2.*m, 2.*m};
  mpt->AddProperty("ABSLENGTH", energies_lim, abslength, 2);

  return mpt;
}
