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


#ifndef AABB_H
#define AABB_H

#include "Vector3.h"
#include <vector>


class AABB
{
  public:
    AABB();
    AABB(const AABB &other);
    AABB(const Vector3 &minPoint, const Vector3 &maxPoint);
    virtual ~AABB();

    Vector3 min;
    Vector3 max;

	// Returns the minimum world-space coordinate along the given axis.
	inline float minX() const { return min.x(); }
	inline float minY() const { return min.y(); }
	inline float minZ() const { return min.z(); }
	// Returns the maximum world-space coordinate along the given axis.
	inline float maxX() const { return max.x(); }
	inline float maxY() const { return max.y(); }
    inline float maxZ() const { return max.z(); }


    void enclose(const Vector3 &point);

    void enclose(const std::vector<Vector3>& points);

    void setFrom(const std::vector<Vector3> &points);

    bool isInside(const Vector3 &point) const;

    inline bool isValid() { return min.x() < max.x() && min.y() < max.y() && min.z() < max.z(); }

    inline Vector3 getCenter() const { return (min + max) * 0.5f; }

    inline Vector3 getSize() const { return max - min; }

    inline Vector3 getHalfSize() const { return getSize() * 0.5f; }

    float getVolume() const;

    float getSurfaceArea() const;

    inline AABB operator+(const Vector3 &vec) { return AABB(min + vec, max + vec); }
    inline void operator+=(const Vector3 &vec) { *this = *this + vec; }

    inline AABB operator-(const Vector3 &vec) { return AABB(min - vec, max - vec); }
    inline void operator-=(const Vector3 &vec) { *this = *this - vec; }

  protected:

  private:
};

#endif // AABB_H
