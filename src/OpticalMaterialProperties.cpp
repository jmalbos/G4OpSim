// -----------------------------------------------------------------------------
//  G4OpSim | OpticalMaterialProperties.cpp
//
//  * Author:
//  * Creation date: 26 March 2020
// -----------------------------------------------------------------------------

#include "OpticalMaterialProperties.h"

#include <G4MaterialPropertiesTable.hh>

#include <CLHEP/Units/PhysicalConstants.h>

#include <cassert>

using CLHEP::h_Planck;
using CLHEP::c_light;


G4MaterialPropertiesTable* OpticalMaterialProperties::Vacuum()
{
  G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();

  G4double energies[] = {OpticalMaterialProperties::energy_min,
                         OpticalMaterialProperties::energy_max};

  // Refractive index (RINDEX)
  G4double rindex[] = {1., 1.};
  mpt->AddProperty("RINDEX", energies, rindex, 2);

  // Absorption length (ABSLENGTH)
  G4double abslength[] = {OpticalMaterialProperties::abslength_max,
                          OpticalMaterialProperties::abslength_max};
  mpt->AddProperty("ABSLENGTH", energies, abslength, 2);

  return mpt;
}

G4MaterialPropertiesTable* OpticalMaterialProperties::LAr()
{
  G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();

  G4double energy_step = 0.05 * eV;
  G4double energy_curr = OpticalMaterialProperties::energy_min;
  std::vector<G4double> energies;

  do {
    energies.push_back(energy_curr);
    energy_curr += energy_step;
  } while(energy_curr < OpticalMaterialProperties::energy_max);

  // Refractive index (RINDEX)
  // https://arxiv.org/pdf/2002.09346.pdf
  G4double a0  = 0.335;
  G4double aUV = 0.099;
  G4double aIR = 0.008;
  G4double lambdaUV = 106.6 * nm;
  G4double lambdaIR = 908.3 * nm;
  std::vector<G4double> rindex;

  for (auto energy: energies) {

    G4double wavelength = h_Planck * c_light / energy;
    G4double x = a0 + aUV * pow (wavelength, 2) / (pow (wavelength, 2) - pow (lambdaUV, 2)) + aIR * pow (wavelength, 2) / (pow (wavelength, 2) - pow (lambdaIR, 2));
    G4double rindex_element = sqrt (1 + 3 * x / (3 - x));
    rindex.push_back(rindex_element);
  }

  mpt->AddProperty("RINDEX", energies.data(), rindex.data(), energies.size());

  // Absorption length (ABSLENGTH)
  G4double energies_lim[]  = {OpticalMaterialProperties::energy_min,
                              OpticalMaterialProperties::energy_max};
  G4double abslength[]     = {OpticalMaterialProperties::abslength_max,
                              OpticalMaterialProperties::abslength_max};
  mpt->AddProperty("ABSLENGTH", energies_lim, abslength, 2);

  return mpt;
}

G4MaterialPropertiesTable* OpticalMaterialProperties::PVT()
{
  G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();

  G4double energy_step = 0.05 * eV;
  G4double energy_curr = OpticalMaterialProperties::energy_min;
  std::vector<G4double> energies;

  do {
    energies.push_back(energy_curr);
    energy_curr += energy_step;
  } while(energy_curr < 7.5 * eV); // Put 7.5 eV instead of energy_max = 11.3 eV in order to avoid divergence.

  // Refractive index (RINDEX)
  // The refractive index is fitted to a Sellmeier function: https://en.wikipedia.org/wiki/Sellmeier_equation
  G4double A = 1.421;
  G4double B1 = 0.9944;
  G4double C1 = 26250 * pow (nm, 2);
  std::vector<G4double> rindex;

  for (auto energy: energies) {

    G4double wavelength = h_Planck * c_light / energy;
    G4double rindex_element = sqrt (A + B1 * pow (wavelength, 2) / (pow (wavelength, 2) - C1));;
    rindex.push_back(rindex_element);
  }

  mpt->AddProperty("RINDEX", energies.data(), rindex.data(), energies.size());

  //Absorption length (ABSLENGTH)
  //assuming just WLS can absorve
  G4double energies_lim[]  = {OpticalMaterialProperties::energy_min,
                              OpticalMaterialProperties::energy_max};
  G4double abslength[]     = {OpticalMaterialProperties::abslength_max,
			      OpticalMaterialProperties::abslength_max};
  mpt->AddProperty("ABSLENGTH", energies_lim, abslength, 2);

  //WLS absorption WLSABSLENGTH
  //assuming maximum absorption length for extremal value
  G4double WLS_abslength[]  = {OpticalMaterialProperties::abslength_max,
			       OpticalMaterialProperties::abslength_max,
			       879.673*mm ,406.487*mm ,296.786*mm,165.772*mm,
			       118.88*mm  ,96.5343*mm ,70.4532*mm,38.5933*mm,
			       26.714 *mm ,16.9549*mm ,11.2558*mm,9.49174*mm,
			       8.78259*mm ,7.31249*mm ,6.59717*mm,6.05747*mm,
			       5.24668*mm ,4.38833*mm ,3.73978*mm,2.71100*mm,
			       2.1048*mm  ,1.97013*mm ,1.20762*mm,0.71176*mm,
			       0.803417*mm,0.803417*mm,0.97114*mm,1.63289*mm};
  G4double WLS_abs_energy[] = {OpticalMaterialProperties::energy_min,
			       2.90388*eV,
			       2.9723 *eV,2.98595*eV,2.9987 *eV,3.00867*eV,
			       3.01317*eV,3.01997*eV,3.02443*eV,3.03555*eV,
			       3.04441*eV,3.0508*eV ,3.05709*eV,3.06822*eV,
			       3.07265*eV,3.07916*eV,3.08353*eV,3.08565*eV,
			       3.09449*eV,3.10326*eV,3.11208*eV,3.12862*eV,
			       3.14538*eV,3.15101*eV,3.20314*eV,3.25895*eV,
			       3.27264*eV,3.88859*eV,3.91598*eV,4.01202*eV};
  G4int WLS_abs_entries = 30;
  mpt->AddProperty("WLSABSLENGTH", WLS_abs_energy, WLS_abslength, WLS_abs_entries);
  
  // Emision spectrum (WLSCOMPONENT)
  // from https://arxiv.org/pdf/1912.09191.pdf EJ286
  G4double WLS_emi_energy[]   = {2.32899*eV,2.38369*eV,2.46565*eV,2.57017*eV,2.61099*eV,
				 2.64714*eV,2.69664*eV,2.72479*eV,2.77871*eV,2.79583*eV,
				 2.84050*eV,2.85033*eV,2.86809*eV,2.89408*eV,2.91221*eV,
				 2.94121*eV,2.97986*eV,2.98148*eV,3.00011*eV,3.01669*eV,
				 3.01794*eV,3.03617*eV,3.04968*eV,3.04979*eV,3.06040*eV,
				 3.07350*eV,3.07475*eV,3.08767*eV,3.11679*eV,3.12031*eV};
  G4double WLS_emi_Spectrum[] = {0.00293557,0.0122252,0.0439192,0.121378,0.166870,
				 0.23135100,0.3592200,0.4271160,0.539329,0.579575,
				 0.75539500,0.7947390,0.8865420,0.973837,0.995832,
				 0.97616000,0.8592200,0.8414600,0.771788,0.658673,
				 0.63818100,0.5706950,0.4619520,0.442280,0.378345,
				 0.27670600,0.2562140,0.1865420,0.074247,0.056214};
  G4int WLS_emi_entries = 30;
  mpt->AddProperty("WLSCOMPONENT", WLS_emi_energy, WLS_emi_Spectrum, WLS_emi_entries); 
  
  //time that the WLS takes to emmit the absorved photon
  mpt->AddConstProperty("WLSTIMECONSTANT", 1. * ns);
  //mpt->AddConstProperty("WLSMEANNUMBERPHOTONS", 1);

  return mpt;
}

G4MaterialPropertiesTable* OpticalMaterialProperties::BC418()
{
  // crystals.saint-gobain.com/radiation-detection-scintillators/plastic-scintillators/fast-timing-bc-418-bc-420-bc-422-bc-422q#
  G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();

  G4double energy_step = 0.05 * eV;
  G4double energy_curr = OpticalMaterialProperties::energy_min;
  std::vector<G4double> energies;

  do {
    energies.push_back(energy_curr);
    energy_curr += energy_step;
  } while(energy_curr < 7.5 * eV); // Put 7.5 eV instead of energy_max = 11.3 eV in order to avoid divergence.

  // Refractive index (RINDEX)
  // The refractive index is fitted to a Sellmeier function: https://en.wikipedia.org/wiki/Sellmeier_equation
  G4double A = 1.421;
  G4double B1 = 0.9944;
  G4double C1 = 26250 * pow (nm, 2);
  std::vector<G4double> rindex;

  for (auto energy: energies) {

    G4double wavelength = h_Planck * c_light / energy;
    G4double rindex_element = sqrt (A + B1 * pow (wavelength, 2) / (pow (wavelength, 2) - C1));;
    rindex.push_back(rindex_element);
  }

  mpt->AddProperty("RINDEX", energies.data(), rindex.data(), energies.size());

  //Absorption length (ABSLENGTH)
  //assuming just WLS can absorve
  G4double energies_lim[]  = {OpticalMaterialProperties::energy_min,
                              OpticalMaterialProperties::energy_max};
  //from * https://www.crystals.saint-gobain.com/sites/hps-mac3-cma-crystals/files/2021-09/BC418-420-422%20Data%20Sheet.pdf
  G4double abslength[]     = {1.*m, 1.*m};
  mpt->AddProperty("ABSLENGTH", energies_lim, abslength, 2);

  //WLS absorption WLSABSLENGTH
  //assuming it is the same as for PVT
  G4double WLS_abslength[]  = {OpticalMaterialProperties::abslength_max,
			       OpticalMaterialProperties::abslength_max,
			       879.673*mm ,406.487*mm ,296.786*mm,165.772*mm,
			       118.88*mm  ,96.5343*mm ,70.4532*mm,38.5933*mm,
			       26.714 *mm ,16.9549*mm ,11.2558*mm,9.49174*mm,
			       8.78259*mm ,7.31249*mm ,6.59717*mm,6.05747*mm,
			       5.24668*mm ,4.38833*mm ,3.73978*mm,2.71100*mm,
			       2.1048*mm  ,1.97013*mm ,1.20762*mm,0.71176*mm,
			       0.803417*mm,0.803417*mm,0.97114*mm,1.63289*mm};
  G4double WLS_abs_energy[] = {OpticalMaterialProperties::energy_min,
			       2.90388*eV,
			       2.9723 *eV,2.98595*eV,2.9987 *eV,3.00867*eV,
			       3.01317*eV,3.01997*eV,3.02443*eV,3.03555*eV,
			       3.04441*eV,3.0508*eV ,3.05709*eV,3.06822*eV,
			       3.07265*eV,3.07916*eV,3.08353*eV,3.08565*eV,
			       3.09449*eV,3.10326*eV,3.11208*eV,3.12862*eV,
			       3.14538*eV,3.15101*eV,3.20314*eV,3.25895*eV,
			       3.27264*eV,3.88859*eV,3.91598*eV,4.01202*eV};
  G4int WLS_abs_entries = 30;
  mpt->AddProperty("WLSABSLENGTH", WLS_abs_energy, WLS_abslength, WLS_abs_entries);
  
  // Emision spectrum (WLSCOMPONENT)
  // from *
  G4double WLS_emi_energy[]   = {2.3896*eV, 2.4057*eV, 2.4283*eV, 2.4514*eV, 2.4777*eV, 
        2.5118*eV, 2.5448*eV, 2.5722*eV, 2.5991*eV, 2.6231*eV, 
        2.6508*eV, 2.677*eV, 2.7058*eV, 2.7307*eV, 2.7501*eV, 
        2.7819*eV, 2.8059*eV, 2.8246*eV, 2.8457*eV, 2.86*eV, 
        2.8773*eV, 2.8933*eV, 2.908*eV, 2.9199*eV, 2.9318*eV, 
        2.9424*eV, 2.956*eV, 2.9713*eV, 2.9837*eV, 2.9946*eV, 
        3.0056*eV, 3.0199*eV, 3.0246*eV, 3.0342*eV, 3.0422*eV, 
        3.0535*eV, 3.0617*eV, 3.0732*eV, 3.083*eV, 3.0914*eV, 
        3.1064*eV, 3.1171*eV, 3.1348*eV, 3.1682*eV, 3.1953*eV, 
        3.2098*eV, 3.2208*eV, 3.2301*eV, 3.2397*eV, 3.2414*eV, 
        3.249*eV, 3.2566*eV, 3.2642*eV, 3.2681*eV, 3.274*eV, 
        3.2778*eV, 3.2836*eV, 3.2914*eV, 3.2986*eV, 3.3031*eV, 
        3.3071*eV, 3.311*eV, 3.3189*eV, 3.3268*eV, 3.3406*eV, 
        3.3485*eV, 3.3625*eV, 3.3792*eV, 3.4008*eV, 3.4211*eV, 
        3.4478*eV, 3.4866*eV, 3.5343*eV};
  G4double WLS_emi_Spectrum[] = {0.0, 0.0, 0.0, 0.0, 0.0021, 
        0.0085, 0.0128, 0.0213, 0.0233, 0.0318, 
        0.0407, 0.0519, 0.0645, 0.081, 0.0936, 
        0.1237, 0.1498, 0.1733, 0.203, 0.2258, 
        0.2539, 0.2821, 0.3081, 0.3374, 0.3609, 
        0.3917, 0.4292, 0.4727, 0.5027, 0.5307, 
        0.557, 0.6055, 0.6264, 0.6595, 0.6837, 
        0.7373, 0.76, 0.7855, 0.8163, 0.839, 
        0.8814, 0.9087, 0.9337, 0.9441, 0.9173, 
        0.8839, 0.851, 0.8169, 0.7655, 0.7277, 
        0.6862, 0.6304, 0.5892, 0.5514, 0.4858, 
        0.4614, 0.4257, 0.376, 0.3294, 0.3023, 
        0.2687, 0.2384, 0.2018, 0.1756, 0.135, 
        0.1121, 0.0865, 0.0614, 0.0382, 0.0192, 
        0.0043, 0.0, 0.0};
  G4int WLS_emi_entries = 73;
  mpt->AddProperty("WLSCOMPONENT", WLS_emi_energy, WLS_emi_Spectrum, WLS_emi_entries); 
  
  //time that the WLS takes to emmit the absorved photon
  //from *
  mpt->AddConstProperty("WLSTIMECONSTANT", 1.4 * ns);
  //mpt->AddConstProperty("WLSMEANNUMBERPHOTONS", 1);

  return mpt;
}

G4MaterialPropertiesTable* OpticalMaterialProperties::PTP()
{
  G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();
  
  // Literature is not very extensive about this material
  // Refractive index (RINDEX)
  // https://en.wikipedia.org/wiki/Terphenyl it is not the best reference ever, i know.
  G4double rindex_energy[] = {OpticalMaterialProperties::energy_min,
			      OpticalMaterialProperties::energy_max};
  //TODO: look for RINDEX dependence 
  G4double rindex[]        = {1.65,1.65};
  int rindex_entries       = 2;

  mpt->AddProperty("RINDEX", rindex_energy, rindex, rindex_entries);

  // Absorption length (WLSABSLENGTH)
  // for the moment it is assumed that all LAr photons are absorved and then all emited photons pass through
    //TODO: look for real spectrum
  G4double abslength_energy[] = {OpticalMaterialProperties::energy_min, 5.49*eV,
				 5.5*eV, OpticalMaterialProperties::energy_max}; 
  G4double abslength[]        = {OpticalMaterialProperties::abslength_max,
				 OpticalMaterialProperties::abslength_max,
				 OpticalMaterialProperties::abslength_min,
				 OpticalMaterialProperties::abslength_min};
  int abslength_entries       = 4;
  mpt->AddProperty("WLSABSLENGTH", abslength_energy, abslength, abslength_entries);

  //PTP emision spsectra "WLSCOMPONENT"
  //https://deepblue.lib.umich.edu/bitstream/handle/2027.42/30880/0000545.pdf;jsessionid=574463A6129BB3C95B63594EBC262D68?sequence=1
  G4double emi_energy[]   = {3.06894*eV,3.15885*eV,3.21398*eV,3.30108*eV,3.32320*eV,
			     3.36117*eV,3.41334*eV,3.43137*eV,3.45221*eV,3.46007*eV,
			     3.47011*eV,3.47800*eV,3.50550*eV,3.51258*eV,3.52198*eV,
			     3.52792*eV,3.53843*eV,3.55350*eV,3.56298*eV,3.57339*eV,
			     3.59301*eV,3.60393*eV,3.62111*eV,3.63872*eV,3.64298*eV,
			     3.65721*eV,3.66756*eV,3.67709*eV,3.68821*eV,3.72512*eV};
  G4double emi_Spectrum[] = {0.0226077,0.101296,0.199985,0.378345,0.506214,
			     0.6566240,0.712772,0.752936,0.864903,0.931214,
			     0.9730180,0.989821,0.966870,0.922608,0.872608,
			     0.8275260,0.782444,0.738181,0.742280,0.785996,
			     0.8144110,0.763045,0.681078,0.517143,0.432444,
			     0.3308040,0.245012,0.170695,0.082170,0.006214};
  G4int emi_entries = 30;
  
  mpt->AddProperty("WLSCOMPONENT", emi_energy, emi_Spectrum, emi_entries);

  mpt->AddConstProperty("WLSTIMECONSTANT", 1. * ns);
  //mpt->AddConstProperty("WLSMEANNUMBERPHOTONS", 1);

  return mpt;
}

G4MaterialPropertiesTable* OpticalMaterialProperties::VIKUITI()
{
  G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();
  //info from David Warner 11/2019 talk
  //it is said that the reflective foils are coated with TPB,
  //shall we care about this?

  //reflection  (REFLECTIVITY)
  //from this talk https://indico.fnal.gov/event/24273/contributions/188657/attachments/130083/158244/DUNE_60Review1.pdf
  //we can see depedences in angle for large wavelengths > 800 nm.
  //For shorter wavelength almost no angle dependence, so we are using 60º curves.
  //We are only considering two options: the photon is either reflected or bulk-absorved.
  //If we want to consider transmission to the outer world we need to know refractive index.
  const G4int entries = 50;
  G4double energy[entries]       = {h_Planck * c_light / (351.408*nm),h_Planck * c_light / (360.963*nm),
				    h_Planck * c_light / (368.310*nm),h_Planck * c_light / (372.535*nm),
				    h_Planck * c_light / (375.704*nm),h_Planck * c_light / (376.761*nm),
				    h_Planck * c_light / (378.873*nm),h_Planck * c_light / (377.817*nm),
				    h_Planck * c_light / (379.930*nm),h_Planck * c_light / (382.042*nm),
				    h_Planck * c_light / (383.099*nm),h_Planck * c_light / (385.211*nm),
				    h_Planck * c_light / (387.621*nm),h_Planck * c_light / (393.662*nm),
				    h_Planck * c_light / (401.987*nm),h_Planck * c_light / (424.296*nm),
				    h_Planck * c_light / (451.312*nm),h_Planck * c_light / (476.056*nm),
				    h_Planck * c_light / (496.127*nm),h_Planck * c_light / (528.873*nm),
				    h_Planck * c_light / (555.282*nm),h_Planck * c_light / (591.197*nm),
				    h_Planck * c_light / (632.394*nm),h_Planck * c_light / (665.141*nm),
				    h_Planck * c_light / (691.231*nm),h_Planck * c_light / (720.826*nm),
				    h_Planck * c_light / (726.189*nm),h_Planck * c_light / (759.155*nm),
				    h_Planck * c_light / (796.127*nm),h_Planck * c_light / (837.324*nm),
				    h_Planck * c_light / (883.803*nm),h_Planck * c_light / (901.460*nm),
				    h_Planck * c_light / (911.037*nm),h_Planck * c_light / (923.967*nm),
				    h_Planck * c_light / (925.883*nm),h_Planck * c_light / (933.545*nm),
				    h_Planck * c_light / (934.981*nm),h_Planck * c_light / (940.845*nm),
				    h_Planck * c_light / (946.953*nm),h_Planck * c_light / (948.869*nm),
				    h_Planck * c_light / (957.010*nm),h_Planck * c_light / (958.925*nm),
				    h_Planck * c_light / (968.503*nm),h_Planck * c_light / (1008.45*nm),
				    h_Planck * c_light / (1038.03*nm),h_Planck * c_light / (1060.21*nm),
				    h_Planck * c_light / (1048.59*nm),h_Planck * c_light / (1103.52*nm),
				    h_Planck * c_light / (1154.23*nm),h_Planck * c_light / (1186.97*nm)};
								    			  
  G4double reflectivity[entries] = {0.120930,0.132038,0.125581,0.153488,0.206977,
				    0.272093,0.479070,0.376744,0.548837,0.623256,
				    0.702326,0.781395,0.848983,0.932558,0.970015,
				    0.979070,0.971596,0.983721,0.983721,0.979070,
				    0.988372,0.986047,0.981395,0.988372,0.970542,
				    0.969909,0.973705,0.990698,0.990698,0.993023,
				    0.995349,0.961053,0.912205,0.788151,0.764430,
				    0.658897,0.638444,0.553488,0.449727,0.420735,
				    0.310914,0.287368,0.216732,0.186047,0.169767,
				    0.172093,0.162791,0.167442,0.158140,0.174419};

  mpt->AddProperty("REFLECTIVITY" , energy, reflectivity , entries);
  
  return mpt;
}

G4MaterialPropertiesTable* OpticalMaterialProperties::FusedSilica()
{
  G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();
  //refractive index RINDEX
  //data from https://www.filmetrics.com/refractive-index-database/SiO2
  ///Fused-Silica-Silica-Silicon-Dioxide-Thermal-Oxide-ThermalOxide
  //it can be seen from https://nvlpubs.nist.gov/nistpubs/jres/75A/jresv75An4p279_A1b.pdf
  //that there is almost no dependence with temperature.
  //since PTP emits at 3.45 eV (~360 nm) and WLS emits at 2.9 eV (~430 nm)
  //no need to go to lower wavelengths
  G4double energy[] = {h_Planck * c_light / (210*nm) ,h_Planck * c_light / (220*nm),
		       h_Planck * c_light / (240*nm) ,h_Planck * c_light / (260*nm),
		       h_Planck * c_light / (280*nm) ,h_Planck * c_light / (300*nm),
		       h_Planck * c_light / (320*nm) ,h_Planck * c_light / (340*nm),
		       h_Planck * c_light / (360*nm) ,h_Planck * c_light / (380*nm),
		       h_Planck * c_light / (400*nm) ,h_Planck * c_light / (420*nm),
		       h_Planck * c_light / (440*nm) ,h_Planck * c_light / (460*nm),
		       h_Planck * c_light / (480*nm) ,h_Planck * c_light / (500*nm),
		       h_Planck * c_light / (520*nm) ,h_Planck * c_light / (540*nm),
		       h_Planck * c_light / (560*nm) ,h_Planck * c_light / (580*nm),
		       h_Planck * c_light / (600*nm) ,h_Planck * c_light / (650*nm),
		       h_Planck * c_light / (700*nm) ,h_Planck * c_light / (750*nm),
		       h_Planck * c_light / (800*nm) ,h_Planck * c_light / (850*nm),
		       h_Planck * c_light / (900*nm) ,h_Planck * c_light / (1000*nm),
		       h_Planck * c_light / (1100*nm),h_Planck * c_light / (1200*nm)}; 
  G4double rindex[] = {1.5384,1.5285,1.5133,1.5024,1.4942,
		       1.4878,1.4827,1.4787,1.4753,1.4725,
		       1.4701,1.4681,1.4663,1.4648,1.4635,
		       1.4623,1.4613,1.4603,1.4595,1.4587,
		       1.4580,1.4565,1.4553,1.4542,1.4533,
		       1.4525,1.4518,1.4504,1.4492,1.4481};
  G4int entries = 30;

  mpt->AddProperty("RINDEX", energy, rindex, entries);
  
  //data for the dichroic filter is stored in /data/dichroic_data
  
  
  return mpt;
}

G4MaterialPropertiesTable* OpticalMaterialProperties::GlassEpoxy()
{
  // Optical properties of Optorez 1330 glass epoxy.
  // Obtained from http://refractiveindex.info and
  // https://www.zeonex.com/Optics.aspx.html#glass-like
  G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();
  // REFRACTIVE INDEX
  // The range is chosen to be up to ~10.7 eV because Sellmeier's equation
  // for fused silica is valid only in that range
  const G4int ri_entries = 200;
  G4double eWidth = 
    (OpticalMaterialProperties::energy_max - OpticalMaterialProperties::energy_min) / ri_entries;
  G4double ri_energy[ri_entries];
  for (int i = 0; i < ri_entries; i++){
    ri_energy[i] = OpticalMaterialProperties::energy_min + i * eWidth;
  }
  G4double rIndex[ri_entries];
  for (int i = 0; i < ri_entries; i++) {
    G4double lambda = h_Planck*c_light/ri_energy[i]*1000; // in micron
    G4double n2 = 2.291142 - 3.311944E-2*pow(lambda,2) - 1.630099E-2*pow(lambda,-2) +
      7.265983E-3*pow(lambda,-4) - 6.806145E-4*pow(lambda,-6) +
      1.960732E-5*pow(lambda,-8);
    rIndex[i] = sqrt(n2);
    // G4cout << "* GlassEpoxy rIndex:  " << std::setw(5)
    //        << ri_energy[i]/eV << " eV -> " << rIndex[i] << G4endl;
  }
  assert(sizeof(rIndex) == sizeof(ri_energy));
  mpt->AddProperty("RINDEX", ri_energy, rIndex, ri_entries);
  // ABSORPTION LENGTH
  G4double abs_energy[] = {
    OpticalMaterialProperties::energy_max, 
    2.001 * eV,   2.132 * eV,   2.735 * eV,  2.908 * eV,
    3.119 * eV,   3.320 * eV,   3.476 * eV,  3.588 * eV,  
    3.749 * eV,   3.869 * eV,   3.973 * eV,  4.120 * eV,  
    4.224 * eV,   4.320 * eV,   4.420 * eV,  5.018 * eV
  };
  const G4int abs_entries = sizeof(abs_energy) / sizeof(G4double);
  G4double absLength[] = {
    OpticalMaterialProperties::abslength_max,
    OpticalMaterialProperties::abslength_max,
                  326.00 * mm,  117.68 * mm,  85.89 * mm,  
    50.93 * mm,   31.25 * mm ,   17.19 * mm,  10.46 * mm,   
    5.26 * mm ,   3.77 * mm  ,    2.69 * mm,   1.94 * mm,
    1.33 * mm ,   0.73 * mm  ,    0.32 * mm,   0.10 * mm
  };
  assert(sizeof(absLength) == sizeof(abs_energy));
  mpt->AddProperty("ABSLENGTH", abs_energy, absLength, abs_entries);
  return mpt;
}
