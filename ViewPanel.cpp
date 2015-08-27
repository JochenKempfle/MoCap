/*Copyright (c) 2014, Jochen KempfleAll rights reserved.Redistribution and use in source and binary forms, with or without modification,are permitted provided that the following conditions are met:1. Redistributions of source code must retain the above copyright notice,this list of conditions and the following disclaimer.2. Redistributions in binary form must reproduce the above copyright notice,this list of conditions and the following disclaimer in the documentation and/orother materials provided with the distribution.THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" ANDANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIEDWARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITYOF SUCH DAMAGE.*/#include "wx_pch.h"
#include <gl/glu.h>#include "ViewPanel.h"#include "SensorDataPanel.h"#include "SensorManager.h"#include "MoCapManager.h"#include "CustomEvents.h"#include <vector>

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(ViewPanel)
	#include <wx/intl.h>
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(ViewPanel)
//*)

//(*IdInit(ViewPanel)
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

	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer3 = new wxBoxSizer(wxVERTICAL);
	int GLCanvasAttributes_1[] = {
		WX_GL_RGBA,
		WX_GL_DOUBLEBUFFER,
		WX_GL_DEPTH_SIZE,      16,
		WX_GL_STENCIL_SIZE,    0,
		0, 0 };
	glCanvas = new GLCanvas(this, ID_GLCANVAS, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE, _T("ID_GLCANVAS"), GLCanvasAttributes_1);
	glCanvas->SetMinSize(wxSize(400,300));
	BoxSizer3->Add(glCanvas, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer3, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizerSensors = new wxBoxSizer(wxVERTICAL);
	sensorContainerPanel = new ScrolledContainerPanel(this,ID_SENSORCONTAINERPANEL,wxDefaultPosition,wxDefaultSize);
	BoxSizerSensors->Add(sensorContainerPanel, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizerSensors->Add(100,0,0, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizerSensors, 0, wxEXPAND|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	//*)	Connect(UpdateEvent, (wxObjectEventFunction)&ViewPanel::OnUpdateEvent);    // addSensor(0);//(new SensorDataPanel(sensorContainerPanel->ScrolledWindow));	// addSensor(1);//(new SensorDataPanel(sensorContainerPanel->ScrolledWindow));
}

ViewPanel::~ViewPanel()
{
	//(*Destroy(ViewPanel)
	//*)
}

void ViewPanel::OnUpdateEvent(wxEvent& event){    std::vector<SensorNode> sensors = theSensorManager.getSensors();    for (size_t i = 0; i < sensors.size(); ++i)    {        int id = sensors[i].getId();        auto it = _dataPanelFromId.find(id);        if (it != _dataPanelFromId.end())        {            it->second->update(sensors[i]);        }        else        {            addSensor(id)->update(sensors[i]);        }    }    BoxSizerSensors->Layout();    Refresh();    // set sensor state to not updated, such that they have to receive a update first before having the updated flag set again    // this way not updated sensors can be detected    theSensorManager.resetAllSensorStatesUpdated();}
SensorDataPanel* ViewPanel::addSensor(int id){    SensorDataPanel* sensorPanel = new SensorDataPanel(sensorContainerPanel->ScrolledWindow);    sensorPanel->setSensorId(id);    sensorContainerPanel->add(sensorPanel);    _dataPanelFromId[id] = sensorPanel;    return sensorPanel;}

