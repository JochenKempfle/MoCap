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


#ifndef BONE_H
#define BONE_H

#include "Quaternion.h"
#include "Vector3.h"
#include <vector>

class Bone
{
  public:
    Bone();
    ~Bone();

    int getId() const { return _id; }

    void setLength(float length) { _length = length; }
    // TODO(JK#2#): don't allow setting of absolute positions? => more safe
    void setAbsOrientation(const Quaternion &rotation);
    void setRelOrientation(const Quaternion &rotation);
    void setRelDefaultOrientation(const Quaternion &rotation);
    void rotate(const Quaternion &rotation);
    // rotate current orientation by euler angles
    void rotate(double roll, double pitch, double yaw);
    void setCurrentOrientationAsDefault();
    void setStartPos(const Vector3 &pos) { _startPos = pos; }
    void setStartPos(float x, float y, float z) { _startPos = Vector3(x, y, z); }
    void setEndPos(const Vector3 &pos) { _endPos = pos; }
    void setParent(Bone* parent);
    // TODO(JK#9#): move functions alternating bones' parent/children to protected as this should only be done by Skeleton class.
    void appendChild(Bone* child);
    // Removes the child from the list of children, but does not delete the child. To delete a child, use the Skeleton class.
    void removeChild(int id);
    void removeChild(Bone* bone);

    float getLength() const { return _length; }
    Quaternion getAbsOrientation() const { return _absOrientation; }
    Quaternion getRelOrientation() const { return _relOrientation; }
    Quaternion getRelDefaultOrientation() const { return _relDefaultOrientation; }
    Vector3 getStartPos() const { return _startPos; }
    Vector3 getEndPos() const { return _endPos; }
    Bone* getParent() { return _parent; }
    size_t getNumChildren() const { return _children.size(); }
    std::vector<Bone*> getChildren() { return _children; }
    // Returns all direct and indirect children breadth first sorted
    std::vector<Bone*> getAllChildren();
    bool hasChild(Bone* child) const;
    bool hasChild(int id) const;

    Bone& copyContent(const Bone &other);
    void update();

    Vector3 getDirection();
    Vector3 getUpDirection();
    Vector3 getRightDirection();

    friend class Skeleton;

  protected:
    Bone(int id);
    Bone(int id, const Bone &other);

  private:
    int _id;
    float _length;
    Quaternion _absOrientation;
    Quaternion _relOrientation;
    Quaternion _relDefaultOrientation;
    Vector3 _startPos;
    Vector3 _endPos;
    bool _useAbsOrientationForUpdate;
    Bone* _parent;
    std::vector<Bone*> _children;
};

#endif // BONE_H
