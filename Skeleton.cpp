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


#include "Skeleton.h"

Skeleton::Skeleton()
{
    //ctor
    _nextId = 0;
    _selectedBoneId = -1;
    _root = nullptr;
}

Skeleton::Skeleton(const Skeleton &other)
{
    _nextId = other._nextId;
    _selectedBoneId = other._selectedBoneId;

    // copy the bone data
    for (auto it = other._bones.begin(); it != other._bones.end(); ++it)
    {
        _bones[it->first] = new Bone(it->first, *it->second);
    }

    // now that all bones exist, construct the skeleton by seting the root and child-parent relationships
    _root = _bones[other._root->getId()];
    Bone* parent = nullptr;
    for (auto it = other._bones.begin(); it != other._bones.end(); ++it)
    {
        parent = it->second->getParentId() < 0 ? nullptr : _bones[it->second->getParentId()];
        _bones[it->first]->setParent(parent);
    }
}

Skeleton::Skeleton(Skeleton &&other) : Skeleton()
{
    _nextId = other._nextId;
    _selectedBoneId = other._selectedBoneId;
    std::swap(_root, other._root);
    std::swap(_bones, other._bones);
}

Skeleton::~Skeleton()
{
    clear();
    //dtor
}


int Skeleton::createBone(int parent)
{
    int id = _nextId++;
    Bone* bone = new Bone(id);
    _bones[id] = bone;

    // check if parent exists
    auto it = _bones.find(parent);
    if (it == _bones.end())
    {
        // if parent does not exist, the bone becomes the root bone
        if (_root != nullptr)
        {
            _root->setParent(bone);
        }
        _root = bone;
    }
    else
    {
        // if parent exists, add bone to its children
        it->second->appendChild(bone);
    }
    return id;
}

int Skeleton::createBone(const Bone& boneData, int parent)
{
    int id = _nextId++;
    Bone* bone = new Bone(id, boneData);
    _bones[id] = bone;

    // check if parent exists
    auto it = _bones.find(parent);
    if (it == _bones.end())
    {
        // if parent does not exist, the bone becomes the root bone
        if (_root != nullptr)
        {
            _root->setParent(bone);
        }
        _root = bone;
    }
    else
    {
        // if parent exists, add bone to its children
        it->second->appendChild(bone);
    }
    return id;
}


void Skeleton::setBoneName(int id, std::string name)
{
    auto it = _bones.find(id);
    if (it == _bones.end())
    {
        return;
    }
    it->second->setName(name);
}

std::string Skeleton::getBoneName(int id) const
{
    auto it = _bones.find(id);
    if (it == _bones.end())
    {
        return std::string();
    }
    return it->second->getName();
}

std::vector<std::pair<int, std::string> > Skeleton::getBoneIdsWithName() const
{
    std::vector<std::pair<int, std::string> > bones;
    for (auto it = _bones.begin(); it != _bones.end(); ++it)
    {
        bones.push_back(make_pair(it->first, it->second->getName()));
    }
    return bones;
}

void Skeleton::setToDefault()
{
    for (auto it = _bones.begin(); it != _bones.end(); ++it)
    {
        it->second->setToDefault();
    }
    update();
}

void Skeleton::setCurrentAsDefault()
{
    for (auto it = _bones.begin(); it != _bones.end(); ++it)
    {
        it->second->setCurrentOrientationAsDefault();
    }
}

bool Skeleton::eraseBone(int id, bool eraseChildren)
{
    auto it = _bones.find(id);
    if (it == _bones.end())
    {
        // bone with specified id does not exist. Return true as the bone was already erased
        return true;
    }

    Bone* bone = it->second;
    // somehow an invalid bone pointer was inserted in _bones. Erase it
    if (bone == nullptr)
    {
        _bones.erase(it);
        return true;
    }
    std::vector<Bone*> children = bone->getChildren();

    if (eraseChildren)
    {
        // recursively erase all children
        for (size_t i = 0; i < children.size(); ++i)
        {
            eraseBone(children[i]->getId(), true);
        }
    }
    else
    {
        // catch the special case for the root being erased
        if (bone->getParent() == nullptr)
        {
            if (children.size() > 1)
            {
                // no parent available and bone has more than one child. Return false
                return false;
            }
            else if (children.size() == 1)
            {
                _root = children[0];
            }
            else
            {
                _root = nullptr;
            }
        }
        // reparent child bones
        for (size_t i = 0; i < children.size(); ++i)
        {
            children[i]->setParent(bone->getParent());
        }
    }
    // remove bone from parents children list
    if (bone->getParent() != nullptr)
    {
        bone->getParent()->removeChild(bone);
    }
    // finally erase this bone
    _bones.erase(it);
    delete bone;
    return true;
}

void Skeleton::clear()
{
    // delete all bones
    for (auto it = _bones.begin(); it != _bones.end(); ++it)
    {
        delete it->second;
    }
    _root = nullptr;
    _bones.clear();
    _nextId = 0;
}

bool Skeleton::reparent(int boneId, int newParentId, bool keepChildren)
{
    if (boneId == newParentId)
    {
        return false;
    }
    auto it = _bones.find(boneId);
    if (it == _bones.end())
    {
        return false;
    }
    Bone* bone = it->second;
    it = _bones.find(newParentId);
    Bone* parentBone;
    if (it == _bones.end())
    {
        parentBone = nullptr;
    }
    else
    {
        parentBone = it->second;
    }
    return bone->reparent(parentBone, keepChildren);
}

std::vector<int> Skeleton::getBoneIds() const
{
    std::vector<int> ids;
    for (auto it = _bones.begin(); it != _bones.end(); ++it)
    {
        ids.push_back(it->first);
    }
    return ids;
}

int Skeleton::getRootId() const
{
    if (_root == nullptr)
    {
        return -1;
    }
    return _root->getId();
}

Bone* Skeleton::getBone(int id)
{
    auto it = _bones.find(id);
    if (it != _bones.end())
    {
        return it->second;
    }
    return nullptr;
}

bool Skeleton::setBoneData(int id, const Bone &boneData)
{
    auto it = _bones.find(id);
    if (it != _bones.end())
    {
        it->second->copyContent(boneData);
        return true;
    }
    return false;
}

bool Skeleton::setAbsBoneRotation(int id, const Quaternion &rotation)
{
    auto it = _bones.find(id);
    if (it != _bones.end())
    {
        it->second->setAbsOrientation(rotation);
        return true;
    }
    return false;
}

bool Skeleton::setRelBoneRotation(int id, const Quaternion &rotation)
{
    auto it = _bones.find(id);
    if (it != _bones.end())
    {
        it->second->setRelOrientation(rotation);
        return true;
    }
    return false;
}

void Skeleton::update()
{
    // when updating the root bone, its children will automatically get updated recursively
    _root->update();
/*    for (size_t i = 0; i < _rootBones.size(); ++i)
    {
        _rootBones[i]->update();
    }
    */
}


Skeleton& Skeleton::operator=(Skeleton other)
{
    _nextId = other._nextId;
    _selectedBoneId = other._selectedBoneId;
    std::swap(_root, other._root);
    std::swap(_bones, other._bones);
    return *this;
}



