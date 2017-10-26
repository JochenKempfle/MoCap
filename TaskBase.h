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


#ifndef TASKBASE_H
#define TASKBASE_H


#include <string>
#include "DataProvider.h"
#include "SensorData.h"
#include <wx/thread.h>


class TaskBase : public wxThreadHelper, public DataProvider
{
  public:
    TaskBase();
    virtual ~TaskBase();

    virtual std::string getName() const = 0;
    virtual std::string getInfo() const = 0;

    virtual bool setup() = 0;
    virtual bool update() = 0;

    // onStart is called before the thread is run
    virtual void onStart() = 0;
    // onStop is called after the thread is stopped (within the thread but shortly before exiting it)
    virtual void onStop() = 0;

    virtual SensorDataType getInputDataType() const = 0;

    //bool acceptsDataType(SensorDataType type) const = 0;

    void start(bool recording = false);
    void stop();

    bool isRunning() const { return _running; }
    bool isRecording() const { return _recording; }

    void subscribe(DataProvider* provider, int channel);

  protected:
    virtual wxThread::ExitCode Entry();

    // float _frameTime;
    // uint64_t _startTime;
    volatile bool _running;
    bool _recording;

    std::vector<SensorBuffer*> _buffers;

  private:
};

// task can have variable or fixed number of channels (recorder one for each bone)
// each task has the same kind of channels
// channel has fixed num of inputs and one output

typedef TaskBase* (*CreateTaskFunc)();

class TaskFactory
{
  public:
    static size_t getNumTasks();
    static std::string getTaskName(unsigned int type);

    static TaskBase* createTask(unsigned int type);

    static const int registerTask(std::string name, CreateTaskFunc func);

  protected:
    TaskFactory();

    static TaskFactory* getInstance();
    static TaskFactory* _instance;

    std::vector<std::pair<std::string, CreateTaskFunc> > _taskList;
};

// maybe use instead of the #define mixin the curiously recurring template pattern (with abstract base class)
#define DECLARE_TASK(TASK_CLASS) \
  public: \
    static TaskBase* createSelf() { return new TASK_CLASS(); } \
    static const int _taskTypeID;

#define REGISTER_TASK(TASK_NAME, TASK_CLASS) \
    const int TASK_CLASS::_taskTypeID = TaskFactory::registerTask(TASK_NAME, TASK_CLASS::createSelf);


#endif // TASKBASE_H
