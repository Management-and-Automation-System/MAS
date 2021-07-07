#include "classes.hpp"
#include <fstream>
#include <istream>
#include <ostream>
#include <string>
#include <vector>

using namespace std;
Vehicle ::Vehicle() = default;
Vehicle ::Vehicle(string const& company, string const& modelName, vector<string> const& attributes, long const& quantity, double const& cost, double const& profitMargin)
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
double Vehicle::getCost() const
{
    return m_cost;
}
double Vehicle::getProfitMargin() const
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
void Vehicle::setCost(double const& cost)
{
    m_cost = cost;
}
void Vehicle::setProfitMargin(double const& profitMargin)
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

dbms::DataIter dbms::insert(Vehicle const& vehicle, bool redoing, bool noproc)
{
    m_data.push_back(vehicle);
    auto temp = --m_data.end();
    if (!noproc)
    {
        if (!redoing)
            m_redoHistory.clear();
        m_undoHistory.emplace_back(temp, 2);
    }
    return temp;
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
dbms ::DataIterVec dbms ::searchByRange(double const& lb, double const& ub, bool asc)
{
    DataIterVec result;
    for (auto iter = m_data.begin(); iter != m_data.end(); iter++)
    {
        if (iter->getQuantity() >= lb && iter->getQuantity() <= ub)
            result.push_back(iter);
    }
    auto compAsc = [](DataIter a, DataIter b)
    { return a->getCost() > b->getCost(); };
    auto compDsc = [&](DataIter a, DataIter b)
    { return !compAsc(a, b); };
    if (asc)
        sort(result.begin(), result.end(), compAsc);
    else
        sort(result.begin(), result.end(), compDsc);
    return result;
}
dbms::DataIter dbms::edit(DataIter toChange, Vehicle const& change, bool redoing, bool noproc)
{
    if (!noproc)
    {
        m_undoHistory.emplace_back(toChange, 0);
        *toChange = change;
        if (!redoing)
            m_redoHistory.clear();
    }
    else
        *toChange = change;
    return toChange;
}
dbms::DataIter dbms::editByCompany(DataIter toChange, std::string const& chComapny, bool redoing, bool noproc)
{
    Vehicle temp(*toChange);
    temp.setCompany(chComapny);
    return edit(toChange, temp);
}
dbms::DataIter dbms::editByModel(DataIter toChange, std::string const& chModel, bool redoing, bool noproc)
{
    Vehicle temp(*toChange);
    temp.setModel(chModel);
    return edit(toChange, temp);
}
dbms::DataIter dbms::editByAttributes(DataIter toChange, std::vector<std::string> const& chComapny, bool redoing, bool noproc)
{
    Vehicle temp(*toChange);
    temp.setAttributes(chComapny);
    return edit(toChange, temp, redoing, noproc);
}
dbms::DataIter dbms::editByQuantity(DataIter toChange, long const& chComapny, bool redoing, bool noproc)
{
    Vehicle temp(*toChange);
    temp.setQuantity(chComapny);
    return edit(toChange, temp, redoing, noproc);
}
dbms ::DataIter dbms::editProfitMargin(DataIter toChange, double const& newMargin, bool redoing, bool noproc)
{
    Vehicle temp(*toChange);
    temp.setProfitMargin(newMargin);
    return edit(toChange, temp, redoing, noproc);
}
dbms ::DataIter dbms::editByCost(DataIter toChange, double const& newCost, bool redoing, bool noproc)
{
    Vehicle temp(*toChange);
    temp.setCost(newCost);
    return edit(toChange, temp, redoing, noproc);
}
dbms::DataIter dbms::toDelete(DataIter toDel, bool redoing, bool noproc)
{
    if (!noproc)
    {
        m_undoHistory.emplace_back(toDel, 1, toDel);
        toDel = m_data.erase(toDel);
        m_undoHistory.back().m_absPos = toDel;
        if (!redoing)
            m_redoHistory.clear();
    }
    else
    {
        toDel = m_data.erase(toDel);
    }
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
        m_redoHistory.push_back(History(change.m_absPos, 0));
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
        m_redoHistory.push_back(History(temp, 1));
        break;
    }
    case 2:
    {
        m_redoHistory.push_back(History(change.m_absPos, 2));
        m_data.erase(change.m_absPos);
        break;
    }
    }
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

bool dbms::load(std::istream& is)
{
    is >> ws;
    if (is.eof())
        return false;
    while (!is.eof())
    {
        is >> ws;
        auto getline = [&]()
        {
            string result;
            for (char c = is.get(); c != '\n'; c = is.get())
            {
                if (c == EOF)
                    break;
                result.push_back(c);
            }
            return result;
        };
        is >> ws;
        if (is.eof())
            break;
        std::string comp(getline()), model(getline());
        long quantity;
        vector<string> attr;
        double cost, prof;
        is >> quantity >> cost >> prof;
        is >> ws;
        for (int i = 0; i < ATTR; ++i)
        {
            attr.push_back(getline());
        }
        if (comp == "" || model == "" || attr.empty())
            return false;
        insert(Vehicle(comp, model, attr, quantity, cost, prof), 0, 1);
    }
    return true;
}

bool dbms::save(std::ostream& os)
{
    for (auto iter : *this)
    {
        os << iter.getCompany() << '\n'
           << iter.getModelName() << '\n'
           << iter.getQuantity() << '\n'
           << iter.getCost() << '\n'
           << iter.getProfitMargin() << '\n';
        for (auto iter2 : iter.getAttributes())
            os << iter2 << '\n';
    }
    return true;
}

bool dbms::save(std::string const& filename)
{
    std::ofstream os(filename);
    return save(os);
}
bool dbms::load(std::string const& filename)
{
    std::ifstream is(filename);
    return load(is);
}

bool dbms::empty()
{
    return m_data.empty();
}
