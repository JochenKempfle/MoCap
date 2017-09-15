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

SensorData::SensorData()
{
    //ctor
}

SensorData::SensorData(unsigned int timestamp/*, uint64_t receiveTime, int dataType*/) : _timestamp(timestamp)//, _receiveTime(receiveTime), _dataType(dataType)
{

}

SensorData::~SensorData()
{
    //dtor
}

SensorDataOrientation::SensorDataOrientation()
{

}

SensorDataOrientation::SensorDataOrientation(const SensorDataOrientation &other) : _orientation(other._orientation)
{

}

SensorDataOrientation::SensorDataOrientation(const Quaternion &orientation) : _orientation(orientation)
{

}

SensorDataPosition::SensorDataPosition()
{

}

SensorDataPosition::SensorDataPosition(const SensorDataPosition &other) : _position(other._position)
{

}

SensorDataPosition::SensorDataPosition(const Vector3 &position) : _position(position)
{

}

SensorDataAcceleration::SensorDataAcceleration()
{

}

SensorDataAcceleration::SensorDataAcceleration(const SensorDataAcceleration &other) : _acceleration(other._acceleration)
{

}

SensorDataAcceleration::SensorDataAcceleration(const Vector3 &acc) : _acceleration(acc)
{

}

SensorDataGyroscope::SensorDataGyroscope()
{

}

SensorDataGyroscope::SensorDataGyroscope(const SensorDataGyroscope &other) : _gyroscope(other._gyroscope)
{

}

SensorDataGyroscope::SensorDataGyroscope(const Vector3 &gyro) : _gyroscope(gyro)
{

}

SensorDataMagnetometer::SensorDataMagnetometer()
{

}

SensorDataMagnetometer::SensorDataMagnetometer(const SensorDataMagnetometer &other) : _magnetometer(other._magnetometer)
{

}

SensorDataMagnetometer::SensorDataMagnetometer(const Vector3 &magnet) : _magnetometer(magnet)
{

}
