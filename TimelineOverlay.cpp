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


#include "TimelineOverlay.h"
#include "TimelineTrack.h"
#include <cmath>

TimelineOverlay::TimelineOverlay()
{
    _startTime = 0;
    _length = 0;
    _track1 = nullptr;
    _track2 = nullptr;
    _priority = 1;
    _type = OverlayType::ADDITIVE;
}

TimelineOverlay::~TimelineOverlay()
{
    //dtor
}

void TimelineOverlay::setTracks(TimelineTrack* track1, TimelineTrack* track2)
{
    _track1 = track1;
    _track2 = track2;
}

TimelineTrack* TimelineOverlay::getFirstTrack() const
{
    return _track1->getChannel() < _track2->getChannel() ? _track1 : _track2;
}

TimelineTrack* TimelineOverlay::getSecondTrack() const
{
    return _track1->getChannel() < _track2->getChannel() ? _track2 : _track1;
}

int TimelineOverlay::getChannelDifference() const
{
    return std::abs(_track1->getChannel() - _track2->getChannel());
}

bool TimelineOverlay::compPriority(TimelineOverlay* overlay1, TimelineOverlay* overlay2)
{
    if (overlay1->_priority != overlay2->_priority)
    {
        return overlay1->_priority < overlay2->_priority;
    }
    return overlay1->getFirstTrack()->getChannel() < overlay2->getFirstTrack()->getChannel();
}

std::istream& TimelineOverlay::read(std::istream& s)
{
    s >> _startTime;
    s >> _length;
    s >> _priority;
    unsigned char type;
    s >> type;
    _type = static_cast<OverlayType>(type);
    return s;
}

std::ostream& TimelineOverlay::write(std::ostream& s) const
{
    s << _startTime << ' ' << _length << ' ' << _priority << ' ' << static_cast<unsigned char>(_type);
    return s;
}

std::istream& TimelineOverlay::readBinary(std::istream& s)
{
    s.read((char*)&_startTime, sizeof(_startTime));
    s.read((char*)&_length, sizeof(_length));
    s.read((char*)&_priority, sizeof(_priority));
    s.read((char*)&_type, sizeof(_type));
    return s;
}

std::ostream& TimelineOverlay::writeBinary(std::ostream& s) const
{
    s.write((char*)&_startTime, sizeof(_startTime));
    s.write((char*)&_length, sizeof(_length));
    s.write((char*)&_priority, sizeof(_priority));
    s.write((char*)&_type, sizeof(_type));
    return s;
}

std::ostream& operator<<(std::ostream& out, const TimelineOverlay& overlay)
{
    return overlay.write(out);
}

std::istream& operator>>(std::istream& in, TimelineOverlay& overlay)
{
    return overlay.read(in);
}
