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


#ifndef KINECTHACK_H
#define KINECTHACK_H

// include KinectHack.h before including Kinect.h

// This header is a hack to get the Kinect SDK 2.0 compiled with other compilers than VisualStudio
// It effectively removes SAL annotations and declares the kinect enums. In the original Kinect.h
// file, the declaration of an enum comes after the typedef.


// TODO(JK#5#2017-05-24): maybe include <Kinect.h> in "Kinecthack.h"

#pragma once

#if __GNUC__ >=3
#pragma GCC system_header
#endif

/*#define __null*/ // << Conflicts with GCC internal type __null

#define _In_

#define _Out_
#define _Out_writes_z_(size)

#define _COM_Outptr_
#define _Out_range_(a, b)

#define _Out_writes_all_(size)

#define _Outptr_result_buffer_(n)

#define _Inout_updates_all_(size)

#define _Pre_equal_to_(size)

#define _In_reads_(size)


// safe release
template <class T>
void SafeRelease(T **ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = nullptr;
    }
}


// enum declarations

#ifndef _KinectCapabilities_
#define _KinectCapabilities_

enum _KinectCapabilities
    {
        KinectCapabilities_None	= 0,
        KinectCapabilities_Vision	= 0x1,
        KinectCapabilities_Audio	= 0x2,
        KinectCapabilities_Face	= 0x4,
        KinectCapabilities_Expressions	= 0x8,
        KinectCapabilities_Gamechat	= 0x10
    } ;

typedef enum _KinectCapabilities KinectCapabilities;

#endif // _KinectCapabilities_
#ifndef _FrameSourceTypes_
#define _FrameSourceTypes_

enum _FrameSourceTypes
    {
        FrameSourceTypes_None	= 0,
        FrameSourceTypes_Color	= 0x1,
        FrameSourceTypes_Infrared	= 0x2,
        FrameSourceTypes_LongExposureInfrared	= 0x4,
        FrameSourceTypes_Depth	= 0x8,
        FrameSourceTypes_BodyIndex	= 0x10,
        FrameSourceTypes_Body	= 0x20,
        FrameSourceTypes_Audio	= 0x40
    } ;

typedef enum _FrameSourceTypes FrameSourceTypes;

#endif // _FrameSourceTypes_
#ifndef _ColorImageFormat_
#define _ColorImageFormat_

enum _ColorImageFormat
    {
        ColorImageFormat_None	= 0,
        ColorImageFormat_Rgba	= 1,
        ColorImageFormat_Yuv	= 2,
        ColorImageFormat_Bgra	= 3,
        ColorImageFormat_Bayer	= 4,
        ColorImageFormat_Yuy2	= 5
    } ;

typedef enum _ColorImageFormat ColorImageFormat;

#endif // _ColorImageFormat_
#ifndef _HandState_
#define _HandState_

enum _HandState
    {
        HandState_Unknown	= 0,
        HandState_NotTracked	= 1,
        HandState_Open	= 2,
        HandState_Closed	= 3,
        HandState_Lasso	= 4
    } ;

typedef enum _HandState HandState;

#endif // _HandState_
#ifndef _Expression_
#define _Expression_

enum _Expression
    {
        Expression_Neutral	= 0,
        Expression_Happy	= 1,
        Expression_Count	= ( Expression_Happy + 1 )
    } ;

typedef enum _Expression Expression;

#endif // _Expression_
#ifndef _DetectionResult_
#define _DetectionResult_

enum _DetectionResult
    {
        DetectionResult_Unknown	= 0,
        DetectionResult_No	= 1,
        DetectionResult_Maybe	= 2,
        DetectionResult_Yes	= 3
    } ;

typedef enum _DetectionResult DetectionResult;

#endif // _DetectionResult_
#ifndef _TrackingConfidence_
#define _TrackingConfidence_

enum _TrackingConfidence
    {
        TrackingConfidence_Low	= 0,
        TrackingConfidence_High	= 1
    } ;

typedef enum _TrackingConfidence TrackingConfidence;

#endif // _TrackingConfidence_
#ifndef _Activity_
#define _Activity_

enum _Activity
    {
        Activity_EyeLeftClosed	= 0,
        Activity_EyeRightClosed	= 1,
        Activity_MouthOpen	= 2,
        Activity_MouthMoved	= 3,
        Activity_LookingAway	= 4,
        Activity_Count	= ( Activity_LookingAway + 1 )
    } ;

typedef enum _Activity Activity;

#endif // _Activity_
#ifndef _Appearance_
#define _Appearance_

enum _Appearance
    {
        Appearance_WearingGlasses	= 0,
        Appearance_Count	= ( Appearance_WearingGlasses + 1 )
    } ;

typedef enum _Appearance Appearance;

#endif // _Appearance_
#ifndef _JointType_
#define _JointType_

enum _JointType
    {
        JointType_SpineBase	= 0,
        JointType_SpineMid	= 1,
        JointType_Neck	= 2,
        JointType_Head	= 3,
        JointType_ShoulderLeft	= 4,
        JointType_ElbowLeft	= 5,
        JointType_WristLeft	= 6,
        JointType_HandLeft	= 7,
        JointType_ShoulderRight	= 8,
        JointType_ElbowRight	= 9,
        JointType_WristRight	= 10,
        JointType_HandRight	= 11,
        JointType_HipLeft	= 12,
        JointType_KneeLeft	= 13,
        JointType_AnkleLeft	= 14,
        JointType_FootLeft	= 15,
        JointType_HipRight	= 16,
        JointType_KneeRight	= 17,
        JointType_AnkleRight	= 18,
        JointType_FootRight	= 19,
        JointType_SpineShoulder	= 20,
        JointType_HandTipLeft	= 21,
        JointType_ThumbLeft	= 22,
        JointType_HandTipRight	= 23,
        JointType_ThumbRight	= 24,
        JointType_Count	= ( JointType_ThumbRight + 1 )
    } ;

typedef enum _JointType JointType;

#endif // _JointType_
#ifndef _TrackingState_
#define _TrackingState_

enum _TrackingState
    {
        TrackingState_NotTracked	= 0,
        TrackingState_Inferred	= 1,
        TrackingState_Tracked	= 2
    } ;

typedef enum _TrackingState TrackingState;

#endif // _TrackingState_
#ifndef _FrameEdges_
#define _FrameEdges_

enum _FrameEdges
    {
        FrameEdge_None	= 0,
        FrameEdge_Right	= 0x1,
        FrameEdge_Left	= 0x2,
        FrameEdge_Top	= 0x4,
        FrameEdge_Bottom	= 0x8
    } ;

typedef enum _FrameEdges FrameEdges;

#endif // _FrameEdges_
#ifndef _FrameCapturedStatus_
#define _FrameCapturedStatus_

enum _FrameCapturedStatus
    {
        FrameCapturedStatus_Unknown	= 0,
        FrameCapturedStatus_Queued	= 1,
        FrameCapturedStatus_Dropped	= 2
    } ;

typedef enum _FrameCapturedStatus FrameCapturedStatus;

#endif // _FrameCapturedStatus_
#ifndef _AudioBeamMode_
#define _AudioBeamMode_

enum _AudioBeamMode
    {
        AudioBeamMode_Automatic	= 0,
        AudioBeamMode_Manual	= 1
    } ;

typedef enum _AudioBeamMode AudioBeamMode;

#endif // _AudioBeamMode_
#ifndef _KinectAudioCalibrationState_
#define _KinectAudioCalibrationState_

enum _KinectAudioCalibrationState
    {
        KinectAudioCalibrationState_Unknown	= 0,
        KinectAudioCalibrationState_CalibrationRequired	= 1,
        KinectAudioCalibrationState_Calibrated	= 2
    } ;

typedef enum _KinectAudioCalibrationState KinectAudioCalibrationState;

#endif // _KinectAudioCalibrationState_


#ifndef _PointerDeviceType_
#define _PointerDeviceType_

enum _PointerDeviceType
    {
        PointerDeviceType_Touch	= 0,
        PointerDeviceType_Pen	= 1,
        PointerDeviceType_Mouse	= 2,
        PointerDeviceType_Kinect	= 3
    } ;

typedef enum _PointerDeviceType PointerDeviceType;

#endif // _PointerDeviceType_
#ifndef _HandType_
#define _HandType_

enum _HandType
    {
        HandType_NONE	= 0,
        HandType_LEFT	= ( HandType_NONE + 1 ) ,
        HandType_RIGHT	= ( HandType_LEFT + 1 )
    } ;

typedef enum _HandType HandType;

#endif // _HandType_
#ifndef _KinectHoldingState_
#define _KinectHoldingState_

enum _KinectHoldingState
    {
        KinectHoldingState_Started	= 0,
        KinectHoldingState_Completed	= 1,
        KinectHoldingState_Canceled	= 2
    } ;

typedef enum _KinectHoldingState KinectHoldingState;

#endif // _KinectHoldingState_
#ifndef _KinectGestureSettings_
#define _KinectGestureSettings_

enum _KinectGestureSettings
    {
        KinectGestureSettings_None	= 0,
        KinectGestureSettings_Tap	= 0x1,
        KinectGestureSettings_ManipulationTranslateX	= 0x40,
        KinectGestureSettings_ManipulationTranslateY	= 0x80,
        KinectGestureSettings_ManipulationTranslateRailsX	= 0x100,
        KinectGestureSettings_ManipulationTranslateRailsY	= 0x200,
        KinectGestureSettings_ManipulationScale	= 0x800,
        KinectGestureSettings_ManipulationTranslateInertia	= 0x1000,
        KinectGestureSettings_KinectHold	= 0x10000
    } ;

typedef enum _KinectGestureSettings KinectGestureSettings;

#endif // _KinectGestureSettings_
#ifndef _KinectInteractionMode_
#define _KinectInteractionMode_

enum _KinectInteractionMode
    {
        KinectInteractionMode_Normal	= 0,
        KinectInteractionMode_Off	= 1,
        KinectInteractionMode_Media	= 2
    } ;

typedef enum _KinectInteractionMode KinectInteractionMode;

#endif // _KinectInteractionMode_
#ifndef _KinectEngagementMode_
#define _KinectEngagementMode_

enum _KinectEngagementMode
    {
        KinectEngagementMode_None	= 0,
        KinectEngagementMode_SystemOnePerson	= 1,
        KinectEngagementMode_SystemTwoPerson	= 2,
        KinectEngagementMode_ManualOnePerson	= 3,
        KinectEngagementMode_ManualTwoPerson	= 4
    } ;

typedef enum _KinectEngagementMode KinectEngagementMode;

#endif // _KinectEngagementMode_

#endif // KINECTHACK_H
