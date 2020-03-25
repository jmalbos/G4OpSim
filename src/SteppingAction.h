// -----------------------------------------------------------------------------
//  G4Basic | SteppingAction.h
//
//  User stepping action class.
// -----------------------------------------------------------------------------

#ifndef STEPPING_ACTION_H
#define STEPPING_ACTION_H

#include <G4UserSteppingAction.hh>

class G4Step;


class SteppingAction: public G4UserSteppingAction
{
public:
  SteppingAction();
  virtual ~SteppingAction();
  virtual void UserSteppingAction(const G4Step*);
};

inline SteppingAction::SteppingAction() {}
inline SteppingAction::~SteppingAction() {}

#endif
