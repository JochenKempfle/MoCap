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
#include "SensorDataExtPanel.h"
#include "SensorManager.h"
#include <math.h>

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(SensorDataExtPanel)
	#include <wx/intl.h>
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(SensorDataExtPanel)
//*)

//(*IdInit(SensorDataExtPanel)
const long SensorDataExtPanel::ID_STATICTEXTNAME = wxNewId();
const long SensorDataExtPanel::ID_STATICTEXTIP = wxNewId();
const long SensorDataExtPanel::ID_STATICTEXTSTATE = wxNewId();
const long SensorDataExtPanel::ID_GLCANVAS = wxNewId();
const long SensorDataExtPanel::ID_TOGGLEBUTTONOFFSET = wxNewId();
const long SensorDataExtPanel::ID_PANELPLOT = wxNewId();
//*)

BEGIN_EVENT_TABLE(SensorDataExtPanel,wxPanel)
	//(*EventTable(SensorDataExtPanel)
	//*)
END_EVENT_TABLE()

SensorDataExtPanel::SensorDataExtPanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(SensorDataExtPanel)
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer3;

	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	SetBackgroundColour(wxColour(187,187,187));
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	StaticBoxSizerInfo = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Sensor Info"));
	BoxSizer2 = new wxBoxSizer(wxVERTICAL);
	StaticTextName = new wxStaticText(this, ID_STATICTEXTNAME, _("Sensor Name"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXTNAME"));
	BoxSizer2->Add(StaticTextName, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticTextIP = new wxStaticText(this, ID_STATICTEXTIP, _("IP"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXTIP"));
	BoxSizer2->Add(StaticTextIP, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticTextState = new wxStaticText(this, ID_STATICTEXTSTATE, _("State"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXTSTATE"));
	BoxSizer2->Add(StaticTextState, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizerInfo->Add(BoxSizer2, 1, wxALL|wxEXPAND, 5);
	BoxSizer1->Add(StaticBoxSizerInfo, 0, wxALL|wxEXPAND, 5);
	int GLCanvasAttributes_1[] = {
		WX_GL_RGBA,
		WX_GL_DOUBLEBUFFER,
		WX_GL_DEPTH_SIZE,      16,
		WX_GL_STENCIL_SIZE,    0,
		0, 0 };
	glCanvas = new GLCanvas(this, ID_GLCANVAS, wxDefaultPosition, wxSize(200,-1), 0, _T("ID_GLCANVAS"), GLCanvasAttributes_1);
	glCanvas->SetBackgroundColour(wxColour(0,0,0));
	BoxSizer1->Add(glCanvas, 0, wxALL|wxEXPAND, 5);
	BoxSizer3 = new wxBoxSizer(wxVERTICAL);
	ToggleButtonOffset = new wxToggleButton(this, ID_TOGGLEBUTTONOFFSET, _("Set Offset"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TOGGLEBUTTONOFFSET"));
	BoxSizer3->Add(ToggleButtonOffset, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer3, 0, wxALL|wxEXPAND, 5);
	PanelPlot = new wxPanel(this, ID_PANELPLOT, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER, _T("ID_PANELPLOT"));
	PanelPlot->SetBackgroundColour(wxColour(255,255,255));
	BoxSizer1->Add(PanelPlot, 1, wxALL|wxEXPAND, 5);
	BoxSizer1->Add(-1,-1,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);

	Connect(ID_TOGGLEBUTTONOFFSET,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&SensorDataExtPanel::OnToggleButtonOffsetToggle);
	PanelPlot->Connect(wxEVT_PAINT,(wxObjectEventFunction)&SensorDataExtPanel::OnPanelPlotPaint,0,this);
	//*)
	glCanvas->setStyle(SINGLE_SENSOR_MODE);
	_updateCounter = 0;
	_sensor = nullptr;
}

SensorDataExtPanel::~SensorDataExtPanel()
{
	//(*Destroy(SensorDataExtPanel)
	//*)
}

void SensorDataExtPanel::setSensor(SensorNode* sensor)
{
    _sensor = sensor;
    _buffer.subscribe(sensor);

    wxString name;
    name << _("Sensor ") << sensor->getId();
    StaticTextName->SetLabel(name);
    StaticTextIP->SetLabel(sensor->getIPAddress());

    if (sensor->isUpdated())
    {
        StaticTextState->SetLabel(_("online"));
    }
    else
    {
        StaticTextState->SetLabel(_("offline"));
    }
}

void SensorDataExtPanel::update()
{
    if (_sensor == nullptr)
    {
        return;
    }
    // TODO(JK#3#): handle sensor is offline case for displaying
    ++_updateCounter;
    if (_updateCounter >= 10)
    {
        _updateCounter = 0;

        if (!_sensor->isUpdated())
        {
            StaticTextState->SetLabel(_("offline"));
        }
    }
	if (_sensor->isUpdated())
    {
        StaticTextState->SetLabel(_("online"));
    }

    if (ToggleButtonOffset->GetValue())
    {
        Quaternion q = _offset.inv() * _sensor->getRotation();// * _offset;
        glCanvas->setSensorOrientation(q);
        while (_buffer.size() > 0)
        {
            q = _offset.inv() * _buffer.front().getOrientation();
            _plotData.push_back(q.toEuler());
            _buffer.pop_front();
        }
    }
    else
    {
        Quaternion q = _sensor->getRotation();
        glCanvas->setSensorOrientation(q);
        while (_buffer.size() > 0)
        {
            q = _buffer.front().getOrientation();
            _plotData.push_back(q.toEuler());
            _buffer.pop_front();
        }
    }

    while (_plotData.size() > _numPlotPoints)
    {
        _plotData.pop_front();
    }

	glCanvas->Refresh();
	StaticTextState->Refresh();
	PanelPlot->Refresh();

    // StaticBoxSizerInfo->Layout();
    // StaticBoxSizerRotation->Layout();
    // StaticBoxSizerPosition->Layout();
    // Refresh();
}

void SensorDataExtPanel::OnToggleButtonOffsetToggle(wxCommandEvent& event)
{
    if (ToggleButtonOffset->GetValue())
    {
        _offset = _sensor->getRotation();
        ToggleButtonOffset->SetLabel(_("Unset Offset"));
    }
    else
    {
        ToggleButtonOffset->SetLabel(_("Set Offset"));
    }
}

void SensorDataExtPanel::OnPanelPlotPaint(wxPaintEvent& event)
{
    wxPaintDC dc(PanelPlot);
    if (!dc.IsOk())
    {
        return;
    }
    wxSize size = PanelPlot->GetSize();

    //dc.DrawRectangle(wxPoint(0, 0), size);

    size -= wxSize(2, 2);
    int x = 0;
    int y = size.y/2;
    int step = size.x/_numPlotPoints;

    dc.DrawText(_("180"), wxPoint(size.x - dc.GetTextExtent(_("180")).x, 0));
    dc.DrawText(_("0"), wxPoint(size.x - dc.GetTextExtent(_("0")).x, y - dc.GetTextExtent(_("0")).y/2));
    dc.DrawText(_("-180"), wxPoint(size.x - dc.GetTextExtent(_("-180")).x, size.y - dc.GetTextExtent(_("-180")).y));

    dc.SetPen(wxPen(wxColour(100, 100, 100), 1, wxPENSTYLE_LONG_DASH));
    dc.DrawLine(wxPoint(0, y), wxPoint(size.x - 10, y));

    dc.SetPen(wxPen(wxColour(255, 0, 0)));
    wxPoint last = wxPoint(0, y + _plotData.front().x()/M_PI * y + 1);
    for (auto it = _plotData.begin(); it != _plotData.end(); ++it)
    {
        wxPoint next(x, y + it->x()/M_PI * y);
        if (abs(next.y - last.y) < y + y/2)
        {
            dc.DrawLine(last, next);
        }
        last = next;
        x += step;
    }

    x = 0;
    dc.SetPen(wxPen(wxColour(0, 255, 0)));
    last = wxPoint(0, y + _plotData.front().y()/M_PI * y + 1);
    for (auto it = _plotData.begin(); it != _plotData.end(); ++it)
    {
        wxPoint next(x, y + it->y()/M_PI * y);
        if (abs(next.y - last.y) < y + y/2)
        {
            dc.DrawLine(last, next);
        }
        last = next;
        x += step;
    }

    x = 0;
    dc.SetPen(wxPen(wxColour(0, 0, 255)));
    last = wxPoint(0, y + _plotData.front().z()/M_PI * y + 1);
    for (auto it = _plotData.begin(); it != _plotData.end(); ++it)
    {
        wxPoint next(x, y + it->z()/M_PI * y);
        if (abs(next.y - last.y) < y + y/2)
        {
            dc.DrawLine(last, next);
        }
        last = next;
        x += step;
    }
}
