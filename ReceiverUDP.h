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


#ifndef UDPRECEIVER_H
#define UDPRECEIVER_H

#include "ReceiverBase.h"
#include "Quaternion.h"
#include "Vector3.h"
#include "SensorData.h"
#include "wx/socket.h"
#include <map>
#include <list>

class ReceiverUDP : public ReceiverBase
{
    DECLARE_RECEIVER(ReceiverUDP)

  public:
    ReceiverUDP();
    ReceiverUDP(wxString hostname, unsigned short service);
    virtual ~ReceiverUDP();

    virtual std::string getName() const;
    virtual std::string getInfo() const;

    virtual bool setup();

    virtual bool isConnected() const;

    // set the address of the socket, when currently connected, this will take only effect after disconnecting and connecting again
    void setAddress(wxString hostname, unsigned short service);
    // wxString getHostname() const;
    wxString getIPAddress() const;
    unsigned short getService() const;

  protected:
    bool onUpdate();
    bool onConnect();
    void onDisconnect();

  private:
    Quaternion getQuaternion(unsigned char* pBuffer) const;
    Vector3 getVector(unsigned char* pBuffer) const;

    wxIPV4address _addressLocal;
    wxIPV4address _addressPeer;
    wxDatagramSocket* _socket;
    // the receive buffer (maximum expected data size is less than 100 bytes)
    alignas(float) unsigned char _rcvBuffer[100];

    const unsigned char BUFFERSIZE = 10;
    std::map<std::string, std::list<SensorDataOrientation> > _buffer;
};

#endif // UDPRECEIVER_H
