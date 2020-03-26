// -----------------------------------------------------------------------------
//  G4OpSim | Materials.h
//
//  Definitions of materials used in the geometry.
//  * Author:
//  * Creation date: 26 March 2020
// -----------------------------------------------------------------------------

#ifndef MATERIALS_H_
#define MATERIALS_H_

class G4Material;


namespace Materials {

  G4Material* FR4();

  G4Material* OpticalSilicone();

  G4Material* Epoxy();

  G4Material* FusedSilica();

} // end namespace Materials

#endif
