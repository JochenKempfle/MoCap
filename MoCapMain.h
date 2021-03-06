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



#ifndef MOCAPMAIN_H
#define MOCAPMAIN_H

//(*Headers(MoCapFrame)
#include <wx/sizer.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/frame.h>
//*)

#include "wx/socket.h"
#include "wx/timer.h"
#include "ViewPanel.h"
#include "ReceiverBase.h"
#include "KinectHack.h"
#include <Kinect.h>


//wxDECLARE_EVENT(UpdateEvent, wxEvent);

class MoCapFrame: public wxFrame
{
    public:

        MoCapFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~MoCapFrame();

        void SensorDetailMode();
        void ViewMode();
        void startTimer(int ms) { _timer->Start(ms); }

        // TODO(JK#1#2017-05-31): isConnected no longer working - rewrite!
        bool isConnected() const;

        std::map<wxString, unsigned int> _receivedPackets;
        std::map<wxString, uint64_t> _receiveStartTime;

    private:

        //(*Handlers(MoCapFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnButtonSensorDetailClick(wxCommandEvent& event);
        void OnButtonVisualClick(wxCommandEvent& event);
        void OnButtonDisconnectClick(wxCommandEvent& event);
        void OnButtonFullScreenClick(wxCommandEvent& event);
        void OnButtonConnectionClick(wxCommandEvent& event);
        void OnButtonSkeletonClick(wxCommandEvent& event);
        void OnButtonPostProcessClick(wxCommandEvent& event);
        void OnKeyDown(wxKeyEvent& event);
        void OnMainPanelKeyDown(wxKeyEvent& event);
        void OnButtonMotionPlayerClick(wxCommandEvent& event);
        void OnButtonLogClick(wxCommandEvent& event);
        void OnButtonDataFlowClick(wxCommandEvent& event);
        //*)

        // socket event handler
        void OnSocketEvent(wxSocketEvent& event);
        // timer event handler
        void OnTimerEvent(wxTimerEvent& event);

        //(*Identifiers(MoCapFrame)
        static const long ID_BUTTONSKELETON;
        static const long ID_BUTTONSENSORDETAIL;
        static const long ID_BUTTONDATAFLOW;
        static const long ID_BUTTONVISUAL;
        static const long ID_BUTTONMOTIONPLAYER;
        static const long ID_BUTTONPOSTPROCESS;
        static const long ID_BUTTONFULLSCREEN;
        static const long ID_PANEL1;
        static const long ID_DATAPANEL;
        static const long ID_BUTTONLOG;
        static const long ID_BUTTONCONNECTION;
        static const long ID_BUTTONDISCONNECT;
        static const long ID_PANEL2;
        static const long ID_MAINPANEL;
        static const long idMenuQuit;
        static const long idMenuAbout;
        //*)

        static const long ID_VIEWPANEL;
        static const long ID_DATAFLOWPANEL;
        static const long ID_SENSORDETAILPANEL;
        static const long ID_SKELETONCREATORPANEL;
        static const long ID_MOTIONPLAYERPANEL;
        static const long ID_POSTPROCESSPANEL;
        static const long ID_SOCKET;
        static const long ID_TIMER;

        //(*Declarations(MoCapFrame)
        wxButton* ButtonSkeleton;
        wxBoxSizer* MainSizer;
        wxButton* ButtonLog;
        wxBoxSizer* FrameSizer;
        wxButton* ButtonConnection;
        wxButton* ButtonPostProcess;
        wxPanel* Panel1;
        wxPanel* DataPanel;
        wxBoxSizer* DataPanelSizer;
        wxButton* ButtonDisconnect;
        wxPanel* MainPanel;
        wxButton* ButtonDataFlow;
        wxButton* ButtonSensorDetail;
        wxPanel* Panel2;
        wxButton* ButtonFullScreen;
        wxButton* ButtonVisual;
        wxButton* ButtonMotionPlayer;
        //*)

        wxLogWindow* _logger;
        wxTimer* _timer;
        int _counter;

        // TODO(JK#2#2017-09-28): remove socket and IP addresses from MoCapMain (is solved in UDP receiver)
        wxIPV4address _addressPeer;
        wxIPV4address _addressLocal;
        wxDatagramSocket* _socket;


        std::vector<ReceiverBase*> _receivers;

        DECLARE_EVENT_TABLE()
};

#endif // MOCAPMAIN_H
