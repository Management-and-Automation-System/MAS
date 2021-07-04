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
    std ::unordered_map<std ::string, long> m_sales;
    std ::unordered_map<std ::string, long> m_profit;
    // Why not add these as members?
    double m_gst, m_roadtax;
    // std::unordered_map<std::string, std::pair<double , double>> m_locTax; //We could do this for different locations or are these taxes same all over India?

public:
    Revenue(double const& gst = 0.28, double const& roadtax = 0.16);
    void calcRevenue(DataIter const&, long const&); // call the func -> editByQuantity(Dataiter, updated quantity)
    void receipt(DataIter const&, double const&, double const&, double const&, double const&);
    void disp(DataIterVec const&);
};

#endif