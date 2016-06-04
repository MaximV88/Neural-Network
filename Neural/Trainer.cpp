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

double Trainer::Train(double percentage,
                            const std::string& data_file_path,
                            const std::string& key_file_path,
                            std::function<void (const Data &, size_t)> train_handler,
                            std::function<double (const Data &)> answer_handler,
                            bool log) {
    
    //Train all networks
    size_t index = 0;
    size_t correct = 0;
    
    size_t all_records = RecordsInFile(key_file_path);
    size_t train_limit = all_records * (percentage / 100.0);
    size_t validate_limit = all_records - train_limit;
    
    //Set attribute for logging
    if (log) { std::cout << std::fixed; }
    
    for (DataIterator data(data_file_path), results(key_file_path) ;
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
                << "correct: "
                << correct / static_cast<double>(index - train_limit) * 100
                << "%\t\t\toverall progress: " << (index - train_limit) / static_cast<double>(validate_limit) * 100.0
                << "%\n";
            
        }
    }
    
    return correct / static_cast<double>(validate_limit);
}

double Trainer::Test(const std::string &test_file_path,
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

        if (log && index % (all_values / 100) == 0)
            std::cout
            << std::setprecision(3)
            << "correct: "
            << correct / static_cast<double>(index) * 100
            << "%\t\t\toverall progress: " << index / static_cast<double>(all_values) * 100.0
            << "%\n";
    }

    return correct / static_cast<double>(all_values) * 100.0;
}
