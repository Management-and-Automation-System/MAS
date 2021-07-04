#include "classes.h"
#include <vector>

using namespace std;
Vehicle ::Vehicle() {};
Vehicle ::Vehicle(string const& company, string const& modelName, vector<string> const& attributes, long const& quantity, float const& cost, float const& profitMargin)
    : m_company(company), m_modelName(modelName), m_attributes(attributes), m_quantity(quantity), m_cost(cost), m_profitMargin(profitMargin) { }
string Vehicle::getCompany() const
{
    return m_company;
}
string Vehicle::getModelName() const
{
    return m_modelName;
}
vector<string> Vehicle::getAttributes() const
{
    return m_attributes;
}
long Vehicle ::getQuantity() const
{
    return m_quantity;
}
float Vehicle::getCost() const
{
    return m_cost;
}
float Vehicle::getProfitMargin() const
{
    return m_profitMargin;
}
void Vehicle::setCompany()
{
    m_company = "";
}
void Vehicle::setModel()
{
    m_modelName = "";
}
void Vehicle::setCompany(string const& company)
{
    m_company = company;
}
void Vehicle::setModel(string const& modelName)
{
    m_modelName = modelName;
}
void Vehicle::setAttributes(vector<string> const& attributes)
{
    m_attributes = attributes;
}
void Vehicle::setQuantity(long const& quantity)
{
    m_quantity = quantity;
}
void Vehicle::setCost(float const& cost)
{
    m_cost = cost;
}
void Vehicle::setProfitMargin(float const& profitMargin)
{
    m_profitMargin = profitMargin;
}
bool Vehicle::operator==(Vehicle const& other) const
{
    if (m_company != other.m_company)
        return false;
    if (m_modelName != other.m_modelName)
        return false;
    if (m_attributes != other.m_attributes)
        return false;
    if (m_quantity != other.m_quantity)
        return false;
    return true;
}

History::History() = default;
History::History(std::list<Vehicle>::iterator absPos, char deletiness, std::list<Vehicle>::iterator oldPos = std::list<Vehicle>::iterator())
    : Vehicle(*absPos), m_deletiness(deletiness), m_absPos(absPos), m_oldPos(oldPos) { }

dbms::DataIter dbms::insert(Vehicle const& vehicle, bool redoing)
{
    m_data.push_back(vehicle);
    auto temp = --m_data.end();
    if (!redoing)
        m_redoHistory.clear();
    m_undoHistory.emplace_back(temp, 2);
    return temp;
}
dbms ::DataIterVec dbms::extractAll()
{
    DataIterVec result;
    for (auto iter = m_data.begin(); iter != m_data.end(); iter++)
        result.push_back(iter);
    return result;
}
dbms::DataIterVec dbms::search(Vehicle const& vehicle)
{
    DataIterVec result;
    for (auto iter = m_data.begin(); iter != m_data.end(); iter++)
    {
        if (*iter == vehicle)
            result.push_back(iter);
    }
    return result;
}
dbms::DataIterVec dbms::searchByCompany(std::string const& comapny)
{
    DataIterVec result;
    for (auto iter = m_data.begin(); iter != m_data.end(); iter++)
    {
        if (iter->getCompany() == comapny)
            result.push_back(iter);
    }
    return result;
}
dbms::DataIterVec dbms::searchByModel(std::string const& model)
{
    DataIterVec result;
    for (auto iter = m_data.begin(); iter != m_data.end(); iter++)
    {
        if (iter->getModelName() == model)
            result.push_back(iter);
    }
    return result;
}
dbms::DataIterVec dbms::searchByAttribute(std::vector<std::string> const& attributes)
{
    DataIterVec result;
    for (auto iter = m_data.begin(); iter != m_data.end(); iter++)
    {
        if (iter->getAttributes() == attributes)
            result.push_back(iter);
    }
    return result;
}
dbms::DataIterVec dbms::searchByQuantity(long const& quantity)
{
    DataIterVec result;
    for (auto iter = m_data.begin(); iter != m_data.end(); iter++)
    {
        if (iter->getQuantity() == quantity)
            result.push_back(iter);
    }
    return result;
}
dbms ::DataIterVec dbms ::searchByRange(double const& lb, double const& ub)
{
    DataIterVec result;
    for (auto iter = m_data.begin(); iter != m_data.end(); iter++)
    {
        if (iter->getQuantity() >= lb && iter->getQuantity() <= ub)
            result.push_back(iter);
    }
    return result;
}
dbms::DataIter dbms::edit(DataIter toChange, Vehicle const& change, bool redoing)
{
    m_undoHistory.emplace_back(toChange, 0);
    *toChange = change;
    if (!redoing)
        m_redoHistory.clear();
    return toChange;
}
dbms::DataIter dbms::editByCompany(DataIter toChange, std::string const& chComapny)
{
    Vehicle temp(*toChange);
    temp.setCompany(chComapny);
    return edit(toChange, temp);
}
dbms::DataIter dbms::editByModel(DataIter toChange, std::string const& chModel)
{
    Vehicle temp(*toChange);
    temp.setModel(chModel);
    return edit(toChange, temp);
}
dbms::DataIter dbms::editByAttributes(DataIter toChange, std::vector<std::string> const& chComapny)
{
    Vehicle temp(*toChange);
    temp.setAttributes(chComapny);
    return edit(toChange, temp);
}
dbms::DataIter dbms::editByQuantity(DataIter toChange, long const& chComapny)
{
    Vehicle temp(*toChange);
    temp.setQuantity(chComapny);
    return edit(toChange, temp);
}
dbms ::DataIter dbms::editProfitMargin(DataIter toChange, float const& newMargin)
{
    Vehicle temp(*toChange);
    temp.setProfitMargin(newMargin);
    return edit(toChange, temp);
}
dbms::DataIter dbms::toDelete(DataIter toDel, bool redoing)
{
    m_undoHistory.emplace_back(toDel, 1, toDel);
    toDel = m_data.erase(toDel);
    m_undoHistory.back().m_absPos = toDel;
    if (!redoing)
        m_redoHistory.clear();
    return toDel;
}
int dbms::undo()
{
    if (m_undoHistory.size() == 0)
        return 0;
    History const change = m_undoHistory.back();
    m_undoHistory.pop_back();
    switch (change.m_deletiness)
    {
    case 0:
    {
        *change.m_absPos = change;
        break;
    }
    case 1:
    {
        auto temp = m_data.insert(change.m_absPos, change);
        for (auto iter : m_undoHistory)
        {
            if (iter.m_absPos == change.m_oldPos)
                iter.m_absPos = temp;
        }
        break;
    }
    case 2:
    {
        m_data.erase(change.m_absPos);
        break;
    }
    }
    m_redoHistory.push_back(std::move(change));
    return m_undoHistory.size();
}

int dbms::redo()
{
    if (m_redoHistory.size() == 0)
        return 0;
    History const change = m_redoHistory.back();
    m_redoHistory.pop_back();
    switch (change.m_deletiness)
    {
    case 0:
    {
        edit(change.m_absPos, change, 1);
        break;
    }
    case 1:
    {
        toDelete(change.m_absPos, 1);
        break;
    }
    case 2:
    {
        insert(change, 1);
        break;
    }
    }
    return m_redoHistory.size();
}
dbms::DataIter dbms::begin()
{
    return m_data.begin();
}
dbms::DataIter dbms::end()
{
    return m_data.end();
}

#if 0
bool Search::searchByCompany(string company, Vehicle obj[], int size)
{
    bool found = false;
    for (int i = 0; i < size; i++)
    {
        if (obj[i].getCompany() == company && obj[i].getCompany() != "")
        {
            found = true;
            Display tempObj;
            tempObj.displayRecord(&obj[i]);
        }
    }
    if (found)
        return true;
    return false;
}
bool Search::searchByModel(string modelName, Vehicle obj[], int size)
{
    bool found = false;
    for (int i = 0; i < size; i++)
    {
        if (obj[i].getModelName() == modelName && obj[i].getModelName() != "")
        {
            found = true;
            Display tempObj;
            tempObj.displayRecord(&obj[i]);
        }
    }
    if (found)
        return true;
    return false;
}
bool Search::searchByAttributes(vector<string> attributes, Vehicle obj[], int size)
{
    bool found = false;
    Display tempObj;
    for (int i = 0; i < size; i++)
    {
        int count = 0, j = 0;
        for (auto& x : obj[i].getAttributes())
        {
            if (attributes[j++] == x)
                count++;
        }
        if (count == 3)
        {
            found = true;
            tempObj.displayRecord(&obj[i]);
        }
    }
    if (found)
        return true;
    return false;
}

void Display::displayRecord(Vehicle* obj)
{
    if (obj->getCompany() != "" && obj->getModelName() != "")
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
    else
    {
        cout << "NO SUCH RECORD EXISTS\n";
    }
}
void Display ::displayCompanies(Vehicle* obj, int size)
{
    cout << "COMPANIES\n";
    for (int i = 0; i < size; i++)
    {
        if (obj->getCompany() != "")
            cout << obj[i].getCompany() << '\n';
    }
}
void Display ::displayCompanyAndQuantity(Vehicle obj[], int size)
{
    cout << setw(15) << "COMPANIES" << setw(15) << "QUANTITIES\n";
    for (int i = 0; i < size; i++)
    {
        if (obj[i].getCompany() != "")
            cout << setw(15) << obj[i].getCompany() << setw(15) << obj[i].getQuantity() << '\n';
    }
}
void displayAll(Vehicle obj[], int size)
{
    for (int i = 0; i < size; i++)
    {
        if (obj[i].getCompany() != "" && obj[i].getModelName() != "")
        {
            cout << setw(15) << "COMPANY" << setw(15) << "MODEL" << setw(15) << "QUANTITY" << setw(15) << "QUANTITY\n";
            cout << setw(15) << obj[i].getCompany() << setw(15) << obj[i].getModelName() << setw(15) << obj[i].getQuantity();
            int flag = 0;
            for (auto& x : obj[i].getAttributes())
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
}
void Delete ::delByCompany(string company, Vehicle obj[], int size)
{
    for (int i = 0; i < size; i++)
    {
        if (company == obj[i].getCompany())
        {
            string s = obj[i].getCompany() + " " + obj[i].getModelName() + " " + to_string(obj[i].getQuantity());
            for (auto& x : obj[i].getAttributes())
            {
                s = s + " " + x;
            }
            delHistory.push_back(s);
            obj[i].setCompany();
        }
    }
}
void Delete ::delByModel(string model, Vehicle obj[], int size)
{
    for (int i = 0; i < size; i++)
    {
        if (m_company == obj[i].getModelName())
        {
            string s = obj[i].getCompany() + " " + obj[i].getModelName() + " " + to_string(obj[i].getQuantity());
            for (auto& x : obj[i].getAttributes())
            {
                s = s + " " + x;
            }
            delHistory.push_back(s);
            obj[i].setModel();
        }
    }
}
#endif