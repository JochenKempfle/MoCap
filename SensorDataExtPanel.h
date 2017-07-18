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


#ifndef SENSORDATAEXTPANEL_H
#define SENSORDATAEXTPANEL_H

#ifndef WX_PRECOMP
	//(*HeadersPCH(SensorDataExtPanel)
	#include <wx/sizer.h>
	#include <wx/stattext.h>
	#include <wx/panel.h>
	//*)
#endif
//(*Headers(SensorDataExtPanel)
#include <wx/glcanvas.h>
#include <wx/tglbtn.h>
//*)

#include "SensorNode.h"
#include "SensorBuffer.h"
#include "GLCanvas.h"
#include <list>


class SensorDataExtPanel: public wxPanel
{
	public:

		SensorDataExtPanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~SensorDataExtPanel();

		void setSensor(SensorNode* sensor);
		int getSensorId() const { return _sensor->getId(); }
		void update();

		//(*Declarations(SensorDataExtPanel)
		wxPanel* PanelPlot;
		wxStaticText* StaticText2;
		wxStaticText* StaticTextID;
		wxStaticText* StaticTextState;
		wxStaticText* StaticTextFPS;
		wxStaticText* StaticTextDelay;
		wxStaticText* StaticText1;
		wxStaticText* StaticText3;
		wxStaticText* StaticTextName;
		wxStaticBoxSizer* StaticBoxSizerInfo;
		wxStaticText* StaticTextReceived;
		wxStaticText* StaticTextLost;
		wxToggleButton* ToggleButtonOffset;
		wxStaticText* StaticTextTimeStamp;
		wxStaticText* StaticText4;
		GLCanvas* glCanvas;
		//*)

	protected:

		//(*Identifiers(SensorDataExtPanel)
		static const long ID_STATICTEXTID;
		static const long ID_STATICTEXTName;
		static const long ID_STATICTEXTFPS;
		static const long ID_STATICTEXTSTATE;
		static const long ID_STATICTEXT1;
		static const long ID_STATICTEXT2;
		static const long ID_STATICTEXT3;
		static const long ID_STATICTEXT4;
		static const long ID_STATICTEXTRUNTIME;
		static const long ID_STATICTEXTTIMESTAMP;
		static const long ID_STATICTEXTRECEIVED;
		static const long ID_STATICTEXTLOST;
		static const long ID_GLCANVAS;
		static const long ID_TOGGLEBUTTONOFFSET;
		static const long ID_PANELPLOT;
		//*)

	private:

		//(*Handlers(SensorDataExtPanel)
		void OnToggleButtonOffsetToggle(wxCommandEvent& event);
		void OnPanelPlotPaint(wxPaintEvent& event);
		//*)

		SensorNode* _sensor;
		Quaternion _offset;

		const unsigned int _numPlotPoints = 150;
		SensorBufferType<SensorDataOrientation> _buffer;
		std::list<Vector3> _plotData;

		DECLARE_EVENT_TABLE()
};

#endif
