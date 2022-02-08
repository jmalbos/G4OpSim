// -----------------------------------------------------------------------------
//  G4Basic | PrimaryGeneration.cpp
//
//
// -----------------------------------------------------------------------------

#include <stdlib.h>
#include <time.h>
#include <vector>

#include "params.h"
#include "PrimaryGeneration.h"

#include <G4SystemOfUnits.hh>
#include <G4PhysicalConstants.hh>
#include <G4OpticalPhoton.hh>
#include <G4ParticleDefinition.hh>
#include <G4SystemOfUnits.hh>
#include <G4PrimaryParticle.hh>
#include <G4PrimaryVertex.hh>
#include <G4Event.hh>
#include <G4RandomDirection.hh>

using CLHEP::h_Planck;
using CLHEP::c_light;

PrimaryGeneration::PrimaryGeneration():
  G4VUserPrimaryGeneratorAction(),
  kinetic_energy_(6*eV)
{
}


PrimaryGeneration::~PrimaryGeneration()
{
}


void PrimaryGeneration::GeneratePrimaries(G4Event* event)
{
  // Generate a random momentum direction for the photon

  //G4double cost = 1. - 2.*G4UniformRand();
  //G4double sint = std::sqrt((1.-cost)*(1.+cost));
  //G4double theta = twopi / 4. + twopi / 360 * 30 + twopi / 360 * 15 * G4UniformRand(); //isotropic distribution, careful with angles for which the photon misses the ARAPUCA
  G4double theta; // = twopi / 4. + twopi / 360. * 45.;
  G4double randpicker = G4UniformRand();
  if (randpicker < 0.5) theta = twopi / 4. + twopi / 360 * 30;
  else theta = twopi / 4. + twopi / 360 * 45;
  G4double cost = std::cos(theta);
  G4double sint = std::sin(theta);

  G4double phi = twopi / 4.; //* G4UniformRand();
  G4double sinp = std::sin(phi); //twopi/2. + twopi/360*30+ twopi/360*15*G4UniformRand();
  G4double cosp = std::cos(phi);

  G4double wavelengths[29] = { 250, 295, 300, 305, 310, 315, 320, 325, 330, 335, 340, 345, 350, 355, 360, 365, 370, 375, 380, 385, 390, 395, 400, 405, 410, 415, 420, 425, 500 };
  //srand(time(NULL));
  int randPicker = int((G4UniformRand()*1000000.)) % sizeof(wavelengths);
  //G4double wavelengths_sample[3] = {300, 350, 400}; // simulation tests with smaller sample size
  //int randPicker_sample = int((G4UniformRand()*100000.))%3;

  G4double px = sint*cosp;
  G4double py = sint*sinp;
  G4double pz = cost;

  //G4double Edelta = h_Planck * c_light * (1 / (200 * nm) - 1 / (500 * nm) ); // used in combination with a minimal energy in order to simulate a better approximation of a continuous spectrum

  init_Ekin = h_Planck * c_light / (wavelengths[randPicker] * nm);

  G4ThreeVector momentum(px,pz,py);
  //G4ThreeVector momentum(0.,-1.,0.); //vertical incidence onto the ARAPUCA

  //momentum.unit() for normalisation

  // Determine the polarization of the photon

  G4double sx = cost*cosp;
  G4double sy = cost*sinp;
  G4double sz = -sint;

  G4ThreeVector polarization(sx, sy, sz);

  G4ThreeVector perp = momentum.cross(polarization);

  phi  = twopi*G4UniformRand();
  sinp = std::sin(phi);
  cosp = std::cos(phi);

  polarization = cosp * polarization + sinp * perp;
  polarization = polarization.unit();

  // Create a new photon

  G4PrimaryParticle* particle = new G4PrimaryParticle(G4OpticalPhoton::Definition());
  particle->SetMomentumDirection(momentum);
  particle->SetPolarization(polarization);
  particle->SetKineticEnergy(init_Ekin);

  G4PrimaryVertex* vertex = new G4PrimaryVertex(G4ThreeVector(0.,10.*cm,0.), 0.);
  vertex->SetPrimary(particle);

  event->AddPrimaryVertex(vertex);
}
