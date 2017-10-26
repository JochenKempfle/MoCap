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


#ifndef MOTIONFILTERBASE_H
#define MOTIONFILTERBASE_H

#include <string>
#include "Skeleton.h"
#include "MotionSequence.h"
#include "SensorNode.h"
#include "SensorBuffer.h"
#include <wx/thread.h>

class MotionFilterBase : public wxThreadHelper
{
  public:
    MotionFilterBase();
    virtual ~MotionFilterBase();

    virtual std::string getName() const = 0;
    virtual void update() = 0;

    void setSensors(std::vector<SensorNode*> sensors);
    void setSkeleton(Skeleton* skeleton);
    void setFrameTime(float frameTime);
    float getFrameTime() const;
    void setStartTime(uint64_t startTime) { _startTime = startTime; }
    uint64_t getStartTime() const { return _startTime; }

    void start();
    void stop();
    bool isRunning() const { return _running; }

    void setRecording(bool recording = true);
    bool isRecording() const;
    MotionSequence getSequence();


  protected:
    virtual void onStartRecording() = 0;
    virtual void onStopRecording() = 0;

    virtual wxThread::ExitCode Entry();

    std::vector<SensorBuffer*> _buffers;
    Skeleton* _skeleton;
    MotionSequence _sequence;
    float _frameTime;
    uint64_t _startTime;
    bool _running;
    bool _recording;

  private:
};

#endif // MOTIONFILTERBASE_H
