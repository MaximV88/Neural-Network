//
//  DataIterator.hpp
//  Neural
//
//  Created by Maxim Vainshtein on 03/06/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#ifndef DataIterator_hpp
#define DataIterator_hpp
#include "Definitions.h"
#include "Data.hpp"
#include <memory>
#include <string>
NAMESPACE_NEURAL_BEGIN

/**
 * Returns the number of values in the given input file.
 *
 * @param file_path     The file path to count the number of records of.
 * @return Number of records in the input file.
 */
size_t RecordsInFile(const std::string& file_path);

class DataIterator {
public:
    
    /**
     * Constructor.
     *
     * @param file_path     The path to the data file.
     */
    DataIterator(const std::string& file_path);
    
    /**
     * Advances the iterator to the next position.
     */
    void Next();
    
    /**
     * Returns the value at the current position of the iterator.
     *
     * @return Data representation of the current position.
     */
    Data Value() const;
    
    /**
     * Checks if the current position is valid.
     *
     * @return True if valid, false otherwise.
     */
    bool Valid() const;
    
    /**
     * Destructor.
     */
    ~DataIterator();
    
private:
    
    class Impl;
    std::unique_ptr<Impl> m_pimpl;
    
};

NAMESPACE_NEURAL_END
#endif /* DataIterator_hpp */
