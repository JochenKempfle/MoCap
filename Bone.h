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
#include "JointConstraint.h"
#include <vector>
#include <string>

enum BONE_UPDATE_TYPE
{
    UPDATE_ABS_ORIENTATION,
    UPDATE_REL_ORIENTATION,
    UPDATE_POSITION,
};

class Bone
{
  public:
    Bone();
    ~Bone();

    inline int getId() const { return _id; }

    inline void setName(std::string name) { _name = name; }
    inline std::string getName() const { return _name; }

    /*
    // TODO(JK#8#2017-06-28): add color to bones (just add required render routines, color is supported)
    inline void setColor(const Vector3 &color) { _color = color; }
    inline void setColor(float r, float g, float b) { _color = Vector3(r, g, b); }
    inline void setColor(char r, char g, char b) { setColor(float(r)/255.0f, float(g)/255.0f, float(b)/255.0f); }
    inline Vector3 getColor() const { return _color; }
    */

    inline void setLength(float length) { _length = length; }
    inline float getLength() const { return _length; }
    // TODO(JK#2#): make bone orientation dependent on relative default orientation, add new set***Orientation method
    void setAbsOrientation(const Quaternion &orientation);
    void setRelOrientation(const Quaternion &orientation);

    void setRelDefaultOrientation(const Quaternion &orientation);
    void setDefaultOrientation(const Quaternion &orientation);
    void setDefaultOrientation(const Vector3 &direction);

    void setCurrentOrientationAsDefault();

    void rotate(const Quaternion &rotation);
    // rotate current orientation by euler angles
    void rotate(double roll, double pitch, double yaw);


    inline void setStartPos(const Vector3 &pos) { _startPos = pos; }
    inline void setStartPos(float x, float y, float z) { _startPos = Vector3(x, y, z); }

    inline void setEndPos(const Vector3 &pos) { _endPos = pos; _boneUpdateType = UPDATE_POSITION; }
    inline void setEndPos(float x, float y, float z) { setEndPos(Vector3(x, y, z)); }

    inline Vector3 getStartPos() const { return _startPos; }
    inline Vector3 getEndPos() const { return _endPos; }

    inline void setJointConstraint(const JointConstraint &constraint) { _constraint = constraint; }
    inline JointConstraint getJointConstraint() const { return _constraint; }

    bool reparent(Bone* parent, bool keepChildren = true);

    void setToDefault() { setRelOrientation(Quaternion()); }

    inline Quaternion getChainedOrientation() const { return _chainedOrientation; }
    inline Quaternion getAbsOrientation() const { return _chainedOrientation * _defaultOrientation; }
    inline Quaternion getRelOrientation() const { return _relOrientation; }
    inline Quaternion getDefaultOrientation() const { return _defaultOrientation; }

    inline Bone* getParent() { return _parent; }
    inline int getParentId() const { return _parent == nullptr ? -1 : _parent->getId(); }
    inline size_t getNumChildren() const { return _children.size(); }
    inline std::vector<Bone*> getChildren() { return _children; }
    // Returns all direct and indirect children breadth first sorted
    std::vector<Bone*> getAllChildren();
    std::vector<Bone*> getAllChildrenDFS();

    bool hasChild(Bone* child, bool checkGrandChildren = false) const;
    bool hasChild(int id, bool checkGrandChildren = false) const;

    Bone& copyContent(const Bone &other);
    void update();

    Vector3 getDirection();
    Vector3 getUpDirection();
    Vector3 getRightDirection();

    // read bone data from a stream. Note that you will loose all parent-child relations when reading from a stream!
    std::istream& read(std::istream &s);
    // write bone data to a stream. Note that no parent-child relations are written to the stream!
    std::ostream& write(std::ostream &s) const;
    // read binary bone data from a stream. Note that you will loose all parent-child relations when reading from a stream!
    std::istream& readBinary(std::istream &s);
    // write binary bone data to a stream. Note that no parent-child relations are written to the stream!
    std::ostream& writeBinary(std::ostream &s) const;

    friend class Skeleton;

  protected:
    Bone(int id);
    Bone(int id, const Bone &other);

    void setParent(Bone* parent);
    void appendChild(Bone* child);
    // Removes the child from the list of children, but does not delete the child. To delete a child, use the Skeleton class.
    void removeChild(int id);
    void removeChild(Bone* bone);

  private:
    int _id;
    std::string _name;
    // Vector3 _color;
    float _length;
    // the orientation of this bone after chaining all parents' orientations
    Quaternion _chainedOrientation;
    // the current orientation relative to the bone's parent
    Quaternion _relOrientation;
    // the absolute default orientation of this bone
    Quaternion _defaultOrientation;

    Vector3 _startPos;
    Vector3 _endPos;

    unsigned char _boneUpdateType;

    JointConstraint _constraint;

    Bone* _parent;
    std::vector<Bone*> _children;
};

// write bone data to a stream. Note that no parent-child relations are written to the stream!
std::ostream& operator<<(std::ostream& out, const Bone& bone);

// read bone data from a stream. Note that you will loose all parent-child relations when reading from a stream!
std::istream& operator>>(std::istream& in, Bone& bone);

#endif // BONE_H
