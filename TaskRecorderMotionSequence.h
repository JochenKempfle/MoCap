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


#ifndef TASKRECORDERMOTIONSEQUENCE_H
#define TASKRECORDERMOTIONSEQUENCE_H

#include "TaskBase.h"
#include "MotionSequence.h"
#include "Skeleton.h"

class TaskRecorderMotionSequence : public TaskBase
{
    DECLARE_TASK(TaskRecorderMotionSequence)

  public:
    TaskRecorderMotionSequence();
    virtual ~TaskRecorderMotionSequence();

    std::string getName() const;
    std::string getInfo() const;

    bool setup();
    bool update();

    // onStart is called before the thread is run
    void onStart();
    // onStop is called after the thread is stopped (within the thread but shortly before exiting it)
    void onStop();

    SensorDataType getInputDataType() const { return SensorDataOrientation::getType(); }
    SensorDataType getOutputDataType() const { return SensorDataOrientation::getType(); }

  protected:

  private:
    std::vector<SensorDataOrientation> _prevData;
    Skeleton* _skeleton;
    MotionSequence _sequence;
    float _frameTime;
};

#endif // TASKRECORDERMOTIONSEQUENCE_H
