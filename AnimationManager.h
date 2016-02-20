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


#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H

#include "Skeleton.h"
#include "MotionSequence.h"
#include "Timeline.h"
#include <map>
#include <vector>
#include <wx/string.h>

#define theAnimationManager AnimationManager::getInstance()

class AnimationManager
{
  public:
    AnimationManager();
    virtual ~AnimationManager();

    static AnimationManager& getInstance();

    Skeleton* getSequenceSkeleton(int sequenceId);
    void setTimelineSkeleton(Skeleton* skeleton) { _timeline.setSkeleton(skeleton); }
    Skeleton* getTimelineSkeleton();

    Timeline* getTimeline() { return &_timeline; }

    int addProjectSequence(MotionSequence* sequence);
    MotionSequence* getProjectSequence(int pos);
    const std::vector<MotionSequence*>& getProjectSequences();

    void addSequenceToTimeline(int sequenceId, unsigned int targetChannel, unsigned int time = 0);
    void addChannelsToTimeline(int sequenceId, std::vector<int> channels, unsigned int targetChannel, unsigned int time = 0);
    void addTrackToTimeline(const TimelineTrack &track, unsigned int targetChannel, unsigned int time = 0);

    // TODO(JK#2#): do not return a pointer, a copy constructor exists and a skeleton pointer is likely to get lost
    Skeleton* readSkeletonFromBVH(wxString filename);
    MotionSequence* readBVH(wxString filename);

  protected:
    static AnimationManager* _animationManager;
  private:
    Skeleton _previewSkeleton;
    MotionSequence _sequence;
    std::vector<MotionSequence*> _projectSequences;
    std::map<int, int> _channelIdFromBoneId;
    std::map<int, int> _boneIdFromChannelId;
    Timeline _timeline;
};

#endif // ANIMATIONMANAGER_H
