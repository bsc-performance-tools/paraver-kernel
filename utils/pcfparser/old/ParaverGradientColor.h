#ifndef LIBPARAVER_PARAVERGRADIENTCOLOR_H
#define LIBPARAVER_PARAVERGRADIENTCOLOR_H


namespace libparaver {

class ParaverGradientColor {
  protected:
    int key;

    int color1;

    int color2;

    int color3;


  public:
    ParaverGradientColor(int key, int color1, int color2, int color3);

    virtual ~ParaverGradientColor();

    inline const int get_key() const;

    inline const int get_color1() const;

    inline const int get_color2() const;

    inline const int get_color3() const;

};
inline const int ParaverGradientColor::get_key() const {
  return key;
}

inline const int ParaverGradientColor::get_color1() const {
  return color1;
}

inline const int ParaverGradientColor::get_color2() const {
  return color2;
}

inline const int ParaverGradientColor::get_color3() const {
  return color3;
}


} // namespace libparaver
#endif
