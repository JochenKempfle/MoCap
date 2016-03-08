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


#ifndef VIEWPANEL_H
#define VIEWPANEL_H

#include "GLCanvas.h"
#include "SensorDataPanel.h"
#include <map>

#ifndef WX_PRECOMP
	//(*HeadersPCH(ViewPanel)
	#include <wx/sizer.h>
	#include <wx/panel.h>
	#include <wx/button.h>
	//*)
#endif
//(*Headers(ViewPanel)
#include "ScrolledContainerPanel.h"
#include <wx/glcanvas.h>
//*)

class ViewPanel: public wxPanel
{
	public:

		ViewPanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~ViewPanel();

		//(*Declarations(ViewPanel)
		wxButton* ButtonMapCoordinates;
		ScrolledContainerPanel* sensorContainerPanel;
		wxButton* ButtonMapBones;
		wxButton* ButtonRecord;
		wxBoxSizer* BoxSizerSensors;
		wxButton* ButtonCalibrate;
		wxButton* ButtonAutoAssign;
		wxButton* ButtonStop;
		wxBoxSizer* BoxSizerControl;
		GLCanvas* glCanvas;
		wxButton* ButtonSimulate;
		//*)

	protected:

		//(*Identifiers(ViewPanel)
		static const long ID_BUTTONSIMULATE;
		static const long ID_BUTTONRECORD;
		static const long ID_BUTTONSTOP;
		static const long ID_BUTTONAUTOASSIGN;
		static const long ID_BUTTONCALIBRATE;
		static const long ID_BUTTONMAPCOORDINATES;
		static const long ID_BUTTONMAPBONES;
		static const long ID_GLCANVAS;
		static const long ID_SENSORCONTAINERPANEL;
		//*)

	private:

		//(*Handlers(ViewPanel)
		void OnButtonSimulateClick(wxCommandEvent& event);
		void OnButtonStopClick(wxCommandEvent& event);
		void OnButtonAutoAssignClick(wxCommandEvent& event);
		void OnButtonCalibrateClick(wxCommandEvent& event);
		void OnButtonRecordClick(wxCommandEvent& event);
		void OnButtonMapCoordinatesClick(wxCommandEvent& event);
		void OnButtonMapBonesClick(wxCommandEvent& event);
		//*)
		void OnUpdateEvent(wxEvent& event);
        void OnPopupClick(wxCommandEvent& event);

		SensorDataPanel* addSensor(int id);

		std::map<int, SensorDataPanel*> _dataPanelFromId;

        int _counter;
        bool _simulationStarted;
        bool _autoAssignStarted;
        int _autoAssignBoneId;

		DECLARE_EVENT_TABLE()
};

#endif
