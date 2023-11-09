#include "argumentparser.hpp"

#include "io.hpp"

namespace Fluid {

  ArgumentParser::ArgumentParser(std::span<gsl::czstring> args) {
    auto const size = args.size() - 1;
    if (size != 3) {  // Checks number of args
      error(-1, "Error: Invalid number of arguments: ", size, ".");
      return;
    }
    try {  // Checks that the amount of steps can be parsed as an integer
      steps_ = std::stoi(args[1]);
      // if not, gives the appropriate errors
    } catch (std::invalid_argument const &) {
      error(-1, "Error: time steps must be numeric.");
      return;
    } catch (std::out_of_range const &) { steps_ = -1; }
    if (steps_ < 0) {  // Checks that the parsed value is valid
      error(-2, "Error: Invalid number of time steps.");
      return;
    }
    // Tries opening and parsing the IO files, checking for errors
    file_.open(args[2], args[3]);
    error_ = file_.error();
    check(-3, "Error: Cannot open ", args[2], " for reading");
    check(-4, "Error: Cannot open ", args[3], " for writing");
  }

}  // namespace Fluid
