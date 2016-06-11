//
//  DataIterator.cpp
//  Neural
//
//  Created by Maxim Vainshtein on 03/06/2016.
//  Copyright © 2016 Maxim Vainshtein. All rights reserved.
//

#include "DataIterator.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>

using namespace neural;

size_t neural::RecordsInFile(const std::string &file_path) {
    
    std::ifstream myfile(file_path);
    
    // new lines will be skipped unless we stop it from happening:
    myfile.unsetf(std::ios_base::skipws);
    
    // count the newlines with an algorithm specialized for counting:
    return std::count(std::istream_iterator<char>(myfile), std::istream_iterator<char>(), '\n');
}

/**
 * Implementation.
 */
class DataIterator::Impl {
public:
    
    Impl(const std::string& file_path);
    
    void Next();
    
    Data Value() const;
    
    bool Valid() const;
    
    
private:
    
    ///Stores the file stream that reads the input file
    std::fstream m_file_stream;
    
    ///Stores the current value that has been read
    std::string m_value;
    
};

#pragma mark - Implementation

DataIterator::Impl::Impl(const std::string& file_path) :
m_file_stream(file_path) {
    
    //The iterator starts an the first position that has a value
    Next();
}

void DataIterator::Impl::Next() {
    std::getline(m_file_stream, m_value);
}

Data DataIterator::Impl::Value() const {
    
    Data data;
    
    std::string value;
    std::stringstream string_stream(m_value);
    
    //Get every number seperated by ','
    while (std::getline(string_stream, value, ','))
        data.content.push_back(std::stoi(value));
    
    return data;
}

bool DataIterator::Impl::Valid() const {
    return !m_value.empty();
}

#pragma mark - Data functions

DataIterator::DataIterator(const std::string& file_path) :
m_pimpl(new Impl(file_path))
{ }

DataIterator::~DataIterator() = default;

Data DataIterator::Value() const {
    return m_pimpl->Value();
}

void DataIterator::Next() {
    m_pimpl->Next();
}

bool DataIterator::Valid() const {
    return m_pimpl->Valid();
}
