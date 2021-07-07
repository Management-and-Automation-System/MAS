#include "classes.hpp"
#include "menu.hpp"
#include "revenue.hpp"
#include <iomanip>

using namespace std;
using DataIter = dbms::DataIter;
using DataIterVec = dbms::DataIterVec;
Revenue db;

void flushCin() {};

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

namespace gItems
{
auto search = makeChild(
    makeMenu(
        "search",
        makeMenuItem(function<DataIterVec()>([]()
                         {
                             auto temp = db.searchByCompany(makeCompany());
                             display(temp);
                             return temp;
                         }),
            "company", "Sort vai Company"),
        makeMenuItem(function<DataIterVec()>([]()
                         {
                             auto temp = db.searchByModel(makeCompany());
                             display(temp);
                             return temp;
                         }),
            "model", "Sort vai Model"),
        makeMenuItem(function<DataIterVec()>([]()
                         {
                             auto temp = db.searchByAttribute(makeAttr());
                             display(temp);
                             return temp;
                         }),
            "attributes", "Sort vai attributes"),
        makeMenuItem(function<DataIterVec()>([]()
                         {
                             auto temp = db.searchByQuantity(makeQuant());
                             display(temp);
                             return temp;
                         }),
            "quantity", "Search vai Quantity"),
        makeMenuItem(function<DataIterVec()>([]()
                         {
                             double ub, lb;
                             cout << "Enter Lowerbound and Upperbound(seperated by whitespace): ";
                             cin >> ub >> lb;
                             auto temp = db.searchByRange(ub, lb);
                             display(temp);
                             return temp;
                         }),
            "range", "Sort vai Range of price"),
        makeMenuItem(function<DataIterVec()>([]()
                         {
                             auto temp = db.search(makeVehicle());
                             display(temp);
                             return temp;
                         }),
            "whole", "Search using whole Object")),
    "search", "Search through the database");
auto clrscr = makeMenuItem(function<void()>(::clrscr),
    "clear",
    "Clear the screen");
auto undo = makeMenuItem(function<void()>(::undo),
    "undo",
    "Undoes a change");
auto redo = makeMenuItem(function<void()>(::redo),
    "redo",
    "Redoes an undone change");
auto load = makeMenuItem(function<void()>(::load),
    "load",
    "Load from savefile");
auto save = makeMenuItem(function<void()>(::save),
    "save",
    "Save to a savefile");
auto selectSearch = []()
{
    DataIterVec vec;
    int choice;
    bool redo = 0;
    int goodIn = 0;
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
    return make_pair(vec, goodIn);
};
auto select = [](string const& printBefore = "Search for a Vehicle: \n",
                  string const& printAfter = "Select a Vehicle(Serial Number): \n")
{
    cout << printBefore;
    auto searchRes = selectSearch();
    cout << printAfter;
    auto& goodIn = searchRes.second;
    auto const& vec = searchRes.first;
    DataIter res;
    if (goodIn || vec.size() != 0)
    {
        int choice;
        bool redo;
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
                res = vec[choice - 1];
            }
        } while (redo);
    }
    return std::make_pair(res, goodIn);
};

}

namespace dbms_menu
{
    auto del = makeMenuItem(std::function<void()>([]()
                                {
                                    auto it = gItems::select();
                                    if (it.second)
                                        db.toDelete(it.first);
                                }),
        "delete", "Delete a vehicle");
    auto insert = makeMenuItem(std::function<void()>([]()
                                   { db.insert(makeVehicle()); }),
        "insert", "Insert a Vehicle");
    auto edit = makeChild(
        makeMenu(
            "edit",
            makeMenuItem(function<void()>([]()
                             {
                                 auto it = gItems::select();
                                 if (it.second)
                                     db.editByCompany(it.first, makeCompany());
                             }),
                "company", "Edit Company Field")),
        "edit", "Editing Menu");
    auto printAll = makeMenuItem(std::function<void()>([]()
                                     {
                                         DataIterVec temp;
                                         for (auto it = db.begin(); it != db.end(); ++it)
                                             temp.push_back(it);
                                         display(temp);
                                     }),
        "list", "List all Vehicles in database");

    Menu menu = makeMenu("dbms",
        insert,
        del,
        printAll,
        shared_ptr<MenuItem<int()>>(gItems::search),
        shared_ptr<MenuItem<int()>>(edit),
        gItems::undo,
        gItems::redo,
        gItems::save,
        gItems::load,
        gItems::clrscr);
}

void rev_menu()
{

}

int main()
{
    dbms_menu::menu.input();
}