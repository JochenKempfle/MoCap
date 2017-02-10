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


#include "MotionSequence.h"

MotionSequence::MotionSequence()
{
    //ctor
    _numFrames = 0;
    _frameTime = 0.0;
    _hasAbsOrientations = false;
}

MotionSequence::MotionSequence(const MotionSequence &other)
{
    _name = other._name;
    _skeleton = other._skeleton;
    _numFrames = other._numFrames;
    _frameTime = other._frameTime;
    _hasAbsOrientations = other._hasAbsOrientations;
    for (auto it = other._channels.begin(); it != other._channels.end(); ++it)
    {
        _channels[it->first] = new MotionSequenceChannel(*(it->second));
    }
}

MotionSequence::MotionSequence(MotionSequence &&other)
{
    _name = other._name;
    _skeleton = other._skeleton;
    _numFrames = other._numFrames;
    _frameTime = other._frameTime;
    _hasAbsOrientations = other._hasAbsOrientations;
    std::swap(_channels, other._channels);
}

MotionSequence::~MotionSequence()
{
    clear();
}

void MotionSequence::setName(std::string name)
{
    _name = name;
}

std::string MotionSequence::getName() const
{
    return _name;
}

void MotionSequence::setFrameTime(float time)
{
    _frameTime = time;
    for (auto it = _channels.begin(); it != _channels.end(); ++it)
    {
        it->second->setFrameTime(time);
    }
}

void MotionSequence::convertToRelOrientations()
{
    if (_hasAbsOrientations == false)
    {
        return;
    }
    for (size_t i = 0; i < _numFrames; ++i)
    {
        setToFrame(i);
        for (auto it = _channels.begin(); it != _channels.end(); ++it)
        {
            MotionSequenceFrame frame(_skeleton.getBone(it->second->getId())->getRelOrientation());
            if (it->second->getFrame(i).hasPositionData())
            {
                frame.setPosition(it->second->getFrame(i).getPosition());
            }
            it->second->setFrame(i, frame);
        }
    }
    _hasAbsOrientations = false;
}

void MotionSequence::createFromSkeleton(const Skeleton &skeleton)
{
    clear();
    _skeleton = skeleton;
    auto boneIdsWithName = skeleton.getBoneIdsWithName();
    for (size_t i = 0; i < boneIdsWithName.size(); ++i)
    {
        MotionSequenceChannel* channel = new MotionSequenceChannel(boneIdsWithName[i].first);
        channel->setName(boneIdsWithName[i].second);
        _channels[boneIdsWithName[i].first] = channel;
    }
}

int MotionSequence::createChannel(int parent)
{
    int id = _skeleton.createBone(parent);
    _channels[id] = new MotionSequenceChannel(id);
    return id;
}

int MotionSequence::createChannel(const MotionSequenceChannel &channelData, int parent)
{
    int id = _skeleton.createBone(parent);
    _channels[id] = new MotionSequenceChannel(id, channelData);
    return id;
}

bool MotionSequence::eraseChannel(int id, bool eraseChildren)
{
    auto it = _channels.find(id);
    if (it == _channels.end())
    {
        // channel with specified id does not exist. Return true as the channel was already erased
        return true;
    }

    MotionSequenceChannel* channel = it->second;

    Bone* bone = _skeleton.getBone(id);

    std::vector<Bone*> children = bone->getAllChildren();
    std::vector<int> childIds;
    if (eraseChildren)
    {
        for (size_t i = 0; i < children.size(); ++i)
        {
            childIds.push_back(children[i]->getId());
        }
    }

    if (_skeleton.eraseBone(id, eraseChildren))
    {
        _channels.erase(it);
        delete channel;

        for (size_t i = 0; i < childIds.size(); ++i)
        {
            it = _channels.find(childIds[i]);
            if (it != _channels.end())
            {
                channel = it->second;
                _channels.erase(it);
                delete channel;
            }
        }
    }
    else
    {
        return false;
    }
    return true;
}

void MotionSequence::clear()
{
    // delete all bones
    _skeleton.clear();
    for (auto it = _channels.begin(); it != _channels.end(); ++it)
    {
        delete it->second;
    }
    _channels.clear();
    _numFrames = 0;
}

void MotionSequence::clearFrames()
{
    for (auto it = _channels.begin(); it != _channels.end(); ++it)
    {
        it->second->clear();
    }
    _numFrames = 0;
}

std::vector<int> MotionSequence::getChannelIds() const
{
    std::vector<int> channelIds;
    for (auto it = _channels.begin(); it != _channels.end(); ++it)
    {
        channelIds.push_back(it->first);
    }
    return channelIds;
}

int MotionSequence::getRootId() const
{
    return _skeleton.getRootId();
}

MotionSequenceChannel* MotionSequence::getRoot()
{
    auto it = _channels.find(_skeleton.getRootId());
    if (it == _channels.end())
    {
        return nullptr;
    }
    return it->second;
}

MotionSequenceChannel* MotionSequence::getChannel(int id)
{
    auto it = _channels.find(id);
    if (it != _channels.end())
    {
        return it->second;
    }
    return nullptr;
}

void MotionSequence::setToFrame(unsigned int frame)
{
    for (auto it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (_hasAbsOrientations && frame < it->second->getNumFrames())
        {
            _skeleton.getBone(it->first)->setAbsOrientation(it->second->getFrame(frame).getOrientation());
        }
        else
        {
            _skeleton.getBone(it->first)->setRelOrientation(it->second->getFrame(frame).getOrientation());
        }
        if (it->second->getFrame(frame).hasPositionData())
        {
            _skeleton.getBone(it->first)->setStartPos(it->second->getFrame(frame).getPosition());
        }
    }
    _skeleton.update();
}

MotionSequence& MotionSequence::operator=(MotionSequence other)
{
    _name = other._name;
    _skeleton = other._skeleton;
    _numFrames = other._numFrames;
    _frameTime = other._frameTime;
    _hasAbsOrientations = other._hasAbsOrientations;
    std::swap(_channels, other._channels);
    return *this;
}


std::istream& MotionSequence::read(std::istream& s)
{
    clear();

    std::getline(s, _name);

    s >> _numFrames;
    s >> _frameTime;
    s >> _hasAbsOrientations;

    size_t numChannels;
    s >> numChannels;

    s.ignore(100, '\n');

    s >> _skeleton;

    MotionSequenceChannel channel;
    for (size_t i = 0; i < numChannels; ++i)
    {
        s.ignore(100, '\n');
        s >> channel;
        _channels[channel.getId()] = new MotionSequenceChannel(channel);
    }
    return s;
}

std::ostream& MotionSequence::write(std::ostream& s) const
{
    s << _name << std::endl << _numFrames << ' ' << _frameTime << ' ' << _hasAbsOrientations << ' ' << _channels.size() << std::endl;
    s << _skeleton;

    for (auto it = _channels.begin(); it != _channels.end(); ++it)
    {
        s << std::endl;
        it->second->write(s);
    }
    return s;
}

std::istream& MotionSequence::readBinary(std::istream& s)
{
    clear();

    size_t strLength;
    s.read((char*)&strLength, sizeof(size_t));
    _name.resize(strLength);
    s.read((char*)&_name[0], strLength);

    s.read((char*)&_numFrames, sizeof(_numFrames));
    s.read((char*)&_frameTime, sizeof(_frameTime));
    s.read((char*)&_hasAbsOrientations, sizeof(_hasAbsOrientations));

    size_t numChannels;
    s.read((char*)&numChannels, sizeof(size_t));
    _skeleton.readBinary(s);

    MotionSequenceChannel channel;
    for (size_t i = 0; i < numChannels; ++i)
    {
        channel.readBinary(s);
        _channels[channel.getId()] = new MotionSequenceChannel(channel);
    }
    return s;
}

std::ostream& MotionSequence::writeBinary(std::ostream& s) const
{
    size_t size = _name.size();
    s.write((char*)&size, sizeof(size_t));
    s.write((char*)&_name[0], _name.size());
    s.write((char*)&_numFrames, sizeof(_numFrames));
    s.write((char*)&_frameTime, sizeof(_frameTime));
    s.write((char*)&_hasAbsOrientations, sizeof(_hasAbsOrientations));

    size = _channels.size();
    s.write((char*)&size, sizeof(size_t));

    // write skeleton
    _skeleton.writeBinary(s);

    // write channels
    for (auto it = _channels.begin(); it != _channels.end(); ++it)
    {
        it->second->writeBinary(s);
    }
    return s;
}

std::ostream& operator<<(std::ostream& out, const MotionSequence& sequence)
{
    return sequence.write(out);
}

std::istream& operator>>(std::istream& in, MotionSequence& sequence)
{
    return sequence.read(in);
}

