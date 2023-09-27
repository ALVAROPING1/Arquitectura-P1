// define the file input and output
#ifndef __FILE_IO
#define __FILE_IO

#include <gsl/gsl>

#include <fstream>
#include "progargs.hpp"


namespace Fluid{    
    class FileIO{
    public:
        template <typename String> 
        constexpr FileIO(String input_name, String output_name): 
        std::ifstream input_file{input_name}, std::ofstream output_file{output_name}
        {}

    private:
        int error_;
    };    
}

#endif