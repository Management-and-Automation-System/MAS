#ifndef REV_H
#define REV_H

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
    std ::unordered_map<std ::string, double> m_sales; 
    std ::unordered_map<std ::string, double> m_profit;
    // Why not add these as members?
    double m_gst , m_roadtax;
    // std::unordered_map<std::string, std::pair<double , double>> m_locTax; //We could do this for different locations or are these taxes same all over India?

public:
    Revenue(double const & gst = 0.28 , double const & roadtax = 0.16);
    void searchCompany(std::string const &);
    void searchModel(std::string const &, DataIterVec const &);
    void searchRange(double const &, double const &);
    void calcRevenue(DataIter const & , long const &); // call the func -> editByQuantity(Dataiter, updated quantity)
    void disp(DataIterVec const &);
};

#endif