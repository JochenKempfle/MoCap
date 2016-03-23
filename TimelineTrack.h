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


#ifndef TIMELINETRACK_H
#define TIMELINETRACK_H

#include "MotionSequence.h"
#include "MotionSequenceChannel.h"
#include "TimelineOverlay.h"

class TimelineTrack
{
  public:
    TimelineTrack();
    TimelineTrack(MotionSequenceChannel* channel);
    TimelineTrack(MotionSequenceChannel* channel, uint64_t startTime);
    virtual ~TimelineTrack();

    void setId(int id);
    int getId() const;

    void setChannel(int channel);
    int getChannel() const;

    void setName(std::string name);
    std::string getName() const;

    void setStartTime(uint64_t startTime);
    uint64_t getStartTime() const;
    uint64_t getEndTime() const;

    uint64_t getLength() const;
    uint64_t getMaxLength() const;

    void setFrameTime(float frameTime);
    float getFrameTime() const;

    void clear();
    // void split();

    void setFrames(const std::vector<MotionSequenceFrame> &frames);
    void appendFrames(const std::vector<MotionSequenceFrame> &frames);
    void appendFrame(const MotionSequenceFrame &frame);

    size_t getNumFrames() const;

    bool cut(uint64_t time, TimelineTrack* newTrack = nullptr);

    MotionSequenceFrame getFrameFromAbsTime(uint64_t time) const;
    MotionSequenceFrame getFrameFromRelTime(uint64_t time) const;
    MotionSequenceFrame getFrame(unsigned int pos) const;

    MotionSequenceFrame getFirstFrame() const;
    MotionSequenceFrame getLastFrame() const;

  protected:
  private:
    int _id;
    int _channel;
    std::string _name;
    uint64_t _startTime;
    float _frameTime;
    std::vector<MotionSequenceFrame> _frames;
};

#endif // TIMELINETRACK_H
