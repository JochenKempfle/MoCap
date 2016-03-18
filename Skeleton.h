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


#ifndef SKELETON_H
#define SKELETON_H

#include "Bone.h"
#include "Quaternion.h"
#include <string>
#include <vector>
#include <map>

class Skeleton
{
  public:
    Skeleton();
    Skeleton(const Skeleton &other);
    Skeleton(Skeleton &&other);
    virtual ~Skeleton();

    void setName(std::string name) { _name = name; }
    std::string getName() const { return _name; }

    int createBone(int parent = -1);
    int createBone(const Bone& boneData, int parent = -1);

    void setBoneName(int id, std::string name);
    std::string getBoneName(int id) const;
    std::vector<std::pair<int, std::string> > getBoneIdsWithName() const;

    void setToDefault();
    void setCurrentAsDefault();

    // Erase the bone specified by id. Set eraseChildren to true, to also erase all its children.
    // Returns true on success, false otherwise.
    // Does not erase the bone, when more than one child could possibly become the new root.
    bool eraseBone(int id, bool eraseChildren = false);
    void clear();

    bool reparent(int boneId, int parentId, bool keepChildren = true);

    void scale(float scaleFactor);

    std::vector<int> getBoneIds() const;
    int getRootId() const;
    Bone* getRoot() { return _root; }

    Bone* getBone(int id);
    bool setBoneData(int id, const Bone &boneData);
    bool setAbsBoneOrientation(int id, const Quaternion &orientation);
    bool setRelBoneOrientation(int id, const Quaternion &orientation);
    int getNextFreeId() const { return _nextId; }
    void update();

    void selectBone(int id) { _selectedBoneId = id; }
    void unselectBone() { _selectedBoneId = -1; }
    Bone* getSelectedBone() { return getBone(_selectedBoneId); }
    int getSelectedBoneId() const { return _selectedBoneId; }

    Skeleton& operator=(Skeleton other);

    std::map<int, Bone*>::iterator beginBones() { return _bones.begin(); }
    std::map<int, Bone*>::iterator endBones() { return _bones.end(); }

  protected:

  private:
    int _nextId;
    int _selectedBoneId;
    std::string _name;
    Bone* _root;
    std::map<int, Bone*> _bones;
};

#endif // SKELETON_H
