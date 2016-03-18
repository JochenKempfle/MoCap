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
#include "Skeleton.h"
#include <vector>
#include <map>
#include <set>
#include <string>

class Timeline
{
  public:
    Timeline();
    virtual ~Timeline();

    void setSkeletonToTime(uint64_t time);

    void setSkeleton(Skeleton* skeleton);
    Skeleton* getSkeleton();
    bool hasSkeleton() const { return _skeleton.getRootId() >= 0; }

    void setChannelAffiliation(int channel, int boneId);
    int getChannelAffiliation(int channelId) const;
    std::string getChannelAffiliationName(int channelId) const;

    // TODO(JK#1#): maybe make toChannel unsigned
    void insert(MotionSequence* sequence, int toChannel, uint64_t time);
    void insert(MotionSequenceChannel* channel, int toChannel, uint64_t time, std::string name = "");
    void insert(const TimelineTrack &track, int toChannel, uint64_t time);

    void insertChannelAfter(int channel);
    void moveChannelsDown(int startChannel, unsigned int numChannels);
    void moveTrack(int trackId, int toChannel, uint64_t toTime);

    bool erase(int trackId);

    void clear();

    void swapChannels(int channel1, int channel2);
    void clearChannel(int channel);
    void sortChannels();

    // cut the track at given absolute time point (if it covers the time point)
    void cut(int trackId, uint64_t time);
    // cut all tracks covering the given time point at this time point
    void cut(uint64_t time);

    size_t getNumTracks() const { return _tracks.size(); }

    TimelineTrack* getTrack(int id);
    TimelineTrack* getTrack(int channel, uint64_t time);

    TimelineTrack* getTrackBefore(int channel, uint64_t time);
    TimelineTrack* getTrackAfter(int channel, uint64_t time);

    bool isBetweenTwoTracks(int channel, uint64_t time) const;
    bool isInsideTrack(int channel, uint64_t time) const;

    std::vector<TimelineTrack*> getOverlapping(const TimelineTrack* track);

    std::vector<TimelineTrack*> getInRange(int channel, uint64_t startTime, uint64_t endTime);

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
