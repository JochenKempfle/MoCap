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


#ifndef SENSORNODEBASE_H
#define SENSORNODEBASE_H

#include "Vector3.h"
#include "Quaternion.h"
#include "SensorRawData.h"
#include "SensorData.h"
#include "SensorBuffer.h"
#include "DataProvider.h"
#include <string>
#include <list>
#include <vector>

enum SensorState
{
    NONE = 0,
    UPDATED = 1,
    HAS_BONE = 2,
    CALIBRATED = 4,
    INTERMEDIATE = 8
    // TODO(JK#5#): add appropriate states (done?)
};

class SensorNode : public DataProvider
{
  public:
    SensorNode(int id, std::string name);
    SensorNode();
    virtual ~SensorNode();

    void update(SensorData* data, uint64_t receiveTime);

    virtual void calibrate(int step) = 0;

    virtual void applyCalibration(SensorData* data) = 0;

    int getId() const { return _id; }
    std::string getName() const { return _name; }

    void setBoneId(int boneId);
    int getBoneId() const { return _boneId; }

    void setState(unsigned char state) { _state = state; }
    unsigned char getState() const { return _state; }

    void setUpdated(bool value = true) { _state = (value ? _state | SensorState::UPDATED : _state & ~SensorState::UPDATED); }
    // void setHasBone(bool value = true) { _state = (value ? _state | SensorState::HAS_BONE : _state & ~SensorState::HAS_BONE); }
    void setCalibrated(bool value = true) { _state = (value ? _state | SensorState::CALIBRATED : _state & ~SensorState::CALIBRATED); }
    void setIntermediate(bool value = true) { _state = (value ? _state | SensorState::INTERMEDIATE : _state & ~SensorState::INTERMEDIATE); }

    // TODO(JK#4#): handle SensorState somehow else, depending on member variables (i.e. buffer size, _boneId > 0 etc.)
    //bool isUpdated() const { return _state & SensorState::UPDATED ? true : false; }
    bool hasBone() const { return _boneId >= 0; }
    bool isCalibrated() const { return _state & SensorState::CALIBRATED ? true : false; }

    void setRotation(const Quaternion &rotation) { _rotation = rotation; }
    Quaternion getRotation() const { return _rotation; }

    // TODO(JK#1#2017-09-13): remove getCalRotation from sensor node!
    // returns the relative rotation to the offset
    virtual Quaternion getCalRotation() const = 0;

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


    // void addBuffer(SensorBuffer* buffer);
    // void removeBuffer(SensorBuffer* buffer);

    // TODO(JK#2#2017-06-29): allow a sensor node to reset its calibration values and to set it individually
    // TODO(JK#2#2017-06-29): allow a sensor node to use one sensor for whole coordinate mapping
  protected:
    virtual void onUpdate(SensorData* data) = 0;

    int _boneId;
    // std::vector<SensorBuffer*> _buffers;

    // TODO(JK#1#2017-10-20): intermediate sensor nodes require source info (source sensor IDs)

    // TODO(JK#1#2017-07-04): get rid of sensor rotation!
    Quaternion _rotation;

  private:
    int _id;
    unsigned char _state;
    std::string _name;

    // the global start time in ms
    uint64_t _startTime;
    uint64_t _lastReceiveTime;
    float _frameTime;
    float _currentFrameTime;
    int _delay;
    int _currentDelay;

    unsigned int _numReceivedPackets;
    int _numLostPackets;
    unsigned int _currentTimeStamp;

    bool _synchronizing;
};

#endif // SENSORNODEBASE_H

