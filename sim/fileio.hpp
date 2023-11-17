// define the file input and output
#ifndef FILE_IO_HPP
#define FILE_IO_HPP

#include "grid.hpp"
#include "io.hpp"

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

  // Objeto que gestiona los archivos de IO
  class FileIO {
    public:
      /// Crea el objeto y abre los archivos de IO
      /// @param input_name
      /// Nombre del archivo de entrada
      /// @param output_name
      /// Nombre del archivo de salida
      template <FileNameType S1, FileNameType S2>
      FileIO(S1 input_name, S2 output_name) {
        // Input file
        input_file_.open(input_name, std::ios::binary);
        if (not input_file_) {
          IO::put_line(std::cerr, "Error: Cannot open ", input_name, " for reading");
          std::exit(-3);
        }
        // Output file
        output_file_.open(output_name, std::ios::binary);
        if (not output_file_) {
          IO::put_line(std::cerr, "Error: Cannot open ", output_name, " for writing");
          std::exit(-4);
        }
        // Parses the input file header
        load_header();
      }

      /// Carga el archivo de entrada en la malla
      void load(Grid & grid);
      /// Escribe la malla en el archivo de salida
      void dump(Grid const & grid);

      /// Devuelve las partículas por metro
      constexpr double ppm() const { return ppm_; }

      /// Devuelve el número de partículas
      constexpr std::size_t np() const { return np_; }

    private:
      std::ifstream input_file_;   // Archivo de entrada
      std::ofstream output_file_;  // Archivo de salida
      int np_{};                   // Número de partículas
      double ppm_{};               // Partículas por metro

      /// Carga la cabecera del archivo de entrada
      void load_header();
  };
}  // namespace Fluid

#endif  // FILE_IO_HPP
