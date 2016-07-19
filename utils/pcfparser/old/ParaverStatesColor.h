#ifndef LIBPARAVER_PARAVERSTATESCOLOR_H
#define LIBPARAVER_PARAVERSTATESCOLOR_H


namespace libparaver {

class ParaverStatesColor {
  protected:
    int key;

    int color1;

    int color2;

    int color3;


  public:
    ParaverStatesColor(int key, int color1, int color2, int color3);

    virtual ~ParaverStatesColor();

    inline const int get_color1() const;

    inline const int get_color2() const;

    inline const int get_color3() const;

    inline const int get_key() const;

};
inline const int ParaverStatesColor::get_color1() const {
  return color1;
}

inline const int ParaverStatesColor::get_color2() const {
  return color2;
}

inline const int ParaverStatesColor::get_color3() const {
  return color3;
}

inline const int ParaverStatesColor::get_key() const {
  return key;
}


} // namespace libparaver
#endif
