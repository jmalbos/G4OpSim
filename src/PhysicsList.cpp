// -----------------------------------------------------------------------------
//  G4Basic | G4Basic.cpp
//
//
// -----------------------------------------------------------------------------

#include "PhysicsList.h"

#include <G4EmStandardPhysics_option4.hh>
#include <G4DecayPhysics.hh>
#include <G4RadioactiveDecayPhysics.hh>
#include <G4StepLimiterPhysics.hh>


PhysicsList::PhysicsList(): G4VModularPhysicsList()
{
  RegisterPhysics(new G4EmStandardPhysics_option4());
  RegisterPhysics(new G4DecayPhysics());
  RegisterPhysics(new G4RadioactiveDecayPhysics());
  RegisterPhysics(new G4StepLimiterPhysics());
}


PhysicsList::~PhysicsList()
{
}


void PhysicsList::SetCuts()
{
  G4VUserPhysicsList::SetCuts();
}
