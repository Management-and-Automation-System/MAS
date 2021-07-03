#ifndef CLASSES_H
#define CLASSES_H

#include <algorithm>
#include <exception>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <list>
#include <sys/types.h>
#include <vector>
#include <stack>

class Vehicle;
class History;
class dbms;

class Vehicle
{
protected:
    std::string m_company;
    std::string m_modelName;
    std::vector<std::string> m_attributes; // only 3 attributes -> color, wheel size and milage
    long m_quantity;
    float m_cost;
    float m_profitMargin;

public:
    Vehicle();
    Vehicle(std::string const &company, std::string const &modelName, std::vector<std::string> const &attributes, long const &quantiy, float const &cost, float const &profitMargin);
    std::string getCompany() const;
    std::string getModelName() const;
    std::vector<std::string> getAttributes() const;
    long getQuantity() const;
    float getCost() const;
    float getProfitMargin() const;
    void setCompany();
    void setModel();
    void setCompany(std::string const &);
    void setModel(std::string const &);
    void setAttributes(std::vector<std::string> const &);
    void setQuantity(long const &);
    void setCost(float const &);
    void setProfitMargin(float const &);
    bool operator==(Vehicle const &other) const;
};

class History : public Vehicle
{
    friend class dbms;
    char m_deletiness = 0;
    std::list<Vehicle>::iterator m_absPos;
    std::list<Vehicle>::iterator m_oldPos;

public:
    History();
    History(std::list<Vehicle>::iterator, char, std::list<Vehicle>::iterator);
};

class dbms
{
protected:
    using DataIter = std::list<Vehicle>::iterator;
    using DataIterVec = std::vector<DataIter>;
    std::list<Vehicle> m_data;
    std::vector<History> m_undoHistory;
    std::vector<History> m_redoHistory;
    void pushToUndo();
    void pushToRedo();

public:
    DataIter insert(Vehicle const &, bool = 0);
    DataIterVec search(Vehicle const &);
    DataIterVec searchByCompany(std::string const &);
    DataIterVec searchByModel(std::string const &);
    DataIterVec searchByAttribute(std::vector<std::string> const &);
    DataIterVec searchByQuantity(long const &);
    DataIterVec searchByRange(float const &, float const &);
    DataIter edit(DataIter, Vehicle const &, bool = 0);
    DataIter editByCompany(DataIter, std::string const &);
    DataIter editByModel(DataIter, std::string const &);
    DataIter editByAttributes(DataIter, std::vector<std::string> const &);
    DataIter editByQuantity(DataIter, long const &);
    DataIter editProfitMargin(DataIter, float const &);
    DataIter toDelete(DataIter, bool = 0);
    DataIter begin();
    DataIter end();
    int undo();
    int redo();
};

/*
class Search : public Vehicle
{
public:
    bool searchByCompany(std::string, Vehicle[], int);
    bool searchByModel(std::string, Vehicle[], int);
    bool searchByAttributes(std::vector<std::string>, Vehicle[], int);
};
class Display : public Vehicle
{
public:
    void displayRecord(Vehicle *);
    void displayCompanies(Vehicle[], int);
    void displayCompanyAndQuantity(Vehicle[], int);
    void displayAll(Vehicle[], int);
};
class WriteToFile : public Vehicle
{
public:
    void writeToFile(Vehicle[]);
    void writeOneRecord(Vehicle);
};
class ReadFromFile : public Vehicle
{
public:
    void readFromFile();
};
class Delete : virtual public Search
{
protected:
    std::vector<std::string> delHistory;

public:
    void delByCompany(std::string, Vehicle[], int);
    void delByModel(std::string, Vehicle[], int);
};
class Update : virtual public Search
{
protected:
    std::vector<std::string> updateHistory;

public:
    void updateCompany();
    void updateModel();
    void updateQuantity();
    void updateAttributes();
};
class History : public Delete, public Update
{
public:
    void showDeleteHistory();
    void showUpdateHistory();
};
*/

#endif