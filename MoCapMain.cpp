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
#include "DataFlowPanel.h"
#include "SensorDetailPanel.h"
#include "SkeletonCreatorPanel.h"
#include "MotionPlayerPanel.h"
#include "PostProcessPanel.h"
#include "SensorManager.h"
#include "MoCapManager.h"
#include "ConnectionDialog.h"
#include "ConnectionManagerDialog.h"
#include "SensorData.h"
#include "CustomEvents.h"

#include "ReceiverUDP.h"
#include "ReceiverKinect.h"
#include "ReceiverFile.h"

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
const long MoCapFrame::ID_BUTTONDATAFLOW = wxNewId();
const long MoCapFrame::ID_BUTTONVISUAL = wxNewId();
const long MoCapFrame::ID_BUTTONMOTIONPLAYER = wxNewId();
const long MoCapFrame::ID_BUTTONPOSTPROCESS = wxNewId();
const long MoCapFrame::ID_BUTTONFULLSCREEN = wxNewId();
const long MoCapFrame::ID_PANEL1 = wxNewId();
const long MoCapFrame::ID_DATAPANEL = wxNewId();
const long MoCapFrame::ID_BUTTONLOG = wxNewId();
const long MoCapFrame::ID_BUTTONCONNECTION = wxNewId();
const long MoCapFrame::ID_BUTTONDISCONNECT = wxNewId();
const long MoCapFrame::ID_PANEL2 = wxNewId();
const long MoCapFrame::ID_MAINPANEL = wxNewId();
const long MoCapFrame::idMenuQuit = wxNewId();
const long MoCapFrame::idMenuAbout = wxNewId();
//*)

const long MoCapFrame::ID_VIEWPANEL = wxNewId();
const long MoCapFrame::ID_DATAFLOWPANEL = wxNewId();
const long MoCapFrame::ID_SENSORDETAILPANEL = wxNewId();
const long MoCapFrame::ID_SKELETONCREATORPANEL = wxNewId();
const long MoCapFrame::ID_MOTIONPLAYERPANEL = wxNewId();
const long MoCapFrame::ID_POSTPROCESSPANEL = wxNewId();

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

    Create(parent, wxID_ANY, _("Motion Capturing Toolkit"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    FrameSizer = new wxBoxSizer(wxHORIZONTAL);
    MainPanel = new wxPanel(this, ID_MAINPANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_MAINPANEL"));
    MainSizer = new wxBoxSizer(wxVERTICAL);
    BoxSizer2 = new wxBoxSizer(wxVERTICAL);
    Panel1 = new wxPanel(MainPanel, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    ButtonSkeleton = new wxButton(Panel1, ID_BUTTONSKELETON, _("Skeleton Editor"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONSKELETON"));
    BoxSizer1->Add(ButtonSkeleton, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ButtonSensorDetail = new wxButton(Panel1, ID_BUTTONSENSORDETAIL, _("Sensor Info"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONSENSORDETAIL"));
    BoxSizer1->Add(ButtonSensorDetail, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ButtonDataFlow = new wxButton(Panel1, ID_BUTTONDATAFLOW, _("Flow Diagram"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONDATAFLOW"));
    BoxSizer1->Add(ButtonDataFlow, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ButtonVisual = new wxButton(Panel1, ID_BUTTONVISUAL, _("Motion Capture"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONVISUAL"));
    ButtonVisual->SetFocus();
    BoxSizer1->Add(ButtonVisual, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ButtonMotionPlayer = new wxButton(Panel1, ID_BUTTONMOTIONPLAYER, _("Motion Player"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONMOTIONPLAYER"));
    BoxSizer1->Add(ButtonMotionPlayer, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ButtonPostProcess = new wxButton(Panel1, ID_BUTTONPOSTPROCESS, _("Motion Editor"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONPOSTPROCESS"));
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
    ButtonLog = new wxButton(Panel2, ID_BUTTONLOG, _("Show Log"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONLOG"));
    BoxSizer4->Add(ButtonLog, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ButtonConnection = new wxButton(Panel2, ID_BUTTONCONNECTION, _("Connection"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONCONNECTION"));
    BoxSizer4->Add(ButtonConnection, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ButtonDisconnect = new wxButton(Panel2, ID_BUTTONDISCONNECT, _("Disconnect"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONDISCONNECT"));
    ButtonDisconnect->Disable();
    BoxSizer4->Add(ButtonDisconnect, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer4->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
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
    Connect(ID_BUTTONDATAFLOW,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MoCapFrame::OnButtonDataFlowClick);
    Connect(ID_BUTTONVISUAL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MoCapFrame::OnButtonVisualClick);
    Connect(ID_BUTTONMOTIONPLAYER,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MoCapFrame::OnButtonMotionPlayerClick);
    Connect(ID_BUTTONPOSTPROCESS,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MoCapFrame::OnButtonPostProcessClick);
    Connect(ID_BUTTONFULLSCREEN,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MoCapFrame::OnButtonFullScreenClick);
    Connect(ID_BUTTONLOG,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MoCapFrame::OnButtonLogClick);
    Connect(ID_BUTTONCONNECTION,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MoCapFrame::OnButtonConnectionClick);
    Connect(ID_BUTTONDISCONNECT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MoCapFrame::OnButtonDisconnectClick);
    MainPanel->Connect(wxEVT_KEY_DOWN,(wxObjectEventFunction)&MoCapFrame::OnMainPanelKeyDown,0,this);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&MoCapFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&MoCapFrame::OnAbout);
    Connect(wxEVT_KEY_DOWN,(wxObjectEventFunction)&MoCapFrame::OnKeyDown);
    //*)
    // Connect(ID_SOCKET, wxEVT_SOCKET, wxSocketEventHandler(MoCapFrame::OnSocketEvent));
    Connect(ID_TIMER, wxEVT_TIMER, wxTimerEventHandler(MoCapFrame::OnTimerEvent));

    Maximize(true);


    _logger = new wxLogWindow(this, _("Log"), false);

    _addressPeer.BroadcastAddress();
    _addressPeer.Service(5040);
    _addressLocal.Hostname(_("192.168.0.101"));//wxGetFullHostName());
    _addressLocal.Service(5040);


    // TODO(JK#3#): maybe directly start the timer, allows GUI to react to whatever even when not connected
    _timer = new wxTimer(this, ID_TIMER);
    _timer->Start(10);
    _counter = 0;

    Freeze();
    DataPanel->Destroy();
    DataPanel = new ViewPanel(MainPanel, ID_DATAPANEL);
    DataPanelSizer->Add(DataPanel, 1, wxEXPAND, 5);
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

bool MoCapFrame::isConnected() const
{
    // TODO(JK#9#2017-06-01): MoCapFrame::isConnected can be more efficient with a variable
    for (size_t i = 0; i < _receivers.size(); ++i)
    {
        if (_receivers[i]->isConnected())
        {
            return true;
        }
    }
    return false;
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
                ButtonDisconnect->Disable();
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
    alignas(float) unsigned char buffer[36];
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
        msg << _("Timestamp: ") << data.timestamp << _("\nRotation: ");
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
    // handle setting of id for sensor node (and data) (solved by setting IP + id as name)
    wxString name = sensorAddress.IPAddress() + _("-");
    name << data.id;

    if (theSensorManager.getSensorIdFromName(name.ToStdString()) == -1)
    {
        _receiveStartTime[name] = receiveTime;
        Refresh();
    }

    if (bytesReceived >= 24 && bytesReceived <= 36)//== sizeof(SensorData))
    {
        ++_receivedPackets[name];

    // wxString msg;
    // msg << name << _("   Timestamp: ") << data.timestamp << _("   receive Time: ") << receiveTime - _receiveStartTime[name];
    // wxLogDebug(msg);

        // theSensorManager.updateSensor(name.ToStdString(), data);
    }
}

void MoCapFrame::OnTimerEvent(wxTimerEvent& event)
{
    /* now done in MoCapManager::update
    for (size_t i = 0; i < _receivers.size(); ++i)
    {
        if (!_receivers[i]->update())
        {
            wxLogDebug(_("failed to update receiver ") + _receivers[i]->getName());
            // TODO(JK#2#2017-06-01): what to do if update in receivers fails?
            // _receivers[i]->disconnect();
        }
    }
    */

    theMoCapManager.update();

    // send a dummy over the socket every second. Somehow this is needed to ensure we get socket events.
    ++_counter;
    /*
    if (_counter >= 40 && _socket != nullptr && _socket->IsOk())
    {
        _counter = 0;
        char dummy = 0;
        if (_socket->SendTo(_addressPeer, &dummy, sizeof(dummy)).LastCount() != sizeof(dummy))
        {
            wxLogDebug("ERROR: failed to send data");
            return;
        }
        for (auto it = _receivedPackets.begin(); it != _receivedPackets.end(); ++it)
        {
            wxString msg;
            msg << it->first << _("  packets per sec: ") << it->second;
            //wxLogDebug(msg);
            it->second = 0;
        }
        wxString msg = _("\n-----------------------------\n");
        //wxLogDebug(msg);
    }
    */
    // send an update event to update the GUI
    wxCommandEvent updateEvent(UpdateEvent);
    wxPostEvent(DataPanel, updateEvent);
}

void MoCapFrame::OnButtonSensorDetailClick(wxCommandEvent& event)
{
    Freeze();
    DataPanel->Destroy();
    DataPanel = new SensorDetailPanel(MainPanel, ID_SENSORDETAILPANEL);
    DataPanelSizer->Add(DataPanel, 1, wxEXPAND, 5);
    DataPanelSizer->Layout();
    MainSizer->Layout();
    Thaw();
    Refresh();
    theMoCapManager.setState(SENSOR_INFO);
}

void MoCapFrame::OnButtonDataFlowClick(wxCommandEvent& event)
{
    Freeze();
    DataPanel->Destroy();
    DataPanel = new DataFlowPanel(MainPanel, ID_DATAFLOWPANEL);
    DataPanelSizer->Add(DataPanel, 1, wxEXPAND, 5);
    DataPanelSizer->Layout();
    MainSizer->Layout();
    Thaw();
    Refresh();
    theMoCapManager.setState(DATA_FLOW);
    //theMoCapManager.setRenderStyle(HIGHLIGHT_SELECTED_BONE);
}

void MoCapFrame::OnButtonVisualClick(wxCommandEvent& event)
{
    Freeze();
    DataPanel->Destroy();
    DataPanel = new ViewPanel(MainPanel, ID_VIEWPANEL);
    DataPanelSizer->Add(DataPanel, 1, wxEXPAND, 5);
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
    DataPanelSizer->Add(DataPanel, 1, wxEXPAND, 5);
    DataPanelSizer->Layout();
    MainSizer->Layout();
    Thaw();
    Refresh();
    theMoCapManager.setState(EDITOR);
    //theMoCapManager.setRenderStyle(DRAW_SPIN_ARROWS | HIGHLIGHT_SELECTED_BONE);
}

void MoCapFrame::OnButtonMotionPlayerClick(wxCommandEvent& event)
{
    Freeze();
    DataPanel->Destroy();
    DataPanel = new MotionPlayerPanel(MainPanel, ID_MOTIONPLAYERPANEL);
    DataPanelSizer->Add(DataPanel, 1, wxEXPAND, 5);
    DataPanelSizer->Layout();
    MainSizer->Layout();
    Thaw();
    Refresh();
    theMoCapManager.setState(MOTION_PLAYER);
}

void MoCapFrame::OnButtonPostProcessClick(wxCommandEvent& event)
{
    Freeze();
    DataPanel->Destroy();
    DataPanel = new PostProcessPanel(MainPanel, ID_POSTPROCESSPANEL);
    DataPanelSizer->Add(DataPanel, 1, wxEXPAND, 5);
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
    /*
    if (_connectionManagerPopup == nullptr)
    {
        _connectionManagerPopup = new PopupWindow<ConnectionManagerPanel>(this);
    }
    _connectionManagerPopup->setSize(500, 400);
    _connectionManagerPopup->Center();
    _connectionManagerPopup->Show();
    */

    ConnectionManagerDialog connectionDialog(this);
    connectionDialog.ShowModal();
    return;

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
        _receivers.push_back(new ReceiverUDP(dialog->getIP(), dialog->getPort()));

        dialog->Destroy();

        if (!_receivers.back()->connect())
        {
            wxMessageBox(_("Failed to create UDP socket at ") + _addressLocal.IPAddress(), _("Error"), wxICON_ERROR);
            return;
        }
        // Create the socket
        /*
        _socket = new wxDatagramSocket(_addressLocal, wxSOCKET_BROADCAST | wxSOCKET_NOWAIT | wxSOCKET_REUSEADDR);
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
        */
    }
    else
    {
        dialog->Destroy();
        // return;
    }


    ReceiverKinect* recv = new ReceiverKinect();
    if (!recv->connect())
    {
        recv->disconnect();
        delete recv;
        wxMessageBox(_("unable to connect kinect"));
    }
    else
    {
        _receivers.push_back(recv);
    }

    // show the file dialog
    wxFileDialog* fileDialog = new wxFileDialog(this, _("Open file"), _(""), _(""), _("csv files (*.csv)|*.csv"), wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if (fileDialog->ShowModal() == wxID_CANCEL)
    {
        fileDialog->Destroy();
        return;
    }
    SetCursor(wxCURSOR_ARROWWAIT);
    ReceiverFile* recvFile = new ReceiverFile(fileDialog->GetPath());
    fileDialog->Destroy();

    SetCursor(wxCURSOR_DEFAULT);

    if (!recvFile->connect())
    {
        recvFile->disconnect();
        delete recvFile;
        wxMessageBox(_("unable to open file"));
    }
    else
    {
        _receivers.push_back(recvFile);
    }

    ButtonDisconnect->Enable();
    Refresh();
    _timer->Start(15);
}


void MoCapFrame::OnButtonDisconnectClick(wxCommandEvent& event)
{
    if (theMoCapManager.isRecording())
    {
        if (wxMessageBox(_("Do you want to disconnect and stop recording without saving?"), _("Attention"), wxICON_EXCLAMATION|wxYES|wxNO|wxCENTRE) == wxNO)
        {
            return;
        }
    }

    theMoCapManager.stopSimulation();
    _timer->Stop();

    if (_socket != nullptr)
    {
        _socket->Destroy();
        _socket = nullptr;
    }

    for (size_t i = 0; i < _receivers.size(); ++i)
    {
        _receivers[i]->disconnect();
    }

    ButtonDisconnect->Disable();
    theMoCapManager.resetSkeleton();
    Refresh();
    // update panels
    theSensorManager.resetAllSensorStatesUpdated();
    wxCommandEvent updateEvent(UpdateEvent);
    wxPostEvent(DataPanel, updateEvent);
}



void MoCapFrame::OnKeyDown(wxKeyEvent& event)
{
    switch (event.GetKeyCode())
    {
        case WXK_ESCAPE:
            if (IsFullScreen())
            {
                ButtonFullScreen->SetLabel("Full Screen");
                ShowFullScreen(false, wxFULLSCREEN_ALL);
            }
            break;
        default:
            return;
    }
}

void MoCapFrame::OnMainPanelKeyDown(wxKeyEvent& event)
{
    event.Skip();
}

void MoCapFrame::OnButtonLogClick(wxCommandEvent& event)
{
    if (_logger == nullptr)
    {
        _logger = new wxLogWindow(this, _("Log"));
    }
    _logger->Show();
}




