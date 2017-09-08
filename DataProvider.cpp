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


#include "DataProvider.h"
#include "SensorBuffer.h"

DataProvider::DataProvider()
{
    //ctor
}

DataProvider::~DataProvider()
{
    //dtor
}

bool DataProvider::hasBuffer(SensorBuffer* buffer) const
{
    for (size_t i = 0; i < _buffers.size(); ++i)
    {
        if (_buffers[i] == buffer)
        {
            return true;
        }
    }
    return false;
}

void DataProvider::updateBuffers(SensorData* data)
{
    for (size_t i = 0; i < _buffers.size(); ++i)
    {
        // TODO(JK#9#): what sensor data to push_back in the buffer? calRotation? - solved with received rotation
        //_buffers[i]->lock();
        _buffers[i]->push_back(data);
        //_buffers[i]->unlock();
    }
}

void DataProvider::addBuffer(SensorBuffer* buffer)
{
    if (buffer == nullptr)
    {
        return;
    }
    for (size_t i = 0; i < _buffers.size(); ++i)
    {
        if (_buffers[i] == buffer)
        {
            return;
        }
    }
    _buffers.push_back(buffer);
}

void DataProvider::removeBuffer(SensorBuffer* buffer)
{
    for (auto it = _buffers.begin(); it != _buffers.end(); ++it)
    {
        if (*it == buffer)
        {
            _buffers.erase(it);
            return;
        }
    }
}
