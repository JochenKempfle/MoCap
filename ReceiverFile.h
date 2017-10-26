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


#ifndef RECEIVERFILE_H
#define RECEIVERFILE_H

#include "ReceiverBase.h"
#include <vector>
#include <string>
#include <wx/string.h>

#include "SensorData.h"


class ReceiverFile : public ReceiverBase
{
    DECLARE_RECEIVER(ReceiverFile)

  public:
    ReceiverFile();
    ReceiverFile(wxString filename);
    virtual ~ReceiverFile();

    std::string getName() const { return "ReceiverFile"; }
    std::string getInfo() const { return _filename.ToStdString(); }

    bool setup();

    bool isConnected() const;

    void setFilename(wxString filename) { _filename = filename; }
    wxString getFilename() const { return _filename; }

    // TODO(JK#5#2017-09-13): in ReceiverBase implement binary read mode
    /*
    void setBinary(bool binary = true) { _binary = binary; }
    void unsetBinary() { _binary = false; }
    bool isBinary() const { return _binary; }
    */

  protected:
    bool onUpdate();
    bool onConnect();
    void onDisconnect();

  private:
    wxString _filename;
    bool _binary;

    int64_t _startTime;

    size_t _pos;
    int _id;

    unsigned int _time;

    std::vector<SensorDataIMURaw> _data;

    // calibration values
    const float ratio_acc  = 4.0f/32767.0f;
    const float ratio_gyro = (1000.0f/32767.0f) * (M_PI/180.0f);
    const float ratio_mag  = 48.0f/32767.0f;
};

#endif // RECEIVERFILE_H
