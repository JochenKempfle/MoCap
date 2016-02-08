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

class TimelineChannel
{
  public:
    TimelineChannel();
    virtual ~TimelineChannel();

    void setChannelPos(int pos);
    int getChannelPos() const;

    void setBoneId(int boneId);
    int getBoneId() const;

    // Insert a track at time point given in ms
    void insert(TimelineTrack* track, unsigned int time);

    // Remove the track with given id. Use timeHint to give a hint where the track may be located (see MotionTrack::getStartTime()).
    // Returns true, if the track was found and successfully removed, else false.
    // Only removes the track from the channel, but does not delete it (use Timeline::erase() for this)
    bool remove(int id, unsigned int timeHint = 0);

    void clear();

    // returns all tracks having a portion in the given range in ms
    std::vector<TimelineTrack*> getInRange(unsigned int startTime, unsigned int endTime);

    TimelineTrack* getTrack(unsigned int time);
    std::vector<TimelineTrack*> getTracks();
    MotionSequenceFrame getFrame(unsigned int time);
  protected:

  private:
    int _channelPos;
    int _boneId;
    std::map<unsigned int, TimelineTrack*> _tracks;
};

#endif // TIMELINECHANNEL_H
