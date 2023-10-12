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
    for (; IO::read(input_file_, pos, hvel, vel); ++idx) { grid.add<true>(pos, hvel, vel); }
    if (idx != np_) {
      error_ = particle_count_error;
      IO::put_line(std::cerr, "Error: Number of particles mismatch. Header: ", np_,
                   " Found: ", idx);
    }
  }

  void FileIO::dump(Grid const & grid) {
    IO::write(output_file_, static_cast<float>(ppm_), np_);
    auto const particles = grid.get_all();
    for (auto particle : particles) {
      IO::write(output_file_, static_cast<Vector3<float>>(particle.p),
                static_cast<Vector3<float>>(particle.hv), static_cast<Vector3<float>>(particle.v));
    }
  }

  void FileIO::load_header() {
    float ppm{};
    IO::read(input_file_, ppm, np_);
    if (np_ <= 0) {
      error_ = particle_count_error;
      IO::put_line(std::cerr, "Error: Invalid number of particles: ", np_);
    }
    ppm_ = static_cast<double>(ppm);
  }

}  // namespace Fluid
