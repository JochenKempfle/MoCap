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
    _startOffset = 0;
    _endOffset = 0;
    _frameTime = 0.0f;
}

TimelineTrack::TimelineTrack(MotionSequenceChannel* channel)
{
    _id = -1;
    _channel = -1;
    _startTime = 0;
    _startOffset = 0;
    _endOffset = 0;

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

TimelineTrack::TimelineTrack(MotionSequenceChannel* channel, int startTime)
{
    _id = -1;
    _channel = -1;
    _startOffset = 0;
    _endOffset = 0;

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

void TimelineTrack::setStartTime(unsigned int startTime)
{
    _startTime = startTime;
}

unsigned int TimelineTrack::getStartTime() const
{
    return _startTime;
}

unsigned int TimelineTrack::getEndTime() const
{
    return _startTime + getLength();
}

int TimelineTrack::getLength() const
{
    return round(1000 * _frames.size() * _frameTime);
}

int TimelineTrack::getMaxLength() const
{
    return 1000 * _frames.size() * _frameTime;
}

void TimelineTrack::setStartOffset(int offset)
{
    _startOffset = offset;
}

int TimelineTrack::getStartOffset() const
{
    return _startOffset;
}

void TimelineTrack::setEndOffset(int offset)
{
    _endOffset = offset;
}

int TimelineTrack::getEndOffset() const
{
    return _endOffset;
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

bool TimelineTrack::cut(unsigned int time, TimelineTrack* newTrack)
{
    // TODO(JK#1#): implement cutting
    unsigned int pos = (time/_frameTime)/1000;
    if (pos <= 0 || pos >= _frames.size())
    {
        return false;
    }
    if (newTrack != nullptr)
    {
        newTrack->_frames.insert(newTrack->_frames.begin(), _frames.begin() + pos, _frames.end());
        newTrack->_channel = _channel;
        newTrack->_name = _name;
        newTrack->_startTime = _startTime + round(pos * _frameTime * 1000.0);
        newTrack->_frameTime = _frameTime;
    }
    _frames.resize(pos);
    return true;
}

MotionSequenceFrame TimelineTrack::getFrameFromAbsTime(unsigned int time) const
{
    if (time < _startTime)
    {
        return MotionSequenceFrame();
    }
    unsigned int pos = ((time - _startTime)/_frameTime)/1000 + _startOffset;
    if (pos >= _frames.size())
    {
        // TODO(JK#1#): do not return the last frame, also consider returning a pointer or a reference
        return _frames.back();
    }
    return _frames[pos];
}

MotionSequenceFrame TimelineTrack::getFrameFromRelTime(unsigned int time) const
{
    unsigned int pos = (time/_frameTime)/1000 + _startOffset;
    if (pos >= _frames.size())
    {
        return _frames.back();
    }
    return _frames[pos];
}

MotionSequenceFrame TimelineTrack::getFrame(unsigned int pos) const
{
    if (pos >= _frames.size())
    {
        return _frames.back();
    }
    return _frames[pos];
}

