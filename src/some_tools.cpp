#include <G4PhysicalConstants.hh>
#include <TH1D.h>
#include <TFile.h>
#include <iostream>
#include <string>


namespace my_constants{
        /*The physical constants used in the body 
        this function are defined within the G4 headers file "PhysicalConstans.h".
        CLHEP::h_Planck is given in MeV*ns
        CLHEP::c_light is given in mm/ns */
        double h_Planck = CLHEP::h_Planck * 1e+6 * CLHEP::e_SI * 1e-9;
        double c_light = CLHEP::c_light*1e+6;
    }


std::string get_histogram_name(std::string rootfile_path){
    if(rootfile_path=="./data/LED430L-IvsLambda-TH1D.root"){
        return "LED430L_spectrum";
    }
    else if(rootfile_path=="./data/LED430L-IvsAngle-TH1D.root"){
        return "LED430L_angular_pattern";
    }
    else if(rootfile_path=="./data/LED450LW-IvsLambda-TH1D.root"){
        return "LED450LW_spectrum";
    }
    else if(rootfile_path=="./data/LED450LW-IvsAngle-TH1D.root"){
        return "LED450LW_angular_pattern";
    }
    else if(rootfile_path=="./data/LED325W2-IvsLambda-TH1D.root"){
        return "LED325W2_spectrum";
    }
    else if(rootfile_path=="./data/LED325W2-IvsAngle-TH1D.root"){
        return "LED325W2_angular_pattern";
    }
    else{
        std::cout << "There is no existing histogram for the path provided." << std::endl;
        return "-1";
    }
}


TH1D * get_TH1D(std::string rootfile_path){
    TFile * tempFile = TFile::Open(rootfile_path.c_str());
    TH1D * tempHistogram = (TH1D*)tempFile->Get(get_histogram_name(rootfile_path).c_str());
    return tempHistogram;
}


double energy_in_eV(double lambda){
    /* This function gets a photon wavelength, lambda, in nanometers. It returns its 
    associated photon energy in electronvolts. */
    return 1e+9*my_constants::h_Planck*my_constants::c_light/(CLHEP::e_SI*lambda);
}