// -----------------------------------------------------------------------------
//  G4Basic | TrackingHit.h
//
//  TODO: Class description
//   * Author: Justo Martin-Albo
//   * Creation date: 12 Feb 2019
// -----------------------------------------------------------------------------

#ifndef TRACKING_HIT_H
#define TRACKING_HIT_H

#include <G4VHit.hh>
#include <G4THitsCollection.hh>
#include <G4ThreeVector.hh>


class TrackingHit: public G4VHit
{
public:
  TrackingHit();
  TrackingHit(const TrackingHit&);
  virtual ~TrackingHit();

  const TrackingHit& operator=(const TrackingHit&);
  G4bool operator==(const TrackingHit&) const;

  inline void* operator new(size_t);
  inline void  operator delete(void*);

  virtual void Draw();
  virtual void Print();

  void SetEdep(G4double);
  void SetPosition(const G4ThreeVector&);

private:
  G4double edep_;
  G4ThreeVector xyz_;
};

typedef G4THitsCollection<TrackingHit> TrackingHitsCollection;

extern G4ThreadLocal G4Allocator<TrackingHit>* TrackingHitAllocator;

inline void* TrackingHit::operator new(size_t)
{
  if (!TrackingHitAllocator) TrackingHitAllocator = new G4Allocator<TrackingHit>;
  return (void*) TrackingHitAllocator->MallocSingle();
}

inline void TrackingHit::operator delete(void* hit)
{
  TrackingHitAllocator->FreeSingle((TrackingHit*) hit);
}

inline void TrackingHit::SetEdep(G4double e) { edep_ = e; }

inline void TrackingHit::SetPosition(const G4ThreeVector& p) { xyz_ = p; }

#endif
