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


#include "FileHandler.h"
#include <wx/wfstream.h>
#include <wx/stdstream.h>
#include <wx/txtstrm.h>
#include <wx/tokenzr.h>
#include <wx/filename.h>

FileHandler::FileHandler()
{
    //ctor
}

FileHandler::~FileHandler()
{
    //dtor
}

MotionSequence* FileHandler::read(wxString filename)
{
    if (filename.Matches(_("*.bvh")))
    {
        return readBVH(filename);
    }
    else if (filename.Matches(_("*.htr")))
    {
        return readHTR(filename);
    }
    return nullptr;
}

bool FileHandler::write(wxString filename, MotionSequence* sequence)
{
    if (sequence == nullptr)
    {
        return false;
    }
    if (filename.Matches(_("*.bvh")))
    {
        return writeBVH(filename, sequence);
    }
    else if (filename.Matches(_("*.htr")))
    {
        return writeHTR(filename, sequence);
    }
    else
    {
        // by default write sequence to a bvh file
        filename = filename + _(".bvh");
        return writeBVH(filename, sequence);
    }
    return false;
}

bool FileHandler::write(wxString filename, Timeline* timeline)
{
    // TODO(JK#3#): save timeline
    if (timeline == nullptr)
    {
        return false;
    }
    if (filename.Matches(_("*.mct")))
    {
        return writeMCT(filename, timeline);
    }
    return false;
}

Skeleton* FileHandler::readBVHSkeleton(wxString filename)
{
    if (!wxFileExists(filename))
    {
        return nullptr;
    }
    wxFFileInputStream input(filename);
    wxTextInputStream textIn(input);

    if (!input.IsOk() || input.Eof())
    {
        return nullptr;
    }

    Skeleton* skeleton = new Skeleton();
    skeleton->setName(wxFileName(filename).GetName().ToStdString());

    int numOpenBraces = 0;
    bool rootSeen = false;
    bool endSite = false;

    int currentChannel = -1;

    wxStringTokenizer tokenizer;

    tokenizer.SetString(textIn.ReadLine());
    if (!tokenizer.GetNextToken().IsSameAs(_("HIERARCHY")))
    {
        delete skeleton;
        return nullptr;
    }

    while (input.IsOk() && !input.Eof())
    {
        tokenizer.SetString(textIn.ReadLine());

        wxString token = tokenizer.GetNextToken();
        if (token.IsSameAs(_("ROOT")))
        {
            // no multiple skeletons per bvh file supported
            if (rootSeen)
            {
                delete skeleton;
                return nullptr;
            }
            rootSeen = true;
            currentChannel = skeleton->createBone(-1);
            // currentChannel = sequence->getChannel(id);
            wxString name = tokenizer.GetNextToken();
            while (tokenizer.HasMoreTokens())
            {
                name << _(" ") << tokenizer.GetNextToken();
            }

            // if there is no name, store "Root Joint"
            if (name.size() == 0)
            {
                name << _("Root Joint");
            }
            // store name along with bone data from BVH
            skeleton->getBone(currentChannel)->setName(name.ToStdString());
        }
        else if (token.IsSameAs(_("JOINT")))
        {
            currentChannel = skeleton->createBone(currentChannel);
            // currentChannel = sequence->getChannel(id);
            wxString name = tokenizer.GetNextToken();
            while (tokenizer.HasMoreTokens())
            {
                name << _(" ") << tokenizer.GetNextToken();
            }

            // if there is no name, store "Joint" + id
            if (name.size() == 0)
            {
                name << _("Joint ") << currentChannel;
            }
            // store name along with bone data from BVH
            skeleton->getBone(currentChannel)->setName(name.ToStdString());
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
            Bone* parentBone = skeleton->getBone(currentChannel)->getParent();
            int parentBoneId = -1;
            if (parentBone != nullptr)
            {
                parentBoneId = parentBone->getId();
            }
            // check if this is the endeffector channel in the hierarchy
            if (!endSite)
            {
                // MotionSequenceFrame pose(Quaternion(), offset);
                // sequence->getChannel(currentChannel)->setInitialPose(pose);
                if (parentBone != nullptr)
                {
                    skeleton->getBone(currentChannel)->setStartPos(parentBone->getStartPos() + offset);
                }
                else
                {
                    skeleton->getBone(currentChannel)->setStartPos(offset);
                }
            }
            else
            {
                parentBoneId = currentChannel;
            }
            if (parentBone != nullptr)
            {
                // TODO(JK#3#): in bvh file the bone length can have a factor of 100 or of 1...
                offset /= 100.0;
                skeleton->getBone(parentBoneId)->setDefaultOrientation(offset);
                skeleton->getBone(parentBoneId)->setLength(offset.norm());
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
                Bone* parentBone = skeleton->getBone(currentChannel)->getParent();
                if (parentBone != nullptr)
                {
                    currentChannel = parentBone->getId();
                }
                else
                {
                    // numOpenBraces has to be zero when root is reached. If not, something went wrong while parsing the file
                    if (numOpenBraces != 0)
                    {
                        delete skeleton;
                        return nullptr;
                    }
                }
            }
        }
        else if (token.IsSameAs(_("CHANNELS")))
        {
            continue;
        }
        else if (token.IsSameAs(_("End")) && tokenizer.GetNextToken().Matches(_("Site*")))
        {
            endSite = true;
        }
        else if (token.IsSameAs(_("MOTION")))
        {
            // reading the skeleton is finished, update the skeleton and break the loop to return the skeleton
            skeleton->update();
            break;

        }
    }
    return skeleton;
}

MotionSequence* FileHandler::readBVH(wxString filename)
{
    if (!wxFileExists(filename))
    {
        return nullptr;
    }
    wxFFileInputStream input(filename);
    wxTextInputStream textIn(input);

    if (!input.IsOk() || input.Eof())
    {
        return nullptr;
    }

    MotionSequence* sequence = new MotionSequence();
    sequence->setName(wxFileName(filename).GetName().ToStdString());
    sequence->getSkeleton()->setName(wxFileName(filename).GetName().ToStdString());

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
                wxString name = tokenizer.GetNextToken();
                while (tokenizer.HasMoreTokens())
                {
                    name << _(" ") << tokenizer.GetNextToken();
                }

                // if there is no name, store "Root Joint"
                if (name.size() == 0)
                {
                    name << _("Root Joint");
                }
                // store name along with bone data from BVH
                sequence->getChannel(currentChannel)->setName(name.ToStdString());
                sequence->getSkeleton()->getBone(currentChannel)->setName(name.ToStdString());
            }
            else if (token.IsSameAs(_("JOINT")))
            {
                currentChannel = sequence->createChannel(currentChannel);
                // currentChannel = sequence->getChannel(id);
                wxString name = tokenizer.GetNextToken();
                while (tokenizer.HasMoreTokens())
                {
                    name << _(" ") << tokenizer.GetNextToken();
                }

                // if there is no name, store "Joint" + id
                if (name.size() == 0)
                {
                    name << _("Joint ") << currentChannel;
                }
                // store name along with bone data from BVH
                sequence->getChannel(currentChannel)->setName(name.ToStdString());
                sequence->getSkeleton()->getBone(currentChannel)->setName(name.ToStdString());
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
                    // TODO(JK#3#): in bvh file the bone length can have a factor of 100 or of 1...
                    offset /= 100.0;
                    sequence->getSkeleton()->getBone(parentChannel)->setDefaultOrientation(offset);
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
            else if (token.IsSameAs(_("End")) && tokenizer.GetNextToken().Matches(_("Site*")))
            {
                endSite = true;
            }
            else if (token.IsSameAs(_("MOTION")))
            {
                // reading the skeleton is finished, update the skeleton and set its current pose as default
                sequence->getSkeleton()->update();
                //sequence->getSkeleton()->setCurrentAsDefault();

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
                    /*
                    int numFrames = sequence->getChannel(channelInfo[channelPos].first)->getNumFrames();
                    if (numFrames >= 0)
                    {
                        //frameOrientation = frameOrientation * sequence->getSkeleton()->getBone(channelInfo[channelPos].first)->getRelDefaultOrientation();
                    }
                    else
                    {
                        //frameOrientation = sequence->getChannel(channelInfo[channelPos].first)->getFrame(numFrames - 1).getOrientation() * frameOrientation;
                    }*/

                    MotionSequenceFrame frame(frameOrientation);
                    if (frameHasPosition)
                    {
                        // TODO(JK#3#): in bvh file the frame position can have a factor of 100 or of 1...
                        framePosition /= 100.0f;
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

bool FileHandler::writeBVH(wxString filename, MotionSequence* sequence)
{
    if (sequence == nullptr)
    {
        return false;
    }
    Skeleton* skeleton = sequence->getSkeleton();
    if (skeleton == nullptr)
    {
        return false;
    }
    skeleton->setToDefault();

    Bone* root = skeleton->getRoot();
    if (root == nullptr)
    {
        return false;
    }

    {
        wxFileName file(filename);
        //check if the directory exists
        if (!wxDirExists(file.GetPath()))
        {
            wxMkdir(file.GetPath());
        }
    }

    wxFFileOutputStream output(filename);
    wxTextOutputStream textOut(output);

    std::vector<Bone*> bones = root->getAllChildrenDFS();
    bones.insert(bones.begin(), root);

    Vector3 offset;

    textOut << _("HIERARCHY") << endl;

    for (size_t i = 0; i < bones.size(); ++i)
    {
        if (i == 0)
        {
            textOut << _("ROOT ") << root->getName() << endl;
            textOut << _("{") << endl;
            offset = root->getStartPos() * 100.0f;
            textOut << _("OFFSET ") << offset.x() << _(" ") << offset.y() << _(" ") << offset.z() << endl;
            textOut << _("CHANNELS") << _(" 6 Xposition Yposition Zposition Zrotation Yrotation Xrotation") << endl;
        }
        else
        {
            if (bones[i]->getParent() != bones[i - 1])
            {
                textOut << _("End Site") << endl;
                textOut << _("{") << endl;
                offset = bones[i-1]->getDirection() * bones[i-1]->getLength() * 100.0f;
                textOut << _("OFFSET ") << offset.x() << _(" ") << offset.y() << _(" ") << offset.z() << endl;
                textOut << _("}") << endl;
                textOut << _("}") << endl;
                Bone* parentBone = bones[i - 1]->getParent();
                while (bones[i]->getParent() != parentBone)
                {
                    textOut << _("}") << endl;
                    parentBone = parentBone->getParent();
                }
            }

            textOut << _("JOINT ") << bones[i]->getName() << endl;
            textOut << _("{") << endl;
            offset = bones[i]->getParent()->getDirection() * bones[i]->getParent()->getLength() * 100.0f;
            textOut << _("OFFSET ") << offset.x() << _(" ") << offset.y() << _(" ") << offset.z() << endl;
            textOut << _("CHANNELS") << _(" 3 Zrotation Yrotation Xrotation") << endl;
        }
        if (i == bones.size() - 1)
        {
            textOut << _("End Site") << endl;
            textOut << _("{") << endl;
            offset = bones[i]->getDirection() * bones[i]->getLength() * 100.0f;
            textOut << _("OFFSET ") << offset.x() << _(" ") << offset.y() << _(" ") << offset.z() << endl;
            textOut << _("}") << endl;
            textOut << _("}") << endl;
            Bone* parentBone = bones[i]->getParent();
            while (root != parentBone)
            {
                textOut << _("}") << endl;
                parentBone = parentBone->getParent();
            }
        }
    }
    textOut << _("}") << endl;

    textOut << _("MOTION") << endl;
    textOut << _("Frames: ") << sequence->getNumFrames() << endl;
    textOut << _("Frame Time: ") << sequence->getFrameTime() << endl;

    Vector3 euler;

    for (size_t i = 0; i < sequence->getNumFrames(); ++i)
    {
        for (size_t j = 0; j < sequence->getNumChannels(); ++j)
        {
            if (j == 0)
            {
                offset = sequence->getChannel(bones[j]->getId())->getFrame(i).getPosition() * 100.0f;
                textOut << offset.x() << _(" ") << offset.y() << _(" ") << offset.z();
            }
            euler = sequence->getChannel(bones[j]->getId())->getFrame(i).getOrientation().toEuler() * (180.0/M_PI);
            textOut  << _(" ") << euler.z() << _(" ") << euler.y() << _(" ") << euler.x();
        }
        textOut << endl;
    }

    return true;
}

Skeleton* FileHandler::readHTRSkeleton(wxString filename)
{
    return nullptr;
}

MotionSequence* FileHandler::readHTR(wxString filename)
{
    if (!wxFileExists(filename))
    {
        return nullptr;
    }
    wxFFileInputStream input(filename);
    wxTextInputStream textIn(input);

    if (!input.IsOk() || input.Eof())
    {
        return nullptr;
    }

    MotionSequence* sequence = new MotionSequence();
    sequence->setName(wxFileName(filename).GetName().ToStdString());
    sequence->getSkeleton()->setName(wxFileName(filename).GetName().ToStdString());

    size_t numSegments = 0;
    size_t numFrames = 0;
    float frameTime = 0.0f;

    double calibrationFactor = 1.0;
    double scaleFactor = 1.0;
    double rotationFactor = 1.0;

    wxString rotationOrder;

    wxString rootBoneName;
    wxString currentBoneName;
    int currentBoneId = -1;

    std::map<std::string, std::vector<std::string> > parentChildMap;
    std::map<std::string, Bone> boneMap;

    wxStringTokenizer tokenizer;

    // read mode describes how to interpret data:
    // 0 -> default/ignore, 1 -> header, 2 -> hierarchy,
    // 3 -> basePosition, 4 -> data, 5 -> EndOfFile
    char readMode = 0;

    long longBuffer;
    double doubleBuffer;

    // TODO(JK#3#): improve read HTR (still buggy)
    while (input.IsOk() && !input.Eof())
    {
        tokenizer.SetString(textIn.ReadLine());

        while (tokenizer.HasMoreTokens())
        {
            wxString token = tokenizer.GetNextToken();

            if (token.Matches(_("#*")))
            {
                break;
            }
            else if (token.IsSameAs(_("[Header]"), false))
            {
                // set header mode
                readMode = 1;
                break;
            }
            else if (token.IsSameAs(_("[SegmentNames&Hierarchy]"), false))
            {
                // set hierarchy mode
                readMode = 2;
                break;
            }
            else if (token.IsSameAs(_("[BasePosition]"), false))
            {
                // set basePosition mode
                readMode = 3;
                break;
            }
            else if (token.IsSameAs(_("[EndOfFile]"), false))
            {
                // set EndOfFile mode
                readMode = 5;
                break;
            }
            else if (token.Matches(_("[*]")))
            {
                // set data mode
                readMode = 4;
                // create skeleton and sequence (if not done yet)
                if (sequence->getSkeleton()->getNumBones() == 0)
                {
                    std::map<std::string, std::vector<std::string> >::iterator parentIt;

                    sequence->setFrameTime(frameTime);

                    Skeleton skeleton;
                    skeleton.setName(sequence->getName());

                    std::vector<std::pair<int, std::string> > bones;
                    bones.push_back(std::make_pair(-1, rootBoneName.ToStdString()));

                    // TODO(JK#9#): HTR format can contain multiple roots - what to do?
                    // detect multiple roots and return in this case
                    parentIt = parentChildMap.find("global");
                    if (parentIt != parentChildMap.end())
                    {
                        if (parentIt->second.size() != 1)
                        {
                            return nullptr;
                        }
                    }

                    for (size_t i = 0; i < bones.size(); ++i)
                    {
                        int boneId;
                        auto boneIt = boneMap.find(bones[i].second);
                        if (boneIt != boneMap.end())
                        {
                            boneId = skeleton.createBone(boneIt->second, bones[i].first);

                            parentIt = parentChildMap.find(bones[i].second);
                            if (parentIt != parentChildMap.end())
                            {
                                for (size_t j = 0; j < parentIt->second.size(); ++j)
                                {
                                    bones.push_back(std::make_pair(boneId, parentIt->second[j]));
                                }
                            }
                        }
                    }
                    skeleton.update();
                    sequence->createFromSkeleton(skeleton);
                }
                // set current bone
                currentBoneName = token.Truncate(token.Length() - 1).AfterFirst('[');
                currentBoneId = sequence->getSkeleton()->getBoneId(currentBoneName.ToStdString());
                break;
            }

            // read header
            if (readMode == 1)
            {
                if (token.IsSameAs(_("FileType"), false))
                {
                    token = tokenizer.GetNextToken();
                    // TODO(JK#9#): consider to allow gtr files (in readHTR)
                    if (!token.IsSameAs(_("htr"), false))
                    {
                        delete sequence;
                        return nullptr;
                    }
                }
                else if (token.IsSameAs(_("DataType"), false))
                {
                    // DataType HTRS #Translation followed by rotation and scale data
                    token = tokenizer.GetNextToken();
                    // TODO(JK#9#): consider to allow other than HTRS files (in readHTR)
                    if (!token.IsSameAs(_("HTRS"), false))
                    {
                        delete sequence;
                        return nullptr;
                    }
                }
                else if (token.IsSameAs(_("FileVersion"), false))
                {
                    // FileVersion 1 #integer
                    token = tokenizer.GetNextToken();
                    // TODO(JK#9#): only file version 1 for HTR files supported yet
                    if (!token.IsSameAs(_("1")))
                    {
                        delete sequence;
                        return nullptr;
                    }
                }
                else if (token.IsSameAs(_("NumSegments"), false))
                {
                    //NumSegments 18 #integer
                    token = tokenizer.GetNextToken();
                    token.ToLong(&longBuffer);
                    numSegments = longBuffer;
                }
                else if (token.IsSameAs(_("NumFrames"), false))
                {
                    // NumFrames 2 #integer
                    token = tokenizer.GetNextToken();
                    token.ToLong(&longBuffer);
                    numFrames = longBuffer;
                }
                else if (token.IsSameAs(_("DataFrameRate"), false))
                {
                    // DataFrameRate 60 #integer, data frame rate in this file
                    token = tokenizer.GetNextToken();
                    token.ToLong(&longBuffer);
                    frameTime = 1.0f/float(longBuffer);
                }
                else if (token.IsSameAs(_("EulerRotationOrder"), false))
                {
                    // EulerRotationOrder ZYX
                    rotationOrder = tokenizer.GetNextToken().Lower();
                }
                else if (token.IsSameAs(_("CalibrationUnits"), false))
                {
                    // CalibrationUnits mm
                    token = tokenizer.GetNextToken().Lower();
                    if (token.IsSameAs(_("mm")) || token.Matches(_("millimeter*")))
                    {
                        calibrationFactor = 0.001;
                    }
                    else if (token.IsSameAs(_("cm")) || token.Matches(_("centimeter*")))
                    {
                        calibrationFactor = 0.01;
                    }
                    else if (token.IsSameAs(_("dm")) || token.Matches(_("decimeter*")))
                    {
                        calibrationFactor = 0.1;
                    }
                    else if (token.IsSameAs(_("m")) || token.Matches(_("meter*")))
                    {
                        calibrationFactor = 1.0;
                    }
                    else if (token.IsSameAs(_("in")) || token.Matches(_("inch*")))
                    {
                        calibrationFactor = 0.0254;
                    }
                }
                else if (token.IsSameAs(_("RotationUnits"), false))
                {
                    // RotationUnits Degrees
                    token = tokenizer.GetNextToken();
                    if (token.Lower().Matches(_("deg*")))
                    {
                        rotationFactor = M_PI/180.0;
                    }
                    else
                    {
                        rotationFactor = 1.0;
                    }
                }
                else if (token.IsSameAs(_("GlobalAxisofGravity"), false))
                {
                    // GlobalAxisofGravity Y
                    token = tokenizer.GetNextToken();
                }
                else if (token.IsSameAs(_("BoneLengthAxis"), false))
                {
                    // BoneLengthAxis Y
                    token = tokenizer.GetNextToken();
                }
                else if (token.IsSameAs(_("ScaleFactor"), false))
                {
                    // ScaleFactor 1.000000
                    token = tokenizer.GetNextToken();
                    token.ToDouble(&doubleBuffer);
                    scaleFactor = doubleBuffer;
                }
            }
            // read hierarchy
            else if (readMode == 2)
            {
                //token = tokenizer.GetNextToken();
                wxString parent = tokenizer.GetNextToken();
                if (parent.IsSameAs(_("global"), false))
                {
                    rootBoneName = token;
                    parent = parent.Lower();
                }
                parentChildMap[parent.ToStdString()].push_back(token.ToStdString());
            }
            // read basePosition
            else if (readMode == 3)
            {
                wxString boneName = token;

                token = tokenizer.GetNextToken();
                token.ToDouble(&doubleBuffer);
                float tx = doubleBuffer * calibrationFactor;
                token = tokenizer.GetNextToken();
                token.ToDouble(&doubleBuffer);
                float ty = doubleBuffer * calibrationFactor;
                token = tokenizer.GetNextToken();
                token.ToDouble(&doubleBuffer);
                float tz = doubleBuffer * calibrationFactor;

                token = tokenizer.GetNextToken();
                token.ToDouble(&doubleBuffer);
                Quaternion rx = Quaternion(doubleBuffer * rotationFactor, 0.0, 0.0);
                token = tokenizer.GetNextToken();
                token.ToDouble(&doubleBuffer);
                Quaternion ry = Quaternion(0.0, doubleBuffer * rotationFactor, 0.0);
                token = tokenizer.GetNextToken();
                token.ToDouble(&doubleBuffer);
                Quaternion rz = Quaternion(0.0, 0.0, doubleBuffer * rotationFactor);

                token = tokenizer.GetNextToken();
                token.ToDouble(&doubleBuffer);
                float boneLength = doubleBuffer * calibrationFactor;

                Quaternion boneOrientation;

                for (size_t i = 0; i < 3; ++i)
                {
                    if (rotationOrder[i] == 'x')
                    {
                        boneOrientation = rx * boneOrientation;
                    }
                    else if (rotationOrder[i] == 'y')
                    {
                        boneOrientation = ry * boneOrientation;
                    }
                    else if (rotationOrder[i] == 'z')
                    {
                        boneOrientation = rz * boneOrientation;
                    }
                }
                Bone bone;
                bone.setStartPos(tx, ty, tz);
                bone.setDefaultOrientation(boneOrientation);
                bone.setLength(boneLength);
                bone.setName(boneName.ToStdString());
                boneMap[boneName.ToStdString()] = bone;
            }
            // read data
            else if (readMode == 4)
            {
                token.ToLong(&longBuffer);
                size_t frameNumber = longBuffer;

                token = tokenizer.GetNextToken();
                token.ToDouble(&doubleBuffer);
                float tx = doubleBuffer * calibrationFactor;
                token = tokenizer.GetNextToken();
                token.ToDouble(&doubleBuffer);
                float ty = doubleBuffer * calibrationFactor;
                token = tokenizer.GetNextToken();
                token.ToDouble(&doubleBuffer);
                float tz = doubleBuffer * calibrationFactor;

                token = tokenizer.GetNextToken();
                token.ToDouble(&doubleBuffer);
                Quaternion rx = Quaternion(doubleBuffer * rotationFactor, 0.0, 0.0);
                token = tokenizer.GetNextToken();
                token.ToDouble(&doubleBuffer);
                Quaternion ry = Quaternion(0.0, doubleBuffer * rotationFactor, 0.0);
                token = tokenizer.GetNextToken();
                token.ToDouble(&doubleBuffer);
                Quaternion rz = Quaternion(0.0, 0.0, doubleBuffer * rotationFactor);

                token = tokenizer.GetNextToken();
                token.ToDouble(&doubleBuffer);
                float boneLength = doubleBuffer;

                Quaternion frameOrientation;

                for (size_t i = 0; i < 3; ++i)
                {
                    if (rotationOrder[i] == 'x')
                    {
                        frameOrientation = rx * frameOrientation;
                    }
                    else if (rotationOrder[i] == 'y')
                    {
                        frameOrientation = ry * frameOrientation;
                    }
                    else if (rotationOrder[i] == 'z')
                    {
                        frameOrientation = rz * frameOrientation;
                    }
                }

                MotionSequenceFrame frame;
                if (currentBoneName == rootBoneName)
                {
                    frame.setPosition(Vector3(tx, ty, tz));
                }
                frame.setOrientation(frameOrientation);
                sequence->getChannel(currentBoneId)->appendFrame(frame);
            }
            // end of file
            else if (readMode == 5)
            {
            }
        }
        sequence->setNumFrames(numFrames);
        // sequence->setHasAbsOrientations();
    }
    return sequence;
}

bool FileHandler::writeHTR(wxString filename, MotionSequence* sequence)
{
    if (sequence == nullptr)
    {
        return false;
    }
    Skeleton* skeleton = sequence->getSkeleton();
    if (skeleton == nullptr)
    {
        return false;
    }
    skeleton->setToDefault();

    Bone* root = skeleton->getRoot();
    if (root == nullptr)
    {
        return false;
    }

    {
        wxFileName file(filename);
        //check if the directory exists
        if (!wxDirExists(file.GetPath()))
        {
            wxMkdir(file.GetPath());
        }
    }

    wxFFileOutputStream output(filename);
    wxTextOutputStream textOut(output);

    std::vector<Bone*> bones = root->getAllChildrenDFS();
    bones.insert(bones.begin(), root);

    float degreeFactor = 180.0/M_PI;

    textOut << _("#Hierarchical Translation and Rotation (.htr) file") << endl;
    textOut << _("[Header]") << endl;
    textOut << _("FileType htr") << endl;
    textOut << _("DataType HTRS") << endl;
    textOut << _("FileVersion 1") << endl;
    textOut << _("NumSegments ") << sequence->getNumChannels() << endl;
    textOut << _("NumFrames ") << sequence->getNumFrames() << endl;
    textOut << _("DataFrameRate ") << int(1.0f/sequence->getFrameTime() + 0.5f) << endl;
    textOut << _("EulerRotationOrder XYZ") << endl;
    textOut << _("CalibrationUnits mm") << endl;
    textOut << _("RotationUnits Degrees") << endl;
    textOut << _("GlobalAxisofGravity Y") << endl;
    textOut << _("BoneLengthAxis Y") << endl;
    textOut << _("ScaleFactor 1.000000") << endl;

    textOut << _("[SegmentNames&Hierarchy]") << endl;
    textOut << _("#CHILD PARENT") << endl;
    textOut << root->getName() << _(" GLOBAL") << endl;

    for (size_t i = 1; i < bones.size(); ++i)
    {
        textOut << bones[i]->getName() << _(" ") << bones[i]->getParent()->getName() << endl;
    }

    textOut << _("[BasePosition]") << endl;
    textOut << _("#SegmentName Tx, Ty, Tz, Rx, Ry, Rz, BoneLength") << endl;

    for (size_t i = 0; i < bones.size(); ++i)
    {
        // convert to mm
        Vector3 pos = bones[i]->getStartPos() * 1000.0f;
        // convert to degrees
        Vector3 rot = bones[i]->getDefaultOrientation().toEuler() * degreeFactor;

        float length = bones[i]->getLength() * 1000.0f;
        textOut << bones[i]->getName() << _(" ") << pos.x() << _(" ") << pos.y() << _(" ") << pos.z() << _(" ");
        textOut << rot.x() << _(" ") << rot.y() << _(" ") << rot.z() << _(" ") << length << endl;
    }

    textOut << _("#Beginning of Data") << endl;

    for (auto it = sequence->beginChannels(); it != sequence->endChannels(); ++it)
    {
        textOut << _("[") << it->second->getName() << _("]") << endl;
        int i = 1;
        for (auto frameIt = it->second->beginFrames(); frameIt != it->second->endFrames(); ++frameIt)
        {
            // convert to mm
            Vector3 pos = frameIt->getPosition() * 1000.0f;
            // convert to degrees
            Vector3 rot = frameIt->getOrientation().toEuler() * degreeFactor;

            textOut << i++ << _(" ") << pos.x() << _(" ") << pos.y() << _(" ") << pos.z() << _(" ");
            textOut << rot.x() << _(" ") << rot.y() << _(" ") << rot.z() << _(" ") << _(" 1.000000") << endl;
        }
    }
    textOut << _("[ENDOFFILE]") << endl;

    return true;
}


bool FileHandler::writeMCT(wxString filename, Timeline* timeline)
{
    if (timeline == nullptr)
    {
        return false;
    }

    wxFFileOutputStream stream(filename);
    if (!stream.IsOk())
    {
        return false;
    }
    wxStdOutputStream out(stream);

    timeline->writeBinary(out);

    return true;
}

bool FileHandler::readMCT(wxString filename, Timeline* timeline)
{
    if (timeline == nullptr)
    {
        return false;
    }

    wxFFileInputStream stream(filename);
    if (!stream.IsOk())
    {
        return false;
    }
    wxStdInputStream in(stream);

    timeline->readBinary(in);

    return true;
}


