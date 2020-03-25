// -----------------------------------------------------------------------------
//  G4Basic | RunAction.h
//
//  User run action class.
// -----------------------------------------------------------------------------

#ifndef RUN_ACTION_H
#define RUN_ACTION_H

#include <G4UserRunAction.hh>

class G4Run;


class RunAction: public G4UserRunAction
{
public:
  RunAction();
  virtual ~RunAction();
  virtual void BeginOfRunAction(const G4Run*);
  virtual void EndOfRunAction(const G4Run*);
};

inline RunAction::RunAction() {}
inline RunAction::~RunAction() {}

#endif
