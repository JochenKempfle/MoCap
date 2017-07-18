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


#include "MotionFilterSlerp.h"

MotionFilterSlerp::MotionFilterSlerp() : MotionFilterBase()
{
    //ctor
}

MotionFilterSlerp::MotionFilterSlerp(Skeleton* skeleton) : MotionFilterBase()
{
    setSkeleton(skeleton);
}

MotionFilterSlerp::~MotionFilterSlerp()
{
    //dtor
}


std::string MotionFilterSlerp::getName() const
{
    return "Slerp Filter";
}

void MotionFilterSlerp::update()
{
    if (_skeleton == nullptr)
    {
        return;
    }
    if (_prevData.size() != _buffers.size())
    {
        _prevData.clear();
        _prevData.resize(_buffers.size(), SensorDataOrientation());
    }
    for (size_t i = 0; i < _buffers.size(); ++i)
    {
        SensorBuffer* buffer = _buffers[i];
        SensorNode* sensor = buffer->getSensor();
        int boneId = sensor->getBoneId();

        buffer->lock();
        size_t bufferSize = buffer->sizeFront();
        buffer->unlock();
        if (boneId < 0 || bufferSize == 0)
        {
            // a buffer with no bone affiliation is not used, so clear it to safe memory
            if (bufferSize > 5)
            {
                buffer->lock();
                buffer->clear();
                buffer->unlock();
            }
            continue;
        }
        // update the skeleton with the calibrated absolute orientation measured by the sensors

        SensorDataOrientation data;
        // apply sensor model
        buffer->lock();
        try
        {
            data = dynamic_cast<SensorDataOrientation&>(buffer->front());
        }
        catch (const std::bad_cast &e)
        {
            // Cast failed -> no orientation data present
            wxLogDebug(e.what());
        }
        buffer->unlock();

        sensor->applyCalibration(&data);

        _skeleton->setAbsBoneOrientation(boneId, data.getOrientation());//buffer->back().getOrientation());
        //_skeleton->setAbsBoneOrientation(boneId, sensor->getCalRotation());//buffer->back().getOrientation());
        if (!_recording && bufferSize > 1)
        {
            buffer->lock();
            buffer->clear();
            buffer->unlock();
            continue;
        }

        uint64_t currentTime = _startTime + uint64_t(_sequence.getChannel(boneId)->getNumFrames() * _frameTime * 1000.0f);
        uint64_t nextTime = currentTime;// + _frameTime * 1000;
        // SensorData prevData;

        while (bufferSize > 1)
        {
            --bufferSize;

            buffer->lock();
            try
            {
                data = dynamic_cast<SensorDataOrientation&>(buffer->front());
            }
            catch (const std::bad_cast &e)
            {
                // Cast failed -> no orientation data present
                wxLogDebug(e.what());
            }
            buffer->unlock();
            uint64_t dataTime = sensor->getStartTime() + uint64_t(data.getTimestamp());

            sensor->applyCalibration(&data);

            // apply sensor model
//            Quaternion q = sensor->getRotationOffset() * data.getOrientation();
//            q.normalize();
//            data.setOrientation((q * sensor->getBoneMapping()).normalize());

/*
            wxString msg;
            msg << _("data time: ") << dataTime << _("\nnext time: ") << nextTime << _("\nframes: ") << _sequence.getChannel(boneId)->getNumFrames();
            msg << _("\nsensor start time: ") << sensor->getStartTime() << _("\nrecv time: ") << data.getReceiveTime();
            msg << _("\nlast recv time: ") << sensor->getLastReceiveTime() << _("\nstart time: ") << _startTime << _("\ncurrent time: ");
            msg << wxGetUTCTimeMillis() << _("\nbuffer size: ") << buffer->size() << _("\nadd: ") << _sequence.getChannel(boneId)->getNumFrames() * _frameTime * 1000 << _("\n");
            wxLogDebug(msg);
*/
            if (dataTime <= nextTime)
            {
                _prevData[i] = data;

                buffer->lock();
                buffer->pop_front();
                buffer->unlock();
                continue;
            }
            else
            {
                double t = double(nextTime - (sensor->getStartTime() + _prevData[i].getTimestamp())) / double(data.getTimestamp() - _prevData[i].getTimestamp());
                Quaternion rotation = _prevData[i].getOrientation().slerp(data.getOrientation().normalize(), t);

                // apply sensor model
                // rotation = sensor->getRotationOffset() * rotation;
                // rotation.normalize();

                MotionSequenceFrame frame(rotation/* sensor->getBoneMapping()*/);
                _sequence.getChannel(boneId)->appendFrame(frame);

                currentTime = nextTime;
                nextTime = _startTime + uint64_t(_sequence.getChannel(boneId)->getNumFrames() * _frameTime * 1000.0f);
                // nextTime += _frameTime * 1000;
            }
            //prevData = data;
            //buffer->pop_front();
        }
        if (_sequence.getNumFrames() < _sequence.getChannel(boneId)->getNumFrames())
        {
            _sequence.setNumFrames(_sequence.getChannel(boneId)->getNumFrames());
        }
    }
    _skeleton->update();
}


void MotionFilterSlerp::onStartRecording()
{
    _sequence.clearFrames();
    _sequence.setFrameTime(_frameTime);
    _sequence.setHasAbsOrientations(true);

    _prevData.clear();
    _prevData.resize(_buffers.size(), SensorDataOrientation());
    for (size_t i = 0; i < _buffers.size(); ++i)
    {
        _buffers[i]->clear();
    }
}

void MotionFilterSlerp::onStopRecording()
{
    _sequence.convertToRelOrientations();
}
