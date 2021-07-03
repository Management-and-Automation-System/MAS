#include "classes.h"
#include "revenue.h"
#include <map>
#include <string>
#include <vector>
using namespace std;
dbms data;
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
int main()
{

    cout << "1 -> DATABASE MANAGEMENT SECTION\n";
    cout << "2 -> REVENUE SECTION\n";
    int section;
    cin >> section;
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
                data.search(obj);
            }
            break;
            }
        }
        }
    }
    }
}