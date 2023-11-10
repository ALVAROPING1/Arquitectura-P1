// NOLINTBEGIN
#define FLUID_DEBUG
#include "sim/argumentparser.hpp"
#include "sim/grid.hpp"
#include "sim/fileio.hpp"
#include "sim/io.hpp"

#include <gsl/gsl>
#include <span>
#include <vector>
#include <iostream>
#include <chrono>
#include <string>

struct Clock {
  using duration_t = std::chrono::duration<long long,std::micro>;
  template<typename T>
  static duration_t timeit (T t) {
    auto start = std::chrono::high_resolution_clock::now();
    t();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  }

  duration_t densinc{};
  duration_t denstransf{};
  duration_t acctransf{};
  duration_t partcol{};
  duration_t motion{};
  duration_t boundint{};
  duration_t repos{};
};

namespace {
  std::string image (Clock::duration_t duration) {
    return std::to_string(duration.count()) + "μs";
  }
}

int main(int argc, const char ** argv) {
  Fluid::ArgumentParser argp{
      {argv, static_cast<std::size_t>(argc)}
  };
  if (!argp) { return argp.error(); }
  Fluid::FileIO & file = argp.file();
  Fluid::Grid grid{file.np(), file.ppm()};
  Clock clock{};
  file.load(grid);
  for (int i = 0; i < argp.step_count(); ++i) {
    clock.densinc    += Clock::timeit([&grid] () { grid.increment_densities(); });
    //clock.denstransf += Clock::timeit([&grid] () { grid.transform_densities(); });
    clock.acctransf  += Clock::timeit([&grid] () { grid.transfer_accelerations(); });
    clock.partcol    += Clock::timeit([&grid] () { grid.wall_interactions(); });
    clock.motion     += Clock::timeit([&grid] () { grid.particle_movement(); });
    clock.boundint   += Clock::timeit([&grid] () { grid.wall_collisions(); });
    clock.repos      += Clock::timeit([&grid] () { grid.particle_repositioning(); });
    grid.swap_buffers();
  }

  Fluid::IO::put_line(std::cerr, "densinc    => ", image(clock.densinc));
  // Fluid::IO::put_line(std::cerr, "denstransf => ", clock.denstransf);
  Fluid::IO::put_line(std::cerr, "acctransf  => ", image(clock.acctransf));
  Fluid::IO::put_line(std::cerr, "partcol    => ", image(clock.partcol));
  Fluid::IO::put_line(std::cerr, "motion     => ", image(clock.motion));
  Fluid::IO::put_line(std::cerr, "boundint   => ", image(clock.boundint));
  Fluid::IO::put_line(std::cerr, "repos      => ", image(clock.repos));

  file.dump(grid);
  return 0;
}
// NOLINTEND
