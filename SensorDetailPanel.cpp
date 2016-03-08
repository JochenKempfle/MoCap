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
#include "SensorDetailPanel.h"
#include "SensorManager.h"
#include "CustomEvents.h"
#include "SensorNode.h"
#include <vector>

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(SensorDetailPanel)
	#include <wx/intl.h>
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(SensorDetailPanel)
//*)

//(*IdInit(SensorDetailPanel)
const long SensorDetailPanel::ID_SENSOREXTCONTAINERPANEL = wxNewId();
//*)

BEGIN_EVENT_TABLE(SensorDetailPanel,wxPanel)
	//(*EventTable(SensorDetailPanel)
	//*)
END_EVENT_TABLE()

SensorDetailPanel::SensorDetailPanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(SensorDetailPanel)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	BoxSizerSensors = new wxBoxSizer(wxHORIZONTAL);
	sensorContainerPanel = new ScrolledContainerPanel(this,ID_SENSOREXTCONTAINERPANEL,wxDefaultPosition,wxDefaultSize);
	BoxSizerSensors->Add(sensorContainerPanel, 1, wxALL|wxEXPAND, 5);
	SetSizer(BoxSizerSensors);
	BoxSizerSensors->Fit(this);
	BoxSizerSensors->SetSizeHints(this);
	//*)
	Connect(UpdateEvent, (wxObjectEventFunction)&SensorDetailPanel::OnUpdateEvent);
	// addSensor(0);
}

SensorDetailPanel::~SensorDetailPanel()
{
	//(*Destroy(SensorDetailPanel)
	//*)
}

void SensorDetailPanel::OnUpdateEvent(wxEvent& event)
{
    //Freeze();
    std::vector<SensorNode*> sensors = theSensorManager.getSensors();
    for (size_t i = 0; i < sensors.size(); ++i)
    {
        int id = sensors[i]->getId();
        auto it = _dataPanelFromId.find(id);
        if (it != _dataPanelFromId.end())
        {
            it->second->update();
        }
        else
        {
            addSensor(sensors[i])->update();
            BoxSizerSensors->Layout();
            Refresh();
        }
    }
    theSensorManager.resetAllSensorStatesUpdated();
    //Thaw();
}

SensorDataExtPanel* SensorDetailPanel::addSensor(SensorNode* sensor)
{
    SensorDataExtPanel* sensorPanel = new SensorDataExtPanel(sensorContainerPanel->ScrolledWindow);
    sensorContainerPanel->add(sensorPanel);
    _dataPanelFromId[sensor->getId()] = sensorPanel;
    sensorPanel->setSensor(sensor);

    return sensorPanel;
}



