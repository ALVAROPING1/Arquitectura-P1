// NOLINTBEGIN
#define FLUID_DEBUG
#ifndef TRACEIO_HPP
#define TRACEIO_HPP

#include <fstream>
#include <exception>
#include <cstdint>
#include <vector>
#include <utility>
#include "sim/vector3.hpp"
#include "sim/block.hpp"
#include "sim/io.hpp"

class TraceIO {
    using block_vector = std::vector<Fluid::Block>;
  public:

    template<typename T>
    block_vector load (T name) {
      using namespace Fluid;
      block_vector blocks;
      std::ifstream in_file{name};
      if (not in_file) { throw std::runtime_error{"No se puede abrir el archivo :)"}; }
      int32_t block_count;
      int64_t particle_count, id;
      double density;
      Vector3<double> pos, hvel, vel, acc;
      Particle particle;
      IO::read(in_file, block_count);
      for (int i = 0; i < block_count; ++i) {
        IO::read(in_file, particle_count);
        Block block{static_cast<std::size_t>(particle_count)};
        for (int64_t p = 0; p < particle_count; ++p) {
          IO::read(in_file, id, pos, hvel, vel, density, acc);
          particle.id = static_cast<int>(id);
          particle.density = density;
          particle.p = pos;
          particle.hv = hvel;
          particle.v = vel;
          particle.a = acc;
          block.add(particle);
        }
        blocks.emplace_back(std::move(block));
      }
      return blocks;
    }

  private:
};

#endif//TRACEIO_HPP
// NOLINTEND
