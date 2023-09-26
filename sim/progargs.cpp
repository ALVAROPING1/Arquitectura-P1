#include <iostream>
#include "progargs.hpp"

namespace Fluid {

  ArgumentParser::ArgumentParser(std::span<gsl::zstring> args) : error_{0} {
    const auto size = args.size() - 1;
    if (size != 3) {
      error_ = -1;
      std::cerr << "Error: Invalid number of arguments: " << size << ".\n";
      return;
    }
    try {
      nts_ = std::stoi(args[1]);
    } catch (std::invalid_argument const&) {
      error_ = -1;
      std::cerr << "Error: time steps must be numeric.\n";
      return;
    } catch (std::out_of_range const&) {
      nts_ = -1;
    }
    if (nts_ < 0) {
      error_ = -2;
      std::cerr << "Error: Invalid number of time steps.\n";
      return;
    }
    openFiles(args);
  }

  void ArgumentParser::openFiles(std::span<gsl::zstring> args) {
    inputfile_.open(args[2]);
    outputfile_.open(args[3]);
    if (!inputfile_) {
      error_ = -3;
      std::cerr << "Error: Cannot open " << args[2] << " for reading\n";
    } else if (!outputfile_) {
      error_ = -4;
      std::cerr << "Error: Cannot open " << args[3] << " for writing\n";
    }
  }

}
