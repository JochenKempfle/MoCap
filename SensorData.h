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


#ifndef SENSORDATA_H
#define SENSORDATA_H

#include "Quaternion.h"

class SensorData
{
  public:
    SensorData();
    SensorData(uint64_t receiveTime, int timestamp, const Quaternion &orientation);
    virtual ~SensorData();

    void setTimestamp(unsigned int timestamp) { _timestamp = timestamp; }
    unsigned int getTimestamp() const { return _timestamp; }

    void setReceiveTime(uint64_t receiveTime) { _receiveTime = receiveTime; }
    uint64_t getReceiveTime() const { return _receiveTime; }

    void setOrientation(Quaternion orientation) { _orientation = orientation; }
    Quaternion getOrientation() const { return _orientation; }

  protected:

  private:
    uint64_t _receiveTime;
    unsigned int _timestamp;
    Quaternion _orientation;
};

#endif // SENSORDATA_H
