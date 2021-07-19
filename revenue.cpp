
#include "revenue.h"
#include "classes.h"
#include <fstream>

using namespace std;

Revenue::Revenue(double const& gst, double const& roadtax)
    : m_gst(gst), m_roadtax(roadtax) {};
std ::unordered_map<std ::string, double> Revenue ::getSales() const
{
    return m_sales;
}
std ::unordered_map<std ::string, double> Revenue ::getProfit() const
{
    return m_profit;
}
double Revenue::getGst() const
{
    return m_gst;
} 
double Revenue::getRoadTax() const 
{
    return m_roadtax;
}
double Revenue ::calcRevenue(DataIter const& it, long const& c_quantity)
{
    if(it->getQuantity() < c_quantity)
        return 0;
    double basePrice = getSalePrice(it);
    double finalCost;
    finalCost += (basePrice) + (basePrice * m_gst) + (basePrice * m_roadtax);
    finalCost *= c_quantity;
    m_sales[it->getCompany()] += finalCost;
    m_profit[it->getCompany()] += (basePrice - it->getCost());
    editByQuantity<0, 1>(it, it->getQuantity() - c_quantity);
    if(it->getQuantity() == 0)
        toDelete<0,1>(it);
    return finalCost;
}
double Revenue::getSalePrice(DataIter const & it) const
{
    return it->getCost() + (it->getCost() * it->getProfitMargin()/100);
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
    if (!dbms::load(is))
    {
        return false;
    }

    m_sales.clear();
    m_profit.clear();
    auto getLine = [&]()
    {
        string temp;
        std::getline(is >> std::ws, temp);
        if (temp.back() == '\r')
            temp.pop_back();
        return temp;
    };
    int sales_size;
    int profit_size;
    decltype(m_sales) sales;
    decltype(m_profit) profit;
    sales_size = stoi(getLine());
    for (int i = 0; i < sales_size; ++i)
    {
        auto first = getLine();
        auto second = stod(getLine());
        sales[first] = second;
    }
    profit_size = stoi(getLine());
    for (int i = 0; i < profit_size; ++i)
    {
        auto first = getLine();
        auto second = stod(getLine());
        profit[first] = second;
    }
    m_sales = sales;
    m_profit = profit;
    return true;
}

bool Revenue::load(std::string const& filename)
{
    std::ifstream is(filename);
    return load(is);
}
bool Revenue::save(std::string const& filename)
{
    std::ofstream os(filename);
    return save(os);
}
