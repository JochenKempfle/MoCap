/*Copyright (c) 2014, Jochen KempfleAll rights reserved.Redistribution and use in source and binary forms, with or without modification,are permitted provided that the following conditions are met:1. Redistributions of source code must retain the above copyright notice,this list of conditions and the following disclaimer.2. Redistributions in binary form must reproduce the above copyright notice,this list of conditions and the following disclaimer in the documentation and/orother materials provided with the distribution.THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" ANDANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIEDWARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITYOF SUCH DAMAGE.*/#ifndef SENSORDATAEXTPANEL_H
#define SENSORDATAEXTPANEL_H

#ifndef WX_PRECOMP
	//(*HeadersPCH(SensorDataExtPanel)
	#include <wx/sizer.h>
	#include <wx/stattext.h>
	#include <wx/panel.h>
	//*)
#endif
//(*Headers(SensorDataExtPanel)
//*)#include "SensorNode.h"

class SensorDataExtPanel: public wxPanel
{
	public:

		SensorDataExtPanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~SensorDataExtPanel();		void update(const SensorNode &sensor);

		//(*Declarations(SensorDataExtPanel)
		wxStaticText* StaticText10;
		wxStaticText* StaticText9;
		wxStaticText* StaticText2;
		wxStaticText* StaticTextPitch;
		wxStaticText* StaticTextYaw;
		wxStaticText* StaticText6;
		wxStaticBoxSizer* StaticBoxSizerRotation;
		wxStaticText* StaticText8;
		wxStaticText* StaticText11;
		wxStaticBoxSizer* StaticBoxSizerPosition;
		wxStaticText* StaticText1;
		wxStaticText* StaticTextRoll;
		wxStaticText* StaticTextZ;
		wxStaticText* StaticText7;
		wxStaticText* StaticTextY;
		wxStaticBoxSizer* StaticBoxSizerInfo;
		wxStaticText* StaticText15;
		wxStaticText* StaticTextX;
		//*)

	protected:

		//(*Identifiers(SensorDataExtPanel)
		static const long ID_STATICTEXT1;
		static const long ID_STATICTEXT2;
		static const long ID_STATICTEXT15;
		static const long ID_STATICTEXT6;
		static const long ID_STATICTEXT7;
		static const long ID_STATICTEXT8;
		static const long ID_STATICTEXTPITCH;
		static const long ID_STATICTEXTYAW;
		static const long ID_STATICTEXTROLL;
		static const long ID_STATICTEXT9;
		static const long ID_STATICTEXT10;
		static const long ID_STATICTEXT11;
		static const long ID_STATICTEXTX;
		static const long ID_STATICTEXTY;
		static const long ID_STATICTEXTZ;
		//*)

	private:

		//(*Handlers(SensorDataExtPanel)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
