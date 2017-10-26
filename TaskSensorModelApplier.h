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


#ifndef TASKSENSORMODELAPPLIER_H
#define TASKSENSORMODELAPPLIER_H

#include "TaskBase.h"

class TaskSensorModelApplier : public TaskBase
{
    DECLARE_TASK(TaskSensorModelApplier)

  public:
    TaskSensorModelApplier();
    virtual ~TaskSensorModelApplier();

    std::string getName() const { return "Sensor Model Applier"; }
    std::string getInfo() const { return "Applies sensor model"; }

    bool setup();
    bool update();

    // onStart is called before the thread is run
    void onStart();
    // onStop is called after the thread is stopped (within the thread but shortly before exiting it)
    void onStop();

    SensorDataType getInputDataType() const { return SensorDataType(); }
    SensorDataType getOutputDataType() const { return SensorDataType(); }

  protected:

  private:
};

#endif // TASKSENSORMODELAPPLIER_H
