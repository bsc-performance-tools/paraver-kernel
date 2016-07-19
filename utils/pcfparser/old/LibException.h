#ifndef LIBPARAVER_LIBEXCEPTION_H
#define LIBPARAVER_LIBEXCEPTION_H


#include <exception>
#include <string>

namespace libparaver {

class LibException : public std::exception {
  protected:
    std::string reason;

    std::string stack;


  public:
    LibException();

    LibException(std::string file, int line, std::string message);

    virtual ~LibException() throw();

    virtual const char * what() const throw();

    virtual const char * where() const throw();

};

} // namespace libparaver
#endif
