/*Copyright (c) 2014, Jochen KempfleAll rights reserved.Redistribution and use in source and binary forms, with or without modification,are permitted provided that the following conditions are met:1. Redistributions of source code must retain the above copyright notice,this list of conditions and the following disclaimer.2. Redistributions in binary form must reproduce the above copyright notice,this list of conditions and the following disclaimer in the documentation and/orother materials provided with the distribution.THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" ANDANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIEDWARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITYOF SUCH DAMAGE.*/#include "Bone.h"Bone::Bone() : _id(-1){    _parent = nullptr;}Bone::Bone(int id) : _id(id){    _parent = nullptr;}Bone::Bone(int id, const Bone &other) : _id(id){    _length = other._length;    _orientation = other._orientation;    _startPos = other._startPos;    _endPos = other._endPos;    _parent = nullptr;}Bone::~Bone(){    //dtor}Bone& Bone::copyContent(const Bone &other){    _length = other._length;    _orientation = other._orientation;    _startPos = other._startPos;    _endPos = other._endPos;    return *this;}void Bone::setRotation(const Quaternion &rotation){    _orientation = rotation.normalized();}void Bone::update(){    if (_parent != nullptr)    {        _startPos = _parent->getEndPos();    }    Vector3 vec(1.0, 0.0, 0.0);    vec = _orientation.rotate(vec);    _endPos = vec.normalized() * _length + _startPos;    // update children    for (size_t i = 0; i < _children.size(); ++i)    {        _children[i]->update();    }}Vector3 Bone::getDirection(){    Vector3 vec(1.0, 0.0, 0.0);    vec = _orientation.rotate(vec);    return vec.normalized();}Vector3 Bone::getUpDirection(){    Vector3 vec(0.0, 1.0, 0.0);    vec = _orientation.rotate(vec);    return vec.normalized();}Vector3 Bone::getRightDirection(){    Vector3 vec(0.0, 0.0, 1.0);    vec = _orientation.rotate(vec);    return vec.normalized();}