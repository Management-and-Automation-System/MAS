#include "revenue.h"
#include "classes.h"
using namespace std;

Revenue::Revenue(double const& gst, double const& roadtax)
    : m_gst(gst), m_roadtax(roadtax) {};

void Revenue ::calcRevenue(DataIter const& it, long const& c_quantity)
{
    double basePrice = it->getCost();
    double profitPercent = it->getProfitMargin();
    profitPercent /= 100;
    finalCost += (basePrice) + (basePrice * m_gst) + (basePrice * m_roadtax) + (basePrice * profitPercent);
    finalCost *= c_quantity;
    m_sales[it->getCompany()] += finalCost;
    m_profit[it->getCompany()] += (basePrice * profitPercent);
    editByQuantity(it, it->getQuantity() - c_quantity, 0, 1);
}
