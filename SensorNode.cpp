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


#include "SensorNode.h"
#include "MoCapManager.h"
#include <limits>

SensorNode::SensorNode(int id, std::string name) : _id(id), _name(name)
{
    _boneId = -1;
    _state = 0;
    // _coordinateMapping = Quaternion(Vector3(1.0, 0.0, 0.0), -M_PI*90.0/180.0);
    _startTime = std::numeric_limits<uint64_t>::max();
    _synchronizing = true;
    _numReceivedPackets = 0;
    _numLostPackets = 0;
    _currentTimeStamp = 0;
    _frameTime = 0.0f;
    _currentFrameTime = 0.1f;
    _delay = 0;
    _currentDelay = 0;
}

SensorNode::SensorNode() : _id(-1)
{
    _boneId = -1;
    _state = 0;
    // _coordinateMapping = Quaternion(Vector3(1.0, 0.0, 0.0), -M_PI*90.0/180.0);
    _startTime = std::numeric_limits<uint64_t>::max();
    _synchronizing = true;
    _numReceivedPackets = 0;
    _numLostPackets = 0;
    _currentTimeStamp = 0;
    _frameTime = 0.0f;
    _currentFrameTime = 0.1f;
    _delay = 0;
    _currentDelay = 0;
}

SensorNode::~SensorNode()
{
    //dtor
}

void SensorNode::setBoneId(int boneId)
{
    _boneId = boneId;
    setHasBone(boneId >= 0);
}

void SensorNode::update(SensorData* data)
{
    /*
    auto it = _buffer.end();
    while (it != _buffer.begin())
    {
        --it;
        if (it->getTimestamp() < data->getTimestamp())
        {
            ++it;
            break;
        }
    }
    _buffer.insert(it, data);
    */

    // adjust the start time
    if (_synchronizing)
    {
        if (data->getReceiveTime() < _startTime + data->getTimestamp() || _startTime == std::numeric_limits<uint64_t>::max())
        {
            _startTime = data->getReceiveTime() - data->getTimestamp();
        }
    }

    _lastReceiveTime = data->getReceiveTime();


    int deltaTimestamp = data->getTimestamp() - _currentTimeStamp;

    if (deltaTimestamp > 0)
    {
        _currentFrameTime = float(deltaTimestamp)/1000.0f;
        _currentTimeStamp = data->getTimestamp();
    }
    _currentDelay = _lastReceiveTime - _startTime - _currentTimeStamp;
    ++_numReceivedPackets;

    // TODO(JK#1#2017-07-12): In SensorNode: guess frame time and delay without having a buffer

    // check if the sensor was offline in the meantime and restarted from the beginning
    if (data->getTimestamp() < 0.5f * _currentTimeStamp)
    {
        _startTime = data->getReceiveTime() - data->getTimestamp();
        _numReceivedPackets = 0;
        _numLostPackets = 0;
        _currentTimeStamp = 0;
        _frameTime = 0.0f;
        _currentFrameTime = 0.1f;
        _delay = 0;
        _currentDelay = 0;
    }

    //_currentFrameTime = float(deltaTimestamp) / 1000.0f;

    // assuming the sensor has a constant frame rate, a packet was lost when the current frame time is nearly double the average frame time
    /*
    int lostPackets = 0;
    if (_currentFrameTime > _frameTime * 1.8f)
    {
        lostPackets = int(round((_currentFrameTime - _frameTime) / _frameTime));
        _currentFrameTime /= float(lostPackets + 1);
        //++_numLostPackets;
    }
    _numLostPackets += lostPackets;
    */
/*
    // correct unreasonable estimated frame times
    if (_currentFrameTime * 1000.0f * _numReceivedPackets > data.getTimestamp())
    {
        _currentFrameTime = float(data.getTimestamp()) / (1000 * _numReceivedPackets);
    }
*/

    if (_frameTime <= 0.0f)
    {
        _frameTime = _currentFrameTime;
    }
    _frameTime = 0.98f * _frameTime + 0.02f * _currentFrameTime;

    _delay = 0.98f * _delay + 0.02f * _currentDelay;

    // sensor update is done here (as implemented by child class)
    onUpdate(data);
/*
    for (size_t i = 0; i < _buffers.size(); ++i)
    {
        // TODO(JK#9#): what sensor data to push_back in the buffer? calRotation? - solved with received rotation
        //_buffers[i]->lock();
        _buffers[i]->push_back(data);
        //_buffers[i]->unlock();
    }
*/
    // setUpdated(true);

    // TODO(JK#9#): implement some logic for a state update! (done?)
}


int SensorNode::getNumLostPackets() const
{
    int lost = _currentTimeStamp / (_frameTime * 1000.0f) - _numReceivedPackets;
    return lost < 0 ? 0 : lost;
}


void SensorNode::addBuffer(SensorBuffer* buffer)
{
    for (size_t i = 0; i < _buffers.size(); ++i)
    {
        if (_buffers[i] == buffer)
        {
            return;
        }
    }
    _buffers.push_back(buffer);
}

void SensorNode::removeBuffer(SensorBuffer* buffer)
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

