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


#include "MotionFilterBase.h"

MotionFilterBase::MotionFilterBase()
{
    _skeleton = nullptr;
    _frameTime = 0.01f;
    _recording = false;
}

MotionFilterBase::~MotionFilterBase()
{
    //dtor
}

void MotionFilterBase::setSensors(std::vector<SensorNode*> sensors)
{
    _sensors = sensors;
}

void MotionFilterBase::setSkeleton(Skeleton* skeleton)
{
    _skeleton = skeleton;
    _sequence.createFromSkeleton(*skeleton);
}

void MotionFilterBase::setFrameTime(float frameTime)
{
    _frameTime = frameTime;
}

float MotionFilterBase::getFrameTime() const
{
    return _frameTime;
}

void MotionFilterBase::setRecording(bool recording)
{
    if (!_recording && recording)
    {
        onStartRecording();
    }
    if (_recording && !recording)
    {
        onStopRecording();
    }
    _recording = recording;
}

bool MotionFilterBase::isRecording() const
{
    return _recording;
}

MotionSequence MotionFilterBase::getSequence()
{
    return _sequence;
}


