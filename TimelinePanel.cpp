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
#include "TimelinePanel.h"
#include <wx/dcbuffer.h>

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(TimelinePanel)
	#include <wx/intl.h>
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(TimelinePanel)
//*)

//(*IdInit(TimelinePanel)
const long TimelinePanel::ID_BUTTONCLEAR = wxNewId();
const long TimelinePanel::ID_BUTTONREMOVE = wxNewId();
const long TimelinePanel::ID_BUTTONCUT = wxNewId();
const long TimelinePanel::ID_BUTTONZOOMIN = wxNewId();
const long TimelinePanel::ID_BUTTONZOOMOUT = wxNewId();
//*)

BEGIN_EVENT_TABLE(TimelinePanel,wxPanel)
	//(*EventTable(TimelinePanel)
	//*)
END_EVENT_TABLE()

TimelinePanel::TimelinePanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
    SetBackgroundStyle(wxBG_STYLE_PAINT);
	//(*Initialize(TimelinePanel)
	wxBoxSizer* BoxSizer1;

	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxFULL_REPAINT_ON_RESIZE, _T("id"));
	SetBackgroundColour(wxColour(120,120,120));
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer1->Add(100,10,0, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 0);
	ButtonClear = new wxButton(this, ID_BUTTONCLEAR, _("Clear All"), wxDefaultPosition, wxSize(-1,26), 0, wxDefaultValidator, _T("ID_BUTTONCLEAR"));
	BoxSizer1->Add(ButtonClear, 0, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 2);
	ButtonRemove = new wxButton(this, ID_BUTTONREMOVE, _("Remove"), wxDefaultPosition, wxSize(-1,26), 0, wxDefaultValidator, _T("ID_BUTTONREMOVE"));
	BoxSizer1->Add(ButtonRemove, 0, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 2);
	ButtonCut = new wxButton(this, ID_BUTTONCUT, _("Cut"), wxDefaultPosition, wxSize(-1,26), 0, wxDefaultValidator, _T("ID_BUTTONCUT"));
	BoxSizer1->Add(ButtonCut, 0, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 2);
	BoxSizer1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonZoomIn = new wxButton(this, ID_BUTTONZOOMIN, _("Zoom +"), wxDefaultPosition, wxSize(-1,26), 0, wxDefaultValidator, _T("ID_BUTTONZOOMIN"));
	BoxSizer1->Add(ButtonZoomIn, 0, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 2);
	ButtonZoomOut = new wxButton(this, ID_BUTTONZOOMOUT, _("Zoom -"), wxDefaultPosition, wxSize(-1,26), 0, wxDefaultValidator, _T("ID_BUTTONZOOMOUT"));
	BoxSizer1->Add(ButtonZoomOut, 0, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 2);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);

	Connect(ID_BUTTONZOOMIN,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&TimelinePanel::OnButtonZoomInClick);
	Connect(ID_BUTTONZOOMOUT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&TimelinePanel::OnButtonZoomOutClick);
	Connect(wxEVT_PAINT,(wxObjectEventFunction)&TimelinePanel::OnPaint);
	Connect(wxEVT_KILL_FOCUS,(wxObjectEventFunction)&TimelinePanel::OnKillFocus);
	Connect(wxEVT_LEFT_DOWN,(wxObjectEventFunction)&TimelinePanel::OnLeftDown);
	Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&TimelinePanel::OnLeftUp);
	Connect(wxEVT_MOTION,(wxObjectEventFunction)&TimelinePanel::OnMouseMove);
	Connect(wxEVT_ENTER_WINDOW,(wxObjectEventFunction)&TimelinePanel::OnMouseEnter);
	Connect(wxEVT_LEAVE_WINDOW,(wxObjectEventFunction)&TimelinePanel::OnMouseLeave);
	Connect(wxEVT_MOUSEWHEEL,(wxObjectEventFunction)&TimelinePanel::OnMouseWheel);
	//*)
	_msPerTimeUnit = 1000;
	_timeOffset = 0;
	_channelOffset = 0;
	_cursorPosition = 0;
	_clickedTime = false;
	_clickedChannel = -1;
}

TimelinePanel::~TimelinePanel()
{
	//(*Destroy(TimelinePanel)
	//*)
}


void TimelinePanel::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    if (!dc.IsOk())
    {
        return;
    }
    dc.Clear();
    wxPen pen;
    wxBrush brush;
    wxSize size = GetSize();

    pen.SetColour(0, 0, 0);
    pen.SetWidth(1);
    dc.SetPen(pen);
    dc.SetBrush(*wxTRANSPARENT_BRUSH);

    wxFont font = dc.GetFont();
    font.SetPointSize(8);
    dc.SetFont(font);

    // draw sorting area
    brush.SetColour(200, 200, 200);
    dc.SetBrush(brush);
    dc.DrawRectangle(wxPoint(0, 0), wxSize(_channelOptionsWidth + 1, _optionsHeight + _timeHeight + 1));
    dc.DrawText(_("Sort by"), wxPoint(5, 5));

    // draw options
    dc.DrawRectangle(wxPoint(_channelOptionsWidth, 0), wxSize(size.x, _optionsHeight + 1));

    // dc.DrawLine(wxPoint(size.x - 500, _optionsHeight), wxPoint(size.x - 500, _optionsHeight - 20));
    // dc.DrawLine(wxPoint(size.x - 450, _optionsHeight), wxPoint(size.x - 450, _optionsHeight - 20));
    // dc.DrawRectangle(wxPoint(size.x - 500, 0), wxSize(50, _optionsHeight + 1));
    // font.SetPointSize(25);
    // dc.SetFont(font);
    // dc.DrawText(_("Zoom +"), wxPoint(size.x - 500 + 4, 10));

    font.SetPointSize(8);
    dc.SetFont(font);

    // draw frame times
    brush.SetColour(220, 220, 220);
    dc.SetBrush(brush);
    dc.DrawRectangle(wxPoint(_channelOptionsWidth, _optionsHeight), wxSize(size.x, _timeHeight + 1));

    // reset brush
    dc.SetBrush(*wxTRANSPARENT_BRUSH);

    int numTimeUnits = size.x/_numPixelsPerTimeUnit + 1;
    for (int i = 0; i < numTimeUnits; ++i)
    {
        int time = i * _msPerTimeUnit + _timeOffset;
        int min = time / (60 * 1000);
        int sec = (time / 1000) % 60;
        int ms = time % 1000;
        wxString timeLabel;
        timeLabel << (min < 10 ? _("0") : _("")) << min << (sec < 10 ? _(":0") : _(":")) << sec;
        timeLabel << (ms < 100 ? (ms < 10 ? _(":00") : _(":0")) : _(":")) << ms;
        int timeUnitPos = _channelOptionsWidth + i * _numPixelsPerTimeUnit;
        dc.DrawText(timeLabel, wxPoint(timeUnitPos + 2, _optionsHeight + 2));
        // main time units (long line)
        dc.DrawLine(wxPoint(timeUnitPos, _optionsHeight + 5), wxPoint(_channelOptionsWidth + i*100, _optionsHeight + _timeHeight));
        // sub time units at every half time unit (medium line)
        dc.DrawLine(wxPoint(timeUnitPos + 50, _optionsHeight + 10), wxPoint(timeUnitPos + 50, _optionsHeight + _timeHeight));
        // sub time units at every 1/10 time units (short lines)
        for (int j = 0; j < 10; ++j)
        {
            dc.DrawLine(wxPoint(timeUnitPos + j*10, _optionsHeight + 15), wxPoint(timeUnitPos + j*10, _optionsHeight + _timeHeight));
        }
    }

    // draw channel options (name etc)
    int numChannels = size.y/_channelHeight + 1;
    for (int i = 0; i < numChannels; ++i)
    {
        wxString channelName;
        channelName << _("Channel ") << i + _channelOffset;
        dc.GradientFillLinear(wxRect(0, _optionsHeight + _timeHeight + i*_channelHeight, _channelOptionsWidth, _channelHeight), wxColour(50, 50, 50), wxColour(200, 200, 200), wxUP);
        dc.DrawLine(wxPoint(0, _optionsHeight + _timeHeight + i*_channelHeight), wxPoint(size.x, _optionsHeight + _timeHeight + i*_channelHeight));
        dc.DrawText(channelName, wxPoint(5, _optionsHeight + _timeHeight + i*_channelHeight + 5));
    }

    dc.DrawLine(wxPoint(_channelOptionsWidth, 0), wxPoint(_channelOptionsWidth, size.y));

    // draw the time cursor
    int cursorPosition = _channelOptionsWidth + (_numPixelsPerTimeUnit * (_cursorPosition - _timeOffset)) / _msPerTimeUnit;
    if (cursorPosition >= _channelOptionsWidth)
    {
        pen.SetWidth(1);
        pen.SetColour(255, 10, 0);
        brush.SetColour(255, 10, 0);
        dc.SetPen(pen);
        dc.SetBrush(brush);

        dc.DrawLine(wxPoint(cursorPosition, _optionsHeight + 5), wxPoint(cursorPosition, size.y));
        // dc.DrawLine(wxPoint(_channelOptionsWidth + 275, _optionsHeight + 5), wxPoint(_channelOptionsWidth + 275, _optionsHeight + _timeHeight));


        wxPoint points[4] = {wxPoint(cursorPosition - 5, _optionsHeight + 5),
                             wxPoint(cursorPosition, _optionsHeight + _timeHeight),
                             wxPoint(cursorPosition + 5, _optionsHeight + 5),
                             wxPoint(cursorPosition - 5, _optionsHeight + 5)};
        dc.DrawPolygon(4, points);
    }

    // draw rectangle around whole control
    pen.SetWidth(1);
    pen.SetColour(0, 0, 0);
    dc.SetPen(pen);
    dc.SetBrush(*wxTRANSPARENT_BRUSH);

    dc.DrawRectangle(wxPoint(0, 0), size);
}



void TimelinePanel::OnLeftDown(wxMouseEvent& event)
{
    wxPoint pos = event.GetPosition();
    if (pos.x < _channelOptionsWidth)
    {
        if (pos.y < _optionsHeight + _timeHeight)
        {

        }
        else
        {
            int channelId = (pos.y - (_optionsHeight + _timeHeight))/_channelHeight + _channelOffset;
            wxString msg;
            msg << channelId;
            wxMessageBox(msg);
        }
    }
    else
    {
        if (pos.y < _optionsHeight)
        {

        }
        else if (pos.y < _optionsHeight + _timeHeight)
        {
            _cursorPosition = _timeOffset + (_msPerTimeUnit * (pos.x - _channelOptionsWidth)) / _numPixelsPerTimeUnit;
            _clickedTime = true;
            _clickedChannel = -1;
        }
        else
        {
            int channelId = (pos.y - (_optionsHeight + _timeHeight))/_channelHeight + _channelOffset;
            _clickedTime = false;
            _clickedChannel = -1;
        }
    }
    Refresh();
}

void TimelinePanel::OnLeftUp(wxMouseEvent& event)
{
    _clickedTime = false;
    _clickedChannel = -1;
    // SetCursor(wxCURSOR_ARROW);
}

void TimelinePanel::OnMouseMove(wxMouseEvent& event)
{
    wxPoint pos = event.GetPosition();
    if (_clickedChannel >= 0)
    {
        Refresh();
    }
    else if (_clickedTime)
    {
        _cursorPosition = _timeOffset + (_msPerTimeUnit * (pos.x - _channelOptionsWidth)) / _numPixelsPerTimeUnit;
        if (_cursorPosition < _timeOffset)
        {
            _cursorPosition = _timeOffset;
        }
        Refresh();
    }
}

void TimelinePanel::OnMouseWheel(wxMouseEvent& event)
{
    wxPoint pos = event.GetPosition();
    if (pos.y < 0)
    {
        return;
    }
    if (pos.x < _channelOptionsWidth)
    {
        _channelOffset -= event.GetWheelRotation()/event.GetWheelDelta();
        if (_channelOffset <= 0)
        {
            _channelOffset = 0;
        }
    }
    else
    {
        _timeOffset -= _msPerTimeUnit * (event.GetWheelRotation()/event.GetWheelDelta());
        if (_timeOffset <= 0)
        {
            _timeOffset = 0;
        }
    }
    Refresh();
}

void TimelinePanel::OnMouseEnter(wxMouseEvent& event)
{
    SetFocus();
}

void TimelinePanel::OnMouseLeave(wxMouseEvent& event)
{
    _clickedTime = false;
    _clickedChannel = -1;
}

void TimelinePanel::OnKillFocus(wxFocusEvent& event)
{
    _clickedTime = false;
    _clickedChannel = -1;
}

void TimelinePanel::OnButtonZoomInClick(wxCommandEvent& event)
{
    switch (_msPerTimeUnit)
    {
        case 20:
            _msPerTimeUnit = 10;
            break;
        case 50:
            _msPerTimeUnit = 20;
            break;
        case 100:
            _msPerTimeUnit = 50;
            break;
        case 200:
            _msPerTimeUnit = 100;
            break;
        case 500:
            _msPerTimeUnit = 200;
            break;
        case 1000:
            _msPerTimeUnit = 500;
            break;
        case 2000:
            _msPerTimeUnit = 1000;
            break;
        case 5000:
            _msPerTimeUnit = 2000;
            break;
        case 10000:
            _msPerTimeUnit = 5000;
            break;
        case 20000:
            _msPerTimeUnit = 10000;
            break;
        case 30000:
            _msPerTimeUnit = 20000;
            break;
        case 60000:
            _msPerTimeUnit = 30000;
            break;
        case 120000:
            _msPerTimeUnit = 60000;
            break;
        case 300000:
            _msPerTimeUnit = 120000;
            break;
        case 600000:
            _msPerTimeUnit = 300000;
            break;
        default:
            break;
    }
    Refresh();
}

void TimelinePanel::OnButtonZoomOutClick(wxCommandEvent& event)
{
    switch (_msPerTimeUnit)
    {
        case 10:
            _msPerTimeUnit = 20;
            _timeOffset /= 20;
            _timeOffset *= 20;
            break;
        case 20:
            _msPerTimeUnit = 50;
            _timeOffset /= 50;
            _timeOffset *= 50;
            break;
        case 50:
            _msPerTimeUnit = 100;
            _timeOffset /= 100;
            _timeOffset *= 100;
            break;
        case 100:
            _msPerTimeUnit = 200;
            _timeOffset /= 200;
            _timeOffset *= 200;
            break;
        case 200:
            _msPerTimeUnit = 500;
            _timeOffset /= 500;
            _timeOffset *= 500;
            break;
        case 500:
            _msPerTimeUnit = 1000;
            _timeOffset /= 1000;
            _timeOffset *= 1000;
            break;
        case 1000:
            _msPerTimeUnit = 2000;
            _timeOffset /= 2000;
            _timeOffset *= 2000;
            break;
        case 2000:
            _msPerTimeUnit = 5000;
            _timeOffset /= 5000;
            _timeOffset *= 5000;
            break;
        case 5000:
            _msPerTimeUnit = 10000;
            _timeOffset /= 10000;
            _timeOffset *= 10000;
            break;
        case 10000:
            _msPerTimeUnit = 20000;
            _timeOffset /= 20000;
            _timeOffset *= 20000;
            break;
        case 20000:
            _msPerTimeUnit = 30000;
            _timeOffset /= 30000;
            _timeOffset *= 30000;
            break;
        case 30000:
            _msPerTimeUnit = 60000;
            _timeOffset /= 60000;
            _timeOffset *= 60000;
            break;
        case 60000:
            _msPerTimeUnit = 120000;
            _timeOffset /= 120000;
            _timeOffset *= 120000;
            break;
        case 120000:
            _msPerTimeUnit = 300000;
            _timeOffset /= 300000;
            _timeOffset *= 300000;
            break;
        case 300000:
            _msPerTimeUnit = 600000;
            _timeOffset /= 600000;
            _timeOffset *= 600000;
            break;
        default:
            break;
    }
    Refresh();
}



