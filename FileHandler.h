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


#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "MotionSequence.h"
#include "Timeline.h"
#include <wx/string.h>

class FileHandler
{
  public:
    FileHandler();
    virtual ~FileHandler();

    static MotionSequence* read(wxString filename);
    static bool write(wxString filename, MotionSequence* sequence);
    static bool write(wxString filename, Timeline* timeline);

    static Skeleton* readBVHSkeleton(wxString filename);
    static MotionSequence* readBVH(wxString filename);
    static bool writeBVH(wxString filename, MotionSequence* sequence);

    // TODO(JK#2#): implement HTR read and write
    static Skeleton* readHTRSkeleton(wxString filename);
    static MotionSequence* readHTR(wxString filename);
    static bool writeHTR(wxString filename, MotionSequence* sequence);

    static Skeleton* readMCTSkeleton(wxString filename);
    static bool readMCT(wxString filename, Timeline* timeline);
    static bool writeMCT(wxString filename, Timeline* timeline);

  protected:

  private:
};

#endif // FILEHANDLER_H
