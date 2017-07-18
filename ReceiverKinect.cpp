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


#include "ReceiverKinect.h"
#include "SensorManager.h"

ReceiverKinect::ReceiverKinect()
{
    _bodyFrameReader = nullptr;
    _coordinateMapper = nullptr;
    _kinectSensor = nullptr;
    _startTime = -1;
    // TODO(JK#9#2017-07-05): Kinect requires to run KinectService.exe, but the file (and Kinect) does not exist to the software (although it does exist on the system)
    // luckily the KinectService seems to get started at system startup time
    if (wxFileExists(_("C:\\Windows\\System32\\Kinect\\KinectService.exe")))
    {
        long pid = wxExecute(_("C:\\Windows\\System32\\Kinect\\KinectService.exe"));
        if (pid == 0)
        {
            wxLogDebug(_("Could not start KinectService.exe"));
        }
    }
    else
    {
        wxLogDebug(_("KinectService.exe does not exist!"));
    }
}

ReceiverKinect::~ReceiverKinect()
{
    disconnect();
}


std::string ReceiverKinect::getName() const
{
    return "ReceiverKinect";
}

bool ReceiverKinect::update()
{
    /*
    wxString msg;
    msg << _("update!");
    wxLogDebug(msg);
    */
    // TODO(JK#1#2017-05-24): Kinect update stuff
    if (_bodyFrameReader == nullptr)
    {
        wxString msg;
        msg << _("update failed!");
        wxLogDebug(msg);
        return false;
    }

    BOOLEAN available = false;
    _kinectSensor->get_IsAvailable(&available);

    if (!available)
    {
        wxLogDebug(_("Kinect not available"));
        return false;
    }


    if (_multiSourceFrameReader == nullptr)
    {
        wxLogDebug(_("Kinect update failed - multi source frame reader not available!"));
        return false;
    }

    IMultiSourceFrame* multiSourceFrame = nullptr;
    IDepthFrame* depthFrame = nullptr;
    IColorFrame* colorFrame = nullptr;
    IBodyIndexFrame* bodyIndexFrame = nullptr;
    IBodyFrame* bodyFrame = nullptr;

    HRESULT hr = _multiSourceFrameReader->AcquireLatestFrame(&multiSourceFrame);

    // acquire frames
    if (SUCCEEDED(hr))
    {
        IDepthFrameReference* depthFrameReference = NULL;

        hr = multiSourceFrame->get_DepthFrameReference(&depthFrameReference);
        if (SUCCEEDED(hr))
        {
            hr = depthFrameReference->AcquireFrame(&depthFrame);
        }

        SafeRelease(&depthFrameReference);
    }

    if (SUCCEEDED(hr))
    {
        IColorFrameReference* colorFrameReference = NULL;

        hr = multiSourceFrame->get_ColorFrameReference(&colorFrameReference);
        if (SUCCEEDED(hr))
        {
            hr = colorFrameReference->AcquireFrame(&colorFrame);
        }

        SafeRelease(&colorFrameReference);
    }

    if (SUCCEEDED(hr))
    {
        IBodyIndexFrameReference* bodyIndexFrameReference = NULL;

        hr = multiSourceFrame->get_BodyIndexFrameReference(&bodyIndexFrameReference);
        if (SUCCEEDED(hr))
        {
            hr = bodyIndexFrameReference->AcquireFrame(&bodyIndexFrame);
        }

        SafeRelease(&bodyIndexFrameReference);
    }

    if (SUCCEEDED(hr))
    {
        IBodyFrameReference* bodyFrameReference = NULL;

        hr = multiSourceFrame->get_BodyFrameReference(&bodyFrameReference);
        if (SUCCEEDED(hr))
        {
            hr = bodyFrameReference->AcquireFrame(&bodyFrame);
        }

        SafeRelease(&bodyFrameReference);
    }

    if (SUCCEEDED(hr))
    {
        int64_t nTime = 0;

        hr = bodyFrame->get_RelativeTime(&nTime);

        // get time point at which the data arrived
        wxLongLong time = wxGetUTCTimeMillis();
        uint64_t receiveTime = (uint64_t(time.GetHi()) << 32) + time.GetLo();

        if (_startTime < 0)
        {
            _startTime = receiveTime;
        }


        INT64 nDepthTime = 0;
        IFrameDescription* depthFrameDescription = NULL;
        int nDepthWidth = 0;
        int nDepthHeight = 0;
        UINT nDepthBufferSize = 0;
        UINT16 *pDepthBuffer = NULL;

        IFrameDescription* colorFrameDescription = NULL;
        int nColorWidth = 0;
        int nColorHeight = 0;
        ColorImageFormat imageFormat = ColorImageFormat_None;
        UINT nColorBufferSize = 0;
        RGBQUAD *pColorBuffer = NULL;

        IFrameDescription* bodyIndexFrameDescription = NULL;
        int nBodyIndexWidth = 0;
        int nBodyIndexHeight = 0;
        UINT nBodyIndexBufferSize = 0;
        BYTE *pBodyIndexBuffer = NULL;

        // get depth frame data

        hr = depthFrame->get_RelativeTime(&nDepthTime);

        if (SUCCEEDED(hr))
        {
            hr = depthFrame->get_FrameDescription(&depthFrameDescription);
        }

        if (SUCCEEDED(hr))
        {
            hr = depthFrameDescription->get_Width(&nDepthWidth);
        }

        if (SUCCEEDED(hr))
        {
            hr = depthFrameDescription->get_Height(&nDepthHeight);
        }

        if (SUCCEEDED(hr))
        {
            hr = depthFrame->AccessUnderlyingBuffer(&nDepthBufferSize, &pDepthBuffer);
        }

        // get color frame data

        if (SUCCEEDED(hr))
        {
            hr = colorFrame->get_FrameDescription(&colorFrameDescription);
        }

        if (SUCCEEDED(hr))
        {
            hr = colorFrameDescription->get_Width(&nColorWidth);
        }

        if (SUCCEEDED(hr))
        {
            hr = colorFrameDescription->get_Height(&nColorHeight);
        }

        if (SUCCEEDED(hr))
        {
            hr = colorFrame->get_RawColorImageFormat(&imageFormat);
        }

        if (SUCCEEDED(hr))
        {
            /*
            if (imageFormat == ColorImageFormat_Bgra)
            {
                hr = colorFrame->AccessRawUnderlyingBuffer(&nColorBufferSize, reinterpret_cast<BYTE**>(&pColorBuffer));
            }
            else if (m_pColorRGBX)
            {
                pColorBuffer = m_pColorRGBX;
                nColorBufferSize = _colorWidth * _colorHeight * sizeof(RGBQUAD);
                hr = colorFrame->CopyConvertedFrameDataToArray(nColorBufferSize, reinterpret_cast<BYTE*>(pColorBuffer), ColorImageFormat_Bgra);
            }
            else
            {
                hr = E_FAIL;
            }
            */
        }

        // get body index frame data

        if (SUCCEEDED(hr))
        {
            hr = bodyIndexFrame->get_FrameDescription(&bodyIndexFrameDescription);
        }

        if (SUCCEEDED(hr))
        {
            hr = bodyIndexFrameDescription->get_Width(&nBodyIndexWidth);
        }

        if (SUCCEEDED(hr))
        {
            hr = bodyIndexFrameDescription->get_Height(&nBodyIndexHeight);
        }

        if (SUCCEEDED(hr))
        {
            hr = bodyIndexFrame->AccessUnderlyingBuffer(&nBodyIndexBufferSize, &pBodyIndexBuffer);
        }

        if (SUCCEEDED(hr))
        {
            /*
            ProcessFrame(nDepthTime, pDepthBuffer, nDepthWidth, nDepthHeight,
                         pColorBuffer, nColorWidth, nColorHeight,
                         pBodyIndexBuffer, nBodyIndexWidth, nBodyIndexHeight);
            */
        }

        SafeRelease(&depthFrameDescription);
        SafeRelease(&colorFrameDescription);
        SafeRelease(&bodyIndexFrameDescription);

        // get body frame data
        IBody* ppBodies[BODY_COUNT] = {0};

        if (SUCCEEDED(hr))
        {
            hr = bodyFrame->GetAndRefreshBodyData(BODY_COUNT, ppBodies);
        }

        if (SUCCEEDED(hr))
        {
            //ProcessBody(nTime, BODY_COUNT, ppBodies);
            for (int i = 0; i < BODY_COUNT; ++i)
            {
                IBody* pBody = ppBodies[i];
                if (pBody)
                {
                    BOOLEAN bTracked = false;
                    hr = pBody->get_IsTracked(&bTracked);

                    if (SUCCEEDED(hr) && bTracked)
                    {
                        Joint joints[JointType_Count];
                        JointOrientation jointOrientations[JointType_Count];

                        // TODO(JK#9#2017-06-07): use kinect hand state for something useful
                        /*
                        HandState leftHandState = HandState_Unknown;
                        HandState rightHandState = HandState_Unknown;

                        pBody->get_HandLeftState(&leftHandState);
                        pBody->get_HandRightState(&rightHandState);
                        */

                        hr = pBody->GetJoints(JointType_Count, joints);
                        // TODO(JK#3#2017-06-07): get more kinect streams and incorporate them in the mocap process
                        hr = pBody->GetJointOrientations(JointType_Count, jointOrientations);

                        if (SUCCEEDED(hr))
                        {
                            for (size_t i = 0; i < JointType_Count; ++i)
                            {
                                // handle setting of id for sensor node (and data) (solved by setting IP + id as name)
                                std::string name = "K-";
                                name += getJointName(i);

                                //wxString msg;
                                //msg << name << _(" - ") << jointOrientations[i].Orientation.w << _(" ") << jointOrientations[i].Orientation.x << _(" ") << jointOrientations[i].Orientation.y << _(" ") << jointOrientations[i].Orientation.z;
                                //wxLogDebug(msg);

                                Vector3 posJoint = Vector3(joints[i].Position.X, joints[i].Position.Y, joints[i].Position.Z);
                                Vector3 posParent;

                                if (i == JointType_SpineBase)
                                {
                                    // the root bone has no parent, so it gets the same orientation as JointType_SpineMid,
                                    // i.e. the root now has the same orientation as the lower body
                                    posParent = posJoint;
                                    posJoint = Vector3(joints[JointType_SpineMid].Position.X, joints[JointType_SpineMid].Position.Y, joints[JointType_SpineMid].Position.Z);
                                }
                                else
                                {
                                    posParent = Vector3(joints[getJointParentId(i)].Position.X, joints[getJointParentId(i)].Position.Y, joints[getJointParentId(i)].Position.Z);
                                }

                                Vector3 dir = posJoint - posParent;
                                Quaternion orientationFromPosition(Vector3(1.0f, 0.0f, 0.0f), dir);


                                Quaternion orientationFromKinect(jointOrientations[i].Orientation.w, jointOrientations[i].Orientation.x, jointOrientations[i].Orientation.y, jointOrientations[i].Orientation.z);

                                // kinect bones always rotate around y-axis (i.e. their direction is the positive y-axis), so as in this case
                                // the bones always rotate around the positive x-axis (i.e. their direction is the positive x-axis), the kinect
                                // quaternions have to be mapped to the local coordinate system.
                                Quaternion coordinateMapping = Quaternion(Vector3(0.0, 0.0, 1.0), M_PI*90.0/180.0) * Quaternion(Vector3(1.0, 0.0, 0.0), M_PI*90.0/180.0);

                                orientationFromKinect = orientationFromKinect * coordinateMapping;
                                orientationFromKinect.normalize();

                                SensorDataOrientation data;

                                data.setType(RGBD);
                                data.setTimestamp(static_cast<unsigned int>(receiveTime - _startTime));
                                data.setReceiveTime(receiveTime);

                                // toggle comments to either get kinect orientations or orientations from joint positions
                                data.setOrientation(orientationFromKinect);
                                // data.setOrientation(orientationFromPosition);

                                theSensorManager.updateSensor(name, &data);
                            }
                        }
                    }
                }
            }
        }

        for (int i = 0; i < BODY_COUNT; ++i)
        {
            SafeRelease(&ppBodies[i]);
        }
    }
    else
    {
        wxLogDebug(_("error getting body data"));
        return false;
    }

    SafeRelease(&depthFrame);
    SafeRelease(&colorFrame);
    SafeRelease(&bodyIndexFrame);
    SafeRelease(&bodyFrame);
    SafeRelease(&multiSourceFrame);

    return true;
}


bool ReceiverKinect::connect()
{
    HRESULT hr;

    hr = GetDefaultKinectSensor(&_kinectSensor);
    if (FAILED(hr))
    {
        wxLogDebug(_("Failed to access kinect interface"));
        return false;
    }

    if (_kinectSensor)
    {
        // Initialize the Kinect and get coordinate mapper and the body reader
        IBodyFrameSource* bodyFrameSource = NULL;

        hr = _kinectSensor->Open();

        if (SUCCEEDED(hr))
        {
            hr = _kinectSensor->get_CoordinateMapper(&_coordinateMapper);
        }


        if (SUCCEEDED(hr))
        {
            hr = _kinectSensor->get_BodyFrameSource(&bodyFrameSource);
        }

        if (SUCCEEDED(hr))
        {
            hr = bodyFrameSource->OpenReader(&_bodyFrameReader);
        }

        if (_bodyFrameReader == nullptr)
        {
            wxLogDebug(_("error getting body frame reader"));
        }

        SafeRelease(&bodyFrameSource);


        if (SUCCEEDED(hr))
        {
            hr = _kinectSensor->OpenMultiSourceFrameReader(FrameSourceTypes::FrameSourceTypes_Depth | FrameSourceTypes::FrameSourceTypes_Color
                                                           | FrameSourceTypes::FrameSourceTypes_BodyIndex | FrameSourceTypes::FrameSourceTypes_Body, &_multiSourceFrameReader);
        }
    }

    if (!_kinectSensor || FAILED(hr))
    {
        wxLogDebug(_("No ready Kinect found!"));
        return false;
    }

    BOOLEAN available = false;
    // WCHAR buffer[16] = {0};
    // _kinectSensor->get_UniqueKinectId(16, buffer);
    _kinectSensor->get_IsAvailable(&available);
    /*available = false;
    _kinectSensor->get_IsAvailable(&available);
    available = false;
    _kinectSensor->get_IsAvailable(&available);

    ComPtr<IIsAvailableChangedEventArgs> args;*/
    /*
    IIsAvailableChangedEventArgs* data = new IIsAvailableChangedEventArgs();
    WAITABLE_HANDLE handle;
    _kinectSensor->SubscribeIsAvailableChanged(&handle);
    _kinectSensor->GetIsAvailableChangedEventData(handle, &data);

    data->get_IsAvailable(&available);

    _kinectSensor->UnsubscribeIsAvailableChanged(handle);
    data->Release();
    */

    if (!available)
    {
        wxLogDebug(_("Kinect not available"));
        return false;
    }
    return true;
}

void ReceiverKinect::disconnect()
{
    // done with body frame reader
    SafeRelease(&_bodyFrameReader);

    // done with coordinate mapper
    SafeRelease(&_coordinateMapper);

    // close the Kinect Sensor
    if (_kinectSensor)
    {
        _kinectSensor->Close();
    }

    SafeRelease(&_kinectSensor);
}

bool ReceiverKinect::isConnected()
{
    if (_kinectSensor == nullptr)
    {
        return false;
    }

    BOOLEAN val = false;
    _kinectSensor->get_IsOpen(&val);
    if (!val)
    {
        return false;
    }
    _kinectSensor->get_IsAvailable(&val);

    return val;
}

std::string ReceiverKinect::getJointName(unsigned int id) const
{
    std::string name;
    switch (id)
    {
        case JointType_SpineBase:
            name = "SpineBase";
            break;

        case JointType_SpineMid:
            name = "SpineMid";
            break;

        case JointType_Neck:
            name = "Neck";
            break;

        case JointType_Head:
            name = "Head";
            break;

        case JointType_ShoulderLeft:
            name = "ShoulderLeft";
            break;

        case JointType_ElbowLeft:
            name = "ElbowLeft";
            break;

        case JointType_WristLeft:
            name = "WristLeft";
            break;

        case JointType_HandLeft:
            name = "HandLeft";
            break;

        case JointType_ShoulderRight:
            name = "ShoulderRight";
            break;

        case JointType_ElbowRight:
            name = "ElbowRight";
            break;

        case JointType_WristRight:
            name = "WristRight";
            break;

        case JointType_HandRight:
            name = "HandRight";
            break;

        case JointType_HipLeft:
            name = "HipLeft";
            break;

        case JointType_KneeLeft:
            name = "KneeLeft";
            break;

        case JointType_AnkleLeft:
            name = "AnkleLeft";
            break;

        case JointType_FootLeft:
            name = "FootLeft";
            break;

        case JointType_HipRight:
            name = "HipRight";
            break;

        case JointType_KneeRight:
            name = "KneeRight";
            break;

        case JointType_AnkleRight:
            name = "AnkleRight";
            break;

        case JointType_FootRight:
            name = "FootRight";
            break;

        case JointType_SpineShoulder:
            name = "SpineShoulder";
            break;

        case JointType_HandTipLeft:
            name = "HandTipLeft";
            break;

        case JointType_ThumbLeft:
            name = "ThumbLeft";
            break;

        case JointType_HandTipRight:
            name = "HandTipRight";
            break;

        case JointType_ThumbRight:
            name = "ThumbRight";
            break;


        default:
            break;
    }
    return name;
}

int ReceiverKinect::getJointParentId(unsigned int id) const
{
    int parentId = -1;
    switch (id)
    {
        case JointType_SpineBase:
            parentId = -1;
            break;

        case JointType_SpineMid:
            parentId = JointType_SpineBase;
            break;

        case JointType_Neck:
            parentId = JointType_SpineShoulder;
            break;

        case JointType_Head:
            parentId = JointType_Neck;
            break;

        case JointType_ShoulderLeft:
            parentId = JointType_SpineShoulder;
            break;

        case JointType_ElbowLeft:
            parentId = JointType_ShoulderLeft;
            break;

        case JointType_WristLeft:
            parentId = JointType_ElbowLeft;
            break;

        case JointType_HandLeft:
            parentId = JointType_WristLeft;
            break;

        case JointType_ShoulderRight:
            parentId = JointType_SpineShoulder;
            break;

        case JointType_ElbowRight:
            parentId = JointType_ShoulderRight;
            break;

        case JointType_WristRight:
            parentId = JointType_ElbowRight;
            break;

        case JointType_HandRight:
            parentId = JointType_WristRight;
            break;

        case JointType_HipLeft:
            parentId = JointType_SpineBase;
            break;

        case JointType_KneeLeft:
            parentId = JointType_HipLeft;
            break;

        case JointType_AnkleLeft:
            parentId = JointType_KneeLeft;
            break;

        case JointType_FootLeft:
            parentId = JointType_AnkleLeft;
            break;

        case JointType_HipRight:
            parentId = JointType_SpineBase;
            break;

        case JointType_KneeRight:
            parentId = JointType_HipRight;
            break;

        case JointType_AnkleRight:
            parentId = JointType_KneeRight;
            break;

        case JointType_FootRight:
            parentId = JointType_AnkleRight;
            break;

        case JointType_SpineShoulder:
            parentId = JointType_SpineMid;
            break;

        case JointType_HandTipLeft:
            parentId = JointType_HandLeft;
            break;

        case JointType_ThumbLeft:
            parentId = JointType_HandLeft;
            break;

        case JointType_HandTipRight:
            parentId = JointType_HandRight;
            break;

        case JointType_ThumbRight:
            parentId = JointType_HandRight;
            break;

        default:
            break;
    }
    return parentId;
}

const Bone ReceiverKinect::getJointDefaultBoneData(unsigned int id) const
{
    Bone boneData;
    switch (id)
    {
        case JointType_SpineBase:
            boneData.setName("SpineBase");
            boneData.setStartPos(0.0f, 0.999f, 0.0f);
            boneData.setEndPos(0.0f, 1.0f, 0.0f);
            boneData.setLength(0.001f);
            break;

        case JointType_SpineMid:
            boneData.setName("SpineMid");
            boneData.setEndPos(0.0f, 1.25f, 0.0f);
            boneData.setLength(0.25f);
            break;

        case JointType_Neck:
            boneData.setName("Neck");
            boneData.setEndPos(0.0f, 1.6f, 0.0f);
            boneData.setLength(0.1f);
            break;

        case JointType_Head:
            boneData.setName("Head");
            boneData.setEndPos(0.0f, 1.8f, 0.0f);
            boneData.setLength(0.2f);
            break;

        case JointType_ShoulderLeft:
            boneData.setName("ShoulderLeft");
            boneData.setEndPos(-0.2f, 1.5f, 0.0f);
            boneData.setLength(0.2f);
            break;

        case JointType_ElbowLeft:
            boneData.setName("ElbowLeft");
            boneData.setEndPos(-0.55f, 1.5f, 0.0f);
            boneData.setLength(0.35f);
            break;

        case JointType_WristLeft:
            boneData.setName("WristLeft");
            boneData.setEndPos(-0.85f, 1.5f, 0.0f);
            boneData.setLength(0.3f);
            break;

        case JointType_HandLeft:
            boneData.setName("HandLeft");
            boneData.setEndPos(-0.95f, 1.5f, 0.0f);
            boneData.setLength(0.1f);
            break;

        case JointType_ShoulderRight:
            boneData.setName("ShoulderRight");
            boneData.setEndPos(0.2f, 1.5f, 0.0f);
            boneData.setLength(0.2f);
            break;

        case JointType_ElbowRight:
            boneData.setName("ElbowRight");
            boneData.setEndPos(0.55f, 1.5f, 0.0f);
            boneData.setLength(0.35f);
            break;

        case JointType_WristRight:
            boneData.setName("WristRight");
            boneData.setEndPos(0.85f, 1.5f, 0.0f);
            boneData.setLength(0.3f);
            break;

        case JointType_HandRight:
            boneData.setName("HandRight");
            boneData.setEndPos(0.95f, 1.5f, 0.0f);
            boneData.setLength(0.1f);
            break;

        case JointType_HipLeft:
            boneData.setName("HipLeft");
            boneData.setEndPos(-0.2f, 1.0f, 0.0f);
            boneData.setLength(0.2f);
            break;

        case JointType_KneeLeft:
            boneData.setName("KneeLeft");
            boneData.setEndPos(-0.2f, 0.5f, 0.0f);
            boneData.setLength(0.5f);
            break;

        case JointType_AnkleLeft:
            boneData.setName("AnkleLeft");
            boneData.setEndPos(-0.2f, 0.05f, 0.0f);
            boneData.setLength(0.45f);
            break;

        case JointType_FootLeft:
            boneData.setName("FootLeft");
            boneData.setEndPos(-0.2f, 0.05f, 0.25f);
            boneData.setLength(0.25f);
            break;

        case JointType_HipRight:
            boneData.setName("HipRight");
            boneData.setEndPos(0.2f, 1.0f, 0.0f);
            boneData.setLength(0.2f);
            break;

        case JointType_KneeRight:
            boneData.setName("KneeRight");
            boneData.setEndPos(0.2f, 0.5f, 0.0f);
            boneData.setLength(0.5f);
            break;

        case JointType_AnkleRight:
            boneData.setName("AnkleRight");
            boneData.setEndPos(0.2f, 0.05f, 0.0f);
            boneData.setLength(0.45f);
            break;

        case JointType_FootRight:
            boneData.setName("FootRight");
            boneData.setEndPos(0.2f, 0.05f, 0.25f);
            boneData.setLength(0.25f);
            break;

        case JointType_SpineShoulder:
            boneData.setName("SpineShoulder");
            boneData.setEndPos(0.0f, 1.5f, 0.0f);
            boneData.setLength(0.25f);
            break;

        case JointType_HandTipLeft:
            boneData.setName("HandTipLeft");
            boneData.setEndPos(-1.05f, 1.5f, 0.0f);
            boneData.setLength(0.1f);
            break;

        case JointType_ThumbLeft:
            boneData.setName("ThumbLeft");
            boneData.setEndPos(-0.95f, 1.57f, 0.0f);
            boneData.setLength(0.07f);
            break;

        case JointType_HandTipRight:
            boneData.setName("HandTipRight");
            boneData.setEndPos(1.05f, 1.5f, 0.0f);
            boneData.setLength(0.1f);
            break;

        case JointType_ThumbRight:
            boneData.setName("ThumbRight");
            boneData.setEndPos(0.95f, 1.57f, 0.0f);
            boneData.setLength(0.07f);
            break;

        default:
            break;
    }
    return boneData;
}

void ReceiverKinect::createKinectSkeleton()
{
    _kinectSkeleton.clear();

    Bone boneData;

    for (size_t i = 0; i < JointType_Count; ++i)
    {
        boneData = getJointDefaultBoneData(i);
        _kinectSkeleton.createBone(boneData, getJointParentId(i));
    }

    for (size_t i = 0; i < JointType_Count; ++i)
    {
        // reparent all bones, as due to the inconsistent enumeration of the kinect joints
        // (shoulder joint is created after its children), the skeleton is ill-formed.
        _kinectSkeleton.reparent(i, getJointParentId(i), true);
    }
    _kinectSkeleton.update();
    _kinectSkeleton.setCurrentAsDefault();
}


