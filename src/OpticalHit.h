// -----------------------------------------------------------------------------
//  G4Basic | OpticalHit.h
//
//  TODO: Class description
//   * Author: <justo.martin-albo@ific.uv.es>
//   * Creation date: 5 Nov 2020
// -----------------------------------------------------------------------------

#ifndef OPTICAL_HIT_H
#define OPTICAL_HIT_H

#include <G4VHit.hh>
#include <G4THitsCollection.hh>
#include <G4Allocator.hh>
#include <G4ThreeVector.hh>



class OpticalHit: public G4VHit
{
public:
  OpticalHit();
  OpticalHit(const OpticalHit&);
  ~OpticalHit();

  const OpticalHit& operator=(const OpticalHit&);
  G4int operator==(const OpticalHit&) const;

  void* operator new(size_t);
  void  operator delete(void*);

  G4int GetSensorID() const;
  void  SetSensorID(G4int);

  G4double GetTimeBinWidth() const;
  void     SetTimeBinWidth(G4double);

  void Fill(G4double time, G4int counts=1);

  const std::map<G4double, G4int>& GetWaveform() const;

private:
  G4int sensor_id_;
  G4double time_bin_width_;
  std::map<G4double, G4int> wvf_;
};

//////////////////////////////////////////////////////////////////////

typedef G4THitsCollection<OpticalHit> OpticalHitCollection;
extern G4Allocator<OpticalHit> OpticalHitAllocator;

inline void* OpticalHit::operator new(size_t)
{ return ((void*) OpticalHitAllocator.MallocSingle()); }

inline void OpticalHit::operator delete(void* hit)
{ OpticalHitAllocator.FreeSingle((OpticalHit*) hit); }

inline G4int OpticalHit::GetSensorID() const { return sensor_id_; }
inline void  OpticalHit::SetSensorID(G4int id) { sensor_id_ = id; }

inline G4double OpticalHit::GetTimeBinWidth() const { return time_bin_width_; }

inline const std::map<G4double, G4int>& OpticalHit::GetWaveform() const
{ return wvf_; }

#endif
