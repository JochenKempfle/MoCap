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


#ifndef SENSORNODE_H
#define SENSORNODE_H

#include "Vector3.h"
#include "Quaternion.h"
#include "SensorRawData.h"
#include "SensorData.h"
#include "SensorBuffer.h"
#include <string>
#include <list>
#include <vector>




enum SensorState
{
    NONE = 0,
    UPDATED = 1,
    HAS_BONE = 2,
    CALIBRATED = 4
    // TODO(JK#5#): add appropriate states (done?)
};

class SensorNode
{
  public:
    SensorNode(int id, std::string IPAddress);
    SensorNode();
    virtual ~SensorNode();

    void update(const SensorRawData &data);

    int getId() const { return _id; }
    std::string getIPAddress() const { return _IPAddress; }

    void setBoneId(int boneId);
    int getBoneId() const { return _boneId; }

    unsigned char getState() const { return _state; }
    void setState(unsigned char state) { _state = state; }

    void setUpdated(bool value = true) { _state = (value ? _state | SensorState::UPDATED : _state & ~SensorState::UPDATED); }
    void setHasBone(bool value = true) { _state = (value ? _state | SensorState::HAS_BONE : _state & ~SensorState::HAS_BONE); }
    void setCalibrated(bool value = true) { _state = (value ? _state | SensorState::CALIBRATED : _state & ~SensorState::CALIBRATED); }

    // TODO(JK#4#): handle SensorState somehow else, depending on member variables (i.e. buffer size, _boneId > 0 etc.)
    //bool isUpdated() const { return _state & SensorState::UPDATED ? true : false; }
    bool hasBone() const { return _state & SensorState::HAS_BONE ? true : false; }
    bool isCalibrated() const { return _state & SensorState::CALIBRATED ? true : false; }

    Vector3 getPosition() const { return _position; }
    void setPosition(const Vector3 &position) { _position = position; }
    // void setPosition(float x, float y, float z) { _rotation = Vector3f(x, y, z); }

    Quaternion getRotation() const { return _rotation; }
    void setRotation(const Quaternion &rotation) { _rotation = rotation; }
    // void setRotation(float u, float x, float y, float z) { _rotation = Quaternionf(u, x, y, z); }

    void setMapping(const Quaternion &mapping) { _coordinateMapping = mapping; }
    Quaternion getMapping() const { return _coordinateMapping; }

    void setBoneMapping(const Quaternion &mapping) { _boneMapping = mapping; }
    Quaternion getBoneMapping() const { return _boneMapping; }

    // returns the relative rotation to the offset
    Quaternion getCalRotation() const;// { return _rotationOffset * _rotation; }

    Quaternion getRotationOffset() const { return _rotationOffset; }
    void setRotationOffset(const Quaternion &offset) { _rotationOffset = offset; }

    uint64_t getStartTime() const { return _startTime; }
    uint64_t getLastReceiveTime() const { return _lastReceiveTime; }

    float getFrameTime() const { return _frameTime; } // float(_buffer.back().getTimestamp() - _buffer.front().getTimestamp()) / (_buffer.size() * 1000); }
    float getCurrentFrameTime() const { return _currentFrameTime; }

    int getDelay() const { return _delay; } // (_lastReceiveTime - _startTime - _currentTimeStamp)/10; }

    unsigned int getNumReceivedPackets() const { return _numReceivedPackets; }
    int getNumLostPackets() const;
    unsigned int getCurrentTimeStamp() const { return _currentTimeStamp; }

    void setSynchronizing(bool sync = true) { _synchronizing = sync; }
    bool isSynchronizing() const { return _synchronizing; }

    // returns the rotation in euler angles, using the 1-2-3 convention
    Vector3 toEuler() const;

    void addBuffer(SensorBuffer* buffer);
    void removeBuffer(SensorBuffer* buffer);

  private:
    int _id;
    int _boneId;
    unsigned char _state;
    std::string _IPAddress;
    Quaternion _rotation;
    Quaternion _rotationOffset;
    Quaternion _boneOffset;
    // how to map from sensor coordinates to simulation coordinates
    Quaternion _coordinateMapping;
    Quaternion _boneMapping;
    Vector3 _position;
    // the global start time in ms
    uint64_t _startTime;
    uint64_t _lastReceiveTime;
    float _frameTime;
    float _currentFrameTime;
    int _delay;

    unsigned int _numReceivedPackets;
    int _numLostPackets;
    unsigned int _currentTimeStamp;

    bool _synchronizing;

    std::list<SensorData> _buffer;
    std::vector<SensorBuffer*> _buffers;
};

#endif // SENSORNODE_H

