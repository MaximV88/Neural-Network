//
//  Trainer.cpp
//  Neural
//
//  Created by Maxim Vainshtein on 04/06/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#include "Trainer.hpp"
#include "DataIterator.hpp"
#include <math.h>
#include <iostream>
#include <iomanip>

using namespace neural;

/**
 * Implementation.
 */
class Trainer::Impl {
public:
    
    /**
     * Constructor.
     *
     * @param data_file_path    The file path to the data that represents the numbers.
     * @param key_file_path     The file path to the representations that the data file has.
     */
    Impl(const std::string& data_file_path,
         const std::string& key_file_path);
    
    /**
     * Trains the network at a percentage of the input data, and then returns
     * the correctness of the remaining input as a percentage.
     *
     * @param percentage    The percentage of the data to train.
     * @param log           Prints to the consule the progress.
     * @return Correctness of remaining records.
     */
    double Train(double percentage,
                 std::function<void(const Data&, size_t)> train_handler,
                 std::function<double(const Data&)> answer_handler,
                 bool log);
    
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
                bool log);
    
private:
    
    Data ConformData(const Data& data) const;
    
    std::string m_data_file_path;
    
    std::string m_key_file_path;
    
};

#pragma mark - Implementation

Trainer::Impl::Impl(const std::string& data_file_path,
                    const std::string& key_file_path) :
m_data_file_path(data_file_path),
m_key_file_path(key_file_path)
{ }

double Trainer::Impl::Train(double percentage,
                            std::function<void (const Data &, size_t)> train_handler,
                            std::function<double (const Data &)> answer_handler,
                            bool log) {
    
    //Train all networks
    size_t index = 0;
    size_t correct = 0;
    
    size_t all_records = RecordsInFile(m_key_file_path);
    size_t train_limit = all_records * (percentage / 100.0);
    size_t validate_limit = all_records - train_limit;
    
    //Set attribute for logging
    if (log) { std::cout << std::fixed; }
    
    for (DataIterator data(m_data_file_path), results(m_key_file_path) ;
         data.Valid() && results.Valid() ;
         data.Next(), results.Next(), index++) {
        
        //Simplify data
        Data modified_data = ConformData(data.Value());
        
        size_t real_value = static_cast<size_t>(lround(results.Value().content.front()));
        
        if (index < train_limit) {
            
            //Training session
            train_handler(modified_data, real_value);
            
            if (log && index % (train_limit / 100) == 0)
                std::cout
                << std::setprecision(3)
                << "overall progress: "
                << index / static_cast<double>(train_limit) * 100
                << "%\n";
            
        }
        else {
            
            //Validation session
            if (real_value == static_cast<size_t>(lround(answer_handler(modified_data))))
                ++correct;
            
            if (log && (index - train_limit) % (validate_limit / 100) == 0)
                std::cout
                << std::setprecision(3)
                << "correct:"
                << correct / static_cast<double>(index - train_limit) * 100
                << "%\t\t\toverall progress: " << (index - train_limit) / static_cast<double>(validate_limit) * 100.0 << "%\n";
            
        }
    }
    
    return correct / static_cast<double>(validate_limit);
}

Data Trainer::Impl::ConformData(const neural::Data &data) const {
    
    Data modified_data;
    modified_data.content.reserve(784);
    for (size_t i = 0 ; i < 784 ; i++)
        modified_data.content.push_back(data.content.at(i) > 50 ? 1.0 : 0.0);
    
    return modified_data;
}

double Trainer::Impl::Test(const std::string &test_file_path,
                           const std::string& key_file_path,
                           std::function<double(const Data&)> answer_handler,
                           bool log) {
    
    size_t correct = 0;
    size_t index = 0;
    size_t all_values = RecordsInFile(test_file_path);
    
    //Set attribute for logging
    if (log) { std::cout << std::fixed; }
    
    for (DataIterator test_iterator(test_file_path), key_iterator(key_file_path) ;
         test_iterator.Valid() && key_iterator.Valid() ;
         test_iterator.Next(), key_iterator.Next(), ++index) {
        
        double result = answer_handler(ConformData(test_iterator.Value()));
        
        size_t real_value = static_cast<size_t>(lround(key_iterator.Value().content.front()));
        
        if (real_value == result)
            ++correct;

        if (log && index % all_values / 100 == 0)
            std::cout
            << std::setprecision(3)
            << "correct:"
            << correct / static_cast<double>(all_values) * 100
            << "%\t\t\toverall progress: " << index / static_cast<double>(all_values) * 100.0 << "%\n";
    }

    return correct / static_cast<double>(RecordsInFile(test_file_path));
}

#pragma mark - Trainer functions

Trainer::Trainer(const std::string& data_file_path,
                 const std::string& key_file_path) :
m_pimpl(new Impl(data_file_path, key_file_path))
{ }

Trainer::~Trainer() = default;

double Trainer::Train(double percentage,
                      std::function<void(const Data&, size_t)> train_handler,
                      std::function<double(const Data&)> answer_handler,
                      bool log) {
    return m_pimpl->Train(percentage, train_handler, answer_handler, log);
}

double Trainer::Test(const std::string &test_file_path,
                     const std::string& key_file_path,
                     std::function<double(const Data&)> answer_handler,
                     bool log) {
    return m_pimpl->Test(test_file_path, key_file_path, answer_handler, log);
}
