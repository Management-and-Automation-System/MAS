#include "revenue.h"
#include "classes.h"
using namespace std;

Revenue ::Revenue(){};
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
void Revenue ::calcRevenue(std::string const &model, long c_quantity, DataIter const &it)
{
    float finalCost = 0;
    float basePrice = it->getCost();
    float GST = 0.28, roadTax = 0.16, profitPercent = it->getProfitMargin();
    finalCost += (basePrice) + (basePrice * GST) + (basePrice * roadTax) + (basePrice * profitPercent);
    sales[it->getCompany()] += finalCost;
    profit[it->getCompany()] += (basePrice * profitPercent);
    cout << setw(15) << "GST" << setw(15) << "ROAD TAX" << setw(15) << "OTHER TAXES" << setw(15) << "FINAL COST\n";
    cout << setw(15) << GST << setw(15) << roadTax << setw(15) << profitPercent << setw(15) << finalCost << "\n";
    editByQuantity(it, it->getQuantity() - c_quantity);
}
void Revenue ::disp(DataIterVec const &obj)
{
    cout << setw(15) << "COMPANY" << setw(15) << "MODEL" << setw(15) << "QUANTITY" << setw(15) << "QUANTITY\n";

    for (auto const &it : obj)
    {
        cout << setw(15) << it->getCompany() << setw(15) << it->getModelName() << setw(15) << it->getQuantity();
    }
}
