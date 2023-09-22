// Tratamiento de argumentos para los parámetros de main()

#ifndef __SIM_PROGARGS_HPP
#define __SIM_PROGARGS_HPP

#include <string>
#include <fstream>
#include <span>
#include <utility>
#include <gsl/gsl>

namespace Fluid {

  class ArgumentParser {
    public:
      ArgumentParser(std::span<gsl::zstring> args);
      constexpr explicit operator bool () const { return error_ != 0; }
      constexpr int error () const { return error_; };

      ~ArgumentParser() {
        if (inputfile_.is_open()) inputfile_.close();
        if (outputfile_.is_open()) outputfile_.close();
      }

      ArgumentParser (ArgumentParser const&) = delete; // Can't copy file stream
      ArgumentParser& operator=(ArgumentParser const&) = delete;
      ArgumentParser (ArgumentParser && other)
        : error_{std::exchange(other.error_, 0)}
        , nts_{std::exchange(other.nts_, 0)}
        , inputfile_{std::move(other.inputfile_)}
        , outputfile_{std::move(other.outputfile_)} {}
      ArgumentParser& operator=(ArgumentParser && other) {
        error_ = std::exchange(other.error_, 0);
        nts_ = std::exchange(other.nts_, 0);
        inputfile_ = std::move(other.inputfile_);
        outputfile_ = std::move(other.outputfile_);
        return *this;
      }

    private:
      int error_;
      int nts_;
      std::ifstream inputfile_;
      std::ofstream outputfile_;

      void openFiles (std::span<gsl::zstring> args);
  };

}
#endif//__SIM_PROGARGS_HPP
