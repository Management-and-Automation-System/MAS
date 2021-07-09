#include "revenue.h"
#include "classes.h"
#include <fstream>
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

bool Revenue::save(std::ostream& os)
{
    dbms::save(os);
    os << m_sales.size() << '\n';
    for (auto const iter : m_sales)
    {
        os << iter.first << '\n'
           << iter.second << '\n';
    }
    os << m_profit.size() << '\n';
    for (auto const iter : m_profit)
    {
        os << iter.first << '\n'
           << iter.second << '\n';
    }
    return true;
}

bool Revenue::load(std::istream& is)
{
    m_profit.clear();
    m_sales.clear();
    if(!dbms::load(is))
        return false; 
    m_sales.clear();
    m_profit.clear();
    auto getline = [&]()
    {
        string temp;
        std::getline(is, temp);
        return temp;
    };
    int sales_size;
    int profit_size;
    decltype(m_sales) sales;
    decltype(m_profit) profit;
    sales_size = stoi(getline());
    profit_size = stoi(getline());
    for(int i = 0; i < sales_size; ++i)
    {
        auto first = getline();
        auto second = stod(getline());
        sales[first] = second;
    }
    for(int i = 0; i < profit_size; ++i)
    {
        auto first = getline();
        auto second = stod(getline());
        profit[first] = second;
    }
    m_sales = sales;
    m_profit = profit;
    return true;
}

bool Revenue::load(std::string const & filename)
{
    std::ifstream is(filename);
    return load(is);
}
bool Revenue::save(std::string const & filename)
{
    std::ofstream os(filename);
    return save(os);
}
