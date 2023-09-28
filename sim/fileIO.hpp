// define the file input and output
#ifndef __FILE_IO
#define __FILE_IO

#include <fstream>

namespace Fluid {
  class FileIO {
    public:
      template <typename String>
      constexpr FileIO(String input_name, String output_name)
        : input_file(input_name), output_file(output_name) { }

    private:
      int error_;
      std::ifstream input_file;
      std::ofstream output_file;
  };
}  // namespace Fluid

#endif
