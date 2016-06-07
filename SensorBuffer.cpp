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


#include "SensorBuffer.h"
#include "SensorNode.h"

SensorBuffer::SensorBuffer()
{
    _sensor = nullptr;
    _clearFlag = false;
}

SensorBuffer::SensorBuffer(SensorNode* sensor)
{
    _sensor = nullptr;
    subscribe(sensor);
    _clearFlag = false;
}

SensorBuffer::~SensorBuffer()
{
    unsubscribe();
}

void SensorBuffer::subscribe(SensorNode* sensor)
{
    if (_sensor == sensor)
    {
        return;
    }
    unsubscribe();
    if (sensor == nullptr)
    {
        return;
    }
    _sensor = sensor;
    sensor->addBuffer(this);
}

void SensorBuffer::unsubscribe()
{
    if (_sensor == nullptr)
    {
        return;
    }
    _sensor->removeBuffer(this);
    _sensor = nullptr;
    _bufferBack.clear();
    _bufferFront.clear();
}

void SensorBuffer::push_back(const SensorData& data)
{
    _bufferBack.push_back(data);
    // ensure the data of the back buffer is forwarded to the front buffer at any time
    if (_bufferBack.size() > 10)
    {
        _mtx.lock();
        _bufferFront.splice(_bufferFront.end(), _bufferBack);
        _mtx.unlock();
    }
    else if (_mtx.try_lock())
    {
        _bufferFront.splice(_bufferFront.end(), _bufferBack);
        _mtx.unlock();
    }
}

void SensorBuffer::push_front(const SensorData& data)
{
    _bufferFront.push_front(data);
}

void SensorBuffer::pop_back()
{
    _bufferBack.pop_back();
}

void SensorBuffer::pop_front()
{
    _bufferFront.pop_front();
}

const SensorData& SensorBuffer::back() const
{
    return _bufferBack.back();
}

SensorData& SensorBuffer::back()
{
    if (_clearFlag)
    {
        SensorData data = _bufferBack.back();
        _bufferBack.clear();
        _bufferBack.push_back(data);
    }
    return _bufferBack.back();
}

size_t SensorBuffer::sizeBack()
{
    if (_clearFlag)
    {
        _bufferBack.clear();
    }
    return _bufferBack.size();
}


void SensorBuffer::clear()
{
    _bufferFront.clear();
/*    if (_mtxBack.try_lock())
    {
        msg.Clear();
        msg << _("clear back locked");
        wxLogDebug(msg);
        _bufferBack.clear();
        _mtxBack.unlock();
        msg.Clear();
        msg << _("clear back unlocked");
        wxLogDebug(msg);
    }
    else
    {
        _clearFlag = true;
    }*/
}

