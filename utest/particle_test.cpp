#include "sim/particle.hpp"
#include "sim/vector3.hpp"

#include <gtest/gtest.h>

namespace ParticleTest {
  constexpr Fluid::Vector3<double> vec_default_double{0.0, 0.0, 0.0};
  constexpr Fluid::Vector3<float> pos{1.0, 2.0, 3.0};
  constexpr Fluid::Vector3<float> grad{3.0, 1.0, 2.0};
  constexpr Fluid::Vector3<float> vel{2.0, 0.5, 1.0};
  constexpr double dens{0.5};

  // Valor por defecto
  TEST(ParticleTest, default) {
    Fluid::Particle const particle;
    EXPECT_EQ(particle.id, 0);
    EXPECT_EQ(particle.pos, vec_default_double);
    EXPECT_EQ(particle.gradient, vec_default_double);
    EXPECT_EQ(particle.vel, vec_default_double);
    EXPECT_EQ(particle.acc, Fluid::Constants::gravity);
    EXPECT_EQ(particle.density, 0.0);
  }

  // Constructor
  TEST(ParticleTest, constructor) {
    Fluid::Particle const particle{
      {pos, grad, vel},
      3, dens
    };
    EXPECT_EQ(particle.id, 3);
    EXPECT_EQ(particle.pos, static_cast<Fluid::Vector3<double>>(pos));
    EXPECT_EQ(particle.gradient, static_cast<Fluid::Vector3<double>>(grad));
    EXPECT_EQ(particle.vel, static_cast<Fluid::Vector3<double>>(vel));
    EXPECT_EQ(particle.acc, Fluid::Constants::gravity);
    EXPECT_EQ(particle.density, dens);
  }

  // Operador igual
  TEST(ParticleTest, equals) {
    Fluid::Particle const particle1{
      {pos, grad, vel},
      3, dens
    };
    Fluid::Particle particle2{
      {pos, grad, vel},
      3, dens
    };
    EXPECT_EQ(particle1 == particle2, true);
    particle2.id = 2;
    EXPECT_EQ(particle1 == particle2, false);
  }

  // Operador <<
  TEST(ParticleTest, print) {
    Fluid::Particle const particle{
      {pos, grad, vel},
      3, dens
    };
    std::stringstream out;
    std::stringstream expected;
    out << particle;
    expected << "Particle n: " << particle.id << "\n\tPosition: " << particle.pos
             << "\n\tVelocity: " << particle.vel << "\n\tAccelera: " << particle.acc
             << "\n\tGradient: " << particle.gradient << "\n\tDensity : " << particle.density
             << "\n";
    EXPECT_EQ(out.str(), expected.str());
  }
}  // namespace ParticleTest
