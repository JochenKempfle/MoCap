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


#include "wx_pch.h"
#include "MoCapMain.h"
#include <wx/msgdlg.h>
#include <wx/utils.h>
#include <wx/time.h>
#include "ViewPanel.h"
#include "SensorDetailPanel.h"
#include "SkeletonCreatorPanel.h"
#include "PostProcessPanel.h"
#include "SensorManager.h"
#include "MoCapManager.h"
#include "ConnectionDialog.h"
#include "SensorData.h"
#include "CustomEvents.h"

//(*InternalHeaders(MoCapFrame)
#include <wx/intl.h>
#include <wx/string.h>
//*)


wxDEFINE_EVENT(UpdateEvent, wxEvent);

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(MoCapFrame)
const long MoCapFrame::ID_BUTTONSKELETON = wxNewId();
const long MoCapFrame::ID_BUTTONSENSORDETAIL = wxNewId();
const long MoCapFrame::ID_BUTTONVISUAL = wxNewId();
const long MoCapFrame::ID_BUTTONPOSTPROCESS = wxNewId();
const long MoCapFrame::ID_BUTTONFULLSCREEN = wxNewId();
const long MoCapFrame::ID_PANEL1 = wxNewId();
const long MoCapFrame::ID_DATAPANEL = wxNewId();
const long MoCapFrame::ID_BUTTONCONNECTION = wxNewId();
const long MoCapFrame::ID_PANEL2 = wxNewId();
const long MoCapFrame::ID_MAINPANEL = wxNewId();
const long MoCapFrame::idMenuQuit = wxNewId();
const long MoCapFrame::idMenuAbout = wxNewId();
//*)

const long MoCapFrame::ID_VIEWPANEL = wxNewId();
const long MoCapFrame::ID_SENSORDETAILPANEL = wxNewId();
const long MoCapFrame::ID_SKELETONCREATORPANEL = wxNewId();

const long MoCapFrame::ID_SOCKET = wxNewId();
const long MoCapFrame::ID_TIMER = wxNewId();

BEGIN_EVENT_TABLE(MoCapFrame,wxFrame)
    //(*EventTable(MoCapFrame)
    //*)
END_EVENT_TABLE()

MoCapFrame::MoCapFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(MoCapFrame)
    wxBoxSizer* BoxSizer4;
    wxMenuItem* MenuItem2;
    wxMenuItem* MenuItem1;
    wxBoxSizer* BoxSizer2;
    wxMenu* Menu1;
    wxBoxSizer* BoxSizer1;
    wxMenuBar* MenuBar1;
    wxBoxSizer* BoxSizer3;
    wxMenu* Menu2;

    Create(parent, wxID_ANY, _("Motion Capturing"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    SetMinSize(wxSize(-1,-1));
    FrameSizer = new wxBoxSizer(wxHORIZONTAL);
    MainPanel = new wxPanel(this, ID_MAINPANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_MAINPANEL"));
    MainSizer = new wxBoxSizer(wxVERTICAL);
    BoxSizer2 = new wxBoxSizer(wxVERTICAL);
    Panel1 = new wxPanel(MainPanel, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    ButtonSkeleton = new wxButton(Panel1, ID_BUTTONSKELETON, _("Skeleton"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONSKELETON"));
    BoxSizer1->Add(ButtonSkeleton, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ButtonSensorDetail = new wxButton(Panel1, ID_BUTTONSENSORDETAIL, _("Sensor Info"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONSENSORDETAIL"));
    BoxSizer1->Add(ButtonSensorDetail, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ButtonVisual = new wxButton(Panel1, ID_BUTTONVISUAL, _("Visual"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONVISUAL"));
    ButtonVisual->SetFocus();
    BoxSizer1->Add(ButtonVisual, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ButtonPostProcess = new wxButton(Panel1, ID_BUTTONPOSTPROCESS, _("Post Process"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONPOSTPROCESS"));
    BoxSizer1->Add(ButtonPostProcess, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer1->Add(300,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxFIXED_MINSIZE, 5);
    ButtonFullScreen = new wxButton(Panel1, ID_BUTTONFULLSCREEN, _("Full Screen"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONFULLSCREEN"));
    BoxSizer1->Add(ButtonFullScreen, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Panel1->SetSizer(BoxSizer1);
    BoxSizer1->Fit(Panel1);
    BoxSizer1->SetSizeHints(Panel1);
    BoxSizer2->Add(Panel1, 0, wxEXPAND, 5);
    MainSizer->Add(BoxSizer2, 0, wxEXPAND, 5);
    DataPanelSizer = new wxBoxSizer(wxHORIZONTAL);
    DataPanel = new wxPanel(MainPanel, ID_DATAPANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_DATAPANEL"));
    DataPanelSizer->Add(DataPanel, 1, wxEXPAND, 5);
    MainSizer->Add(DataPanelSizer, 1, wxEXPAND, 5);
    BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    Panel2 = new wxPanel(MainPanel, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL, _T("ID_PANEL2"));
    BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
    ButtonConnection = new wxButton(Panel2, ID_BUTTONCONNECTION, _("Connection"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONCONNECTION"));
    BoxSizer4->Add(ButtonConnection, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Panel2->SetSizer(BoxSizer4);
    BoxSizer4->Fit(Panel2);
    BoxSizer4->SetSizeHints(Panel2);
    BoxSizer3->Add(Panel2, 1, wxEXPAND, 5);
    MainSizer->Add(BoxSizer3, 0, wxEXPAND, 5);
    MainPanel->SetSizer(MainSizer);
    MainSizer->Fit(MainPanel);
    MainSizer->SetSizeHints(MainPanel);
    FrameSizer->Add(MainPanel, 1, wxEXPAND, 5);
    SetSizer(FrameSizer);
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    FrameSizer->Fit(this);
    FrameSizer->SetSizeHints(this);
    Center();

    Connect(ID_BUTTONSKELETON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MoCapFrame::OnButtonSkeletonClick);
    Connect(ID_BUTTONSENSORDETAIL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MoCapFrame::OnButtonSensorDetailClick);
    Connect(ID_BUTTONVISUAL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MoCapFrame::OnButtonVisualClick);
    Connect(ID_BUTTONPOSTPROCESS,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MoCapFrame::OnButtonPostProcessClick);
    Connect(ID_BUTTONFULLSCREEN,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MoCapFrame::OnButtonFullScreenClick);
    Connect(ID_BUTTONCONNECTION,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MoCapFrame::OnButtonConnectionClick);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&MoCapFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&MoCapFrame::OnAbout);
    //*)
    Connect(ID_SOCKET, wxEVT_SOCKET, wxSocketEventHandler(MoCapFrame::OnSocketEvent));
    Connect(ID_TIMER, wxEVT_TIMER, wxTimerEventHandler(MoCapFrame::OnTimerEvent));

    Maximize(true);

    _addressPeer.BroadcastAddress();
    _addressPeer.Service(5050);
    _addressLocal.Hostname(_("192.168.0.102"));//wxGetFullHostName());
    _addressLocal.Service(5050);

    _socket = nullptr;

    // _logger = new wxLogWindow(this, _("Log"));

    // TODO(JK#3#): maybe directly start the timer, allows GUI to react to whatever even when not connected
    _timer = new wxTimer(this, ID_TIMER);
    _counter = 0;

    Freeze();
    DataPanel->Destroy();
    DataPanel = new ViewPanel(MainPanel, ID_DATAPANEL);
    DataPanelSizer->Add(DataPanel, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    DataPanelSizer->Fit(DataPanel);
    DataPanelSizer->SetSizeHints(DataPanel);
    MainSizer->Fit(MainPanel);
    MainSizer->SetSizeHints(MainPanel);
    FrameSizer->Fit(this);
    FrameSizer->SetSizeHints(this);
    Thaw();
    Refresh();

    theMoCapManager.setState(RECORD);
}

MoCapFrame::~MoCapFrame()
{
    //(*Destroy(MoCapFrame)
    //*)
    _timer->Stop();
    if (_socket != nullptr)
    {
        _socket->Destroy();
        _socket = nullptr;
    }
}

void MoCapFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void MoCapFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to the motion capturing GUI"));
}


void MoCapFrame::OnSocketEvent(wxSocketEvent& event)
{
    // get time point at which the data arrived
    wxLongLong time = wxGetUTCTimeMillis();
    uint64_t receiveTime = (uint64_t(time.GetHi()) << 32) + time.GetLo();
    switch (event.GetSocketEvent())
    {
        case wxSOCKET_INPUT:
            break;
        case wxSOCKET_LOST:
            wxLogDebug(_("Socket lost. Try to reconnect."));

            if (_socket != nullptr)
            {
                _socket->Destroy();
                _socket = nullptr;
            }
            return;

        case wxSOCKET_CONNECTION:
            wxLogDebug(_("New incoming connection: IP ..."));
            return;

        case wxSOCKET_OUTPUT:
            wxLogDebug(_("Socket output: ..."));
            return;

        default:
            wxLogDebug(_("Unexpected socket event. Returning from event handler"));
            return;
    }

    if (!_socket->IsOk())
    {
        wxMessageBox(_("Error in Event: Socket is not available!"));
        return;
    }

    SensorRawData data;
    // 36 bytes is the maximum expected packet size
    unsigned char buffer[36];
    wxIPV4address sensorAddress = _addressPeer;

    _socket->RecvFrom(sensorAddress, buffer, sizeof(buffer));

    unsigned int bytesReceived = _socket->LastCount();

    if (bytesReceived == 0)
    {
        wxLogDebug(_("ERROR in Event: failed to receive data"));
        return;
    }
    unsigned char* pBuffer = buffer;

    if (bytesReceived >= 24 && bytesReceived <= 36)
    {
        data.receiveTime = receiveTime;
        if (bytesReceived == 24 || bytesReceived == 36)
        {
            data.id = *(reinterpret_cast<uint32_t*>(pBuffer));
            pBuffer += sizeof(uint32_t);
        }
        data.timestamp = *(reinterpret_cast<uint32_t*>(pBuffer));
        pBuffer += sizeof(uint32_t);
        data.rotation[0] = *reinterpret_cast<float*>(pBuffer);
        pBuffer += sizeof(float);
        data.rotation[1] = *reinterpret_cast<float*>(pBuffer);
        pBuffer += sizeof(float);
        data.rotation[2] = *reinterpret_cast<float*>(pBuffer);
        pBuffer += sizeof(float);
        data.rotation[3] = *reinterpret_cast<float*>(pBuffer);
        /*
        if (bytesReceived == 32 || bytesReceived == 36)
        {
            pBuffer += sizeof(float);
            data.position[0] = *reinterpret_cast<float*>(pBuffer);
            pBuffer += sizeof(float);
            data.position[1] = *reinterpret_cast<float*>(pBuffer);
            pBuffer += sizeof(float);
            data.position[2] = *reinterpret_cast<float*>(pBuffer);
        }*/
    }

    if (_socket->LastError() != wxSOCKET_NOERROR)
    {
        wxMessageBox("ERROR in Event: socket returned error");
        return;
    }
/*
    wxString msg;

    msg << _("Received: ") << _socket->LastCount() << _(" Bytes.\n");

    if (_socket->LastCount() == sizeof(SensorData))
    {
        msg << _("Timestamp: ") << data.timeStamp << _("\nRotation: ");
        msg << data.rotation[0] << _(" ") << data.rotation[1] << _(" ") << data.rotation[2] << _(" ") << data.rotation[3];
        msg << _("\nPosition: ") << data.position[0] << _(" ") << data.position[1] << _(" ") << data.position[2] << _("\n");

        msg << _("from ");
        //msg << _("Received: \"") << wxString::From8BitData(buf, _socket->LastCount() - 1) << _("\" from ");
        msg << sensorAddress.IPAddress() << _(" : ") << sensorAddress.Service();
        msg << _("\nLocal is: ") << _addressLocal.IPAddress() << _(" : ") << _addressLocal.Service();
        //msg << _("\nPeer is: ") << _addressPeer.IPAddress() << _(" : ") << _addressPeer.Service() << _("\n");
    }
    else if (_socket->LastCount() == 1)
    {
        msg << _("--- dummy byte to keep connection stable ---\n");
    }
    else
    {
        msg << _("---- RECEIVED UNKNOWN DATA! ----\n\n");
    }

    wxLogDebug(msg);
*/

    // TODO(JK#2#): handle setting of id for sensor node (and data)
    wxString name = sensorAddress.IPAddress() + _("-");
    name << data.id;

    if (theSensorManager.getSensorIdFromIP(name.ToStdString()) == -1)
    {
        Refresh();
    }

    if (bytesReceived >= 24 && bytesReceived <= 36)//== sizeof(SensorData))
    {
        theSensorManager.updateSensor(name.ToStdString(), data);
        // theSensorManager.updateSensor(sensorAddress.IPAddress().ToStdString(), data);
    }
}

void MoCapFrame::OnTimerEvent(wxTimerEvent& event)
{
    // send a dummy over the socket every second. Somehow this is needed to ensure we get socket events.
    ++_counter;
    if (_counter >= 40 && _socket != nullptr && _socket->IsOk())
    {
        _counter = 0;
        char dummy = 0;
        if (_socket->SendTo(_addressPeer, &dummy, sizeof(dummy)).LastCount() != sizeof(dummy))
        {
            wxLogDebug("ERROR: failed to send data");
            return;
        }
    }
    // send an update event to update the GUI
    wxCommandEvent updateEvent(UpdateEvent);
    wxPostEvent(DataPanel, updateEvent);
}

void MoCapFrame::OnButtonSensorDetailClick(wxCommandEvent& event)
{
    Freeze();
    DataPanel->Destroy();
    DataPanel = new SensorDetailPanel(MainPanel, ID_SENSORDETAILPANEL);
    DataPanelSizer->Add(DataPanel, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    DataPanelSizer->Layout();
    MainSizer->Layout();
    Thaw();
    Refresh();
    theMoCapManager.setState(SENSOR_INFO);
}

void MoCapFrame::OnButtonVisualClick(wxCommandEvent& event)
{
    Freeze();
    DataPanel->Destroy();
    DataPanel = new ViewPanel(MainPanel, ID_VIEWPANEL);
    DataPanelSizer->Add(DataPanel, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    DataPanelSizer->Layout();
    MainSizer->Layout();
    Thaw();
    Refresh();
    theMoCapManager.setState(RECORD);
    //theMoCapManager.setRenderStyle(HIGHLIGHT_SELECTED_BONE);
}


void MoCapFrame::OnButtonSkeletonClick(wxCommandEvent& event)
{
    Freeze();
    DataPanel->Destroy();
    DataPanel = new SkeletonCreatorPanel(MainPanel, ID_SKELETONCREATORPANEL);
    DataPanelSizer->Add(DataPanel, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    DataPanelSizer->Layout();
    MainSizer->Layout();
    Thaw();
    Refresh();
    theMoCapManager.setState(EDITOR);
    //theMoCapManager.setRenderStyle(DRAW_SPIN_ARROWS | HIGHLIGHT_SELECTED_BONE);
}

void MoCapFrame::OnButtonPostProcessClick(wxCommandEvent& event)
{
    Freeze();
    DataPanel->Destroy();
    DataPanel = new PostProcessPanel(MainPanel, ID_SKELETONCREATORPANEL);
    DataPanelSizer->Add(DataPanel, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    DataPanelSizer->Layout();
    MainSizer->Layout();
    Thaw();
    Refresh();
    theMoCapManager.setState(POST_PROCESS);
    //theMoCapManager.setRenderStyle(DRAW_SPIN_ARROWS | HIGHLIGHT_SELECTED_BONE);
}

void MoCapFrame::OnButtonFullScreenClick(wxCommandEvent& event)
{
    if (IsFullScreen())
    {
        ButtonFullScreen->SetLabel("Full Screen");
        ShowFullScreen(false, wxFULLSCREEN_ALL);
    }
    else
    {
        ButtonFullScreen->SetLabel("Normal Screen");
        ShowFullScreen(true, wxFULLSCREEN_ALL);
    }
}


void MoCapFrame::OnButtonConnectionClick(wxCommandEvent& event)
{
    ConnectionDialog* dialog = new ConnectionDialog(this);
    dialog->setIP(_addressLocal.IPAddress());
    dialog->setPort(_addressLocal.Service());
    if (dialog->ShowModal() == wxID_OK)
    {
        _addressLocal.Hostname(dialog->getIP());
        _addressLocal.Service(dialog->getPort());
        _addressPeer.BroadcastAddress();
        _addressPeer.Service(dialog->getPort());
        // check if for some reason there is already a socket allocated
        if (_socket != nullptr)
        {
            _socket->Destroy();
            _socket = nullptr;
        }
        // Create the socket
        _socket = new wxDatagramSocket(_addressLocal, wxSOCKET_BROADCAST | wxSOCKET_NOWAIT);
        if (!_socket->IsOk())
        {
            wxMessageBox(_("Failed to create UDP socket at ") + _addressLocal.IPAddress(), _("Error"), wxICON_ERROR);
            _socket->Destroy();
            _socket = nullptr;
            return;
        }
        // Setup the event handler and subscribe to most events
        _socket->SetEventHandler(*this, ID_SOCKET);
        _socket->SetNotify(wxSOCKET_CONNECTION_FLAG | wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
        _socket->Notify(true);
    }
    else
    {
        return;
    }

    dialog->Destroy();

    Freeze();
/*    ButtonConnect->Hide();
    ButtonAutoAssign->Show();
    ButtonStart->Show();
    ButtonDisconnect->Show();
    ButtonCalibrate->Show();*/
    MainSizer->Layout();
    Thaw();
    Refresh();
    _timer->Start(25);
}

/*
void MoCapFrame::OnButtonDisconnectClick(wxCommandEvent& event)
{
    _timer->Stop();

    if (_socket != nullptr)
    {
        _socket->Destroy();
        _socket = nullptr;
    }

    Freeze();
    ButtonDisconnect->Hide();
    ButtonCalibrate->Hide();
    ButtonStart->Hide();
    ButtonStop->Hide();
    ButtonAutoAssign->Hide();
    ButtonConnect->Show();
    MainSizer->Layout();
    Thaw();
    Refresh();
    theMoCapManager.resetSkeleton();
    // update panels
    theSensorManager.resetAllSensorStatesUpdated();
    wxCommandEvent updateEvent(UpdateEvent);
    wxPostEvent(DataPanel, updateEvent);
}


*/




