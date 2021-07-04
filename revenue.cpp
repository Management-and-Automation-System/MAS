#include "revenue.h"
#include "classes.h"
using namespace std;

Revenue::Revenue(double const& gst, double const& roadtax)
    : m_gst(gst), m_roadtax(roadtax) {};
/*
void Revenue ::searchCompany(std::string const &company)
{
    DataIterVec res = searchByCompany(company);
    if (!res.empty())
    {
        disp(res);
        cout << "ENTER THE MODEL YOU WANT TO BUY\n";
        std::string model;
        cin >> model;
        searchModel(model, res);
    }
    else
    {
        cout << "NO COMPANY FOUND\n";
    }
}
void Revenue ::searchModel(std::string const &model, DataIterVec const &l_companies)
{
    bool found = false;
    for (auto const &it : l_companies)
    {
        if (it->getModelName() == model)
        {
            found = true;
            cout << setw(15) << "QUANTITY AVAILABLE" << setw(15) << "COST\n";
            cout << setw(15) << it->getQuantity() << setw(15) << it->getCost() << "\n";
            cout << "ENTER THE QUANTITY\n";
            long c_quantity;
            cin >> c_quantity;
            if (c_quantity <= it->getQuantity())
            {
                calcRevenue(model, c_quantity, it);
            }
            else
            {
                cout << "ENTERED QUANTITY NOT AVAILABLE\n";
            }
            break;
        }
    }
    if (!found)
        cout << "MODEL NOT FOUND\n";
}
void Revenue ::searchRange(float const &lb, float const &ub)
{
    DataIterVec res = searchByRange(lb, ub);
    if (!res.empty())
    {
        disp(res);
        cout << "ENTER THE MODEL YOU WANT TO BUY\n";
        std::string model;
        cin >> model;
        searchModel(model, res);
    }
    else
    {
        cout << "NO MODEL FOUND\n";
    }
}
*/
void Revenue ::calcRevenue(DataIter const& it, long const& c_quantity)
{
    if (it->getQuantity() >= c_quantity)
    {
        double finalCost = 0;
        double basePrice = it->getCost();
        double profitPercent = it->getProfitMargin();
        profitPercent /= 100;
        finalCost += (basePrice) + (basePrice * m_gst) + (basePrice * m_roadtax) + (basePrice * profitPercent);
        m_sales[it->getCompany()] += finalCost;
        m_profit[it->getCompany()] += (basePrice * profitPercent);
        /*
    cout << setw(15) << "GST" << setw(15) << "ROAD TAX" << setw(15) << "OTHER TAXES" << setw(15) << "FINAL COST\n";
    cout << setw(15) << m_gst << setw(15) << m_roadtax << setw(15) << profitPercent << setw(15) << finalCost << "\n";
    */
        editByQuantity(it, it->getQuantity() - c_quantity);
        receipt(it, basePrice * m_gst, basePrice * m_roadtax, basePrice * profitPercent, finalCost);
    }
    else
    {
        cout << "REQUIRED QUANTITY NOT AVAILABLE\n";
    }
}
void Revenue ::receipt(DataIter const& it, double const& f_gst, double const& f_roadTax, double const& f_otherTax, double const& finalCost)
{
    cout << setw(10) << "COMPANY" << setw(15) << it->getCompany() << '\n';
    cout << setw(10) << "MODEL" << setw(15) << it->getModelName() << '\n';
    cout << setw(10) << "BASE PRICE" << setw(15) << it->getCost() << '\n';
    cout << setw(10) << "GST" << setw(15) << f_gst << '\n';
    cout << setw(10) << "ROAD TAX" << setw(15) << f_roadTax << '\n';
    cout << setw(10) << "OTHER TAXES" << setw(15) << f_otherTax << '\n';
    cout << setw(10) << "FINAL COST" << setw(15) << finalCost << '\n';
}
void Revenue ::disp(DataIterVec const& obj)
{
    cout << setw(15) << "COMPANY" << setw(15) << "MODEL" << setw(15) << "QUANTITY" << setw(15) << "QUANTITY\n";

    for (auto const& it : obj)
    {
        cout << setw(15) << it->getCompany() << setw(15) << it->getModelName() << setw(15) << it->getQuantity();
    }
}
