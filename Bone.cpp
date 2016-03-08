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
    _useAbsOrientation = false;
}

Bone::Bone(int id) : _id(id)
{
    _parent = nullptr;
    _useAbsOrientation = false;
}

Bone::Bone(int id, const Bone &other) : _id(id)
{
    _name = other._name;
    _length = other._length;
    _chainedOrientation = other._chainedOrientation;
    _relOrientation = other._relOrientation;
    _defaultOrientation = other._defaultOrientation;
    _startPos = other._startPos;
    _endPos = other._endPos;
    _useAbsOrientation = other._useAbsOrientation;
    _parent = nullptr;
}

Bone::~Bone()
{
    //dtor
}

bool Bone::reparent(Bone* parent, bool keepChildren)
{
    if (!keepChildren)
    {
        if (_parent == nullptr)
        {
            if (_children.size() > 1)
            {
                // not clear how to reorganize child bones if there is more than one child. Do nothing
                return false;
            }
        }
        for (size_t i = 0; i < _children.size(); ++i)
        {
            _children[i]->setParent(_parent);
        }
    }
    if (hasChild(parent, true))
    {
        parent->setParent(_parent);
    }
    setParent(parent);
    return true;
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

std::vector<Bone*> Bone::getAllChildrenDFS()
{
    std::vector<Bone*> bones;
    for (size_t i = 0; i < _children.size(); ++i)
    {
        bones.push_back(_children[i]);
        std::vector<Bone*> childBones = _children[i]->getAllChildrenDFS();
        bones.insert(bones.end(), childBones.begin(), childBones.end());
    }
    return bones;
}

bool Bone::hasChild(Bone* child, bool checkGrandChildren) const
{
    if (child == nullptr)
    {
        return false;
    }
    return hasChild(child->getId(), checkGrandChildren);
}

bool Bone::hasChild(int id, bool checkGrandChildren) const
{
    for (size_t i = 0; i < _children.size(); ++i)
    {
        if (id == _children[i]->_id)
        {
            return true;
        }
    }

    if (checkGrandChildren)
    {
        for (size_t i = 0; i < _children.size(); ++i)
        {
            if (_children[i]->hasChild(id, true))
            {
                return true;
            }
        }
    }
    return false;
}

Bone& Bone::copyContent(const Bone &other)
{
    _name = other._name;
    _length = other._length;
    _chainedOrientation = other._chainedOrientation;
    _relOrientation = other._relOrientation;
    _defaultOrientation = other._defaultOrientation;
    _startPos = other._startPos;
    _endPos = other._endPos;
    _useAbsOrientation = other._useAbsOrientation;
    return *this;
}

void Bone::setAbsOrientation(const Quaternion &orientation)
{
    // setting the bone to an absolute orientation implies having a specific orientation in the chain,
    // so set _chainedOrientation accordingly and find the relative orientation in the update() routine (by setting _useAbsOrientation)
    _chainedOrientation = orientation.normalized() * _defaultOrientation.inv();
    _useAbsOrientation = true;
}

void Bone::setRelOrientation(const Quaternion &orientation)
{
    _relOrientation = orientation.normalized();
    _useAbsOrientation = false;
}

void Bone::setRelDefaultOrientation(const Quaternion &orientation)
{
    if (getParent() != nullptr)
    {
        _defaultOrientation = getParent()->getDefaultOrientation() * orientation.normalized();
    }
    else
    {
        _defaultOrientation = orientation.normalized();
    }
}

void Bone::setDefaultOrientation(const Quaternion &orientation)
{
    _defaultOrientation = orientation.normalized();
}

void Bone::setDefaultOrientation(const Vector3 &direction)
{
    _defaultOrientation = Quaternion(Vector3(1.0, 0.0, 0.0), direction);
}

void Bone::rotate(const Quaternion &rotation)
{
    _relOrientation = _relOrientation * _defaultOrientation * rotation.normalized() * _defaultOrientation.inv();
    _relOrientation.normalize();
}

void Bone::rotate(double roll, double pitch, double yaw)
{
    rotate(Quaternion(roll, pitch, yaw));
}

void Bone::setCurrentOrientationAsDefault()
{
    // the new default orientation becomes the old default orientation rotated by the overall rotation chain,
    // i.e. the absolute orientation of the bone with respect to the global coordinate system
    _defaultOrientation = _chainedOrientation * _defaultOrientation;
    _defaultOrientation.normalize();
    // by setting the current orientation as default, the relative orientation with respect to the bones parent becomes zero (unit quaternion)
    _relOrientation = Quaternion();
    // the rotation chain also becomes zero in this bone. The bones' children therefore have a new relative orientation to this bone
    // equal to the overall rotation chain until this bone + their own relative orientation
    for (size_t i = 0; i < _children.size(); ++i)
    {
        _children[i]->setRelOrientation(_chainedOrientation * _children[i]->getRelOrientation());
    }
    _chainedOrientation = Quaternion();
}

void Bone::update()
{
    if (_parent == nullptr)
    {
        // the orientation is an absolute orientation in the global coordinate system
        if (_useAbsOrientation)
        {
            // _chained orientation waa set before by setting the bone to an absolute orientation
            _relOrientation = _chainedOrientation;
            _useAbsOrientation = false;
        }
        else
        {
            _chainedOrientation = _relOrientation;
        }
    }
    else
    {
        _startPos = _parent->getEndPos();
        if (_useAbsOrientation)
        {
            // _chainedOrientation waa set before by setting the bone to an absolute orientation, now find the new relative orientation
            // of the bone with respect to its parent
            _relOrientation = _parent->getChainedOrientation().inv() * _chainedOrientation;
            _useAbsOrientation = false;
        }
        else
        {
            _chainedOrientation = _parent->getChainedOrientation() * _relOrientation;
        }
    }
    _endPos = getDirection() * _length + _startPos;

    // update children
    for (size_t i = 0; i < _children.size(); ++i)
    {
        _children[i]->update();
    }
}

Vector3 Bone::getDirection()
{
    // get the final direction of the bone by finally rotating its default direction by the overall rotation chain
    return (_chainedOrientation * _defaultOrientation).rotate(Vector3(1.0, 0.0, 0.0));
}

Vector3 Bone::getUpDirection()
{
    return (_chainedOrientation * _defaultOrientation).rotate(Vector3(0.0, 1.0, 0.0));
}

Vector3 Bone::getRightDirection()
{
    return (_chainedOrientation * _defaultOrientation).rotate(Vector3(0.0, 0.0, 1.0));
}

