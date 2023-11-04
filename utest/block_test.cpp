// Pruebas unitarias para block
#include "../sim/block.hpp"
#include "block_test.hpp"
#include <gtest/gtest.h>
#include <iostream>


namespace BlockTest{
    // prueba add
    // clases de equivalencia
    TEST_F(BlockTestClass, agregar_particula){
        Fluid::Particle expected_answer = {};
        expected_answer.id = 4;
        Fluid::Block bloque = get_block();
        bloque.add(expected_answer);
        Fluid::Particle const actual_answer = bloque[0];
        ASSERT_EQ(actual_answer.id, expected_answer.id);
    }
    // prueba clear
    // clases de equivalencia
    TEST_F(BlockTestClass, quitar_particulas){
        int const expected_answer = 0;
        Fluid::Block bloque = get_block();
        add_particle();
        bloque.clear();
        ASSERT_EQ(bloque.size(), expected_answer);
    }
    // prueba indexar
    // clases de equivalencia
    TEST_F(BlockTestClass, operador_idexar){
        Fluid::Particle expected_answer = {};
        expected_answer.id = 4;
        add_particle();
        Fluid::Block bloque = get_block();
        bloque.add(expected_answer);
        
        Fluid::Particle const actual_answer = bloque[1];
        ASSERT_EQ(actual_answer.id, expected_answer.id);
    }
    // prueba size
    // clases de equivalencia
    TEST_F(BlockTestClass, numero_elementos){
        const int expected_answer = 10;
        for (int i = 0; i < expected_answer; i++)
        {
            add_particle();
        }
        Fluid::Block const bloque = get_block();
        ASSERT_EQ(bloque.size(), expected_answer);        
    }
    // operador begin y end
    // clases de equivalencia
    TEST_F(BlockTestClass, operador_begin){
        int const expected_answer = 10;
        Fluid::Block bloque = get_block();
        // agruegar particulas al bloque
        for (int i = 0; i < expected_answer; i++)
        {
            Fluid::Particle particula {};
            particula.id = i;
            bloque.add(particula);
        }
        // iterar
        int iterador = 0;    
        for (auto i = bloque.begin(); i != bloque.end(); i++){
            iterador++;
        }
        ASSERT_EQ(iterador, expected_answer);      
    }
    // get particles
    // clases de equivalencia
    TEST_F(BlockTestClass, obtener_particulas){
        std::vector <Fluid::Particle> expected_answer;
        Fluid::Block bloque = get_block();
        int const count = 5;
        for (int i = 0; i < count; i++){
            Fluid::Particle particula{};
            particula.id = i;
            expected_answer.push_back(particula);
            bloque.add(particula);
        }
        // comparar vectores
        std::vector <Fluid::Particle> const actual_answer = bloque.get_particles();
        bool equal = true;
        for (int i = 0; i < count; i++){
            if (expected_answer[i].id != actual_answer[i].id){
                equal = false;
            }
        }
        ASSERT_EQ(equal, true);
    }

    
} // namespace BlockTest

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
