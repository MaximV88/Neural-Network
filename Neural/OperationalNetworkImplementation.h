//
//  OperationalNetworkImplementation.h
//  Neural
//
//  Created by Maxim Vainshtein on 05/06/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#ifndef OperationalNetworkImplementation_h
#define OperationalNetworkImplementation_h
#include "OperationalNetwork.hpp"
#include <stdlib.h>
NAMESPACE_NEURAL_BEGIN
class Data;

class OperationalNetwork::Impl {
public:
    
    /**
     * Destructor.
     */
    virtual ~Impl() { };
    
    /**
     * Returns the type of the network as an enum.
     *
     * @return An enum that represents the type of the network.
     */
    virtual OperationalNetwork::Type Type() const = 0;
    
    /**
     * Trains the network with given input and it's answer.
     *
     * @param data  The data to train on.
     * @param key   The answer to the data.
     */
    virtual void Train(const Data& data, size_t key) = 0;
    
    /**
     * Estimates the result to the given input.
     *
     * @return The estimation about the answer.
     */
    virtual double Estimate(const Data& input) const = 0;
    
    /**
     * This will serialize the network into a form that can be saved and
     * later construct an identical network to the current one.
     *
     * @return A string that contains the serialized form of the network.
     */
    virtual std::string Serialize() const = 0;
    
};

NAMESPACE_NEURAL_END
#endif /* OperationalNetworkImplementation_h */
