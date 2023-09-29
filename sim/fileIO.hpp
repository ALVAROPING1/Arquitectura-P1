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
      // bool operator to check if the files opened succesfully
      int operator!() {
        if (!input_file){
          error_ = -3;          
        }       
        
        else if (!output_file)
        {
          error_ = -4;
        }
        else{
          error_ = 1;
        }
        return error_;

      }

    private:
      int error_;
      std::ifstream input_file;
      std::ofstream output_file;
       
  };
}  

#endif
