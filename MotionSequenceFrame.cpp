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


#include "MotionSequenceFrame.h"

MotionSequenceFrame::MotionSequenceFrame()
{
    _hasPositionData = false;
}

MotionSequenceFrame::MotionSequenceFrame(Quaternion orientation) : _orientation(orientation), _hasPositionData(false)
{

}

MotionSequenceFrame::MotionSequenceFrame(Quaternion orientation, Vector3 position)
{
    _hasPositionData = true;
    _orientation = orientation;
    _position = position;
}

MotionSequenceFrame::~MotionSequenceFrame()
{
    //dtor
}


void MotionSequenceFrame::setOrientation(Quaternion orientation)
{
    _orientation = orientation;
}

Quaternion MotionSequenceFrame::getOrientation() const
{
    return _orientation;
}

void MotionSequenceFrame::setPosition(Vector3 position)
{
    _position = position;
    _hasPositionData = true;
}

Vector3 MotionSequenceFrame::getPosition() const
{
    return _position;
}

bool MotionSequenceFrame::hasPositionData() const
{
    return _hasPositionData;
}


std::istream& MotionSequenceFrame::read(std::istream& s)
{
    s >> _hasPositionData;
    _orientation.read(s);
    if (_hasPositionData)
    {
        _position.read(s);
    }
    return s;
}

std::ostream& MotionSequenceFrame::write(std::ostream& s) const
{
    s << _hasPositionData << ' ';
    _orientation.write(s);
    if (_hasPositionData)
    {
        s << ' ';
        _position.write(s);
    }
    return s;
}

std::istream& MotionSequenceFrame::readBinary(std::istream& s)
{
    s.read((char*)&_hasPositionData, sizeof(_hasPositionData));
    _orientation.readBinary(s);
    if (_hasPositionData)
    {
        _position.readBinary(s);
    }
    return s;
}

std::ostream& MotionSequenceFrame::writeBinary(std::ostream& s) const
{
    s.write((char*)&_hasPositionData, sizeof(_hasPositionData));
    _orientation.writeBinary(s);
    if (_hasPositionData)
    {
        _position.writeBinary(s);
    }
    return s;
}

std::ostream& operator<<(std::ostream& out, const MotionSequenceFrame& frame)
{
    return frame.write(out);
}

std::istream& operator>>(std::istream& in, MotionSequenceFrame& frame)
{
    return frame.read(in);
}
