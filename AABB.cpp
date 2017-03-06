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


#include "AABB.h"
#include <limits>

AABB::AABB() : min(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()),
               max(-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity())
{

}

AABB::AABB(const AABB& other) : min(other.min), max(other.max)
{

}

AABB::AABB(const Vector3& minPoint, const Vector3& maxPoint) : min(minPoint), max(maxPoint)
{

}

AABB::~AABB()
{
    //dtor
}


void AABB::enclose(const Vector3& point)
{
    min.x() = std::min(min.x(), point.x());
    min.x() = std::min(min.y(), point.y());
    min.x() = std::min(min.z(), point.z());

    max.x() = std::max(max.x(), point.x());
    max.x() = std::max(max.y(), point.y());
    max.x() = std::max(max.z(), point.z());
}

void AABB::enclose(const std::vector<Vector3>& points)
{
    for (size_t i = 0; i < points.size(); ++i)
    {
        enclose(points[i]);
    }
}

void AABB::setFrom(const std::vector<Vector3>& points)
{
    min = Vector3(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
    max = Vector3(-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity());
    for (size_t i = 0; i < points.size(); ++i)
    {
        enclose(points[i]);
    }
}

bool AABB::isInside(const Vector3& point) const
{
    return min.x() <= point.x() && max.x() >= point.x() && min.y() <= point.y() && max.y() >= point.y() && min.z() <= point.z() && max.z() >= point.z();
}

float AABB::getVolume() const
{
    Vector3 size = getSize();
    return size.x() * size.y() * size.z();
}

float AABB::getSurfaceArea() const
{
    Vector3 size = getSize();
    return 2.0f * (size.x() * size.y() + size.x() * size.z() + size.y() * size.z());
}
