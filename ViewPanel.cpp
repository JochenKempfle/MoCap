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
#include <gl/glu.h>
#include "ViewPanel.h"
#include "SensorDataPanel.h"
#include "SensorManager.h"
#include "MoCapManager.h"
#include "CustomEvents.h"
#include <vector>

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(ViewPanel)
	#include <wx/intl.h>
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(ViewPanel)
//*)

//(*IdInit(ViewPanel)
const long ViewPanel::ID_BUTTONSIMULATE = wxNewId();
const long ViewPanel::ID_BUTTONRECORD = wxNewId();
const long ViewPanel::ID_BUTTONSTOP = wxNewId();
const long ViewPanel::ID_BUTTONAUTOASSIGN = wxNewId();
const long ViewPanel::ID_BUTTONCALIBRATE = wxNewId();
const long ViewPanel::ID_GLCANVAS = wxNewId();
const long ViewPanel::ID_SENSORCONTAINERPANEL = wxNewId();
//*)

BEGIN_EVENT_TABLE(ViewPanel,wxPanel)
	//(*EventTable(ViewPanel)
	//*)
END_EVENT_TABLE()

ViewPanel::ViewPanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(ViewPanel)
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer3;

	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizerControl = new wxBoxSizer(wxVERTICAL);
	BoxSizerControl->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonSimulate = new wxButton(this, ID_BUTTONSIMULATE, _("Simulate"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONSIMULATE"));
	BoxSizerControl->Add(ButtonSimulate, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonRecord = new wxButton(this, ID_BUTTONRECORD, _("Record"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONRECORD"));
	BoxSizerControl->Add(ButtonRecord, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonStop = new wxButton(this, ID_BUTTONSTOP, _("Stop"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONSTOP"));
	ButtonStop->Hide();
	BoxSizerControl->Add(ButtonStop, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonAutoAssign = new wxButton(this, ID_BUTTONAUTOASSIGN, _("Auto Assign"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONAUTOASSIGN"));
	BoxSizerControl->Add(ButtonAutoAssign, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonCalibrate = new wxButton(this, ID_BUTTONCALIBRATE, _("Calibrate"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONCALIBRATE"));
	BoxSizerControl->Add(ButtonCalibrate, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizerControl, 0, wxEXPAND, 5);
	BoxSizer3 = new wxBoxSizer(wxVERTICAL);
	int GLCanvasAttributes_1[] = {
		WX_GL_RGBA,
		WX_GL_DOUBLEBUFFER,
		WX_GL_DEPTH_SIZE,      16,
		WX_GL_STENCIL_SIZE,    0,
		0, 0 };
	glCanvas = new GLCanvas(this, ID_GLCANVAS, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE, _T("ID_GLCANVAS"), GLCanvasAttributes_1);
	glCanvas->SetMinSize(wxSize(400,300));
	glCanvas->SetBackgroundColour(wxColour(0,0,0));
	BoxSizer3->Add(glCanvas, 1, wxALL|wxEXPAND, 5);
	BoxSizer1->Add(BoxSizer3, 1, wxEXPAND, 5);
	BoxSizerSensors = new wxBoxSizer(wxVERTICAL);
	sensorContainerPanel = new ScrolledContainerPanel(this,ID_SENSORCONTAINERPANEL,wxDefaultPosition,wxDefaultSize);
	BoxSizerSensors->Add(sensorContainerPanel, 1, wxALL|wxEXPAND, 5);
	BoxSizerSensors->Add(100,0,0, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizerSensors, 0, wxEXPAND, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);

	Connect(ID_BUTTONSIMULATE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ViewPanel::OnButtonSimulateClick);
	Connect(ID_BUTTONRECORD,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ViewPanel::OnButtonRecordClick);
	Connect(ID_BUTTONSTOP,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ViewPanel::OnButtonStopClick);
	Connect(ID_BUTTONAUTOASSIGN,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ViewPanel::OnButtonAutoAssignClick);
	Connect(ID_BUTTONCALIBRATE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ViewPanel::OnButtonCalibrateClick);
	//*)
	Connect(UpdateEvent, (wxObjectEventFunction)&ViewPanel::OnUpdateEvent);
    // addSensor(0);//(new SensorDataPanel(sensorContainerPanel->ScrolledWindow));
	// addSensor(1);//(new SensorDataPanel(sensorContainerPanel->ScrolledWindow));

	glCanvas->setSkeleton(theMoCapManager.getSkeleton());
	glCanvas->setStyle(DRAW_LOCAL_COORDINATE_SYSTEM);

    _counter = 0;
    _simulationStarted = false;
    _autoAssignStarted = false;
    _autoAssignBoneId = -1;
    // TODO(JK#3#): restore previous state when switching to this panel again (maybe don't destroy panels in MoCapMain)
}

ViewPanel::~ViewPanel()
{
	//(*Destroy(ViewPanel)
	//*)
}

void ViewPanel::OnUpdateEvent(wxEvent& event)
{
    if (_counter >= 4)
    {
        _counter = 0;

        // check for auto assignment
        if (_autoAssignStarted)
        {
            std::vector<int> moving = theSensorManager.getMoving(30.0);
            if (moving.size() == 1)
            {
                theMoCapManager.assignSensorToBone(moving[0], _autoAssignBoneId);
                _autoAssignStarted = false;
                _autoAssignBoneId = -1;
            }
        }
        // update sensor states displayed in the GUI
        std::vector<SensorNode*> sensors = theSensorManager.getSensors();
        for (size_t i = 0; i < sensors.size(); ++i)
        {
            int id = sensors[i]->getId();
            auto it = _dataPanelFromId.find(id);
            if (it != _dataPanelFromId.end())
            {
                it->second->update(sensors[i]);
            }
            else
            {
                addSensor(id)->update(sensors[i]);
            }
        }
        BoxSizerSensors->Layout();
        Refresh();
        // set sensor state to not updated, such that they have to receive a update first before having the updated flag set again
        // this way not updated sensors can be detected
        theSensorManager.resetAllSensorStatesUpdated();
    }
    // tell the mocap manager to update itself
    if (_simulationStarted)
    {
        theMoCapManager.update();
    }
    ++_counter;
}

SensorDataPanel* ViewPanel::addSensor(int id)
{
    SensorDataPanel* sensorPanel = new SensorDataPanel(sensorContainerPanel->ScrolledWindow);
    sensorPanel->setSensorId(id);
    sensorContainerPanel->add(sensorPanel);
    _dataPanelFromId[id] = sensorPanel;
    return sensorPanel;
}


void ViewPanel::OnButtonSimulateClick(wxCommandEvent& event)
{
    _simulationStarted = true;
    Freeze();
    ButtonSimulate->Hide();
    ButtonCalibrate->Hide();
    ButtonStop->Show();
    BoxSizerControl->Layout();
    Thaw();
    Refresh();
    // TODO(JK#3#): check if connection is not lost while started
}

void ViewPanel::OnButtonRecordClick(wxCommandEvent& event)
{
    _simulationStarted = true;
    Freeze();
    ButtonSimulate->Hide();
    ButtonRecord->Hide();
    ButtonCalibrate->Hide();
    ButtonStop->Show();
    BoxSizerControl->Layout();
    Thaw();
    Refresh();
    theMoCapManager.startRecording();
}

void ViewPanel::OnButtonStopClick(wxCommandEvent& event)
{
    _simulationStarted = false;
    Freeze();
    ButtonStop->Hide();
    ButtonCalibrate->Show();
    ButtonSimulate->Show();
    ButtonRecord->Show();
    BoxSizerControl->Layout();
    theMoCapManager.resetSkeleton();
    Thaw();
    Refresh();
    theMoCapManager.stopRecording();
}

void ViewPanel::OnButtonAutoAssignClick(wxCommandEvent& event)
{
    // TODO(JK#3#): check if connected, otherwise this will cause problems, also notice the user that auto assign is in progress
    auto bones = theMoCapManager.getBoneIdsWithName();
    wxMenu menu;
    menu.Append(-1, _("no bone"));
    for (size_t i = 0; i < bones.size(); ++i)
    {
        menu.Append(bones[i].first, bones[i].second);
    }
 	menu.Connect(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&ViewPanel::OnPopupClick, NULL, this);
 	PopupMenu(&menu);
}

void ViewPanel::OnPopupClick(wxCommandEvent &event)
{
    _autoAssignBoneId = event.GetId();
    // call getMoving to start motion detection
    theSensorManager.getMoving(30.0);
    _autoAssignStarted = true;
}

void ViewPanel::OnButtonCalibrateClick(wxCommandEvent& event)
{
    theMoCapManager.calibrate();
}




