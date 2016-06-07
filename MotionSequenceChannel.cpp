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


#include "MotionSequenceChannel.h"

MotionSequenceChannel::MotionSequenceChannel() : _id(-1)
{

}

MotionSequenceChannel::MotionSequenceChannel(int id) : _id(id)
{

}

MotionSequenceChannel::MotionSequenceChannel(const MotionSequenceChannel &other)
{
    _id = other._id;
    _name = other._name;
    _frames = other._frames;
    _frameTime = other._frameTime;
    _initialPose = other._initialPose;
    _boneLength = other._boneLength;
}

MotionSequenceChannel::MotionSequenceChannel(int id, const MotionSequenceChannel &other) : _id(id)
{
    _name = other._name;
    _frames = other._frames;
    _frameTime = other._frameTime;
    _initialPose = other._initialPose;
    _boneLength = other._boneLength;
}

MotionSequenceChannel::~MotionSequenceChannel()
{
    //dtor
}

void MotionSequenceChannel::setName(std::string name)
{
    _name = name;
}

std::string MotionSequenceChannel::getName() const
{
    return _name;
}

void MotionSequenceChannel::setFrame(size_t frameNumber, const MotionSequenceFrame &frame)
{
    if (frameNumber < _frames.size())
    {
        _frames[frameNumber] = frame;
    }
    else
    {
        _frames.push_back(frame);
    }
}

void MotionSequenceChannel::appendFrame(const MotionSequenceFrame &frame)
{
    _frames.push_back(frame);
}

MotionSequenceFrame MotionSequenceChannel::getFrame(size_t frameNumber) const
{
    if (_frames.size() == 0)
    {
        return MotionSequenceFrame();
    }
    if (frameNumber < _frames.size())
    {
        return _frames[frameNumber];
    }
    return _frames[_frames.size() - 1];
}

MotionSequenceFrame MotionSequenceChannel::getLastFrame() const
{
    if (_frames.size() == 0)
    {
        return MotionSequenceFrame();
    }
    return _frames[_frames.size() - 1];
}

size_t MotionSequenceChannel::getNumFrames() const
{
    return _frames.size();
}

void MotionSequenceChannel::clear()
{
    _frames.clear();
}

void MotionSequenceChannel::setBoneLength(float length)
{
    _boneLength = length;
}

float MotionSequenceChannel::getBoneLength() const
{
    return _boneLength;
}

void MotionSequenceChannel::setInitialPose(MotionSequenceFrame pose)
{
    _initialPose = pose;
}

MotionSequenceFrame MotionSequenceChannel::getInitialPose() const
{
    return _initialPose;
}

