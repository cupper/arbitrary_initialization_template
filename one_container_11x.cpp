#include <iostream>
#include <string>
#include <boost/mpl/set.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/bool.hpp>

using namespace std;

#define static_type(name) \
    static const std::string& type() \
    { \
        static std::string type_(name); \
        return type_; \
    } \

struct deferred_plugin
{ static_type("deferred"); };

struct deadline_plugin
{ static_type("deadline"); };

struct disable_plugin
{ static_type("disable"); };

template<class T, class Deferred, class Deadline>
struct some_container
{
    static const std::string& type()
    {
        static std::string type_("some_container<" + 
                                    Deferred::type() + ", " +
                                    Deadline::type() + ">");
        return type_;
    }
};

template <class Plugin,  class IsSet>
struct get_plugin_impl;

template <class Plugin>
struct get_plugin_impl<Plugin, boost::mpl::false_>
{
    typedef disable_plugin type;
};

template <class Plugin>
struct get_plugin_impl<Plugin, boost::mpl::true_>
{
    typedef Plugin type;
};

template <class List, class Plugin>
struct get_plugin
{
    typedef typename get_plugin_impl<Plugin,
        typename boost::mpl::has_key<List, Plugin>::type>::type type;
};

template <class List>
struct get_plugins
{
    typedef typename get_plugin<List, deferred_plugin>::type deferred;
    typedef typename get_plugin<List, deadline_plugin>::type deadline;
};

template<class T, typename... Plugins>
struct container
{
    typedef boost::mpl::set<Plugins...> plugin_list;
    typedef get_plugins<plugin_list> plugs;
    
    typedef typename plugs::deferred deferred;
    typedef typename plugs::deadline deadline;
    
    typedef some_container<T, deferred, deadline> type; 
};

int main()
{
   cout << "Hello World" << endl; 
  
   cout << container<int>::type::type() << std::endl;
   cout << container<int, deadline_plugin>::type::type() << std::endl;
   cout << container<int, deferred_plugin>::type::type() << std::endl;
   cout << container<int, deferred_plugin, deadline_plugin>::type::type() << std::endl;
   cout << container<int, deadline_plugin, deferred_plugin>::type::type() << std::endl;
   
   return 0;
}

