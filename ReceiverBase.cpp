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


#include "ReceiverBase.h"

ReceiverBase::ReceiverBase() : _running(false), _updated(false), _threaded(false)
{
    //ctor
}

ReceiverBase::~ReceiverBase()
{
    //dtor
}

void ReceiverBase::setThreaded(bool threaded)
{
    if (_running)
    {
        return;
    }
    _threaded = threaded;
}

bool ReceiverBase::update()
{
    if (!_running)
    {
        return false;
    }
    if (!_threaded)
    {
        return onUpdate();
    }
    if (GetThread() != nullptr && GetThread()->IsRunning())
    {
        return _updated;
    }
    return false;
}

bool ReceiverBase::connect()
{
    if (GetThread() && GetThread()->IsRunning())
    {
        return false;
    }
    if (!GetThread())
    {
        if (CreateThread(wxTHREAD_DETACHED) != wxTHREAD_NO_ERROR)
        {
            wxMessageBox(_("Could not create the receiver thread!"), _("Error"), wxICON_ERROR);
            return false;
        }
    }
    // call onConnect
    if (isConnected() || !onConnect())
    {
        return false;
    }

    if (!_running)
    {
        _running = true;
        if (GetThread()->Run() != wxTHREAD_NO_ERROR)
        {
            wxMessageBox(_("Could not run the receiver thread!"), _("Error"), wxICON_ERROR);
            _running = false;
            return false;
        }
    }
    return true;
}

void ReceiverBase::disconnect()
{
    _running = false;
    if (!_threaded)
    {
        onDisconnect();
    }
}

wxThread::ExitCode ReceiverBase::Entry()
{
    // here we do our long task, periodically calling TestDestroy():
    while (_running && !GetThread()->TestDestroy())
    {
        _updated = onUpdate();
        // give other tasks some time
        wxMilliSleep(1);
    }
    // call onDisconnect when the thread exited its main loop
    onDisconnect();

    // to be sure: reset _running
    _running = false;
    _updated = false;

    // TestDestroy() returned true (which means the main thread asked us
    // to terminate as soon as possible) or we ended the long task...
    return (wxThread::ExitCode)0;
}




ReceiverFactory* ReceiverFactory::_instance = nullptr;

ReceiverFactory::ReceiverFactory()
{

}

size_t ReceiverFactory::getNumReceivers()
{
    return getInstance()->_receiverList.size();
}

std::string ReceiverFactory::getReceiverName(unsigned int type)
{
    return getInstance()->_receiverList[type].first;
}

ReceiverBase* ReceiverFactory::createReceiver(unsigned int type)
{
    if (type > getInstance()->getNumReceivers())
    {
        return nullptr;
    }
    return getInstance()->_receiverList[type].second();
}

const int ReceiverFactory::registerReceiver(std::string name, CreateRecvFunc func)
{
    getInstance()->_receiverList.push_back(std::make_pair(name, func));
    return getInstance()->_receiverList.size();
}

ReceiverFactory* ReceiverFactory::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = new ReceiverFactory();
    }
    return _instance;
}

