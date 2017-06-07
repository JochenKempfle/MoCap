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
    _name = other._name;

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
    _name = other._name;
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

void Skeleton::scale(float scaleFactor)
{
    for (auto it = _bones.begin(); it != _bones.end(); ++it)
    {
        it->second->setLength(it->second->getLength() * scaleFactor);
    }
    update();
}

void Skeleton::setPosition(float x, float y, float z)
{
    setPosition(Vector3(x, y, z));
}

void Skeleton::setPosition(Vector3 pos)
{
    if (_root == nullptr)
    {
        return;
    }
    // set the skeleton position such that the root end point is located at the new global skeleton position
    Vector3 rootDirection = _root->getEndPos() - _root->getStartPos();
    _root->setStartPos(pos - rootDirection);
    _root->update();
}

Vector3 Skeleton::getPosition() const
{
    if (_root == nullptr)
    {
        return Vector3(0.0f, 0.0f, 0.0f);
    }
    return _root->getStartPos();
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

Bone* Skeleton::getBone(std::string name)
{
    for (auto it = _bones.begin(); it != _bones.end(); ++it)
    {
        if (it->second->getName() == name)
        {
            return it->second;
        }
    }
    return nullptr;
}

int Skeleton::getBoneId(std::string name) const
{
    for (auto it = _bones.begin(); it != _bones.end(); ++it)
    {
        if (it->second->getName() == name)
        {
            return it->first;
        }
    }
    return -1;
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

bool Skeleton::setAbsBoneOrientation(int id, const Quaternion &orientation)
{
    auto it = _bones.find(id);
    if (it != _bones.end())
    {
        it->second->setAbsOrientation(orientation);
        return true;
    }
    return false;
}

bool Skeleton::setRelBoneOrientation(int id, const Quaternion &orientation)
{
    auto it = _bones.find(id);
    if (it != _bones.end())
    {
        it->second->setRelOrientation(orientation);
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

AABB Skeleton::getAABB() const
{
    AABB box(_root->getStartPos(), _root->getEndPos());
    for (auto it = _bones.begin(); it != _bones.end(); ++it)
    {
        box.enclose(it->second->getEndPos());
    }
    return box;
}

AABB Skeleton::getAABBFromDefaultPose() const
{
    Skeleton dummy = *this;
    dummy.setToDefault();
    return dummy.getAABB();
}

Skeleton& Skeleton::operator=(Skeleton other)
{
    _nextId = other._nextId;
    _selectedBoneId = other._selectedBoneId;
    _name = other._name;
    std::swap(_root, other._root);
    std::swap(_bones, other._bones);
    return *this;
}


std::istream& Skeleton::read(std::istream& s)
{
    clear();
    std::getline(s, _name);

    s >> _nextId;

    size_t numBones = 0;
    s >> numBones;

    s.ignore(100, '\n');

    Bone boneData;

    // catch the root
    if (numBones > 0)
    {
        s >> boneData;
        _root = new Bone(boneData.getId());
        _root->copyContent(boneData);
        _bones[boneData.getId()] = _root;
        --numBones;
    }

    // catch all other bones
    int parentId = _root->getId();
    for (size_t i = 0; i < numBones; ++i)
    {
        s >> parentId;
        s >> boneData;
        Bone* bone = new Bone(boneData.getId());
        bone->copyContent(boneData);
        getBone(parentId)->appendChild(bone);
        _bones[boneData.getId()] = bone;
    }

    // ensure the skeleton is set to default after reading in data
    setToDefault();

    return s;
}

std::ostream& Skeleton::write(std::ostream& s) const
{
    std::vector<Bone*> bones = _root->getAllChildrenDFS();

    s << _name << std::endl << _nextId << ' ' << bones.size() + 1 << std::endl;

    _root->write(s);
    for (size_t i = 0; i < bones.size(); ++i)
    {
        s << std::endl << bones[i]->getParentId() << ' ';
        bones[i]->write(s);
    }
    return s;
}

std::istream& Skeleton::readBinary(std::istream& s)
{
    clear();

    size_t strLength;
    s.read((char*)&strLength, sizeof(size_t));
    _name.resize(strLength);
    s.read((char*)&_name[0], strLength);

    s.read((char*)&_nextId, sizeof(_nextId));

    size_t numBones = 0;
    s.read((char*)&numBones, sizeof(size_t));

    Bone boneData;

    // catch the root
    if (numBones > 0)
    {
        boneData.readBinary(s);
        _root = new Bone(boneData.getId());
        _root->copyContent(boneData);
        _bones[boneData.getId()] = _root;
        --numBones;
    }

    // catch all other bones
    int parentId = _root->getId();
    for (size_t i = 0; i < numBones; ++i)
    {
        s.read((char*)&parentId, sizeof(parentId));
        boneData.readBinary(s);
        Bone* bone = new Bone(boneData.getId());
        bone->copyContent(boneData);
        getBone(parentId)->appendChild(bone);
        _bones[boneData.getId()] = bone;
    }

    // ensure the skeleton is set to default after reading in data
    setToDefault();

    return s;
}

std::ostream& Skeleton::writeBinary(std::ostream& s) const
{
    std::vector<Bone*> bones = _root->getAllChildrenDFS();

    size_t size = _name.size();
    s.write((char*)&size, sizeof(size_t));
    s.write((char*)&_name[0], _name.size());
    s.write((char*)&_nextId, sizeof(_nextId));
    size = bones.size() + 1;
    s.write((char*)&size, sizeof(size_t));

    _root->writeBinary(s);
    for (size_t i = 0; i < bones.size(); ++i)
    {
        int parentId = bones[i]->getParentId();
        s.write((char*)&parentId, sizeof(parentId));
        bones[i]->writeBinary(s);
    }

    return s;
}

std::ostream& operator<<(std::ostream& out, const Skeleton& skeleton)
{
    return skeleton.write(out);
}

std::istream& operator>>(std::istream& in, Skeleton& skeleton)
{
    return skeleton.read(in);
}

