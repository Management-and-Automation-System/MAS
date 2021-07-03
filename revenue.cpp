#include "revenue.h"
#include "classes.h"
using namespace std;

Revenue::Revenue(double const &gst , double const &roadtax)
    : m_gst(gst) , m_roadtax(roadtax) {};
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
void Revenue ::searchRange(double const &lb, double const &ub)
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
void Revenue ::calcRevenue(DataIter const &it , long const &c_quantity)
{
    double finalCost = 0;
    double basePrice = it->getCost();
    double profitPercent = it->getProfitMargin();
    profitPercent /= 100;
    finalCost += (basePrice) + (basePrice * m_gst) + (basePrice * m_roadtax) + (basePrice * profitPercent);
    m_sales[it->getCompany()] += finalCost;
    m_profit[it->getCompany()] += (basePrice * profitPercent);
    editByQuantity(it, it->getQuantity() - c_quantity, 0 , 1);
}
