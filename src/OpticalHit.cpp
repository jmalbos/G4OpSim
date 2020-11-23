// -----------------------------------------------------------------------------
//  G4Basic | OpticalHit.h
//
//  TODO: Class description
//   * Author: <justo.martin-albo@ific.uv.es>
//   * Creation date: 5 Nov 2020
// -----------------------------------------------------------------------------

#include "OpticalHit.h"


G4Allocator<OpticalHit> OpticalHitAllocator;


OpticalHit::OpticalHit():
  G4VHit(),
  sensor_id_(-1),
  time_bin_width_(0.)
{
}


OpticalHit::~OpticalHit()
{
}


OpticalHit::OpticalHit(const OpticalHit& other): G4VHit()
{
  *this = other;
}


const OpticalHit& OpticalHit::operator=(const OpticalHit& other)
{
  sensor_id_      = other.sensor_id_;
  time_bin_width_ = other.time_bin_width_;
  wvf_            = other.wvf_;

  return *this;
}


G4int OpticalHit::operator==(const OpticalHit& other) const
{
  return (this == &other) ? 1 : 0;
}


void OpticalHit::SetTimeBinWidth(G4double bin_size)
{
  if (wvf_.size() == 0) {
    time_bin_width_ = bin_size;
  }
  else {
    G4String msg = "A OpticalHit cannot be rebinned once it has been filled.";
    G4Exception("[OpticalHit]", "SetTimeBinWidth()", JustWarning, msg);
  }
}


void OpticalHit::Fill(G4double time, G4int counts)
{
  G4double time_bin = floor(time/time_bin_width_) * time_bin_width_;
  wvf_[time_bin] += counts;
}
