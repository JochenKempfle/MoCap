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


#include "MotionFilterNone.h"
#include <list>

MotionFilterNone::MotionFilterNone() : MotionFilterBase()
{
    //ctor
}

MotionFilterNone::MotionFilterNone(Skeleton* skeleton) : MotionFilterBase()
{
    setSkeleton(skeleton);
}

MotionFilterNone::~MotionFilterNone()
{
    //dtor
}

std::string MotionFilterNone::getName() const
{
    return "MotionFilterNone";
}

void MotionFilterNone::update()
{
    if (_skeleton == nullptr)
    {
        return;
    }
    for (size_t i = 0; i < _buffers.size(); ++i)
    {
        SensorBuffer* buffer = _buffers[i];
        SensorNode* sensor = buffer->getSensor();
        int boneId = sensor->getBoneId();

        size_t bufferSize = buffer->size();
        if (boneId < 0 || bufferSize == 0)
        {
            continue;
        }
        // TODO(JK#1#): updating the skeleton needs abs orientation(sensors measure abs values, but the MotionSequence needs rel values
        _skeleton->setAbsBoneOrientation(boneId, sensor->getCalRotation());//buffer->back().getOrientation());
        if (!_recording)
        {
            buffer->clear();
            continue;
        }
        for (size_t i = 0; i < bufferSize; ++i)
        {
            MotionSequenceFrame frame(buffer->front().getOrientation());
            _sequence.getChannel(boneId)->appendFrame(frame);
            buffer->pop_front();
            //_skeleton->setAbsBoneRotation(_sensors[i]->getBoneId(), buffer->front().second.getOrientation());
        }
        if (_sequence.getNumFrames() < _sequence.getChannel(boneId)->getNumFrames())
        {
            _sequence.setNumFrames(_sequence.getChannel(boneId)->getNumFrames());
        }
    }
    _skeleton->update();
}

void MotionFilterNone::onStartRecording()
{
    _sequence.clearFrames();
    _sequence.setFrameTime(0.01f);
    _sequence.setHasAbsOrientations(true);
    for (size_t i = 0; i < _buffers.size(); ++i)
    {
        _buffers[i]->clear();
    }
}

void MotionFilterNone::onStopRecording()
{
    _sequence.convertToRelOrientations();
}


