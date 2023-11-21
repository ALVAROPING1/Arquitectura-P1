// Pruebas unitarias para la simulación
#include "sim/fileio.hpp"
#include "sim/grid.hpp"
#include "sim/io.hpp"
#include "traceio.hpp"

#include <algorithm>
#include <gsl/gsl>
#include <gtest/gtest.h>
#include <iostream>
#include <span>
#include <string>
#include <vector>

namespace SimulationTest {

  using namespace Fluid::IO;
  using block_vector = std::vector<Fluid::Block>;

  constexpr double epsilon = 1e-8;  // Floating point comparison tolerance
  constexpr int iterations{5};      // Amount of iterations for which there is trazes

  /// Compara 2 `double`s con una toleracia
  bool compare(double left, double right) {
    return std::abs(left - right) <= epsilon * std::max(std::abs(left), std::abs(right));
  }

  /// Compara 2 `Vector3<double>`s con una tolerancia
  bool compare(Fluid::Vector3<double> left, Fluid::Vector3<double> right) {
    return compare(left.x, right.x) && compare(left.y, right.y) && compare(left.z, right.z);
  }

  /// Compara 2 `Particle`s
  bool compare(Fluid::Particle const & left, Fluid::Particle const & right) {
    if (left.id != right.id) {
      put_line(std::cerr, "Ids differ: ", left.id, " != ", right.id);
      return false;
    }
    if (compare(left.pos, right.pos) && compare(left.density, right.density) &&
        compare(left.gradient, right.gradient) && compare(left.vel, right.vel) &&
        compare(left.acc, right.acc)) {
      return true;
    }
    put_line(std::cerr, "Left: correct, right: calculated");
    put_line(std::cerr, left, " != ", right);
    return false;
  }

  /// Compara 2 `Block`s
  bool compare(Fluid::Block const & left, Fluid::Block const & right) {
    // Gets the particles
    auto left_vec  = left.get_particles();
    auto right_vec = right.get_particles();
    // Checks that the amounts match
    if (left_vec.size() != right_vec.size()) {
      put_line(std::cerr, "\033[31mBlock sizes differ\033[0m");
      return false;
    }
    // Sorts the vectors according to the particle IDs
    constexpr auto cmp = [](Fluid::Particle const & l, Fluid::Particle const & r) {
      return l.id < r.id;
    };
    std::sort(left_vec.begin(), left_vec.end(), cmp);
    std::sort(right_vec.begin(), right_vec.end(), cmp);
    // Iterates through both vectors
    for (auto l_it{left_vec.begin()}, r_it{right_vec.begin()};
         l_it != left_vec.end() and r_it != right_vec.end(); ++l_it, ++r_it) {
      // Checks if the particles match
      if (!compare(*l_it, *r_it)) { return false; }
    }
    return true;
  }

  /// Compara 2 vectores de `Block`
  bool compare(block_vector const & left, block_vector const & right) {
    // Compares the amount of blocks
    if (left.size() != right.size()) {
      put_line(std::cerr, "\033[31mBlock vector sizes differ\033[0m");
      return false;
    }
    int block_id{0};
    // Iterates through both vectors
    for (auto l_it{left.begin()}, r_it{right.begin()}; l_it != left.end() and r_it != right.end();
         ++l_it, ++r_it, ++block_id) {
      // Checks if the blocks match
      if (not compare(*l_it, *r_it)) {
        put_line(std::cerr, "Error comparing block \033[1m", block_id, "\033[0m");
        return false;
      }
    }
    return true;
  }

  /// Resetea la densidad de todas las partículas a 0
  void init_density(std::vector<Fluid::Block> & block_vec) {
    for (auto & block : block_vec) {
      for (std::size_t i{0}; i < block.size(); ++i) { block.pos_dens(i).dens = 0; }
    }
  }

  /// Compara el estado del programa con el del archivo de trazas `<name>-base-<i>.trz`
  /// @param grid
  /// Malla de la simulación
  /// @param name
  /// Nombre del paso
  /// @param i
  /// Iteración de la simulación
  void trace_state(Fluid::Grid & grid, std::string const & model, std::string const & name, int i) {
    // Carga el vector de bloques correcto y del programa
    std::vector<Fluid::Block> const correct =
        TraceIO{"trz/" + model + "/" + name + "-base-" + std::to_string(i + 1) + ".trz"}.load();
    std::vector<Fluid::Block> attempt = grid.get_blocks();
    if (name == "repos" or name == "initacc") { init_density(attempt); }
    // Compara los vectores
    ASSERT_TRUE(compare(correct, attempt))
        << "Simulation result doesn't match the trazes. Step: " << name << ", Iteration: " << i;
  }

  /// Compara el estado del programa con el de los archivos de trazas para una iteración
  /// @param grid
  /// Malla de la simulación
  /// @param model
  /// Nombre del modelo, usado para seleccionar las trazas a cargar
  /// @param i
  /// Iteración de la simulación
  void compare_iteration(Fluid::Grid & grid, std::string const & model, int i) {
    trace_state(grid, model, "repos", i);
    trace_state(grid, model, "initacc", i);

    grid.increment_densities();
    // Densinc step can't be tested since the densinc and denstransf steps are combined in the
    // program for performance reasons.
    trace_state(grid, model, "denstransf", i);
    grid.transfer_accelerations();
    trace_state(grid, model, "acctransf", i);
    grid.wall_interactions();
    trace_state(grid, model, "partcol", i);
    grid.particle_movement();
    trace_state(grid, model, "motion", i);
    grid.wall_collisions();
    trace_state(grid, model, "boundint", i);

    grid.particle_repositioning();
  }

  /// Compara el archivo de salida del programa con el de referencia
  /// @param result_file
  /// Archivo de salida generado
  /// @param model
  /// Nombre del modelo, usado para seleccionar el archivo de salida de referencia
  /// @param i
  /// Iteración de la simulación
  void compare_result(std::ifstream result_file, std::string const & model, int i) {
    // Abre el archivo de salida de referencia
    std::string const correct_name{"correct/" + model + "-" + std::to_string(i + 1) + ".fld"};
    std::ifstream correct_file{correct_name};
    ASSERT_TRUE(correct_file) << "No se puede abrir el archivo de salida de referencia: "
                              << correct_name;
    // Compara byte a byte los archivos
    char correct_byte{};
    char result_byte{};
    while (result_file.read(&result_byte, sizeof(char)) and
           correct_file.read(&correct_byte, sizeof(char))) {
      // Si hay alguna diferencia, da un error y termina
      ASSERT_EQ(correct_byte, result_byte)
          << "El archivo de salida generado difiere del archivo de salida de referencia";
    }
  }

  void test_simulation(std::string const & model) {
    // Inicializa el programa
    Fluid::FileIO file{"in/" + model + ".fld", "out.fld"};
    Fluid::Grid grid{file.np(), file.ppm()};
    // Desactiva temporalmente std::cout para no mostrar la salida de load en la ejecución del test
    std::cout.setstate(std::ios_base::failbit);
    // Carga el archivo de entrada
    file.load(grid);
    std::cout.clear();
    std::cerr.precision(4L * 2);
    // Obtiene el nombre del modelo a usar del nombre del archivo de entrada
    // Realiza las iteraciones
    for (int i = 0; i < iterations; ++i) {
      compare_iteration(grid, model, i);
      // Escribe el archivo de salida y comprueba que es correcto. Se vuelve a abrir el archivo para
      // sobreescribirlo
      Fluid::FileIO file{"in/" + model + ".fld", "out.fld"};
      file.dump(grid);
      compare_result(std::ifstream{"out.fld"}, model, i);
    }
  }

  TEST(SimulationTest, small_model) {
    test_simulation("small");
  }

  TEST(SimulationTest, large_model) {
    test_simulation("large");
  }
};  // namespace SimulationTest
