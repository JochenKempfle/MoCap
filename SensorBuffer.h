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

class SensorBuffer
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

    virtual void push_back(SensorData* data) = 0;
    virtual void push_front(SensorData* data) = 0;

    virtual void pop_back() = 0;
    virtual void pop_front() = 0;

    virtual SensorData& back() = 0;
    virtual const SensorData& back() const = 0;

    virtual SensorData& front() = 0;// { return _bufferFront.front(); }
    virtual const SensorData& front() const = 0;// { return _bufferFront.front(); }

    virtual size_t sizeFront() const = 0;// { return _bufferFront.size(); }
    virtual size_t sizeBack() = 0;
    virtual size_t sizeTotal() = 0;// { return _bufferFront.size() + sizeBack(); }
    virtual void clear() = 0;

//    std::list<SensorData>::iterator begin() { return _buffer.begin(); }
//    std::list<SensorData>::iterator end() { return _buffer.end(); }
//
//    std::list<SensorData>::const_iterator begin() const { return _buffer.begin(); }
//    std::list<SensorData>::const_iterator end() const { return _buffer.end(); }

  protected:
    SensorNode* _sensor;
    std::mutex _mtx;

  private:
};


template <typename T>
class SensorBufferType : public virtual SensorBuffer
{
  public:
    SensorBufferType();
    SensorBufferType(SensorNode* sensor);
    virtual ~SensorBufferType();

    void push_back(SensorData* data);
    void push_front(SensorData* data);

    void pop_back();
    void pop_front();

    T& back();
    const T& back() const;

    T& front() { return _bufferFront.front(); }
    const T& front() const { return _bufferFront.front(); }

    size_t sizeFront() const { return _bufferFront.size(); }
    size_t sizeBack();
    size_t sizeTotal() { return _bufferFront.size() + sizeBack(); }
    virtual void clear() override;

//    std::list<SensorData>::iterator begin() { return _buffer.begin(); }
//    std::list<SensorData>::iterator end() { return _buffer.end(); }
//
//    std::list<SensorData>::const_iterator begin() const { return _buffer.begin(); }
//    std::list<SensorData>::const_iterator end() const { return _buffer.end(); }

  protected:

  private:
    std::list<T> _bufferFront;
    std::list<T> _bufferBack;
};


// template function implementations

template <typename T>
SensorBufferType<T>::SensorBufferType() : SensorBuffer()
{

}

template <typename T>
SensorBufferType<T>::SensorBufferType(SensorNode* sensor) : SensorBuffer(sensor)
{

}

template <typename T>
SensorBufferType<T>::~SensorBufferType()
{
    unsubscribe();
}

template <typename T>
void SensorBufferType<T>::push_back(SensorData* data)
{
    _bufferBack.push_back(*dynamic_cast<T*>(data));
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

template <typename T>
void SensorBufferType<T>::push_front(SensorData* data)
{
    _bufferFront.push_front(*dynamic_cast<T*>(data));
}

template <typename T>
void SensorBufferType<T>::pop_back()
{
    _bufferBack.pop_back();
}

template <typename T>
void SensorBufferType<T>::pop_front()
{
    _bufferFront.pop_front();
}

template <typename T>
T& SensorBufferType<T>::back()
{
    return _bufferBack.back();
}

template <typename T>
const T& SensorBufferType<T>::back() const
{
    return _bufferBack.back();
}

template <typename T>
size_t SensorBufferType<T>::sizeBack()
{
    return _bufferBack.size();
}

template <typename T>
void SensorBufferType<T>::clear()
{
    _mtx.lock();
    _bufferFront.clear();
    _bufferBack.clear();
    _mtx.unlock();
}


#endif // SENSORBUFFER_H
