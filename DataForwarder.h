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


#ifndef DATAFORWARDER_H
#define DATAFORWARDER_H

#include "DataChannel.h"


template <class OUTPUT, class... INPUTS>
class DataForwarder
{
  public:
    DataForwarder();
    virtual ~DataForwarder();

    // template <class OUTPUT, class... INPUTS>
    int appendChannel();

    virtual std::vector<SensorDataType> getInputDataTypes() const { return DataChannelTyped<OUTPUT, INPUTS...>::getInputDataTypesStatic(); }
    virtual SensorDataType getOutputDataType() const { return OUTPUT::getType(); }

    size_t getNumChannels() { return _channels.size(); }

    // input data type 1, channel 1
    // input data type 2, channel 1
    // -> output channel 1

    // input data type 1, channel 2
    // input data type 2, channel 2
    // output channel 2

  protected:
    std::vector<DataChannelTyped<OUTPUT, INPUTS...>*> _channels;

  private:
    // std::map<int, SensorBufferChannel> _channels;

    // std::map<SensorBufferChannel, SensorBuffer*> _inputChannels;
    // std::map<SensorBufferChannel, std::vector<SensorBuffer*> > _outputChannels;

};


template <class OUTPUT, class... INPUTS>
int DataForwarder<OUTPUT, INPUTS...>::appendChannel()
{
    int id = _channels.size();
    _channels.push_back(new DataChannelTyped<OUTPUT, INPUTS...>());
    return id;
}



#endif // DATAFORWARDER_H
