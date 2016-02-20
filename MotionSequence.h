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


#ifndef MOTIONSEQUENCE_H
#define MOTIONSEQUENCE_H

#include <map>
#include <string>
#include "MotionSequenceChannel.h"
#include "Skeleton.h"

class MotionSequence
{
  public:
    MotionSequence();
    virtual ~MotionSequence();

    void setName(std::string name);
    std::string getName() const;

    int getLength() const { return 1000 * _numFrames * _frameTime; }

    void setNumFrames(unsigned int numFrames) { _numFrames = numFrames; }
    unsigned int getNumFrames() const { return _numFrames; }

    void setFrameTime(float time);
    float getFrameTime() const { return _frameTime; }

    size_t getNumChannels() const { return _channels.size(); }
    int createChannel(int parent = -1);
    int createChannel(const MotionSequenceChannel &channelData, int parent = -1);
    bool eraseChannel(int id, bool eraseChildren = false);
    void clear();

    std::vector<int> getChannelIds() const;

    int getRootId() const;
    MotionSequenceChannel* getRoot();
    MotionSequenceChannel* getChannel(int id);

    void setToFrame(unsigned int frame);

    Skeleton* getSkeleton() { return &_skeleton; }

    bool readBVH(std::string path);

    std::map<int, MotionSequenceChannel*>::iterator beginChannels() { return _channels.begin(); }
    std::map<int, MotionSequenceChannel*>::iterator endChannels() { return _channels.end(); }
  protected:
  private:
    std::string _name;
    Skeleton _skeleton;
    unsigned int _numFrames;
    float _frameTime;
    std::map<int, MotionSequenceChannel*> _channels;
};

#endif // MOTIONSEQUENCE_H
