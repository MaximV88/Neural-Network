//
//  Perceptron.hpp
//  Neural
//
//  Created by Maxim Vainshtein on 03/06/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#ifndef Perceptron_hpp
#define Perceptron_hpp
#include "Definitions.h"
#include "RandomGenerator.hpp"
#include <vector>
#include <stdio.h>
NAMESPACE_NEURAL_BEGIN
class Network;

class Perceptron {
public:
    
    /**
     * Constructor.
     *
     * @param weights_count         Number of weights that the perceptron will have.
     * @param learning_constant     The learning rate for weights adjustments.
     */
    Perceptron(size_t weights_count, double learning_constant = 0.01, double bias = 1.0);
    
    /**
     * Constructor.
     *
     * @param serialized The serialized perceptron.
     */
    Perceptron(const std::string& serialized);
    
    /**
     * Recieves input and returns the the sum.
     *
     * @param input     The input to calculate (either all of the previous perceptron's 
     *                  results or the input data).
     *
     * @return The value that the perceptron has given this input.
     */
    double Feed(const std::vector<double>& input) const;

    /**
     * Trains the perceptron according to the given learning
     * constant (eta), the delta of the current layer and the
     * omicron (output) of the previous layer.
     *
     * @param delta The delta of the current layer (depends on hidden or output layer).
     * @param omicron The omicron (output) of the previous layer.
     */
    void Train(double delta, const std::vector<double>& omicron);
    
    /**
     * Outputs the perceptron into a format that can later
     * be loaded to recreate the setup and weights.
     *
     * @return The serialized version of the perceptron.
     */
    std::string Serialize() const;
    
    //The random generator.
    RandomGenerator m_generator;
    
    //Holds the weights.
    std::vector<double> m_weights;
    
    ///Stores the learning constant.
    double m_learning_constant;
    
    ///Stores the bias
    double m_bias;
    
};

NAMESPACE_NEURAL_END
#endif /* Perceptron_hpp */
