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


#include "SensorNode.h"

SensorNode::SensorNode(int id, std::string IPAddress) : _id(id), _IPAddress(IPAddress)
{
    _state = 0;
    //ctor
}

SensorNode::SensorNode() : _id(-1)
{
    _state = 0;
}

SensorNode::~SensorNode()
{
    //dtor
}

void SensorNode::update(const SensorData &data)
{
    // TODO(JK#2#): implement some kind of filtering here!
    _rotation.u() = data.rotation[0];
    _rotation.x() = data.rotation[1];
    _rotation.y() = data.rotation[2];
    _rotation.z() = data.rotation[3];

    // TODO(JK#1#): update position of sensor node
/*    _position.x() = data.position[0];
    _position.y() = data.position[1];
    _position.z() = data.position[2];
*/
    // TODO(JK#9#): implement some logic for a state update! (done?)
}

Quaternion SensorNode::getCalRotation() const
{
    // Quaternion x(1.0, 0.0, 0.0, M_PI*90.0/180.0);
    return _rotationOffset * _rotation;
}

Vector3 SensorNode::toEuler() const
{
    return _rotation.toEuler();
}


