
template<class Iterator, class ActionHandler>
void ParaverTraceConfigGrammar<Iterator, ActionHandler>::test() {
    std::cout<<"Hola\n";
}

template<class Iterator, class ActionHandler>
ParaverTraceConfigGrammar<Iterator, ActionHandler>::ParaverTraceConfigGrammar(ActionHandler & handler):ParaverTraceConfigGrammar::base_type(pcfData, "pcfData") {

    /* Generic rules */
    numeric_index.name("numeric_index");
    numeric_index %= qi::lexeme[qi::int_];

    phrase.name("phrase");
    /* a % b is equal to "a >> *(b >> a)" */
    phrase = qi::lexeme[+(ascii::char_ - qi::eol)];

    /* PCF grammar */

    pcfData.name("pcfData");
    pcfData =
        qi::eps >
        +(
            default_options |
            default_semantic |
            states_colors |
            states |
            gradient_colors |
            gradient_names |
            +event_type
        ) >
        qi::eoi
        ;

    /* DEFAULT_OPTIONS rules */

    default_options.name("DEFAULT_OPTIONS");
    default_options =
        qi::lit("DEFAULT_OPTIONS") >
        +(
            level |
            units |
            look_back |
            speed |
            flag_icons |
            num_of_state_colors |
            ymax_scale
        )
        ;

    level.name("LEVEL");
    level = qi::lit("LEVEL") > phrase[phoenix::bind(&ActionHandler::setLevel, phoenix::ref(handler), qi::_1)];

    units.name("UNITS");
    units = qi::lit("UNITS") > phrase[phoenix::bind(&ActionHandler::setUnits, phoenix::ref(handler), qi::_1)];

    look_back.name("LOOK_BACK");
    look_back = qi::lit("LOOK_BACK") > qi::int_[phoenix::bind(&ActionHandler::setLook_back, phoenix::ref(handler), qi::_1)];

    speed.name("SPEED");
    speed = qi::lit("SPEED") > qi::int_[phoenix::bind(&ActionHandler::setSpeed, phoenix::ref(handler), qi::_1)];

    flag_icons.name("FLAG_ICONS");
    flag_icons = qi::lit("FLAG_ICONS") > phrase[phoenix::bind(&ActionHandler::setFlag_icons, phoenix::ref(handler), qi::_1)];

    num_of_state_colors.name("NUM_OF_STATE_COLORS");
    num_of_state_colors = qi::lit("NUM_OF_STATE_COLORS") > qi::int_[phoenix::bind(&ActionHandler::setNum_of_state_colors, phoenix::ref(handler), qi::_1)];

    ymax_scale.name("YMAX_SCALE");
    ymax_scale = qi::lit("YMAX_SCALE") > qi::int_[phoenix::bind(&ActionHandler::setYmax_scale, phoenix::ref(handler), qi::_1)];

    /* DEFAULT_SEMANTIC rules */
    default_semantic.name("DEFAULT_SEMANTIC");
    default_semantic = qi::lit("DEFAULT_SEMANTIC") > thread_func;

    thread_func.name("THREAD_FUNC");
    thread_func =
        qi::lit("THREAD_FUNC") > phrase;//[phoenix::bind(&ActionHandler::setThread_func, phoenix::ref(handler), qi::_1)];

    /* STATES rules */

    states.name("states");
    states =
        qi::lit("STATES") >
        +(state)
        ;

    state.name("state");
    state = (numeric_index > phrase)[phoenix::bind(&ActionHandler::addState, phoenix::ref(handler),qi::_1, qi::_2)];

    /* STATES_COLOR rules */

    states_colors.name("STATES_COLOR");
    states_colors = qi::lit("STATES_COLOR") > +state_color;

    state_color.name("index   {r,g,b}");
    state_color = (numeric_index >
                   '{' > red >
                   ',' > green >
                   ',' > blue >
                   '}')[phoenix::bind(&ActionHandler::addStateColor, phoenix::ref(handler), qi::_1, qi::_2, qi::_3, qi::_4)];

    red.name("Red color 0-255");
    red %= qi::lexeme[qi::short_];

    green.name("Green color 0-255");
    green %= qi::lexeme[qi::short_];

    blue.name("Blue color 0-255");
    blue %= qi::lexeme[qi::short_];

    /* GRADIENT_COLOR */

    gradient_colors.name("GRADIENT_COLORS");
    gradient_colors = qi::lit("GRADIENT_COLOR") > +gradient_color;

    gradient_color.name("index {r,g,b}");
    gradient_color = (numeric_index >
                      '{' > red >
                      ',' > green >
                      ',' > blue >
                      '}')[phoenix::bind(&ActionHandler::addGradientColor, phoenix::ref(handler), qi::_1, qi::_2, qi::_3, qi::_4)];

    /* GRADIENT_NAMES */

    gradient_names.name("index phrase");
    gradient_names =
        qi::lit("GRADIENT_NAMES") >
        +(numeric_index > phrase)[
            phoenix::bind(&ActionHandler::addGradientName, phoenix::ref(handler), qi::_1, qi::_2)
        ]
        ;

    /* EVENT_TYPE rules */

    event_type.name("EVENT_TYPE");
    event_type =
        qi::lit("EVENT_TYPE") >
        +(numeric_index > numeric_index > phrase)[
            phoenix::push_back(qi::_a,phoenix::new_<typename ActionHandler::EventType>(qi::_1, qi::_2, qi::_3))
        ] >
        -(
            qi::lit("VALUES")[
                qi::_b = phoenix::new_<typename ActionHandler::EventValues>()
            ] >
            +(numeric_index > phrase)[
                phoenix::bind(&ActionHandler::EventValues::addValue, qi::_b, qi::_1, qi::_2) //qi::_b is a reference
            ]
        )[
            //qi::_a -> std::vector<typename ActionHandler::EventType *>
            //qi::_b -> typename ActionHandler::EventValues *
            phoenix::setEventValues(qi::_a, qi::_b) // Lazy function defined in ParaverTraceConfigGrammar.h
        ] >
        qi::eps[
            phoenix::bind(&ActionHandler::addEventTypes, phoenix::ref(handler), qi::_a)
        ]
        ;

    /* DEBUGGING */

    if (handler.getDebug()) {
        qi::debug(pcfData);
        qi::debug(default_options);
        qi::debug(level);
        qi::debug(units);
        qi::debug(look_back);
        qi::debug(speed);
        qi::debug(flag_icons);
        qi::debug(num_of_state_colors);
        qi::debug(ymax_scale);
        qi::debug(default_semantic);
        qi::debug(thread_func);
        qi::debug(states);
        qi::debug(state);
        qi::debug(states_colors);
        qi::debug(state_color);
        qi::debug(red);
        qi::debug(green);
        qi::debug(blue);
        qi::debug(gradient_colors);
        qi::debug(gradient_color);
        qi::debug(gradient_names);
        qi::debug(numeric_index);
        qi::debug(phrase);
    }
}
