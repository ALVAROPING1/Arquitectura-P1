#ifndef VECTOR_TEST_HPP
#define VECTOR_TEST_HPP

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "../sim/vector3.hpp"

namespace Vector3Test {
    
class Vector3TestClass: public testing::Test{
    public:
    Vector3TestClass() = default;
    // funciones para obtener los valores de los vectores prueba
    Fluid::Vector3 <int> get_int_vector_1() {
        return vector_int_1;
    };
    Fluid::Vector3 <int> get_int_vector_2() {
        return vector_int_2;
    };
    Fluid::Vector3 <float> get_float_vector_1() {
        return vector_float_1;
    };
    Fluid::Vector3 <float> get_float_vector_2() {
        return vector_float_2;
    };
    Fluid::Vector3 <double> get_double_vector_1() {
        return vector_double_1;
    };
    Fluid::Vector3 <double> get_double_vector_2() {
        return vector_double_2;
    };
    // dar valores para cada prueba
    void test_int_sumar_1(){
        Fluid::Vector3 <int> const test1 = {10, 12 , 23};
        Fluid::Vector3 <int> const test2 = {7, 34, 18};
        vector_int_1 = test1;
        vector_int_2 = test2;
    };
    void test_float_sumar_1(){
        Fluid::Vector3 <float> const test1 = {1.0, 0.5, 2.0};
        Fluid::Vector3 <float> const test2 = {3.5, 1.0, 0.5};
        vector_float_1 = test1;
        vector_float_2 = test2;
    };
    void test_double_sumar_1(){
        Fluid::Vector3 <double> const test1 = {1.0, 0.5, 2.0};
        Fluid::Vector3 <double> const test2 = {3.5, 1.0, 0.5};
        vector_double_1 = test1;
        vector_double_2 = test2;
    };

    void test_int_dividir_1(){
        Fluid::Vector3 <int> const test1 = {33, 121 , 132};
        Fluid::Vector3 <int> const test2 = {3, 11 , 2};
        vector_int_1 = test1;
        vector_int_2 = test2;
    };
    void test_float_dividir_1(){
        Fluid::Vector3 <float> const test1 = {42.0, 84.0, 35.0};
        Fluid::Vector3 <float> const test2 = {7.0, 12.0, 0.5};
        vector_float_1 = test1;
        vector_float_2 = test2;
    };
    void test_double_dividir_1(){
        Fluid::Vector3 <double> const test1 = {81.0, 36.0, 45.0};
        Fluid::Vector3 <double> const test2 = {3.0, 2.0, 5.0};
        vector_double_1 = test1;
        vector_double_2 = test2;
    };

    void test_menor_que_int_2(){
        Fluid::Vector3 <int> const test1 = {4, 5, 6};
        Fluid::Vector3 <int> const test2 = {1, 2 , 3};
        vector_int_1 = test1;
        vector_int_2 = test2;
    };
    void test_menor_que_float_2(){
        Fluid::Vector3 <float> const test1 = {1.0, 0.5, 2.0};
        Fluid::Vector3 <float> const test2 = {0.5, 0.3, 0.5};
        vector_float_1 = test1;
        vector_float_2 = test2;
    };
    void test_menor_que_double_2(){
        Fluid::Vector3 <double> const test1 = {1.0, 0.5, 2.0};
        Fluid::Vector3 <double> const test2 = {0.5, 0.3, 0.5};
        vector_double_1 = test1;
        vector_double_2 = test2;
    };

    void test_igual_que_int_2(){
        Fluid::Vector3 <int> const test1 = {4, 5, 6};
        Fluid::Vector3 <int> const test2 = {4, 5, 6};
        vector_int_1 = test1;
        vector_int_2 = test2;
    };
    void test_igual_que_float_2(){
        Fluid::Vector3 <float> const test1 = {1.0, 0.5, 2.0};
        Fluid::Vector3 <float> const test2 = {1.0, 0.5, 2.0};
        vector_float_1 = test1;
        vector_float_2 = test2;
    };
    void test_igual_que_double_2(){
        Fluid::Vector3 <double> const test1 = {1.0, 0.5, 2.0};
        Fluid::Vector3 <double> const test2 = {1.0, 0.5, 2.0};
        vector_double_1 = test1;
        vector_double_2 = test2;
    };

    


    
    private:
    Fluid::Vector3 <int>  vector_int_1;
    Fluid::Vector3 <int>  vector_int_2;
    Fluid::Vector3 <float>  vector_float_1;
    Fluid::Vector3 <float>  vector_float_2;
    Fluid::Vector3 <double>  vector_double_1;
    Fluid::Vector3 <double>  vector_double_2;
    

};
} //namsespace Vector3Test
#endif //VECTOR_TEST_HPP
