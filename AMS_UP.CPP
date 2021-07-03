#include "classes.h"
#include "revenue.h"
#include "classes.cpp"
#include <map>
#include <string>
#include <vector>
using namespace std;
dbms data;
using m_DataIter = std::list<Vehicle>::iterator;
using m_DataIterVec = std::vector<m_DataIter>;
Vehicle createObject()
{
    cout << "ENTER COMPANY NAME\n";
    string company;
    cin >> company;
    cout << "ENTER MODEL NAME\n";
    string model;
    cin >> model;
    cout << "ENTER MODEL ATTRIBUTES(color, wheel size, milage)\n";
    vector<string> attr;
    for (int i = 0; i < 3; i++)
    {
        string s;
        cin >> s;
        attr.push_back(s);
    }
    cout << "ENTER QUANTITY OF THE VEHICLE\n";
    long quantity;
    cin >> quantity;
    cout << "ENTER COST OF THE VEHICLE\n";
    float cost;
    cin >> cost;
    cout << "ENTER PROFIT MARGIN\n";
    float prof;
    cin >> prof;
    return Vehicle(company, model, attr, quantity, cost, prof);
}
void display(m_DataIterVec const &n_obj)
{
    if (!n_obj.empty())
    {
        for (auto const &obj : n_obj)
        {
            cout << setw(15) << "COMPANY" << setw(15) << "MODEL" << setw(15) << "QUANTITY" << setw(15) << "QUANTITY\n";
            cout << setw(15) << obj->getCompany() << setw(15) << obj->getModelName() << setw(15) << obj->getQuantity();
            int flag = 0;
            for (auto &x : obj->getAttributes())
            {
                if (!flag)
                {
                    cout << setw(15) << x << '\n';
                    flag = 1;
                }
                else
                {
                    cout << setw(60) << x << '\n';
                }
            }
        }
    }
    else
    {
        cout << "NO RECORD IN THE DATABASE TO BE DISPLAYED WITH GIVEN SPECIFICATIONS\n";
    }
}
int main()
{

    cout << "1 -> DATABASE MANAGEMENT SECTION\n";
    cout << "2 -> REVENUE SECTION\n";
    int section;
    cin >> section;
    char ch = 'Y';
    while (ch == 'Y')
    {
        switch (section)
        {
        case 1:
        {
            dbms data;
            cout << "1 -> INSERT VEHICLE DATA\n";
            cout << "2 -> SEARCH VEHICLE DATA\n";
            cout << "3 -> EDIT VEHICLE DATA\n";
            cout << "4 -> DELETE VEHICLE DATA\n";
            int choice;
            cin >> choice;
            switch (choice)
            {
            case 1:
            {
                Vehicle obj = createObject();
                data.insert(obj);
            }
            break;
            case 2:
            {
                cout << "1 -> SEARCH WITH FULL SPECIFICATIONS\n";
                cout << "2 -> SEARCH BY COMPANY\n";
                cout << "3 -> SEARCH BY MODEL NAME\n";
                cout << "4 -> SEARCH BY ATTRIBUTES\n";
                cout << "5 -> SEARCH BY QUANTITY\n";
                cout << "6 -> SEARCH BY RANGE(IN RUPPESS)\n";
                int choice;
                cin >> choice;
                switch (choice)
                {
                case 1:
                {
                    Vehicle obj = createObject();
                    m_DataIterVec n_obj = data.search(obj);
                    display(n_obj);
                }
                break;
                case 2:
                {
                    cout << "ENTER COMPANY NAME\n";
                    string company;
                    cin >> company;
                    m_DataIterVec n_obj = data.searchByCompany(company);
                    display(n_obj);
                }
                break;
                }
            }
            }
        }
        }
        cout << "WANT TO CONTINUE?(Y/N)?\n";
        cin >> ch;
    }
}