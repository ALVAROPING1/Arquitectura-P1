#include "argumentparser.hpp"

#include "io.hpp"

namespace Fluid {

  ArgumentParser::ArgumentParser(std::span<gsl::czstring> args) {
    auto const size = args.size() - 1;
    if (size != 3) {  // Checks number of args
      IO::put_line(std::cerr, "Error: Invalid number of arguments: ", size, ".");
      std::exit(-1);
    }
    try {  // Checks that the amount of steps can be parsed as an integer
      steps_ = std::stoi(args[1]);
      // if not, gives the appropriate errors
    } catch (std::invalid_argument const &) {
      IO::put_line(std::cerr, "Error: time steps must be numeric.");
      std::exit(-1);
    } catch (std::out_of_range const &) { steps_ = -1; }
    if (steps_ < 0) {  // Checks that the parsed value is valid
      IO::put_line(std::cerr, "Error: Invalid number of time steps.");
      std::exit(-2);
    }
    // Tries opening and parsing the IO files, checking for errors
    file_.open(args[2], args[3]);
  }

}  // namespace Fluid
