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
#include <map>
#include <vector>
#include <queue>
#include <string>


#define theSensorManager SensorManager::getInstance()

class SensorManager
{
  public:
    SensorManager();
    virtual ~SensorManager();

    static SensorManager& getInstance();

    int createSensorNode(std::string IPAddress);
    void updateSensor(std::string IPAddress, const SensorRawData &data);
    bool updateSensor(int id, const SensorRawData &data);

    bool setSensorOffset(int id, const Quaternion &rotation);

    std::vector<int> getMoving(double minDegree);

    SensorNode* getSensor(int id);
    int getSensorIdFromIP(std::string IPAddress);

    void resetAllSensorStatesUpdated();
    void setSensorStateCalibrated(int id, bool calibrated = true);
    void setSensorStateHasBone(int id, bool hasBone = true);

    std::vector<SensorNode*> getSensors();

  protected:
    static SensorManager* _sensorManager;

  private:
    int _nextId;
    // TODO(JK#3#): check all occurences of getSensor() or related, as now a pointer to SensorNode is used
    std::map<std::string, SensorNode*> _sensorFromIP;
    // std::map<std::string, int> _sensorIdFromName;
    std::map<int, SensorNode*> _sensors;
    std::vector<SensorNode> _previousSensorData;
};


#endif // SENSORMANAGER_H
