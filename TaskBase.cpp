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


#include "TaskBase.h"

TaskBase::TaskBase()
{
    //ctor
}

TaskBase::~TaskBase()
{
    //dtor
}

void TaskBase::start(bool recording)
{
    // if started again but this time with recording, fall through to call onStart again. Otherwise check if running and act accordingly.
    if (!_recording && recording)
    {
        _recording = true;
    }
    else if (GetThread() && GetThread()->IsRunning())
    {
        return;
    }
    if (!GetThread())
    {
        if (CreateThread(wxTHREAD_DETACHED) != wxTHREAD_NO_ERROR)
        {
            wxMessageBox(_("Could not create the task thread!"), _("Error"), wxICON_ERROR);
            return;
        }
    }
    // call onStart before running the thread (or when an already running thread is set to recording)
    onStart();

    if (!_running)
    {
        _running = true;
        if (GetThread()->Run() != wxTHREAD_NO_ERROR)
        {
            wxMessageBox(_("Could not run the task thread!"), _("Error"), wxICON_ERROR);
            _running = false;
            _recording = false;
            return;
        }
    }
}

void TaskBase::stop()
{
    // tell the thread to finish
    _running = false;
}

void TaskBase::subscribe(DataProvider* provider, int channel)
{
    if (provider != nullptr)
    {
        // SensorBuffer* buffer = new SensorBuffer(provider, channel);
        // _buffers.push_back(buffer);
    }
}

wxThread::ExitCode TaskBase::Entry()
{
    // here we do our long task, periodically calling TestDestroy():
    // TODO(JK#1#2017-09-29): do not use _running to stop the task but another variable. Use _running = false here to check afterwards if running
    while (_running && !GetThread()->TestDestroy())
    {
        update();
        // give other tasks some time
        wxMilliSleep(1);
    }
    // call onStop whenever the thread exits its main loop (i.e is stopped by user or system)
    onStop();
    // reset recording state
    _recording = false;
    // TestDestroy() returned true (which means the main thread asked us
    // to terminate as soon as possible) or we ended the long task...
    return (wxThread::ExitCode)0;
}


TaskFactory* TaskFactory::_instance = nullptr;

TaskFactory::TaskFactory()
{

}

size_t TaskFactory::getNumTasks()
{
    return getInstance()->_taskList.size();
}

std::string TaskFactory::getTaskName(unsigned int type)
{
    return getInstance()->_taskList[type].first;
}

TaskBase* TaskFactory::createTask(unsigned int type)
{
    if (type >= getInstance()->getNumTasks())
    {
        return nullptr;
    }
    return getInstance()->_taskList[type].second();
}

const int TaskFactory::registerTask(std::string name, CreateTaskFunc func)
{
    getInstance()->_taskList.push_back(std::make_pair(name, func));
    return getInstance()->_taskList.size();
}

TaskFactory* TaskFactory::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = new TaskFactory();
    }
    return _instance;
}

