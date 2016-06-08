//
//  RandomGenerator.cpp
//  Neural
//
//  Created by Maxim Vainshtein on 03/06/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#include "RandomGenerator.hpp"
#include <random>

using namespace neural;

/**
 * Implementation.
 */
class RandomGenerator::Impl {
public:
    
    /**
     * Constructor.
     *
     * @param start     The begining of the range.
     * @param end       The end of the range.
     */
    Impl(double start, double end);
    
    /**
     * Returns a random number between the supplied range.
     *
     * @return A random number between the range supplied in the constructor.
     */
    double Random();
    
private:
    
    //Get random number from hardware
    std::default_random_engine m_random_engine;
    
    //The random generator
    std::mt19937 m_generator;
    
    //Defines the range
    std::uniform_int_distribution<double> m_uniform_distribution;
};

#pragma mark - Implementation

RandomGenerator::Impl::Impl(double start, double end) :
m_generator(m_random_engine()),
m_uniform_distribution(start, end)
{ }

double RandomGenerator::Impl::Random() {
    return m_uniform_distribution(m_generator);
}

#pragma mark - RandomGenerator

RandomGenerator::RandomGenerator(double start, double end) :
m_pimpl(new Impl(start, end))
{ }

RandomGenerator::~RandomGenerator() { };

double RandomGenerator::Random() {
    return m_pimpl->Random();
}
