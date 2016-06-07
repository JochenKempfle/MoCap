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


#include "TimelineTrack.h"

TimelineTrack::TimelineTrack()
{
    _id = -1;
    _channel = -1;
    _startTime = 0;
    _frameTime = 0.0f;
}

TimelineTrack::TimelineTrack(MotionSequenceChannel* channel)
{
    _id = -1;
    _channel = -1;
    _startTime = 0;

    if (channel != nullptr)
    {
        _name = channel->getName();
        _frameTime = channel->getFrameTime();
        _frames = channel->getFrames();
    }
    else
    {
        _frameTime = 0.0f;
    }
}

TimelineTrack::TimelineTrack(MotionSequenceChannel* channel, uint64_t startTime)
{
    _id = -1;
    _channel = -1;

    if (channel != nullptr)
    {
        _name = channel->getName();
        _startTime = startTime;
        _frameTime = channel->getFrameTime();
        _frames = channel->getFrames();
    }
    else
    {
        _startTime = startTime;
        _frameTime = 0.0f;
    }
}

TimelineTrack::~TimelineTrack()
{
    //dtor
}

void TimelineTrack::setId(int id)
{
    _id = id;
}

int TimelineTrack::getId() const
{
    return _id;
}

void TimelineTrack::setChannel(int channel)
{
    _channel = channel;
}

int TimelineTrack::getChannel() const
{
    return _channel;
}

void TimelineTrack::setName(std::string name)
{
    _name = name;
}

std::string TimelineTrack::getName() const
{
    return _name;
}

void TimelineTrack::setStartTime(uint64_t startTime)
{
    _startTime = startTime;
}

uint64_t TimelineTrack::getStartTime() const
{
    return _startTime;
}

uint64_t TimelineTrack::getEndTime() const
{
    return _startTime + getLength();
}

uint64_t TimelineTrack::getLength() const
{
    return 1000000 * uint64_t(_frames.size()) * _frameTime;
}

void TimelineTrack::setFrameTime(float frameTime)
{
    _frameTime = frameTime;
}

float TimelineTrack::getFrameTime() const
{
    return _frameTime;
}

void TimelineTrack::clear()
{
    _frames.clear();
}

void TimelineTrack::setFrames(const std::vector<MotionSequenceFrame> &frames)
{
    _frames = frames;
}
void TimelineTrack::appendFrames(const std::vector<MotionSequenceFrame> &frames)
{
    _frames.insert(_frames.end(), frames.begin(), frames.end());
}

void TimelineTrack::appendFrame(const MotionSequenceFrame &frame)
{
    _frames.push_back(frame);
}

size_t TimelineTrack::getNumFrames() const
{
    return _frames.size();
}

bool TimelineTrack::cut(uint64_t time, TimelineTrack* newTrack)
{
    unsigned int pos = (time/_frameTime)/1000000;
    if (pos <= 0 || pos >= _frames.size())
    {
        return false;
    }
    if (newTrack != nullptr)
    {
        newTrack->_frames.insert(newTrack->_frames.begin(), _frames.begin() + pos, _frames.end());
        newTrack->_channel = _channel;
        newTrack->_name = _name;
        newTrack->_startTime = _startTime + pos * _frameTime * 1000000.0;
        newTrack->_frameTime = _frameTime;
        if (getWeight(pos) != 1.0f)
        {
            newTrack->_weightPoints[0] = getWeight(pos);
            setWeightPoint(pos - 1, getWeight(pos - 1));
        }
        for (auto it = _weightPoints.lower_bound(pos); it != _weightPoints.end(); ++it)
        {
            newTrack->_weightPoints[it->first - pos] = it->second;
        }
    }
    _frames.resize(pos);
    _weightPoints.erase(_weightPoints.lower_bound(pos), _weightPoints.end());
    return true;
}

uint64_t TimelineTrack::getAbsTimeFromFrame(unsigned int frame) const
{
    return _startTime + frame * _frameTime * 1000000.0f;
}

uint64_t TimelineTrack::getRelTimeFromFrame(unsigned int frame) const
{
    return frame * _frameTime * 1000000.0f;
}


unsigned int TimelineTrack::getFrameNumFromAbsTime(uint64_t time) const
{
    if (time < _startTime)
    {
        return 0;
    }
    return ((time - _startTime)/_frameTime)/1000000;
}

unsigned int TimelineTrack::getFrameNumFromRelTime(uint64_t time) const
{
    return (time/_frameTime)/1000000;
}


MotionSequenceFrame TimelineTrack::getFrameFromAbsTime(uint64_t time, bool weighted) const
{
    if (time < _startTime)
    {
        // TODO(JK#9#): what to return if the time in getFrame - functions is outside the boundary?
        return MotionSequenceFrame();
    }
    unsigned int pos = ((time - _startTime)/_frameTime)/1000000;
    return getFrame(pos, weighted);
}

MotionSequenceFrame TimelineTrack::getFrameFromRelTime(uint64_t time, bool weighted) const
{
    unsigned int pos = (time/_frameTime)/1000000;
    return getFrame(pos, weighted);
}

MotionSequenceFrame TimelineTrack::getFrame(unsigned int pos, bool weighted) const
{
    if (pos >= _frames.size())
    {
        return MotionSequenceFrame();
    }
    MotionSequenceFrame frame = _frames[pos];
    if (weighted)
    {
        frame.setOrientation(frame.getOrientation().pow(getWeight(pos)));
        // TODO(JK#9#): should the frame position data be weighted?
        /*
        if frame.hasPositionData()
        {
            frame.setPosition(frame.getPosition() * getWeight(pos));
        }
        */
    }
    return frame;
}

MotionSequenceFrame TimelineTrack::getFirstFrame(bool weighted) const
{
    return getFrame(0, weighted);
}

MotionSequenceFrame TimelineTrack::getLastFrame(bool weighted) const
{
    return getFrame(_frames.size() - 1, weighted);
}

bool TimelineTrack::isInside(uint64_t time) const
{
    return time >= _startTime && time <= getEndTime();
}

void TimelineTrack::setWeightPoint(unsigned int frame, float weight)
{
    if (frame < _frames.size())
    {
        _weightPoints[frame] = weight;
    }
}

void TimelineTrack::moveWeightPoint(unsigned int oldFrame, unsigned int newFrame, float weight)
{
    removeWeightPoint(oldFrame);
    setWeightPoint(newFrame, weight);
}

void TimelineTrack::removeWeightPoint(unsigned int frame)
{
    auto it = _weightPoints.find(frame);
    if (it != _weightPoints.end())
    {
        _weightPoints.erase(it);
    }
}

std::vector<std::pair<unsigned int, float> > TimelineTrack::getWeightPoints() const
{
    std::vector<std::pair<unsigned int, float> > weightPoints;
    weightPoints.reserve(_weightPoints.size());
    for (auto it = _weightPoints.begin(); it != _weightPoints.end(); ++it)
    {
        weightPoints.push_back(*it);
    }
    return weightPoints;
}

float TimelineTrack::getWeight(unsigned int frame) const
{
    if (_weightPoints.empty())
    {
        return 1.0f;
    }
    if (frame >= _frames.size())
    {
        return 0.0f;
    }
    auto it = _weightPoints.lower_bound(frame);
    if (it != _weightPoints.end())
    {
        if (it->first == frame)
        {
            return it->second;
        }
    }
    if (it == _weightPoints.begin())
    {
        double t = double(frame) / it->first;
        return (1.0 - t) + t * it->second;
    }
    auto it2 = it;
    --it;
    if (it2 == _weightPoints.end())
    {
        double t = double(frame - it->first) / (_frames.size() - 1 - it->first);
        return (1.0 - t) * it->second + t;
    }
    double t = double(frame - it->first) / (it2->first - it->first);
    return (1.0 - t) * it->second + t * it2->second;
}

