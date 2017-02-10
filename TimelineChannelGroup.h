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


#ifndef TIMELINECHANNELGROUP_H
#define TIMELINECHANNELGROUP_H

#include <set>
#include <vector>
#include "TimelineChannel.h"
#include "TimelineOverlay.h"

class TimelineChannelGroup
{
  public:
    TimelineChannelGroup() = delete;
    TimelineChannelGroup(int boneId);
    virtual ~TimelineChannelGroup();

    void setBoneId(int boneId) { _boneId = boneId; }
    int getBoneId() const { return _boneId; }

    void addChannel(TimelineChannel* channel) { _channels.insert(channel); }
    void removeChannel(TimelineChannel* channel) { _channels.erase(channel); }
    std::vector<int> getChannelIds() const;
    std::set<TimelineChannel*> getChannels() const { return _channels; }
    bool hasChannel(TimelineChannel* channel) const { return *_channels.find(channel) == channel; }

    std::istream& read(std::istream &s);
    std::ostream& write(std::ostream &s) const;
    std::istream& readBinary(std::istream &s);
    std::ostream& writeBinary(std::ostream &s) const;

  protected:

  private:
    int _boneId;
    std::set<TimelineChannel*> _channels;
};

std::ostream& operator<<(std::ostream& out, const TimelineChannelGroup& group);

std::istream& operator>>(std::istream& in, TimelineChannelGroup& group);

#endif // TIMELINECHANNELGROUP_H
