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


#ifndef SENSORBUFFER_H
#define SENSORBUFFER_H

#include "SensorData.h"
#include <mutex>
#include <atomic>
#include <list>

class SensorNode;

class SensorBuffer// : public std::list<SensorData>
{
  public:
    SensorBuffer();
    SensorBuffer(SensorNode* sensor);
    virtual ~SensorBuffer();

    void subscribe(SensorNode* sensor);
    void unsubscribe();

    void lock() { _mtx.lock(); }
    void unlock() { _mtx.unlock(); }

    SensorNode* getSensor() const { return _sensor; }

    void push_back(const SensorData &data);
    void push_front(const SensorData &data);

    void pop_back();
    void pop_front();

    SensorData& back();
    const SensorData& back() const;
    SensorData& front() { return _bufferFront.front(); }
    const SensorData& front() const { return _bufferFront.front(); }

    size_t sizeFront() const { return _bufferFront.size(); }
    size_t sizeBack();
    size_t sizeTotal() { return _bufferFront.size() + sizeBack(); }
    void clear();

//    std::list<SensorData>::iterator begin() { return _buffer.begin(); }
//    std::list<SensorData>::iterator end() { return _buffer.end(); }
//
//    std::list<SensorData>::const_iterator begin() const { return _buffer.begin(); }
//    std::list<SensorData>::const_iterator end() const { return _buffer.end(); }

  protected:

  private:
    SensorNode* _sensor;
    std::mutex _mtx;
    std::atomic_bool _clearFlag;
    std::list<SensorData> _bufferFront;
    std::list<SensorData> _bufferBack;
};

#endif // SENSORBUFFER_H
