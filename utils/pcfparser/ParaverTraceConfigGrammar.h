#ifndef PARAVER_TRACE_CONFIG_GRAMMAR_H
#define PARAVER_TRACE_CONFIG_GRAMMAR_H

#include <iostream>
#include <sstream>
#include <string>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
//#include <boost/spirit/home/phoenix/container.hpp>
#include <boost/spirit/include/phoenix_container.hpp>

#include <boost/spirit/include/phoenix_function.hpp>

/* boost::bind/val/ref stuff */
#include <boost/spirit/include/lex_lexertl.hpp>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phoenix = boost::phoenix;
namespace classic = boost::spirit::classic;

#ifndef SKIPPER
#define SKIPPER qi::space | qi::eol
#endif


namespace boost {
namespace phoenix
{
struct setEventValues_Impl
{
    //template <typename C, typename F>
    template <typename C>
    struct result
    {
        typedef void type;
    };

    template <typename C, typename F>
    void operator()(C &c, F * f) const
    {
        typename C::const_iterator begin = c.begin();
        typename C::const_iterator end = c.end();
        while (begin != end) {
            (*begin)->setEventValues(f);
            begin++;
        }
    }
};
function<setEventValues_Impl> const setEventValues = setEventValues_Impl();
}
}

namespace libparaver {

#ifdef WIN32
typedef BOOST_TYPEOF(SKIPPER) skipper_type_;
#define skipper_type skipper_type_
#else
typedef BOOST_TYPEOF(SKIPPER) skipper_type;
#endif

template<class Iterator, class ActionHandler>
class ParaverTraceConfigGrammar : public qi::grammar<Iterator, skipper_type> {
private:
    qi::rule<Iterator, skipper_type> pcfData;

    qi::rule<Iterator, skipper_type> states;

    qi::rule<Iterator, skipper_type> state;

    qi::rule<Iterator, int(), skipper_type> numeric_index;

    qi::rule<Iterator, std::string(), skipper_type> phrase;

    qi::rule<Iterator, skipper_type> default_options;

    qi::rule<Iterator, std::string(), skipper_type> level;

    qi::rule<Iterator, std::string(), skipper_type> units;

    qi::rule<Iterator, int(), skipper_type> look_back;

    qi::rule<Iterator, int(), skipper_type> speed;

    qi::rule<Iterator, std::string(), skipper_type> flag_icons;

    qi::rule<Iterator, int(), skipper_type> num_of_state_colors;

    qi::rule<Iterator, int(), skipper_type> ymax_scale;

    qi::rule<Iterator, skipper_type> default_semantic;

    qi::rule<Iterator, std::string(), skipper_type> thread_func;

    qi::rule<Iterator, skipper_type> states_colors;

    qi::rule<Iterator, skipper_type> state_color;

    qi::rule<Iterator, skipper_type> color;

    qi::rule<Iterator, int(), skipper_type> red;

    qi::rule<Iterator, int(), skipper_type> blue;

    qi::rule<Iterator, int(), skipper_type> green;

    qi::rule<Iterator, skipper_type> gradient_colors;

    qi::rule<Iterator, skipper_type> gradient_color;

    qi::rule<Iterator, skipper_type> gradient_names;

    // NOTE: This rule need more tthan 128 deep level -ftemplate-depth-256
    qi::rule
    <
    Iterator,
    qi::locals<
    std::vector<typename ActionHandler::EventType *>,
    typename ActionHandler::EventValues *
    >,
    skipper_type
    > event_type;
public:
    ParaverTraceConfigGrammar(ActionHandler & handler);
    void test();
};

#include "ParaverTraceConfigGrammar.ipp"
}
#endif
