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

class DataProvider;

class SensorBufferChannel
{
  public:
    SensorBufferChannel();
    SensorBufferChannel(int sourceId, int targetId);

    inline int getSourceId() const { return _sourceId; }
    inline int getTargetId() const { return _targetId; }

    // SensorBufferChannel& operator+=(const SensorBufferChannel &other);
    // SensorBufferChannel operator+(const SensorBufferChannel &other);

    bool operator==(const SensorBufferChannel &other) const;
    bool operator<(const SensorBufferChannel &other) const;
    bool operator<=(const SensorBufferChannel &other) const;
    bool operator>(const SensorBufferChannel &other) const;
    bool operator>=(const SensorBufferChannel &other) const;

    bool hasSameSource(const SensorBufferChannel &other) const;
    bool hasSameTarget(const SensorBufferChannel &other) const;

  protected:
  private:
    // source Id (typically the sensor ID)
    int _sourceId;
    // target Id (typically the bone ID)
    int _targetId;
};


class SensorBuffer
{
  public:
    SensorBuffer();
    virtual ~SensorBuffer();

    void subscribe(DataProvider* provider, int channel);
    void unsubscribe();
    bool isSubscribed() const { return _provider == nullptr; }

    void lock() { _mtx.lock(); }
    void unlock() { _mtx.unlock(); }

    DataProvider* getDataProvider() const { return _provider; }

    int getChannel() const { return _channel; }

    virtual void push_front(SensorData* data) = 0;
    virtual void pop_front() = 0;

    virtual const SensorData& front() = 0;

    virtual size_t sizeFront() const = 0;
    virtual size_t sizeBack() = 0;
    virtual size_t sizeTotal() = 0;
    virtual void clear() = 0;

    virtual SensorDataType getDataType() const = 0;

  protected:
    friend class DataProvider;

    virtual void push_back(SensorData* data) = 0;
    virtual void pop_back() = 0;

    virtual SensorData& back() = 0;
    virtual const SensorData& back() const = 0;

    DataProvider* _provider;
    std::mutex _mtx;
    std::mutex _mtxClear;

  private:
    int _channel;
};


template <typename T>
class SensorBufferType : public virtual SensorBuffer
{
  public:
    SensorBufferType();
    SensorBufferType(DataProvider* provider, int channel);
    virtual ~SensorBufferType();

    void push_front(SensorData* data);

    void pop_front();

    // TODO(JK#1#2017-10-19): lock buffer within front or outside or use a function accessFront(SensorData* data)?
    const T& front();

    size_t sizeFront() const { return _bufferFront.size(); }
    size_t sizeBack();
    size_t sizeTotal();
    virtual void clear() override;

    SensorDataType getDataType() const { return T::getType(); }

    static SensorBuffer* createSelf() { return new SensorBufferType<T>(); }

  protected:
    void push_back(SensorData* data);
    void pop_back();

    T& back();
    const T& back() const;

  private:
    std::list<T> _bufferFront;
    std::list<T> _bufferBack;
};


// TODO(JK#1#2017-09-19): check mutex correctness in SensorBuffers
// template function implementations

template <typename T>
SensorBufferType<T>::SensorBufferType() : SensorBuffer()
{

}

template <typename T>
SensorBufferType<T>::SensorBufferType(DataProvider* provider, int channel)
{
    subscribe(provider, channel);
}

template <typename T>
SensorBufferType<T>::~SensorBufferType()
{
    unsubscribe();
}

template <typename T>
void SensorBufferType<T>::push_back(SensorData* data)
{
    // only push_back data when not clearing
    if (_mtxClear.try_lock())
    {
        // Implicitly cast sensor data to the supported type.
        T typedData(data);
        _bufferBack.push_back(typedData);
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
        _mtxClear.unlock();
    }
}

template <typename T>
void SensorBufferType<T>::push_front(SensorData* data)
{
    // cast sensor data to the supported type. If not supported fall through
    T* typedData = dynamic_cast<T*>(data);
    if (typedData == nullptr)
    {
        return;
    }
    _bufferBack.push_front(*typedData);
}

template <typename T>
void SensorBufferType<T>::pop_back()
{
    _bufferBack.pop_back();
}

template <typename T>
void SensorBufferType<T>::pop_front()
{
    _mtx.lock();
    _bufferFront.pop_front();
    _mtx.unlock();
}

template <typename T>
const T& SensorBufferType<T>::front()
{
    //_mtx.lock();
    T& front = _bufferFront.front();
    //_mtx.unlock();
    return front;
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
    _mtxClear.lock();
    _mtx.lock();
    size_t size = _bufferBack.size();
    _mtx.unlock();
    _mtxClear.unlock();
    return size;
}

template <typename T>
size_t SensorBufferType<T>::sizeTotal()
{
    _mtxClear.lock();
    _mtx.lock();
    size_t size = _bufferFront.size() + _bufferBack.size();
    _mtx.unlock();
    _mtxClear.unlock();
    return size;
}

template <typename T>
void SensorBufferType<T>::clear()
{
    _mtxClear.lock();
    _mtx.lock();
    _bufferFront.clear();
    _bufferBack.clear();
    _mtx.unlock();
    _mtxClear.unlock();
}


#endif // SENSORBUFFER_H
