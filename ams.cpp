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
    int i = 1;
    if (!n_obj.empty())
    {
        for (auto const& obj : n_obj)
        {
            cout << setw(15) << "COMPANY" << setw(15) << "MODEL" << setw(15) << "QUANTITY" << setw(15) << "ATTRIBUTES\n";
            cout << i++ << setw(15) << obj->getCompany() << setw(15) << obj->getModelName() << setw(15) << obj->getQuantity();
            int flag = 0;
            for (auto& x : obj->getAttributes())
            {
                if (!flag)
                {
                    cout << setw(10) << x;
                    flag = 1;
                }
                else
                {
                    cout << setw(10) << x;
                }
            }
            cout << '\n';
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
    cout << "Command: close" << '\n';
    cout << "   Exit the program" << '\n';
}
}

namespace dbm
{
DataIterVec n_obj;
void help_1()
{
    cout << "1 -> SEARCH WITH FULL SPECIFICATIONS\n";
    cout << "2 -> SEARCH BY COMPANY\n";
    cout << "3 -> SEARCH BY MODEL NAME\n";
    cout << "4 -> SEARCH BY ATTRIBUTES\n";
    cout << "5 -> SEARCH BY QUANTITY\n";
    cout << "6 -> SEARCH BY RANGE(IN RUPPESS)\n";
}
std::pair<DataIter, bool> search(int prompt_flg = 0, string const& prompt_msg = "Please Choose a Serial Number: ")
{
    DataIterVec n_obj;
    if (prompt_flg == 0)
    {
        dbm::help_1();
        int choice;
        cin >> choice;
        switch (choice)
        {
        case 1:
        {

            Vehicle obj = createObject();
            n_obj = db.search(obj);
            display(n_obj);
        }
        break;
        case 2:
        {
            string company;
            cout << "ENTER COMPANY NAME\n";
            cin >> company;
            n_obj = db.searchByCompany(company);
            display(n_obj);
        }
        break;
        case 3:
        {

            string model;
            cout << "ENTER MODEL NAME\n";
            cin >> model;
            n_obj = db.searchByModel(model);
            display(n_obj);
        }
        break;
        case 4:
        {

            vector<string> attr;
            cout << "ENTER ATTRIBUTES\n";
            for (int i = 0; i < 3; i++)
            {
                string s;
                cin >> s;
                attr.push_back(s);
            }
            n_obj = db.searchByAttribute(attr);
            display(n_obj);
        }
        break;
        case 5:
        {

            long quantity;
            cout << "ENTER QUANTITY\n";
            cin >> quantity;
            n_obj = db.searchByQuantity(quantity);
            display(n_obj);
        }
        break;
        case 6:
        {

            double lb, ub;
            cout << "ENTER LOWER BOUND AND UPPER BOUND OF RANGE\n";
            cin >> lb >> ub;
            n_obj = db.searchByRange(lb, ub);
            display(n_obj);
        }
        break;
        }
    }
    else if (prompt_flg == 1)
    {
        int choice;
        cin >> choice;
        DataIterVec n_obj;
        switch (choice)
        {
        case 1:
        {
            string company;
            cout << "ENTER COMPANY NAME\n";
            cin >> company;
            n_obj = db.searchByCompany(company);
            display(n_obj);
        }
        break;
        case 2:
        {
            string model;
            cout << "ENTER MODEL NAME\n";
            cin >> model;
            n_obj = db.searchByModel(model);
            display(n_obj);
        }
        break;
        case 3:
        {
            double lb, ub;
            cout << "ENTER LOWER BOUND AND UPPER BOUND OF RANGE\n";
            cin >> lb >> ub;
            n_obj = db.searchByRange(lb, ub);
            display(n_obj);
        }
        break;
        }
        int serial_num;
        cout << prompt_msg;
        cin >> serial_num;

        return { n_obj[serial_num - 1], 1 };
    }

    return { DataIter(), 0 };
}
void edit()
{
    auto it = dbm::search(1, "Enter Serial No.");
    cout << "Enter the new Value for Company: ";
    string comp;
    cin >> comp;
    if (it.second)
        db.editByCompany(it.first, comp);
}
void ins()
{
    Vehicle obj = createObject();
    db.insert(obj);
}
void del()
{
}
void disp()
{
    DataIterVec res = db.extractAll();
    if (!res.empty())
    {
        int i = 1;
        for (auto const& obj : res)
        {

            cout << setw(15) << "COMPANY" << setw(15) << "MODEL" << setw(15) << "QUANTITY" << setw(15) << "ATTRIBUTES\n";
            cout << i++ << setw(15) << obj->getCompany() << setw(15) << obj->getModelName() << setw(15) << obj->getQuantity();
            int flag = 0;
            for (auto& x : obj->getAttributes())
            {
                if (!flag)
                {
                    cout << setw(10) << x;
                    flag = 1;
                }
                else
                {
                    cout << setw(10) << x;
                }
            }
            cout << '\n';
        }
    }
    else
    {
        cout << "NO RECORD IN THE DATABASE TO BE DISPLAYED \n";
    }
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
    cout << "Command: disp" << '\n';
    cout << "   To display the entire database\n";
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
void help()
{
    cout << "Command: sell" << '\n';
    cout << "   Sell the vehicle(s)\n";
    cout << "Command: receipt" << '\n';
    cout << "   Show receipt\n";
    cout << "Command: up" << '\n';
    cout << "   Go up a menu\n";
    cout << "Command: clear" << '\n';
    cout << "   Clear the screen" << '\n';
    cout << "Command: help" << '\n';
    cout << "   Print this menu" << '\n';
}
void help_1()
{
    cout << "1 -> SEARCH BY COMPANY\n";
    cout << "2 -> SEARCH BY MODEL\n";
    cout << "3 -> SEARCH BY RANGE\n";
}
void receipt()
{
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
        receipt,
        up,
        help
    };
    std::map<string, int> revCases {
        { "sell", sell },
        { "clear", clear },
        { "receipt", receipt },
        { "up", up },
        { "help", help }
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
            rev::help_1();
            rev::sell();
            break;
        }
        case clear:
        {
            clrscr();
            break;
        }
        case receipt:
        {
            rev::receipt();
            break;
        }
        case up:
        {
            active = 0;
            break;
        }
        case help:
        {
            rev::help();
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
        up,
        close
    };
    std::map<string, int> mainCases {
        { "help", help },
        { "dsys", dbms },
        { "rsys", revenue },
        { "clear", clear },
        { "up", up },
        { "close", close }
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
        case close:
        {
            exit(0);
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
        disp,
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
        { "disp", disp },
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
        case disp:
        {
            dbm::disp();
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