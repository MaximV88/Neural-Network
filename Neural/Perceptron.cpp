//
//  Perceptron.cpp
//  Neural
//
//  Created by Maxim Vainshtein on 03/06/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#include "Perceptron.hpp"
#include <math.h>
#include <string>

NAMESPACE_NEURAL_BEGIN

inline double ActivationFunction(double value) {
    
    //Sigmoid function
    return 1/(1 + exp(-value));
}

NAMESPACE_NEURAL_END

using namespace neural;

Perceptron::Perceptron(size_t weights_count, double learning_constant, double bias) :
m_learning_constant(learning_constant),
m_bias(bias),
m_generator(-1.0, 1.0) {
    
    m_weights.reserve(weights_count);
    
    //Fill the weights with random numbers between -1 and 1
    for (size_t index = 0 ; index < weights_count ; index++)
        m_weights.push_back(m_generator.Random());
}

Perceptron::Perceptron(const std::string& serialized) {
    
    //Deserialize manually
    size_t delimiter_index = serialized.find_first_of(':');
    size_t weights_count = std::stoul(std::string(serialized.begin(), serialized.begin() + delimiter_index));
    m_weights.reserve(weights_count);
    
    for (size_t index = 0 ; index < weights_count ; index++) {
        
        size_t new_delimiter_index = serialized.find_first_of(',', delimiter_index);
        double weight_value = std::stod(std::string(serialized.begin() + delimiter_index + 1, serialized.begin() + new_delimiter_index));
        m_weights.push_back(weight_value);
        
        delimiter_index = new_delimiter_index + 1;
    }
}

double Perceptron::Feed(const std::vector<double> &input) const {
    
    double sum = 0.0;
    
    for (size_t index = 0 ; index < m_weights.size() ; index++)
        sum += m_weights[index] * input[index];
    
    sum += m_bias;
    
    return ActivationFunction(sum);
}

void Perceptron::Train(double delta, const std::vector<double> &omicron) {
    
    for (size_t index = 0, total = m_weights.size() ; index < total ; index++)
        m_weights[index] += -m_learning_constant * delta * omicron[index];
    
    //Find it there is a bias and update it accordingly
    m_bias += -m_learning_constant * delta;
    
}

std::string Perceptron::Serialize() const {
    
    std::string serialized = std::to_string(m_weights.size()) + ':';
    for (size_t index = 0, total = m_weights.size() ; index < total ; index++) {
        serialized += std::to_string(m_weights[index]);
        
        //Add a delimiter for later deserialization
        serialized += ',';
    }
    
    return serialized;
}

