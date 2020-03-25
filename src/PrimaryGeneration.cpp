// -----------------------------------------------------------------------------
//  G4Basic | PrimaryGeneration.cpp
//
//
// -----------------------------------------------------------------------------

#include "PrimaryGeneration.h"

#include <G4Electron.hh>
#include <G4ParticleDefinition.hh>
#include <G4SystemOfUnits.hh>
#include <G4PrimaryParticle.hh>
#include <G4PrimaryVertex.hh>
#include <G4Event.hh>
#include <G4RandomDirection.hh>


PrimaryGeneration::PrimaryGeneration():
  G4VUserPrimaryGeneratorAction(),
  particle_def_(G4Electron::Definition()),
  kinetic_energy_(2.5*MeV)
{
}


PrimaryGeneration::~PrimaryGeneration()
{
}


void PrimaryGeneration::GeneratePrimaries(G4Event* event)
{
  G4PrimaryParticle* particle = new G4PrimaryParticle(particle_def_);
  particle->SetMomentumDirection(G4RandomDirection());
  particle->SetKineticEnergy(kinetic_energy_);

  G4PrimaryVertex* vertex = new G4PrimaryVertex(G4ThreeVector(0.,0.,0.), 0.);
  vertex->SetPrimary(particle);

  event->AddPrimaryVertex(vertex);
}
