// -----------------------------------------------------------------------------
//  G4Basic | PhysicsList.h
//
//  
// -----------------------------------------------------------------------------

#ifndef PHYSICS_LIST_H
#define PHYSICS_LIST_H

#include <G4VModularPhysicsList.hh>


class PhysicsList: public G4VModularPhysicsList
{
public:
  PhysicsList();
  virtual ~PhysicsList();
  virtual void SetCuts();
};

#endif
