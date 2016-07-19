#ifndef LIBPARAVER_PARAVERTRACECONFIG_H
#define LIBPARAVER_PARAVERTRACECONFIG_H


//#define BOOST_SPIRIT_DEBUG
#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_assign_actor.hpp>
#include <boost/spirit/include/classic_increment_actor.hpp>
#include <boost/bind.hpp>
#include <boost/spirit/include/phoenix1_primitives.hpp>
#include <boost/spirit/include/phoenix1_operators.hpp>
#include <boost/spirit/include/classic_insert_at_actor.hpp>
#include <boost/spirit/include/classic_file_iterator.hpp>
#include <boost/spirit/include/classic_push_back_actor.hpp>
#include <boost/spirit/include/classic_functor_parser.hpp>
using BOOST_SPIRIT_CLASSIC_NS::grammar;
using BOOST_SPIRIT_CLASSIC_NS::rule;
using BOOST_SPIRIT_CLASSIC_NS::space_p;
using BOOST_SPIRIT_CLASSIC_NS::end_p;
using BOOST_SPIRIT_CLASSIC_NS::anychar_p;
using BOOST_SPIRIT_CLASSIC_NS::eol_p;
using BOOST_SPIRIT_CLASSIC_NS::lexeme_d;
using BOOST_SPIRIT_CLASSIC_NS::alnum_p;
using BOOST_SPIRIT_CLASSIC_NS::alpha_p;
using BOOST_SPIRIT_CLASSIC_NS::int_p;
using BOOST_SPIRIT_CLASSIC_NS::real_p;
using BOOST_SPIRIT_CLASSIC_NS::str_p;
using BOOST_SPIRIT_CLASSIC_NS::ch_p;
using BOOST_SPIRIT_CLASSIC_NS::uint_parser;
using BOOST_SPIRIT_CLASSIC_NS::functor_parser;
using BOOST_SPIRIT_CLASSIC_NS::assign_a;
using BOOST_SPIRIT_CLASSIC_NS::increment_a;
using BOOST_SPIRIT_CLASSIC_NS::push_back_a;
using BOOST_SPIRIT_CLASSIC_NS::parse_info;
using BOOST_SPIRIT_CLASSIC_NS::file_iterator;
using BOOST_SPIRIT_CLASSIC_NS::scanner;
using boost::ref;
using boost::bind;
using phoenix::var;

#include <string>
#include <vector>
#include <iostream>

namespace libparaver { class ParaverEventValue; }
namespace libparaver { class ParaverState; }
namespace libparaver { class ParaverStatesColor; }
namespace libparaver { class ParaverEventType; }
namespace libparaver { class ParaverGradientColor; }
namespace libparaver { class ParaverGradientNames; }
namespace libparaver { class LibException; }
namespace libparaver { template<typename Actions> class ParaverTraceConfigGrammar; }

namespace libparaver {

class ParaverTraceConfig {
  public:
    typedef char char_t;

    typedef file_iterator<char_t> iterator_t;

    typedef scanner<iterator_t> scanner_t;

    typedef rule<scanner_t> rule_t;


  protected:
    std::string pcfFile;


  public:
    std::string level;

    std::string units;

    int look_back;

    int speed;

    std::string flag_icons;

    int num_of_state_colors;

    int ymax_scale;

    // vector<string> level_func;  ?
    std::string default_task_semantic_func;
    std::string default_thread_semantic_func;

  protected:
    ParaverEventValue * eventValue;

    std::vector<int> i_eventTypes;

    std::vector<ParaverState *> states;

    std::vector<ParaverStatesColor *> statesColor;

    std::vector<ParaverEventType *> eventTypes;

    std::vector<ParaverEventType *> currentEvents;

    std::vector<ParaverGradientColor *> gradientColors;

    std::vector<ParaverGradientNames *> gradientNames;


  public:
    ParaverTraceConfig(std::string pcfFile);

    virtual ~ParaverTraceConfig();

    inline const std::string get_pcfFile() const;

    inline const std::string get_level() const;

    inline const std::string get_default_thread_semantic_func() const;
    inline const std::string get_default_task_semantic_func() const;

    void parse();

    ParaverEventType * getEventType(const std::string eventType);

    ParaverEventType * getEventType(int eventType);

    int getNumValues();

    int getValue();

    void addState(const int key, const std::string value);

    void addStateColor(const int key, const int red, const int green, int blue);

    void addEventType(const int color, const int key, const std::string description);

    void addGradientColor(const int key, const int red, const int green, const int blue);

    void addGradientNames(const int key, const std::string value);

    void commitEventTypes();

    void addValues(const int key, const std::string value);

    inline const std::vector<ParaverEventType *> & get_eventTypes() const;

    inline const std::vector<int> & get_i_eventTypes() const;

    inline const std::vector<ParaverStatesColor *> & get_statesColor() const;

    inline const std::vector<ParaverGradientColor *> & get_gradientColors() const;

    inline const std::vector<ParaverState *> & get_states() const;

    friend std::ostream & operator<<(std::ostream & os, const ParaverTraceConfig & ptraceConfig);

};
inline const std::string ParaverTraceConfig::get_pcfFile() const {
  return pcfFile;
}

inline const std::string ParaverTraceConfig::get_level() const {
  return level;
}

inline const std::string ParaverTraceConfig::get_default_thread_semantic_func() const {
  return default_thread_semantic_func;
}

inline const std::string ParaverTraceConfig::get_default_task_semantic_func() const {
  return default_task_semantic_func;
}

inline const std::vector<ParaverEventType *> & ParaverTraceConfig::get_eventTypes() const {
  return eventTypes;
}

inline const std::vector<int> & ParaverTraceConfig::get_i_eventTypes() const {
  return i_eventTypes;
}

inline const std::vector<ParaverStatesColor *> & ParaverTraceConfig::get_statesColor() const {
  return statesColor;
}

inline const std::vector<ParaverGradientColor *> & ParaverTraceConfig::get_gradientColors() const {
  return gradientColors;
}

inline const std::vector<ParaverState *> & ParaverTraceConfig::get_states() const {
  return states;
}

} // namespace libparaver
#endif
