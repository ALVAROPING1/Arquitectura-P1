#ifndef TRACEIO_HPP
#define TRACEIO_HPP

#include "sim/block.hpp"
#include "sim/io.hpp"
#include "sim/vector3.hpp"

#include <cstdint>
#include <exception>
#include <fstream>
#include <utility>
#include <vector>

// Parser de un archivo de trazas
class TraceIO {
    using block_vector = std::vector<Fluid::Block>;
    block_vector blocks;    // Vector de bloques
    std::ifstream in_file;  // Archivo de trazas

  public:
    /// Carga un archivo
    /// @param name
    /// Nombre del archivo de trazas
    template <typename T>
    TraceIO(T name) : in_file{name} {
      if (not in_file) {
        throw std::runtime_error{"No se puede abrir el archivo de trazas: " + name};
      }
      load_file();
    }

    /// Devuelve el vector de bloques cargado
    block_vector load() { return blocks; }

  private:
    /// Carga el archivo
    void load_file() {
      using namespace Fluid;
      int32_t block_count{};
      int64_t particle_count{};
      IO::read(in_file, block_count);
      // Itera por el vector de bloques
      for (int i = 0; i < block_count; ++i) {
        IO::read(in_file, particle_count);
        // Crea un nuevo bloque con la capacidad de partículas necesaria
        Block block{static_cast<std::size_t>(particle_count)};
        // Carga las partículas del bloque
        for (int64_t i = 0; i < particle_count; ++i) { load_particle(block); }
        // Añade el bloque al vector cargado
        blocks.emplace_back(std::move(block));
      }
    }

    /// Carga la siguiente partícula del archivo en el bloque
    void load_particle(Fluid::Block & block) {
      using namespace Fluid;
      int64_t particle_id{};
      double density{};
      Vector3<double> pos;
      Vector3<double> dens;
      Vector3<double> vel;
      Vector3<double> acc;
      // Lee la partícula
      IO::read(in_file, particle_id, pos, dens, vel, density, acc);
      Particle particle;
      particle.id       = static_cast<int>(particle_id);
      particle.density  = density;
      particle.pos      = pos;
      particle.gradient = dens;
      particle.vel      = vel;
      particle.acc      = acc;
      // Añade la partícula al bloque
      block.add(particle);
    }
};

#endif  // TRACEIO_HPP
