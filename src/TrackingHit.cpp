// -----------------------------------------------------------------------------
//  G4Basic | TrackingHit.cpp
//
//  TODO: Class description
//   * Author: Justo Martin-Albo
//   * Creation date: 14 Aug 2019
// -----------------------------------------------------------------------------

#include "TrackingHit.h"


G4ThreadLocal G4Allocator<TrackingHit>* TrackingHitAllocator=0;


TrackingHit::TrackingHit():
  G4VHit()
{
}


TrackingHit::~TrackingHit()
{
}


TrackingHit::TrackingHit(const TrackingHit& right): G4VHit()
{
  edep_ = right.edep_;
  xyz_  = right.xyz_;
}


const TrackingHit& TrackingHit::operator=(const TrackingHit& right)
{
  edep_ = right.edep_;
  xyz_  = right.xyz_;

  return *this;
}


G4bool TrackingHit::operator==(const TrackingHit& right) const
{
return ( this == &right ) ? true : false;
}


void TrackingHit::Draw()
{
}


void TrackingHit::Print()
{
}
