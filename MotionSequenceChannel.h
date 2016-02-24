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


#ifndef MOTIONSEQUENCECHANNEL_H
#define MOTIONSEQUENCECHANNEL_H

#include <vector>
#include <string>
#include "Quaternion.h"
#include "Vector3.h"
#include "MotionSequenceFrame.h"

class MotionSequenceChannel
{
  public:
    MotionSequenceChannel();
    MotionSequenceChannel(int id);
    MotionSequenceChannel(const MotionSequenceChannel &other);
    MotionSequenceChannel(int id, const MotionSequenceChannel &other);

    virtual ~MotionSequenceChannel();

    void setId(int id) { _id = id; }
    int getId() const { return _id; }

    void setName(std::string name);
    std::string getName() const;

    void setFrame(size_t frameNumber, const MotionSequenceFrame &frame);
    void appendFrame(const MotionSequenceFrame &frame);

    MotionSequenceFrame getFrame(size_t frameNumber) const;
    const std::vector<MotionSequenceFrame>& getFrames() const { return _frames; }
    size_t getNumFrames() const;

    void setFrameTime(float time) { _frameTime = time; }
    float getFrameTime() const { return _frameTime; }

    void clear();

    void setBoneLength(float length);
    float getBoneLength() const;

    void setInitialPose(MotionSequenceFrame pose);
    MotionSequenceFrame getInitialPose() const;

    std::vector<MotionSequenceFrame>::iterator beginFrames() { return _frames.begin(); }
    std::vector<MotionSequenceFrame>::iterator endFrames() { return _frames.end(); }

  protected:

  private:
    int _id;
    std::string _name;
    float _frameTime;
    std::vector<MotionSequenceFrame> _frames;
    // TODO(JK#5#): remove initial pose and bone length from MotionSequenceChannel
    MotionSequenceFrame _initialPose;
    float _boneLength;
};

#endif // MOTIONSEQUENCECHANNEL_H
