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


#ifndef DATACHANNEL_H
#define DATACHANNEL_H

#include "SensorData.h"
#include "SensorBuffer.h"
#include "DataProvider.h"


class DataChannel : public DataProvider
{
  public:
    DataChannel();
    virtual ~DataChannel();

    virtual void subscribe(DataProvider* provider) = 0;

    virtual std::vector<SensorDataType> getInputDataTypes() const = 0;

    virtual size_t getNumInputBuffers() const = 0;
    virtual SensorBuffer* getInputBuffer(SensorDataType type) = 0;
    virtual std::vector<SensorBuffer*> getInputBuffers() = 0;
};



template <class OUTPUT, class... INPUTS>
class DataChannelTyped
{
  public:
    DataChannelTyped();
    virtual ~DataChannelTyped();

    void subscribe(DataProvider* provider);

    std::vector<SensorDataType> getOutputDataType() const { return _outputType; }
    std::vector<SensorDataType> getInputDataTypes() const { return _inputTypes; }

    static std::vector<SensorDataType> getOutputDataTypeStatic() { return _outputType; }
    static std::vector<SensorDataType> getInputDataTypesStatic() { return _inputTypes; }

    virtual size_t getNumInputBuffers() const { return _inputs.size(); }

    SensorBuffer* getInputBuffer(SensorDataType type);
    std::vector<SensorBuffer*> getInputBuffers();

    // std::map<SensorDataType, SensorBuffer*>::iterator beginInput() { return _inputs.begin(); }
    // std::map<SensorDataType, SensorBuffer*>::iterator endInput() { return _inputs.end(); }

  private:
    int _id;
    std::map<SensorDataType, SensorBuffer*> _inputs;

    static const SensorDataType _outputType;
    static const std::vector<SensorDataType> _inputTypes;
};


template <class OUTPUT, class... INPUTS>
DataChannelTyped<OUTPUT, INPUTS...>::DataChannelTyped() : _inputs({{INPUTS::getType(), new SensorBufferType<INPUTS>()}...}) // _outType(OUTPUT::getType()), _inTypes({INPUTS::getType()...})
{
    static_assert(isBaseOfSensorData<OUTPUT>{}, "DataChannel output type must be of base SensorData!");
    static_assert(isBaseOfSensorData<INPUTS...>{}, "DataChannel input types must be of base SensorData!");
}

template <class OUTPUT, class... INPUTS>
DataChannelTyped<OUTPUT, INPUTS...>::~DataChannelTyped()
{
    // delete pointers!
    for (auto it = _inputs.begin(); it != _inputs.end(); ++it)
    {
        delete it->second;
    }
}


template <class OUTPUT, class... INPUTS>
SensorBuffer* DataChannelTyped<OUTPUT, INPUTS...>::getInputBuffer(SensorDataType type)
{
    auto it = _inputs.find(type);
    if (it == _inputs.end())
    {
        return nullptr;
    }
    return it->second;
}

template <class OUTPUT, class... INPUTS>
std::vector<SensorBuffer*> DataChannelTyped<OUTPUT, INPUTS...>::getInputBuffers()
{
    std::vector<SensorBuffer*> buffers;
    for (auto it = _inputs.begin(); it != _inputs.end(); ++it)
    {
        buffers.push_back(it->second);
    }
    return buffers;
}



#endif // DATACHANNEL_H
