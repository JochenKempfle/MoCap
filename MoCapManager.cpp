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


#include "MoCapManager.h"

#include "SensorManager.h"
#include "MotionFilterNone.h"
#include "MotionFilterSlerp.h"
#include "AnimationManager.h"

MoCapManager::MoCapManager()
{
    _state = RECORD;
    // create a skeleton
    createDefaultSkeleton();
    _filters.push_back(new MotionFilterNone(&_skeleton));
    _filters.push_back(new MotionFilterSlerp(&_skeleton));
    _recording = false;
    _currentFilter = 1;
}

MoCapManager::~MoCapManager()
{
    //dtor
}

MoCapManager* MoCapManager::_moCapManager = nullptr;

MoCapManager& MoCapManager::getInstance()
{
    if (_moCapManager == nullptr)
    {
        _moCapManager = new MoCapManager();
    }
    return *_moCapManager;
}

void MoCapManager::assignSensorToBone(int sensorId, int boneId)
{
    // TODO(JK#1#): assign sensor to bone is sometimes buggy (assigns multiple sensors to same bone) rewrite!
    if (sensorId < 0)
    {
        return;
    }
    else if (boneId < 0)
    {
        removeSensorFromBones(sensorId);
        return;
    }
    // get the sensor which was assigned to chosen bone before and remove it from the bone
    removeSensorFromBones(sensorId);
    int prevSensorId = getSensorIdFromBoneId(boneId);
    removeSensorFromBones(prevSensorId);

    _boneIdFromSensorId[sensorId] = boneId;
    _sensorIdFromBoneId[boneId] = sensorId;
    theSensorManager.setSensorStateHasBone(sensorId, true);
    theSensorManager.getSensor(sensorId)->setBoneId(boneId);
    // TODO(JK#1#): set sensors to filter only when recording, also only set sensors with bone id
    _filters[_currentFilter]->setSensors(theSensorManager.getSensors());
}

void MoCapManager::removeSensorFromBones(int sensorId)
{
    auto it = _boneIdFromSensorId.find(sensorId);
    if (it != _boneIdFromSensorId.end())
    {
        _sensorIdFromBoneId.erase(it->second);
        _boneIdFromSensorId.erase(sensorId);
        theSensorManager.getSensor(sensorId)->setBoneId(-1);
        theSensorManager.setSensorStateHasBone(sensorId, false);
    }
}

int MoCapManager::getSensorIdFromBoneId(int boneId)
{
    // TODO(JK#1#): remove _sensorIdFromBoneId, as there is no use for this, this may fix the buggy assign sensor behaviour
    auto it = _sensorIdFromBoneId.find(boneId);
    if (it != _sensorIdFromBoneId.end())
    {
        return it->second;
    }
    return -1;
}

int MoCapManager::getBoneIdFromSensorId(int sensorId)
{
    auto it = _boneIdFromSensorId.find(sensorId);
    if (it != _boneIdFromSensorId.end())
    {
        return it->second;
    }
    return -1;
}

void MoCapManager::setBoneName(int boneId, std::string name)
{
    _skeleton.setBoneName(boneId, name);
}

std::vector<int> MoCapManager::getBoneIds()
{
    return _skeleton.getBoneIds();
}

std::vector<std::pair<int, std::string> > MoCapManager::getBoneIdsWithName()
{
    return _skeleton.getBoneIdsWithName();
}

std::string MoCapManager::getBoneName(int id)
{
    return _skeleton.getBoneName(id);
}

void MoCapManager::resetSkeleton()
{
    _skeleton.setToDefault();
    // TODO(JK#9#): rewrite bone associations (i.e. sensor-bone ids and names) for now it works but with custom skeletons this will cause trouble!
}

void MoCapManager::calibrate()
{
    std::vector<SensorNode*> sensors = theSensorManager.getSensors();
    for (size_t i = 0; i < sensors.size(); ++i)
    {
        Quaternion rotationOffset = sensors[i]->getRotation().inv();
        sensors[i]->setRotationOffset(rotationOffset.normalize());
        // theSensorManager.setSensorStateCalibrated(sensors[i]->getId(), true);

        // TODO(JK#4#): calibration! If bone.dir does not match, rotate 180° around y
    }
}

void MoCapManager::setSensorBoneMapping()
{
    std::vector<SensorNode*> sensors = theSensorManager.getSensors();
    for (size_t i = 0; i < sensors.size(); ++i)
    {
        Bone* bone = _skeleton.getBone(sensors[i]->getBoneId());
        if (bone == nullptr)
        {
            continue;
        }
        Quaternion mapped = sensors[i]->getRotation().normalized();
        mapped = sensors[i]->getRotationOffset() * mapped;// * sensors[i]->getRotationOffset();
        mapped.normalize();
        mapped = bone->getDefaultOrientation().inv() * mapped;
        mapped.normalize();
        sensors[i]->setBoneMapping(mapped.inv());

        theSensorManager.setSensorStateCalibrated(sensors[i]->getId(), true);
    }
}

void MoCapManager::startSimulation()
{
    _filters[_currentFilter]->setSensors(theSensorManager.getSensors());
    _filters[_currentFilter]->start();
}

void MoCapManager::stopSimulation()
{
    _filters[_currentFilter]->stop();
}

void MoCapManager::selectFilter(int filter)
{
    if (filter < 0 || filter >= _filters.size())
    {
        return;
    }
    _currentFilter = filter;
}

void MoCapManager::startRecording(uint64_t startTime, float frameTime)
{
    // disable synchronizing in order to have a consistent recording
    // _filters[_currentFilter]->setSensors(theSensorManager.getSensors());
    theSensorManager.setSynchronizing(false);
    _filters[_currentFilter]->setFrameTime(frameTime);
    _filters[_currentFilter]->setStartTime(startTime);
    _filters[_currentFilter]->setRecording(true);
    _recording = true;
}

MotionSequence* MoCapManager::stopRecording()
{
    if (!_recording)
    {
        return nullptr;
    }
    // enable synchronizing again
    theSensorManager.setSynchronizing(true);
    _recording = false;
    _filters[_currentFilter]->setRecording(false);
    MotionSequence* sequence = new MotionSequence(_filters[_currentFilter]->getSequence());
    // TODO(JK#1#): set recorded MotionSequence details (name, frameTime, numFrames etc in appropriate function eg in the filter)
    sequence->setName("recording");
    //sequence->setFrameTime(0.01f);
    theAnimationManager.addProjectSequence(sequence);
    return sequence;
}

void MoCapManager::update()
{
    // TODO(JK#1#): don't use a recording filter for update, maybe use special filter
    //_filters[_currentFilter]->update();
    return;
    std::vector<SensorNode*> sensors = theSensorManager.getSensors();
    for (size_t i = 0; i < sensors.size(); ++i)
    {
        int boneId = getBoneIdFromSensorId(sensors[i]->getId());
        // update skeleton from sensor data ( sensor data is always an absolute orientation)
        _skeleton.setAbsBoneOrientation(boneId, sensors[i]->getCalRotation());
    }

    // as for now only the new rotations were set, update the skeleton to also get new bone positions
    _skeleton.update();
}

void MoCapManager::createDefaultSkeleton()
{
    _skeleton.clear();

    Bone boneData;
    // hip - this will be the anchor of the skeleton
    boneData.setStartPos(Vector3(0.0, 1.0, 0.0));
    boneData.setLength(0.001);
    boneData.setDefaultOrientation(Quaternion(Vector3(0.0, 0.0, 1.0), M_PI*90.0/180.0) * Quaternion(Vector3(-1.0, 0.0, 0.0), M_PI*90.0/180.0));
    int hip = _skeleton.createBone(boneData);
    setBoneName(hip, "hip");

    // hip - neck (i.e. the body)
    boneData.setLength(0.55);
    boneData.setDefaultOrientation(Quaternion(Vector3(0.0, 0.0, 1.0), M_PI*90.0/180.0) * Quaternion(Vector3(-1.0, 0.0, 0.0), M_PI*90.0/180.0));
    int neck = _skeleton.createBone(boneData, hip);
    setBoneName(neck, "body");

    // neck - head
    boneData.setLength(0.30);
    boneData.setDefaultOrientation(Quaternion(Vector3(0.0, 0.0, 1.0), M_PI*90.0/180.0) * Quaternion(Vector3(-1.0, 0.0, 0.0), M_PI*90.0/180.0));
    int head = _skeleton.createBone(boneData, neck);
    setBoneName(head, "head");

    // neck - right shoulder
    boneData.setLength(0.20);
    boneData.setDefaultOrientation(Quaternion(1.0, 0.0, 0.0, 0.0));
    int rShoulder = _skeleton.createBone(boneData, neck);
    setBoneName(rShoulder, "rShoulder");

    // right upper arm
    boneData.setLength(0.35);
    boneData.setDefaultOrientation(Quaternion(1.0, 0.0, 0.0, 0.0));
    int rUpperArm = _skeleton.createBone(boneData, rShoulder);
    setBoneName(rUpperArm, "rUpperArm");

    // right lower arm
    boneData.setLength(0.30);
    boneData.setDefaultOrientation(Quaternion(1.0, 0.0, 0.0, 0.0));
    int rLowerArm = _skeleton.createBone(boneData, rUpperArm);
    setBoneName(rLowerArm, "rLowerArm");

    // neck - left shoulder
    boneData.setLength(0.20);
    boneData.setDefaultOrientation(Quaternion(Vector3(0.0, 1.0, 0.0), M_PI*180.0/180.0));
    int lShoulder = _skeleton.createBone(boneData, neck);
    setBoneName(lShoulder, "lShoulder");

    // left upper arm
    boneData.setLength(0.35);
    boneData.setDefaultOrientation(Quaternion(Vector3(0.0, 1.0, 0.0), M_PI*180.0/180.0));
    int lUpperArm = _skeleton.createBone(boneData, lShoulder);
    setBoneName(lUpperArm, "lUpperArm");

    // left lower arm
    boneData.setLength(0.30);
    boneData.setDefaultOrientation(Quaternion(Vector3(0.0, 1.0, 0.0), M_PI*180.0/180.0));
    int lLowerArm = _skeleton.createBone(boneData, lUpperArm);
    setBoneName(lLowerArm, "lLowerArm");

    // hip - right leg
    boneData.setLength(0.20);
    boneData.setDefaultOrientation(Quaternion(1.0, 0.0, 0.0, 0.0));
    int rHip = _skeleton.createBone(boneData, hip);
    setBoneName(rHip, "rHip");

    // right upper leg
    boneData.setLength(0.45);
    boneData.setDefaultOrientation(Quaternion(Vector3(0.0, 0.0, -1.0), M_PI*90.0/180.0) * Quaternion(Vector3(-1.0, 0.0, 0.0), M_PI*90.0/180.0));
    int rUpperLeg = _skeleton.createBone(boneData, rHip);
    setBoneName(rUpperLeg, "rUpperLeg");

    // right lower leg
    boneData.setLength(0.45);
    boneData.setDefaultOrientation(Quaternion(Vector3(0.0, 0.0, -1.0), M_PI*90.0/180.0) * Quaternion(Vector3(-1.0, 0.0, 0.0), M_PI*90.0/180.0));
    int rLowerLeg = _skeleton.createBone(boneData, rUpperLeg);
    setBoneName(rLowerLeg, "rLowerLeg");

    // hip - left leg
    boneData.setLength(0.20);
    boneData.setDefaultOrientation(Quaternion(Vector3(0.0, 1.0, 0.0), M_PI*180.0/180.0));
    int lHip = _skeleton.createBone(boneData, hip);
    setBoneName(lHip, "lHip");

    // left upper leg
    boneData.setLength(0.45);
    boneData.setDefaultOrientation(Quaternion(Vector3(0.0, 0.0, -1.0), M_PI*90.0/180.0) * Quaternion(Vector3(1.0, 0.0, 0.0), M_PI*90.0/180.0));
    int lUpperLeg = _skeleton.createBone(boneData, lHip);
    setBoneName(lUpperLeg, "lUpperLeg");

    // left lower leg
    boneData.setLength(0.45);
    boneData.setDefaultOrientation(Quaternion(Vector3(0.0, 0.0, -1.0), M_PI*90.0/180.0) * Quaternion(Vector3(1.0, 0.0, 0.0), M_PI*90.0/180.0));
    int lLowerLeg = _skeleton.createBone(boneData, lUpperLeg);
    setBoneName(lLowerLeg, "lLowerLeg");

    _skeleton.setName("Default Skeleton");
    _skeleton.update();
    //_skeleton.setCurrentAsDefault();
}


