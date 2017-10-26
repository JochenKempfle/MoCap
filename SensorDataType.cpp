/*
Copyright (c) 2016, Jochen Kempfle
All rights reserved.


Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.


THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/


#include "SensorDataType.h"
#include "SensorDataFactory.h"
#include <vector>


SensorDataType::SensorDataType(std::initializer_list<int> initList) : _typeIds(initList)
{

}

SensorDataType::SensorDataType(int id) : _typeIds({id})
{

}

void SensorDataType::operator=(std::initializer_list<int> initList)
{
    _typeIds = initList;
}

bool SensorDataType::operator==(const SensorDataType &other) const
{
    if (_typeIds.size() != other._typeIds.size())
    {
        return false;
    }
    for (auto it1 = _typeIds.begin(), it2 = other._typeIds.begin(); it1 != _typeIds.end(); ++it1, ++it2)
    {
        if (*it1 != *it2)
        {
            return false;
        }
    }
    return true;
}

bool SensorDataType::operator<(const SensorDataType &other) const
{
    for (auto it1 = _typeIds.begin(), it2 = other._typeIds.begin(); it1 != _typeIds.end() && it2 != other._typeIds.end(); ++it1, ++it2)
    {
        if (*it1 != *it2)
        {
            return *it1 < *it2;
        }
    }
    // when so far all type ids were identical, check the size (smaller size means smaller type)
    return _typeIds.size() < other._typeIds.size();
}

bool SensorDataType::operator<=(const SensorDataType &other) const
{
    for (auto it1 = _typeIds.begin(), it2 = other._typeIds.begin(); it1 != _typeIds.end() && it2 != other._typeIds.end(); ++it1, ++it2)
    {
        if (*it1 != *it2)
        {
            return *it1 < *it2;
        }
    }
    // when so far all type ids were identical, check the size (smaller or equal size means smaller or equal type)
    return _typeIds.size() <= other._typeIds.size();
}

bool SensorDataType::isSubsetOf(const SensorDataType &other) const
{
    if (_typeIds.size() > other._typeIds.size())
    {
        return false;
    }
    auto it1 = _typeIds.begin();
    for (auto it2 = other._typeIds.begin(); it2 != _typeIds.end(); ++it2)
    {
        if (*it1 == *it2)
        {
            // equal -> go to next type ID
            ++it1;
        }
        else if (*it1 < *it2)
        {
            // type ID was not found in other (sorted) set -> no subset
            return false;
        }
        if (it1 == _typeIds.end())
        {
            // all type IDs were found in other -> subset
            return true;
        }
    }
    // some type IDs at the end were not found in other -> no subset
    return false;
}

bool SensorDataType::isSupersetOf(const SensorDataType &other) const
{
    return other.isSubsetOf(*this);
}

std::vector<std::string> SensorDataType::getTypeNames() const
{
    std::vector<std::string> names;
    names.reserve(_typeIds.size());
    for (auto it = _typeIds.begin(); it != _typeIds.end(); ++it)
    {
        names.push_back(SensorDataFactory::getSensorDataTypeName(*it));
    }
    return names;
}

