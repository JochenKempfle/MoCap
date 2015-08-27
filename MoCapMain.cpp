/*Copyright (c) 2014, Jochen KempfleAll rights reserved.Redistribution and use in source and binary forms, with or without modification,are permitted provided that the following conditions are met:1. Redistributions of source code must retain the above copyright notice,this list of conditions and the following disclaimer.2. Redistributions in binary form must reproduce the above copyright notice,this list of conditions and the following disclaimer in the documentation and/orother materials provided with the distribution.THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" ANDANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIEDWARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITYOF SUCH DAMAGE.*/

#include "wx_pch.h"
#include "MoCapMain.h"
#include <wx/msgdlg.h>#include <wx/utils.h>#include "ViewPanel.h"#include "SensorDetailPanel.h"#include "SensorManager.h"#include "MoCapManager.h"#include "ConnectionDialog.h"#include "SensorData.h"#include "CustomEvents.h"

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
const long MoCapFrame::ID_BUTTONSENSORDETAIL = wxNewId();
const long MoCapFrame::ID_BUTTONVISUAL = wxNewId();
const long MoCapFrame::ID_BUTTONFULLSCREEN = wxNewId();
const long MoCapFrame::ID_BUTTONSTART = wxNewId();
const long MoCapFrame::ID_BUTTONSTOP = wxNewId();
const long MoCapFrame::ID_BUTTONAUTOASSIGN = wxNewId();
const long MoCapFrame::ID_BUTTONCALIBRATE = wxNewId();
const long MoCapFrame::ID_BUTTONCONNECT = wxNewId();
const long MoCapFrame::ID_BUTTONDISCONNECT = wxNewId();
const long MoCapFrame::ID_DATAPANEL = wxNewId();
const long MoCapFrame::ID_MAINPANEL = wxNewId();
const long MoCapFrame::idMenuQuit = wxNewId();
const long MoCapFrame::idMenuAbout = wxNewId();
const long MoCapFrame::ID_STATUSBAR1 = wxNewId();
//*)
const long MoCapFrame::ID_VIEWPANEL = wxNewId();const long MoCapFrame::ID_SENSORDETAILPANEL = wxNewId();
const long MoCapFrame::ID_SOCKET = wxNewId();const long MoCapFrame::ID_TIMER = wxNewId();

BEGIN_EVENT_TABLE(MoCapFrame,wxFrame)
    //(*EventTable(MoCapFrame)
    //*)
END_EVENT_TABLE()

MoCapFrame::MoCapFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(MoCapFrame)
    wxMenuItem* MenuItem2;
    wxMenuItem* MenuItem1;
    wxBoxSizer* BoxSizer2;
    wxMenu* Menu1;
    wxMenuBar* MenuBar1;
    wxStaticBoxSizer* StaticBoxSizer1;
    wxMenu* Menu2;

    Create(parent, wxID_ANY, _("Motion Capturing"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    FrameSizer = new wxBoxSizer(wxHORIZONTAL);
    MainPanel = new wxPanel(this, ID_MAINPANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_MAINPANEL"));
    MainSizer = new wxBoxSizer(wxHORIZONTAL);
    BoxSizer2 = new wxBoxSizer(wxVERTICAL);
    StaticBoxSizer1 = new wxStaticBoxSizer(wxVERTICAL, MainPanel, _("View Mode"));
    ButtonSensorDetail = new wxButton(MainPanel, ID_BUTTONSENSORDETAIL, _("Sensor Detail"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONSENSORDETAIL"));
    ButtonSensorDetail->SetDefault();
    StaticBoxSizer1->Add(ButtonSensorDetail, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ButtonVisual = new wxButton(MainPanel, ID_BUTTONVISUAL, _("Visual"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONVISUAL"));
    ButtonVisual->Hide();
    StaticBoxSizer1->Add(ButtonVisual, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ButtonFullScreen = new wxButton(MainPanel, ID_BUTTONFULLSCREEN, _("Full Screen"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONFULLSCREEN"));
    StaticBoxSizer1->Add(ButtonFullScreen, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer2->Add(StaticBoxSizer1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer2->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ButtonStart = new wxButton(MainPanel, ID_BUTTONSTART, _("Start"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONSTART"));
    ButtonStart->Hide();
    BoxSizer2->Add(ButtonStart, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ButtonStop = new wxButton(MainPanel, ID_BUTTONSTOP, _("Stop"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONSTOP"));
    ButtonStop->Hide();
    BoxSizer2->Add(ButtonStop, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ButtonAutoAssign = new wxButton(MainPanel, ID_BUTTONAUTOASSIGN, _("Auto Assign"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONAUTOASSIGN"));
    ButtonAutoAssign->Hide();
    BoxSizer2->Add(ButtonAutoAssign, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ButtonCalibrate = new wxButton(MainPanel, ID_BUTTONCALIBRATE, _("Calibrate"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONCALIBRATE"));
    ButtonCalibrate->Hide();
    BoxSizer2->Add(ButtonCalibrate, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ButtonConnect = new wxButton(MainPanel, ID_BUTTONCONNECT, _("Connect"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONCONNECT"));
    BoxSizer2->Add(ButtonConnect, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ButtonDisconnect = new wxButton(MainPanel, ID_BUTTONDISCONNECT, _("Disconnect"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONDISCONNECT"));
    ButtonDisconnect->Hide();
    BoxSizer2->Add(ButtonDisconnect, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    MainSizer->Add(BoxSizer2, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    DataPanelSizer = new wxBoxSizer(wxHORIZONTAL);
    DataPanel = new wxPanel(MainPanel, ID_DATAPANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_DATAPANEL"));
    DataPanelSizer->Add(DataPanel, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    MainSizer->Add(DataPanelSizer, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    MainPanel->SetSizer(MainSizer);
    MainSizer->Fit(MainPanel);
    MainSizer->SetSizeHints(MainPanel);
    FrameSizer->Add(MainPanel, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
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
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);
    FrameSizer->Fit(this);
    FrameSizer->SetSizeHints(this);
    Center();

    Connect(ID_BUTTONSENSORDETAIL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MoCapFrame::OnButtonSensorDetailClick);
    Connect(ID_BUTTONVISUAL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MoCapFrame::OnButtonVisualClick);
    Connect(ID_BUTTONFULLSCREEN,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MoCapFrame::OnButtonFullScreenClick);
    Connect(ID_BUTTONSTART,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MoCapFrame::OnButtonStartClick);
    Connect(ID_BUTTONSTOP,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MoCapFrame::OnButtonStopClick);
    Connect(ID_BUTTONAUTOASSIGN,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MoCapFrame::OnButtonAutoAssignClick);
    Connect(ID_BUTTONCALIBRATE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MoCapFrame::OnButtonCalibrateClick);
    Connect(ID_BUTTONCONNECT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MoCapFrame::OnButtonConnectClick);
    Connect(ID_BUTTONDISCONNECT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MoCapFrame::OnButtonDisconnectClick);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&MoCapFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&MoCapFrame::OnAbout);
    //*)
    Connect(ID_SOCKET, wxEVT_SOCKET, wxSocketEventHandler(MoCapFrame::OnSocketEvent));    Connect(ID_TIMER, wxEVT_TIMER, wxTimerEventHandler(MoCapFrame::OnTimerEvent));    Maximize(true);    _addressPeer.BroadcastAddress();    _addressPeer.Service(5050);    _addressLocal.Hostname(wxGetFullHostName());    _addressLocal.Service(5050);    _socket = nullptr;    // _logger = new wxLogWindow(this, _("Log"));    _timer = new wxTimer(this, ID_TIMER);    _counter = 0;    _simulationStarted = false;    _autoAssignStarted = false;    _autoAssignBoneId = -1;    Freeze();    DataPanel->Destroy();    DataPanel = new ViewPanel(MainPanel, ID_DATAPANEL);    DataPanelSizer->Add(DataPanel, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);    DataPanelSizer->Fit(DataPanel);    DataPanelSizer->SetSizeHints(DataPanel);
    MainSizer->Fit(MainPanel);
    MainSizer->SetSizeHints(MainPanel);    FrameSizer->Fit(this);
    FrameSizer->SetSizeHints(this);    Thaw();    Refresh();
}

MoCapFrame::~MoCapFrame()
{
    //(*Destroy(MoCapFrame)
    //*)    _timer->Stop();    if (_socket != nullptr)    {        _socket->Destroy();        _socket = nullptr;    }
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
void MoCapFrame::OnSocketEvent(wxSocketEvent& event){    switch (event.GetSocketEvent())    {        case wxSOCKET_INPUT:            break;        case wxSOCKET_LOST:            wxLogDebug(_("Socket lost. Try to reconnect."));            _socket->Destroy();            _socket = nullptr;            return;        case wxSOCKET_CONNECTION:            wxLogDebug(_("New incoming connection: IP ..."));            return;        case wxSOCKET_OUTPUT:            wxLogDebug(_("Socket output: ..."));            return;        default:            wxLogDebug(_("Unexpected socket event. Returning from event handler"));            return;    }    if (!_socket->IsOk())    {        wxMessageBox(_("Error in Event: Socket is not available!"));        return;    }    SensorData data;    wxIPV4address sensorAddress = _addressPeer;    if (_socket->RecvFrom(sensorAddress, &data, sizeof(data)).LastCount() == 0)    {        wxMessageBox("ERROR in Event: failed to receive data");        return;    }    if (_socket->LastError() != wxSOCKET_NOERROR)    {        wxMessageBox("ERROR in Event: socket returned error");        return;    }/*    wxString msg;    msg << _("Received: ") << _socket->LastCount() << _(" Bytes.\n");    if (_socket->LastCount() == sizeof(SensorData))    {        msg << _("Timestamp: ") << data.timeStamp << _("\nRotation: ");        msg << data.rotation[0] << _(" ") << data.rotation[1] << _(" ") << data.rotation[2] << _(" ") << data.rotation[3];        msg << _("\nPosition: ") << data.position[0] << _(" ") << data.position[1] << _(" ") << data.position[2] << _("\n");        msg << _("from ");        //msg << _("Received: \"") << wxString::From8BitData(buf, _socket->LastCount() - 1) << _("\" from ");        msg << sensorAddress.IPAddress() << _(" : ") << sensorAddress.Service();        msg << _("\nLocal is: ") << _addressLocal.IPAddress() << _(" : ") << _addressLocal.Service();        //msg << _("\nPeer is: ") << _addressPeer.IPAddress() << _(" : ") << _addressPeer.Service() << _("\n");    }    else if (_socket->LastCount() == 1)    {        msg << _("--- dummy byte to keep connection stable ---\n");    }    else    {        msg << _("---- RECEIVED UNKNOWN DATA! ----\n\n");    }    wxLogDebug(msg);*/    if (theSensorManager.getSensorIdFromIP(sensorAddress.IPAddress().ToStdString()) == -1)    {        Refresh();    }    if (_socket->LastCount() == sizeof(SensorData))    {        theSensorManager.updateSensor(sensorAddress.IPAddress().ToStdString(), data);    }}void MoCapFrame::OnTimerEvent(wxTimerEvent& event){    // send a dummy over the socket every second. Somehow this is needed to ensure we get socket events.    ++_counter;    if (_counter >= 40 && _socket != nullptr && _socket->IsOk())    {        _counter = 0;        char dummy = 0;        if (_socket->SendTo(_addressPeer, &dummy, sizeof(dummy)).LastCount() != sizeof(dummy))        {            wxMessageBox("ERROR: failed to send data");            return;        }    }    // send an update event to update the GUI    if (_counter % 4 == 0)    {        if (_autoAssignStarted)        {            std::vector<int> moving = theSensorManager.getMoving(30.0);            if (moving.size() == 1)            {                theMoCapManager.assignSensorToBone(moving[0], _autoAssignBoneId);                _autoAssignStarted = false;                _autoAssignBoneId = -1;            }        }        wxCommandEvent updateEvent(UpdateEvent);        wxPostEvent(DataPanel, updateEvent);    }    if (_simulationStarted)    {        theMoCapManager.update();    }}
void MoCapFrame::OnButtonSensorDetailClick(wxCommandEvent& event)
{    Freeze();    DataPanel->Destroy();    DataPanel = new SensorDetailPanel(MainPanel, ID_SENSORDETAILPANEL);    DataPanelSizer->Add(DataPanel, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);    ButtonSensorDetail->Hide();    ButtonVisual->Show();    DataPanelSizer->Layout();    MainSizer->Layout();
    Thaw();    Refresh();
}

void MoCapFrame::OnButtonVisualClick(wxCommandEvent& event)
{    Freeze();    DataPanel->Destroy();    DataPanel = new ViewPanel(MainPanel, ID_VIEWPANEL);    DataPanelSizer->Add(DataPanel, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);    ButtonSensorDetail->Show();    ButtonVisual->Hide();    DataPanelSizer->Layout();    MainSizer->Layout();
    Thaw();    Refresh();
}

void MoCapFrame::OnButtonConnectClick(wxCommandEvent& event)
{    ConnectionDialog* dialog = new ConnectionDialog(this);    dialog->setIP(_addressLocal.IPAddress());    dialog->setPort(_addressLocal.Service());    if (dialog->ShowModal() == wxID_OK)    {        _addressLocal.Hostname(dialog->getIP());        _addressLocal.Service(dialog->getPort());        _addressPeer.BroadcastAddress();        _addressPeer.Service(dialog->getPort());        // check if for some reason there is already a socket allocated        if (_socket != nullptr)        {            _socket->Destroy();            _socket = nullptr;        }        // Create the socket        _socket = new wxDatagramSocket(_addressLocal, wxSOCKET_BROADCAST | wxSOCKET_NOWAIT);        if (!_socket->IsOk())        {            wxMessageBox(_("Failed to create UDP socket at ") + _addressLocal.IPAddress(), _("Error"), wxICON_ERROR);            _socket->Destroy();            _socket = nullptr;            return;        }        // Setup the event handler and subscribe to most events        _socket->SetEventHandler(*this, ID_SOCKET);        _socket->SetNotify(wxSOCKET_CONNECTION_FLAG | wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);        _socket->Notify(true);    }    else    {        return;    }    dialog->Destroy();    Freeze();    ButtonConnect->Hide();    ButtonAutoAssign->Show();    ButtonStart->Show();    ButtonDisconnect->Show();    ButtonCalibrate->Show();    MainSizer->Layout();    Thaw();    Refresh();    _timer->Start(25);
}

void MoCapFrame::OnButtonDisconnectClick(wxCommandEvent& event)
{    _timer->Stop();    if (_socket != nullptr)    {        _socket->Destroy();        _socket = nullptr;    }    Freeze();    ButtonDisconnect->Hide();    ButtonCalibrate->Hide();    ButtonStart->Hide();    ButtonStop->Hide();    ButtonAutoAssign->Hide();    ButtonConnect->Show();    MainSizer->Layout();    Thaw();    Refresh();    theMoCapManager.resetSkeleton();    // update panels    theSensorManager.resetAllSensorStatesUpdated();    wxCommandEvent updateEvent(UpdateEvent);    wxPostEvent(DataPanel, updateEvent);
}

void MoCapFrame::OnButtonCalibrateClick(wxCommandEvent& event)
{    theMoCapManager.calibrate();
}

void MoCapFrame::OnButtonStartClick(wxCommandEvent& event)
{    _simulationStarted = true;    Freeze();    ButtonStart->Hide();    ButtonCalibrate->Hide();    ButtonDisconnect->Hide();    ButtonStop->Show();    MainSizer->Layout();    Thaw();    Refresh();
}

void MoCapFrame::OnButtonStopClick(wxCommandEvent& event)
{    _simulationStarted = false;    Freeze();    ButtonStop->Hide();    ButtonCalibrate->Show();    ButtonStart->Show();    ButtonDisconnect->Show();    MainSizer->Layout();    theMoCapManager.resetSkeleton();    Thaw();    Refresh();
}

void MoCapFrame::OnButtonAutoAssignClick(wxCommandEvent& event)
{    auto bones = theMoCapManager.getBoneIdsWithName();    wxMenu menu;    menu.Append(-1, _("no bone"));    for (size_t i = 0; i < bones.size(); ++i)    {        menu.Append(bones[i].first, bones[i].second);    } 	menu.Connect(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MoCapFrame::OnPopupClick, NULL, this); 	PopupMenu(&menu);}void MoCapFrame::OnPopupClick(wxCommandEvent &event){    _autoAssignBoneId = event.GetId();    // call getMoving to start motion detection    theSensorManager.getMoving(30.0);    _autoAssignStarted = true;}

void MoCapFrame::OnButtonFullScreenClick(wxCommandEvent& event)
{    if (IsFullScreen())    {        ButtonFullScreen->SetLabel("Full Screen");        ShowFullScreen(false, wxFULLSCREEN_ALL);    }    else    {        ButtonFullScreen->SetLabel("Normal Screen");        ShowFullScreen(true, wxFULLSCREEN_ALL);    }
}


