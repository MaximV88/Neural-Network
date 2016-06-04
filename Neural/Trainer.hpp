//
//  Trainer.hpp
//  Neural
//
//  Created by Maxim Vainshtein on 04/06/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#ifndef Trainer_hpp
#define Trainer_hpp
#include "Definitions.h"
#include <string>
#include <memory>
#include <functional>
NAMESPACE_NEURAL_BEGIN
class Network;
class Data;

class Trainer {
public:
    
    /**
     * Constructor.
     */
    Trainer();
    
    /**
     * Trains the network at a percentage of the input data, and then returns
     * the correctness of the remaining input as a percentage.
     *
     * @param percentage    The percentage of the data to train.
     * @param log           Prints to the consule the progress.
     * @return Correctness of remaining records.
     */
    double Train(double percentage,
                 const std::string& data_file_path,
                 const std::string& key_file_path,
                 std::function<void(const Data& data, size_t key)> train_handler,
                 std::function<double(const Data&)> answer_handler,
                 bool log = true);
    
    /**
     * Trains the network on all of the values that were specified
     * in the files that were given in the constructor, and then
     * returns the correctness on the test file as a percentage.
     *
     * @param test_file_path    The file path to the file that the network will be tested against.
     * @param log               Flag that indicates if to print to the consule the progress.
     * @return The percentage of correct calculations from all records in the test file.
     */
    double Test(const std::string& test_file_path,
                const std::string& key_file_path,
                std::function<double(const Data&)> answer_handler,
                bool log = true);
    
    /**
     * Destructor.
     */
    ~Trainer();
    
private:
    
    class Impl;
    std::unique_ptr<Impl> m_pimpl;
    
};

NAMESPACE_NEURAL_END
#endif /* Trainer_hpp */
