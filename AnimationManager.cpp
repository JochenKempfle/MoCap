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


#include "AnimationManager.h"
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/tokenzr.h>
#include <wx/filename.h>
#include <utility>

AnimationManager::AnimationManager()
{
    //ctor
}

AnimationManager::~AnimationManager()
{
    //dtor
}

AnimationManager* AnimationManager::_animationManager = nullptr;

AnimationManager& AnimationManager::getInstance()
{
    if (_animationManager == nullptr)
    {
        _animationManager = new AnimationManager();
    }
    return *_animationManager;
}

Skeleton* AnimationManager::getSequenceSkeleton(int sequenceId)
{
    if (sequenceId < 0 || sequenceId >= int(_projectSequences.size()))
    {
        return nullptr;
    }
    return _projectSequences[sequenceId]->getSkeleton();
}

Skeleton* AnimationManager::getTimelineSkeleton()
{
    return _timeline.getSkeleton();
}

int AnimationManager::addProjectSequence(MotionSequence* sequence)
{
    _projectSequences.push_back(sequence);
    return _projectSequences.size() - 1;
}

MotionSequence* AnimationManager::getProjectSequence(int pos)
{
    if (pos < 0 || pos >= int(_projectSequences.size()))
    {
        return nullptr;
    }
    return _projectSequences[pos];
}

const std::vector<MotionSequence*>& AnimationManager::getProjectSequences()
{
    return _projectSequences;
}

void AnimationManager::addSequenceToTimeline(int sequenceId, unsigned int targetChannel, uint64_t time)
{
    _timeline.insert(_projectSequences[sequenceId], targetChannel, time);
}

void AnimationManager::addChannelsToTimeline(int sequenceId, std::vector<int> channels, unsigned int targetChannel, uint64_t time)
{
    MotionSequence* sequence = _projectSequences[sequenceId];
    for (size_t i = 0; i < channels.size(); ++i)
    {
        MotionSequenceChannel* channel = sequence->getChannel(channels[i]);
        _timeline.insert(channel, targetChannel, time, sequence->getName() + ": " + channel->getName());
        ++targetChannel;
    }
}

void AnimationManager::addTrackToTimeline(const TimelineTrack &track, unsigned int targetChannel, uint64_t time)
{
    _timeline.insert(track, targetChannel, time);
}



