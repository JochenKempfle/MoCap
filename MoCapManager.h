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


#ifndef MoCapManager_H
#define MoCapManager_H

#include "Skeleton.h"
#include "Bone.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "MotionFilterBase.h"
#include <vector>
#include <string>
#include <map>

enum ProgramState
{
    EDITOR,
    SENSOR_INFO,
    RECORD,
    POST_PROCESS
    // TODO(JK#5#): add something like RECORDING, IDLE, CONNECTED to program state or own enum
};

//enum RenderStyle
//{
//    STANDARD = 0,
//    HIGHLIGHT_SELECTED_BONE = 1,
//    DRAW_GRID = 2,
//    DRAW_LOCAL_COORDINATE_SYSTEM = 4,
//    DRAW_SPIN_ARROWS = 8,
//    SELECTION_MODE = 16
//};

#define theMoCapManager MoCapManager::getInstance()

class MoCapManager
{
  public:
    MoCapManager();
    ~MoCapManager();

    static MoCapManager& getInstance();

    void assignSensorToBone(int sensorId, int boneId);
    void removeSensorFromBones(int sensorId);
    int getSensorIdFromBoneId(int boneId);
    int getBoneIdFromSensorId(int sensorId);

    void selectBone(int id) { _skeleton.selectBone(id); }
    void selectBone(Bone* bone) { _skeleton.selectBone(bone->getId()); }
    void unselectBone() { _skeleton.unselectBone(); }
    Bone* getSelectedBone() { return _skeleton.getSelectedBone(); }
    int getSelectedBoneId() const { return _skeleton.getSelectedBoneId(); }

    Skeleton* getSkeleton() { return &_skeleton; }

    // TODO(JK#9#): program state in MoCapManager necessary?
    void setState(ProgramState state) { _state = state; }
    unsigned char getState() const { return _state; }

    void setBoneName(int boneId, std::string name);

    std::vector<int> getBoneIds();
    std::vector<std::pair<int, std::string> > getBoneIdsWithName();
    std::string getBoneName(int id);

    void resetSkeleton();

    void autoAssign();
    void calibrate();

    void startRecording();
    void stopRecording();

    void update();

  protected:
    static MoCapManager* _moCapManager;

    void createSkeleton();

  private:
    std::map<int, int> _boneIdFromSensorId;
    std::map<int, int> _sensorIdFromBoneId;
    Skeleton _skeleton;
    int _selectedBoneId;

    std::vector<MotionFilterBase*> _filters;

    unsigned char _state;
};

#endif // MoCapManager_H
