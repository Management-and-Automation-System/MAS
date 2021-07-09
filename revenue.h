#ifndef REV_H
#define REV_H

#include "classes.h"
#include <list>
#include <unordered_map>
class Revenue : public Vehicle, public dbms
{
    std ::unordered_map<std ::string, double> m_sales;
    std ::unordered_map<std ::string, double> m_profit;
    double m_gst, m_roadtax, finalCost;

public:
    Revenue(double const &gst = 0.28, double const &roadtax = 0.16);
    std ::unordered_map<std ::string, double> getSales() const;
    std ::unordered_map<std ::string, double> getProfit() const;
    void receipt(DataIter const &, int const &);
    void searchCompany(std::string const &);
    void searchModel(std::string const &, DataIterVec const &);
    void searchRange(double const &, double const &);
    void calcRevenue(DataIter const &, long const &); // call the func -> editByQuantity(Dataiter, updated quantity)
    void dispRev(std ::unordered_map<std ::string, double> const &, std ::unordered_map<std ::string, double> const &);
    bool load(std::string const &) override;
    bool save(std::string const &) override;
    bool load(std::istream &) override;
    bool save(std::ostream &) override;
    ~Revenue() = default;
};

#endif