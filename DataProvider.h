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


#ifndef DATAPROVIDER_H
#define DATAPROVIDER_H

#include <map>
#include <vector>
#include "SensorData.h"
#include "SensorBuffer.h"

// forward declaration of sensor buffer
// class SensorBuffer;

class DataProvider
{
  public:
    DataProvider();
    virtual ~DataProvider();

    bool hasBuffer(SensorBuffer* buffer) const;
    size_t numBuffers() const;

    void updateBuffers(SensorData* data);

    virtual SensorDataType getOutputDataType() const = 0;

    friend class SensorBuffer;

  protected:
    void addBuffer(SensorBuffer* buffer);
    void removeBuffer(SensorBuffer* buffer);

  private:
    std::vector<SensorBuffer*> _buffers;
};



#endif // DATAPROVIDER_H
