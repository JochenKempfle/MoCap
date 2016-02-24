/*
 * OctoMap - An Efficient Probabilistic 3D Mapping Framework Based on Octrees
 * http://octomap.github.com/
 *
 * Copyright (c) 2009-2013, K.M. Wurm and A. Hornung, University of Freiburg
 * All rights reserved.
 * License: New BSD
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the University of Freiburg nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "Vector3.h"
#include "Quaternion.h"
//#include <octomap/math/Utils.h>

#include <cassert>
#include <math.h>


// used from Vector: norm2, unit, *

Quaternion::Quaternion(const Quaternion &other)
{
    data[0] = other(0);
    data[1] = other(1);
    data[2] = other(2);
    data[3] = other(3);
}

Quaternion::Quaternion(float uu, float xx, float yy, float zz)
{
    u() = uu;
    x() = xx;
    y() = yy;
    z() = zz;
}

Quaternion::Quaternion(const Vector3 &other)
{
    operator=(Quaternion(other.roll(), other.pitch(), other.yaw()));
}

Quaternion::Quaternion(const Vector3 &from, const Vector3 &to)
{
    float norm = sqrt(from.norm_sq() * to.norm_sq());
    float realPart = norm + from.dot(to);
    Vector3 vec;

    if (realPart < 1.e-6f * norm)
    {
        // If "from" and "to" are exactly opposite, rotate 180 degrees around an arbitrary orthogonal axis.
        realPart = 0.0f;
        vec = abs(from.x()) > abs(from.z()) ? Vector3(-from.y(), from.x(), 0.f) : Vector3(0.f, -from.z(), from.y());
    }
    else
    {
        // Otherwise, build quaternion the standard way.
        vec = from.cross(to);
    }
    x() = vec.x();
    y() = vec.y();
    z() = vec.z();
    u() = realPart;
    normalize();
}

Quaternion::Quaternion(double roll, double pitch, double yaw)
{
    double sroll   = sin(roll);
    double spitch = sin(pitch);
    double syaw   = sin(yaw);
    double croll   = cos(roll);
    double cpitch = cos(pitch);
    double cyaw   = cos(yaw);

    double m[3][3] = { //create rotational Matrix
      {cyaw*cpitch, cyaw*spitch*sroll - syaw*croll, cyaw*spitch*croll + syaw*sroll},
      {syaw*cpitch, syaw*spitch*sroll + cyaw*croll, syaw*spitch*croll - cyaw*sroll},
      {    -spitch,                  cpitch*sroll,                  cpitch*croll}
    };

    float _u = (float) (sqrt(std::max(0., 1 + m[0][0] + m[1][1] + m[2][2]))/2.0);
    float _x = (float) (sqrt(std::max(0., 1 + m[0][0] - m[1][1] - m[2][2]))/2.0);
    float _y = (float) (sqrt(std::max(0., 1 - m[0][0] + m[1][1] - m[2][2]))/2.0);
    float _z = (float) (sqrt(std::max(0., 1 - m[0][0] - m[1][1] + m[2][2]))/2.0);
    u() = _u;
    x() = (m[2][1] - m[1][2])>=0?fabs(_x):-fabs(_x);
    y() = (m[0][2] - m[2][0])>=0?fabs(_y):-fabs(_y);
    z() = (m[1][0] - m[0][1])>=0?fabs(_z):-fabs(_z);
}

Quaternion::Quaternion(const Vector3& axis, double angle)
{
    double sa = sin(angle/2);
    double ca = cos(angle/2);
    x() = float(axis.x()*sa);
    y() = float(axis.y()*sa);
    z() = float(axis.z()*sa);
    u() = float(ca);
}

float Quaternion::norm () const
{
    double n = 0;
    for (unsigned int i=0; i<4; i++)
    {
      n += operator()(i) * operator()(i);
    }
    return (float) sqrt(n);
}

void Quaternion::operator/= (float x)
{
    for (unsigned int i=0; i<4; ++i)
      operator()(i) /= x;
}

bool Quaternion::operator== (const Quaternion& other) const
{
    for (unsigned int i=0; i<4; i++)
    {
      if (operator()(i) != other(i))
        return false;
    }
    return true;
}


Vector3 Quaternion::toEuler() const
{
    // create rotational matrix
    double n = norm ();
    double s = n > 0?2./(n*n):0.;

    double xs = x()*s;
    double ys = y()*s;
    double zs = z()*s;

    double ux = u()*xs;
    double uy = u()*ys;
    double uz = u()*zs;

    double xx = x()*xs;
    double xy = x()*ys;
    double xz = x()*zs;

    double yy = y()*ys;
    double yz = y()*zs;
    double zz = z()*zs;

    double m[3][3];

    m[0][0] = 1.0 - (yy + zz);
    m[1][1] = 1.0 - (xx + zz);
    m[2][2] = 1.0 - (xx + yy);

    m[1][0] = xy + uz;
    m[0][1] = xy - uz;

    m[2][0] = xz - uy;
    m[0][2] = xz + uy;
    m[2][1] = yz + ux;
    m[1][2] = yz - ux;

    float roll  = (float) atan2(m[2][1], m[2][2]);
    float pitch = (float) atan2(-m[2][0], sqrt(m[2][1]*m[2][1] + m[2][2]*m[2][2]));
    float yaw   = (float) atan2(m[1][0], m[0][0]);

    return Vector3(roll, pitch, yaw);
}


void Quaternion::toRotMatrix(std::vector <double>& rot_matrix_3_3) const
{
    // create rotational matrix
    double n = norm ();
    double s = n > 0?2./(n*n):0.;

    double xs = x()*s;
    double ys = y()*s;
    double zs = z()*s;

    double ux = u()*xs;
    double uy = u()*ys;
    double uz = u()*zs;

    double xx = x()*xs;
    double xy = x()*ys;
    double xz = x()*zs;

    double yy = y()*ys;
    double yz = y()*zs;
    double zz = z()*zs;

    double m[3][3];
    m[0][0] = 1.0 - (yy + zz);
    m[1][1] = 1.0 - (xx + zz);
    m[2][2] = 1.0 - (xx + yy);

    m[1][0] = xy + uz;
    m[0][1] = xy - uz;

    m[2][0] = xz - uy;
    m[0][2] = xz + uy;
    m[2][1] = yz + ux;
    m[1][2] = yz - ux;

    rot_matrix_3_3.clear();
    rot_matrix_3_3.resize(9,0.);
    for (unsigned int i=0; i<3; i++)
    {
        rot_matrix_3_3[i*3] = m[i][0];
        rot_matrix_3_3[i*3+1] = m[i][1];
        rot_matrix_3_3[i*3+2] = m[i][2];
    }
}

Quaternion& Quaternion::operator= (const Quaternion& other)
{
    u() = other.u();
    x() = other.x();
    y() = other.y();
    z() = other.z();
    return *this;
}

Quaternion Quaternion::operator* (const Quaternion& other) const
{
    return Quaternion(u()*other.u() - x()*other.x() - y()*other.y() - z()*other.z(),
		      y()*other.z() - other.y()*z() + u()*other.x() + other.u()*x(),
		      z()*other.x() - other.z()*x() + u()*other.y() + other.u()*y(),
		      x()*other.y() - other.x()*y() + u()*other.z() + other.u()*z());
}

Quaternion Quaternion::operator* (const Vector3& v) const
{
    return *this * Quaternion(0, v(0), v(1), v(2));
}

Quaternion operator* (const Vector3& v, const Quaternion& q)
{
    return Quaternion(0, v(0), v(1), v(2)) * q;
}

Quaternion& Quaternion::normalize()
{
    double len = norm();
    if (len > 0)
      *this /= (float) len;
    return *this;
}

Quaternion Quaternion::normalized() const
{
    Quaternion result(*this);
    result.normalize();
    return result;
}

double Quaternion::dot(const Quaternion &other) const
{
    double n = u() * other.u() + x() * other.x() + y() * other.y() + z() * other.z();
    // clamp n to -1.0 , 1.0 to account for floating point precision errors
    return n < -1.0 ? -1.0 : (n > 1.0 ? 1.0 : n);
}

float Quaternion::getShortestAngleTo(const Quaternion &other) const
{
    return 2.0 * acos(fabs(dot(other)));
}

Quaternion Quaternion::lerp(const Quaternion &other, float t) const
{
    Quaternion quat;
    quat.u() = (1.0f - t) * u() + t * other.u();
    quat.x() = (1.0f - t) * x() + t * other.x();
    quat.y() = (1.0f - t) * y() + t * other.y();
    quat.z() = (1.0f - t) * z() + t * other.z();
    return quat;
}

Quaternion Quaternion::slerp(const Quaternion &other, double t) const
{
    // double theta = acos(dot(other));
    double d = dot(other);
    double absD = std::abs(d);

    double scale0;
    double scale1;

    if(absD == 1.0)
    {
        scale0 = 1.0 - t;
        scale1 = t;
    }
    else
    {
        double theta = std::acos(absD);
        double sinTheta = std::sin(theta);

        scale0 = std::sin((1.0 - t) * theta) / sinTheta;
        scale1 = std::sin((t * theta)) / sinTheta;
    }
    if (d < 0)
    {
        scale1 = -scale1;
    }

    return Quaternion(*this * scale0 + other * scale1);
}

void Quaternion::decomposeSwingTwist(const Vector3 &direction, Quaternion* swing, Quaternion* twist) const
{
    // swing is rotation around vector perpendicular to direction
    // twist is the rotation around direction vector
    Vector3 p = Vector3(x(), y(), z()).dot(direction) * direction;
    *twist = Quaternion(u(), p.x(), p.y(), p.z()).normalize();
    *swing = *this * twist->inv();
}

Quaternion Quaternion::getRotationAround(const Vector3 &direction) const
{
    Vector3 p = Vector3(x(), y(), z()).dot(direction) * direction;
    return Quaternion(u(), p.x(), p.y(), p.z()).normalize();
}

Quaternion Quaternion::operator*(float val) const
{
    return Quaternion(u() * val, x() * val, y() * val, z() * val);
}

Quaternion Quaternion::operator+(const Quaternion &other) const
{
    return Quaternion(u() + other.u(), x() + other.x(), y() + other.y(), z() + other.z());
}

Quaternion& Quaternion::inv_IP()
{
    x() = -x();
    y() = -y();
    z() = -z();
    return *this;
}

Vector3 Quaternion::rotate(const Vector3& v) const
{
    Quaternion q = *this * v * this->inv();
    return Vector3(q.x(), q.y(), q.z());
}


std::istream& Quaternion::read(std::istream &s)
{
    int temp;
    s >> temp; // should be 4
    for (unsigned int i=0; i<4; i++)
      s >> operator()(i);
    return s;
}


std::ostream& Quaternion::write(std::ostream &s) const
{
    s << 4;
    for (unsigned int i=0; i<4; i++)
      s << " " << operator()(i);
    return s;
}



std::istream& Quaternion::readBinary(std::istream &s)
{
    int temp;
    s.read((char*)&temp, sizeof(temp));
    double val = 0;
    for (unsigned int i=0; i<4; i++)
    {
      s.read((char*)&val, sizeof(val));
      operator()(i) = (float) val;
    }
    return s;
}


std::ostream& Quaternion::writeBinary(std::ostream &s) const
{
    int temp = 4;
    s.write((char*)&temp, sizeof(temp));
    double val = 0;
    for (unsigned int i=0; i<4; i++)
    {
      val = operator()(i);
      s.write((char*)&val, sizeof(val));
    }
    return s;
}



std::ostream& operator<<(std::ostream& s, const Quaternion& q)
{
    s << "(" << q.u() << " " << q.x() << " " << q.y() << " " << q.z() << ")";
    return s;
}

