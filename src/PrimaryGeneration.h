// -----------------------------------------------------------------------------
//  G4Basic | PrimaryGeneration.cpp
//
//
// -----------------------------------------------------------------------------

#ifndef PRIMARY_GENERATION_H
#define PRIMARY_GENERATION_H

#include <G4VUserPrimaryGeneratorAction.hh>
#include <globals.hh>

class G4ParticleDefinition;


class PrimaryGeneration: public G4VUserPrimaryGeneratorAction
{
public:
  PrimaryGeneration();
  virtual ~PrimaryGeneration();
  virtual void GeneratePrimaries(G4Event*);

private:
  G4ParticleDefinition* particle_def_;
  G4double kinetic_energy_;
};

#endif
