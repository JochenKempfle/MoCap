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


#ifndef TIMELINECHANNEL_H
#define TIMELINECHANNEL_H

#include "TimelineTrack.h"
#include <cstdint>
#include <limits>

class TimelineChannel
{
  public:
    TimelineChannel();
    TimelineChannel(int position);
    virtual ~TimelineChannel();

    void setChannelPos(int pos);
    int getChannelPos() const;

    void setBoneId(int boneId);
    int getBoneId() const;

    // Insert a track at time point given in ms
    void insert(TimelineTrack* track, uint64_t time);

    // Remove the track with given id. Use timeHint to give a hint where the track may be located (see MotionTrack::getStartTime()).
    // Returns true, if the track was found and successfully removed, else false.
    // Only removes the track from the channel, but does not delete it (use Timeline::erase() for this)
    bool remove(int id, uint64_t timeHint = 0);

    void clear();

    // returns the start time of the first track, max(uint64_t) if no track is added
    uint64_t getMinTime() const;
    // returns the end time of the last track, 0 if no track is added
    uint64_t getMaxTime() const;

    size_t getNumTracks() const { return _tracks.size(); }

    TimelineTrack* getTrackBefore(uint64_t time);
    TimelineTrack* getTrackAfter(uint64_t time);

    bool isBetweenTwoTracks(uint64_t time) const;
    bool isInsideTrack(uint64_t time) const;

    // returns all tracks having a portion in the given range in ms
    std::vector<TimelineTrack*> getInRange(uint64_t startTime, uint64_t endTime);

    TimelineTrack* getTrack(uint64_t time);
    std::vector<TimelineTrack*> getTracks();
    //MotionSequenceFrame getFrame(uint64_t time);

    bool operator<(const TimelineChannel &other) { return _channelPos < other._channelPos; }

    // read and write methods will not write the channel content
    std::istream& read(std::istream &s);
    std::ostream& write(std::ostream &s) const;
    std::istream& readBinary(std::istream &s);
    std::ostream& writeBinary(std::ostream &s) const;

  protected:

  private:
    int _channelPos;
    int _boneId;
    std::map<uint64_t, TimelineTrack*> _tracks;
};

std::ostream& operator<<(std::ostream& out, const TimelineChannel& channel);

std::istream& operator>>(std::istream& in, TimelineChannel& channel);

#endif // TIMELINECHANNEL_H
