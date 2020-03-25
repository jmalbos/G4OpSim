// -----------------------------------------------------------------------------
//  G4Basic | EventAction.h
//
//  User run action class.
// -----------------------------------------------------------------------------

#ifndef EVENT_ACTION_H
#define EVENT_ACTION_H

#include <G4UserEventAction.hh>

class G4Event;


class EventAction: public G4UserEventAction
{
public:
  EventAction();
  virtual ~EventAction();
  virtual void BeginOfEventAction(const G4Event*);
  virtual void EndOfEventAction(const G4Event*);
};

inline EventAction::EventAction() {}
inline EventAction::~EventAction() {}

#endif
