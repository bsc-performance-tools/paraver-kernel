/* Color class */

inline int UIParaverTraceConfig::Color::getRed() const {
    return red;
}

inline int UIParaverTraceConfig::Color::getGreen() const {
    return green;
}

inline int UIParaverTraceConfig::Color::getBlue() const {
    return blue;
}

/* EventType class */

inline int UIParaverTraceConfig::EventType::getColor() const {
    return color;
}

inline int UIParaverTraceConfig::EventType::getKey() const {
    return key;
}

inline const std::string UIParaverTraceConfig::EventType::getDescription() const {
    return descr;
}

