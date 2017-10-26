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


#ifndef RECEIVERBASE_H
#define RECEIVERBASE_H

#include <string>
#include <vector>
#include <utility>
#include <wx/thread.h>


class ReceiverBase : public wxThreadHelper
{
  public:
    ReceiverBase();
    virtual ~ReceiverBase();

    virtual std::string getName() const = 0;
    virtual std::string getInfo() const = 0;

    void setThreaded(bool threaded = true);
    bool isThreaded() const { return _threaded; }

    virtual bool setup() = 0;
    bool update();

    virtual bool connect();
    virtual void disconnect();
    virtual bool isConnected() const = 0;

  protected:
    virtual wxThread::ExitCode Entry();

    virtual bool onUpdate() = 0;
    virtual bool onConnect() = 0;
    virtual void onDisconnect() = 0;

  private:
    volatile bool _running;
    volatile bool _updated;
    bool _threaded;
};


typedef ReceiverBase* (*CreateRecvFunc)();

class ReceiverFactory
{
  public:
    static size_t getNumReceivers();
    static std::string getReceiverName(unsigned int type);

    static ReceiverBase* createReceiver(unsigned int type);

    static const int registerReceiver(std::string name, CreateRecvFunc func);

  protected:
    ReceiverFactory();

    static ReceiverFactory* getInstance();
    static ReceiverFactory* _instance;

    std::vector<std::pair<std::string, CreateRecvFunc> > _receiverList;
};

// maybe use instead of the #define mixin the curiously recurring template pattern (with abstract base class)
#define DECLARE_RECEIVER(RECEIVER_CLASS) \
  public: \
    static ReceiverBase* createSelf() { return new RECEIVER_CLASS(); } \
    static const int getTypeId() { return _receiverTypeId; } \
    static const int _receiverTypeId;

#define REGISTER_RECEIVER(RECEIVER_NAME, RECEIVER_CLASS) \
    const int RECEIVER_CLASS::_receiverTypeId = ReceiverFactory::registerReceiver(RECEIVER_NAME, RECEIVER_CLASS::createSelf);

#endif // RECEIVERBASE_H
