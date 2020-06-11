// -----------------------------------------------------------------------------
//  G4Basic | PrimaryGeneration.cpp
//
//
// -----------------------------------------------------------------------------

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


PrimaryGeneration::PrimaryGeneration():
  G4VUserPrimaryGeneratorAction(),
  kinetic_energy_(6.*eV)
{
}


PrimaryGeneration::~PrimaryGeneration()
{
}


void PrimaryGeneration::GeneratePrimaries(G4Event* event)
{
  // Generate a random momentum direction for the photon

  G4double cost = 1. - 2.*G4UniformRand();
  G4double sint = std::sqrt((1.-cost)*(1.+cost));

  G4double phi = twopi*G4UniformRand();
  G4double sinp = std::sin(phi);
  G4double cosp = std::cos(phi);

  G4double px = sint*cosp;
  G4double py = sint*sinp;
  G4double pz = cost;

  G4ThreeVector momentum(px,py,pz);

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
  particle->SetKineticEnergy(kinetic_energy_);

  G4PrimaryVertex* vertex = new G4PrimaryVertex(G4ThreeVector(0.,0.,0.), 0.);
  vertex->SetPrimary(particle);

  event->AddPrimaryVertex(vertex);
}
