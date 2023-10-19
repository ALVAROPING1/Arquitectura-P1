// Representación de un bloque

#ifndef SIM_BLOCK_HPP
#define SIM_BLOCK_HPP

#include "particle.hpp"
#include "io.hpp"
#include "vector3.hpp"

#include <iostream>
#include <ostream>
#include <utility>
#include <vector>
#include <tuple>

namespace Fluid {

  class Block {
    public:
      using PositionType = Vector3<double>;
      using HvType = Vector3<double>;
      using VelocityType = Vector3<double>;
      using AccelerationType = Vector3<double>;
      using DensityType = double;
      using IDType = int;
      std::vector<PositionType> position_{};
      std::vector<HvType> hv_{};
      std::vector<VelocityType> velocity_{};
      std::vector<AccelerationType> acceleration_{};
      std::vector<DensityType> density_{};
      std::vector<IDType> id_{};

    public:
      constexpr explicit Block(std::size_t estimated_amount) {
        position_.reserve(estimated_amount);
        hv_.reserve(estimated_amount);
        velocity_.reserve(estimated_amount);
        acceleration_.reserve(estimated_amount);
        density_.reserve(estimated_amount);
        id_.reserve(estimated_amount);
      };

      void add(Particle const & particle) {
        if (particle.p.x == 0 && particle.p.y == 0 && particle.p.z == 0) {
          IO::put_line(std::cout, "Partícula rara");
    }
        position_.push_back(particle.p);
        hv_.push_back(particle.hv);
        velocity_.push_back(particle.v);
        acceleration_.push_back(particle.a);
        density_.push_back(particle.density);
        id_.push_back(particle.id);
      }

      void clear() {
        position_.clear();
        hv_.clear();
        velocity_.clear();
        acceleration_.clear();
        density_.clear();
        id_.clear();
      };

      [[nodiscard]] constexpr std::size_t size() const { 
        // All attribute vectors are appended to/cleared at once, so they will
        // always have the same size and it's safe to get the size from any of them
        return id_.size();
      }

      // [[nodiscard]] constexpr auto begin() const { return particles_.begin(); }
      //
      // [[nodiscard]] constexpr auto end() const { return particles_.end(); }
      //
      // constexpr auto begin() { return particles_.begin(); }
      //
      // constexpr auto end() { return particles_.end(); }

      // struct Position {
      //     using type = PositionType &;
      //     using const_type = const PositionType &;
      //
      //     constexpr static type ref(Block & block, std::size_t i) { return block.position_[i]; }
      //     constexpr static const_type ref(Block const & block, std::size_t i) { return block.position_[i]; }
      // };
      // struct Hv {
      //     using type = HvType &;
      //     using const_type = const HvType &;
      //
      //     constexpr static type ref(Block & block, std::size_t i) { return block.hv_[i]; }
      //     constexpr static const_type ref(Block const & block, std::size_t i) { return block.hv_[i]; }
      // };
      // struct Velocity {
      //     using type = VelocityType &;
      //     using const_type = const VelocityType &;
      //
      //     constexpr static type ref(Block & block, std::size_t i) { return block.velocity_[i]; }
      //     constexpr static const_type ref(Block const & block, std::size_t i) { return block.velocity_[i]; }
      // };
      // struct Acceleration {
      //     using type = AccelerationType &;
      //     using const_type = const AccelerationType &;
      //
      //     constexpr static type ref(Block & block, std::size_t i) { return block.acceleration_[i]; }
      //     constexpr static const_type ref(Block const & block, std::size_t i) { return block.acceleration_[i]; }
      // };
      // struct Density {
      //     using type = DensityType &;
      //     using const_type = const DensityType &;
      //
      //     constexpr static type ref(Block & block, std::size_t i) { return block.density_[i]; }
      //     constexpr static const_type ref(Block const & block, std::size_t i) { return block.density_[i]; }
      // };
      // struct ID {
      //     using type = IDType &;
      //     using const_type = const IDType &;
      //
      //     constexpr static type ref(Block & block, std::size_t i) { return block.id_[i]; }
      //     constexpr static const_type ref(Block const & block, std::size_t i) { return block.id_[i]; }
      // };
      //
      // template <typename... Query>
      // constexpr std::tuple<typename Query::type...> query(std::size_t i) {
      //   return std::make_tuple(std::ref(Query::ref(*this, i))...);
      // }
      //
      // template <typename... Query>
      // [[nodiscard]] constexpr std::tuple<typename Query::const_type...> query(std::size_t i) const {
      //   return std::make_tuple(std::ref(Query::ref(*this, i))...);
      // }

      constexpr Particle operator[](std::size_t i) const {
        // auto [p, hv, v, a, density, id]{query<Position, Hv, Velocity, Acceleration, Density, ID>(i)};
        Particle particle;
        particle.p = position_[i];
        particle.hv = hv_[i];
        particle.v = velocity_[i];
        particle.a = acceleration_[i];
        particle.density = density_[i];
        particle.id = id_[i];
        return particle;
      }

      [[nodiscard]] constexpr std::vector<Particle> get_particles() const { 
        std::vector<Particle> out{};
        out.reserve(size());
        for (std::size_t i{0}; i < size(); ++i) {
          Particle particle{operator[](i)};
          // if (particle.p.x == 0 && particle.p.y == 0 && particle.p.z == 0) {IO::put_line(std::cerr, "wrong particle", particle);}
          out.push_back(particle);
        }
        return out;
      }

      friend std::ostream & operator<<(std::ostream & out, Block const & block) {
        out << "Block[";
        for (std::size_t i{0}; i < block.size(); ++i) { out << block[i]; }
        return out << "]";
      }

  };

}  // namespace Fluid
#endif  // SIM_PROGARGS_HPP
