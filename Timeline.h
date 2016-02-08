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


#ifndef TIMELINE_H
#define TIMELINE_H

#include "TimelineChannel.h"
#include "TimelineTrack.h"
#include <vector>
#include <map>
#include <set>

class Timeline
{
  public:
    Timeline();
    virtual ~Timeline();

    void setChannelAffiliation(int channelId, int boneId);
    int getChannelAffiliation(int channelId) const;

    void insert(MotionSequence* sequence, int toChannel, unsigned int time);
    void insert(MotionSequenceChannel* channel, int toChannel, unsigned int time, std::string name = "");
    void insert(const TimelineTrack &track, int toChannel, unsigned int time, std::string name = "");

    void moveTrack(int trackId, int toChannel, unsigned int toTime);

    bool erase(int trackId);

    void swapChannels(int channel1, int channel2);
    void clearChannel(int channel);
    void sortChannels();

    TimelineTrack* getTrack(int id);
    TimelineTrack* getTrack(int channel, unsigned int time);

    std::vector<TimelineTrack*> getInRange(int channel, unsigned int startTime, unsigned int endTime);

    std::map<int, TimelineChannel>::iterator beginChannels() { return _channels.begin(); }
    std::map<int, TimelineChannel>::iterator endChannels() { return _channels.end(); }

  protected:

  private:
    int _nextTrackId;
    Skeleton _skeleton;
    std::map<int, TimelineTrack*> _tracks;
    std::map<int, std::set<int> > _boneToChannel;
    std::map<int, TimelineChannel> _channels;
};

#endif // TIMELINE_H
