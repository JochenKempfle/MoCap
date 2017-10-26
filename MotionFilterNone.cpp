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
    return "No Filter";
}

// TODO(JK#2#2017-07-03): MotionFilterNone's update routine is outdated -> update
void MotionFilterNone::update()
{
    if (_skeleton == nullptr)
    {
        return;
    }
    for (size_t i = 0; i < _buffers.size(); ++i)
    {
        SensorBuffer* buffer = _buffers[i];
        SensorNode* sensor = dynamic_cast<SensorNode*>(buffer->getDataProvider());
        if (sensor == nullptr)
        {
            continue;
        }
        int boneId = sensor->getBoneId();

        size_t bufferSize = buffer->sizeFront();
        if (boneId < 0 || bufferSize == 0)
        {
            continue;
        }
        // update the skeleton with the calibrated absolute orientation measured by the sensors
        _skeleton->setAbsBoneOrientation(boneId, sensor->getCalRotation());//buffer->back().getOrientation());
        if (!_recording && bufferSize > 5)
        {
            buffer->clear();
            continue;
        }
        for (size_t i = 0; i < bufferSize; ++i)
        {
            SensorDataOrientation data;
            try
            {
                data = *dynamic_cast<const SensorDataOrientation*>(&buffer->front());
            }
            catch (const std::bad_cast &e)
            {
                // Cast failed -> no orientation data present
                wxLogDebug(e.what());
            }
            MotionSequenceFrame frame(data.getOrientation());
            _sequence.getChannel(boneId)->appendFrame(frame);
            buffer->pop_front();
            /*
            SensorDataOrientation* data = dynamic_cast<SensorDataOrientation*>(&buffer->front());
            MotionSequenceFrame frame(data.getOrientation());
            _sequence.getChannel(boneId)->appendFrame(frame);
            buffer->pop_front();
            */
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
    _sequence.setFrameTime(_frameTime);
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


