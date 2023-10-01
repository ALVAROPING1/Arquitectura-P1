// define the file input and output
#ifndef __FILE_IO
#define __FILE_IO

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
      // FIXME: Para que no se abra el archivo de salida, no uses listas de
      //        inicialización. Usa input_file_.open(input_name) y después
      //        comprueba su estatus.
      template <FileNameType S1, FileNameType S2>
      constexpr FileIO(S1 input_name, S2 output_name)
        : input_file_(input_name), output_file_(output_name) { }

      // bool operator to check if the files opened succesfully
      // XXX: operator bool no debería jamás devolver un entero, solo puede
      //      devolver un bool.
      //      El operador que me estabas sobreescribiendo era el operator!, no
      //      operator bool. Mejor llamarlo status y que devuelva el valor.
      //
      // XXX: Si el número de partículas es 0 o negativo, devuelve el código de
      //      error -5.
      //
      // FIXME: Si el archivo de entrada no se puede abrir y tampoco el de
      //        salida, se debe devolver -3 (primero se prueba el de salida)
      //
      // FIXME: Si no hay error, devuelve 0.
      //
      // FIXME: Haz que calcule el error_ en el constructor. Error no tiene
      // valor por defecto
      int status() {
        if (!input_file_) {
          error_ = -3;
        } else if (!output_file_) {
          error_ = -4;
        } else {
          error_ = 1;
        }
        return error_;
      }

      // XXX: Funciones a implementar, deserializar los datos para cargarlos y
      //      serializarlos para volcarlos en el archivo.

      void load(Grid & grid);
      void dump(Grid const & grid);

    private:
      int error_{};     // XXX: error_ no tiene valor por defecto
      std::ifstream input_file_;
      std::ofstream output_file_;
  };
}  // namespace Fluid

#endif
