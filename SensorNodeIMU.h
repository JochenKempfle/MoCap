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


#ifndef SENSORNODEIMU_H
#define SENSORNODEIMU_H

#include "SensorNode.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "SensorRawData.h"
#include "SensorData.h"
#include "SensorBuffer.h"
#include <string>
#include <list>
#include <vector>


class SensorNodeIMU : public SensorNode
{
  public:
    SensorNodeIMU(int id, std::string IPAddress);
    SensorNodeIMU();
    virtual ~SensorNodeIMU();

    virtual void calibrate(int step);

    virtual void applyCalibration(SensorData* data);

    virtual int getType() const { return IMU; }

    std::string getIPAddress() const { return _IPAddress; }

    void setPosition(const Vector3 &position) { _position = position; }
    Vector3 getPosition() const { return _position; }
    // void setPosition(float x, float y, float z) { _rotation = Vector3f(x, y, z); }

    Quaternion getMapping() const { return _coordinateMapping; }
    void setMapping(const Quaternion &mapping) { _coordinateMapping = mapping; }
    void resetMapping() { _coordinateMapping = Quaternion(); }

    Quaternion getBoneMapping() const { return _boneMapping; }
    void setBoneMapping(const Quaternion &mapping) { _boneMapping = mapping; }
    void resetBoneMapping() { _boneMapping = Quaternion(); }

    // returns the relative rotation to the offset
    virtual Quaternion getCalRotation() const;// { return _rotationOffset * _rotation; }

    Quaternion getRotationOffset() const { return _rotationOffset; }
    void setRotationOffset(const Quaternion &offset) { _rotationOffset = offset; }
    void resetRotationOffset() { _rotationOffset = Quaternion(); }

    // returns the rotation in euler angles, using the 1-2-3 convention
    Vector3 toEuler() const;

  protected:
    virtual void onUpdate(SensorData* data);

  private:
    std::string _IPAddress;
    // Quaternion _rotation;
    Quaternion _rotationOffset;
    Quaternion _boneOffset;
    // how to map from sensor coordinates to simulation coordinates
    Quaternion _coordinateMapping;
    Quaternion _boneMapping;
    Vector3 _position;
};

#endif // SENSORNODEIMU_H

