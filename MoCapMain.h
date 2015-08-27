/*Copyright (c) 2014, Jochen KempfleAll rights reserved.Redistribution and use in source and binary forms, with or without modification,are permitted provided that the following conditions are met:1. Redistributions of source code must retain the above copyright notice,this list of conditions and the following disclaimer.2. Redistributions in binary form must reproduce the above copyright notice,this list of conditions and the following disclaimer in the documentation and/orother materials provided with the distribution.THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" ANDANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIEDWARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITYOF SUCH DAMAGE.*/
#ifndef MOCAPMAIN_H
#define MOCAPMAIN_H

//(*Headers(MoCapFrame)
#include <wx/sizer.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/statusbr.h>
//*)#include "wx/socket.h"#include "wx/timer.h"//wxDECLARE_EVENT(UpdateEvent, wxEvent);

class MoCapFrame: public wxFrame
{
    public:

        MoCapFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~MoCapFrame();        void SensorDetailMode();        void ViewMode();

    private:

        //(*Handlers(MoCapFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnGLCanvas1LeftDown(wxMouseEvent& event);
        void OnButtonSensorDetailClick(wxCommandEvent& event);
        void OnButtonVisualClick(wxCommandEvent& event);
        void OnButtonConnectClick(wxCommandEvent& event);
        void OnButtonDisconnectClick(wxCommandEvent& event);
        void OnButtonCalibrateClick(wxCommandEvent& event);
        void OnButtonStartClick(wxCommandEvent& event);
        void OnButtonStopClick(wxCommandEvent& event);
        void OnButtonAutoAssignClick(wxCommandEvent& event);
        void OnButtonFullScreenClick(wxCommandEvent& event);
        //*)        void OnPopupClick(wxCommandEvent &event);
        // socket event handler        void OnSocketEvent(wxSocketEvent& event);        // timer event handler        void OnTimerEvent(wxTimerEvent& event);
        //(*Identifiers(MoCapFrame)
        static const long ID_BUTTONSENSORDETAIL;
        static const long ID_BUTTONVISUAL;
        static const long ID_BUTTONFULLSCREEN;
        static const long ID_BUTTONSTART;
        static const long ID_BUTTONSTOP;
        static const long ID_BUTTONAUTOASSIGN;
        static const long ID_BUTTONCALIBRATE;
        static const long ID_BUTTONCONNECT;
        static const long ID_BUTTONDISCONNECT;
        static const long ID_DATAPANEL;
        static const long ID_MAINPANEL;
        static const long idMenuQuit;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        //*)
        static const long ID_VIEWPANEL;        static const long ID_SENSORDETAILPANEL;        static const long ID_SOCKET;        static const long ID_TIMER;

        //(*Declarations(MoCapFrame)
        wxBoxSizer* MainSizer;
        wxBoxSizer* FrameSizer;
        wxPanel* DataPanel;
        wxBoxSizer* DataPanelSizer;
        wxStatusBar* StatusBar1;
        wxButton* ButtonDisconnect;
        wxPanel* MainPanel;
        wxButton* ButtonConnect;
        wxButton* ButtonSensorDetail;
        wxButton* ButtonCalibrate;
        wxButton* ButtonAutoAssign;
        wxButton* ButtonStart;
        wxButton* ButtonStop;
        wxButton* ButtonFullScreen;
        wxButton* ButtonVisual;
        //*)        wxIPV4address _addressLocal;        wxIPV4address _addressPeer;        wxDatagramSocket* _socket;        wxLogWindow* _logger;        wxTimer* _timer;        int _counter;        bool _simulationStarted;        bool _autoAssignStarted;        int _autoAssignBoneId;

        DECLARE_EVENT_TABLE()
};

#endif // MOCAPMAIN_H