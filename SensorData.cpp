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


#include "SensorData.h"
#include "SensorDataFactory.h"


SensorData::SensorData()
{
    //ctor
}

SensorData::SensorData(unsigned int timestamp/*, uint64_t receiveTime, int dataType*/) : _timestamp(timestamp)//, _receiveTime(receiveTime), _dataType(dataType)
{

}

SensorData::SensorData(const SensorData &other) : _timestamp(other._timestamp)//, _receiveTime(receiveTime), _dataType(dataType)
{

}

SensorData::~SensorData()
{
    //dtor
}



// register sensors
REGISTER_SENSOR_DATA_TYPE("Orientation", SensorDataOrientation)
REGISTER_SENSOR_DATA_TYPE("Position", SensorDataPosition)
REGISTER_SENSOR_DATA_TYPE("Acceleration", SensorDataAcceleration)
REGISTER_SENSOR_DATA_TYPE("Gyroscope", SensorDataGyroscope)
REGISTER_SENSOR_DATA_TYPE("Magnetometer", SensorDataMagnetometer)
/*
REGISTER_SENSOR_DATA_TYPE("Height", SensorDataHeight)
REGISTER_SENSOR_DATA_TYPE("Distance", SensorDataDistance)
REGISTER_SENSOR_DATA_TYPE("RGB", SensorDataImageRGB)
REGISTER_SENSOR_DATA_TYPE("RGB-D", SensorDataImageDepth)
REGISTER_SENSOR_DATA_TYPE("PointCloud", SensorDataPointCloud)
*/

// "pure" sensor data type implementations
SensorDataOrientation::SensorDataOrientation()
{

}

SensorDataOrientation::SensorDataOrientation(const SensorData* data) : SensorData(*data),
        _orientation(dynamic_cast<const SensorDataOrientation*>(data) == nullptr ? Quaternion() : dynamic_cast<const SensorDataOrientation*>(data)->_orientation)
{

}

SensorDataOrientation::SensorDataOrientation(const SensorData& data) : SensorData(data),
        _orientation(dynamic_cast<const SensorDataOrientation*>(&data) == nullptr ? Quaternion() : dynamic_cast<const SensorDataOrientation*>(&data)->_orientation)
{

}

SensorDataOrientation::SensorDataOrientation(const SensorDataOrientation &other) : SensorData(other), _orientation(other._orientation)
{

}

SensorDataOrientation::SensorDataOrientation(const Quaternion &orientation) : _orientation(orientation)
{

}

SensorDataPosition::SensorDataPosition()
{

}

SensorDataPosition::SensorDataPosition(const SensorData* data) : SensorData(*data),
        _position(dynamic_cast<const SensorDataPosition*>(data) == nullptr ? Vector3() : dynamic_cast<const SensorDataPosition*>(data)->_position)
{

}

SensorDataPosition::SensorDataPosition(const SensorData& data) : SensorData(data),
        _position(dynamic_cast<const SensorDataPosition*>(&data) == nullptr ? Vector3() : dynamic_cast<const SensorDataPosition*>(&data)->_position)
{

}

SensorDataPosition::SensorDataPosition(const SensorDataPosition &other) : SensorData(other), _position(other._position)
{

}

SensorDataPosition::SensorDataPosition(const Vector3 &position) : _position(position)
{

}

SensorDataAcceleration::SensorDataAcceleration()
{

}

SensorDataAcceleration::SensorDataAcceleration(const SensorData* data) : SensorData(*data),
        _acceleration(dynamic_cast<const SensorDataAcceleration*>(data) == nullptr ? Vector3() : dynamic_cast<const SensorDataAcceleration*>(data)->_acceleration)
{

}

SensorDataAcceleration::SensorDataAcceleration(const SensorData &data) : SensorData(data),
        _acceleration(dynamic_cast<const SensorDataAcceleration*>(&data) == nullptr ? Vector3() : dynamic_cast<const SensorDataAcceleration*>(&data)->_acceleration)
{

}

SensorDataAcceleration::SensorDataAcceleration(const SensorDataAcceleration &other) : SensorData(other), _acceleration(other._acceleration)
{

}

SensorDataAcceleration::SensorDataAcceleration(const Vector3 &acc) : _acceleration(acc)
{

}

SensorDataGyroscope::SensorDataGyroscope()
{

}

SensorDataGyroscope::SensorDataGyroscope(const SensorData* data) : SensorData(*data),
        _gyroscope(dynamic_cast<const SensorDataGyroscope*>(data) == nullptr ? Vector3() : dynamic_cast<const SensorDataGyroscope*>(data)->_gyroscope)
{

}

SensorDataGyroscope::SensorDataGyroscope(const SensorData &data) : SensorData(data),
        _gyroscope(dynamic_cast<const SensorDataGyroscope*>(&data) == nullptr ? Vector3() : dynamic_cast<const SensorDataGyroscope*>(&data)->_gyroscope)
{

}

SensorDataGyroscope::SensorDataGyroscope(const SensorDataGyroscope &other) : SensorData(other), _gyroscope(other._gyroscope)
{

}

SensorDataGyroscope::SensorDataGyroscope(const Vector3 &gyro) : _gyroscope(gyro)
{

}

SensorDataMagnetometer::SensorDataMagnetometer()
{

}

SensorDataMagnetometer::SensorDataMagnetometer(const SensorData* data) : SensorData(*data),
        _magnetometer(dynamic_cast<const SensorDataMagnetometer*>(data) == nullptr ? Vector3() : dynamic_cast<const SensorDataMagnetometer*>(data)->_magnetometer)
{

}

SensorDataMagnetometer::SensorDataMagnetometer(const SensorData &data) : SensorData(data),
        _magnetometer(dynamic_cast<const SensorDataMagnetometer*>(&data) == nullptr ? Vector3() : dynamic_cast<const SensorDataMagnetometer*>(&data)->_magnetometer)
{

}

SensorDataMagnetometer::SensorDataMagnetometer(const SensorDataMagnetometer &other) : SensorData(other), _magnetometer(other._magnetometer)
{

}

SensorDataMagnetometer::SensorDataMagnetometer(const Vector3 &magnet) : _magnetometer(magnet)
{

}

