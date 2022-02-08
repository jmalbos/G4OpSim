// -----------------------------------------------------------------------------
//  G4Basic | SteppingAction.h
//
//  User stepping action class.
// -----------------------------------------------------------------------------

#ifndef STEPPING_ACTION_H
#define STEPPING_ACTION_H

#include <G4UserSteppingAction.hh>
#include <globals.hh>

#include <vector>

class G4Step;
class EventAction;


class SteppingAction: public G4UserSteppingAction
{
public:
  SteppingAction(EventAction* eventAction);
  virtual ~SteppingAction();
  virtual void UserSteppingAction(const G4Step*);
private:
  EventAction* fEventAction;
  int counter;
  std::vector<int> wl30_counters;
  std::vector<int> wl30_counters_passed;
  std::vector<int> wl45_counters;
  std::vector<int> wl45_counters_passed;
  G4double ypos;

};

#endif
