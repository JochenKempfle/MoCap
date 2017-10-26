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


#include "SensorNodeRGBD.h"
#include "MoCapManager.h"
#include <limits>

SensorNodeRGBD::SensorNodeRGBD(int id, std::string name) : SensorNode(id, name)
{

}

SensorNodeRGBD::SensorNodeRGBD() : SensorNode()
{

}

SensorNodeRGBD::~SensorNodeRGBD()
{
    //dtor
}

void SensorNodeRGBD::onUpdate(SensorData* data)
{

    SensorDataOrientation dataOrientation(data);
    _rotation = dataOrientation.getOrientation();
    /*
    wxString msg;
    msg << _("SensorNodeRGBD:   ") << dataOrientation.getOrientation().u() << _(" ") << dataOrientation.getOrientation().x() << _(" ") << dataOrientation.getOrientation().y() << _(" ") << dataOrientation.getOrientation().z() << _(" ");
    wxLogDebug(msg);
    */
    updateBuffers(data);
    /*
    for (size_t i = 0; i < _buffers.size(); ++i)
    {
        // TODO(JK#9#): what sensor data to push_back in the buffer? calRotation? - solved with received rotation
        //_buffers[i]->lock();
        _buffers[i]->push_back(data);
        //_buffers[i]->unlock();
    }
    */

    // setUpdated(true);

    // TODO(JK#5#): update position of sensor node
/*    _position.x() = data.position[0];
    _position.y() = data.position[1];
    _position.z() = data.position[2];
*/
    // TODO(JK#9#): implement some logic for a state update! (done?)
}


void SensorNodeRGBD::calibrate(int step)
{
    return;
}

void SensorNodeRGBD::applyCalibration(SensorData* data)
{
    return;
}

Quaternion SensorNodeRGBD::getCalRotation() const
{
    return _rotation;
}



