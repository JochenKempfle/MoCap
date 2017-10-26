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


#ifndef SENSORDATATYPE_H
#define SENSORDATATYPE_H


#include <initializer_list>
#include <string>
#include <set>

// sensor data type identification helper class for sensor data
class SensorDataType
{
  public:
    SensorDataType() = default;
    SensorDataType(const SensorDataType &other) = default;
    SensorDataType(std::initializer_list<int> initList);
    SensorDataType(int id);

    void operator=(std::initializer_list<int> initList);

    bool operator==(const SensorDataType &other) const;
    // operator < for sorting when used as a key (e.g. in a std::map)
    bool operator<(const SensorDataType &other) const;
    bool operator<=(const SensorDataType &other) const;
    bool isSubsetOf(const SensorDataType &other) const;
    bool isSupersetOf(const SensorDataType &other) const;

    size_t numIds() const { return _typeIds.size(); }

    std::vector<std::string> getTypeNames() const;

  private:
    std::set<int> _typeIds;
};

#endif // SENSORDATATYPE_H
