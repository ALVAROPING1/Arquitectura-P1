// NOLINTBEGIN
#define FLUID_DEBUG
#include "sim/argumentparser.hpp"
#include "sim/grid.hpp"
#include "sim/fileio.hpp"
#include "sim/io.hpp"
#include "traceio.hpp"

#include <string>
#include <gsl/gsl>
#include <span>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace Fluid::IO;
using block_vector = std::vector<Fluid::Block>;

constexpr double epsilon = 0.000'001;
// constexpr int max = 3;
// static int count = 0;

bool compare (double left, double right) {
  return std::abs(left - right) <= epsilon * std::max(std::abs(left), std::abs(right));
}

bool compare (Fluid::Vector3<double> left, Fluid::Vector3<double >right) {
  return compare(left.x, right.x)
      && compare(left.y, right.y)
      && compare(left.z, right.z);
}

bool compare (Fluid::Particle const & left, Fluid::Particle const & right) {
  if (left.id != right.id) {
    put_line(std::cerr, "Ids differ: ", left.id, " != ", right.id);
    return false;
  } else if (compare(left.p, right.p)
          && compare(left.density, right.density)
          && compare(left.hv, right.hv)
          && compare(left.v, right.v)
          && compare(left.a, right.a)) {
    return true;
  } else {
    put_line(std::cout, left, " ?= ", right);
    return false;
  }
}

bool compare (Fluid::Block const & left, Fluid::Block const & right) {
  auto lp = left.get_particles();
  auto rp = right.get_particles();
  if (lp.size() != rp.size()) {
    put_line(std::cerr, "\033[31mBlock sizes differ\033[0m");
    return false;
  }
  constexpr auto cmp = [](Fluid::Particle const& l, Fluid::Particle const& r) { return l.id < r.id; };
  std::sort(lp.begin(), lp.end(), cmp);
  std::sort(rp.begin(), rp.end(), cmp);
  auto l_it = lp.begin();
  auto r_it = rp.begin();
  for(; l_it != lp.end() and r_it != rp.end(); ++l_it, ++r_it) {
    if (! compare(*l_it, *r_it)) {
      return false;
    }
  }
  return true;
}

bool compare (block_vector const& left, block_vector const& right) {
  auto l_it = left.begin();
  auto r_it = right.begin();
  if (left.size() != right.size()) {
    put_line(std::cerr, "\033[31mBlock vector sizes differ\033[0m");
    return false;
  }
  int id = 0;
  for (; l_it != left.end() and r_it != right.end(); ++l_it, ++r_it, ++id) {
    if (not compare(*l_it, *r_it)) {
      put_line(std::cout, "Error comparing block \033[1m", id, "\033[0m");
      return false;
    }
  }
  return true;
}

int main(int argc, char ** argv) {
  Fluid::ArgumentParser argp{
      {argv, static_cast<std::size_t>(argc)}
  };
  if (!argp) { return argp.error(); }
  Fluid::FileIO & file = argp.file();
  Fluid::Grid grid{file.np(), file.ppm()};
  TraceIO tracer;
  file.load(grid);
  std::vector<Fluid::Block> correct;
  std::vector<Fluid::Block> attempt;

  auto trace = [&] (std::string const& name, int it) {
    put(std::cout, "\033[33;1mTesting ", name, " (", it+1, ")", "\033[0m...   \t");
    correct = tracer.load("trz/small/" + name + "-base-" + std::to_string(it + 1) + ".trz");
    attempt = grid.get_blocks();
    if (compare(correct, attempt)) {
      put_line(std::cout, "\033[32;1m[SUCCESS]\033[0m");
    } else {
      put_line(std::cout, "\033[31;1m[FAILURE]\033[0m");
      grid.set_blocks(correct);
    }
  };

  for (int i = 0; i < argp.step_count(); ++i) {
    // trace("repos", i);
    // trace("initacc", i);

    grid.increment_densities();
    // "densinc"
    // trace("denstransf", i);

    grid.transfer_accelerations();
    trace("acctransf", i);

    grid.wall_interactions();
    trace("partcol", i);

    grid.particle_movement();
    trace("motion", i);

    grid.wall_collisions();
    trace("boundint", i);

    grid.particle_repositioning();
    grid.swap_buffers();
  }
  file.dump(grid);
  return 0;
}
// NOLINTEND
