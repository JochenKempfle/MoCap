/*Copyright (c) 2014, Jochen KempfleAll rights reserved.Redistribution and use in source and binary forms, with or without modification,are permitted provided that the following conditions are met:1. Redistributions of source code must retain the above copyright notice,this list of conditions and the following disclaimer.2. Redistributions in binary form must reproduce the above copyright notice,this list of conditions and the following disclaimer in the documentation and/orother materials provided with the distribution.THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" ANDANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIEDWARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITYOF SUCH DAMAGE.*/#include "wx_pch.h"
#include "SensorDataExtPanel.h"#include <math.h>

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(SensorDataExtPanel)
	#include <wx/intl.h>
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(SensorDataExtPanel)
//*)

//(*IdInit(SensorDataExtPanel)
const long SensorDataExtPanel::ID_STATICTEXT1 = wxNewId();
const long SensorDataExtPanel::ID_STATICTEXT2 = wxNewId();
const long SensorDataExtPanel::ID_STATICTEXT15 = wxNewId();
const long SensorDataExtPanel::ID_STATICTEXT6 = wxNewId();
const long SensorDataExtPanel::ID_STATICTEXT7 = wxNewId();
const long SensorDataExtPanel::ID_STATICTEXT8 = wxNewId();
const long SensorDataExtPanel::ID_STATICTEXTPITCH = wxNewId();
const long SensorDataExtPanel::ID_STATICTEXTYAW = wxNewId();
const long SensorDataExtPanel::ID_STATICTEXTROLL = wxNewId();
const long SensorDataExtPanel::ID_STATICTEXT9 = wxNewId();
const long SensorDataExtPanel::ID_STATICTEXT10 = wxNewId();
const long SensorDataExtPanel::ID_STATICTEXT11 = wxNewId();
const long SensorDataExtPanel::ID_STATICTEXTX = wxNewId();
const long SensorDataExtPanel::ID_STATICTEXTY = wxNewId();
const long SensorDataExtPanel::ID_STATICTEXTZ = wxNewId();
//*)

BEGIN_EVENT_TABLE(SensorDataExtPanel,wxPanel)
	//(*EventTable(SensorDataExtPanel)
	//*)
END_EVENT_TABLE()

SensorDataExtPanel::SensorDataExtPanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(SensorDataExtPanel)
	wxBoxSizer* BoxSizer5;
	wxBoxSizer* BoxSizer7;
	wxBoxSizer* BoxSizer8;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer3;

	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	SetBackgroundColour(wxColour(187,187,187));
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	StaticBoxSizerInfo = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Sensor Info"));
	BoxSizer2 = new wxBoxSizer(wxVERTICAL);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Name"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	BoxSizer2->Add(StaticText1, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Affiliation"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	BoxSizer2->Add(StaticText2, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText15 = new wxStaticText(this, ID_STATICTEXT15, _("State"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT15"));
	BoxSizer2->Add(StaticText15, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizerInfo->Add(BoxSizer2, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(StaticBoxSizerInfo, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizerRotation = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Rotation"));
	BoxSizer5 = new wxBoxSizer(wxVERTICAL);
	StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("pitch"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	BoxSizer5->Add(StaticText6, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _("yaw"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	BoxSizer5->Add(StaticText7, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _("roll"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	BoxSizer5->Add(StaticText8, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizerRotation->Add(BoxSizer5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer3 = new wxBoxSizer(wxVERTICAL);
	StaticTextPitch = new wxStaticText(this, ID_STATICTEXTPITCH, _("0"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXTPITCH"));
	BoxSizer3->Add(StaticTextPitch, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticTextYaw = new wxStaticText(this, ID_STATICTEXTYAW, _("0"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXTYAW"));
	BoxSizer3->Add(StaticTextYaw, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticTextRoll = new wxStaticText(this, ID_STATICTEXTROLL, _("0"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXTROLL"));
	BoxSizer3->Add(StaticTextRoll, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizerRotation->Add(BoxSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(StaticBoxSizerRotation, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizerPosition = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Position"));
	BoxSizer7 = new wxBoxSizer(wxVERTICAL);
	StaticText9 = new wxStaticText(this, ID_STATICTEXT9, _("x"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT9"));
	BoxSizer7->Add(StaticText9, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText10 = new wxStaticText(this, ID_STATICTEXT10, _("y"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT10"));
	BoxSizer7->Add(StaticText10, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText11 = new wxStaticText(this, ID_STATICTEXT11, _("z"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT11"));
	BoxSizer7->Add(StaticText11, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizerPosition->Add(BoxSizer7, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer8 = new wxBoxSizer(wxVERTICAL);
	StaticTextX = new wxStaticText(this, ID_STATICTEXTX, _("0"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXTX"));
	BoxSizer8->Add(StaticTextX, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticTextY = new wxStaticText(this, ID_STATICTEXTY, _("0"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXTY"));
	BoxSizer8->Add(StaticTextY, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticTextZ = new wxStaticText(this, ID_STATICTEXTZ, _("0"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXTZ"));
	BoxSizer8->Add(StaticTextZ, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizerPosition->Add(BoxSizer8, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(StaticBoxSizerPosition, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(-1,-1,2, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	//*)
}

SensorDataExtPanel::~SensorDataExtPanel()
{
	//(*Destroy(SensorDataExtPanel)
	//*)
}
void SensorDataExtPanel::update(const SensorNode &sensor){    Vector3 rotation = sensor.toEuler();    wxString value;    value << rotation.x()*180.0/M_PI;    StaticTextPitch->SetLabel(value);    value.clear();    value << rotation.y()*180.0/M_PI;    StaticTextYaw->SetLabel(value);    value.clear();    value << rotation.z()*180.0/M_PI;    StaticTextRoll->SetLabel(value);    value.clear();    // StaticBoxSizerInfo->Layout();    // StaticBoxSizerRotation->Layout();    // StaticBoxSizerPosition->Layout();    // Refresh();}

