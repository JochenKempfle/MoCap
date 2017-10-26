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


#include "SensorBuffer.h"
#include "SensorNode.h"


SensorBufferChannel::SensorBufferChannel() : _sourceId(-1), _targetId(-1)
{

}

SensorBufferChannel::SensorBufferChannel(int sourceId, int targetId) : _sourceId(sourceId), _targetId(targetId)
{

}

/*
SensorBufferChannel& SensorBufferChannel::operator+=(const SensorBufferChannel& other)
{
    _sourceIds.insert(other._sourceIds.begin(), other._sourceIds.end());
    return *this;
}

SensorBufferChannel SensorBufferChannel::operator+(const SensorBufferChannel& other)
{
    return SensorBufferChannel(*this) += other;
}
*/

bool SensorBufferChannel::operator==(const SensorBufferChannel& other) const
{
    return _targetId == other._targetId && _sourceId == other._sourceId;
}

bool SensorBufferChannel::operator<(const SensorBufferChannel &other) const
{
    // SensorBufferChannels are sorted by target ID, then by source ID
    if (_targetId != other._targetId)
    {
        return _targetId < other._targetId;
    }
    // targets equal? Check source IDs!
    return _sourceId < other._sourceId;
}

bool SensorBufferChannel::operator<=(const SensorBufferChannel &other) const
{
    // SensorBufferChannels are sorted by target ID, then by source ID
    if (_targetId != other._targetId)
    {
        return _targetId <= other._targetId;
    }
    // targets equal? Check source IDs!
    return _sourceId <= other._sourceId;
}

bool SensorBufferChannel::operator>(const SensorBufferChannel &other) const
{
    return other < *this;
}

bool SensorBufferChannel::operator>=(const SensorBufferChannel &other) const
{
    return other <= *this;
}

bool SensorBufferChannel::hasSameSource(const SensorBufferChannel &other) const
{
    return _sourceId == other._sourceId;
}

bool SensorBufferChannel::hasSameTarget(const SensorBufferChannel &other) const
{
    return _targetId == other._targetId;
}




SensorBuffer::SensorBuffer()
{
    _provider = nullptr;
}

SensorBuffer::~SensorBuffer()
{

}

void SensorBuffer::subscribe(DataProvider* provider, int channel)
{
    if (_provider == provider)
    {
        return;
    }
    unsubscribe();
    if (provider == nullptr)
    {
        return;
    }
    if (sizeTotal() > 0)
    {
        // just in case: clear the buffer to avoid having old data from previous provider
        clear();
    }
    _provider = provider;
    // setChannel before calling addBuffer, as the channel gets read from this
    _channel = channel;
    provider->addBuffer(this);
}

void SensorBuffer::unsubscribe()
{
    if (_provider == nullptr)
    {
        return;
    }
    _provider->removeBuffer(this);
    _provider = nullptr;
    clear();
}



