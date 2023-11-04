#ifndef BLOCK_TEST_HPP
#define BLOCK_TEST_HPP

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "../sim/block.hpp"

namespace BlockTest{
    namespace Const
    {
        unsigned long const estimated_amount = 5;
    } // namespace const
    
    

    class BlockTestClass: public testing::Test
    {
    
    public:  
        Fluid::Block get_block(){
            return bloque_prueba;
        }

        void add_particle(){
            Fluid::Particle particula{};
            particula.id = 1;
            bloque_prueba.add(particula);
        }

        
    private:
        Fluid::Block bloque_prueba{BlockTest::Const::estimated_amount};
    };
    
    
    
} // namespace BlockTest
#endif // BLOCK_TEST_HPP