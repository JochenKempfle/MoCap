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
#include <vector>
#include <map>

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

    // set the frame time of a track. Use Timeline::changeFrameTime to update the overlays!
    void setFrameTime(float frameTime);
    float getFrameTime() const;

    void clear();
    // void split();

    void setFrames(const std::vector<MotionSequenceFrame> &frames);
    void appendFrames(const std::vector<MotionSequenceFrame> &frames);
    void appendFrame(const MotionSequenceFrame &frame);

    size_t getNumFrames() const;

    bool cut(uint64_t time, TimelineTrack* newTrack = nullptr);

    uint64_t getAbsTimeFromFrame(unsigned int frame) const;
    uint64_t getRelTimeFromFrame(unsigned int frame) const;

    unsigned int getFrameNumFromAbsTime(uint64_t time) const;
    unsigned int getFrameNumFromRelTime(uint64_t time) const;

    MotionSequenceFrame getFrameFromAbsTime(uint64_t time, bool weighted = false) const;
    MotionSequenceFrame getFrameFromRelTime(uint64_t time, bool weighted = false) const;
    MotionSequenceFrame getFrame(unsigned int pos, bool weighted = false) const;

    void setFrame(unsigned int pos, const MotionSequenceFrame &frame);

    MotionSequenceFrame getFirstFrame(bool weighted = false) const;
    MotionSequenceFrame getLastFrame(bool weighted = false) const;

    bool isInside(uint64_t time) const;

    void setWeightPoint(unsigned int frame, float weight);
    void moveWeightPoint(unsigned int oldFrame, unsigned int newFrame, float weight);
    void removeWeightPoint(unsigned int frame);

    std::vector<std::pair<unsigned int, float> > getWeightPoints() const;
    float getWeight(unsigned int frame) const;

    std::istream& read(std::istream &s);
    std::ostream& write(std::ostream &s) const;
    std::istream& readBinary(std::istream &s);
    std::ostream& writeBinary(std::ostream &s) const;

  protected:
  private:
    int _id;
    int _channel;
    std::string _name;
    uint64_t _startTime;
    float _frameTime;
    std::vector<MotionSequenceFrame> _frames;
    std::map<unsigned int, float> _weightPoints;
};

std::ostream& operator<<(std::ostream& out, const TimelineTrack& track);

std::istream& operator>>(std::istream& in, TimelineTrack& track);

#endif // TIMELINETRACK_H
