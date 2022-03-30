#pragma once

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

    virtual ~LibException() noexcept;

    virtual const char * what() const noexcept;

    virtual const char * where() const noexcept;

};

} // namespace libparaver
