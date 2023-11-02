// Tratamiento de argumentos para los parámetros de main()

#ifndef SIM_PROGARGS_HPP
#define SIM_PROGARGS_HPP

#include "fileio.hpp"
#include "io.hpp"

#include <gsl/gsl>
#include <iostream>
#include <span>

namespace Fluid {

class ArgumentParser {
  public:
    ArgumentParser(std::span<gsl::czstring> args);

    constexpr explicit operator bool() const { return error_ == 0; }

    constexpr int error() const { return error_; }

    constexpr int step_count() const { return nts_; }

    FileIO &file() { return file_; }

  private:
    int error_;
    int nts_;
    FileIO file_;
    void openFiles(std::span<gsl::zstring> args);

    template <typename... Args> constexpr void error(int code, Args... args) {
        error_ = code;
        IO::put_line(std::cerr, args...);
    }

    template <typename... Args> constexpr void check(int code, Args... args) {
        if (error_ == code) {
            IO::put_line(std::cerr, args...);
        }
    }
};

} // namespace Fluid
#endif // SIM_PROGARGS_HPP
