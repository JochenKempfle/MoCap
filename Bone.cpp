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


#include "Bone.h"

Bone::Bone() : _id(-1)
{
    _parent = nullptr;
    _useAbsOrientationForUpdate = false;
}

Bone::Bone(int id) : _id(id)
{
    _parent = nullptr;
    _useAbsOrientationForUpdate = false;
}

Bone::Bone(int id, const Bone &other) : _id(id)
{
    _length = other._length;
    _absOrientation = other._absOrientation;
    _relOrientation = other._relOrientation;
    _relDefaultOrientation = other._relDefaultOrientation;
    _startPos = other._startPos;
    _endPos = other._endPos;
    _useAbsOrientationForUpdate = other._useAbsOrientationForUpdate;
    _parent = nullptr;
}

Bone::~Bone()
{
    //dtor
}

void Bone::setParent(Bone* parent)
{
    if (_parent != parent)
    {
        if (_parent != nullptr)
        {
            _parent->removeChild(this);
        }
        _parent = parent;
        if (parent != nullptr)
        {
            parent->appendChild(this);
        }
    }
}

void Bone::appendChild(Bone* child)
{
    if (child != nullptr && !hasChild(child->_id))
    {
        _children.push_back(child);
        child->setParent(this);
    }
}

void Bone::removeChild(int id)
{
    for (auto it = _children.begin(); it != _children.end(); ++it)
    {
        if ((*it)->_id == id)
        {
            _children.erase(it);
            break;
        }
    }
}

void Bone::removeChild(Bone* bone)
{
    removeChild(bone->_id);
}

std::vector<Bone*> Bone::getAllChildren()
{
    std::vector<Bone*> bones(_children);
    for (size_t i = 0; i < bones.size(); ++i)
    {
        for (size_t j = 0; j < bones[i]->_children.size(); ++j)
        {
            bones.push_back(bones[i]->_children[j]);
        }
    }
    return bones;
}

bool Bone::hasChild(Bone* child) const
{
    for (size_t i = 0; i < _children.size(); ++i)
    {
        if (child->_id == _children[i]->_id)
        {
            return true;
        }
    }
    return false;
}

bool Bone::hasChild(int id) const
{
    for (size_t i = 0; i < _children.size(); ++i)
    {
        if (id == _children[i]->_id)
        {
            return true;
        }
    }
    return false;
}

Bone& Bone::copyContent(const Bone &other)
{
    _length = other._length;
    _absOrientation = other._absOrientation;
    _relOrientation = other._relOrientation;
    _relDefaultOrientation = other._relDefaultOrientation;
    _startPos = other._startPos;
    _endPos = other._endPos;
    _useAbsOrientationForUpdate = other._useAbsOrientationForUpdate;
    return *this;
}

void Bone::setAbsOrientation(const Quaternion &rotation)
{
    // TODO(JK#5#): set abs orientation can get corrupted if child gets updated before parent
    _absOrientation = rotation.normalized();
    _useAbsOrientationForUpdate = true;
}

void Bone::setRelOrientation(const Quaternion &rotation)
{
    _relOrientation = rotation.normalized();
    _useAbsOrientationForUpdate = false;
}

void Bone::setRelDefaultOrientation(const Quaternion &rotation)
{
    _relDefaultOrientation = rotation.normalized();
}

void Bone::rotate(const Quaternion &rotation)
{
    _relOrientation = _relOrientation * rotation;
}

void Bone::rotate(double roll, double pitch, double yaw)
{
    rotate(Quaternion(roll, pitch, yaw));
}

void Bone::setCurrentOrientationAsDefault()
{
    _relDefaultOrientation = _relOrientation;
}

void Bone::update()
{
    if (_parent != nullptr)
    {
        _startPos = _parent->getEndPos();
        if (_useAbsOrientationForUpdate)
        {
            _relOrientation = _parent->getAbsOrientation().inv() * _absOrientation;
            _useAbsOrientationForUpdate = false;
        }
        else
        {
            _absOrientation = _parent->getAbsOrientation() * _relOrientation;
        }
    }
    else
    {
        if (_useAbsOrientationForUpdate)
        {
            _relOrientation = _absOrientation;
            _useAbsOrientationForUpdate = false;
        }
        else
        {
            _absOrientation = _relOrientation;
        }
    }
    Vector3 vec(1.0, 0.0, 0.0);
    vec = _absOrientation.rotate(vec);
    _endPos = vec.normalized() * _length + _startPos;

    // update children
    for (size_t i = 0; i < _children.size(); ++i)
    {
        _children[i]->update();
    }
}

Vector3 Bone::getDirection()
{
    Vector3 vec(1.0, 0.0, 0.0);
    vec = _absOrientation.rotate(vec);
    return vec.normalized();
}

Vector3 Bone::getUpDirection()
{
    Vector3 vec(0.0, 1.0, 0.0);
    vec = _absOrientation.rotate(vec);
    return vec.normalized();
}

Vector3 Bone::getRightDirection()
{
    Vector3 vec(0.0, 0.0, 1.0);
    vec = _absOrientation.rotate(vec);
    return vec.normalized();
}

