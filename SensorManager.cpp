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


#include "SensorManager.h"
#include "SensorNodeIMU.h"
#include "SensorNodeRGBD.h"

SensorManager::SensorManager()
{
    _nextId = 0;
//    _currentBufferPos = -1;
//    _bufferSize = 0;
    //ctor
}

SensorManager::~SensorManager()
{
    //dtor
}

SensorManager* SensorManager::_sensorManager = nullptr;

SensorManager& SensorManager::getInstance()
{
    if (_sensorManager == nullptr)
    {
        _sensorManager = new SensorManager();
    }
    return *_sensorManager;
}

bool SensorManager::updateSensor(std::string name, SensorData* data, uint64_t receiveTime)
{
    auto it = _sensorFromName.find(name);
    if (it != _sensorFromName.end())
    {
        it->second->update(data, receiveTime);
        return true;
    }
    return false;
    /*
    else
    {
        int id = createSensorNode(name);
        if (id >= 0)
        {
            it = _sensorFromName.find(name);
            it->second->update(data);
        }
        else
        {
            // uh-oh sensor could not be created!
        }
    }
    */
}

bool SensorManager::updateSensor(int id, SensorData* data, uint64_t receiveTime)
{
    auto it = _sensors.find(id);
    if (it != _sensors.end())
    {
        it->second->update(data, receiveTime);
        return true;
    }
    return false;
}

/*
bool SensorManager::setSensorOffset(int id, const Quaternion &rotation)
{
    auto it = _sensors.find(id);
    if (it != _sensors.end())
    {
        it->second->setRotationOffset(rotation);
        return true;
    }
    return false;
}
*/

std::vector<int> SensorManager::getMoving(double minDegree)
{
    // TODO(JK#2#2017-07-03): getMoving (or auto assign) leads to a SW crash - rewrite
    std::vector<int> moving;
    double rotationThreshold = fabs(M_PI * minDegree/180.0);
    for (size_t i = 0; i < _previousSensorData.size(); ++i)
    {
        // int id = _previousSensorData[i].getId();
        auto it = _sensors.find(i);
        if (it == _sensors.end())
        {
            continue;
        }
        //Vector3 difference = _previousSensorData[i].getRotation().toEuler() - it->second.getRotation().toEuler();
        // difference.normalize();
        //float angle = difference.norm();
        double angle = _previousSensorData[i].getShortestAngleTo(it->second->getRotation());
        if (angle >= rotationThreshold)
        {
            moving.push_back(i);
        }
    }
    //_previousSensorData = getSensors();
    // TODO(JK#2#): handle get previous sensor data with the sensor buffer
    _previousSensorData.clear();
    for (size_t i = 0; i < getSensors().size(); ++i)
    {
        _previousSensorData.push_back(getSensors()[i]->getRotation());
    }
    return moving;
}

SensorNode* SensorManager::getSensor(int id)
{
    auto it = _sensors.find(id);
    if (it != _sensors.end())
    {
        return it->second;
    }
    return nullptr;
}

void SensorManager::resetAllSensorStatesUpdated()
{
    for (auto it = _sensors.begin(); it != _sensors.end(); ++it)
    {
        it->second->setUpdated(false);
    }
}

void SensorManager::setSensorStateCalibrated(int id, bool calibrated)
{
    auto it = _sensors.find(id);
    if (it != _sensors.end())
    {
        it->second->setCalibrated(calibrated);
    }
}

void SensorManager::setSynchronizing(bool sync)
{
    for (auto it = _sensors.begin(); it != _sensors.end(); ++it)
    {
        it->second->setSynchronizing(sync);
    }
}

int SensorManager::getSensorIdFromName(std::string name)
{
    auto it = _sensorFromName.find(name);
    if (it != _sensorFromName.end())
    {
        return it->second->getId();
    }
    return -1;
}

std::vector<SensorNode*> SensorManager::getSensors()
{
    std::vector<SensorNode*> sensors;
    sensors.reserve(_sensors.size());
    for (auto it = _sensors.begin(); it != _sensors.end(); ++it)
    {
        sensors.push_back(it->second);
    }
    return sensors;
}



