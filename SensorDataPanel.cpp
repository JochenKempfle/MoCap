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
#include "SensorDataPanel.h"
#include "MoCapManager.h"
#include "SensorManager.h"

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(SensorDataPanel)
	#include <wx/intl.h>
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(SensorDataPanel)
//*)

//(*IdInit(SensorDataPanel)
//*)

BEGIN_EVENT_TABLE(SensorDataPanel,wxPanel)
	//(*EventTable(SensorDataPanel)
	//*)
END_EVENT_TABLE()

SensorDataPanel::SensorDataPanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(SensorDataPanel)
	Create(parent, id, wxDefaultPosition, wxSize(90,40), wxNO_BORDER|wxTAB_TRAVERSAL, _T("id"));

	Connect(wxEVT_PAINT,(wxObjectEventFunction)&SensorDataPanel::OnPaint);
	Connect(wxEVT_LEFT_DOWN,(wxObjectEventFunction)&SensorDataPanel::OnLeftDown);
	Connect(wxEVT_RIGHT_DOWN,(wxObjectEventFunction)&SensorDataPanel::OnRightDown);
	//*)
	_sensorId = -1;
	_isUpdated = false;
	_hasBone = false;
	_isCalibrated = false;
	_dataExtPopup = nullptr;
}

SensorDataPanel::~SensorDataPanel()
{
	//(*Destroy(SensorDataPanel)
	//*)
}

void SensorDataPanel::update(const SensorNode* sensor)
{
    wxLongLong time = wxGetUTCTimeMillis();
    uint64_t currentTime = (uint64_t(time.GetHi()) << 32) + time.GetLo();

    // TODO(JK#4#2017-06-19): make the sensorDataPanel "is updated" - test more flexible (own function to set delay time)
    if (sensor->getLastReceiveTime() < currentTime - 1000)
    {
        _isUpdated = false;
    }
    else
    {
        _isUpdated = true;
    }
    _hasBone = sensor->hasBone();
    _isCalibrated = sensor->isCalibrated();

    if (_dataExtPopup != nullptr && _dataExtPopup->IsShownOnScreen())
    {
        _dataExtPopup->getPanel()->update();
    }
    else
    {
        if (_dataExtPopup != nullptr)
        {
            _dataExtPopup->Destroy();
            _dataExtPopup = nullptr;
        }
    }
}

void SensorDataPanel::setSensorId(int id)
{
    _sensorId = id;
}

void SensorDataPanel::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    if (!dc.IsOk())
    {
        return;
    }
    wxPen pen;
    wxBrush brush;
    wxSize size = GetSize();

    // draw background
    if (!_isUpdated)
    {
        dc.GradientFillLinear(wxRect(0, 0, size.x, size.y), wxColour(130, 20, 10), wxColour(220, 20, 10), wxUP);
    }
    else
    {
        if (_hasBone)
        {
            if (_isCalibrated)
            {
                dc.GradientFillLinear(wxRect(0, 0, size.x, size.y), wxColour(20, 100, 5), wxColour(20, 200, 5), wxUP);
            }
            else
            {
                dc.GradientFillLinear(wxRect(0, 0, size.x, size.y), wxColour(210, 110, 0), wxColour(230, 210, 0), wxUP);
            }
        }
        else
        {
            dc.GradientFillLinear(wxRect(0, 0, size.x, size.y), wxColour(150, 50, 0), wxColour(150, 150, 50), wxUP);
        }
    }

    wxString name;// = _("Sensor ");
    //name << _sensorId;

    if (_sensorId >= 0)
    {
        name << theSensorManager.getSensor(_sensorId)->getName();
    }
    wxString affiliation;

    int boneId = theMoCapManager.getBoneIdFromSensorId(_sensorId);
    if (boneId < 0)
    {
        affiliation = _("no bone");
        //dc.GradientFillLinear(wxRect(0, 0, size.x, size.y), wxColour(130, 20, 10), wxColour(220, 20, 10), wxUP);
    }
    else
    {
        affiliation = theMoCapManager.getBoneName(boneId);
        //dc.GradientFillLinear(wxRect(0, 0, size.x, size.y), wxColour(20, 100, 5), wxColour(20, 200, 5), wxUP);
    }

    dc.DrawText(name, wxPoint(10, 5));
    dc.DrawText(affiliation, wxPoint(10, 20));

    // draw a rectangle around the panel
    pen.SetColour(20, 20, 20);
    pen.SetWidth(1);
    dc.SetPen(pen);
    dc.SetBrush(*wxTRANSPARENT_BRUSH);

    dc.DrawRectangle(wxPoint(0, 0), size);
    event.Skip();
}


void SensorDataPanel::OnLeftDown(wxMouseEvent& event)
{
    auto bones = theMoCapManager.getBoneIdsWithName();
    wxMenu menu;
    menu.Append(-1, _("no bone"));
    for (size_t i = 0; i < bones.size(); ++i)
    {
        menu.Append(bones[i].first, bones[i].second);
    }
 	menu.Connect(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&SensorDataPanel::OnPopupClick, NULL, this);
 	PopupMenu(&menu);
}

void SensorDataPanel::OnPopupClick(wxCommandEvent &event)
{
    int boneId = event.GetId();
    theMoCapManager.assignSensorToBone(_sensorId, boneId);
    GetParent()->Refresh();
}

void SensorDataPanel::OnRightDown(wxMouseEvent& event)
{
    SetFocus();
    if (_dataExtPopup == nullptr)//) || !_dataExtPopup->IsOK())
    {
        _dataExtPopup = new PopupTransientWindow<SensorDataExtPanel>(this);
        _dataExtPopup->getPanel()->setSensor(theSensorManager.getSensor(_sensorId));
    }
    _dataExtPopup->Position(ClientToScreen(event.GetPosition()), wxSize(0, 0));
    _dataExtPopup->getPanel()->update();
    _dataExtPopup->Popup(this);
}



