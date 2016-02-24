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


#include "TimelineChannel.h"

TimelineChannel::TimelineChannel()
{
    _channelPos = -1;
    _boneId = -1;
}

TimelineChannel::TimelineChannel(int position)
{
    _channelPos = position;
    _boneId = -1;
}

TimelineChannel::~TimelineChannel()
{
    //dtor
}

void TimelineChannel::setChannelPos(int pos)
{
    _channelPos = pos;
    for (auto it = _tracks.begin(); it != _tracks.end(); ++it)
    {
        it->second->setChannel(pos);
    }
}

int TimelineChannel::getChannelPos() const
{
    return _channelPos;
}

void TimelineChannel::setBoneId(int boneId)
{
    _boneId = boneId;
}

int TimelineChannel::getBoneId() const
{
    return _boneId;
}

void TimelineChannel::insert(TimelineTrack* track, unsigned int time)
{
    track->setChannel(_channelPos);
    track->setStartTime(time);
    // TODO(JK#9#): handle the case when there is already a track at given timepoint
    _tracks[time] = track;
}

bool TimelineChannel::remove(int id, unsigned int timeHint)
{
    auto it = _tracks.lower_bound(timeHint);
    if (it != _tracks.end())
    {
        // check if lower bound track of hint is the track to be removed (should normally be the case)
        if (it->second->getId() == id)
        {
            it->second->setChannel(-1);
            it->second->setStartTime(0);
            _tracks.erase(it);
            return true;
        }
        // check if the track right before the lower bound is correct (if hint is a time point inside the track)
        if (it != _tracks.begin())
        {
            --it;
            if (it->second->getId() == id)
            {
                it->second->setChannel(-1);
                it->second->setStartTime(0);
                _tracks.erase(it);
                return true;
            }
        }
    }

    for (it = _tracks.begin(); it != _tracks.end(); ++it)
    {
        if (it->second->getId() == id)
        {
            it->second->setChannel(-1);
            it->second->setStartTime(0);
            _tracks.erase(it);
            return true;
        }
    }
    return false;
}

void TimelineChannel::clear()
{
    /*
    for (auto it = _tracks.begin(); it != _tracks.end(); ++it)
    {
        it->second->setChannel(-1);
        it->second->setStartTime(0);
    }
    */
    _tracks.clear();
}

TimelineTrack* TimelineChannel::getTrackBefore(unsigned int time)
{
    auto it = _tracks.upper_bound(time);
    if (it == _tracks.begin() || _tracks.size() == 0)
    {
        return nullptr;
    }
    --it;
    return it->second;
}

TimelineTrack* TimelineChannel::getTrackAfter(unsigned int time)
{
    auto it = _tracks.upper_bound(time);
    if (it == _tracks.end())
    {
        return nullptr;
    }
    return it->second;
}

bool TimelineChannel::isBetweenTwoTracks(unsigned int time) const
{
    auto it = _tracks.upper_bound(time);
    if (it == _tracks.begin() || it == _tracks.end())
    {
        return false;
    }
    --it;
    if (it->second->getStartTime() + it->second->getLength() <= time)
    {
        return true;
    }
    return false;
}

std::vector<TimelineTrack*> TimelineChannel::getInRange(unsigned int startTime, unsigned int endTime)
{
    std::vector<TimelineTrack*> tracks;

    if (endTime < startTime || _tracks.size() == 0)
    {
        return tracks;
    }
    auto it = _tracks.lower_bound(startTime);
    auto end = _tracks.upper_bound(endTime);

    // find tracks starting before startTime but ending after
    while (it != _tracks.begin())
    {
        --it;
        if (it->second->getStartTime() + it->second->getLength() <= startTime)
        {
            ++it;
            break;
        }
    }

    // store tracks
    while (it != end)
    {
        tracks.push_back(it->second);
        ++it;
    }
    return tracks;
}

TimelineTrack* TimelineChannel::getTrack(unsigned int time)
{
    std::vector<TimelineTrack*> tracks = getInRange(time, time);
    if (tracks.size() == 0)
    {
        return nullptr;
    }
    return tracks[0];
}

std::vector<TimelineTrack*> TimelineChannel::getTracks()
{
    std::vector<TimelineTrack*> tracks;
    for (auto it = _tracks.begin(); it != _tracks.end(); ++it)
    {
        tracks.push_back(it->second);
    }
    return tracks;
}

MotionSequenceFrame TimelineChannel::getFrame(unsigned int time)
{
    // TODO(JK#1#): get frame can get tricky when there is no track. Solve this by adding a default frame (equal to bone default)
    if (_tracks.size() == 0)
    {
        return MotionSequenceFrame();
    }

    TimelineTrack* track = getTrack(time);
    // if there is no track at specified time point, return default orientation
    if (track == nullptr)
    {
        return MotionSequenceFrame();
    }
    return track->getFrameFromAbsTime(time);
}




