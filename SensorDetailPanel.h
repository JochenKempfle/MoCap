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


#ifndef SENSORDETAILPANEL_H
#define SENSORDETAILPANEL_H

#ifndef WX_PRECOMP
	//(*HeadersPCH(SensorDetailPanel)
	#include <wx/sizer.h>
	#include <wx/panel.h>
	//*)
#endif
//(*Headers(SensorDetailPanel)
#include "ScrolledContainerPanel.h"
//*)

#include "SensorDataExtPanel.h"
#include <map>

class SensorDetailPanel: public wxPanel
{
	public:

		SensorDetailPanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~SensorDetailPanel();

		//(*Declarations(SensorDetailPanel)
		ScrolledContainerPanel* sensorContainerPanel;
		wxBoxSizer* BoxSizerSensors;
		//*)

	protected:

		//(*Identifiers(SensorDetailPanel)
		static const long ID_SENSOREXTCONTAINERPANEL;
		//*)

	private:

		//(*Handlers(SensorDetailPanel)
		//*)
		void OnUpdateEvent(wxEvent& event);

		SensorDataExtPanel* addSensor(int id);

		std::map<int, SensorDataExtPanel*> _dataPanelFromId;

		DECLARE_EVENT_TABLE()
};

#endif
