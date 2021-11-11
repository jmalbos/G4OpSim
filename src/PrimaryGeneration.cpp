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

#include "some_tools.h"


PrimaryGeneration::PrimaryGeneration():
  G4VUserPrimaryGeneratorAction(),
  //Introduce the histogram filepath with this exact format!
  spectrum_histogram(get_TH1D("./data/LED325W2-IvsLambda-TH1D.root")),
  angular_histogram(get_TH1D("./data/LED325W2-IvsAngle-TH1D.root"))
{
}


PrimaryGeneration::~PrimaryGeneration()
{
}


void PrimaryGeneration::GeneratePrimaries(G4Event* event)
{  
  //Azimuthal angle is uniformly sampled from [0,twopi]
  G4double phi = twopi*G4UniformRand();
  
  //Polar angle is sampled from the manufacturer's info.
  G4double theta = (angular_histogram->GetRandom())*(pi/180.0);
  
  //Kinetic energy is sampled from the manufacturer's info.
  G4double kinetic_energy_ = energy_in_eV(spectrum_histogram->GetRandom())*eV;

  G4double cosp = std::cos(phi);
  G4double sinp = std::sin(phi);

  G4double cost = std::cos(theta);
  G4double sint = std::sin(theta);

  G4double px = sint*cosp;
  G4double py = -cost;
  G4double pz = sint*sinp;

  G4ThreeVector momentum(px, py, pz);

  G4double sx = cost*cosp;
  G4double sy = sint;
  G4double sz = cost*sinp;

  //polarization is just a perpendicular vector to momentum
  G4ThreeVector polarization(sx, sy, sz);

  //perp, polarization and momentum are a cartesian basis
  G4ThreeVector perp = momentum.cross(polarization);

  //The actual polarization is a normalized random combination of perp and polarization
  phi  = twopi*G4UniformRand();
  sinp = std::sin(phi);
  cosp = std::cos(phi);

  polarization = cosp * polarization + sinp * perp;
  polarization = polarization.unit();
  
  //Create a new photon
  G4PrimaryParticle * particle = new G4PrimaryParticle(G4OpticalPhoton::Definition());
  particle->SetMomentumDirection(momentum);
  particle->SetPolarization(polarization);
  particle->SetKineticEnergy(kinetic_energy_);

  G4PrimaryVertex* vertex = new G4PrimaryVertex(G4ThreeVector(0.,10.*cm,0.), 0.);
  vertex->SetPrimary(particle);

  event->AddPrimaryVertex(vertex);
}
