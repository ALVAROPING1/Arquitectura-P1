#include "fileio.hpp"

#include "io.hpp"
#include "vector3.hpp"

#include <istream>
#include <ostream>

namespace Fluid {

  constexpr int particle_count_error = -5;

  void FileIO::load(Grid & grid) {
    Vector3<float> pos;
    Vector3<float> hvel;
    Vector3<float> vel;
    int idx = 0;
    // Hasta que se llegue al final del archivo, lee los atributos de cada partícula, y las añade a
    // la malla
    for (; IO::read(input_file_, pos, hvel, vel); ++idx) { grid.add(pos, hvel, vel); }
    // Comprueba que el número de partículas leídas sea el especificado en la cabecera
    if (idx != np_) {
      error_ = particle_count_error;
      IO::put_line(std::cerr, "Error: Number of particles mismatch. Header: ", np_,
                   " Found: ", idx);
    }
  }

  void FileIO::dump(Grid const & grid) {
    // Escribe la cabecera
    IO::write(output_file_, static_cast<float>(ppm_), np_);
    auto const particles = grid.get_all();
    // Escribe las partículas de la malla
    for (auto particle : particles) {
      IO::write(output_file_, static_cast<Vector3<float>>(particle.p),
                static_cast<Vector3<float>>(particle.hv), static_cast<Vector3<float>>(particle.v));
    }
  }

  void FileIO::load_header() {
    float ppm{};
    // Lee los valores
    IO::read(input_file_, ppm, np_);
    // Comprueba que el número de partículas sea válido
    if (np_ <= 0) {
      error_ = particle_count_error;
      IO::put_line(std::cerr, "Error: Invalid number of particles: ", np_);
    }
    ppm_ = static_cast<double>(ppm);
  }

}  // namespace Fluid
