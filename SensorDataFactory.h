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


#ifndef SENSORDATAFACTORY_H
#define SENSORDATAFACTORY_H

#include "SensorDataType.h"
#include <map>


class SensorData;
class SensorBuffer;

typedef SensorData* (*CreateSensorDataTypeFunc)();

typedef SensorBuffer* (*CreateSensorBufferFunc)();

class SensorDataFactory
{
  public:
    static size_t getNumSensorDataTypes();
    static std::string getSensorDataTypeName(unsigned int type);

    static SensorData* createSensorDataType(unsigned int type);
    static SensorBuffer* createSensorBuffer(unsigned int type);

    static const int registerSensorDataType(std::string name, CreateSensorDataTypeFunc dataFunc, CreateSensorBufferFunc bufFunc);
    static const int registerSensorDataCompositionType(SensorDataType type, CreateSensorBufferFunc bufFunc);

  protected:
    SensorDataFactory();

    static SensorDataFactory* getInstance();
    static SensorDataFactory* _instance;

    std::vector<std::pair<std::string, CreateSensorDataTypeFunc> > _sensorDataTypeList;
    std::map<SensorDataType, CreateSensorBufferFunc> _sensorBufferMap;
};

#endif // SENSORDATAFACTORY_H
