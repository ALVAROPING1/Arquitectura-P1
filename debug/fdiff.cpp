// NOLINTBEGIN
#include "sim/io.hpp"
#include "sim/vector3.hpp"

#include <fstream>
#include <vector>

using namespace Fluid;

constexpr float epsilon = 1;

bool compare(float left, float right) {
  return std::abs(left - right) <= epsilon * std::max(std::abs(left), std::abs(right));
}

bool compare(Fluid::Vector3<float> left, Fluid::Vector3<float> right) {
  return compare(left.x, right.x) && compare(left.y, right.y) && compare(left.z, right.z);
}

using vv3f = std::vector<Vector3<float>>;

void load(std::istream & in, vv3f & vpos, vv3f & vhvel, vv3f & vvel) {
  Vector3<float> pos, hvel, vel;
  while (IO::read(in, pos, hvel, vel)) {
    // IO::put_line(std::cerr, pos, hvel, vel);
    vpos.push_back(pos);
    vhvel.push_back(hvel);
    vvel.push_back(vel);
  }
}

bool do_compare(std::string const & name, vv3f const & l, vv3f const & r) {
  IO::put_line(std::cerr, "Comparing ", name);
  if (l.size() != r.size()) {
    IO::put_line(std::cerr, "Sizes differ: ", l.size(), " != ", r.size());
    return false;
  }
  for (std::size_t i = 0; i < l.size(); ++i) {
    if (not compare(l[i], r[i])) {
      IO::put_line(std::cerr, "At ", i, ": ", l[i], " != ", r[i]);
      return false;
    }
  }
  return true;
}

int main(int argc, char * argv[]) {
  if (argc != 3) {
    IO::put_line(std::cerr, "Escribe dos archivos");
    return -1;
  }
  std::ifstream if1{argv[1]};
  std::ifstream if2{argv[2]};
  vv3f vpos1, vhvel1, vvel1;
  vv3f vpos2, vhvel2, vvel2;
  load(if1, vpos1, vhvel1, vvel1);
  load(if2, vpos2, vhvel2, vvel2);

  do_compare("Positions", vpos1, vpos2);
  do_compare("Gradients", vhvel1, vhvel2);
  do_compare("Velocities", vvel1, vvel2);
}

// NOLINTEND
