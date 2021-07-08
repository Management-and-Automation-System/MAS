#ifndef REV_H
#define REV_H

#include "classes.h"
#include <algorithm>
#include <exception>
#include <iomanip>
#include <iostream>
#include <list>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
class Revenue : public Vehicle, public dbms
{
    std ::unordered_map<std ::string, double> m_sales;
    std ::unordered_map<std ::string, double> m_profit;
    double m_gst, m_roadtax, finalCost;

public:
    Revenue(double const& gst = 0.28, double const& roadtax = 0.16);
    void receipt(DataIter const&, int const&);
    void searchCompany(std::string const&);
    void searchModel(std::string const&, DataIterVec const&);
    void searchRange(double const&, double const&);
    void calcRevenue(DataIter const&, long const&); // call the func -> editByQuantity(Dataiter, updated quantity)
    void disp(DataIterVec const&);
};

#endif