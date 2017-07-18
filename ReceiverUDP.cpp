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


#include "ReceiverUDP.h"
#include "SensorData.h"
#include "SensorManager.h"

/*
ReceiverUDP::ReceiverUDP()
{
    //ctor

    _addressPeer.BroadcastAddress();
    _addressPeer.Service(5040);
    _addressLocal.Hostname(_("192.168.0.102"));//wxGetFullHostName());
    _addressLocal.Service(5040);

    _socket = nullptr;
}
*/

ReceiverUDP::ReceiverUDP(wxString hostname, unsigned short service)
{
    _addressPeer.BroadcastAddress();
    _addressPeer.Service(service);
    _addressLocal.Hostname(hostname);//wxGetFullHostName());
    _addressLocal.Service(service);

    _socket = nullptr;
}

ReceiverUDP::~ReceiverUDP()
{
    //dtor

    if (_socket != nullptr)
    {
        _socket->Destroy();
        _socket = nullptr;
    }
}

std::string ReceiverUDP::getName() const
{
    return "ReceiverUDP";
}

bool ReceiverUDP::update()
{
    if (_socket == nullptr || !_socket->IsOk())
    {
        return false;
    }
    // get time point at which the data arrived
    wxLongLong time = wxGetUTCTimeMillis();
    uint64_t receiveTime = (uint64_t(time.GetHi()) << 32) + time.GetLo();

    SensorDataOrientation data;
    // ~68 bytes is the maximum expected packet size, so
    //alignas(float) unsigned char buffer[100];
    wxIPV4address sensorAddress = _addressPeer;

    unsigned int bytesReceived = 1;

    unsigned int id = 0;

    // take data from socket until no more bytes were received
    while (bytesReceived != 0)
    {
        _socket->RecvFrom(sensorAddress, _rcvBuffer, sizeof(_rcvBuffer));

        bytesReceived = _socket->LastCount();

        unsigned char* pBuffer = _rcvBuffer;

        if (bytesReceived >= 24 && bytesReceived <= 36)
        {
            data.setReceiveTime(receiveTime);
            //if (bytesReceived == 28 || bytesReceived == 36)
            {
                id = *(reinterpret_cast<uint32_t*>(pBuffer));
                pBuffer += sizeof(uint32_t);
            }
            //data.setType(*(reinterpret_cast<int32_t*>(pBuffer)));
            //pBuffer += sizeof(uint32_t);

            data.setTimestamp(*(reinterpret_cast<uint32_t*>(pBuffer)));
            pBuffer += sizeof(uint32_t);

            //if (data.getType() & ORIENTATION)
            {
                data.setOrientation(getQuaternion(pBuffer));
            }
            /*
            if (bytesReceived == 32 || bytesReceived == 36)
            {
                pBuffer += sizeof(float);
                data.position[0] = *reinterpret_cast<float*>(pBuffer);
                pBuffer += sizeof(float);
                data.position[1] = *reinterpret_cast<float*>(pBuffer);
                pBuffer += sizeof(float);
                data.position[2] = *reinterpret_cast<float*>(pBuffer);
            }*/
        }

        if (_socket->LastError() != wxSOCKET_NOERROR)
        {
            wxMessageBox("ERROR in Event: socket returned error");
            return false;
        }

        // handle setting of id for sensor node (and data) (solved by setting IP + id as name)
        wxString name = sensorAddress.IPAddress() + _("-");
        name << id;

        data.setType(IMU);

        std::string stdName = name.ToStdString();

        auto it = _buffer[stdName].end();
        auto mapIt = _buffer.find(stdName);

        if (bytesReceived >= 24 && bytesReceived <= 36)//== sizeof(SensorData))
        {
            while (it != mapIt->second.begin())
            {
                --it;
                if (it->getTimestamp() < data.getTimestamp())
                {
                    ++it;
                    break;
                }
            }
            mapIt->second.insert(it, data);
        }

        if (mapIt->second.size() >= this->BUFFERSIZE)
        {
            theSensorManager.updateSensor(stdName, &_buffer[stdName].front());
            _buffer[stdName].pop_front();
            // theSensorManager.updateSensor(sensorAddress.IPAddress().ToStdString(), data);
        }

        // get rotation quaternion directly from stream (uncomment to do so)
        /*
        if (bytesReceived == 24)
        {
            wxString msg;
            msg << data.rotation[0] << _(" ") << data.rotation[1] << _(" ") << data.rotation[2] << _(" ") << data.rotation[3];// << _("iteration no: ") << i << _("  - bytes received: ") << bytesReceived;
            wxLogDebug(msg);
        }
        */
    }
    return true;
}

bool ReceiverUDP::connect()
{
    // check if there is already a socket allocated
    if (_socket != nullptr)
    {
        _socket->Destroy();
        _socket = nullptr;
    }
    // Create the socket
    _socket = new wxDatagramSocket(_addressLocal, wxSOCKET_BROADCAST | wxSOCKET_NOWAIT | wxSOCKET_REUSEADDR);
    if (!_socket->IsOk())
    {
        _socket->Destroy();
        _socket = nullptr;
        return false;
    }
    return true;
}

void ReceiverUDP::disconnect()
{
    if (_socket != nullptr)
    {
        _socket->Destroy();
        _socket = nullptr;
    }
}

bool ReceiverUDP::isConnected()
{
    return _socket != nullptr;
}

void ReceiverUDP::setAddress(wxString hostname, unsigned short service)
{
    _addressPeer.BroadcastAddress();
    _addressPeer.Service(service);
    _addressLocal.Hostname(hostname);
    _addressLocal.Service(service);
}

wxString ReceiverUDP::getHostname() const
{
    return _addressLocal.Hostname();
}

unsigned short ReceiverUDP::getService() const
{
    return _addressLocal.Service();
}

Quaternion ReceiverUDP::getQuaternion(unsigned char* pBuffer) const
{
    Quaternion orientation;
    orientation.u() = *reinterpret_cast<float*>(pBuffer);
    pBuffer += sizeof(float);
    orientation.x() = *reinterpret_cast<float*>(pBuffer);
    pBuffer += sizeof(float);
    orientation.y() = *reinterpret_cast<float*>(pBuffer);
    pBuffer += sizeof(float);
    orientation.z() = *reinterpret_cast<float*>(pBuffer);
    pBuffer += sizeof(float);
    return orientation;
}

Vector3 ReceiverUDP::getVector(unsigned char* pBuffer) const
{
    Vector3 vec;
    vec.x() = *reinterpret_cast<float*>(pBuffer);
    pBuffer += sizeof(float);
    vec.y() = *reinterpret_cast<float*>(pBuffer);
    pBuffer += sizeof(float);
    vec.z() = *reinterpret_cast<float*>(pBuffer);
    pBuffer += sizeof(float);
    return vec;
}
