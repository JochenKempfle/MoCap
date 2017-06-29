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


#include "JointConstraint.h"

JointConstraint::JointConstraint() : _axis(1.0f, 0.0f, 0.0f)
{

}

JointConstraint::~JointConstraint()
{
    //dtor
}

void JointConstraint::setJointAxis(const Vector3& axis)
{
    // _axis = axis.normalized();
    // _resolution = 10;
}

void JointConstraint::addConstraint(const Quaternion& quat)
{
    Quaternion twist, swing;
    quat.decomposeSwingTwist(_axis, &swing, &twist);
    // Vector3 rotAxis = swing.getRotationAxis();

    // map the angle of the rotation axis to [-2, 2], i.e. use the following formula to map sin and cos values:
    // std::sin(angle*M_PI/180.0) >= 0 ? -1 : 1) * (1 + std::cos(angle*M_PI/180.0));
    // thus directly y and z values of the vector can be used (the vector is unit and on the y-z-plane)
    // float mapValue = (rotAxis.z() >= 0 ? -1.0f : 1.0f) * (1.0f + rotAxis.y());
    _axisAngleToAngle.insert(ConstraintKey(swing));
}

void JointConstraint::addConstraint(const Vector3& vec)
{
    Quaternion swing(Vector3(1.0f, 1.0f, 1.0f), vec);

    Vector3 rotAxis = swing.getRotationAxis();
    // map the angle of the rotation axis to [-2, 2], i.e. use the following formula to map sin and cos values:
    // std::sin(angle*M_PI/180.0) >= 0 ? -1 : 1) * (1 + std::cos(angle*M_PI/180.0));
    // thus directly y and z values of the vector can be used (the vector is unit and on the y-z-plane)
    // float mapValue = (rotAxis.z() >= 0 ? -1.0f : 1.0f) * (1.0f + rotAxis.y());
    _axisAngleToAngle.insert(ConstraintKey(swing));
    // _axisAngleToAngle[mapValue * 10000] = swing.u();
}

void JointConstraint::addConstraint(float axisAngle, float constraintAngle)
{
    //axisAngle *= M_PI/180.0;
    // swing angle will be cos(phi/2) (see quaternion real part)
    // float swingAngleHalf = constraintAngle * M_PI/(180.0 * 2.0);

    // float mapValue = (std::sin(axisAngle) >= 0 ? -1 : 1) * (1 + std::cos(axisAngle));

    _axisAngleToAngle.insert(ConstraintKey(axisAngle, constraintAngle));
    //_axisAngleToAngle[mapValue * 10000] = std::cos(swingAngleHalf);
}

void JointConstraint::removeConstraint(float axisAngle, float delta)
{
    // axisAngle *= M_PI/180.0;

    ConstraintKey key(axisAngle - delta, 0.0f);
    // float mapValue = (std::sin(axisAngle) >= 0 ? -1 : 1) * (1 + std::cos(axisAngle));

    auto it = _axisAngleToAngle.lower_bound(key);
    if (it == _axisAngleToAngle.end())
    {
        return;
    }
    if (it->getAxisAngle() <= key.getAxisAngle() + 2.0f * delta)
    {
        _axisAngleToAngle.erase(it);
    }
}

void JointConstraint::clear()
{
    _axisAngleToAngle.clear();
}

void JointConstraint::setConstraints(const std::vector<Quaternion> &quats)
{
    _axisAngleToAngle.clear();
    for (size_t i = 0; i < quats.size(); ++i)
    {
        addConstraint(quats[i]);
    }
}

void JointConstraint::setConstraints(const std::vector<Vector3> &vecs)
{
    _axisAngleToAngle.clear();
    for (size_t i = 0; i < vecs.size(); ++i)
    {
        addConstraint(vecs[i]);
    }
}

std::vector<std::pair<float, float> > JointConstraint::getAxisAngleToAngleKeyPoints() const
{
    std::vector<std::pair<float, float> > angles;
    /*
    for (auto it = _axisAngleToAngle.begin(); it != _axisAngleToAngle.end(); ++it)
    {
        float axisAngle = float(it->first) / 10000.0f;
        if (axisAngle > 2.0f)
        {
            axisAngle = 2.0f;
        }
        else if (axisAngle < -2.0f)
        {
            axisAngle = -2.0f;
        }
        axisAngle = axisAngle >= 0.0f ? 2.0 * M_PI - std::acos(axisAngle - 1.0f) : std::acos(-axisAngle -1.0f);
        float degreeFactor = 1.0f;

        degreeFactor = 180.0/M_PI;

        angles.push_back(std::make_pair(axisAngle * degreeFactor, std::acos(it->second) * 2.0f * degreeFactor));
    }
    */
    for (std::set<ConstraintKey>::iterator it = _axisAngleToAngle.begin(); it != _axisAngleToAngle.end(); ++it)
    {
        ConstraintKey key = *it;
        angles.push_back(std::make_pair(key.getAxisAngle(), key.getAngle()));
    }

    return angles;
}

std::vector<std::pair<float, float> > JointConstraint::getAxisAngleToAngleKeyPointsRad() const
{
    std::vector<std::pair<float, float> > angles;

    for (std::set<ConstraintKey>::iterator it = _axisAngleToAngle.begin(); it != _axisAngleToAngle.end(); ++it)
    {
        ConstraintKey key = *it;
        angles.push_back(std::make_pair(key.getAxisAngle()*M_PI/180.0, key.getAngle()*M_PI/180.0));
    }

    return angles;
}

std::vector<Quaternion> JointConstraint::getQuaternions() const
{
    std::vector<Quaternion> quats;
    for (auto it = _axisAngleToAngle.begin(); it != _axisAngleToAngle.end(); ++it)
    {
        quats.push_back(it->getQuaternion());
    }
    return quats;
    //float mapValue = (rotAxis.z() >= 0 ? -1.0f : 1.0f) * (1.0f + rotAxis.y());
    //_axisAngleToAngle[mapValue * 10000] = swing.u();
}

std::vector<Vector3> JointConstraint::getKeyPoints(float radius) const
{
    std::vector<Vector3> vecs;
    for (auto it = _axisAngleToAngle.begin(); it != _axisAngleToAngle.end(); ++it)
    {
        vecs.push_back(it->getQuaternion().rotate(Vector3(radius, 0.0f, 0.0f)));
    }
    return vecs;
    //float mapValue = (rotAxis.z() >= 0 ? -1.0f : 1.0f) * (1.0f + rotAxis.y());
    //_axisAngleToAngle[mapValue * 10000] = swing.u();
}

std::vector<Vector3> JointConstraint::getKeyPointsInterpolated(float radius, float stepSizeDegree) const
{
    std::vector<Vector3> vecs;
    for (float i = 0.0f; i < 360.0f; i += stepSizeDegree)
    {
        vecs.push_back(getInterpolatedKeyAt(i).getQuaternion().rotate(Vector3(radius, 0.0f, 0.0f)));
    }
    return vecs;
}


float JointConstraint::getMaxAngleAt(float axisAngle) const
{
    if (_axisAngleToAngle.size() == 0)
    {
        return 180.0f;
    }

    if (_axisAngleToAngle.size() == 1)
    {
        return _axisAngleToAngle.begin()->getAngle();
    }

    return getInterpolatedKeyAt(axisAngle).getAngle();
}

bool JointConstraint::isPossible(const Quaternion& quat) const
{
    return true;
}

bool JointConstraint::adjust(Quaternion* quat) const
{
    return true;
}

JointConstraint::ConstraintKey::ConstraintKey() : _mapValue(-2.0f), _cosConstraintAngleHalf(0.0f), _sinConstraintAngleHalf(1.0f), _cosAxisAngle(1.0f), _sinAxisAngle(0.0f),
                                                  _constraintAngle(180.0f), _axisAngle(0.0f)
{

}

JointConstraint::ConstraintKey::ConstraintKey(const Quaternion &quat) : _cosConstraintAngleHalf(quat.u())
{
    _sinConstraintAngleHalf = std::sqrt(1 - _cosConstraintAngleHalf*_cosConstraintAngleHalf);

    Quaternion swing, twist;
    quat.decomposeSwingTwist(Vector3(1.0f, 0.0f, 0.0f), &swing, &twist);

    _cosAxisAngle = swing.y()/_sinConstraintAngleHalf;
    _sinAxisAngle = swing.z()/_sinConstraintAngleHalf;

    _mapValue = calcMapValue(_sinAxisAngle, _cosAxisAngle);

    _constraintAngle = quat.getRotationAngle();
    _axisAngle = 180/M_PI * (_sinAxisAngle > 0 ? std::acos(_cosAxisAngle) : 2.0*M_PI - std::acos(_cosAxisAngle));
}

JointConstraint::ConstraintKey::ConstraintKey(float axisAngle, float constraintAngle) : _constraintAngle(constraintAngle), _axisAngle(axisAngle)
{
    axisAngle *= M_PI/180.0;
    // set constraintAngle to 1/2 (in rad) -> see construct quaternion from axis and angle
    constraintAngle *= M_PI/(180.0 * 2.0);

    _sinAxisAngle = std::sin(axisAngle);
    _cosAxisAngle = std::cos(axisAngle);
    _sinConstraintAngleHalf = std::sin(constraintAngle);

    _cosConstraintAngleHalf = std::cos(constraintAngle);
    // _u = std::cos(constraintAngle);
    // _yAxis = cosAxisAngle*sinConstraintAngle;
    // _zAxis = sinAxisAngle*sinConstraintAngle;
    _mapValue = calcMapValue(_sinAxisAngle, _cosAxisAngle);
}

JointConstraint::ConstraintKey::~ConstraintKey()
{

}


void JointConstraint::ConstraintKey::setKey(const Quaternion &quat)
{
    *this = ConstraintKey(quat);
}

Quaternion JointConstraint::ConstraintKey::getQuaternion() const
{
    // TODO(JK#1#2017-03-23): axis is normalized, but must be axis * sin x
    return Quaternion(_cosConstraintAngleHalf, 0.0f, _cosAxisAngle*_sinConstraintAngleHalf, _sinAxisAngle*_sinConstraintAngleHalf);
}

void JointConstraint::ConstraintKey::interpolateAngle(const ConstraintKey &key1, const ConstraintKey &key2)
{
    // TODO(JK#3#2017-04-07): joint constraint interpolation not perfect... maybe not possible with sin (with pure angles, this works)
    // calc axis1 cross axis2 (to get sine between both)
    /*
    float sinTheta = (key1.getCosAxisAngle() * key2.getSinAxisAngle() - key1.getSinAxisAngle() * key2.getCosAxisAngle());// + key1.getCosConstraintAngleHalf() * key2.getSinConstraintAngleHalf();// + key1.getSinConstraintAngleHalf() * key2.getCosConstraintAngleHalf();
    float sinStartToKey = (key1.getCosAxisAngle() * getSinAxisAngle() - key1.getSinAxisAngle() * getCosAxisAngle());//sinTheta;
    float sinKeyToEnd = (getCosAxisAngle() * key2.getSinAxisAngle() - getSinAxisAngle() * key2.getCosAxisAngle());//sinTheta;
    float w = (sinStartToKey + sinKeyToEnd);

    _cosConstraintAngleHalf = (sinStartToKey * key2.getCosConstraintAngleHalf() + sinKeyToEnd * key1.getCosConstraintAngleHalf()) / w;
    //_sinConstraintAngleHalf = (sinStartToKey * key1.getSinConstraintAngleHalf() + sinKeyToEnd * key2.getSinConstraintAngleHalf()) / sinTheta;
    _sinConstraintAngleHalf = std::sqrt(1.0f - _cosConstraintAngleHalf * _cosConstraintAngleHalf);
    */

    // code below is working as intended, but not very efficient
    float a = key2.getAxisAngle() - key1.getAxisAngle();
    if (a < 0.0f) a += 360.0f;

    float b = (getAxisAngle() - key1.getAxisAngle());
    if (b < 0.0f) b += 360.0f;

    b /= a;

    //_cosConstraintAngleHalf = (b * key2.getCosConstraintAngleHalf() + c * key1.getCosConstraintAngleHalf())/a;
    //_sinConstraintAngleHalf = std::sqrt(1.0f - _cosConstraintAngleHalf * _cosConstraintAngleHalf);

    setAngle(b*key2.getAngle() + (1.0f-b)*key1.getAngle());

    /*
    // slerp:
    double d = dot(other);
    double absD = std::abs(d);

    double scale0;
    double scale1;

    if (absD == 1.0)
    {
        scale0 = 1.0 - t;
        scale1 = t;
    }
    else
    {
        double theta = std::acos(absD);
        double sinThetaInv = 1.0/std::sin(theta);

        scale0 = std::sin((1.0 - t) * theta) * sinThetaInv;
        scale1 = std::sin((t * theta)) * sinThetaInv;
    }
    if (d < 0)
    {
        scale1 = -scale1;
    }

    return Quaternion(*this * scale0 + other * scale1);

    */
}

void JointConstraint::ConstraintKey::setAxisAngle(float axisAngle)
{
    _axisAngle = axisAngle;

    axisAngle *= M_PI/180.0;

    _sinAxisAngle = std::sin(axisAngle);
    _cosAxisAngle = std::cos(axisAngle);
    // _yAxis = cosAxisAngle*sinConstraintAngle;
    // _zAxis = sinAxisAngle*sinConstraintAngle;
    _mapValue = calcMapValue(_sinAxisAngle, _cosAxisAngle);
}

void JointConstraint::ConstraintKey::setAngle(float constraintAngle)
{
    _constraintAngle = constraintAngle;
    // set constraintAngle to 1/2 (in rad) -> see construct quaternion from axis and angle
    constraintAngle *= M_PI/(180.0 * 2.0);

    _sinConstraintAngleHalf = std::sin(constraintAngle);
    _cosConstraintAngleHalf = std::cos(constraintAngle);
}

float JointConstraint::ConstraintKey::getAxisAngle() const
{
    return _axisAngle;
    // return 180.0/M_PI * (_mapValue >= 0.0f ? (2.0 * M_PI - std::acos(_cosAxisAngle)) : std::acos(_cosAxisAngle));
}

float JointConstraint::ConstraintKey::getAxisAngleRad() const
{
    return _axisAngle * M_PI/180.0;
    // return _mapValue >= 0.0f ? (2.0 * M_PI - std::acos(_cosAxisAngle)) : std::acos(_cosAxisAngle);
}

float JointConstraint::ConstraintKey::getAngle() const
{
    return _constraintAngle;
    // return 180.0/M_PI * 2.0f * std::acos(_cosConstraintAngleHalf);
}

float JointConstraint::ConstraintKey::getAngleRad() const
{
    return _constraintAngle * M_PI/180.0;
    // return 2.0f * std::acos(_cosConstraintAngleHalf);
}

void JointConstraint::ConstraintKey::copyAxis(const ConstraintKey &other)
{
    _sinAxisAngle = other._sinAxisAngle;
    _cosAxisAngle = other._cosAxisAngle;
    _mapValue = other._mapValue;
    _axisAngle = other._axisAngle;
}

void JointConstraint::ConstraintKey::copyAngle(const ConstraintKey &other)
{
    _sinConstraintAngleHalf = other._sinConstraintAngleHalf;
    _cosConstraintAngleHalf = other._cosConstraintAngleHalf;
    _constraintAngle = other._constraintAngle;
}

float JointConstraint::ConstraintKey::calcMapValue(const Quaternion &quat) const
{
    // map the angle of the rotation axis to [-2, 2], i.e. use the following formula to map sin and cos values:
    // sin(angle) >= 0 ? -1 : 1) * (1 + cos(angle));
    // thus directly y and z values of the quaternions rotation vector can be used (the vector is unit and on the y-z-plane)
    // using the y axis as sin(angle) is due the fact that the rotation axis is orthogonal to the original rotation of the quaternion
    Vector3 axis = quat.getRotationAxis();
    return (axis.z() >= 0 ? -1.0f : 1.0f) * (1.0f + axis.y());
}

float JointConstraint::ConstraintKey::calcMapValue(float sinAngle, float cosAngle) const
{
    // map the angle of the rotation axis to [-2, 2], i.e. use the following formula to map sin and cos values:
    return (sinAngle >= 0 ? -1.0f : 1.0f) * (1.0f + cosAngle);
}

float JointConstraint::ConstraintKey::calcMapValue(float axisAngle) const
{
    axisAngle *= M_PI/180.0;
    return calcMapValue(std::sin(axisAngle), std::cos(axisAngle));
}

float JointConstraint::getAxisAngleFromAngle(float angle) const
{
    return angle + 90;
}

float JointConstraint::getAxisAngleFromAngleRad(float angle) const
{
    return angle + M_PI/2.0;
}

JointConstraint::ConstraintKey JointConstraint::getInterpolatedKeyAt(float axisAngle) const
{
    ConstraintKey key(axisAngle, 180.0f);
    if (_axisAngleToAngle.size() == 0)
    {
        return key;
    }

    if (_axisAngleToAngle.size() == 1)
    {
        key.copyAngle(*_axisAngleToAngle.begin());
        return key;
    }

    auto it = _axisAngleToAngle.upper_bound(key);
    auto it2 = it;

    if (it == _axisAngleToAngle.begin())
    {
        it = _axisAngleToAngle.end();
        --it;
    }
    else if (it == _axisAngleToAngle.end())
    {
        --it;
        it2 = _axisAngleToAngle.begin();
    }
    else
    {
        --it;
    }

    key.interpolateAngle(*it, *it2);
    return key;
}

/*
Quaternion JointConstraint::standardizeQuaternion(const Quaternion &quat) const
{
    Vector3 rotatedAxis = quat.rotate(_axis);
    float k = _axis.dot(rotatedAxis);
    Vector3 u = _axis.cross(rotatedAxis).normalized();
    return Quaternion(std::sqrt((1 + k) / 2), std::sqrt((1 - k) / 2) * u);
}
*/
