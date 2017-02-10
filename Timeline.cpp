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


#include "Timeline.h"
#include <algorithm>

Timeline::Timeline()
{
    _nextTrackId = 0;
}

Timeline::~Timeline()
{
    //dtor
}

void Timeline::setSkeletonToTime(uint64_t time)
{
    for (auto it = _channelGroups.begin(); it != _channelGroups.end(); ++it)
    {
        _skeleton.setRelBoneOrientation(it->first, computeBoneOrientation(it->first, time));

        /*
        std::vector<TimelineOverlay*> overlays = getOverlaysByBoneId(it->first, time);

        // if there is no overlay, just set the bone orientation according to the channel content
        if (overlays.size() == 0)
        {
            std::vector<TimelineTrack*> tracks = getTracks(it->first, time);
            if (tracks.size() > 0)
            {
                _skeleton.setRelBoneOrientation(it->first, tracks[0]->getFrameFromAbsTime(time, true).getOrientation());
            }
            else
            {
                _skeleton.setRelBoneOrientation(it->first, Quaternion());
            }
            continue;
        }
        std::sort(overlays.begin(), overlays.end(), TimelineOverlay::compPriority);

        Quaternion q;
        std::map<int, Quaternion> orientations;

        for (size_t i = 0; i < overlays.size(); ++i)
        {
            TimelineTrack* track1 = overlays[i]->getFirstTrack();
            TimelineTrack* track2 = overlays[i]->getSecondTrack();
            OverlayType type = overlays[i]->getType();
            Quaternion q1, q2;
            auto it1 = orientations.find(track1->getId());
            auto it2 = orientations.find(track2->getId());
            if (it1 == orientations.end())
            {
                q1 = track1->getFrameFromAbsTime(time, true).getOrientation();
            }
            else
            {
                q1 = it1->second;
            }
            if (it2 == orientations.end())
            {
                q2 = track2->getFrameFromAbsTime(time, true).getOrientation();
            }
            else
            {
                q2 = it2->second;
            }
            switch (type)
            {
                case OverlayType::ADDITIVE:
                    q = q1 * q2;
                    break;
                case OverlayType::SUBTRACTIVE:
                    q = q1 * q2.inv();
                    break;
                case OverlayType::IGNORING:
                    q = q1;
                    break;
                case OverlayType::OVERWRITE:
                    q = q2;
                    break;
                case OverlayType::LINEAR_INTERPOLATION:
                    q = q1.slerp(q2, double(time - overlays[i]->getStartTime()) / overlays[i]->getLength());
                    break;
                case OverlayType::STATIC_INTERPOLATION:
                    q = q1.slerp(q2, 0.5);
                    break;
                case OverlayType::WEIGHTED_INTERPOLATION:
                    q = track1->getFrameFromAbsTime(time, false).getOrientation().slerp(q2, 1.0 - track1->getWeight(track1->getFrameNumFromAbsTime(time)));
                    break;
                default:
                    q = q1 * q2;
                    break;
            }
            orientations[track1->getId()] = q;
            orientations[track2->getId()] = q;
        }

        _skeleton.setRelBoneOrientation(it->first, q);
        */

        /*
        std::set<TimelineChannel*> channels = it->second->getChannels();
        for (auto it = channels.begin(); it != channels.end(); ++it)
        {
            TimelineTrack* track = (*it)->getTrack(time);
            if (track != nullptr)
            {
                tracks.push_back(track);
                overlays = getOverlays(track, time);
                break;
                q = track->getFrameFromAbsTime(time, true).getOrientation() * q;
            }
        }
        */
    }
    _skeleton.update();
}

Quaternion Timeline::computeBoneOrientation(int boneId, uint64_t time) const
{
    std::vector<TimelineOverlay*> overlays = getOverlaysByBoneId(boneId, time);

    // if there is no overlay, just return the bone orientation according to the channel content
    if (overlays.size() == 0)
    {
        std::vector<const TimelineTrack*> tracks = getTracks(boneId, time);
        if (tracks.size() > 0)
        {
            return tracks[0]->getFrameFromAbsTime(time, true).getOrientation();
        }
        // no orientation data available, just return an unit quaternion
        return Quaternion();
    }

    std::sort(overlays.begin(), overlays.end(), TimelineOverlay::compPriority);

    Quaternion q;
    std::map<int, Quaternion> orientations;

    for (size_t i = 0; i < overlays.size(); ++i)
    {
        TimelineTrack* track1 = overlays[i]->getFirstTrack();
        TimelineTrack* track2 = overlays[i]->getSecondTrack();
        OverlayType type = overlays[i]->getType();
        Quaternion q1, q2;
        auto it1 = orientations.find(track1->getId());
        auto it2 = orientations.find(track2->getId());
        if (it1 == orientations.end())
        {
            q1 = track1->getFrameFromAbsTime(time, true).getOrientation();
        }
        else
        {
            q1 = it1->second;
        }
        if (it2 == orientations.end())
        {
            q2 = track2->getFrameFromAbsTime(time, true).getOrientation();
        }
        else
        {
            q2 = it2->second;
        }
        switch (type)
        {
            case OverlayType::ADDITIVE:
                q = q1 * q2;
                break;
            case OverlayType::SUBTRACTIVE:
                q = q1 * q2.inv();
                break;
            case OverlayType::IGNORING:
                q = q1;
                break;
            case OverlayType::OVERWRITE:
                q = q2;
                break;
            case OverlayType::LINEAR_INTERPOLATION:
                q = q1.slerp(q2, double(time - overlays[i]->getStartTime()) / overlays[i]->getLength());
                break;
            case OverlayType::STATIC_INTERPOLATION:
                q = q1.slerp(q2, 0.5);
                break;
            case OverlayType::WEIGHTED_INTERPOLATION:
                q = track1->getFrameFromAbsTime(time, false).getOrientation().slerp(q2, 1.0 - track1->getWeight(track1->getFrameNumFromAbsTime(time)));
                break;
            default:
                q = q1 * q2;
                break;
        }
        orientations[track1->getId()] = q;
        orientations[track2->getId()] = q;
    }
    return q;
}

void Timeline::createMotionSequence(MotionSequence* sequence, float frameTime) const
{
    if (sequence == nullptr)
    {
        return;
    }

    if (frameTime <= 0.0f)
    {
        frameTime = 0.04f;
    }

    uint64_t maxTime = getMaxTime();
    if (maxTime == 0)
    {
        return;
    }

    // TODO(JK#9#): for motion sequence creation maybe consider min time of the timeline

    sequence->createFromSkeleton(_skeleton);

    sequence->setFrameTime(frameTime);

    // TODO(JK#9#): createMotionSequence can be implemented with better efficiency
    uint64_t timeStep = frameTime * 1000000.0f;
    for (auto it = sequence->beginChannels(); it != sequence->endChannels(); ++it)
    {
        uint64_t time = 0;
        for (size_t i = 0; time < maxTime; ++i)
        {
            Quaternion q = computeBoneOrientation(it->first, time);
            it->second->appendFrame(MotionSequenceFrame(q));
            time += timeStep;
        }
    }
    sequence->setNumFrames(sequence->beginChannels()->second->getNumFrames());
}

uint64_t Timeline::getMinTime() const
{
    uint64_t minTime = std::numeric_limits<uint64_t>::infinity();
    for (auto it = _channels.begin(); it != _channels.end(); ++it)
    {
        uint64_t tempMinTime = it->second->getMinTime();
        if (tempMinTime < minTime)
        {
            minTime = tempMinTime;
            if (minTime == 0)
            {
                break;
            }
        }
    }
    return minTime;
}

uint64_t Timeline::getMaxTime() const
{
    uint64_t maxTime = 0;
    for (auto it = _channels.begin(); it != _channels.end(); ++it)
    {
        uint64_t tempMaxTime = it->second->getMaxTime();
        if (tempMaxTime > maxTime)
        {
            maxTime = tempMaxTime;
        }
    }
    return maxTime;
}

void Timeline::setSkeleton(Skeleton* skeleton)
{
    _skeleton = *skeleton;
}

Skeleton* Timeline::getSkeleton()
{
    return &_skeleton;
}

void Timeline::setChannelAffiliation(int channel, int boneId)
{
    // a negative bone id means "no bone". To store all not associated channels at one place, set all negative bone ids to -1
    if (boneId < -1)
    {
        boneId = -1;
    }
    auto it = _channels.find(channel);
    // if the channel did not exist yet, initialize it with the correct channel position
    if (it == _channels.end())
    {
        _channels[channel] = new TimelineChannel(channel);
        it = _channels.find(channel);
    }

    // erase old channel affiliation
    auto channelGroupIt = _channelGroups.find(it->second->getBoneId());
    if (channelGroupIt != _channelGroups.end())
    {
        channelGroupIt->second->removeChannel(it->second);
    }

    // set new channel affiliation
    channelGroupIt = _channelGroups.find(boneId);
    if (channelGroupIt == _channelGroups.end())
    {
        _channelGroups[boneId] = new TimelineChannelGroup(boneId);
        channelGroupIt = _channelGroups.find(boneId);
    }
    channelGroupIt->second->addChannel(it->second);

    it->second->setBoneId(boneId);

    // update overlays of all tracks in the channel
    std::vector<TimelineTrack*> tracks = it->second->getTracks();
    for (size_t i = 0; i < tracks.size(); ++i)
    {
        updateOverlays(tracks[i]);
    }
}

int Timeline::getChannelAffiliation(int channelId) const
{
    auto it = _channels.find(channelId);
    // if channel not yet exists, it also has no associated bone. Simply return -1
    if (it == _channels.end())
    {
        return -1;
    }
    return it->second->getBoneId();
}

std::string Timeline::getChannelAffiliationName(int channelId) const
{
    auto it = _channels.find(channelId);
    // if channel not yet exists, it also has no associated bone. Simply return "None"
    if (it == _channels.end())
    {
        return "None";
    }
    else if (it->second->getBoneId() < 0)
    {
        return "None";
    }
    else if (it->second->getBoneId() >= _skeleton.getNextFreeId())
    {
        return "Position";
    }
    return _skeleton.getBoneName(it->second->getBoneId());
}

void Timeline::insert(MotionSequence* sequence, int toChannel, uint64_t time)
{
    if (sequence->getNumChannels() == 0)
    {
        return;
    }
    for (auto it = sequence->beginChannels(); it != sequence->endChannels(); ++it)
    {
        insert(it->second, toChannel++, time, sequence->getName() + " " + it->second->getName());
    }
}

void Timeline::insert(MotionSequenceChannel* channel, int toChannel, uint64_t time, std::string name)
{
    TimelineTrack* newTrack = new TimelineTrack(channel);
    newTrack->setId(_nextTrackId);
    newTrack->setChannel(toChannel);
    newTrack->setName(name);

    _tracks[_nextTrackId] = newTrack;

    auto it = _channels.find(toChannel);
    // if the channel did not exist yet, initialize it with the correct channel position
    if (it == _channels.end())
    {
        _channels[toChannel] = new TimelineChannel(toChannel);
        it = _channels.find(toChannel);
    }

    it->second->insert(newTrack, time);
    ++_nextTrackId;

    updateOverlays(newTrack);
}

void Timeline::insert(const TimelineTrack &track, int toChannel, uint64_t time)
{
    TimelineTrack* newTrack = new TimelineTrack(track);
    newTrack->setId(_nextTrackId);
    newTrack->setChannel(toChannel);

    _tracks[_nextTrackId] = newTrack;

    auto it = _channels.find(toChannel);
    // if the channel does not exist yet, initialize it with the correct channel position
    if (it == _channels.end())
    {
        _channels[toChannel] = new TimelineChannel(toChannel);
        it = _channels.find(toChannel);
    }

    it->second->insert(newTrack, time);
    ++_nextTrackId;

    updateOverlays(newTrack);
}

void Timeline::insertChannelAfter(int channel)
{
    if (channel < -1)
    {
        return;
    }
    moveChannelsDown(channel + 1, 1);
}

void Timeline::moveChannelsDown(int startChannel, unsigned int num)
{
    if (startChannel < 0 || num <= 0)
    {
        return;
    }
    auto start = _channels.find(startChannel);
    // if start channel did not exist yet, create it first to have an object to compare against
    if (start == _channels.end())
    {
        _channels[startChannel] = new TimelineChannel(startChannel);
        start = _channels.find(startChannel);
    }
    auto it = _channels.end();
    while (it != start)
    {
        --it;
        swapChannels(it->first, it->first + num);
    }
}

void Timeline::moveTrack(int trackId, int toChannel, uint64_t toTime)
{
    auto it = _tracks.find(trackId);
    if (it == _tracks.end())
    {
        return;
    }
    TimelineTrack* track = it->second;

    if (track->getStartTime() == toTime && track->getChannel() == toChannel)
    {
        // nothing to do
        return;
    }
    auto channelIt = _channels.find(toChannel);
    // if the channel does not exist yet, initialize it with the correct channel position
    if (channelIt == _channels.end())
    {
        _channels[toChannel] = new TimelineChannel(toChannel);
        channelIt = _channels.find(toChannel);
    }

    _channels[track->getChannel()]->remove(trackId, track->getStartTime());
    channelIt->second->insert(track, toTime);

    updateOverlays(track);
}

bool Timeline::erase(int trackId)
{
    auto it = _tracks.find(trackId);
    if (it == _tracks.end())
    {
        return false;
    }

    TimelineTrack* track = it->second;

    removeOverlays(track);

    auto channelIt = _channels.find(track->getChannel());
    if (channelIt != _channels.end())
    {
        channelIt->second->remove(trackId, track->getStartTime());
    }
    _tracks.erase(it);
    delete track;

    return true;
}

void Timeline::clear()
{
    for (auto it = _tracks.begin(); it != _tracks.end(); ++it)
    {
        removeOverlays(it->second);
        delete it->second;
    }
    for (auto it = _channelGroups.begin(); it != _channelGroups.end(); ++it)
    {
        delete it->second;
    }
    for (auto it = _channels.begin(); it != _channels.end(); ++it)
    {
        delete it->second;
    }
    // _overlays should now be empty, but to be sure check it
    if (_overlays.size() > 0)
    {
        for (auto it = _overlays.begin(); it != _overlays.end(); ++it)
        {
            removeOverlay(it->second);
        }
    }
    _tracks.clear();
    _channels.clear();
    _channelGroups.clear();
    // reset _nextTrackId
    _nextTrackId = 0;
}

void Timeline::swapChannels(int channel1, int channel2)
{
    if (channel1 < 0 || channel2 < 0)
    {
        return;
    }

    std::map<int, TimelineChannel*>::iterator channel1It = _channels.find(channel1);
    std::map<int, TimelineChannel*>::iterator channel2It = _channels.find(channel2);
    if (channel1It == _channels.end())
    {
        _channels[channel1] = new TimelineChannel(channel1);
        channel1It = _channels.find(channel1);
    }
    if (channel2It == _channels.end())
    {
        _channels[channel2] = new TimelineChannel(channel2);
        channel2It = _channels.find(channel2);
    }
    std::swap(channel1It->second, channel2It->second);
    // TODO(JK#2#): swapping channels is buggy, also now it uses a channel pointer
    //TimelineChannel* channel_1 = _channels[channel1];
    //TimelineChannel* channel_2 = _channels[channel2];
    //TimelineChannel* swapChannel = channel_1;

    // setChannelAffiliation(channel1, channel_2->getBoneId());
    // setChannelAffiliation(channel2, swapChannel->getBoneId());

    //channel_1 = channel_2;
    //channel_2 = swapChannel;

    channel1It->second->setChannelPos(channel1);
    channel2It->second->setChannelPos(channel2);
}

void Timeline::clearChannel(int channel)
{
    auto it = _channels.find(channel);
    if (it == _channels.end())
    {
        return;
    }
    std::vector<TimelineTrack*> tracks = it->second->getTracks();
    for (size_t i = 0; i < tracks.size(); ++i)
    {
        erase(tracks[i]->getId());
    }
    it->second->clear();
}

void Timeline::sortChannels()
{

}

void Timeline::changeFrameTime(int trackId, float newFrameTime)
{
    changeFrameTime(getTrack(trackId), newFrameTime);
}

void Timeline::changeFrameTime(TimelineTrack* track, float newFrameTime)
{
    if (track == nullptr)
    {
        return;
    }
    track->setFrameTime(newFrameTime);
    updateOverlays(track);
}

void Timeline::cut(int trackId, uint64_t time)
{
    auto it = _tracks.find(trackId);
    if (it == _tracks.end())
    {
        return;
    }
    if (time <= it->second->getStartTime())
    {
        return;
    }
    TimelineTrack newTrack;
    if (it->second->cut(time - it->second->getStartTime(), &newTrack))
    {
        insert(newTrack, it->second->getChannel(), newTrack.getStartTime());
        updateOverlays(it->second);
    }
}

void Timeline::cut(uint64_t time)
{
    for (auto it = _channels.begin(); it != _channels.end(); ++it)
    {
        TimelineTrack* track = getTrack(it->second->getChannelPos(), time);
        if (track != nullptr)
        {
            cut(track->getId(), time);
        }
    }
}

TimelineTrack* Timeline::getTrack(int id)
{
    auto it = _tracks.find(id);
    if (it == _tracks.end())
    {
        return nullptr;
    }
    return it->second;
}

TimelineTrack* Timeline::getTrack(int channel, uint64_t time)
{
    auto it = _channels.find(channel);
    if (it == _channels.end())
    {
        return nullptr;
    }
    return it->second->getTrack(time);
}

std::vector<TimelineTrack*> Timeline::getTracks(int boneId, uint64_t time)
{
    std::vector<TimelineTrack*> tracks;
    auto groupIt = _channelGroups.find(boneId);
    if (groupIt == _channelGroups.end() || boneId < 0)
    {
        return tracks;
    }
    std::set<TimelineChannel*> channels = groupIt->second->getChannels();
    for (auto it = channels.begin(); it != channels.end(); ++it)
    {
        TimelineTrack* track = (*it)->getTrack(time);
        if (track != nullptr)
        {
            tracks.push_back(track);
        }
    }
    return tracks;
}

std::vector<const TimelineTrack*> Timeline::getTracks(int boneId, uint64_t time) const
{
    std::vector<const TimelineTrack*> tracks;
    auto groupIt = _channelGroups.find(boneId);
    if (groupIt == _channelGroups.end() || boneId < 0)
    {
        return tracks;
    }
    std::set<TimelineChannel*> channels = groupIt->second->getChannels();
    for (auto it = channels.begin(); it != channels.end(); ++it)
    {
        TimelineTrack* track = (*it)->getTrack(time);
        if (track != nullptr)
        {
            tracks.push_back(track);
        }
    }
    return tracks;
}

TimelineTrack* Timeline::getTrackBefore(int channel, uint64_t time)
{
    auto it = _channels.find(channel);
    if (it == _channels.end())
    {
        return nullptr;
    }
    return it->second->getTrackBefore(time);
}

TimelineTrack* Timeline::getTrackAfter(int channel, uint64_t time)
{
    auto it = _channels.find(channel);
    if (it == _channels.end())
    {
        return nullptr;
    }
    return it->second->getTrackAfter(time);
}

bool Timeline::isBetweenTwoTracks(int channel, uint64_t time) const
{
    auto it = _channels.find(channel);
    if (it == _channels.end())
    {
        return false;
    }
    return it->second->isBetweenTwoTracks(time);
}

bool Timeline::isInsideTrack(int channel, uint64_t time) const
{
    auto it = _channels.find(channel);
    if (it == _channels.end())
    {
        return false;
    }
    return it->second->isInsideTrack(time);
}

void Timeline::addInterpolation(int channel, uint64_t time, float frameTime)
{
    if (!isBetweenTwoTracks(channel, time))
    {
        return;
    }
    uint64_t interpolationStartTime = getTrackBefore(channel, time)->getEndTime();
    uint64_t interpolationEndTime = getTrackAfter(channel, time)->getStartTime();
    uint64_t length = interpolationEndTime - interpolationStartTime;
    // we want a frame time of ~ 5 ms, so divide delta time given in us by 5*1000 us to get the num of frames fitting into the window
    uint64_t numFrames = length / (frameTime * 1000 * 1000);
    if (numFrames == 0)
    {
        numFrames = 1;
    }
    // now find the exact frame time, which must be given in a fraction of seconds
    frameTime = double(length) / double(numFrames * 1000 * 1000);
    Quaternion q = getTrackBefore(channel, time)->getLastFrame().getOrientation();
    Quaternion p = getTrackAfter(channel, time)->getFirstFrame().getOrientation();
    TimelineTrack newTrack;
    newTrack.setFrameTime(frameTime);
    float step = 1.0f / float(numFrames + 1);

    for (unsigned int i = 1; i < numFrames + 1; ++i)
    {
        newTrack.appendFrame(MotionSequenceFrame(q.slerp(p, i*step)));
    }
    insert(newTrack, channel, interpolationStartTime);
}

std::vector<TimelineTrack*> Timeline::getOverlapping(const TimelineTrack* track)
{
    std::vector<TimelineTrack*> tracks;

    if (track == nullptr)
    {
        return tracks;
    }
    auto it = _channels.find(track->getChannel());
    if (it == _channels.end() || it->second->getBoneId() < 0)
    {
        return tracks;
    }
    // find channels affiliated with same bone
    auto it2 = _channelGroups.find(it->second->getBoneId());
    if (it2 == _channelGroups.end())
    {
        return tracks;
    }
    std::vector<int> channels = it2->second->getChannelIds();
    for (size_t i = 0; i < channels.size(); ++i)
    {
        std::vector<TimelineTrack*> tracksToAdd = getInRange(channels[i], track->getStartTime(), track->getEndTime());
        // remove the track for which the overlapping tracks are seeked from the array
        if (channels[i] == track->getChannel())
        {
            for (size_t j = 0; j < tracksToAdd.size(); ++j)
            {
                if (tracksToAdd[j] == track)
                {
                    tracksToAdd.erase(tracksToAdd.begin() + j);
                }
            }
        }
        tracks.insert(tracks.end(), tracksToAdd.begin(), tracksToAdd.end());
    }
    return tracks;
}

std::vector<TimelineTrack*> Timeline::getInRange(int channel, uint64_t startTime, uint64_t endTime)
{
    auto it = _channels.find(channel);
    if (it == _channels.end())
    {
        return std::vector<TimelineTrack*>();
    }
    return it->second->getInRange(startTime, endTime);
}

unsigned int Timeline::getOverlayId(TimelineTrack* track1, TimelineTrack* track2) const
{
    return (track1->getId() << 16) + track2->getId();
}

TimelineOverlay* Timeline::getOverlay(TimelineTrack* track1, TimelineTrack* track2) const
{
    auto it = _overlays.find(getOverlayId(track1, track2));
    if (it == _overlays.end())
    {
        return nullptr;
    }
    return it->second;
}

std::vector<TimelineOverlay*> Timeline::getOverlays(const TimelineTrack* track) const
{
    std::vector<TimelineOverlay*> overlays;
    unsigned int id = track->getId() << 16;
    unsigned int maxId = (track->getId() + 1) << 16;
    auto it = _overlays.lower_bound(id);
    if (it == _overlays.end())
    {
        return overlays;
    }
    while (it->first < maxId)
    {
        if (it->second->getLength() > 0)
        {
            overlays.push_back(it->second);
        }
        ++it;
        if (it == _overlays.end())
        {
            break;
        }
    }
    return overlays;
}

std::vector<TimelineOverlay*> Timeline::getOverlays(TimelineTrack* track, uint64_t time) const
{
    std::vector<TimelineOverlay*> overlays;
    unsigned int id = track->getId() << 16;
    unsigned int maxId = (track->getId() + 1) << 16;

    auto it = _overlays.lower_bound(id);
    if (it == _overlays.end())
    {
        return overlays;
    }
    while (it->first < maxId)
    {
        if (it->second->getStartTime() <= time && it->second->getEndTime() > time)
        {
            overlays.push_back(it->second);
        }
        ++it;
        if (it == _overlays.end())
        {
            break;
        }
    }
    return overlays;
}

std::vector<TimelineOverlay*> Timeline::getOverlaysByBoneId(int boneId, uint64_t time) const
{
    std::vector<TimelineOverlay*> overlays;
    auto groupIt = _channelGroups.find(boneId);
    if (groupIt == _channelGroups.end() || boneId < 0)
    {
        return overlays;
    }
    TimelineTrack* prevTrack = nullptr;
    TimelineTrack* track = nullptr;
    std::set<TimelineChannel*> channels = groupIt->second->getChannels();
    for (auto it = channels.begin(); it != channels.end(); ++it)
    {
        track = (*it)->getTrack(time);
        if (prevTrack != nullptr && track != nullptr)
        {
            overlays.push_back(getOverlay(prevTrack, track));
            prevTrack = track;
        }
        else if (prevTrack == nullptr)
        {
            prevTrack = track;
        }
    }
    return overlays;
}


void Timeline::updateOverlays(TimelineTrack* track)
{
    std::vector<TimelineOverlay*> overlays = getOverlays(track);
    for (size_t i = 0; i < overlays.size(); ++i)
    {
        // invalidate all overlays (an overlay with zero length is non-existent and can only be found by an id look-up)
        overlays[i]->setLength(0);
    }
    std::vector<TimelineTrack*> overlapping = getOverlapping(track);
    for (size_t i = 0; i < overlapping.size(); ++i)
    {
        TimelineOverlay* overlay = createOverlay(track, overlapping[i]);
        if (track->getStartTime() > overlapping[i]->getStartTime())
        {
            overlay->setStartTime(track->getStartTime());
            overlay->setLength(std::min(track->getLength(), overlapping[i]->getEndTime() - track->getStartTime()));
        }
        else
        {
            overlay->setStartTime(overlapping[i]->getStartTime());
            overlay->setLength(std::min(overlapping[i]->getLength(), track->getEndTime() - overlapping[i]->getStartTime()));
        }
    }
}

std::istream& Timeline::read(std::istream& s)
{
    s >> _nextTrackId;

    size_t numTracks, numChannels, numOverlays;

    s >> numTracks;
    s >> numChannels;
    s >> numOverlays;

    s.ignore(100, '\n');

    _skeleton.read(s);

    s.ignore(100, '\n');

    // read tracks
    for (size_t i = 0; i < numTracks; ++i)
    {
        TimelineTrack* newTrack = new TimelineTrack();
        newTrack->read(s);

        _tracks[newTrack->getId()] = newTrack;

        auto it = _channels.find(newTrack->getChannel());
        // if the channel does not exist yet, initialize it with the correct channel position
        if (it == _channels.end())
        {
            _channels[newTrack->getChannel()] = new TimelineChannel(newTrack->getChannel());
            it = _channels.find(newTrack->getChannel());
        }
        it->second->insert(newTrack, newTrack->getStartTime());

        updateOverlays(newTrack);
    }
    s.ignore(100, '\n');

    for (size_t i = 0; i < numChannels; ++i)
    {
        int channel, boneId;
        s >> channel;
        s >> boneId;
        setChannelAffiliation(channel, boneId);
    }
    s.ignore(100, '\n');

    for (size_t i = 0; i < numOverlays; ++i)
    {
        unsigned int overlayId;
        s >> overlayId;
        _overlays[overlayId]->read(s);
    }
    return s;
}

std::ostream& Timeline::write(std::ostream& s) const
{
    s << _nextTrackId << ' ' << _tracks.size() << ' ' << _channels.size() << ' ' << _overlays.size() << std::endl;
    s << _skeleton << std::endl;

    for (auto it = _tracks.begin(); it != _tracks.end(); ++it)
    {
        it->second->write(s);
        s << ' ';
    }
    s << std::endl;

    for (auto it = _channels.begin(); it != _channels.end(); ++it)
    {
        s << it->first << ' ' << it->second->getBoneId() << ' ';
    }
    s << std::endl;

    for (auto it = _overlays.begin(); it != _overlays.end(); ++it)
    {
        s << it->first << ' ';
        it->second->write(s);
        s << ' ';
    }
    return s;
}

std::istream& Timeline::readBinary(std::istream& s)
{
    size_t numTracks, numChannels, numOverlays;

    s.read((char*)&_nextTrackId, sizeof(_nextTrackId));
    s.read((char*)&numTracks, sizeof(size_t));
    s.read((char*)&numChannels, sizeof(size_t));
    s.read((char*)&numOverlays, sizeof(size_t));

    _skeleton.readBinary(s);

    // read tracks
    for (size_t i = 0; i < numTracks; ++i)
    {
        TimelineTrack* newTrack = new TimelineTrack();
        newTrack->readBinary(s);

        _tracks[newTrack->getId()] = newTrack;

        auto it = _channels.find(newTrack->getChannel());
        // if the channel does not exist yet, initialize it with the correct channel position
        if (it == _channels.end())
        {
            _channels[newTrack->getChannel()] = new TimelineChannel(newTrack->getChannel());
            it = _channels.find(newTrack->getChannel());
        }
        it->second->insert(newTrack, newTrack->getStartTime());

        updateOverlays(newTrack);
    }

    for (size_t i = 0; i < numChannels; ++i)
    {
        int channel, boneId;
        s.read((char*)&channel, sizeof(int));
        s.read((char*)&boneId, sizeof(int));

        setChannelAffiliation(channel, boneId);
    }

    for (size_t i = 0; i < numOverlays; ++i)
    {
        unsigned int overlayId;
        s.read((char*)&overlayId, sizeof(unsigned int));
        _overlays[overlayId]->readBinary(s);
    }
    return s;
}

std::ostream& Timeline::writeBinary(std::ostream& s) const
{
    s.write((char*)&_nextTrackId, sizeof(_nextTrackId));
    size_t size = _tracks.size();
    s.write((char*)&size, sizeof(size_t));
    size = _channels.size();
    s.write((char*)&size, sizeof(size_t));
    size = _overlays.size();
    s.write((char*)&size, sizeof(size_t));

    _skeleton.writeBinary(s);

    for (auto it = _tracks.begin(); it != _tracks.end(); ++it)
    {
        it->second->writeBinary(s);
    }

    for (auto it = _channels.begin(); it != _channels.end(); ++it)
    {
        s.write((char*)&it->first, sizeof(int));
        int boneId = it->second->getBoneId();
        s.write((char*)&boneId, sizeof(int));
    }

    for (auto it = _overlays.begin(); it != _overlays.end(); ++it)
    {
        s.write((char*)&it->first, sizeof(unsigned int));
        it->second->writeBinary(s);
    }
    return s;
}


TimelineOverlay* Timeline::createOverlay(TimelineTrack* track1, TimelineTrack* track2)
{
    unsigned int id1 = getOverlayId(track1, track2);
    auto it = _overlays.find(id1);
    if (it != _overlays.end())
    {
        return it->second;
    }
    // get alternative overlay id
    unsigned int id2 = getOverlayId(track2, track1);
    TimelineOverlay* overlay = new TimelineOverlay();
    overlay->setTracks(track1, track2);
    _overlays[id1] = overlay;
    _overlays[id2] = overlay;
    return overlay;
}

void Timeline::removeOverlay(TimelineTrack* track1, TimelineTrack* track2)
{
    auto it = _overlays.find(getOverlayId(track1, track2));
    if (it != _overlays.end())
    {
        delete it->second;
        _overlays.erase(it);
        it = _overlays.find(getOverlayId(track2, track1));
        _overlays.erase(it);
    }
}

void Timeline::removeOverlays(TimelineTrack* track)
{
    std::vector<TimelineOverlay*> overlays = getOverlays(track);
    for (size_t i = 0; i < overlays.size(); ++i)
    {
        removeOverlay(overlays[i]);
    }
}

void Timeline::removeOverlay(TimelineOverlay* overlay)
{
    removeOverlay(overlay->getFirstTrack(), overlay->getSecondTrack());
}


std::ostream& operator<<(std::ostream& out, const Timeline& timeline)
{
    return timeline.write(out);
}

std::istream& operator>>(std::istream& in, Timeline& timeline)
{
    return timeline.read(in);
}

