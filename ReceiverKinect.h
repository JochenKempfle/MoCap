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


#ifndef KINECTRECEIVER_H
#define KINECTRECEIVER_H

#include "ReceiverBase.h"
#include "Skeleton.h"
#include "KinectHack.h"
#include <Kinect.h>
#include <string>

class ReceiverKinect : public ReceiverBase
{
  public:
    ReceiverKinect();
    virtual ~ReceiverKinect();

    virtual std::string getName() const;

    virtual bool update();

    virtual bool connect();
    virtual void disconnect();
    virtual bool isConnected();
  protected:

  private:

    std::string getJointName(unsigned int id) const;
    int getJointParentId(unsigned int id) const;
    const Bone getJointDefaultBoneData(unsigned int id) const;

    void createKinectSkeleton();

    // Current Kinect
    IKinectSensor* _kinectSensor;
    ICoordinateMapper* _coordinateMapper;

    // Body reader
    IBodyFrameReader* _bodyFrameReader;

    int64_t _startTime;

    Skeleton _kinectSkeleton;
};

#endif // KINECTRECEIVER_H
