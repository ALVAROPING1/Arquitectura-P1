// define the file input and output
#ifndef FILE_IO_HPP
#define FILE_IO_HPP

#include "grid.hpp"

#include <concepts>
#include <fstream>

namespace Fluid {

  // Este concept especifica qué tipos datos se pueden utilizar para abrir
  // un archivo en modo lectura/escritura.
  template <typename T>
  concept FileNameType = requires(T t) {
    std::declval<std::ifstream>().open(t);  // Se puede abrir para lectura
    std::declval<std::ofstream>().open(t);  // Se puede abrir para escritura
  };

  class FileIO {
    public:
      template <FileNameType S1, FileNameType S2>
      void open(S1 input_name, S2 output_name) {
        input_file_.open(input_name, std::ios::binary);
        if (not input_file_) {
          error_ = -3;
          return;
        }
        output_file_.open(output_name, std::ios::binary);
        if (not output_file_) {
          error_ = -4;
          return;
        }
        load_header();
      }

      constexpr int error() const { return error_; }

      constexpr operator bool() const { return error_ == 0; }

      void load(Grid & grid);
      void dump(Grid const & grid);

      constexpr double ppm() const { return ppm_; }

      constexpr std::size_t np() const { return np_; }

    private:
      int error_{};
      std::ifstream input_file_;
      std::ofstream output_file_;
      int np_{};
      double ppm_{};

      void load_header();
  };
}  // namespace Fluid

#endif  // FILE_IO_HPP
