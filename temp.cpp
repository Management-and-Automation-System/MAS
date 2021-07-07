#include "classes.cpp"
#include "classes.hpp"
#include "menu.hpp"
#include "revenue.cpp"
#include "revenue.hpp"
#include <algorithm>
#include <cctype>
#include <csignal>
#include <cstdlib>
#include <functional>
#include <limits>
#include <map>
#include <ostream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

using namespace std;
using DataIter = dbms::DataIter;
using DataIterVec = dbms::DataIterVec;
Revenue db;

//Declerations
void flushCin();
Vehicle makeVehicle();
void display();
void save();
void load();
void clscr();
void undo();
void redo();
void dbms_menu();
void rev_menu();
void main_menu();
void signalHandler();

namespace mainm
{
void help();
}

namespace dbm
{
void printall();
std::pair<DataIter, bool> search(int prompt_flg = 0, string const& prompt_msg = "Please Choose a Serial Number: ");
void edit();
void ins();
void del();
void help();
}

namespace revm
{
void sell();
}

void flushCin()
{
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
    else if (ans == 'n')
    {
        string filename;
        cout << "Enter the loadfile name: ";
        cin >> filename;
        if (db.load(filename))
            cout << "Loaded Successfully\n";
        else
            cout << "Load was not successful, some error occured\n";
    }
    else
    {
        cout << "Invalid Input. Returning to previous menu.";
        return;
    }
}
void clrscr()
{
    cout << "\033[2J\033[1;1H";
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

    cout << "Command: dsys" << '\n';
    cout << "   Open the Database Section" << '\n';
    cout << "Command: rsys" << '\n';
    cout << "   Open the Revenue Section" << '\n';
    cout << "Command: undo\n";
    cout << "   Un-does a change\n";
    cout << "Command: redo\n";
    cout << "   Re-does an undone change\n";
    cout << "Command: load\n";
    cout << "   Tries to load from a file\n";
    cout << "Command: save\n";
    cout << "   Tries to save current database in a file\n";
    cout << "Command: help" << '\n';
    cout << "   Print this menu" << '\n';
    cout << "Command: clear" << '\n';
    cout << "   Clear the screen" << '\n';
    cout << "Command: CTRL+C\n";
    cout << "   Quit the application\n";
}
}

namespace dbm
{
void printAll()
{
    DataIterVec temp;
    for (auto it = db.begin(); it != db.end(); ++it)
        temp.push_back(it);
    display(temp);
}
std::pair<DataIter, bool> search(int prompt_flg, string const& prompt_msg)
{
    auto help = [&]()
    {
        cout << "Command: company\n";
        cout << "   Sort vai Company\n";
        cout << "Command: model\n";
        cout << "   Sort vai Model\n";
        cout << "Command: price\n";
        cout << "   Sort vai price\n";
        if (!prompt_flg)
        {
            cout << "Command: quantity\n";
            cout << "   Sort vai Quantity\n";
            cout << "Command: attributes\n";
            cout << "   Sort vai Attributes\n";
            cout << "Command: whole\n";
            cout << "   Search vai all\n";
        }
        cout << "Command: up\n";
        cout << "   Go Back\n";
        cout << "Command: help\n";
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
    static std::unordered_map<std::string, int> searchCases {
        { "company", Comapny },
        { "model", Model },
        { "quantity", Quantity },
        { "attributes", Attributes },
        { "whole", Whole },
        { "price", Range },
        { "help", Help },
        { "up", Up }
    };
    cout << "Type 'help' for a list of all search commands\n";
    DataIterVec n_obj;
    bool redo = 0;
    do
    {
        redo = 0;
        string choice;
        cout << "search> ";
        cin >> choice;
        try
        {
            switch (searchCases.at(choice))
            {
            case Whole:
            {
                Vehicle obj = makeVehicle();
                n_obj = db.search(obj);
                break;
            }
            case Comapny:
            {
                string company;
                cout << "Enter Company Name: ";
                cin >> company;
                n_obj = db.searchByCompany(company);
                break;
            }
            case Model:
            {
                string model;
                cout << "Enter Model Name: ";
                cin >> model;
                n_obj = db.searchByModel(model);
                break;
            }
            case Quantity:
            {
                string quantity;
                cout << "Enter Quantity: ";
                cin >> quantity;
                n_obj = db.searchByQuantity(Quantity);
                break;
            }
            case Attributes:
            {
                vector<string> temp;
                cout << "Enter Attributes(Color, Wheel Size, Milage): ";
                for (int i = 0; i < ATTR; i++)
                {
                    string tmp;
                    cin >> tmp;
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
                return { DataIter(), 0 };
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
        catch (std::out_of_range& e)
        {
            cout << "Entered Command is invalid, type 'help' for list of valid commands. Type 'up' to return to previous menu\n";
            redo = 1;
        }
        if (redo)
            continue;
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
    return { DataIter(), 0 };
}
void edit()
{
    cout << "This action is reversible, type 'undo' to undo\n";
    cout << "Search a vehicle to edit: \n";
    auto it = dbm::search(1, "Choose a Vehicle: ");
    if (!it.second)
    {
        cout << "edit> Search returned no items...\n";
        cout << "Returning to previous menu\n";
        return;
    }
    auto help = []()
    {
        cout << "Enter which field to edit: \n";
        cout << "Fields to edit could be:\n        company\n        model\n        attributes\n        quantity\n        cost\n        margin\n        whole\n";
        cout << "Type 'up' to exit without editing\n";
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
    static map<string, int> editCases {
        { "company", Company },
        { "model", Model },
        { "attributes", Attributes },
        { "quantity", Quantity },
        { "cost", Cost },
        { "margin", Margin },
        { "whole", Whole },
        { "help", Help },
        { "up", Up }
    };
    bool redo = 0;
    do
    {
        redo = 0;
        try
        {
            cout << "Type 'help' to print fields available for editing\n";
            cout << "edit> ";
            string input;
            cin >> input;
            switch (editCases.at(input))
            {
            case Company:
            {
                std::string comp;
                cout << "Enter New Company Name: ";
                cin >> comp;
                db.editByCompany(it.first, comp);
                break;
            }
            case Model:
            {
                std::string model;
                cout << "Enter New Model Name: ";
                cin >> model;
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
                    cin >> tmp;
                    attr.push_back(tmp);
                }
                db.editByAttributes(it.first, attr);
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
                cout << "Enter New Quantity: ";
                cin >> margin;
                db.editByQuantity(it.first, margin);
                break;
            }
            case Whole:
            {
                db.edit(it.first, makeVehicle());
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
        catch (std::out_of_range& e)
        {
            std::cout << "Invalid Input, type 'up' to return to previous menu\n";
            redo = 1;
        }
    } while (redo);
}
void ins()
{
    Vehicle obj = makeVehicle();
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
    cout << "Command: insert" << '\n';
    cout << "   Insert Vehicle Data\n";
    cout << "Command: search" << '\n';
    cout << "   Get a sorted Vehicle data by various attributes such as company, model etc\n";
    cout << "Command: edit" << '\n';
    cout << "   Edit Vehicle Data\n";
    cout << "Command: del" << '\n';
    cout << "   Delete Vehicle Entry\n";
    cout << "Command: list\n";
    cout << "   List everthing in database\n";
    cout << "Command: undo\n";
    cout << "   Un-does a change\n";
    cout << "Command: redo\n";
    cout << "   Re-does an undone change\n";
    cout << "Command: load\n";
    cout << "   Tries to load from a file\n";
    cout << "Command: save\n";
    cout << "   Tries to save current database in a file\n";
    cout << "Command: up" << '\n';
    cout << "   Go up a menu\n";
    cout << "Command: help" << '\n';
    cout << "   Print this menu" << '\n';
    cout << "Command: clear" << '\n';
    cout << "   Clear the screen" << '\n';
    cout << "Command: CTRL+C\n";
    cout << "   Quit the application\n";
}
}

namespace revm
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

void dbms_menu()
{
    clrscr();
    bool active = 1;
    enum
    {
        insert = 1,
        search,
        edit,
        rem,
        up,
        help,
        clear,
        ls,
        save,
        load,
        undo,
        redo
    };
    static std::map<string, int> dbmsCases {
        { "help", help },
        { "insert", insert },
        { "search", search },
        { "edit", edit },
        { "delete", rem },
        { "up", up },
        { "clear", clear },
        { "list", ls },
        { "save", save },
        { "load", load },
        { "undo", undo },
        { "redo", redo }
    };
    std::cout << "Type 'help' to get a list of all valid commands\n";
    while (active)
    {
        try
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
            case ls:
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
            }
            flushCin();
        }
        catch (std::out_of_range& e)
        {
            cout << "Invalid input, type 'help' to get a list of all valid commands\n";
        }
    }
}
void rev_menu()
{
    clrscr();
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
            revm::sell();
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
        redo
    };
    static std::map<string, int> mainCases {
        { "help", help },
        { "dsys", dbms },
        { "rsys", revenue },
        { "save", save },
        { "load", load },
        { "clear", clear },
        { "up", up },
        { "undo", undo },
        { "redo", redo }
    };
    cout << "Welcome to AMS! Type 'help' to get a list of all valid commands\n";
    while (active)
    {
        try
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
            }
        }
        catch (std::out_of_range& e)
        {
            cout << "Invalid command, type 'help' to get a list of all valid commands\n";
        }
        flushCin();
    }
}

void signalHandler(int signum)
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
    exit(0);
}

string makeCompany()
{
    string res;
    cout << "Enter Company: ";
    cin >> ws;
    getline(cin, res);
    return res;
}

string makeModel()
{
    string res;
    cout << "Enter Model: ";
    cin >> ws;
    getline(cin, res);
    return res;
}

long makeQuant()
{
    long res;
    cout << "Enter Quantity: ";
    cin >> res;
    return res;
}

double makeProf()
{
    double res;
    cout << "Enter Profit Margin: ";
    cin >> res;
    return res;
}

double makeCost()
{
    double res;
    cout << "Enter Cost: ";
    cin >> res;
    return res;
}

vector<string> makeAttr()
{
    vector<string> res;
    cout << "Enter Attributes: \n";
    for (int i = 0; i < ATTR; i++)
    {
        string tmp;
        string toPrint = "\tEnter ";
        switch (i)
        {
        case 0:
            toPrint += "Color: ";
            break;
        case 1:
            toPrint += "Wheel Size: ";
            break;
        case 2:
            toPrint += "Milage: ";
            break;
        }
        std::cout << toPrint;
        cin >> ws;
        getline(cin, tmp);
        res.push_back(std::move(tmp));
    }
    return res;
}
Vehicle makeVehicle()
{
    auto company = makeCompany();
    auto model = makeModel();
    auto attr = makeAttr();
    auto quant = makeQuant();
    auto cost = makeCost();
    auto prof = makeProf();
    return Vehicle(company, model, attr, quant, cost, prof);
}

int main()
{
    /*
    Menu<void(), void()> menu( "dbms",
        MenuItem<void()>(dbm::ins, "insert" , "Insert an Element"),
        MenuItem<void()>(dbm::edit, "edit" , "Edit an Element")
    );
    */
    //Menu men = makeMenu("dbms" , makeMenuItem( function<void()>(dbm::ins), "insert" , "Insert an Element") , makeMenuItem(function<void()>(dbm::edit) , "edit" , "Edit an Element"));
    auto search = makeChild(
        makeMenu(
            "search",
            makeMenuItem(function<DataIterVec()>([&]()
                             {
                                 auto temp = db.searchByCompany(makeCompany());
                                 display(temp);
                                 return temp;
                             }),
                "company", "Sort vai Company"),
            makeMenuItem(function<DataIterVec()>([&]()
                             {
                                 auto temp = db.searchByModel(makeCompany());
                                 display(temp);
                                 return temp;
                             }),
                "model", "Sort vai Model"),
            makeMenuItem(function<DataIterVec()>([&]()
                             {
                                 auto temp = db.searchByAttribute(makeAttr());
                                 display(temp);
                                 return temp;
                             }),
                "attributes", "Sort vai attributes"),
            makeMenuItem(function<DataIterVec()>([&]()
                             {
                                 auto temp = db.searchByQuantity(makeQuant());
                                 display(temp);
                                 return temp;
                             }),
                "quantity", "Search vai Quantity"),
            makeMenuItem(function<DataIterVec()>([&]()
                             {
                                 double ub, lb;
                                 cout << "Enter Lowerbound and Upperbound(seperated by whitespace): ";
                                 cin >> ub >> lb;
                                 auto temp = db.searchByRange(ub, lb);
                                 display(temp);
                                 return temp;
                             }),
                "range", "Sort vai Range of price"),
            makeMenuItem(function<DataIterVec()>([&]()
                             {
                                 auto temp = db.search(makeVehicle());
                                 display(temp);
                                 return temp;
                             }),
                "whole", "Search using whole Object")),
        "search", "Search through the database");
    auto editSearch = [&]()
    {
        DataIterVec vec;
        DataIter res;
        int choice;
        bool redo = 0;
        int goodIn = 0;
        cout << "Search a vehicle you would like to edit\n";
        do
        {
            redo = 0;
            auto res = search->getChild().input<0>(vec);
            if (res == 1)
            {
                goodIn = 1;
                redo = 0;
                break;
            }
            else if (res == 0)
            {
                goodIn = 0;
                redo = 1;
                continue;
            }
            else if (res == -1)
            {
                goodIn = 0;
                redo = 0;
                break;
            }
        } while (redo);
        if (goodIn)
        {
            cout << "Choose a vehicle you would like to edit(Serial Number): ";
            do
            {
                cout << "Enter Choice: ";
                cin >> choice;
                redo = 0;
                if (choice > (int)(vec.size()) || choice < -1)
                {
                    cout << "Invalid Choice. Try again or type '-1' to quit.\n";
                    goodIn = 0;
                    redo = 1;
                }
                else if (choice == -1)
                {
                    redo = 0;
                    goodIn = 0;
                    continue;
                }
                else
                {
                    goodIn = 1;
                    res = vec[choice -1];
                }
            } while (redo);
        }
        return std::make_pair(res, goodIn);
    };
    auto edit = makeChild(
        makeMenu(
            "edit",
            makeMenuItem(function<void()>([&]()
                             {
                                 auto it = editSearch();
                                 if (it.second)
                                     db.editByCompany(it.first, makeCompany());
                             }),
                "company", "Edit Company Field")),
        "edit", "Editing Menu");
    Menu menu = makeMenu("dbms",
        makeMenuItem(std::function<void()>([&]()
                         { db.insert(makeVehicle()); }),
            "insert", "Insert a Vehicle"),
        makeMenuItem(std::function<void()>([&]()
                         { dbm::printAll(); }),
            "list", "List all Vehicles in database"),
        shared_ptr<MenuItem<int()>>(search),
        shared_ptr<MenuItem<int()>>(edit));
    menu.input();
    //menu.input();
    //signal(SIGINT, signalHandler);
    /*
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
    */
}