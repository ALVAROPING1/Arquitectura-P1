#include "argumentparser.hpp"

#include "io.hpp"

namespace Fluid {

  ArgumentParser::ArgumentParser(std::span<gsl::zstring> args) : error_{0} {
    auto const size = args.size() - 1;
    if (size != 3) {
      error(-1, "Error: Invalid number of arguments: ", size, ".");
      return;
    }
    try {
      nts_ = std::stoi(args[1]);
    } catch (std::invalid_argument const &) {
      error(-1, "Error: time steps must be numeric.");
      return;
    } catch (std::out_of_range const &) { nts_ = -1; }
    if (nts_ < 0) {
      error(-2, "Error: Invalid number of time steps.");
      return;
    }
    file_.open(args[2], args[3]);
    error_ = file_.error();
    check(-3, "Error: Cannot open ", args[2], " for reading");
    check(-4, "Error: Cannot open ", args[3], " for writing");
  }

}  // namespace Fluid
