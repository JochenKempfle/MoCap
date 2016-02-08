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

#include <GL/gl.h>
#include <GL/glu.h>
#include "SensorManager.h"

MoCapManager::MoCapManager()
{
    _selectedBoneId = -1;
    _state = RECORD;
    //_renderStyle = STANDARD;
    // create a skeleton
    _rShoulderId = _lShoulderId = _rHipId = _lHipId = -1;
    createSkeleton();
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
    if (sensorId < 0)
    {
        return;
    }
    else if (boneId < 0)
    {
        theMoCapManager.removeSensorFromBones(sensorId);
        return;
    }
    // get the sensor which was assigned to chosen bone before and remove it from the bone
    theMoCapManager.removeSensorFromBones(sensorId);
    int prevSensorId = theMoCapManager.getSensorIdFromBoneId(boneId);
    theMoCapManager.removeSensorFromBones(prevSensorId);

    _boneIdFromSensorId[sensorId] = boneId;
    _sensorIdFromBoneId[boneId] = sensorId;
    theSensorManager.setSensorStateHasBone(sensorId, true);
}

void MoCapManager::removeSensorFromBones(int sensorId)
{
    auto it = _boneIdFromSensorId.find(sensorId);
    if (it != _boneIdFromSensorId.end())
    {
        _sensorIdFromBoneId.erase(it->second);
        _boneIdFromSensorId.erase(sensorId);
        theSensorManager.setSensorStateHasBone(sensorId, false);
    }
}

int MoCapManager::getSensorIdFromBoneId(int boneId)
{
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

void MoCapManager::assignNameToBone(std::string name, int boneId)
{
    _boneNameFromId[boneId] = name;
}

std::vector<int> MoCapManager::getBoneIds()
{
    return _skeleton.getBoneIds();
}

std::vector<std::pair<int, std::string> > MoCapManager::getBoneIdsWithName()
{
    std::vector<std::pair<int, std::string> > bones;
    for (auto it = _boneNameFromId.begin(); it != _boneNameFromId.end(); ++it)
    {
        bones.push_back(*it);
    }
    return bones;
}

std::string MoCapManager::getBoneNameFromId(int id)
{
    std::string name;
    auto it = _boneNameFromId.find(id);
    if (it != _boneNameFromId.end())
    {
        name = it->second;
    }
    return name;
}

void MoCapManager::resetSkeleton()
{
    // TODO(JK#6#): provide some kind of skeleton.getDefault() routine
    _skeleton.clear();
    createSkeleton();
    // TODO(JK#9#): rewrite bone associations (i.e. sensor-bone ids and names) for now it works but with custom skeletons this will cause trouble!
}

void MoCapManager::calibrate()
{
    std::vector<SensorNode> sensors = theSensorManager.getSensors();
    for (size_t i = 0; i < sensors.size(); ++i)
    {
        Quaternion rotationOffset = sensors[i].getRotation().inv();
        if (theSensorManager.setSensorOffset(sensors[i].getId(), rotationOffset))
        {
            theSensorManager.setSensorStateCalibrated(sensors[i].getId(), true);
        }

        // TODO(JK#4#): calibration! If bone.dir does not match, rotate 180° around y
    }
}

void MoCapManager::update()
{
    std::vector<SensorNode> sensors = theSensorManager.getSensors();
    for (size_t i = 0; i < sensors.size(); ++i)
    {
        int boneId = getBoneIdFromSensorId(sensors[i].getId());
        _skeleton.setBoneRotation(boneId, sensors[i].getCalRotation());
    }
    // update static bones (bones without user access)
    // TODO(JK#8#): remove updates for static bones
    /*
    _skeleton.setBoneRotation(_rShoulderId, _skeleton.getBone(_rShoulderId)->getParent()->getAbsOrientation()
                * Quaternion(Vector3(0.0, 0.0, -1.0), M_PI*90.0/180.0) * Quaternion(Vector3(0.0, 1.0, 0.0), M_PI*90.0/180.0));

    _skeleton.setBoneRotation(_lShoulderId, _skeleton.getBone(_lShoulderId)->getParent()->getAbsOrientation()
                * Quaternion(Vector3(0.0, 0.0, -1.0), M_PI*90.0/180.0) * Quaternion(Vector3(0.0, -1.0, 0.0), M_PI*90.0/180.0));

    _skeleton.setBoneRotation(_rHipId, _skeleton.getBone(_rHipId)->getParent()->getAbsOrientation()
                * Quaternion(Vector3(0.0, 0.0, -1.0), M_PI*90.0/180.0) * Quaternion(Vector3(0.0, 1.0, 0.0), M_PI*90.0/180.0));

    _skeleton.setBoneRotation(_lHipId, _skeleton.getBone(_lHipId)->getParent()->getAbsOrientation()
                * Quaternion(Vector3(0.0, 0.0, -1.0), M_PI*90.0/180.0) * Quaternion(Vector3(0.0, -1.0, 0.0), M_PI*90.0/180.0));
    */
    // update not static bones without sensor
    // TODO(JK#1#): update bones without sensor
    // TODO(JK#9#): update upper leg bones without sensor somehow else, not this hardcoded way
/*    std::vector<int> bones = _skeleton.getBoneIds();
    for (size_t i = 0; i < bones.size(); ++i)
    {
        if (getSensorIdFromBoneId(bones[i]) < 0 && getBoneNameFromId(bones[i]).length() > 0)
        {
            Bone* bone = _skeleton.getBone(bones[i]);
            std::string name = getBoneNameFromId(bones[i]);
            if (bone->getParent() == nullptr)
            {
                continue;
            }
            else if (name.compare("rUpperLeg") == 0)
            {
                _skeleton.setBoneRotation(bones[i], bone->getParent()->getAbsOrientation() * Quaternion(Vector3(0.0, 0.0, -1.0), M_PI*90.0/180.0));
            }
            else if (name.compare("lUpperLeg") == 0)
            {
                _skeleton.setBoneRotation(bones[i], bone->getParent()->getAbsOrientation() * Quaternion(Vector3(0.0, 0.0, -1.0), M_PI*90.0/180.0));
            }
            else
            {
                _skeleton.setBoneRotation(bones[i], bone->getParent()->getAbsOrientation());
            }
        }
    }*/
    // as for now only the new rotations were set, update the skeleton to also get new bone positions
    _skeleton.update();
}

void MoCapManager::render()
{
/*    for (auto it = _skeleton.beginBones(); it != _skeleton.endBones(); ++it)
    {
        Bone bone = *(it->second);

        int boneId = bone.getId();

        // don't draw any bone without valid id
        if (boneId < 0)
        {
            continue;
        }

        GLubyte red[4] = {255, 0, 0, 255};
        GLubyte green[4] = {0, 255, 0, 255};
        GLubyte blue[4] = {0, 0, 255, 255};
        GLubyte black[4] = {0, 0, 0, 255};

        GLubyte pointColor[4] = {5, 15, 160, 255};
        GLubyte boneStandardColor1[4] = {128, 128, 128, 255};
        GLubyte boneStandardColor2[4] = {180, 180, 180, 255};
        GLubyte boneHighlightedColor1[4] = {100, 200, 100, 255};
        GLubyte boneHighlightedColor2[4] = {160, 255, 160, 255};

        // get the bone id as color for SELECTION_MODE
        GLubyte boneIdColor[4] = {GLubyte((boneId >> 8) & 255), GLubyte((boneId >> 8) & 255), GLubyte(boneId & 255), 255};

        const GLubyte* boneColor1 = boneStandardColor1;
        const GLubyte* boneColor2 = boneStandardColor2;

        if (_renderStyle & SELECTION_MODE)
        {
            boneColor1 = boneIdColor;
            boneColor2 = boneIdColor;
        }
        else if (_renderStyle & HIGHLIGHT_SELECTED_BONE && bone.getId() == _selectedBoneId)
        {
            boneColor1 = boneHighlightedColor1;
            boneColor2 = boneHighlightedColor2;
        }

        Vector3 startPos = bone.getStartPos();
        Vector3 endPos = bone.getEndPos();

        float length = bone.getLength();

        Vector3 dir = bone.getDirection();
        Vector3 up = bone.getUpDirection();
        Vector3 right = bone.getRightDirection();

        Vector3 upPoint = startPos + up*length*0.1 + dir*length*0.1;
        Vector3 downPoint = startPos - up*length*0.1 + dir*length*0.1;
        Vector3 rightPoint = startPos + right*length*0.1 + dir*length*0.1;
        Vector3 leftPoint = startPos - right*length*0.1 + dir*length*0.1;

        if (!(_renderStyle & SELECTION_MODE))
        {
            //set point size to 10 pixels
            glPointSize(10.0f);
            glColor4ubv(pointColor);

            // TODO(JK#9#): maybe don't draw points for bones (or add render style flag)
            glBegin(GL_POINTS);
                glVertex3f(endPos.x(), endPos.y(), endPos.z());
            glEnd();
        }

        // draw local coordinate system
        if (true)//_renderStyle & DRAW_LOCAL_COORDINATE_SYSTEM)
        {
            glBegin(GL_LINES);
                glColor4ubv(red);
                glVertex3f(startPos.x(), startPos.y(), startPos.z());
                glVertex3f(endPos.x(), endPos.y(), endPos.z());

                glColor4ubv(green);

                glVertex3f(endPos.x(), endPos.y(), endPos.z());
                glVertex3f(endPos.x() + up.x()*0.1, endPos.y() + up.y()*0.1, endPos.z() + up.z()*0.1);

                glColor4ubv(blue);

                glVertex3f(endPos.x(), endPos.y(), endPos.z());
                glVertex3f(endPos.x() + right.x()*0.1, endPos.y() + right.y()*0.1, endPos.z() + right.z()*0.1);
            glEnd();
        }

        // draw bone
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glColor4ubv(boneColor1);

        glBegin(GL_TRIANGLE_FAN);
            glVertex3f(startPos.x(), startPos.y(), startPos.z());
            glVertex3f(upPoint.x(), upPoint.y(), upPoint.z());
            glVertex3f(rightPoint.x(), rightPoint.y(), rightPoint.z());
            glVertex3f(downPoint.x(), downPoint.y(), downPoint.z());
            glVertex3f(leftPoint.x(), leftPoint.y(), leftPoint.z());
            glVertex3f(upPoint.x(), upPoint.y(), upPoint.z());
        glEnd();


        glColor4ubv(boneColor2);

        glBegin(GL_TRIANGLE_FAN);
            glVertex3f(endPos.x(), endPos.y(), endPos.z());
            glVertex3f(upPoint.x(), upPoint.y(), upPoint.z());
            glVertex3f(rightPoint.x(), rightPoint.y(), rightPoint.z());
            glVertex3f(downPoint.x(), downPoint.y(), downPoint.z());
            glVertex3f(leftPoint.x(), leftPoint.y(), leftPoint.z());
            glVertex3f(upPoint.x(), upPoint.y(), upPoint.z());
        glEnd();

        if (!(_renderStyle & SELECTION_MODE))
        {
            // draw black mesh lines around bones
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glColor4ubv(black);

            glBegin(GL_TRIANGLE_FAN);
                glVertex3f(startPos.x(), startPos.y(), startPos.z());
                glVertex3f(upPoint.x(), upPoint.y(), upPoint.z());
                glVertex3f(rightPoint.x(), rightPoint.y(), rightPoint.z());
                glVertex3f(downPoint.x(), downPoint.y(), downPoint.z());
                glVertex3f(leftPoint.x(), leftPoint.y(), leftPoint.z());
                glVertex3f(upPoint.x(), upPoint.y(), upPoint.z());
            glEnd();

            glBegin(GL_TRIANGLE_FAN);
                glVertex3f(endPos.x(), endPos.y(), endPos.z());
                glVertex3f(upPoint.x(), upPoint.y(), upPoint.z());
                glVertex3f(rightPoint.x(), rightPoint.y(), rightPoint.z());
                glVertex3f(downPoint.x(), downPoint.y(), downPoint.z());
                glVertex3f(leftPoint.x(), leftPoint.y(), leftPoint.z());
                glVertex3f(upPoint.x(), upPoint.y(), upPoint.z());
            glEnd();
        }

        if (_renderStyle & DRAW_SPIN_ARROWS && bone.getId() == _selectedBoneId)
        {
            drawSpinArrows(endPos, dir, up, right);
        }
    }*/
}

void MoCapManager::drawSpinArrows(Vector3 pos, Vector3 dir, Vector3 up, Vector3 right)
{
    float length = 0.03f;
    float offset = 0.05f;

    GLubyte red[4] = {255, 0, 0, 255};
    GLubyte green[4] = {0, 255, 0, 255};
    GLubyte blue[4] = {0, 0, 255, 255};
    GLubyte black[4] = {0, 0, 0, 255};

    // set arroe ids to next available free ids
    int idArrowDir = _skeleton.getNextFreeId();
    int idArrowUp = _skeleton.getNextFreeId() + 1;
    int idArrowRight = _skeleton.getNextFreeId() + 2;

    GLubyte idArrowDirColor[4] = {GLubyte((idArrowDir >> 8) & 255), GLubyte((idArrowDir >> 8) & 255), GLubyte(idArrowDir & 255), 255};
    GLubyte idArrowUpColor[4] = {GLubyte((idArrowUp >> 8) & 255), GLubyte((idArrowUp >> 8) & 255), GLubyte(idArrowUp & 255), 255};
    GLubyte idArrowRightColor[4] = {GLubyte((idArrowRight >> 8) & 255), GLubyte((idArrowRight >> 8) & 255), GLubyte(idArrowRight & 255), 255};

    const GLubyte* arrowDirColor = red;
    const GLubyte* arrowUpColor = green;
    const GLubyte* arrowRightColor = blue;

    // base points of the arrow pyramids
    Vector3 basePointUp = pos + up * offset;
    Vector3 basePointRight = pos + right * offset;
    Vector3 basePointDown = pos - up * offset;
    Vector3 basePointLeft = pos - right * offset;

    dir *= length;
    up *= length;
    right *= length;

    Vector3 dirHalf = dir * 0.5;
    Vector3 rightHalf = right * 0.5;

    // base vertices of the circle like arrow
    Vector3 upperCircle[4];
    upperCircle[0] = basePointDown;
    upperCircle[1] = basePointDown - right;
    upperCircle[2] = basePointLeft - up;
    upperCircle[3] = basePointLeft;

    Vector3 lowerCircle[4];
    lowerCircle[0] = basePointDown - up;
    lowerCircle[1] = basePointDown - right*1.4 - up;
    lowerCircle[2] = basePointLeft - right - up*1.4;
    lowerCircle[3] = basePointLeft - right;

    // the arrow rendering is done twice, one iteration for filling color, one for having black corner lines
    int numIterations = 2;
//    if (_renderStyle & SELECTION_MODE)
    {
        // do not draw the corner lines in selection mode
        numIterations = 1;
        // draw the arrows with their id color
        arrowDirColor = idArrowDirColor;
        arrowUpColor = idArrowUpColor;
        arrowRightColor = idArrowRightColor;
    }
    // draw vertices twice, one run with filling and one with black lines
    for (int j = 0; j < numIterations; ++j)
    {
        if (j == 0)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glColor4f(0.0, 0.0, 0.0, 1.0);
            arrowDirColor = arrowUpColor = arrowRightColor = black;
        }

        if (j == 0)
        {
            glColor4ubv(arrowUpColor);
        }
        glColor4ubv(arrowUpColor);

        // arrow pointing upwards
        glBegin(GL_TRIANGLE_FAN);
            glVertex3f(basePointUp.x() + up.x()*2.0, basePointUp.y() + up.y()*2.0, basePointUp.z() + up.z()*2.0);
            glVertex3f(basePointUp.x() + dir.x(), basePointUp.y() + dir.y(), basePointUp.z() + dir.z());
            glVertex3f(basePointUp.x() + right.x(), basePointUp.y() + right.y(), basePointUp.z() + right.z());
            glVertex3f(basePointUp.x() - dir.x(), basePointUp.y() - dir.y(), basePointUp.z() - dir.z());
            glVertex3f(basePointUp.x() - right.x(), basePointUp.y() - right.y(), basePointUp.z() - right.z());
            glVertex3f(basePointUp.x() + dir.x(), basePointUp.y() + dir.y(), basePointUp.z() + dir.z());
        glEnd();

        // arrow pointing downwards
    /*    glBegin(GL_TRIANGLE_FAN);
            glVertex3f(basePointDown.x() - up.x(), basePointDown.y() - up.y(), basePointDown.z() - up.z());
            glVertex3f(basePointDown.x() + dir.x(), basePointDown.y() + dir.y(), basePointDown.z() + dir.z());
            glVertex3f(basePointDown.x() + right.x(), basePointDown.y() + right.y(), basePointDown.z() + right.z());
            glVertex3f(basePointDown.x() - dir.x(), basePointDown.y() - dir.y(), basePointDown.z() - dir.z());
            glVertex3f(basePointDown.x() - right.x(), basePointDown.y() - right.y(), basePointDown.z() - right.z());
            glVertex3f(basePointDown.x() + dir.x(), basePointDown.y() + dir.y(), basePointDown.z() + dir.z());
        glEnd();
    */
        if (j == 0)
        {
            glColor4f(0.0, 0.0, 1.0, 1.0);
        }
        glColor4ubv(arrowRightColor);

        // arrow pointing to the right
        glBegin(GL_TRIANGLE_FAN);
            glVertex3f(basePointRight.x() + right.x()*2.0, basePointRight.y() + right.y()*2.0, basePointRight.z() + right.z()*2.0);
            glVertex3f(basePointRight.x() + dir.x(), basePointRight.y() + dir.y(), basePointRight.z() + dir.z());
            glVertex3f(basePointRight.x() - up.x(), basePointRight.y() - up.y(), basePointRight.z() - up.z());
            glVertex3f(basePointRight.x() - dir.x(), basePointRight.y() - dir.y(), basePointRight.z() - dir.z());
            glVertex3f(basePointRight.x() + up.x(), basePointRight.y() + up.y(), basePointRight.z() + up.z());
            glVertex3f(basePointRight.x() + dir.x(), basePointRight.y() + dir.y(), basePointRight.z() + dir.z());
        glEnd();

        // arrow pointing to the left
        /*    glBegin(GL_TRIANGLE_FAN);
                glVertex3f(basePointLeft.x() - right.x(), basePointLeft.y() - right.y(), basePointLeft.z() - right.z());
                glVertex3f(basePointLeft.x() + dir.x(), basePointLeft.y() + dir.y(), basePointLeft.z() + dir.z());
                glVertex3f(basePointLeft.x() - up.x(), basePointLeft.y() - up.y(), basePointLeft.z() - up.z());
                glVertex3f(basePointLeft.x() - dir.x(), basePointLeft.y() - dir.y(), basePointLeft.z() - dir.z());
                glVertex3f(basePointLeft.x() + up.x(), basePointLeft.y() + up.y(), basePointLeft.z() + up.z());
                glVertex3f(basePointLeft.x() + dir.x(), basePointLeft.y() + dir.y(), basePointLeft.z() + dir.z());
            glEnd();
        */

        if (j == 0)
        {
            glColor4f(1.0, 0.0, 0.0, 1.0);
        }
        glColor4ubv(arrowDirColor);

        glBegin(GL_TRIANGLE_FAN);
            // top of arrow
            glVertex3f(basePointLeft.x() - rightHalf.x() + up.x()*2.0, basePointLeft.y() - rightHalf.y() + up.y()*2.0, basePointLeft.z() - rightHalf.z() + up.z()*2.0);
            glVertex3f(basePointLeft.x() + rightHalf.x(), basePointLeft.y() + rightHalf.y(), basePointLeft.z() + rightHalf.z());
            glVertex3f(basePointLeft.x() - rightHalf.x() + dir.x(), basePointLeft.y() - rightHalf.y() + dir.y(), basePointLeft.z() - rightHalf.z() + dir.z());
            glVertex3f(basePointLeft.x() - right.x() - rightHalf.x(), basePointLeft.y() - right.y() - rightHalf.y(), basePointLeft.z() - right.z() - rightHalf.z());
            glVertex3f(basePointLeft.x() - rightHalf.x() - dir.x(), basePointLeft.y() - rightHalf.y() - dir.y(), basePointLeft.z() - rightHalf.z() - dir.z());
            glVertex3f(basePointLeft.x() + rightHalf.x(), basePointLeft.y() + rightHalf.y(), basePointLeft.z() + rightHalf.z());
        glEnd();

        // draw arrows base
        glBegin(GL_QUAD_STRIP);
            for (int i = 0; i < 4; ++i)
            {
                glVertex3f(upperCircle[i].x() + dirHalf.x(), upperCircle[i].y() + dirHalf.y(), upperCircle[i].z() + dirHalf.z());
                glVertex3f(upperCircle[i].x() - dirHalf.x(), upperCircle[i].y() - dirHalf.y(), upperCircle[i].z() - dirHalf.z());
            }

            for (int i = 3; i >= 0; --i)
            {
                glVertex3f(lowerCircle[i].x() + dirHalf.x(), lowerCircle[i].y() + dirHalf.y(), lowerCircle[i].z() + dirHalf.z());
                glVertex3f(lowerCircle[i].x() - dirHalf.x(), lowerCircle[i].y() - dirHalf.y(), lowerCircle[i].z() - dirHalf.z());
            }

            glVertex3f(upperCircle[0].x() + dirHalf.x(), upperCircle[0].y() + dirHalf.y(), upperCircle[0].z() + dirHalf.z());
            glVertex3f(upperCircle[0].x() - dirHalf.x(), upperCircle[0].y() - dirHalf.y(), upperCircle[0].z() - dirHalf.z());
        glEnd();

        glBegin(GL_QUAD_STRIP);
            for (int i = 0; i < 4; ++i)
            {
                glVertex3f(lowerCircle[i].x() + dirHalf.x(), lowerCircle[i].y() + dirHalf.y(), lowerCircle[i].z() + dirHalf.z());
                glVertex3f(upperCircle[i].x() + dirHalf.x(), upperCircle[i].y() + dirHalf.y(), upperCircle[i].z() + dirHalf.z());
            }

            for (int i = 3; i >= 0; --i)
            {
                glVertex3f(lowerCircle[i].x() - dirHalf.x(), lowerCircle[i].y() - dirHalf.y(), lowerCircle[i].z() - dirHalf.z());
                glVertex3f(upperCircle[i].x() - dirHalf.x(), upperCircle[i].y() - dirHalf.y(), upperCircle[i].z() - dirHalf.z());
            }
        glEnd();
    }
/*    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glColor4f(0.0, 0.0, 0.0, 1.0);
    // arrow pointing upwards
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(basePointUp.x() + up.x()*2.0, basePointUp.y() + up.y()*2.0, basePointUp.z() + up.z()*2.0);
        glVertex3f(basePointUp.x() + dir.x(), basePointUp.y() + dir.y(), basePointUp.z() + dir.z());
        glVertex3f(basePointUp.x() + right.x(), basePointUp.y() + right.y(), basePointUp.z() + right.z());
        glVertex3f(basePointUp.x() - dir.x(), basePointUp.y() - dir.y(), basePointUp.z() - dir.z());
        glVertex3f(basePointUp.x() - right.x(), basePointUp.y() - right.y(), basePointUp.z() - right.z());
        glVertex3f(basePointUp.x() + dir.x(), basePointUp.y() + dir.y(), basePointUp.z() + dir.z());
    glEnd();

    // arrow pointing to the right
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(basePointRight.x() + right.x()*2.0, basePointRight.y() + right.y()*2.0, basePointRight.z() + right.z()*2.0);
        glVertex3f(basePointRight.x() + dir.x(), basePointRight.y() + dir.y(), basePointRight.z() + dir.z());
        glVertex3f(basePointRight.x() - up.x(), basePointRight.y() - up.y(), basePointRight.z() - up.z());
        glVertex3f(basePointRight.x() - dir.x(), basePointRight.y() - dir.y(), basePointRight.z() - dir.z());
        glVertex3f(basePointRight.x() + up.x(), basePointRight.y() + up.y(), basePointRight.z() + up.z());
        glVertex3f(basePointRight.x() + dir.x(), basePointRight.y() + dir.y(), basePointRight.z() + dir.z());
    glEnd();*/
}

void MoCapManager::createSkeleton()
{
    Bone boneData;
    // hip - this will be the anchor of the skeleton
    boneData.setStartPos(Vector3(0.0, 1.0, 0.0));
    boneData.setLength(0.001);
    boneData.setAbsOrientation(Quaternion(Vector3(0.0, 0.0, 1.0), M_PI*90.0/180.0) * Quaternion(Vector3(-1.0, 0.0, 0.0), M_PI*90.0/180.0));
    int hip = _skeleton.createBone(boneData);
    assignNameToBone("hip", hip);

    // hip - neck (i.e. the body)
    boneData.setLength(0.55);
    // boneData.setAbsOrientation(Quaternion(Vector3(0.0, 0.0, 1.0), M_PI*90.0/180.0) * Quaternion(Vector3(-1.0, 0.0, 0.0), M_PI*90.0/180.0));
    boneData.setRelOrientation(Quaternion(1.0, 0.0, 0.0, 0.0));
    int neck = _skeleton.createBone(boneData, hip);
    assignNameToBone("body", neck);

    // neck - head
    boneData.setLength(0.30);
    // boneData.setAbsOrientation(Quaternion(Vector3(0.0, 0.0, 1.0), M_PI*90.0/180.0) * Quaternion(Vector3(-1.0, 0.0, 0.0), M_PI*90.0/180.0));
    boneData.setRelOrientation(Quaternion(1.0, 0.0, 0.0, 0.0));
    int head = _skeleton.createBone(boneData, neck);
    assignNameToBone("head", head);

    // neck - right shoulder
    boneData.setLength(0.20);
    // boneData.setAbsOrientation(Quaternion(Vector3(0.0, 1.0, 0.0), 0.0));
    boneData.setRelOrientation(Quaternion(Vector3(1.0, 0.0, 0.0), M_PI*90.0/180.0) * Quaternion(Vector3(0.0, 0.0, -1.0), M_PI*90.0/180.0));
    _rShoulderId = _skeleton.createBone(boneData, neck);
    assignNameToBone("rShoulder", _rShoulderId);

    // right upper arm
    boneData.setLength(0.35);
    // boneData.setAbsOrientation(Quaternion(Vector3(0.0, 1.0, 0.0), 0.0));
    boneData.setRelOrientation(Quaternion(1.0, 0.0, 0.0, 0.0));
    int rUpperArm = _skeleton.createBone(boneData, _rShoulderId);
    assignNameToBone("rUpperArm", rUpperArm);

    // right lower arm
    boneData.setLength(0.30);
    // boneData.setAbsOrientation(Quaternion(Vector3(0.0, 1.0, 0.0), 0.0));
    boneData.setRelOrientation(Quaternion(1.0, 0.0, 0.0, 0.0));
    int rLowerArm = _skeleton.createBone(boneData, rUpperArm);
    assignNameToBone("rLowerArm", rLowerArm);

    // neck - left shoulder
    boneData.setLength(0.20);
    // boneData.setAbsOrientation(Quaternion(Vector3(0.0, 1.0, 0.0), M_PI*180.0/180.0));
    boneData.setRelOrientation(Quaternion(Vector3(-1.0, 0.0, 0.0), M_PI*90.0/180.0) * Quaternion(Vector3(0.0, 0.0, -1.0), M_PI*90.0/180.0));
    _lShoulderId = _skeleton.createBone(boneData, neck);
    assignNameToBone("lShoulder", _lShoulderId);

    // left upper arm
    boneData.setLength(0.35);
    // boneData.setAbsOrientation(Quaternion(Vector3(0.0, 1.0, 0.0), M_PI*180.0/180.0));
    boneData.setRelOrientation(Quaternion(1.0, 0.0, 0.0, 0.0));
    int lUpperArm = _skeleton.createBone(boneData, _lShoulderId);
    assignNameToBone("lUpperArm", lUpperArm);

    // left lower arm
    boneData.setLength(0.30);
    // boneData.setAbsOrientation(Quaternion(Vector3(0.0, 1.0, 0.0), M_PI*180.0/180.0));
    boneData.setRelOrientation(Quaternion(1.0, 0.0, 0.0, 0.0));
    int lLowerArm = _skeleton.createBone(boneData, lUpperArm);
    assignNameToBone("lLowerArm", lLowerArm);

    // hip - right leg
    boneData.setLength(0.20);
    // boneData.setAbsOrientation(Quaternion(Vector3(0.0, 1.0, 0.0), 0.0));
    boneData.setRelOrientation(Quaternion(Vector3(1.0, 0.0, 0.0), M_PI*90.0/180.0) * Quaternion(Vector3(0.0, 0.0, -1.0), M_PI*90.0/180.0));
    _rHipId = _skeleton.createBone(boneData, hip);
    assignNameToBone("rHip", _rHipId);

    // right upper leg
    boneData.setLength(0.45);
    // boneData.setAbsOrientation(Quaternion(Vector3(0.0, 0.0, -1.0), M_PI*90.0/180.0));
    boneData.setRelOrientation(Quaternion(Vector3(0.0, 0.0, -1.0), M_PI*90.0/180.0) * Quaternion(Vector3(-1.0, 0.0, 0.0), M_PI*90.0/180.0));
    int rUpperLeg = _skeleton.createBone(boneData, _rHipId);
    assignNameToBone("rUpperLeg", rUpperLeg);

    // right lower leg
    boneData.setLength(0.45);
    // boneData.setAbsOrientation(Quaternion(Vector3(0.0, 0.0, -1.0), M_PI*90.0/180.0));
    boneData.setRelOrientation(Quaternion(1.0, 0.0, 0.0, 0.0));
    int rLowerLeg = _skeleton.createBone(boneData, rUpperLeg);
    assignNameToBone("rLowerLeg", rLowerLeg);

    // hip - left leg
    boneData.setLength(0.20);
    // boneData.setAbsOrientation(Quaternion(Vector3(0.0, 1.0, 0.0), M_PI*180.0/180.0));
    boneData.setRelOrientation(Quaternion(Vector3(-1.0, 0.0, 0.0), M_PI*90.0/180.0) * Quaternion(Vector3(0.0, 0.0, -1.0), M_PI*90.0/180.0));
    _lHipId = _skeleton.createBone(boneData, hip);
    assignNameToBone("lHip", _lHipId);

    // left upper leg
    boneData.setLength(0.45);
    // boneData.setAbsOrientation(Quaternion(Vector3(0.0, 0.0, -1.0), M_PI*90.0/180.0));
    boneData.setRelOrientation(Quaternion(Vector3(0.0, 0.0, -1.0), M_PI*90.0/180.0) * Quaternion(Vector3(1.0, 0.0, 0.0), M_PI*90.0/180.0));
    int lUpperLeg = _skeleton.createBone(boneData, _lHipId);
    assignNameToBone("lUpperLeg", lUpperLeg);

    // left lower leg
    boneData.setLength(0.45);
    // boneData.setAbsOrientation(Quaternion(Vector3(0.0, 0.0, -1.0), M_PI*90.0/180.0));
    boneData.setRelOrientation(Quaternion(1.0, 0.0, 0.0, 0.0));
    int lLowerLeg = _skeleton.createBone(boneData, lUpperLeg);
    assignNameToBone("lLowerLeg", lLowerLeg);

    _skeleton.update();
}


