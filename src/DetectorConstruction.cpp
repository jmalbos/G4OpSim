// -----------------------------------------------------------------------------
//  G4OpSim | DetectorConstruction.cpp
//
//  * Author: <justo.martin-albo@ific.uv.es>
//  * Creation date: 10 February 2020
// -----------------------------------------------------------------------------

#include "DetectorConstruction.h"

#include "GenericPhotosensor.h"
#include "Materials.h"
#include "OpticalMaterialProperties.h"
#include "OpticalSD.h"

#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4Sphere.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4SystemOfUnits.hh>
#include <G4PhysicalConstants.hh>
#include <G4NistManager.hh>
#include <G4VisAttributes.hh>
#include <G4RotationMatrix.hh>
#include <G4UserLimits.hh>
#include <G4SDManager.hh>
#include <G4OpticalSurface.hh>
#include <G4LogicalSkinSurface.hh>
#include <G4LogicalBorderSurface.hh>


DetectorConstruction::DetectorConstruction():
  G4VUserDetectorConstruction(),
  world_size_   (10.*m),
  plate_width_  (112.0*mm), // X
  plate_thickn_ (  4.0*mm), // Y
  plate_length_ (491.5*mm), // Z
  foil_thickn_  (0.165*mm), // Z&X
  foil_gap_     (1.*mm),
  filter_thickn_(1.*mm),    // Y
  filter_gap_   (1.5*mm),
  ptp_thickn_   (0.002*mm), // Y
  num_phsensors (48)
{
}


DetectorConstruction::~DetectorConstruction()
{
}


G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // WORLD ///////////////////////////////////////////////////////////
  // Sphere of liquid argon that contains all other volumes.

  const G4String world_name = "WORLD";

  G4Material* LAr = G4NistManager::Instance()->FindOrBuildMaterial("G4_lAr");
  LAr->SetMaterialPropertiesTable(OpticalMaterialProperties::LAr());

  G4Sphere* world_solid_vol =
    new G4Sphere(world_name, 0., world_size_/2., 0., 360.*deg, 0., 180.*deg);

  G4LogicalVolume* world_logic_vol =
    new G4LogicalVolume(world_solid_vol, LAr, world_name);
  world_logic_vol->SetVisAttributes(G4VisAttributes::Invisible);

  G4VPhysicalVolume* world_phys_vol =
    new G4PVPlacement(nullptr, G4ThreeVector(),
                      world_logic_vol, world_name, nullptr, false, 0, true);

  ////////////////////////////////////////////////////////////////////

  ConstructWLSPlate(world_phys_vol);
  ConstructPhotosensors(world_phys_vol);
  ConstructReflectiveFoils(world_phys_vol);
  ConstructDichroicFilter(world_phys_vol);
  ConstructPTPLayer(world_phys_vol);

  return world_phys_vol;
}


void DetectorConstruction::ConstructWLSPlate(G4VPhysicalVolume* world_phys_vol) const
{
  // WLS PLATE ///////////////////////////////////////////////////////

  Assert(world_phys_vol, "DetectorConstruction::ConstructWLSPlate()");

  const G4String plate_name = "WLS_PLATE";

  G4Box* plate_solid_vol =
    new G4Box(plate_name, plate_width_/2., plate_thickn_/2., plate_length_/2.);

  G4Material* pvt = G4NistManager::Instance()->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  pvt->SetMaterialPropertiesTable(OpticalMaterialProperties::PVT());

  G4LogicalVolume* plate_logic_vol =
    new G4LogicalVolume(plate_solid_vol, pvt, plate_name);

  new G4PVPlacement(nullptr, G4ThreeVector(0.,0.,0.),
                    plate_logic_vol, plate_name, world_phys_vol->GetLogicalVolume(),
                    false, 0, true);
}


void DetectorConstruction::ConstructPhotosensors(G4VPhysicalVolume* world_phys_vol) const
{
  Assert(world_phys_vol, "DetectorConstruction::ConstructPhotosensors()");

  GenericPhotosensor photosensor_geom;
  photosensor_geom.Construct();
  G4LogicalVolume* photosensor_logic_vol = photosensor_geom.GetLogicalVolume();

  if (!photosensor_logic_vol) {
    G4Exception("DetectorConstruction::ConstructPhotosensors()", "nullptr",
                FatalException, "    Description : Null pointer to logical volume.");
  }

  G4int phsensor_id = 0;

  G4RotationMatrix* rot = new G4RotationMatrix();
  rot->rotateY(-90*deg);

  for (G4int i=0; i<num_phsensors/2; ++i) {

    phsensor_id = i;

    G4ThreeVector pos(-plate_width_/2. - 1.*mm,
                      0.,
                      -plate_length_/2. + (0.5 + i) * plate_length_/(num_phsensors/2));

    new G4PVPlacement(rot, pos,
                      photosensor_logic_vol, photosensor_logic_vol->GetName(),
                      world_phys_vol->GetLogicalVolume(),
                      false, phsensor_id, true);
  }

  G4RotationMatrix* rot2 = new G4RotationMatrix();
  rot2->rotateY(90*deg);

  for (G4int i=0; i<num_phsensors/2; ++i) {

    phsensor_id = i;

    G4ThreeVector pos(+plate_width_/2. + 1.*mm,
                      0.,
                      -plate_length_/2. + (0.5 + i) * plate_length_/(num_phsensors/2));

    new G4PVPlacement(rot2, pos,
                      photosensor_logic_vol, photosensor_logic_vol->GetName(),
                      world_phys_vol->GetLogicalVolume(),
                      false, phsensor_id, true);
  }

  // //////////////////////////////////////////////////////////
  // // describing photosensors position
  // const G4int nsens = 48; //n SiPMs per arapuca cell
  // G4int nsides = 2; // n sides of the sell covered  with SiPMs
  // const G4double sensSpacing = plate_length/(nsens/2)*mm;
  // G4double firstPosition = sensSpacing/2;
  // G4ThreeVector SiPMpos;
  // G4double Zpos;
  // G4double Xpos;
  // G4int nsensStart;
  // G4String SiPM_name = "SiPM";
  // G4RotationMatrix* rotationMatrix = new G4RotationMatrix();
  //
  // G4VPhysicalVolume* photosens_phys_vol[nsens];
  //
  // for (G4int iside = 0; iside < nsides; iside ++)
  // {
  //   if(iside == 0)
  //   {
  //     Xpos = plate_width/2 + 6*mm;
  //     rotationMatrix->rotateY(90.*deg); //they have to be facing de plate
  //     nsensStart = 0;
  //     //rotationMatrix->print(std::cout);
  //
  //   }
  //   else if(iside == 1)
  //   {
  //     Xpos = -(plate_width/2+6*mm);
  //     rotationMatrix->rotateY(180.*deg); //rotations apply sequently, so this rotation is 90º+180º
  //     nsensStart = nsens/2;
  //     //rotationMatrix->print(std::cout);
  //   }
  //   for (G4int isens = 0; isens < nsens/2; isens++)
  //   {
  //     Zpos  = -plate_length/2 + firstPosition + isens * sensSpacing;
  //    // Not sure if the sensors shuld be plased in the world logic word
  //    // When putting them in plate_logic_vol it produces overlap errors
  //     photosens_phys_vol[nsensStart+isens]=
  //     	new G4PVPlacement(rotationMatrix,G4ThreeVector(Xpos,0.,Zpos),
	// 		  this->GenericPhotosensor(),SiPM_name, world_logic_vol,
	// 		  false, isens, true);
  //   }
  // }
  //
  // return world_phys_vol;

}

void DetectorConstruction::ConstructReflectiveFoils(G4VPhysicalVolume* world_phys_vol) const
{
  // REFLECTIVE FOILS ///////////////////////////////////////////////////////

  Assert(world_phys_vol, "DetectorConstruction::ConstructWLSPlate()");
  
  //there are three reflective foils, one in the bottom, two in the sides
  //first create the logical volume and then create the surface with LAr
  const G4String bottom_foil_name = "REF_BOTTOM_FOIL";
  const G4String side_foil_name   = "REF_SIDE_FOIL";
  
  //for the moment, same area as WLS plate. Should be checked.
  //thickness from manufacturer, should be checked.
  //https://www.isoltronic.ch/assets/of-m-vikuiti-esr-app-guide.pdf
  
  G4Box* bottom_foil_solid_vol =
    new G4Box(bottom_foil_name, plate_width_/2. ,foil_thickn_/2., plate_length_/2.);
  
  G4Box* side_foil_solid_vol =
    new G4Box(side_foil_name, plate_width_/2., plate_thickn_/2., foil_thickn_/2.);
  
  G4Material* plexiglass = G4NistManager::Instance()->FindOrBuildMaterial("G4_PLEXIGLASS");
  
  G4LogicalVolume* bottom_foil_logic_vol = 
    new G4LogicalVolume(bottom_foil_solid_vol, plexiglass, bottom_foil_name);
  
  G4LogicalVolume* side_foil_logic_vol = 
    new G4LogicalVolume(side_foil_solid_vol, plexiglass, side_foil_name);
  
  //the position is still unknown. For the moment put 1mm, should be checked
  G4ThreeVector bottom_foil_pos(0, -plate_thickn_/2 - foil_thickn_/2 - foil_gap_, 0);
  new G4PVPlacement(nullptr, bottom_foil_pos,
		    bottom_foil_logic_vol, bottom_foil_name, 
		    world_phys_vol->GetLogicalVolume(),
		    false, 0, true);
  
  G4ThreeVector side_foil_pos(0, 0, plate_length_/2 + foil_thickn_/2 + foil_gap_);
  new G4PVPlacement(nullptr, side_foil_pos,
	      side_foil_logic_vol, side_foil_name, 
	      world_phys_vol->GetLogicalVolume(),
	      false, 0, true);
  
  new G4PVPlacement(nullptr, -side_foil_pos,
		    side_foil_logic_vol, side_foil_name, 
		    world_phys_vol->GetLogicalVolume(),
		    false, 0, true);
 
  //now create the surface
  const G4String refsurf_name = "REF_SURFACE";
  G4OpticalSurface* refsurf_opsurf = 
    new G4OpticalSurface(refsurf_name, unified, polishedfrontpainted, dielectric_dielectric, 1);
  
  refsurf_opsurf->SetMaterialPropertiesTable(OpticalMaterialProperties::VIKUITI());
  new G4LogicalSkinSurface("REF_FOIL_SURFACE",bottom_foil_logic_vol,refsurf_opsurf);
  new G4LogicalSkinSurface("REF_FOIL_SURFACE",side_foil_logic_vol,refsurf_opsurf);
}

void DetectorConstruction::ConstructDichroicFilter(G4VPhysicalVolume* world_phys_vol) const
{
  // DICHROIC FILTER ///////////////////////////////////////
  const G4String filter_name = "DICHROIC_FILTER";
  
  //dimensions should be checked
  //1.5*mm from https://indico.fnal.gov/event/45283/contributions/195721/attachments/133823/165234/X-ARAPUCA_Cuts_Study.pdf
  //and 1*mm is the assumed filter width
  G4Box* filter_solid_vol =
    new G4Box(filter_name, plate_width_/2., filter_thickn_/2., plate_length_/2.);
  
  G4Material* filter_material = G4NistManager::Instance()->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
  filter_material->SetMaterialPropertiesTable(OpticalMaterialProperties::FusedSilica());
  
  G4LogicalVolume* filter_logic_vol =
    new G4LogicalVolume(filter_solid_vol,filter_material,filter_name);
  
  G4ThreeVector filter_pos(0, plate_thickn_/2 + filter_gap_ + filter_thickn_/2, 0);
  new G4PVPlacement(nullptr, filter_pos,
		    filter_logic_vol, filter_name, 
		    world_phys_vol->GetLogicalVolume(),
		    false, 0, true);
		      
  //now create the surface. It is a dichroic filter, so we need more things
  setenv("G4DICHROICDATA","data/dichroic_data",1);
  const G4String filtersurf_name = "FILTER_SURFACE";
  G4OpticalSurface* filtersurf_opsurf = 
    new G4OpticalSurface(filtersurf_name, dichroic, polished, dielectric_dichroic, 1);
  
  filtersurf_opsurf->SetMaterialPropertiesTable(OpticalMaterialProperties::FusedSilica());
  new G4LogicalSkinSurface("FILTER_SURFACE",filter_logic_vol,filtersurf_opsurf);
}

void DetectorConstruction::ConstructPTPLayer(G4VPhysicalVolume* world_phys_vol) const
{
  // PTP LAYER ///////////////////////////////////////
  const G4String ptp_name = "PTP_LAYER";
  
  //real dimensions not known. To be reviewed
  G4Box* ptp_solid_vol =
    new G4Box(ptp_name, plate_width_/2., ptp_thickn_/2., plate_length_/2.);
  
  G4Material* ptp = G4NistManager::Instance()->FindOrBuildMaterial("G4_TERPHENYL");
  ptp->SetMaterialPropertiesTable(OpticalMaterialProperties::PTP());
  
  G4LogicalVolume* ptp_logic_vol = new G4LogicalVolume(ptp_solid_vol, ptp, ptp_name);
  
  //position still not known, to be reviewed
  G4ThreeVector ptp_pos(0, plate_thickn_/2 + filter_gap_ + filter_thickn_/2 + ptp_thickn_/2, 0);  
  new G4PVPlacement(nullptr, ptp_pos,
		    ptp_logic_vol, ptp_name, 
		    world_phys_vol->GetLogicalVolume(),
		    false, 0, true);
  
}

void DetectorConstruction::Assert(G4VPhysicalVolume* ptr, const G4String& origin) const
{
  if (!ptr) {
    G4ExceptionDescription ed;
    ed << "    Description : World physical volume is not defined.";
    G4Exception(origin, "nullptr", FatalException, ed);
  }
}




  // // REFLECTIVE FOIL /////////////////////////////////////////////
  //
  // //there are three reflective foils, one in the bottom, two in the sides
  // //first create the logical volume and then create the surface with LAr
  // const G4String bottom_foil_name = "REF_BOTTOM_FOIL";
  // const G4String side_foil_name   = "REF_SIDE_FOIL";
  //
  // //for the moment, same area as WLS plate. Should be checked.
  // //thickness from manufacturer, should be checked.
  // //https://www.isoltronic.ch/assets/of-m-vikuiti-esr-app-guide.pdf
  // const G4double bottom_foil_width  = 112.0*mm; // X
  // const G4double bottom_foil_thickn = 0.165*mm; // Y
  // const G4double bottom_foil_length = 491.5*mm; // Z
  //
  // const G4double side_foil_width  = 112.0*mm; // X
  // const G4double side_foil_thickn =   4.0*mm;
  // const G4double side_foil_length = 0.165*mm; // Y
  //
  // G4Box* bottom_foil_solid_vol =
  //   new G4Box(bottom_foil_name, bottom_foil_width/2.,
	//       bottom_foil_thickn/2., bottom_foil_length/2.);
  //
  // G4Box* side_foil_solid_vol =
  //   new G4Box(side_foil_name, side_foil_width/2.,
	//       side_foil_thickn/2., side_foil_length/2.);
  //
  // G4Material* plexiglass = G4NistManager::Instance()->FindOrBuildMaterial("G4_PLEXIGLASS");
  //
  // G4LogicalVolume* bottom_foil_logic_vol = new
  //   G4LogicalVolume(bottom_foil_solid_vol, plexiglass, bottom_foil_name);
  //
  // G4LogicalVolume* side_foil_logic_vol = new
  //   G4LogicalVolume(side_foil_solid_vol, plexiglass, side_foil_name);
  //
  // //the position is still unknown. For the moment put 3mm, should be checked
  // const G4double bottom_foil_posy = -bottom_foil_thickn/2. - 3*mm;
  // const G4double side_foil_posz   = bottom_foil_length/2 + 3*mm;
  //
  // G4VPhysicalVolume* bottom_foil_phys_vol =
  //   new G4PVPlacement(nullptr, G4ThreeVector(0.,bottom_foil_posy,0.),
  //                     bottom_foil_logic_vol, bottom_foil_name, world_logic_vol,
  //                     false, 0, true);
  //
  // G4VPhysicalVolume* side1_foil_phys_vol =
  //   new G4PVPlacement(nullptr, G4ThreeVector(0.,0.,side_foil_posz),
  //                     side_foil_logic_vol, side_foil_name, world_logic_vol,
  //                     false, 0, true);
  //
  // G4VPhysicalVolume* side2_foil_phys_vol =
  //   new G4PVPlacement(nullptr, G4ThreeVector(0.,0.,-side_foil_posz),
	// 	      side_foil_logic_vol, side_foil_name, world_logic_vol,
	// 	      false, 0, true);
  //
  // //now create the surface
  // const G4String refsurf_name = "REF_SURFACE";
  // G4OpticalSurface* refsurf_opsurf = new
  //   G4OpticalSurface(refsurf_name, unified, polishedfrontpainted, dielectric_dielectric, 1);
  //
  // refsurf_opsurf->SetMaterialPropertiesTable(OpticalMaterialProperties::VIKUITI());
  // new G4LogicalSkinSurface("REF_FOIL_SURFACE",bottom_foil_logic_vol,refsurf_opsurf);
  // new G4LogicalSkinSurface("REF_FOIL_SURFACE",side_foil_logic_vol,refsurf_opsurf);
  //
  //
  // // DICHROIC FILTER ///////////////////////////////////////
  //
  // const G4String filter_name = "DICHROIC_FILTER";
  //
  // //dimensions should be checked
  // const G4double filter_width  = 112.0*mm; // X
  // const G4double filter_thickn =   1.0*mm; // Y
  // const G4double filter_length = 491.5*mm; // Z
  //
  // G4Box* filter_solid_vol =
  //   new G4Box(filter_name, filter_width/2., filter_thickn/2., filter_length/2.);
  //
  // G4Material* filter_material = G4NistManager::Instance()->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
  // filter_material->SetMaterialPropertiesTable(OpticalMaterialProperties::FusedSilica());
  //
  // G4LogicalVolume* filter_logic_vol =
  //   new G4LogicalVolume(filter_solid_vol,filter_material,filter_name);
  //
  // G4double filter_wls_gap = 1.5*mm;
  // G4double filter_ypos = plate_thickn/2. + filter_wls_gap + filter_thickn/2.;
  //
  // G4VPhysicalVolume* filter_phys_vol =
  //   new G4PVPlacement(nullptr, G4ThreeVector(0.,filter_ypos,0.),
  // 		      filter_logic_vol, filter_name, world_logic_vol,
	// 	      false, 0, true);
  //
  // //now create the surface. It is a dichroic filter, so we need more things
  // setenv("G4DICHROICDATA","data/dichroic_data",1);
  // const G4String filtersurf_name = "FILTER_SURFACE";
  // G4OpticalSurface* filtersurf_opsurf = new
  // G4OpticalSurface(filtersurf_name, dichroic, polished, dielectric_dichroic, 1);
  //
  // refsurf_opsurf->SetMaterialPropertiesTable(OpticalMaterialProperties::FusedSilica());
  // new G4LogicalSkinSurface("FILTER_SURFACE",filter_logic_vol,filtersurf_opsurf);
  //
  //
  // // PTP LAYER ///////////////////////////////////////
  //
  // const G4String ptp_name = "PTP_LAYER";
  //
  // //real dimensions not known. To be reviewed
  // const G4double ptp_width  = 112.0*mm; // X
  // const G4double ptp_thickn = 0.002*mm; // Y from https://arxiv.org/pdf/1912.09191.pdf
  // const G4double ptp_length = 491.5*mm; // Z
  //
  // G4Box* ptp_solid_vol =
  //   new G4Box(ptp_name, ptp_width/2., ptp_thickn/2., ptp_length/2.);
  //
  // G4Material* ptp = G4NistManager::Instance()->FindOrBuildMaterial("G4_TERPHENYL");
  // ptp->SetMaterialPropertiesTable(OpticalMaterialProperties::PTP());
  //
  // G4LogicalVolume* ptp_logic_vol = new G4LogicalVolume(ptp_solid_vol, ptp, ptp_name);
  //
  // //position still not known, to be reviewed
  // const G4double ptp_posy = plate_thickn/2 + filter_wls_gap + filter_thickn + ptp_thickn/2;
  // //1.5*mm from https://indico.fnal.gov/event/45283/contributions/195721/attachments/133823/165234/X-ARAPUCA_Cuts_Study.pdf
  // //and 1*mm is the assumed filter width
  // G4VPhysicalVolume* ptp_phys_vol =
  //   new G4PVPlacement(nullptr, G4ThreeVector(0.,ptp_posy,0.),
  //                     ptp_logic_vol, ptp_name, world_logic_vol,
  //                     false, 0, true);
  //
  //
  // //////////////////////////////////////////////////////////
  // // describing photosensors position
  // const G4int nsens = 48; //n SiPMs per arapuca cell
  // G4int nsides = 2; // n sides of the sell covered  with SiPMs
  // const G4double sensSpacing = plate_length/(nsens/2)*mm;
  // G4double firstPosition = sensSpacing/2;
  // G4ThreeVector SiPMpos;
  // G4double Zpos;
  // G4double Xpos;
  // G4int nsensStart;
  // G4String SiPM_name = "SiPM";
  // G4RotationMatrix* rotationMatrix = new G4RotationMatrix();
  //
  // G4VPhysicalVolume* photosens_phys_vol[nsens];
  //
  // for (G4int iside = 0; iside < nsides; iside ++)
  // {
  //   if(iside == 0)
  //   {
  //     Xpos = plate_width/2 + 6*mm;
  //     rotationMatrix->rotateY(90.*deg); //they have to be facing de plate
  //     nsensStart = 0;
  //     //rotationMatrix->print(std::cout);
  //
  //   }
  //   else if(iside == 1)
  //   {
  //     Xpos = -(plate_width/2+6*mm);
  //     rotationMatrix->rotateY(180.*deg); //rotations apply sequently, so this rotation is 90º+180º
  //     nsensStart = nsens/2;
  //     //rotationMatrix->print(std::cout);
  //   }
  //   for (G4int isens = 0; isens < nsens/2; isens++)
  //   {
  //     Zpos  = -plate_length/2 + firstPosition + isens * sensSpacing;
  //    // Not sure if the sensors shuld be plased in the world logic word
  //    // When putting them in plate_logic_vol it produces overlap errors
  //     photosens_phys_vol[nsensStart+isens]=
  //     	new G4PVPlacement(rotationMatrix,G4ThreeVector(Xpos,0.,Zpos),
	// 		  this->GenericPhotosensor(),SiPM_name, world_logic_vol,
	// 		  false, isens, true);
  //   }
  // }
  //
  // return world_phys_vol;
// }








// G4LogicalVolume* DetectorConstruction::GenericPhotosensor()
// {
//   // PHOTOSENSOR ENCASING //////////////////////////////////
//
//   const G4double width_     = 6.*mm;
//   const G4double height_    = 6.*mm;
//   const G4double thickness_ = 2.*mm;
//
//   G4String name = "PHOTOSENSOR";
//
//   G4Box* encasing_solid_vol =
//     new G4Box(name, width_/2., height_/2., thickness_/2.);
//
//   G4LogicalVolume* encasing_logic_vol =
//     new G4LogicalVolume(encasing_solid_vol, Materials::FR4(), name);
//
//   // OPTICAL WINDOW ////////////////////////////////////////
//
//   name = "PHOTOSENSOR_WINDOW";
//
//   G4double window_thickness = thickness_/4.;
//
//   G4Box* window_solid_vol =
//     new G4Box(name, width_/2., height_/2., window_thickness/2.);
//
//
//   G4LogicalVolume* window_logic_vol =
//     new G4LogicalVolume(window_solid_vol,
//                         Materials::OpticalSilicone(),
//                         name);
//
//   //THE SIPM IS CREATED FACING +Z DIRECTION
//   G4double zpos = thickness_/2. - window_thickness/2.;
//
//   new G4PVPlacement(nullptr, G4ThreeVector(0., 0., zpos),
//                     window_logic_vol, name, encasing_logic_vol,
//                     false, 0, false);
//
//   // PHOTOSENSITIVE AREA /////////////////////////////////////////////
//
//   name = "PHOTOSENSOR_SENSAREA";
//
//   G4double sensarea_thickness = 0.1*mm;
//
//   G4Box* sensarea_solid_vol =
//     new G4Box(name, width_/2., height_/2., sensarea_thickness/2.);
//
//   G4LogicalVolume* sensarea_logic_vol =
//     new G4LogicalVolume(sensarea_solid_vol,
//                         G4NistManager::Instance()->FindOrBuildMaterial("G4_Si"),
//                         name);
//
//   zpos = thickness_/2. - window_thickness - sensarea_thickness/2.;
//
//   new G4PVPlacement(nullptr, G4ThreeVector(0., 0., zpos),
//                     sensarea_logic_vol, name, encasing_logic_vol,
//                     false, 0, false);
//
//   // OPTICAL PROPERTIES //////////////////////////////////////////////
//
//   name = "PHOTOSENSOR_OPSURF";
//
//   G4double energy[]       = {0.2*eV, 11.5*eV};
//   G4double reflectivity[] = {0.0   ,  0.0};
//   G4double efficiency[]   = {1.0   ,  1.0};
//
//   G4MaterialPropertiesTable* photosensor_mpt = new G4MaterialPropertiesTable();
//   photosensor_mpt->AddProperty("REFLECTIVITY", energy, reflectivity, 2);
//   photosensor_mpt->AddProperty("EFFICIENCY",   energy, efficiency,   2);
//
//   G4OpticalSurface* photosensor_opsurf =
//     new G4OpticalSurface(name, unified, polished, dielectric_metal);
//   photosensor_opsurf->SetMaterialPropertiesTable(photosensor_mpt);
//   new G4LogicalSkinSurface(name, sensarea_logic_vol, photosensor_opsurf);
//
//   // SENSITIVE DETECTOR //////////////////////////////////////////////
//
//   OpticalSD* sensdet = new OpticalSD("/GENERIC_PHOTOSENSOR/SiPM");
//   G4SDManager::GetSDMpointer()->AddNewDetector(sensdet);
//   sensarea_logic_vol->SetSensitiveDetector(sensdet);
//
//   ////////////////////////////////////////////////////////////////////
//
//   return encasing_logic_vol;
// }
