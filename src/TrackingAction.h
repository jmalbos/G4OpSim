// -----------------------------------------------------------------------------
//  G4Basic | TrackingAction.h
//
//  User run action class.
// -----------------------------------------------------------------------------

#ifndef TRACKING_ACTION_H
#define TRACKING_ACTION_H

#include <G4UserTrackingAction.hh>

class G4Track;


class TrackingAction: public G4UserTrackingAction
{
public:
  TrackingAction();
  ~TrackingAction();
  void PreUserTrackingAction(const G4Track*);
  void PostUserTrackingAction(const G4Track*);
};

inline TrackingAction::TrackingAction() {}
inline TrackingAction::~TrackingAction() {}

#endif
