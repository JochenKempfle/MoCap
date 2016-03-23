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


#ifndef TIMELINEOVERLAY_H
#define TIMELINEOVERLAY_H

enum class OverlayType : unsigned char
{
    ADDITIVE = 1,
    SUBTRACTIVE = 2,
    INTERPOLATION = 4,
    OVERWRITE = 8
};

class TimelineTrack;

class TimelineOverlay
{
  public:
    TimelineOverlay();
    virtual ~TimelineOverlay();

    void setStartTime(uint64_t startTime) { _startTime = startTime; }
    uint64_t getStartTime() const { return _startTime; }

    void setLength(uint64_t length) { _length = length; }
    uint64_t getLength() const { return _length; }

    void setTracks(TimelineTrack* track1, TimelineTrack* track2) { _track1 = track1; _track2 = track2; }
    TimelineTrack* getFirstTrack() const { return _track1; }
    TimelineTrack* getSecondTrack() const { return _track2; }

    void setPriority(unsigned char priority) { _priority = priority; }
    unsigned char getPriority() const { return _priority; }

    void setType(OverlayType type) { _type = type; }
    OverlayType getType() const { return _type; }

  protected:

  private:
    uint64_t _startTime;
    uint64_t _length;
    TimelineTrack* _track1;
    TimelineTrack* _track2;
    unsigned char _priority;
    OverlayType _type;
};

#endif // TIMELINEOVERLAY_H