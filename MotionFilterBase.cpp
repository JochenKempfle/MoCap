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
#include <wx/msgdlg.h>

MotionFilterBase::MotionFilterBase()
{
    _skeleton = nullptr;
    _frameTime = 0.01f;
    _startTime = 0;
    _running = false;
    _recording = false;
}

MotionFilterBase::~MotionFilterBase()
{
    _running = false;
    _recording = false;
}

void MotionFilterBase::setSensors(std::vector<SensorNode*> sensors)
{
    for (size_t i = 0; i < _buffers.size(); ++i)
    {
        delete _buffers[i];
    }
    _buffers.clear();
    _buffers.reserve(sensors.size());
    for (size_t i = 0; i < sensors.size(); ++i)
    {
        _buffers.push_back(new SensorBuffer(sensors[i]));
    }
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

void MotionFilterBase::start()
{
    if (GetThread() && GetThread()->IsRunning())
    {
        return;
    }
    if (!GetThread())
    {
        if (CreateThread(wxTHREAD_DETACHED) != wxTHREAD_NO_ERROR)
        {
            wxMessageBox(_("Could not create the filtering thread!"), _("Error"), wxICON_ERROR);
            return;
        }
    }
    _running = true;
    if (GetThread()->Run() != wxTHREAD_NO_ERROR)
    {
        wxMessageBox(_("Could not run the filtering thread!"), _("Error"), wxICON_ERROR);
        _running = false;
        _recording = false;
        return;
    }
}

void MotionFilterBase::stop()
{
    setRecording(false);
    _running = false;
}


void MotionFilterBase::setRecording(bool recording)
{
    if (!_recording && recording)
    {
        onStartRecording();
        if (!GetThread())
        {
            if (CreateThread(wxTHREAD_DETACHED) != wxTHREAD_NO_ERROR)
            {
                wxMessageBox(_("Could not create the filtering thread!"), _("Error"), wxICON_ERROR);
                return;
            }
        }
        _running = true;
        _recording = true;
        if (!GetThread()->IsRunning() && GetThread()->Run() != wxTHREAD_NO_ERROR)
        {
            wxMessageBox(_("Could not run the filtering thread!"), _("Error"), wxICON_ERROR);
            _running = false;
            _recording = false;
            return;
        }
    }
    if (_recording && !recording)
    {
        _recording = false;
        // give the thread some time to finish recording
        wxMilliSleep(10);
        onStopRecording();
    }
}

wxThread::ExitCode MotionFilterBase::Entry()
{
    // here we do our long task, periodically calling TestDestroy():
    while (_running && !GetThread()->TestDestroy())
    {
        update();
        // give other tasks some time
        wxMilliSleep(1);
    }
    // TestDestroy() returned true (which means the main thread asked us
    // to terminate as soon as possible) or we ended the long task...
    return (wxThread::ExitCode)0;
}

bool MotionFilterBase::isRecording() const
{
    return _recording;
}

MotionSequence MotionFilterBase::getSequence()
{
    return _sequence;
}


