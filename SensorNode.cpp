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

SensorNode::SensorNode(int id, std::string IPAddress) : _id(id), _IPAddress(IPAddress)
{
    _boneId = -1;
    _state = 0;
    _coordinateMapping = Quaternion(Vector3(1.0, 0.0, 0.0), -M_PI*90.0/180.0);
    _startTime = std::numeric_limits<uint64_t>::max();
    _synchronizing = true;
    _numReceivedPackets = 0;
    _numLostPackets = 0;
    _currentTimeStamp = 0;
    _frameTime = 0.0f;
    _delay = 0;
}

SensorNode::SensorNode() : _id(-1)
{
    _boneId = -1;
    _state = 0;
    _coordinateMapping = Quaternion(Vector3(1.0, 0.0, 0.0), -M_PI*90.0/180.0);
    _startTime = std::numeric_limits<uint64_t>::max();
    _synchronizing = true;
    _numReceivedPackets = 0;
    _numLostPackets = 0;
    _currentTimeStamp = 0;
    _frameTime = 0.0f;
    _delay = 0;
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

void SensorNode::update(const SensorRawData &rawData)
{
    // TODO(JK#9#): implement some kind of filtering here (do this in a Motion filter!)
    _rotation.u() = rawData.rotation[0];
    _rotation.x() = rawData.rotation[1];
    _rotation.y() = rawData.rotation[2];
    _rotation.z() = rawData.rotation[3];
    // TODO(JK#9#): remove sensor coordinate mapping from SensorNode::update() as soon as there is a valid mapping routine
    // Quaternion x(Vector3(1.0, 0.0, 0.0), -M_PI*90.0/180.0);
    // Quaternion y(Vector3(0.0, 1.0, 0.0), M_PI*180.0/180.0);
    // x = y*x;
    _rotation = _coordinateMapping * _rotation * _coordinateMapping.inv();
    _rotation.normalize();

    auto it = _buffer.end();
    while (it != _buffer.begin())
    {
        --it;
        if (it->getTimestamp() < rawData.timestamp)
        {
            ++it;
            break;
        }
    }
    _buffer.insert(it, SensorData(rawData.receiveTime, rawData.timestamp, _rotation));

    // adjust the start time
    if (_synchronizing)
    {
        if (rawData.receiveTime < _startTime + rawData.timestamp || _startTime == std::numeric_limits<uint64_t>::max())
        {
            _startTime = rawData.receiveTime - rawData.timestamp;
        }
    }

    _lastReceiveTime = rawData.receiveTime;
    _currentTimeStamp = _buffer.back().getTimestamp();
    ++_numReceivedPackets;

    if (_buffer.size() < 10)
    {
        // make first guesses
        if (_buffer.size() == 9)
        {
//            int deltaTimestamp = std::numeric_limits<int>::max();
//            auto it = _buffer.begin();
//            int currentTimestamp = it->getTimestamp();
//            ++it;
//            while (it != _buffer.end())
//            {
//                if (int(it->getTimestamp()) - currentTimestamp < deltaTimestamp && int(it->getTimestamp()) - currentTimestamp > 0)
//                {
//                    deltaTimestamp = it->getTimestamp() - currentTimestamp;
//                }
//                currentTimestamp = it->getTimestamp();
//                ++it;
//            }
            _frameTime = float(_buffer.back().getTimestamp() - _buffer.front().getTimestamp()) / (9.0f * 1000.0f);
            _delay = _lastReceiveTime - _startTime - _currentTimeStamp;
        }
        return;
    }

    SensorData data = _buffer.front();
    _buffer.pop_front();

    int deltaTimestamp = _buffer.front().getTimestamp() - data.getTimestamp();

    // check if the sensor was offline in the meantime and restarted from the beginning
    if (deltaTimestamp > 10 * 1000 * _frameTime && data.getTimestamp() < 0.5f * 1000.0f * _frameTime * _numReceivedPackets)
    {
        _startTime = data.getReceiveTime() - data.getTimestamp();
        _numReceivedPackets = 0;
        _numLostPackets = 0;
        _currentTimeStamp = 0;
        _frameTime = 0.0f;
        _buffer.clear();
        _buffer.push_back(data);
        return;
    }


    _currentFrameTime = float(_buffer.back().getTimestamp() - data.getTimestamp()) / (10.0f * 1000.0f);

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

    _delay = 0.98f * _delay + 0.02f * (_lastReceiveTime - _startTime - _currentTimeStamp);


    for (size_t i = 0; i < _buffers.size(); ++i)
    {
        // TODO(JK#9#): what sensor data to push_back in the buffer? calRotation? - solved with received rotation
        //_buffers[i]->lock();
        _buffers[i]->push_back(data);
        //_buffers[i]->unlock();
    }


    // setUpdated(true);

    // TODO(JK#5#): update position of sensor node
/*    _position.x() = data.position[0];
    _position.y() = data.position[1];
    _position.z() = data.position[2];
*/
    // TODO(JK#9#): implement some logic for a state update! (done?)
}

Quaternion SensorNode::getCalRotation() const
{
    // Quaternion x(1.0, 0.0, 0.0, M_PI*90.0/180.0);
    // Quaternion offset = _rotationOffset * _mapping;
    // return offset * _rotation * offset.inv();
    Quaternion q = _rotationOffset * _rotation;
    return /*_boneMapping */ q * _boneMapping;
//    return _boneMapping * _rotationOffset.inv() * _rotation;// * _rotationOffset;
}

int SensorNode::getNumLostPackets() const
{
    int lost = _currentTimeStamp / (_frameTime * 1000.0f) - _numReceivedPackets;
    return lost < 0 ? 0 : lost;
}

Vector3 SensorNode::toEuler() const
{
    return _rotation.toEuler();
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

