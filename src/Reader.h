#include <string>
#include <vector>

class Reader
{
  private:
    Reader();
    ~Reader();

    Reader(const Reader&) = delete; //Delete the copy constructor
    Reader& operator=(const Reader&) = delete; //Delete the copy assignment

    int HowManyLines(std::string &);
    
    static Reader * instance;
  public:
    int ReadTwoColumnsCsv(std::string &, std::vector<double> &, std::vector<double> &, double, double); //Returns the number of rows that were successfully read.
    void DisplayReadCsv(std::vector<double>, std::vector<double>); //Just in case you want to check a proper readout

    static Reader * getInstance();
};
