#include "rang.h"
#include "classes.h"
#include "revenue.h"
#include <algorithm>
#include <csignal>
#include <iomanip>
#include <iostream>
#include <map>
#include <ostream>
using namespace std;
Revenue db;
using DataIter = dbms::DataIter;
using DataIterVec = dbms::DataIterVec;
void signalHandler(int);
void flushCin()
{
}
string getLine()
{
    string result;
    cin >> ws;
    getline(cin, result);
    return result;
}

void clrscr()
{
    //cout << "\033[2J\033[1;1H";
    cout << "\e[H\e[2J\e[3J";
}

Vehicle createObject()
{
    vector<string> attr;
    long quantity;
    double cost, prof;
    cout << "Enter Company: ";
    auto company = getLine();
    cout << "Enter Model: ";
    auto model = getLine();
    cout << "Enter Attributes:\n";
    for (int i = 0; i < ATTR; i++)
    {
        switch (i)
        {
        case 0:
            cout << "\tColor: ";
            break;
        case 1:
            cout << "\tWheel Size: ";
            break;
        case 2:
            cout << "\tMileage: ";
            break;
        }
        auto s = getLine();
        attr.push_back(s);
    }
    cout << "Enter Quantity: ";
    cin >> quantity;
    flushCin();
    cout << "Enter Cost: ";
    cin >> cost;
    flushCin();
    cout << "Enter Profit Margin: ";
    cin >> prof;
    flushCin();
    return Vehicle(company, model, attr, quantity, cost, prof);
}
void display(DataIterVec const &n_obj)
{
    if (!n_obj.empty())
    {
        const int spaceGap = 15;
        auto seperator = [=]()
        {
            for (int i = 0; i < 9; ++i)
            {
                cout << '+';
                for (int j = 0; j < spaceGap + 1; ++j)
                    cout << '-';
            }
            cout << "+\n";
        };
        cout << rang::style::bold;
        seperator();
        cout << right << "| "
             << setw(spaceGap) << left << "Sr No."
             << right << "| "
             << setw(spaceGap) << left << "Company"
             << right << "| "
             << setw(spaceGap) << left << "Model"
             << right << "| "
             << setw(spaceGap) << left << "Cost(in Rs)"
             << right << "| "
             << setw(spaceGap) << left << "Profit Margin"
             << right << "| "
             << setw(spaceGap) << left << "Instock"
             << right << "| "
             << setw(spaceGap) << left << "Color"
             << right << "| "
             << setw(spaceGap) << left << "Wheel Size"
             << right << "| "
             << setw(spaceGap) << left << "Mileage"
             << right << "| "
             << "\n";
        seperator();
        cout << rang::style::reset;
        int count = 1;
        for (auto const &obj : n_obj)
        {
            cout << right << "| "
                 << setw(spaceGap) << left << count
                 << right << "| "
                 << setw(spaceGap) << left << obj->getCompany()
                 << right << "| "
                 << setw(spaceGap) << left << obj->getModelName()
                 << right << "| "
                 << setw(spaceGap) << left << obj->getCost()
                 << right << "| "
                 << setw(spaceGap) << left << obj->getProfitMargin()
                 << right << "| "
                 << setw(spaceGap) << left << obj->getQuantity()
                 << right << "| ";
            for (auto it : obj->getAttributes())
                cout << setw(spaceGap) << left << it
                     << right << "| ";
            cout << '\n';
            seperator();
            ++count;
        }
    }
    else
    {
        cout << "NO RECORD IN THE DATABASE TO BE DISPLAYED WITH GIVEN SPECIFICATIONS\n";
    }
}
void save()
{
    string filename;
    cout << "Enter the savefile name: ";
    cin >> filename;
    if (db.save(filename))
        cout << "Successfully Saved\n";
    else
        cout << "Save was not sucessful, some error occurred\n";
}
void load()
{
    char ans = 'n';
    if (!db.empty())
    {
        cout << "Would you like to save first?(Y/N): ";
        cin >> ans;
        ans = tolower(ans);
    }
    flushCin();
    if (ans == 'y')
    {
        save();
    }
    else if (ans != 'n')
    {
        cout << "Invalid Input. Returning to previous menu.";
        return;
    }
    string filename;
    cout << "Enter the loadfile name: ";
    filename = getLine();
    if (db.load(filename))
        cout << "Loaded Successfully\n";
    else
        cout << "Load was not successful, some error occured\n";
}
void undo()
{
    db.undo();
}
void redo()
{
    db.redo();
}

namespace mainm
{
    void help()
    {
        using rang::style;
        cout << "Command: " << style::bold << "dsys" << style::reset << '\n';
        cout << "   Open the Database Section" << '\n';
        cout << "Command: " << style::bold << "rsys" << style::reset << '\n';
        cout << "   Open the Revenue Section" << '\n';
        cout << "Command: " << style::bold << "undo" << style::reset << '\n';
        cout << "   Un-does a change\n";
        cout << "Command: " << style::bold << "redo" << style::reset << '\n';
        cout << "   Re-does an undone change\n";
        cout << "Command: " << style::bold << "load" << style::reset << '\n';
        cout << "   Tries to load from a file\n";
        cout << "Command: " << style::bold << "save" << style::reset << '\n';
        cout << "   Tries to save current database in a file\n";
        cout << "Command: " << style::bold << "help" << style::reset << '\n';
        cout << "   Print this menu" << '\n';
        cout << "Command: " << style::bold << "clear" << style::reset << '\n';
        cout << "   Clear the screen" << '\n';
        cout << "Command: " << style::bold << "quit" << style::reset << '\n';
        cout << "   Quit the application\n";
    }
}

namespace dbm
{
    void printAll()
    {
        display(db.extractAll());
    }
    std::pair<DataIter, bool> search(int prompt_flg = 0, string const &prompt_msg = "Please Choose a Serial Number: ")
    {
        auto help = [&]()
        {
            using rang::style;
            cout << "Command: " << style::bold << "company" << style::reset << '\n';
            cout << "   Sort via Company\n";
            cout << "Command: " << style::bold << "model" << style::reset << '\n';
            cout << "   Sort via Model\n";
            cout << "Command: " << style::bold << "price" << style::reset << '\n';
            cout << "   Sort via price\n";
            if (!prompt_flg)
            {
                cout << "Command: " << style::bold << "quantity" << style::reset << '\n';
                cout << "   Sort via Quantity\n";
                cout << "Command: " << style::bold << "attributes" << style::reset << '\n';
                cout << "   Sort via Attributes\n";
                cout << "Command: " << style::bold << "whole" << style::reset << '\n';
                cout << "   Search via all\n";
            }
            cout << "Command: " << style::bold << "up" << style::reset << '\n';
            cout << "   Go Back\n";
            cout << "Command: " << style::bold << "help" << style::reset << '\n';
            cout << "   Print this menu\n";
        };

        enum
        {
            Comapny = 1,
            Model,
            Quantity,
            Attributes,
            Range,
            Whole,
            Help,
            Up
        };
        static std::unordered_map<std::string, int> searchCases{
            {"company", Comapny},
            {"model", Model},
            {"quantity", Quantity},
            {"attributes", Attributes},
            {"whole", Whole},
            {"price", Range},
            {"help", Help},
            {"up", Up}};
        cout << "Type 'help' for a list of all search commands\n";
        DataIterVec n_obj;
        bool redo = 0;
        do
        {
            redo = 0;
            string choice;
            cout << rang::style::bold << rang::fg::green << "search> " << rang::style::reset;
            cin >> choice;
            try
            {
                switch (searchCases.at(choice))
                {
                case Whole:
                {
                    Vehicle obj = createObject();
                    n_obj = db.search(obj);
                    break;
                }
                case Comapny:
                {
                    string company;
                    cout << "Enter Company Name: ";
                    company = getLine();
                    n_obj = db.searchByCompany(company);
                    std::sort(n_obj.begin(), n_obj.end(), [](DataIter a, DataIter b)
                              { return a->getCost() < b->getCost(); });
                    break;
                }
                case Model:
                {
                    string model;
                    cout << "Enter Model Name: ";
                    model = getLine();
                    n_obj = db.searchByModel(model);
                    std::sort(n_obj.begin(), n_obj.end(), [](DataIter a, DataIter b)
                              { return a->getCost() < b->getCost(); });
                    break;
                }
                case Quantity:
                {
                    string quantity;
                    cout << "Enter Quantity: ";
                    cin >> quantity;
                    n_obj = db.searchByQuantity(Quantity);
                    std::sort(n_obj.begin(), n_obj.end(), [](DataIter a, DataIter b)
                              { return a->getQuantity() < b->getQuantity(); });
                    break;
                }
                case Attributes:
                {
                    vector<string> temp;
                    cout << "Enter Attributes(Color, Wheel Size, Milage): ";
                    for (int i = 0; i < ATTR; i++)
                    {
                        string tmp;
                        tmp = getLine();
                        temp.push_back(tmp);
                    }
                    n_obj = db.searchByAttribute(temp);
                    break;
                }
                case Range:
                {
                    long lb, ub;
                    cout << "Enter Lowest Price and Highest Price: ";
                    cin >> lb >> ub;
                    n_obj = db.searchByRange(lb, ub);
                    std::sort(n_obj.begin(), n_obj.end(), [](DataIter a, DataIter b)
                              { return a->getCost() < b->getCost(); });
                    break;
                }
                case Up:
                {
                    cout << "Returning to previous menu\n";
                    return {DataIter(), 0};
                    break;
                }
                case Help:
                {
                    help();
                    redo = 1;
                    break;
                }
                }
            }
            catch (std::out_of_range &e)
            {
                cout << rang::fg::red << "Entered Command is invalid, type 'help' for list of valid commands.\nType 'up' to return to previous menu\n";
                redo = 1;
            }
            if (redo)
                continue;
            display(n_obj);
            if (!n_obj.empty())
            {
                if (prompt_flg == 1)
                {
                    while (true)
                    {
                        int sr;
                        cout << prompt_msg;
                        cin >> sr;
                        if (n_obj.size() >= sr)
                            return {n_obj[sr - 1], 1};
                        else
                        {
                            cout << "Given serial number is not there in the list\n";
                            cout << "Try again\n";
                        }
                    }
                }
                return {n_obj[0], 1};
            }
            else
            {
                flushCin();
                cout << "Would you like to try again?(Y/N): ";
                char ans;
                cin >> ans;
                ans = tolower(ans);
                if (ans == 'y')
                    redo = 1;
                else
                    redo = 0;
            }
        } while (redo);
        return {DataIter(), 0};
    }
    void edit()
    {
        cout << "This action is reversible, type 'undo' to undo\n";
        cout << "Search a vehicle to edit: \n";
        auto it = dbm::search(1, "Choose a Vehicle: ");
        if (!it.second)
        {
            cout << "Search returned no items...\n";
            cout << "Returning to previous menu\n";
            return;
        }
        auto help = []()
        {
            cout << "Enter which field to edit: \n";
            cout << "Fields to edit could be:\n  " << rang::style::bold << "      company\n        model\n        attributes\n        quantity\n        cost\n        margin\n        whole\n";
            cout << rang::style::reset;
            cout << "Type << 'up' to exit without editing\n";
        };
        enum
        {
            Company = 1,
            Model,
            Attributes,
            Quantity,
            Cost,
            Margin,
            Whole,
            Help,
            Up
        };
        static map<string, int> editCases{
            {"company", Company},
            {"model", Model},
            {"attributes", Attributes},
            {"quantity", Quantity},
            {"cost", Cost},
            {"margin", Margin},
            {"whole", Whole},
            {"help", Help},
            {"up", Up}};
        bool redo = 0;
        do
        {
            redo = 0;
            cout << "Type 'help' to print fields available for editing\n";
            cout << rang::style::bold << rang::fg::green << "edit> " << rang::style::reset;
            string input;
            cin >> input;
            try
            {
                switch (editCases.at(input))
                {
                case Company:
                {
                    std::string comp;
                    cout << "Enter New Company Name: ";
                    comp = getLine();
                    db.editByCompany(it.first, comp);
                    break;
                }
                case Model:
                {
                    std::string model;
                    cout << "Enter New Model Name: ";
                    model = getLine();
                    db.editByModel(it.first, model);
                    break;
                }
                case Attributes:
                {
                    vector<string> attr;
                    cout << "Enter New Attributes(Color , Wheel Size , Milage): ";
                    for (int i = 0; i < ATTR; i++)
                    {
                        string tmp;
                        tmp = getLine();
                        attr.push_back(tmp);
                        db.editByAttributes(it.first, attr);
                    }
                    break;
                }
                case Quantity:
                {
                    long quantity;
                    cout << "Enter New Quantity: ";
                    cin >> quantity;
                    db.editByQuantity(it.first, quantity);
                    break;
                }
                case Cost:
                {
                    double cost;
                    cout << "Enter New Cost: ";
                    cin >> cost;
                    db.editByCost(it.first, cost);
                    break;
                }
                case Margin:
                {
                    double margin;
                    cout << "Enter New Profit Margin: ";
                    cin >> margin;
                    db.editProfitMargin(it.first, margin);
                    break;
                }
                case Whole:
                {
                    db.edit(it.first, createObject());
                    break;
                }
                case Help:
                {
                    help();
                    redo = 1;
                    break;
                }
                case Up:
                {
                    return;
                }
                }
            }
            catch (std::out_of_range &e)
            {
                std::cout << rang::fg::red << "Invalid Input, type 'up' to return to previous menu" << std::endl;
                redo = 1;
            }
        } while (redo);
    }
    void ins()
    {
        Vehicle obj = createObject();
        db.insert(obj);
    }
    void del()
    {
        std::cout << "This action is reversable, type 'undo' to reverse.\n";
        cout << "Search a vehicle to delete: \n";
        auto it = dbm::search(1, "Choose a Vehicle: ");
        if (it.second)
        {
            auto iter2 = it.first;
            ++iter2;
            if (db.toDelete(it.first) == iter2)
                cout << "Deletion successful\n";
            else
                cout << "Could not delete, some error occurred\n";
        }
        else
        {
            cout << "Search returned no items... returning to previous menu\n";
        }
    }
    void help()
    {
        using rang::style;
        cout << "Command: " << style::bold << "insert" << style::reset << '\n';
        cout << "   Insert Vehicle Data\n";
        cout << "Command: " << style::bold << "search" << style::reset << '\n';
        cout << "   Get a sorted Vehicle data by various attributes such as company, model etc\n";
        cout << "Command: " << style::bold << "edit" << style::reset << '\n';
        cout << "   Edit Vehicle Data\n";
        cout << "Command: " << style::bold << "del" << style::reset << '\n';
        cout << "   Delete Vehicle Entry\n";
        cout << "Command: " << style::bold << "disp" << style::reset << '\n';
        cout << "   Displays all records" << '\n';
        cout << "Command: " << style::bold << "undo" << style::reset << '\n';
        cout << "   Un-does a change\n";
        cout << "Command: " << style::bold << "redo" << style::reset << '\n';
        cout << "   Re-does an undone change\n";
        cout << "Command: " << style::bold << "load" << style::reset << '\n';
        cout << "   Tries to load from a file\n";
        cout << "Command: " << style::bold << "save" << style::reset << '\n';
        cout << "   Tries to save current database in a file\n";
        cout << "Command: " << style::bold << "up" << style::reset << '\n';
        cout << "   Go up a menu\n";
        cout << "Command: " << style::bold << "help" << style::reset << '\n';
        cout << "   Print this menu" << '\n';
        cout << "Command: " << style::bold << "clear" << style::reset << '\n';
        cout << "   Clear the screen" << '\n';
        cout << "Command: " << style::bold << "quit" << style::reset << '\n';
        cout << "   Quit the application" << '\n';
    }
}
namespace rev
{
    void receipt(DataIter const &n_obj, int const &quantity, double finalCost)
    {
        const int spaceGap = 15;
        auto seperator = [=]()
        {
            for (int i = 0; i < 8; ++i)
            {
                cout << '+';
                for (int j = 0; j < spaceGap + 1; ++j)
                    cout << '-';
            }
            cout << "+\n";
        };
        cout << rang::style::bold;
        seperator();
        cout << right << "| "
             << setw(spaceGap) << left << "Quantity"
             << right << "| "
             << setw(spaceGap) << left << "Company"
             << right << "| "
             << setw(spaceGap) << left << "Model"
             << right << "| "
             << setw(spaceGap) << left << "Price(in Rs)"
             << right << "| "
             << setw(spaceGap) << left << "Sub Total"
             << right << "| "
             << setw(spaceGap) << left << "GST(in %)"
             << right << "| "
             << setw(spaceGap) << left << "Road Tax(in %)"
             << right << "| "
             << setw(spaceGap) << left << "Total(in Rs)"
             << right << "| "
             << "\n";
        seperator();
        cout << rang::style::reset;
        cout << right << "| "
             << setw(spaceGap) << left << quantity
             << right << "| "
             << setw(spaceGap) << left << n_obj->getCompany()
             << right << "| "
             << setw(spaceGap) << left << n_obj->getModelName()
             << right << "| "
             << setw(spaceGap) << left << db.getSalePrice(n_obj)
             << right << "| "
             << setw(spaceGap) << left << db.getSalePrice(n_obj) * quantity
             << right << "| "
             << setw(spaceGap) << left << db.getGst() * 100
             << right << "| "
             << setw(spaceGap) << left << db.getRoadTax() * 100
             << right << "| "
             << setw(spaceGap) << left << finalCost
             << right << "| "
             << "\n";
        seperator();
    }

    void help()
    {
        using rang::style;
        cout << "Command: " << style::bold << "sell" << style::reset << '\n';
        cout << "   Sell vehicle(s)" << '\n';
        cout << "Command: " << style::bold << "disp" << style::reset << '\n';
        cout << "   Display Daybook" << '\n';
        cout << "Command: " << style::bold << "load" << style::reset << '\n';
        cout << "   Tries to load from a file\n";
        cout << "Command: " << style::bold << "save" << style::reset << '\n';
        cout << "   Tries to save current database in a file\n";
        cout << "Command: " << style::bold << "clear" << style::reset << '\n';
        cout << "   Clear the screen" << '\n';
        cout << "Command: " << style::bold << "up" << style::reset << '\n';
        cout << "   Go up a menu\n";
    }

void sell() //Check This Out!
{
    auto it = dbm::search(1, "Which Vehicle Would You Like to buy(Serial Number): ");
    if (it.second)
    {
        cout << "How many would you like to buy: ";
        long quantity;
        cin >> quantity;
        auto finalCost = db.calcRevenue(it.first, quantity);
        if (finalCost == 0)
            cout << "Sorry! It seems we don't have the required quantity of specified Automobiles in stock.\n";
        else
            {
                cout << "Here is your receipt:\n";
                receipt(it.first, quantity, finalCost);
            }
        }
    else
    {
        cout << "No items selected. Returning to the previous menu...\n";
    }
}

    void disp()
    {
        const int spaceGap = 15;
        auto seperator = [=]()
        {
            for (int i = 0; i < 3; ++i)
            {
                cout << '+';
                for (int j = 0; j < spaceGap + 1; ++j)
                    cout << '-';
            }
            cout << "+\n";
        };
        cout << rang::style::bold;
        seperator();
        cout << right << "| "
             << setw(spaceGap) << left << "Comapny"
             << right << "| "
             << setw(spaceGap) << left << "Sales"
             << right << "| "
             << setw(spaceGap) << left << "Profit"
             << right << "| "
             << "\n";
        seperator();
        cout << rang::style::reset;
        for (auto const iter : db.getSales())
        {
            cout << right << "| "
                 << setw(spaceGap) << left << iter.first
                 << right << "| "
                 << setw(spaceGap) << left << iter.second
                 << right << "| "
                 << setw(spaceGap) << left << db.getProfit().at(iter.first)
                 << right << "| "
                 << "\n";
            seperator();
        }
    }
}

void dbms_menu()
{
    clrscr();
    bool active = 1;
    enum
    {
        insert = 1,
        search,
        edit,
        del,
        up,
        help,
        clear,
        save,
        load,
        disp,
        undo,
        redo,
        quit
    };
    static std::map<string, int> dbmsCases{
        {"help", help},
        {"insert", insert},
        {"search", search},
        {"edit", edit},
        {"del", del},
        {"up", up},
        {"clear", clear},
        {"save", save},
        {"load", load},
        {"disp", disp},
        {"undo", undo},
        {"redo", redo},
        {"quit", quit}};
    std::cout << rang::style::bold << "Type 'help' to get a list of all valid commands\n"
              << rang::style::reset;
    while (active && !cin.eof())
    {
        try
        {
            std::string choice;
            cout << rang::style::bold << rang::fg::green << "dbms> " << rang::style::reset;
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
            case del:
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
            case disp:
            {
                dbm::printAll();
                break;
            }
            case save:
            {
                ::save();
                break;
            }
            case load:
            {
                ::load();
                break;
            }
            case undo:
            {
                ::undo();
                break;
            }
            case redo:
            {
                ::redo();
                break;
            }
            case quit:
            {
                signalHandler(2);
            }
            }
            flushCin();
        }
        catch (std::out_of_range &e)
        {
            cout << rang::fg::red << "Invalid input, type 'help' to get a list of all valid commands\n";
        }
    }
}
void rev_menu()
{
    clrscr();
    bool active = 1;
    enum
    {
        help,
        sell,
        disp,
        save,
        load,
        clear,
        up
    };
    std::map<string, int> revCases{
        {"help", help},
        {"sell", sell},
        {"disp", disp},
        {"save", save},
        {"load", load},
        {"clear", clear},
        {"up", up}};
    std::cout << rang::style::bold << "Type 'help' to get a list of all valid commands\n"
              << rang::style::reset;
    while (active && !cin.eof())
    {
        try
        {
            string choice;
            cout << rang::style::bold << rang::fg::green << "revenue> " << rang::style::reset;
            cin >> choice;
            switch (revCases.at(choice))
            {
            case help:
            {
                rev::help();
                break;
            }
            case sell:
            {
                rev::sell();
                break;
            }
            case disp:
            {
                rev::disp();
                break;
            }
            case save:
            {
                ::save();
                break;
            }
            case load:
            {
                ::load();
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
            flushCin();
        }
        catch (std::out_of_range &e)
        {
            cout << rang::fg::red << "Invalid input, type 'help' to get a list of all valid commands\n";
        }
    }
}
void main_menu()
{
    clrscr();
    bool active = 1;
    enum
    {
        dbms = 1,
        revenue,
        save,
        load,
        help,
        clear,
        up,
        undo,
        redo,
        quit
    };
    static std::map<string, int> mainCases{
        {"help", help},
        {"dsys", dbms},
        {"rsys", revenue},
        {"save", save},
        {"load", load},
        {"clear", clear},
        {"up", up},
        {"undo", undo},
        {"redo", redo},
        {"quit", quit}};
    cout << rang::style::bold << rang::fg::blue << "Welcome to MAS! Type 'help' to get a list of all valid commands\n"
         << rang::style::reset;
    while (active && !cin.eof())
    {
        try
        {
            string choice;
            cout << rang::style::bold << rang::fg::green << "ams> " << rang::style::reset;
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
            case save:
            {
                ::save();
                break;
            }
            case load:
            {
                ::load();
                break;
            }
            case undo:
            {
                ::undo();
                break;
            }
            case redo:
            {
                ::redo();
                break;
            }
            case quit:
            {
                signalHandler(2);
            }
            }
        }
        catch (std::out_of_range &e)
        {
            cout << rang::fg::red << "Invalid command, type 'help' to get a list of all valid commands\n";
        }
        flushCin();
    }
}

void signalHandler(int signum)
{
    if (!cin.eof())
    {
        char c;
        cout << "\nQuitting...\n"
             << "Would You like to save?(Y/N): ";
        flushCin();
        cin >> c;
        c = tolower(c);
        bool redo = 0;
        do
        {
            redo = 0;
            if (c == 'y')
                save();
            else if (c == 'n')
                ;
            else
            {
                redo = 1;
                cout << "Invalid input, try again\n";
            }
        } while (redo);
    }
    exit(signum);
}

int main()
{
    signal(SIGINT, signalHandler);
    cout.precision(15);
    while (1 && !cin.eof())
    {
        try
        {
            main_menu();
        }
        catch (std::out_of_range &e)
        {
            cout << "Invalid input, type \"help\" to view a list of all valid commands\n";
        }
    }
    signalHandler(2);
}