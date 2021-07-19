#ifndef CLASSES_H
#define CLASSES_H

#include <list>
#include <string>
#include <vector>

#define ATTR 3

class Vehicle;
class History;
class dbms;

class Vehicle
{
protected:
    std::string m_company;
    std::string m_modelName;
    std::vector<std::string> m_attributes; // only 3 attributes -> color, wheel size and milage
    unsigned long m_quantity;
    double m_cost;
    double m_profitMargin;

public:
    Vehicle();
    Vehicle(std::string const &company, std::string const &modelName, std::vector<std::string> const &attributes, long const &quantiy, double const &cost, double const &profitMargin);
    std::string getCompany() const;
    std::string getModelName() const;
    std::vector<std::string> getAttributes() const;
    long getQuantity() const;
    double getCost() const;
    double getProfitMargin() const;
    void setCompany();
    void setModel();
    void setCompany(std::string const &);
    void setModel(std::string const &);
    void setAttributes(std::vector<std::string> const &);
    void setQuantity(long const &);
    void setCost(double const &);
    void setProfitMargin(double const &);
    bool operator==(Vehicle const &other) const;
    virtual ~Vehicle() = default;
};

class History : public Vehicle
{
    friend class dbms;
    char m_deletiness = 0;
    std::list<Vehicle>::iterator m_absPos;
    std::list<Vehicle>::iterator m_oldPos;

public:
    History();
    History(std::list<Vehicle>::iterator, char, std::list<Vehicle>::iterator = std::list<Vehicle>::iterator());
    virtual ~History() = default;
};

class dbms
{
protected:
    std::list<Vehicle> m_data;
    std::vector<History> m_undoHistory;
    std::vector<History> m_redoHistory;
    void pushToUndo();
    void pushToRedo();

public:
    using DataIter = std::list<Vehicle>::iterator;
    using DataIterVec = std::vector<DataIter>;

    //Templates
    template <bool _Redoing = 0 , bool _Noproc = 0>
    DataIter insert(Vehicle const &);

    template <bool _Redoing = 0 , bool _Noproc = 0>
    DataIter edit(DataIter, Vehicle const &);
    template <bool _Redoing = 0 , bool _Noproc = 0>
    DataIter editByCompany(DataIter, std::string const &);
    template <bool _Redoing = 0 , bool _Noproc = 0>
    DataIter editByModel(DataIter, std::string const &);
    template <bool _Redoing = 0 , bool _Noproc = 0>
    DataIter editByAttributes(DataIter, std::vector<std::string> const &);
    template <bool _Redoing = 0 , bool _Noproc = 0>
    DataIter editByQuantity(DataIter, long const &);
    template <bool _Redoing = 0 , bool _Noproc = 0>
    DataIter editProfitMargin(DataIter, double const &);
    template <bool _Redoing = 0 , bool _Noproc = 0>
    DataIter editByCost(DataIter, double const &);

    template <bool _Redoing = 0 , bool _Noproc = 0>
    DataIter toDelete(DataIter);

    //Search Functions
    DataIterVec extractAll();
    DataIterVec search(Vehicle const &);
    DataIterVec searchByCompany(std::string const &);
    DataIterVec searchByModel(std::string const &);
    DataIterVec searchByAttribute(std::vector<std::string> const &);
    DataIterVec searchByQuantity(long const &);
    DataIterVec searchByRange(double const &, double const &);
    
    //Iteration
    DataIter begin();
    DataIter end();

    //Filehandling
    virtual bool load(std::string const &);
    virtual bool save(std::string const &);
    virtual bool load(std::istream &);
    virtual bool save(std::ostream &);
    virtual bool empty();
    int undo();
    int redo();

    virtual ~dbms() = default;
};

//Template Implementations

template <bool _Redoing , bool _Noproc >
dbms::DataIter dbms::insert(Vehicle const &vehicle)
{
    m_data.push_back(vehicle);
    auto temp = --m_data.end();
    if (!_Noproc)
    {
        if (!_Redoing)
            m_redoHistory.clear();
        m_undoHistory.emplace_back(temp, 2);
    }
    return temp;
}

template <bool _Redoing , bool _Noproc>
dbms::DataIter dbms::edit(DataIter toChange, Vehicle const &change)
{
    if (!_Noproc)
    {
        m_undoHistory.emplace_back(toChange, 0);
        *toChange = change;
        if (!_Redoing)
            m_redoHistory.clear();
    }
    else
        *toChange = change;
    return toChange;
}
template <bool _Redoing , bool _Noproc>
dbms::DataIter dbms::editByCompany(DataIter toChange, std::string const &chComapny)
{
    Vehicle temp(*toChange);
    temp.setCompany(chComapny);
    return edit<_Redoing , _Noproc>(toChange, temp);
}
template <bool _Redoing , bool _Noproc>
dbms::DataIter dbms::editByModel(DataIter toChange, std::string const &chModel)
{
    Vehicle temp(*toChange);
    temp.setModel(chModel);
    return edit<_Redoing , _Noproc>(toChange, temp);
}
template <bool _Redoing , bool _Noproc>
dbms::DataIter dbms::editByAttributes(DataIter toChange, std::vector<std::string> const &chComapny)
{
    Vehicle temp(*toChange);
    temp.setAttributes(chComapny);
    return edit<_Redoing , _Noproc>(toChange, temp);
}
template <bool _Redoing , bool _Noproc>
dbms::DataIter dbms::editByQuantity(DataIter toChange, long const &chComapny)
{
    Vehicle temp(*toChange);
    temp.setQuantity(chComapny);
    return edit<_Redoing , _Noproc>(toChange, temp);
}
template <bool _Redoing , bool _Noproc>
dbms ::DataIter dbms::editProfitMargin(DataIter toChange, double const &newMargin)
{
    Vehicle temp(*toChange);
    temp.setProfitMargin(newMargin);
    return edit<_Redoing , _Noproc>(toChange, temp);
}
template <bool _Redoing , bool _Noproc>
dbms ::DataIter dbms::editByCost(DataIter toChange, double const &newCost)
{
    Vehicle temp(*toChange);
    temp.setCost(newCost);
    return edit<_Redoing , _Noproc>(toChange, temp);
}
template <bool _Redoing , bool _Noproc>
dbms::DataIter dbms::toDelete(DataIter toDel)
{
    if (!_Noproc)
    {
        m_undoHistory.emplace_back(toDel, 1, toDel);
        toDel = m_data.erase(toDel);
        m_undoHistory.back().m_absPos = toDel;
        if (!_Redoing)
            m_redoHistory.clear();
    }
    else
    {
        toDel = m_data.erase(toDel);
    }
    return toDel;
}

#endif