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


#ifndef MOTIONSEQUENCEFRAME_H
#define MOTIONSEQUENCEFRAME_H

#include "Quaternion.h"
#include "Vector3.h"
#include <iostream>

class MotionSequenceFrame
{
  public:
    MotionSequenceFrame();
    MotionSequenceFrame(Quaternion orientation);
    MotionSequenceFrame(Quaternion orientation, Vector3 position);
    virtual ~MotionSequenceFrame();
    void setOrientation(Quaternion orientation);
    Quaternion getOrientation() const;
    void setPosition(Vector3 position);
    Vector3 getPosition() const;
    bool hasPositionData() const;

    std::istream& read(std::istream &s);
    std::ostream& write(std::ostream &s) const;
    std::istream& readBinary(std::istream &s);
    std::ostream& writeBinary(std::ostream &s) const;

  protected:
  private:
    Quaternion _orientation;
    Vector3 _position;
    bool _hasPositionData;
    // TODO(JK#9#): maybe add bone length to a frame (then a bone can change its length over time)
};

//! output in format <bool quat vec3>
std::ostream& operator<<(std::ostream& out, const MotionSequenceFrame& frame);

//! input in format <bool quat vec3>
std::istream& operator>>(std::istream& in, MotionSequenceFrame& frame);

#endif // MOTIONSEQUENCEFRAME_H
