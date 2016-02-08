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


#include "AnimationManager.h"
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/tokenzr.h>
#include <wx/filename.h>
#include <utility>

AnimationManager::AnimationManager()
{
    //ctor
}

AnimationManager::~AnimationManager()
{
    //dtor
}

AnimationManager* AnimationManager::_animationManager = nullptr;

AnimationManager& AnimationManager::getInstance()
{
    if (_animationManager == nullptr)
    {
        _animationManager = new AnimationManager();
    }
    return *_animationManager;
}

int AnimationManager::addProjectSequence(MotionSequence* sequence)
{
    _projectSequences.push_back(sequence);
    return _projectSequences.size() - 1;
}

MotionSequence* AnimationManager::getProjectSequence(int pos)
{
    if (pos < 0 || pos >= int(_projectSequences.size()))
    {
        return nullptr;
    }
    return _projectSequences[pos];
}

const std::vector<MotionSequence*>& AnimationManager::getProjectSequences()
{
    return _projectSequences;
}

Skeleton* AnimationManager::readSkeletonFromBVH(wxString filename)
{

}

MotionSequence* AnimationManager::readBVH(wxString filename)
{
    if (!wxFileExists(filename))
    {
        return nullptr;
    }
    wxFileInputStream input(filename);
    wxTextInputStream textIn(input);

    if (!input.IsOk() || input.Eof())
    {
        return nullptr;
    }

    MotionSequence* sequence = new MotionSequence();
    sequence->setName(wxFileName(filename).GetName().ToStdString());

    // int numFrames = 0;

    int section = 1;
    int numOpenBraces = 0;
    bool rootSeen = false;
    bool endSite = false;

    int currentChannel = -1;

    // encodes how to receive channel information (numbers 0-2 are x-z-pos, 3-5 are x-z-rot and -1 means no further data)
    std::vector<std::pair<int, std::vector<char> > > channelInfo;

    size_t channelPos = 0;
    size_t channelDataPos = 0;
    bool frameHasPosition = false;

    Quaternion frameOrientation;
    Vector3 framePosition;

    wxStringTokenizer tokenizer;

    tokenizer.SetString(textIn.ReadLine());
    if (!tokenizer.GetNextToken().IsSameAs(_("HIERARCHY")))
    {
        delete sequence;
        return nullptr;
    }

    while (input.IsOk() && !input.Eof())
    {
        tokenizer.SetString(textIn.ReadLine());
        if (section == 1)
        {
            wxString token = tokenizer.GetNextToken();
            if (token.IsSameAs(_("ROOT")))
            {
                // no multiple skeletons per bvh file supported
                if (rootSeen)
                {
                    delete sequence;
                    return nullptr;
                }
                rootSeen = true;
                currentChannel = sequence->createChannel(-1);
                // currentChannel = sequence->getChannel(id);
                // TODO(JK#3#): if there is no name store Joint + id
                wxString name = tokenizer.GetNextToken();
                while (tokenizer.HasMoreTokens())
                {
                    name << _(" ") << tokenizer.GetNextToken();
                }
                // store name along with bone data from BVH
                sequence->getChannel(currentChannel)->setName(name.ToStdString());
            }
            else if (token.IsSameAs(_("JOINT")))
            {
                currentChannel = sequence->createChannel(currentChannel);
                // currentChannel = sequence->getChannel(id);
                // TODO(JK#3#): if there is no name store Joint + id
                wxString name = tokenizer.GetNextToken();
                while (tokenizer.HasMoreTokens())
                {
                    name << _(" ") << tokenizer.GetNextToken();
                }
                // store name along with bone data from BVH
                sequence->getChannel(currentChannel)->setName(name.ToStdString());
            }
            else if (token.IsSameAs(_("OFFSET")))
            {
                Vector3 offset;
                double val;
                for (size_t i = 0; i < 3; ++i)
                {
                    tokenizer.GetNextToken().ToDouble(&val);
                    offset(i) = val;
                }
                Bone* parentBone = sequence->getSkeleton()->getBone(currentChannel)->getParent();
                int parentChannel = -1;
                if (parentBone != nullptr)
                {
                    parentChannel = parentBone->getId();
                }
//                int parentChannel = sequence->getSkeleton()->getBone(currentChannel)->getParent()->getId();
                // check if this is the endeffector channel in the hierarchy
                if (!endSite)
                {
                    // MotionSequenceFrame pose(Quaternion(), offset);
                    // sequence->getChannel(currentChannel)->setInitialPose(pose);
                    if (sequence->getChannel(parentChannel) != nullptr)
                    {
                        sequence->getSkeleton()->getBone(currentChannel)->setStartPos(parentBone->getStartPos() + offset);
                    }
                    else
                    {
                        sequence->getSkeleton()->getBone(currentChannel)->setStartPos(offset);
                    }
                }
                else
                {
                    parentChannel = currentChannel;
                }
                if (sequence->getChannel(parentChannel) != nullptr)
                {
                    offset /= 100.0;// offset - sequence->getSkeleton()->getBone(parentChannel)->getStartPos();
                    // MotionSequenceFrame parentPose = sequence->getChannel(parentChannel)->getInitialPose();
                    // TODO(JK#7#): how to retrieve quaternion from incomplete orientation data in BVH?
                    Quaternion orientation(Vector3(1.0, 0.0, 0.0), offset);
                    // parentPose.setOrientation(orientation);
                    // sequence->getChannel(parentChannel)->setInitialPose(parentPose);
                    // sequence->getChannel(parentChannel)->setBoneLength(offset.norm());
                    sequence->getSkeleton()->getBone(parentChannel)->setAbsOrientation(orientation);
                    sequence->getSkeleton()->getBone(parentChannel)->setLength(offset.norm());
                }
            }
            else if (token.IsSameAs(_("{")))
            {
                ++numOpenBraces;
            }
            else if (token.IsSameAs(_("}")))
            {
                --numOpenBraces;
                if (endSite)
                {
                    endSite = false;
                }
                else
                {
                    Bone* parentBone = sequence->getSkeleton()->getBone(currentChannel)->getParent();
                    if (parentBone != nullptr)
                    {
                        currentChannel = parentBone->getId();
                    }
                    else
                    {
                        // numOpenBraces has to be zero when root is reached. If not, something went wrong while parsing the file
                        if (numOpenBraces != 0)
                        {
                            delete sequence;
                            return nullptr;
                        }
                    }
                }
            }
            else if (token.IsSameAs(_("CHANNELS")))
            {
                long numChannels;
                tokenizer.GetNextToken().ToLong(&numChannels);
                std::vector<char> currentChannelInfo;
                for (int i = 0; i < numChannels; ++i)
                {
                    token = tokenizer.GetNextToken();
                    if (token.IsSameAs(_("Xposition")))
                    {
                        currentChannelInfo.push_back(0);
                    }
                    else if (token.IsSameAs(_("Yposition")))
                    {
                        currentChannelInfo.push_back(1);
                    }
                    else if (token.IsSameAs(_("Zposition")))
                    {
                        currentChannelInfo.push_back(2);
                    }
                    else if (token.IsSameAs(_("Xrotation")))
                    {
                        currentChannelInfo.push_back(3);
                    }
                    else if (token.IsSameAs(_("Yrotation")))
                    {
                        currentChannelInfo.push_back(4);
                    }
                    else if (token.IsSameAs(_("Zrotation")))
                    {
                        currentChannelInfo.push_back(5);
                    }
                }
                channelInfo.push_back(std::make_pair(currentChannel, currentChannelInfo));
            }
            else if (token.IsSameAs(_("End")) && tokenizer.GetNextToken().IsSameAs(_("Site")))
            {
                endSite = true;
            }
            else if (token.IsSameAs(_("MOTION")))
            {
                // reading the skeleton is finished, update the skeleton and set its current pose as default
                sequence->getSkeleton()->update();
                sequence->getSkeleton()->setCurrentPoseAsDefault();

                // go to next section (motion data part)
                ++section;
                // get next two lines containing frames and frame time
                for (int i = 0; i < 2; ++i)
                {
                    tokenizer.SetString(textIn.ReadLine());
                    token = tokenizer.GetNextToken();
                    if (token.IsSameAs(_("Frames:")))
                    {
                        unsigned long val;
                        tokenizer.GetNextToken().ToULong(&val);
                        sequence->setNumFrames(val);
                    }
                    else if (token.IsSameAs(_("Frame")) && tokenizer.GetNextToken().IsSameAs(_("Time:")))
                    {
                        double val;
                        tokenizer.GetNextToken().ToDouble(&val);
                        sequence->setFrameTime(val);
                    }
                }
            }
        }
        else if (section == 2)
        {
            while (tokenizer.HasMoreTokens())
            {
                // check if all data for current channel was received
                if (channelDataPos == channelInfo[channelPos].second.size())
                {
                    int numFrames = sequence->getChannel(channelInfo[channelPos].first)->getNumFrames();
                    if (numFrames >= 0)
                    {
                        frameOrientation = frameOrientation * sequence->getSkeleton()->getBone(channelInfo[channelPos].first)->getRelDefaultOrientation();
                    }
                    else
                    {
                        frameOrientation = sequence->getChannel(channelInfo[channelPos].first)->getFrame(numFrames - 1).getOrientation() * frameOrientation;
                    }

                    MotionSequenceFrame frame(frameOrientation);
                    if (frameHasPosition)
                    {
                        frame.setPosition(framePosition);
                    }
                    // append current frame to current channel
                    sequence->getChannel(channelInfo[channelPos].first)->appendFrame(frame);
                    frameHasPosition = false;
                    // go to next channel or restart from first channel if all channels processed
                    if (++channelPos == channelInfo.size())
                    {
                        channelPos = 0;
                    }
                    channelDataPos = 0;
                    // reset orientation to identity
                    frameOrientation = Quaternion();
                }

                wxString token = tokenizer.GetNextToken();
                double val;
                token.ToDouble(&val);

                switch (channelInfo[channelPos].second[channelDataPos])
                {
                    case 0:
                        framePosition.x() = val;
                        frameHasPosition = true;
                        break;
                    case 1:
                        framePosition.y() = val;
                        frameHasPosition = true;
                        break;
                    case 2:
                        framePosition.z() = val;
                        frameHasPosition = true;
                        break;
                    case 3:
                        frameOrientation = frameOrientation * Quaternion(M_PI*val/180.0, 0.0, 0.0);
                        break;
                    case 4:
                        frameOrientation = frameOrientation * Quaternion(0.0, M_PI*val/180.0, 0.0);
                        break;
                    case 5:
                        frameOrientation = frameOrientation * Quaternion(0.0, 0.0, M_PI*val/180.0);
                        break;
                    default:
                        break;
                }
                // move to next data position
                ++channelDataPos;
            }
        }
    }
    return sequence;
}

