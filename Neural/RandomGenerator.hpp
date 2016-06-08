//
//  RandomGenerator.hpp
//  Neural
//
//  Created by Maxim Vainshtein on 03/06/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#ifndef RandomGenerator_hpp
#define RandomGenerator_hpp
#include "Definitions.h"
#include <memory>
NAMESPACE_NEURAL_BEGIN

class RandomGenerator {
public:
    
    /**
     * Constructor.
     *
     * @param start     The begining of the range.
     * @param end       The end of the range.
     */
    RandomGenerator(double start = 0.0,
                    double end = 1.0);
    
    /**
     * Returns a random number between the supplied range.
     *
     * @return A random number between the range supplied in the constructor.
     */
    double Random();
    
    /**
     * Destructor.
     */
    ~RandomGenerator();
    
private:
    
    class Impl;
    std::unique_ptr<Impl> m_pimpl;
    
};

NAMESPACE_NEURAL_END
#endif /* RandomGenerator_hpp */
