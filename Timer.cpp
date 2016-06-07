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


#include "Timer.h"
#include "CustomEvents.h"

Timer::Timer(wxWindow* parent)
{
    _parent = parent;
    _running = false;

    //CreateThread(wxTHREAD_JOINABLE);
}

Timer::~Timer()
{
    //dtor
    _running = false;
}

void Timer::start(unsigned int ms)
{
    if (_parent == nullptr)
    {
        return;
    }
    _ms = ms;

    if (_ms == 0)
    {
        _ms = 1;
    }

    if (!_running)
    {
        if (!GetThread())
        {
            if (CreateThread(wxTHREAD_DETACHED) != wxTHREAD_NO_ERROR)
            {
                wxLogError("Could not create the timer thread!");
                return;
            }
        }
        _running = true;
        if (GetThread()->Run() != wxTHREAD_NO_ERROR)
        {
            wxLogError("Could not run the timer thread!");
            _running = false;
            return;
        }
    }
}

void Timer::stop()
{
    _running = false;
}

wxThread::ExitCode Timer::Entry()
{
    // here we do our long task, periodically calling TestDestroy():
    while (_running && !GetThread()->TestDestroy())
    {
        wxMilliSleep(_ms);
        if (_running)
        {
            wxQueueEvent(_parent->GetEventHandler(), new wxCommandEvent(UpdateEvent)); //new wxTimerEvent());
        }
    }
    // TestDestroy() returned true (which means the main thread asked us
    // to terminate as soon as possible) or we ended the long task...
    return (wxThread::ExitCode)0;
}
