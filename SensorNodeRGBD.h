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


#ifndef SENSORNODERGBD_H
#define SENSORNODEBASERGBD_H

#include "SensorNode.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "SensorRawData.h"
#include "SensorData.h"
#include "SensorBuffer.h"
#include <string>
#include <list>
#include <vector>


// TODO(JK#1#2017-10-11): adjust SensorNodeRGBD!!
class SensorNodeRGBD : public SensorNode
{
  public:
    SensorNodeRGBD(int id, std::string IPAddress);
    SensorNodeRGBD();
    virtual ~SensorNodeRGBD();

    virtual void calibrate(int step);

    virtual void applyCalibration(SensorData* data);

    Vector3 getPosition() const { return _position; }
    void setPosition(const Vector3 &position) { _position = position; }
    // void setPosition(float x, float y, float z) { _rotation = Vector3f(x, y, z); }

    // returns the relative rotation to the offset
    virtual Quaternion getCalRotation() const;// { return _rotationOffset * _rotation; }

    SensorDataType getOutputDataType() const { return SensorDataOrientation::getType(); }
  protected:
    virtual void onUpdate(SensorData* data);

  private:
    Vector3 _position;
};

#endif // SENSORNODERGBD_H

