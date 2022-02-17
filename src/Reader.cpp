#include "./Reader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <globals.hh>

Reader * Reader::instance = nullptr;

Reader * Reader::getInstance(){
  if(!instance){
    instance = new Reader();
  }
  return instance;
}

Reader::Reader()
{
}

Reader::~Reader()
{
  if(instance){
    delete instance;
  }
}

int Reader::HowManyLines(std::string& filepath){
  std::ifstream in_file;
  std::string strHolder;
  int counter = 0;

  in_file.open(filepath, std::ios_base::in);
  std::getline(in_file, strHolder);
  while(in_file.eof()==false){
    counter++;
    std::getline(in_file, strHolder);
  }
  counter++;//The last line sets in_file.eof() to true, but still counts as a line.
  in_file.close();

  return counter;
}

int Reader::ReadTwoColumnsCsv(std::string & filepath, std::vector<double> & column_1, std::vector<double> & column_2, double c1_units, double c2_units){
  
  int len = HowManyLines(filepath);
  int counter = 0;

  if(column_1.size()!=0 || column_2.size()!=0){
    G4Exception("Reader::ReadTwoColumnsCsv", "0", JustWarning,
    "Description: The provided containers must be emtpy. Containers will remain unchanged.");
    return 0;
  }

  std::ifstream in_file;
  std::string strHolder;
  std::istringstream iss;
  int commapos, row_len;

  in_file.open(filepath, std::ios_base::in);
  for(int i=0; i<len; i++){
    std::getline(in_file, strHolder);
    iss.str(strHolder);
    commapos = strHolder.find(",");
    if(commapos==-1){//No comma within this row. This row is ill-formed and must be skipped. The counter is not incremented.
      continue;
    }
    row_len = strHolder.length();
    char * charHolder = new char[commapos+1];
    iss.read(charHolder, commapos);
    charHolder[commapos]='\0';
    try{
      column_1.push_back(std::stold(charHolder)*c1_units);
    }
    catch(std::invalid_argument){
      delete[] charHolder;
      continue; //If the first column entry is not available, we are not interested in its matching entry from the second column anymore.
    }
    iss.seekg(commapos+1, std::ios_base::beg);
    delete[] charHolder;
    charHolder = new char[row_len-commapos-1];
    iss.read(charHolder, row_len-commapos-1);
    charHolder[row_len-commapos-1] = '\0';
    try{
      column_2.push_back(std::stold(charHolder)*c2_units);
    }
    catch(std::invalid_argument){
      column_1.pop_back(); //If we have reached here, the corresponding first column entry has already been stored in column_1. We are not interested in keeping it if its matching second column entry is not available.
      delete[] charHolder;
      continue;
    }
    delete[] charHolder;
    counter++;
  }
  in_file.close();
  
  if(len>counter){
    G4String ed = "Description: A total of ";
    ed += std::to_string(len-counter);
    ed += " ill-formed rows were not read. Every row must match the <double>,<double> format.";
    G4Exception("Reader::ReadTwoColumnsCsv()", "0", JustWarning, ed);
  }

  return counter;
}

void Reader::DisplayReadCsv(std::vector<double> column_1, std::vector<double> column_2){
  if(column_1.size()!=column_2.size()){
    G4Exception("Reader::DisplayReadCsv", "0", JustWarning, 
    "Description: The length of the provided columns do not match.");
    return;
  }
  std::cout << "--------CSV DISPLAY--------" << std::endl;
  for(int i=0; i<column_1.size(); i++){
    std::cout << column_1[i] << ", " << column_2[i] << std::endl;
  }
  std::cout << "---------------------------" << std::endl;
  return;
}