#ifndef MENU_H
#define MENU_H
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>

template <typename _T1, typename _T2>
void assignment(_T1& t1, _T2 const& t2)
{
    return;
}
template <typename _T>
void assignment(_T& t1, _T& t2)
{
    t1 = t2;
}

template <typename... _Sigs>
class Menu;

template <typename _Sig>
class MenuItem
{
};

template <typename _Ret, typename... _Args>
class MenuItem<_Ret(_Args...)>
{
protected:
    std::string m_command, m_docs;
    std::function<_Ret(_Args...)> m_function;

public:
    MenuItem(std::function<_Ret(_Args...)> const& function, std::string const& command, std::string const& docs)
        : m_function(function), m_command(command), m_docs(docs) {};
    virtual _Ret operator()(_Args... args)
    {
        return m_function(args...);
    }
    virtual _Ret call(_Args... args)
    {
        return m_function(args...);
    }
    virtual std::string getHelp()
    {
        return m_docs;
    }
    virtual std::string getCommand()
    {
        return m_command;
    }
    virtual ~MenuItem() = default;
};

template <typename... _Args>
class MenuItem<void(_Args...)>
{
protected:
    std::string m_command, m_docs;
    std::function<void(_Args...)> m_function;

public:
    MenuItem(std::function<void(_Args...)> const& function, std::string const& command, std::string const& docs)
        : m_function(function), m_command(command), m_docs(docs) {};
    virtual int operator()(_Args... args)
    {
        m_function(args...);
        return 1;
    }
    virtual int call(_Args... args)
    {
        m_function(args...);
        return 1;
    }
    virtual std::string getHelp()
    {
        return m_docs;
    }
    virtual std::string getCommand()
    {
        return m_command;
    }
    virtual ~MenuItem() = default;
};

template <typename _Ret, typename... _Args>
auto makeMenuItem(std::function<_Ret(_Args...)> const& function, std::string const& command, std::string const& docs)
{
    return std::make_shared<MenuItem<_Ret(_Args...)>>(function, command, docs);
}

template <typename... _Sigs>
class ChildMenu : public MenuItem<int()>
{
protected:
    Menu<_Sigs...> m_child;

public:
    int operator()() override
    {
        return m_child.input();
    }
    int call() override
    {
        return m_child.input();
    }
    Menu<_Sigs...> const& getChild()
    {
        return m_child;
    }
    ChildMenu(Menu<_Sigs...> const& child, std::string const& command, std::string const& docs)
        : MenuItem(std::function<int()>([]
                       {
                           std::cout << "Hello";
                           return 0;
                       }),
            command, docs),
          m_child(child)
    {
    }
};

template <typename... _Sigs>
auto makeChild(Menu<_Sigs...> const& child, std::string const& command, std::string const& docs)
{
    return std::make_shared<ChildMenu<_Sigs...>>(child, command, docs);
}

template <typename... _Sigs>
class Menu
{
    using Items = std::tuple<std::shared_ptr<MenuItem<_Sigs>>...>;
    std::string m_name;
    std::string m_help;
    std::string m_inv_command_msg = "";
    Items m_items;
    std::function<std::string(std::string, std::string)> m_help_format = [](std::string command, std::string docs)
    {
        std::stringstream ss;
        ss << "Command: " << command << '\n'
           << "    " << docs << '\n';
        return ss.str();
    };
    std::function<void()> m_input_func = []() {};
    template <std::size_t _Iter = 0>
    constexpr void init()
    {
        m_help += m_help_format(std::get<_Iter>(m_items)->getCommand(), std::get<_Iter>(m_items)->getHelp());
        if constexpr (_Iter + 1 != sizeof...(_Sigs))
            init<_Iter + 1>();
        else
        {
            m_help += m_help_format("up", "Go up a menu or quit");
            m_help += m_help_format("help or h", "Print help");
        };
    }

public:
    Menu(
        std::string const& name, std::shared_ptr<MenuItem<_Sigs>> const&... args, std::function<void()> input_func = []() {},
        std::string const& inv_command_msg = "Invalid command, type 'help' to get a list of all valid commands")
        : m_name(name), m_items(std::make_tuple(args...)), m_inv_command_msg(inv_command_msg), m_input_func(input_func)
    {
        init();
    };
    Menu(
        std::string const& name, std::shared_ptr<MenuItem<_Sigs>> const&... args, std::function<std::string(std::string, std::string)> const& help_format,
        std::function<void()> input_func = []() {},
        std::string const& inv_command_msg = "Invalid command, type 'help' to get a list of all valid commands")
        : m_name(name), m_items(std::make_tuple(args...)), m_help_format(help_format), m_inv_command_msg(inv_command_msg)
    {
        init();
    };
    void help() const
    {
        std::cout << m_help;
    }
    template <std::size_t _Iter = 0, typename _Arg = int>
    int exec(std::string const& command, _Arg& arg) const
    {
        if (std::get<_Iter>(m_items)->getCommand() == command)
        {
            std::get<_Iter>(m_items);
            auto res = std::get<_Iter>(m_items)->call();
            assignment(arg, res);
            return _Iter;
        }
        if constexpr (_Iter + 1 != sizeof...(_Sigs))
            return exec<_Iter + 1, _Arg>(command, arg);
        else
            return _Iter + 1;
    }
    template <std::size_t _Iter = 0, typename _Ret>
    constexpr _Ret find(std::string const& command) const
    {
        if (std::get<_Iter>(m_items)->getCommand() == command)
        {
            return std::get<_Iter>(m_items);
        }
        if constexpr (_Iter + 1 != sizeof...(_Sigs))
        {
            return std::function<int()>([]()
                { return 0; });
        }
        else
        {
            find<_Iter + 1>();
        }
    }
    template <bool redo = 1, typename _Arg = int>
    int input(_Arg& arg) const
    {
        m_input_func();
        int retVal = 0;
        do
        {
            std::string in;
            std::cout << m_name << "> ";
            std::cin >> in;
            if (in == "up")
                return -1;
            if (in == "help" || in == "h")
            {
                help();
                retVal = 0;
                continue;
            }
            int it = exec(in, arg);
            if (it >= std::tuple_size<decltype(m_items)>::value)
            {
                std::cout << m_inv_command_msg << '\n';
                if constexpr (!redo)
                    return 0;
                else
                    retVal = 0;
            }
            else
                retVal = 1;
        } while (redo);
        return retVal;
    }
    template <bool redo = 1>
    int input() const
    {
        int arg = 0;
        return input(arg);
    }

    Items const& getItems() const
    {
        return m_items;
    }
};

template <typename... _Sigs>
Menu<_Sigs...> makeMenu(
    std::string const& name,
    std::string const& inv_command_msg,
    std::function<void()> input_func,
    std::shared_ptr<MenuItem<_Sigs>> const&... mitems)
{
    return Menu<_Sigs...>(name, mitems..., input_func, inv_command_msg);
}
template <typename... _Sigs>
Menu<_Sigs...> makeMenu(
    std::string const& name,
    std::function<std::string(std::string, std::string)> const& help_format,
    std::string const& inv_command_msg,
    std::function<void()> input_func,
    std::shared_ptr<MenuItem<_Sigs>> const&... mitems)
{
    return Menu<_Sigs...>(name, mitems..., help_format, input_func, inv_command_msg);
}
template <typename... _Sigs>
Menu<_Sigs...> makeMenu(
    std::string const& name,
    std::shared_ptr<MenuItem<_Sigs>> const&... mitems)
{
    return Menu<_Sigs...>(name , mitems...);
}
template <typename... _Sigs>
Menu<_Sigs...> makeMenu(
    std::string const& name,
    std::function<std::string(std::string, std::string)> const& help_format,
    std::shared_ptr<MenuItem<_Sigs>> const&... mitems)

{
    return Menu<_Sigs...>(name , mitems... , help_format);
}
template <typename... _Sigs>
Menu<_Sigs...> makeMenu(
    std::string const& name,
    std::string const& inv_command_msg,
    std::shared_ptr<MenuItem<_Sigs>> const&... mitems)
{
    return Menu<_Sigs...>(name , mitems... , std::function<void()>([](){}), inv_command_msg);
}
template <typename... _Sigs>
Menu<_Sigs...> makeMenu(
    std::string const& name,
    std::function<std::string(std::string, std::string)> const& help_format,
    std::string const &inv_command_msg,
    std::shared_ptr<MenuItem<_Sigs>> const&... mitems)

{
    return Menu<_Sigs...>(name , mitems... , help_format, std::function<void()>([](){}), inv_command_msg);
}

#endif
