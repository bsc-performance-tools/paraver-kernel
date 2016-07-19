#ifndef LIBPARAVER_PARAVERTRACECONFIGGRAMMAR_H
#define LIBPARAVER_PARAVERTRACECONFIGGRAMMAR_H


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

namespace libparaver {

template<typename Actions>
class ParaverTraceConfigGrammar : public ::grammar<ParaverTraceConfigGrammar<Actions> > {
  protected:
    Actions & actions;

    mutable std::string str_value1;

    mutable std::string str_value2;

    mutable std::string str_value3;

    mutable int int_value1;

    mutable int int_value2;

    mutable int int_value3;

    mutable int int_value4;


  public:
    ParaverTraceConfigGrammar(Actions & actions);

    template<typename ScannerT>
    class definition {
      protected:
        rule<ScannerT> anychar_less_newline_p;

        rule<ScannerT> phrase;

        rule<ScannerT> pcfdata;

        rule<ScannerT> default_options;

        rule<ScannerT> states;

        rule<ScannerT> states_pair;

        rule<ScannerT> level;

        rule<ScannerT> units;

        rule<ScannerT> look_back;

        rule<ScannerT> speed;

        rule<ScannerT> flag_icons;

        rule<ScannerT> num_of_state_colors;

        rule<ScannerT> ymax_scale;

        rule<ScannerT> states_color;

        rule<ScannerT> states_color_entry;

        rule<ScannerT> default_semantic;

        rule<ScannerT> default_task_semantic_func;

        rule<ScannerT> default_thread_semantic_func;

        rule<ScannerT> event_type;

        rule<ScannerT> event_entry;

        rule<ScannerT> values;

        rule<ScannerT> values_entry;

        rule<ScannerT> gradient_color;

        rule<ScannerT> gradient_color_entry;

        rule<ScannerT> gradient_names;

        rule<ScannerT> gradient_names_entry;


      public:
        definition(const ParaverTraceConfigGrammar<Actions> & self);

        rule<ScannerT> const & start() const;

    };

};
template<typename Actions>
ParaverTraceConfigGrammar<Actions>::ParaverTraceConfigGrammar(Actions & actions):actions(actions) {
}

template<typename Actions>
template<typename ScannerT>
ParaverTraceConfigGrammar<Actions>::definition<ScannerT>::definition(const ParaverTraceConfigGrammar<Actions> & self) {
  Actions & actions = self.actions;

  anychar_less_newline_p
     = anychar_p - eol_p
     ;

  phrase
     = +anychar_less_newline_p
     ;

  pcfdata
     = +(states | states_color | event_type | gradient_color | gradient_names | default_options | default_semantic) >> end_p[bind(&Actions::commitEventTypes, ref(actions))]
     ;

  states
     = str_p("STATES") >> +space_p >> +(states_pair >> +space_p)
     ;

  states_pair
     = (int_p[assign_a(self.int_value1)]
     >> phrase[assign_a(self.str_value1)])[bind(&Actions::addState, ref(actions), ref(self.int_value1), ref(self.str_value1))]
     ;

  states_color
     = str_p("STATES_COLOR") >> +space_p >> +(states_color_entry >> +space_p)
     ;

  states_color_entry
     = (int_p[assign_a(self.int_value1)]
     >> '{' >> int_p[assign_a(self.int_value2)] >> ','
     >> int_p[assign_a(self.int_value3)] >> ','
     >> int_p[assign_a(self.int_value4)] >> '}'
        )[bind(&Actions::addStateColor, ref(actions), ref(self.int_value1), ref(self.int_value2), ref(self.int_value3), ref(self.int_value4))]
     ;

  gradient_color
     = str_p("GRADIENT_COLOR") >> +space_p >> +(gradient_color_entry >> +space_p)
     ;

  gradient_color_entry
     = (int_p[assign_a(self.int_value1)]
     >> '{' >> int_p[assign_a(self.int_value2)] >> ','
     >> int_p[assign_a(self.int_value3)] >> ','
     >> int_p[assign_a(self.int_value4)] >> '}'
        )[bind(&Actions::addGradientColor, ref(actions), ref(self.int_value1), ref(self.int_value2), ref(self.int_value3), ref(self.int_value4))]
     ;

  gradient_names
     = str_p("GRADIENT_NAMES") >> +space_p >> +(gradient_names_entry >> +space_p)
     ;

  gradient_names_entry
     = (int_p[assign_a(self.int_value1)] >> phrase[assign_a(self.str_value1)])
           [bind(&Actions::addGradientNames, ref(actions), ref(self.int_value1), ref(self.str_value1))]
     ;

  default_options
     = str_p("DEFAULT_OPTIONS") >> +space_p >>
        +(
           (level | units | look_back | speed | flag_icons | num_of_state_colors | ymax_scale) >> +space_p
        )
     ;
  level
     = str_p("LEVEL") >> (+alpha_p)[assign_a(actions.level)]
     ;
  units
     = str_p("UNITS") >> (+alpha_p)[assign_a(actions.units)]
     ;
  look_back
     = str_p("LOOK_BACK") >> int_p[assign_a(actions.look_back)]
     ;
  speed
     = str_p("SPEED") >> int_p[assign_a(actions.speed)]
     ;

  flag_icons
     = str_p("FLAG_ICONS") >>  (+alpha_p)[assign_a(actions.flag_icons)]
     ;

  num_of_state_colors
     = str_p("NUM_OF_STATE_COLORS") >> int_p[assign_a(actions.num_of_state_colors)]
     ;

  ymax_scale
     = str_p("YMAX_SCALE") >> int_p[assign_a(actions.ymax_scale)]
     ;

  default_semantic
     = str_p("DEFAULT_SEMANTIC") >> +eol_p >>
     +( default_task_semantic_func | default_thread_semantic_func ) >> *eol_p
     ;

  default_task_semantic_func
    = str_p("TASK_FUNC") >> phrase[assign_a(actions.default_task_semantic_func)]
    ;

  default_thread_semantic_func
    = str_p("THREAD_FUNC") >> phrase[assign_a(actions.default_thread_semantic_func)]
    ;


  event_type
     = str_p("EVENT_TYPE")[bind(&Actions::commitEventTypes, ref(actions))] >> +space_p >> +(event_entry >> +space_p) >> *(values)
     ;

  event_entry
     = (int_p[assign_a(self.int_value1)] >> int_p[assign_a(self.int_value2)]
     >> phrase[assign_a(self.str_value1)])
           [bind(&Actions::addEventType, ref(actions), ref(self.int_value1), ref(self.int_value2), ref(self.str_value1))]
     ;

  values
     = str_p("VALUES") >> +eol_p >> *(values_entry >> +eol_p)
     ;

  values_entry
     = (int_p[assign_a(self.int_value1)] >> phrase[assign_a(self.str_value1)])
           [bind(&Actions::addValues, ref(actions), ref(self.int_value1), ref(self.str_value1))]
     ;

  BOOST_SPIRIT_DEBUG_NODE(anychar_less_newline_p);
  BOOST_SPIRIT_DEBUG_NODE(phrase);
  BOOST_SPIRIT_DEBUG_NODE(pcfdata);

  BOOST_SPIRIT_DEBUG_NODE(event_type);
  BOOST_SPIRIT_DEBUG_NODE(event_entry);
  BOOST_SPIRIT_DEBUG_NODE(values);
  BOOST_SPIRIT_DEBUG_NODE(values_entry);

  BOOST_SPIRIT_DEBUG_NODE(states);
  BOOST_SPIRIT_DEBUG_NODE(states_pair);

  BOOST_SPIRIT_DEBUG_NODE(states_color);
  BOOST_SPIRIT_DEBUG_NODE(states_color_entry);

  BOOST_SPIRIT_DEBUG_NODE(gradient_color);
  BOOST_SPIRIT_DEBUG_NODE(gradient_color_entry);

  BOOST_SPIRIT_DEBUG_NODE(gradient_names);
  BOOST_SPIRIT_DEBUG_NODE(gradient_names_entry);

  BOOST_SPIRIT_DEBUG_NODE(default_semantic);
  BOOST_SPIRIT_DEBUG_NODE(default_task_semantic_func);
  BOOST_SPIRIT_DEBUG_NODE(default_thread_semantic_func);

  BOOST_SPIRIT_DEBUG_NODE(default_options);
  BOOST_SPIRIT_DEBUG_NODE(level);
  BOOST_SPIRIT_DEBUG_NODE(units);
  BOOST_SPIRIT_DEBUG_NODE(look_back);
  BOOST_SPIRIT_DEBUG_NODE(speed);
  BOOST_SPIRIT_DEBUG_NODE(flag_icons);
  BOOST_SPIRIT_DEBUG_NODE(num_of_state_colors);
  BOOST_SPIRIT_DEBUG_NODE(ymax_scale);
}

template<typename Actions>
template<typename ScannerT>
rule<ScannerT> const & ParaverTraceConfigGrammar<Actions>::definition<ScannerT>::start() const {
  return pcfdata;
}


} // namespace libparaver
#endif
