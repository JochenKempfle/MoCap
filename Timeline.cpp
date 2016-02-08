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


#include "Timeline.h"

Timeline::Timeline()
{
    _nextTrackId = 0;
}

Timeline::~Timeline()
{
    //dtor
}

void Timeline::setChannelAffiliation(int channelId, int boneId)
{
    auto it = _channels.find(channelId);
    // if channel not yet exists, do nothing
    if (it == _channels.end())
    {
        return;
    }
    int oldBoneId = it->second.getBoneId();
    it->second.setBoneId(boneId);
    _boneToChannel[oldBoneId].erase(channelId);
    _boneToChannel[boneId].insert(channelId);
}

int Timeline::getChannelAffiliation(int channelId) const
{
    auto it = _channels.find(channelId);
    // if channel not yet exists, return -1
    if (it == _channels.end())
    {
        return -1;
    }
    return it->second.getBoneId();
}

void Timeline::insert(MotionSequence* sequence, int toChannel, unsigned int time)
{
    if (sequence->getNumChannels() == 0)
    {
        return;
    }
    for (auto it = sequence->beginChannels(); it != sequence->endChannels(); ++it)
    {
        insert(it->second, toChannel++, time);
    }
}

void Timeline::insert(MotionSequenceChannel* channel, int toChannel, unsigned int time, std::string name)
{
    TimelineTrack* newTrack = new TimelineTrack(channel);
    newTrack->setId(_nextTrackId++);
    newTrack->setChannel(toChannel);
    newTrack->setName(name);

    // if the channel did not exist yet, initialize it with the correct channel position
    if (_channels[toChannel].getChannelPos() != toChannel)
    {
        _channels[toChannel].setChannelPos(toChannel);
    }

    _channels[toChannel].insert(newTrack, time);
}

void Timeline::insert(const TimelineTrack &track, int toChannel, unsigned int time, std::string name)
{
    TimelineTrack* newTrack = new TimelineTrack(track);
    newTrack->setId(_nextTrackId++);
    newTrack->setChannel(toChannel);
    newTrack->setName(name);

    // if the channel did not exist yet, initialize it with the correct channel position
    if (_channels[toChannel].getChannelPos() != toChannel)
    {
        _channels[toChannel].setChannelPos(toChannel);
    }

    _channels[toChannel].insert(newTrack, time);
}

void Timeline::moveTrack(int trackId, int toChannel, unsigned int toTime)
{
    auto it = _tracks.find(trackId);
    if (it == _tracks.end())
    {
        return;
    }

    TimelineTrack* track = it->second;

    _channels[track->getChannel()].remove(trackId, track->getStartTime());
    _channels[toChannel].insert(track, toTime);
}

bool Timeline::erase(int trackId)
{
    auto it = _tracks.find(trackId);
    if (it == _tracks.end())
    {
        return false;
    }

    TimelineTrack* track = it->second;
    _channels[track->getChannel()].remove(trackId, track->getStartTime());
    _tracks.erase(it);
    delete track;

    return true;
}

void Timeline::swapChannels(int channel1, int channel2)
{
    TimelineChannel channel = _channels[channel1];
    _channels[channel1] = _channels[channel2];
    _channels[channel2] = channel;

    _channels[channel1].setChannelPos(channel1);
    _channels[channel2].setChannelPos(channel2);
}

void Timeline::clearChannel(int channel)
{
    std::vector<TimelineTrack*> tracks = _channels[channel].getTracks();
    for (size_t i = 0; i < tracks.size(); ++i)
    {
        erase(tracks[i]->getId());
    }
    _channels[channel].clear();
}

void Timeline::sortChannels()
{

}

TimelineTrack* Timeline::getTrack(int id)
{
    auto it = _tracks.find(id);
    if (it == _tracks.end())
    {
        return nullptr;
    }
    return it->second;
}

TimelineTrack* Timeline::getTrack(int channel, unsigned int time)
{
    return _channels[channel].getTrack(time);
}

std::vector<TimelineTrack*> Timeline::getInRange(int channel, unsigned int startTime, unsigned int endTime)
{
    return _channels[channel].getInRange(startTime, endTime);
}






