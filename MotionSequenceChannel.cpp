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
    _frameTime = other._frameTime;
    _frames = other._frames;
}

MotionSequenceChannel::MotionSequenceChannel(int id, const MotionSequenceChannel &other) : _id(id)
{
    _name = other._name;
    _frameTime = other._frameTime;
    _frames = other._frames;
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


std::istream& MotionSequenceChannel::read(std::istream& s)
{
    std::getline(s, _name);

    s >> _id;
    s >> _frameTime;
    size_t numFrames;
    s >> numFrames;

    s.ignore(100, '\n');

    MotionSequenceFrame frame;
    _frames.clear();
    _frames.reserve(numFrames);
    for (size_t i = 0; i < numFrames; ++i)
    {
        frame.read(s);
        _frames.push_back(frame);
    }
    return s;
}

std::ostream& MotionSequenceChannel::write(std::ostream& s) const
{
    s << _name << std::endl << _id << ' ' << _frameTime << ' ' << _frames.size() << std::endl;

    // catch the first frame to avoid a ' ' at the beginning
    if (_frames.size() > 0)
    {
        _frames[0].write(s);
    }
    for (size_t i = 1; i < _frames.size(); ++i)
    {
        s << ' ';
        _frames[i].write(s);
    }
    return s;
}

std::istream& MotionSequenceChannel::readBinary(std::istream& s)
{
    size_t strLength;
    s.read((char*)&strLength, sizeof(size_t));
    _name.resize(strLength);
    s.read((char*)&_name[0], strLength);
    s.read((char*)&_id, sizeof(_id));
    s.read((char*)&_frameTime, sizeof(_frameTime));

    size_t numFrames;
    s.read((char*)&numFrames, sizeof(size_t));

    MotionSequenceFrame frame;
    _frames.clear();
    _frames.reserve(numFrames);

    for (size_t i = 0; i < numFrames; ++i)
    {
        frame.readBinary(s);
        _frames.push_back(frame);
    }
    return s;
}

std::ostream& MotionSequenceChannel::writeBinary(std::ostream& s) const
{
    size_t size = _name.size();
    s.write((char*)&size, sizeof(size_t));
    s.write((char*)&_name[0], _name.size());
    s.write((char*)&_id, sizeof(_id));
    s.write((char*)&_frameTime, sizeof(_frameTime));

    size = _frames.size();
    s.write((char*)&size, sizeof(size_t));

    for (size_t i = 0; i < _frames.size(); ++i)
    {
        _frames[i].writeBinary(s);
    }
    return s;
}

std::ostream& operator<<(std::ostream& out, const MotionSequenceChannel& channel)
{
    return channel.write(out);
}

std::istream& operator>>(std::istream& in, MotionSequenceChannel& channel)
{
    return channel.read(in);
}


