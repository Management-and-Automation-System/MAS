#include "classes.cpp"
#include "classes.h"
#include "revenue.cpp"
#include "revenue.h"
#include <map>
#include <string>
#include <vector>

using namespace std;
Revenue db;
using DataIter = dbms::DataIter;
using DataIterVec = dbms::DataIterVec;

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
void display(DataIterVec const& n_obj)
{
    if (!n_obj.empty())
    {
        for (auto const& obj : n_obj)
        {
            cout << setw(15) << "COMPANY" << setw(15) << "MODEL" << setw(15) << "QUANTITY" << setw(15) << "QUANTITY\n";
            cout << setw(15) << obj->getCompany() << setw(15) << obj->getModelName() << setw(15) << obj->getQuantity();
            int flag = 0;
            for (auto& x : obj->getAttributes())
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
void clrscr()
{
    cout << "\033[2J\033[1;1H";
}

namespace mainm
{
void help()
{

    cout << "Command: dsys" << '\n';
    cout << "   Open the Database Section" << '\n';
    cout << "Command: rsys" << '\n';
    cout << "   Open the Revenue Section" << '\n';
    cout << "Command: help" << '\n';
    cout << "   Print this menu" << '\n';
    cout << "Command: clear" << '\n';
    cout << "   Clear the screen" << '\n';
}
}

namespace dbm
{
std::pair<DataIter, bool> search(int prompt_flg = 0, string const& prompt_msg = "Please Choose a Serial Number: ")
{
    cout << "1 -> SEARCH WITH FULL SPECIFICATIONS\n";
    cout << "2 -> SEARCH BY COMPANY\n";
    cout << "3 -> SEARCH BY MODEL NAME\n";
    cout << "4 -> SEARCH BY ATTRIBUTES\n";
    cout << "5 -> SEARCH BY QUANTITY\n";
    cout << "6 -> SEARCH BY RANGE(IN RUPPESS)\n";
    int choice;
    cin >> choice;
    DataIterVec n_obj;
    switch (choice)
    {
    case 1:
    {
        Vehicle obj = createObject();
        n_obj = db.search(obj);
    }
    break;
    case 2:
    {
        string company;
        cout << "ENTER COMPANY NAME\n";
        cin >> company;
        n_obj = db.searchByCompany(company);
    }
    break;
    }
    display(n_obj);
    if (!n_obj.empty())
    {
        if (prompt_flg == 1)
        {
            int sr;
            cout << prompt_msg;
            cin >> sr;
            return { n_obj[sr - 1], 1 };
        }
        return { n_obj[0], 1 };
    }
    return { DataIter(), 0 };
}
void edit()
{
    auto it = dbm::search(1 , "Enter Serial No.");
    cout << "Enter the new Value for Company: ";
    string comp;
    cin >> comp;
    if(it.second)
        db.editByCompany(it.first , comp);
}
void ins()
{
    Vehicle obj = createObject();
    db.insert(obj);
}
void del()
{
}
void help()
{
    cout << "Command: insert" << '\n';
    cout << "   Insert Vehicle Data\n";
    cout << "Command: search" << '\n';
    cout << "   Search Vehicle Data\n";
    cout << "Command: edit" << '\n';
    cout << "   Edit Vehicle Data\n";
    cout << "Command: del" << '\n';
    cout << "   Delete Vehicle Data\n";
    cout << "Command: up" << '\n';
    cout << "   Go up a menu\n";
    cout << "Command: help" << '\n';
    cout << "   Print this menu" << '\n';
    cout << "Command: clear" << '\n';
    cout << "   Clear the screen" << '\n';
}
}

namespace rev
{
void sell() //Check This Out!
{
    auto it = dbm::search(1, "Which Vehicle Would You Like to buy(Serial Number): ");
    if (it.second)
    {
        int quantity;
        cout << "Please Enter Quantity: ";
        cin >> quantity;
        db.calcRevenue(it.first, quantity);
    }
}
}

void dbms_menu();
void rev_menu()
{
    bool active = 1;
    enum
    {
        sell,
        clear,
        up
    };
    std::map<string, int> revCases {
        { "sell", sell },
        { "clear", clear },
        { "up", up }
    };
    while (active)
    {
        string choice;
        cout << "revenue> ";
        cin >> choice;
        switch (revCases.at(choice))
        {
        case sell:
        {
            rev::sell();
            break;
        }
        case up:
        {
            active = 0;
            break;
        }
        case clear:
        {
            clrscr();
            break;
        }
        }
    }
}
void main_menu()
{
    bool active = 1;
    enum
    {
        dbms = 1,
        revenue,
        help,
        clear,
        up
    };
    std::map<string, int> mainCases {
        { "help", help },
        { "dsys", dbms },
        { "rsys", revenue },
        { "clear", clear },
        { "up", up }
    };
    while (active)
    {
        string choice;
        cout << "ams> ";
        cin >> choice;
        switch (mainCases.at(choice))
        {
        case dbms:
        {
            dbms_menu();
            break;
        }
        case revenue:
        {
            rev_menu();
            break;
        }
        case help:
        {
            mainm::help();
            break;
        }
        case clear:
        {
            clrscr();
            break;
        }
        case up:
        {
            active = 0;
            break;
        }
        }
    }
}

void dbms_menu()
{
    bool active = 1;
    enum
    {
        insert = 1,
        search,
        edit,
        rem,
        up,
        help,
        clear
    };
    std::map<string, int> dbmsCases {
        { "help", help },
        { "insert", insert },
        { "search", search },
        { "edit", edit },
        { "remove", rem },
        { "up", up },
        { "clear", clear }
    };
    dbm::help();
    while (active)
    {
        std::string choice;
        cout << "dbms> ";
        cin >> choice;
        switch (dbmsCases.at(choice))
        {
        case help:
        {
            dbm::help();
            break;
        }
        case insert:
        {
            dbm::ins();
            break;
        }
        case search:
        {
            dbm::search(); //This means global namespace, we need this or the compiler gets confused between search in the enum and search the function
            break;
        }
        case edit:
        {
            dbm::edit();
            break;
        }
        case rem:
        {
            dbm::del();
            break;
        }
        case up:
        {
            active = 0;
            break;
        }
        case clear:
        {
            clrscr();
            break;
        }
        }
    }
}

int main()
{
    while (1)
    {
        try
        {
            main_menu();
        }
        catch (...)
        {
        };
    }
}