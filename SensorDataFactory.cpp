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


#include "SensorDataFactory.h"
#include "SensorData.h"
#include "SensorBuffer.h"


SensorDataFactory* SensorDataFactory::_instance = nullptr;

// std::vector<std::pair<std::string, CreateRecvFunc> > SensorDataFactory::_receiverList;

SensorDataFactory::SensorDataFactory()
{

}

size_t SensorDataFactory::getNumSensorDataTypes()
{
    return getInstance()->_sensorDataTypeList.size();
}

std::string SensorDataFactory::getSensorDataTypeName(unsigned int type)
{
    return getInstance()->_sensorDataTypeList[type].first;
}

SensorData* SensorDataFactory::createSensorDataType(unsigned int type)
{
    return getInstance()->_sensorDataTypeList[type].second();
}

SensorBuffer* SensorDataFactory::createSensorBuffer(unsigned int type)
{
    auto it = getInstance()->_sensorBufferMap.find(type);
    if (it == getInstance()->_sensorBufferMap.end())
    {
        return nullptr;
    }
    return it->second();
}

const int SensorDataFactory::registerSensorDataType(std::string name, CreateSensorDataTypeFunc dataFunc, CreateSensorBufferFunc bufFunc)
{
    int typeId = getInstance()->_sensorDataTypeList.size();
    getInstance()->_sensorDataTypeList.push_back(std::make_pair(name, dataFunc));
    getInstance()->_sensorBufferMap[SensorDataType(typeId)] = bufFunc;
    return typeId;
}

const int SensorDataFactory::registerSensorDataCompositionType(SensorDataType type, CreateSensorBufferFunc bufFunc)
{
    getInstance()->_sensorBufferMap[type] = bufFunc;
    return getInstance()->_sensorDataTypeList.size();
}

SensorDataFactory* SensorDataFactory::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = new SensorDataFactory();
    }
    return _instance;
}
