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


#ifndef SENSORMANAGER_H
#define SENSORMANAGER_H

#include "SensorNode.h"
#include "SensorRawData.h"
#include "Quaternion.h"
#include <map>
#include <vector>
#include <string>


#define theSensorManager SensorManager::getInstance()

class SensorManager
{
  public:
    SensorManager();
    virtual ~SensorManager();

    static SensorManager& getInstance();

    template <class T>
    int createSensorNode(std::string name);
    bool updateSensor(std::string name, SensorData* data, uint64_t receiveTime);
    bool updateSensor(int id, SensorData* data, uint64_t receiveTime);

    bool setSensorOffset(int id, const Quaternion &rotation);

    std::vector<int> getMoving(double minDegree);

    SensorNode* getSensor(int id);
    int getSensorIdFromName(std::string name);

    void resetAllSensorStatesUpdated();
    void setSensorStateCalibrated(int id, bool calibrated = true);

    void setSynchronizing(bool sync = true);

    std::vector<SensorNode*> getSensors();

  protected:
    static SensorManager* _sensorManager;

  private:
    int _nextId;
    std::map<std::string, SensorNode*> _sensorFromName;
    std::map<int, SensorNode*> _sensors;
    std::vector<Quaternion> _previousSensorData;
};


// template implementations

template <class T>
int SensorManager::createSensorNode(std::string name)
{
    if (!std::is_base_of<SensorNode, T>())
    {
        return -1;
    }
    int id = _nextId++;
    SensorNode* sensor = new T(id, name);

    if (sensor != nullptr)
    {
        _sensors[id] = sensor;
        _sensorFromName[name] = sensor;
        return id;
    }

    // no sensor node could be created
    --_nextId;
    return -1;
}

#endif // SENSORMANAGER_H
