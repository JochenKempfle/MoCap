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


#ifndef JOINTCONSTRAINT_H
#define JOINTCONSTRAINT_H

#include "Quaternion.h"
#include <set>
#include <vector>


class JointConstraint
{
  public:
    JointConstraint();
    virtual ~JointConstraint();

    void setJointAxis(const Vector3 &axis);

    void addConstraint(const Quaternion &quat);
    void addConstraint(const Vector3 &vec);
    void addConstraint(float axisAngle, float constraintAngle);

    void removeConstraint(float axisAngle, float delta = 0.00001f);

    void clear();

    void setConstraints(const std::vector<Quaternion> &quats);
    void setConstraints(const std::vector<Vector3> &vecs);

    std::vector<std::pair<float, float> > getAxisAngleToAngleKeyPoints() const;
    std::vector<std::pair<float, float> > getAxisAngleToAngleKeyPointsRad() const;

    std::vector<Quaternion> getQuaternions() const;

    std::vector<Vector3> getKeyPoints(float radius = 1.0f) const;
    std::vector<Vector3> getKeyPointsInterpolated(float radius = 1.0f, float stepSizeDegree = 1.0f) const;

    float getMaxAngleAt(float axisAngle) const;
    float getMaxAngleAtRad(float axisAngle) const;
    // Quaternion getDistance(const Quaternion &quat) const;

    bool isPossible(const Quaternion &quat) const;
    bool adjust(Quaternion* quat) const;

    // std::set<ConstraintKey>::iterator begin() { return _axisAngleToAngle.begin(); }
    // std::set<ConstraintKey>::iterator end() { return _axisAngleToAngle.end(); }

  protected:

  private:

    class ConstraintKey
    {
      public:
        ConstraintKey();
        ConstraintKey(const Quaternion &quat);
        ConstraintKey(float axisAngle, float constraintAngle);

        virtual ~ConstraintKey();

        void setKey(const Quaternion &quat);
        Quaternion getQuaternion() const;

        void interpolateAngle(const ConstraintKey &key1, const ConstraintKey &key2);

        void setAxisAngle(float angle);
        void setAngle(float angle);

        float getAxisAngle() const;
        float getAxisAngleRad() const;
        float getAngle() const;
        float getAngleRad() const;

        void copyAxis(const ConstraintKey &other);
        void copyAngle(const ConstraintKey &other);

        bool operator<(const ConstraintKey &other) const { return getMapValue() < other.getMapValue(); }
        bool operator<=(const ConstraintKey &other) const { return getMapValue() <= other.getMapValue(); }
        bool operator>(const ConstraintKey &other) const { return getMapValue() > other.getMapValue(); }
        bool operator>=(const ConstraintKey &other) const { return getMapValue() >= other.getMapValue(); }

        inline float getMapValue() const { return _mapValue; }
        inline float getCosConstraintAngleHalf() const { return _cosConstraintAngleHalf; }
        inline float getSinConstraintAngleHalf() const { return _sinConstraintAngleHalf; }
        inline float getCosAxisAngle() const { return _cosAxisAngle; }
        inline float getSinAxisAngle() const { return _sinAxisAngle; }

        float calcMapValue(const Quaternion &quat) const;
        float calcMapValue(float sinAngle, float cosAngle) const;
        float calcMapValue(float axisAngle) const;

      private:

        // float _u;
        // float _yAxis;
        // float _zAxis;
        float _mapValue;

        // _cosConstraintAngleHalf is the same as the quaternions real part (u)
        float _cosConstraintAngleHalf;
        float _sinConstraintAngleHalf;

        // cosAxisAngle refers to the length of the y axis (of the normalized rotation axis vector)
        float _cosAxisAngle;
        // sinAxisAngle refers to the length of the z axis (of the normalized rotation axis vector)
        float _sinAxisAngle;

        float _constraintAngle;
        float _axisAngle;
    };

    float getAxisAngleFromAngle(float angle) const;
    float getAxisAngleFromAngleRad(float angle) const;

    ConstraintKey getInterpolatedKeyAt(float axisAngle) const;

    // TODO(JK#5#2017-06-13): in JointConstraint, rotation axis is not needed (?)
    Vector3 _axis;
    // std::vector<Quaternion> _quats;
    // std::vector<float> _LUT;
    std::set<ConstraintKey> _axisAngleToAngle;
};

#endif // JOINTCONSTRAINT_H
