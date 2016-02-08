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

int MotionSequence::createChannel(int parent)
{
    int id = _skeleton.createBone(parent);
    MotionSequenceChannel* channel = new MotionSequenceChannel(id);
    _channels[id] = channel;
    return id;
}

int MotionSequence::createChannel(const MotionSequenceChannel &channelData, int parent)
{
    int id = _skeleton.createBone(parent);
    MotionSequenceChannel* channel = new MotionSequenceChannel(id, channelData);
    _channels[id] = channel;
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
        _skeleton.getBone(it->first)->setRelOrientation(it->second->getFrame(frame).getOrientation());
    }
    _skeleton.update();
}




