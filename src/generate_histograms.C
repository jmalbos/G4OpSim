#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <TH1D.h>


int read_two_columns_csv_double(std::vector<double> & firstcol, std::vector<double> & secondcol, std::string filepath, bool verbose)
{
    int cont = 0, commapos, endpos;
    double holder1, holder2;
    char * charHolder;
    std::ifstream in_file;
    std::size_t pos = 0;
    std::string strHolder1;
    std::istringstream iss;
    in_file.open(filepath, std::ios_base::in);
    std::getline(in_file, strHolder1);
    do{    
        iss.str(strHolder1);
        commapos = strHolder1.find(",", pos);
        endpos = strHolder1.length();
        charHolder = new char[commapos+1];
        iss.read(charHolder, commapos);
        charHolder[commapos]='\0';
        std::string strHolder = charHolder;
        holder1 = std::atof(strHolder.c_str());
        iss.seekg(commapos+1, std::ios_base::beg);
        delete[] charHolder;
        charHolder = new char[endpos-commapos];
        iss.read(charHolder, endpos-commapos-1);
        charHolder[endpos-commapos-1]='\0'; 
        holder2 = std::atof(charHolder);
        firstcol.push_back(holder1);
        secondcol.push_back(holder2);
        //std::cout << std::setprecision (16) << holder1 << "   " << holder2 << std::endl;
        std::getline(in_file, strHolder1);
        cont++;
    }while(in_file.eof()==false);
    in_file.close();
    return cont;
}


void stdvector_to_array(std::vector<double> my_vector, double myarray[], int length)
{
    for(int i=0; i<length; i++){
        myarray[i] = my_vector[i];
    }
    return;
}


double minimum(double myarray[], int length)
{
    double minimum = myarray[0];
    for(int i=1; i<length; i++)
    {
        if(myarray[i]<minimum){minimum = myarray[i];}
    }
    return minimum;
}


double maximum(double myarray[], int length)
{
    double maximum = myarray[0];
    for(int i=1; i<length; i++)
    {
        if(myarray[i]>maximum){maximum = myarray[i];}
    }
    return maximum;
}


void cancel_negative_entries(double myarray[], int length)
{
    for(int i=0; i<length; i++){
        if(myarray[i]<0){myarray[i]=0;}
    }
    return;
}




void generate_histograms(void){
//----------------Parameters input----------------//
    int howManyHistograms = 6;
    bool readout_verbose = false;
    std::string paths_to_csv[]={
        "../data/LED430L-IvsAngle.csv",
        "../data/LED430L-IvsLambda.csv",
        "../data/LED450LW-IvsAngle.csv",    
        "../data/LED450LW-IvsLambda.csv",
        "../data/LED325W2-IvsAngle.csv",    
        "../data/LED325W2-IvsLambda.csv"
    };

    std::string h_names[]={
        "LED430L_angular_pattern",
        "LED430L_spectrum",
        "LED450LW_angular_pattern",
        "LED450LW_spectrum",
        "LED325W2_angular_pattern",
        "LED325W2_spectrum"
    };

    std::string h_titles[]={
        "LED430L Angular pattern",
        "LED430L Spectrum",
        "LED450LW Angular pattern",
        "LED450LW Spectrum",
        "LED325W2 Angular pattern",
        "LED325W2 Spectrum"     
    };

    std::string output_filepaths[]={
        "../data/LED430L-IvsAngle-TH1D.root",
        "../data/LED430L-IvsLambda-TH1D.root",
        "../data/LED450LW-IvsAngle-TH1D.root",    
        "../data/LED450LW-IvsLambda-TH1D.root",     
        "../data/LED325W2-IvsAngle-TH1D.root",    
        "../data/LED325W2-IvsLambda-TH1D.root"   
    };
//------------------------------------------------//

    std::vector<double> firstcol, secondcol;
    //TH1D * pHistogram;
    double * x_values, * y_values;
    for(int i=0; i<howManyHistograms; i++)
    {   
        int how_many_rows = read_two_columns_csv_double(firstcol, secondcol, paths_to_csv[i], readout_verbose);
        int length = firstcol.size();
        if(how_many_rows!=length){ std::cout << "WARNING1" << std::endl; return -1;}
        x_values = new double[length];
        y_values = new double[length];
        stdvector_to_array(firstcol, x_values, length);
        stdvector_to_array(secondcol, y_values, length);
        firstcol.clear();
        secondcol.clear();
        //WARNING: Check whether the following line makes sense now!
        cancel_negative_entries(y_values, how_many_rows);

        TH1D * pHistogram = new TH1D(h_names[i].c_str(), h_titles[i].c_str(), 
                            how_many_rows, minimum(x_values, length), maximum(x_values, length));
        
        for(int j=0; j<length; j++){
            pHistogram->SetBinContent(j, y_values[j]);
        }

        TFile * tempFile = new TFile(output_filepaths[i].c_str(), "NEW");
        pHistogram->Write();
        tempFile->Close();
        delete pHistogram;
        delete x_values;
        delete y_values;
    }
    return;

}