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
}

SensorBuffer::SensorBuffer(SensorNode* sensor)
{
    _sensor = nullptr;
    subscribe(sensor);
}

SensorBuffer::~SensorBuffer()
{

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
    clear();
}



