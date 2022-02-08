# G4OpSim
Geant4-based optical simulation.

# Configuring a dichroic filter in GEANT4

Within the detector construction section of the code, one defines the contstruction of the dichroic filter.

## General remarks

* For the dichroic filter, defining a G4OpticalSurface object is necessary:
 * `G4OpticalSurface* filtersurf_opsurf = new G4OpticalSurface(filtersurf_name, dichroic, polished, dielectric_dichroic, 1.);`
 * `filtersurf_opsurf->SetMaterialPropertiesTable(OpticalMaterialProperties::FusedSilica()); //material may vary` 
 * `new G4LogicalSkinSurface("FILTER_SURFACE", filter_logic_vol, filtersurf_opsurf);`
* To characterise the transmittance of the dichroic filter, it is necessary to define a configuration file that defines transmittance $T(\lambda, \theta)$ as a function of the wavelength $\lambda$ of the photon and  the incident angle $\theta$ (_**defined with respect to to the normal vector of the plane**_). A template will be shown in the next section. After the file is set up, its path needs to be called as an environment variable in the detector class method where one defines the dichroic filter
 * setenv("G4DICHROICDATA", "data/dichroic_data", 1);
 * The environment variable name has to be as written above

## Configuration of Transmittance

The following variables are used for simplification:

* `G4VectorType`: a value telling GEANT4 what type of vector to pick from an enum. For the dichroic filter, we set this to 4
* `m   `: Number of wavelength samples
* `n   `: Number of angle samples
* `wl_i`: $i$-th wavelength value ($1<i<m$)
* `a_j `: $j$-th angle value ($1<j<m$)
* `t_ij`: transmittance matrix element at $i$-th row and $j$-th column ($1<i<m$, $1<j<n$), i.e. $T(\lambda_i,\theta_j)$


```
<G4VectorType> <m> <n>
<wl_1> <wl_2> ... <wl_m>
<a_1> <a_2> ... <a_n>
<t_11> <t_12> ... <t_1n>
.    .              .
.         .         .
.              .    .
<t_m1> <t_m2> ... <t_mn>
```

Remarks on restrictions:
* $m,n\geq2$, otherwise GEANT4 does not accept the configuration file
* There can be formatting issues; the file has been tested that it works if the encoding is UTF-8 and there is always exactly one white space between values in a row and no extra whitespaces otherwise and no extra empty lines at the end of the file
