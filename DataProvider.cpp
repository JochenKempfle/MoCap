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

bool DataProvider::hasChannel(int channel) const
{
    auto it = _buffers.find(channel);
    return it != _buffers.end();
}

std::vector<int> DataProvider::getChannelIds() const
{
    std::vector<int> ids;
    for (auto it = _buffers.begin(); it != _buffers.end(); ++it)
    {
        ids.push_back(it->first);
    }
    return ids;
}

bool DataProvider::hasBuffer(SensorBuffer* buffer) const
{
    auto it = _buffers.find(buffer->getChannel());
    if (it == _buffers.end())
    {
        return false;
    }
    for (size_t i = 0; i < it->second.size(); ++i)
    {
        if (it->second[i] == buffer)
        {
            return true;
        }
    }
    return false;
}

size_t DataProvider::numBuffers(int channel) const
{
    auto it = _buffers.find(channel);
    if (it == _buffers.end())
    {
        return 0;
    }
    return it->second.size();
}

void DataProvider::updateBuffers(SensorData* data, int channel)
{
    auto it = _buffers.find(channel);
    if (it == _buffers.end())
    {
        return;
    }
    for (size_t i = 0; i < it->second.size(); ++i)
    {
        // TODO(JK#9#): what sensor data to push_back in the buffer? calRotation? - solved with received rotation
        //_buffers[i]->lock();
        it->second[i]->push_back(data);
        //_buffers[i]->unlock();
    }
}

void DataProvider::addBuffer(SensorBuffer* buffer)
{
    if (buffer == nullptr)
    {
        return;
    }
    auto it = _buffers.find(buffer->getChannel());
    if (it == _buffers.end())
    {
        _buffers[buffer->getChannel()].push_back(buffer);
    }
    else
    {
        for (size_t i = 0; i < it->second.size(); ++i)
        {
            if (it->second[i] == buffer)
            {
                return;
            }
        }
        it->second.push_back(buffer);
    }
}

void DataProvider::removeBuffer(SensorBuffer* buffer)
{
    auto channelIt = _buffers.find(buffer->getChannel());
    if (channelIt == _buffers.end())
    {
        return;
    }
    for (auto it = channelIt->second.begin(); it != channelIt->second.end(); ++it)
    {
        if (*it == buffer)
        {
            channelIt->second.erase(it);
            return;
        }
    }
}
