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


#include "SensorNodeIMU.h"
#include "MoCapManager.h"
#include <limits>

SensorNodeIMU::SensorNodeIMU(int id, std::string name) : SensorNode(id, name)
{
    // _coordinateMapping = Quaternion(Vector3(1.0, 0.0, 0.0), -M_PI*90.0/180.0);
}

SensorNodeIMU::SensorNodeIMU() : SensorNode()
{
    // _coordinateMapping = Quaternion(Vector3(1.0, 0.0, 0.0), -M_PI*90.0/180.0);
}

SensorNodeIMU::~SensorNodeIMU()
{
    //dtor
}

void SensorNodeIMU::onUpdate(SensorData* data)
{
    /* text file logging
    if (_boneId >= 0)
    {
        Quaternion rotation = data.getOrientation();
        wxString msg;
        msg << data.getTimestamp() << _(" ") << _boneId << _(" ") << rotation.u() << _(" ") << rotation.x() << _(" ") << rotation.y() << _(" ") << rotation.z();
        wxLogDebug(msg);
    }
    */
    SensorDataOrientation* orientationData = dynamic_cast<SensorDataOrientation*>(data);
    if (orientationData == nullptr)
    {
        return;
    }

    // adjust quaternion to local coordinate system (OpenGL coordinate system instead of sensor's world coordinate system)
    Quaternion rotation = _coordinateMapping * orientationData->getOrientation() * _coordinateMapping.inv();
    orientationData->setOrientation(rotation.normalize());
    // TODO(JK#1#2017-07-04): get rid of sensor rotation
    setRotation(rotation);

    updateBuffers(orientationData, getId());
/*
    for (size_t i = 0; i < _buffers.size(); ++i)
    {
        // TODO(JK#9#): what sensor data to push_back in the buffer? calRotation? - solved with received rotation
        //_buffers[i]->lock();
        _buffers[i]->push_back(orientationData);
        //_buffers[i]->unlock();
    }
    */
}

void SensorNodeIMU::calibrate(int step)
{
    if (step == 0)
    {
        _coordinateOffset = getRotation().inv().normalize();
    }
    else if (step == 1)
    {
        Bone* bone = theMoCapManager.getSkeleton()->getBone(_boneId);
        if (bone == nullptr)
        {
            return;
        }
        Quaternion offset = getCoordinateOffset() * getRotation().normalized();
        offset.normalize();
        offset = offset.inv() * bone->getDefaultOrientation();
        setBoneOffset(offset.normalized());
    }
    else
    {
        return;
    }
}

void SensorNodeIMU::applyCalibration(SensorData* data)
{
    SensorDataOrientation* orientationData = dynamic_cast<SensorDataOrientation*>(data);

    if (orientationData != nullptr)
    {
        orientationData->setOrientation(((_coordinateOffset * orientationData->getOrientation()) * _boneOffset).normalize());
    }
}

Quaternion SensorNodeIMU::getCalRotation() const
{
    // Quaternion x(1.0, 0.0, 0.0, M_PI*90.0/180.0);
    // Quaternion offset = _coordinateOffset * _mapping;
    // return offset * _rotation * offset.inv();
    Quaternion q = _coordinateOffset * getRotation();
    return /*_boneMapping */ q * _boneOffset;
//    return _boneMapping * _coordinateOffset.inv() * _rotation;// * _coordinateOffset;
}

Vector3 SensorNodeIMU::toEuler() const
{
    return getRotation().toEuler();
}




