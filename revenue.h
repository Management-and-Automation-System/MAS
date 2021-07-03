#include <algorithm>
#include <exception>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <list>
#include <vector>
#include <unordered_map>
#include "classes.h"
class Revenue : public Vehicle, public dbms
{
    std ::unordered_map<std ::string, long> sales;
    std ::unordered_map<std ::string, long> profit;

public:
    Revenue();
    void searchCompany(std::string const &);
    void searchModel(std::string const &, DataIterVec const &);
    void searchRange(float const &, float const &);
    void calcRevenue(std::string const &, long, DataIter const &); // call the func -> editByQuantity(Dataiter, updated quantity)
    void disp(DataIterVec const &);
};
