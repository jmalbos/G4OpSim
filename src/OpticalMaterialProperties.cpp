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

G4MaterialPropertiesTable* OpticalMaterialProperties::LAr()
{
  G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();

  G4double energy_limits[] = {OpticalMaterialProperties::min_energy,
                         OpticalMaterialProperties::max_energy};

  // Wavelength limits defined by min_energy and max_energy
  G4double lambda_limits[2];

  for (int i = 0; i < 2; i++) {
    lambda_limits[i] = 1239.675 / energy_limits[i]; // nm. We used: lambda = 2*pi*hbar*c/E
  }

  int dim_lambda = 3000; // Just an initialization (set a number big enough)
  G4double step; // nm


  G4double lambda_aux[3000] = {0.0};

  lambda_aux[0] = lambda_limits[1];

  for (int i = 1; i < dim_lambda; i++) {

    if (lambda_aux[i-1] < 300) {
      step = 0.3;
      lambda_aux[i] = lambda_aux[i-1] + step;
    }

    else if (lambda_aux[i-1] >= 300 && lambda_aux[i-1] < lambda_limits[0]) {
      step = 1;
      lambda_aux[i] = lambda_aux[i-1] + step;
    }

    else {
      step = 1;
      lambda_aux[i] = lambda_aux[i-1] + step;
      dim_lambda = i+1;
      break;
    }

  }

  G4double lambda[dim_lambda];

  for (int i = 0; i < dim_lambda; i++) {
    lambda[i] = lambda_aux[i];
  }

  G4double energies[dim_lambda];

  for (int i = 0; i < dim_lambda; i++) {
    energies[i] = 1239.675 / lambda[i];
  }

  // Auxiliar variable defined in the article
  G4double x[dim_lambda];

  // Parameters
  G4double a0 = 0.335;
  G4double aUV = 0.099;
  G4double aIR = 0.008;
  G4double lambdaUV = 106.6 * nm;
  G4double lambdaIR = 908.3 * nm;

  // Refractive index (RINDEX)
  G4double rindex[dim_lambda];

  for (int k = 0; k < dim_lambda; k++) {
    x[k] = a0 + aUV * pow (lambda[k], 2) / (pow (lambda[k], 2) - pow (lambdaUV, 2)) + aIR * pow (lambda[k], 2) / (pow (lambda[k], 2) - pow (lambdaIR, 2));
    rindex[k] = sqrt (1 + 3 * x[k] / (3 - x[k]));
  }

  mpt->AddProperty("RINDEX", energies, rindex, dim_lambda);

  // Absorption length (ABSLENGTH)
  G4double abslength[] = {10000.*m, 10000.*m};
  mpt->AddProperty("ABSLENGTH", energy_limits, abslength, 2);



  return mpt;
}

G4MaterialPropertiesTable* OpticalMaterialProperties::PVT()
{
  G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();

  G4double energy_limits[] = {OpticalMaterialProperties::min_energy,
                         7}; //Fixed a different max_energy to avoid divergence at low wavelengths

  // Wavelength limits defined by min_energy and max_energy
  G4double lambda_limits[2];

  for (int i = 0; i < 2; i++) {
    lambda_limits[i] = 1239.675 / energy_limits[i]; // nm. We used: lambda = 2*pi*hbar*c/E
  }

  int dim_lambda = 3000; // Just an initialization (set a number big enough)
  G4double step; // nm


  G4double lambda_aux[3000] = {0.0};

  lambda_aux[0] = lambda_limits[1];

  for (int i = 1; i < dim_lambda; i++) {

    if (lambda_aux[i-1] < 300) {
      step = 0.3;
      lambda_aux[i] = lambda_aux[i-1] + step;
    }

    else if (lambda_aux[i-1] >= 300 && lambda_aux[i-1] < lambda_limits[0]) {
      step = 1;
      lambda_aux[i] = lambda_aux[i-1] + step;
    }

    else {
      step = 1;
      lambda_aux[i] = lambda_aux[i-1] + step;
      dim_lambda = i+1;
      break;
    }

  }

  G4double lambda[dim_lambda];

  for (int i = 0; i < dim_lambda; i++) {
    lambda[i] = lambda_aux[i];
  }

  G4double energies[dim_lambda];

  for (int i = 0; i < dim_lambda; i++) {
    energies[i] = 1239.675 / lambda[i];
  }

  // Parameters
  G4double A = 1.421;
  G4double B1 = 0.9944;
  G4double C1 = 26250 * pow (nm, 2);

  // Refractive index (RINDEX)
  G4double rindex[dim_lambda];

  for (int i = 0; i < dim_lambda; i++) {
    rindex[i] = sqrt (A + B1 * pow (lambda[i], 2) / (pow (lambda[i], 2) - C1));
  }

  mpt->AddProperty("RINDEX", energies, rindex, dim_lambda);

  // Absorption length (ABSLENGTH)
  G4double abslength[] = {10000.*m, 10000.*m};
  mpt->AddProperty("ABSLENGTH", energy_limits, abslength, 2);



  return mpt;
}
