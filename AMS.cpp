#include "classes.h"
#include <map>
#include <string>
#include <vector>

dbms db;

Vehicle make()
{
    std::cout << "Enter the details for the new object\n";
    std::string company, model;
    std::vector<std::string> attributes;
    long quantity;
    std::cin >> company >> model;
    std::string temp = "";
    while (1)
    {
        char c = 0;
        while (1)
        {
            c = std::cin.get();
            if (c == '\n')
                break;
            temp.push_back(c);
        }
        if (*temp.begin() != '\t')
        {
            break;
        }
        temp.erase(temp.begin());
        attributes.push_back(temp);
    }
    std::cin >> quantity;
    Vehicle result(company, model, attributes, quantity);
    return result;
}

void insert()
{
    db.insert(make());
}

void print(Vehicle const &vehicle)
{
    std::cout << vehicle.getCompany() << '\n'
              << vehicle.getModelName() << '\n';
    for (auto const it : vehicle.getAttributes())
        std::cout << '\t' << it;
    std::cout << '\n'
              << vehicle.getQuantity() << "\n\n";
}

void edit()
{
    auto veh = make();
    auto veh_list = db.searchByComapany(veh.getCompany());
    int temp = 1;
    for (auto iter : veh_list)
    {
        std::cout << temp << '\n';
        print(*iter);
    }
    std::cin >> temp;
    db.edit(veh_list[temp - 1], make());
}
void del()
{
    auto veh_list = db.search(make());
    int temp = 1;
    for (auto iter : veh_list)
    {
        std::cout << temp << '\n';
        print(*iter);
    }
    std::cout << "Enter a number\n";
    std::cin >> temp;
    db.toDelete(veh_list[temp - 1]);
}

void display()
{
    for (auto const it : db)
    {
        print(it);
    }
}

void find()
{
    for (auto const iter : db.search(make()))
        print(*iter);
}

int main()
{
    enum inputs
    {
        insert = 1,
        find,
        edit,
        del,
        undo,
        redo,
        display
    };
    std::map<std::string, int> cases({{"insert", inputs::insert},
                                      {"find", inputs::find},
                                      {"edit", inputs::edit},
                                      {"del", inputs::del},
                                      {"undo", inputs::undo},
                                      {"redo", inputs::redo},
                                      {"display", inputs::display}});

    while (1)
    {
        std::string input;
        std::cout << "Enter Input: ";
        std::cin >> input;
        try
        {
            switch (cases.at(input))
            {
            case inputs::insert:
            {
                ::insert();
                break;
            }
            case find:
            {
                ::find();
                break;
            }
            case edit:
            {
                ::edit();
                break;
            }
            case del:
            {
                ::del();
                break;
            }
            case undo:
            {
                db.undo();
                break;
            }
            case redo:
            {
                db.redo();
                break;
            }
            case display:
            {
                ::display();
                break;
            }
            }
            std::string help;
            std::cin >> help;
            std::cout << "\033[2J\033[1;1H"; // Clear Screen
        }
        catch (...)
        {
        }
    }
}