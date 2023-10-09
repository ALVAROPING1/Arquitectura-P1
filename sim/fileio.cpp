#include "fileio.hpp"

#include "vector3.hpp"

#include "io.hpp"
#include <istream>
#include <ostream>

namespace Fluid {

  // TODO: Less magic name :)
  constexpr int incoherent_particle_count = -5;

  template <typename T>
  void read(std::istream & in, Vector3<T> & result) {
    IO::read(in, result.x, result.y, result.z);
  }

  template <typename T>
  void write(std::ostream & out, Vector3<T> const & result) {
    IO::write(out, result.x, result.y, result.z);
  }
  
  void FileIO::load(Grid & grid) {
    Vector3<float> pos;
    Vector3<float> hvel;
    Vector3<float> vel;
    int idx = 0;
    for (; input_file_ && idx < np_; ++idx) {
      IO::read(input_file_, pos, hvel, vel);
      grid.add(pos, hvel, vel);
    }
    if (idx != np_) { error_ = incoherent_particle_count; }
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
    ppm_ = static_cast<double>(ppm);
  }

}  // namespace Fluid
