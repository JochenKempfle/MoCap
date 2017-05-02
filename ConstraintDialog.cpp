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
#include "ConstraintDialog.h"
#include <wx/dcbuffer.h>

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(ConstraintDialog)
	#include <wx/intl.h>
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(ConstraintDialog)
//*)

//(*IdInit(ConstraintDialog)
const long ConstraintDialog::ID_GLCANVAS = wxNewId();
const long ConstraintDialog::ID_BUTTON1 = wxNewId();
const long ConstraintDialog::ID_BUTTON2 = wxNewId();
const long ConstraintDialog::ID_PANELSETCONSTRAINT = wxNewId();
const long ConstraintDialog::ID_PANEL1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(ConstraintDialog,wxDialog)
	//(*EventTable(ConstraintDialog)
	//*)
END_EVENT_TABLE()

ConstraintDialog::ConstraintDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(ConstraintDialog)
	wxBoxSizer* BoxSizer4;
	wxBoxSizer* BoxSizer5;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer3;

	Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	Panel1 = new wxPanel(this, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	BoxSizer5 = new wxBoxSizer(wxVERTICAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	glCanvas = new GLCanvas(static_cast<wxWindow*>(Panel1),ID_GLCANVAS,wxDefaultPosition,wxSize(400,300),0,_T("ID_GLCANVAS"));
	BoxSizer2->Add(glCanvas, 0, wxALL|wxEXPAND, 5);
	BoxSizer4 = new wxBoxSizer(wxVERTICAL);
	Button1 = new wxButton(Panel1, ID_BUTTON1, _("Label"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
	BoxSizer4->Add(Button1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	Button2 = new wxButton(Panel1, ID_BUTTON2, _("Label"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
	BoxSizer4->Add(Button2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2->Add(BoxSizer4, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer5->Add(BoxSizer2, 1, wxALL|wxEXPAND, 5);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	PanelSetConstraint = new wxPanel(Panel1, ID_PANELSETCONSTRAINT, wxDefaultPosition, wxSize(723,363), wxSIMPLE_BORDER, _T("ID_PANELSETCONSTRAINT"));
	BoxSizer3->Add(PanelSetConstraint, 1, wxALL|wxEXPAND, 5);
	BoxSizer5->Add(BoxSizer3, 0, wxALL|wxEXPAND, 5);
	Panel1->SetSizer(BoxSizer5);
	BoxSizer5->Fit(Panel1);
	BoxSizer5->SetSizeHints(Panel1);
	BoxSizer1->Add(Panel1, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);

	PanelSetConstraint->Connect(wxEVT_PAINT,(wxObjectEventFunction)&ConstraintDialog::OnPanelSetConstraintPaint,0,this);
	PanelSetConstraint->Connect(wxEVT_LEFT_DOWN,(wxObjectEventFunction)&ConstraintDialog::OnPanelSetConstraintLeftDown,0,this);
	PanelSetConstraint->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&ConstraintDialog::OnPanelSetConstraintLeftUp,0,this);
	PanelSetConstraint->Connect(wxEVT_RIGHT_DOWN,(wxObjectEventFunction)&ConstraintDialog::OnPanelSetConstraintRightDown,0,this);
	PanelSetConstraint->Connect(wxEVT_MOTION,(wxObjectEventFunction)&ConstraintDialog::OnPanelSetConstraintMouseMove,0,this);
	//*)

	PanelSetConstraint->Connect(wxEVT_MOUSE_CAPTURE_LOST, (wxObjectEventFunction)&ConstraintDialog::OnPanelSetConstraintMouseCaptureLost);

    // important for double buffered paint dc
	PanelSetConstraint->SetBackgroundStyle(wxBG_STYLE_PAINT);

	glCanvas->setStyleFlag(SINGLE_JOINT_MODE);

	_pointIterator = _points.end();

	_constraint.addConstraint(20, 10);
	_constraint.addConstraint(90, 40);
	_constraint.addConstraint(180, 70);
	_constraint.addConstraint(270, 80);
	_constraint.addConstraint(Vector3(0, 1, 1));

	setConstraint(_constraint);
//	_points[40] = 360 - 20;
//	_points[180] = 360 - 80;
//	_points[360] = 360 - 140;
//	_points[540] = 360 - 160;
	Refresh();
}

ConstraintDialog::~ConstraintDialog()
{
	//(*Destroy(ConstraintDialog)
	//*)
}

void ConstraintDialog::setConstraint(const JointConstraint &constraint)
{
    _constraint = constraint;
    glCanvas->setConstraint(&_constraint);

    _points.clear();
    auto points = constraint.getAxisAngleToAngleKeyPoints();
    for (size_t i = 0; i < points.size(); ++i)
    {
        _points[std::round(2.0f * points[i].first)] = (360 - std::round(2.0f * points[i].second));
    }
}

JointConstraint ConstraintDialog::getConstraint() const
{
    JointConstraint constraint;
    for (auto it = _points.begin(); it != _points.end(); ++it)
    {
        constraint.addConstraint(float(it->first) / 2.0f, float(360 - it->second) / 2.0f);
    }
    return constraint;
}


void ConstraintDialog::OnPanelSetConstraintPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(PanelSetConstraint);
    if (!dc.IsOk())
    {
        return;
    }
    dc.Clear();


    wxPen redPen = wxPen(wxColour(255, 0, 0));
    wxPen blackPen = wxPen(wxColour(0, 0, 0));
    dc.SetPen(redPen);

    if (_points.size() > 0)
    {
        /*
        auto angles = _constraint.getAxisAngleToAngleKeyPoints();

        if (angles.size() == 0)
        {
            return;
        }

        wxPoint pt1, pt2;

        pt1 = wxPoint(std::round(angles[0].first), 180 - std::round(angles[0].second));

        float interpolationValue = float(360 - angles.back().first) / (360 + angles[0].first - angles.back().first);
        int startEndAngle = angles.back().second + (angles[0].second - angles.back().second) * interpolationValue;

        dc.SetBrush(*wxTRANSPARENT_BRUSH);

        dc.DrawLine(2*wxPoint(0, 180 - startEndAngle), 2*pt1);
        for (size_t i = 1; i < angles.size(); ++i)
        {
            pt2 = wxPoint(angles[i].first, 180 - angles[i].second);
            dc.DrawLine(2*pt1, 2*pt2);

            dc.SetPen(blackPen);
            dc.DrawCircle(2*pt1, 2);
            dc.SetPen(redPen);

            pt1 = pt2;
        }
        // draw the line to 360°
        dc.DrawLine(2*pt1, 2*wxPoint(360, 180 - startEndAngle));
        dc.SetPen(blackPen);
        dc.DrawCircle(2*pt1, 2);
        */

        dc.SetBrush(*wxTRANSPARENT_BRUSH);

        float interpolationValue = float(720 - _points.rbegin()->first) / (720 + _points.begin()->first - _points.rbegin()->first);
        int startEndAngle = _points.rbegin()->second + (_points.begin()->second - _points.rbegin()->second) * interpolationValue;

        auto itPrev = _points.begin();
        dc.DrawLine(0, startEndAngle, itPrev->first, itPrev->second);
        for (auto it = _points.begin(); it != _points.end(); ++it)
        {
            dc.SetPen(blackPen);
            dc.DrawCircle(it->first, it->second, 2);
            dc.SetPen(redPen);

            dc.DrawLine(itPrev->first, itPrev->second, it->first, it->second);
            itPrev = it;
        }

        dc.DrawLine(720, startEndAngle, itPrev->first, itPrev->second);
    }

    dc.SetPen(blackPen);

    wxString text;
    text << _mousePos.x / 2 << _(".") << (_mousePos.x % 2) * 5;
    int textExtent = dc.GetTextExtent(text).x;
    wxPoint textPos = wxPoint(_mousePos.x < textExtent + 15 ? _mousePos.x - textExtent + 35 : _mousePos.x - textExtent - 15, _mousePos.y < 360 - 20 ? _mousePos.y + 4 : _mousePos.y - 15);
    dc.DrawText(text, textPos);

    text.Clear();
    text << 180 - (_mousePos.y + 1) / 2 << _(".") << (_mousePos.y % 2) * 5;
    textExtent = dc.GetTextExtent(text).x;
    textPos = wxPoint(_mousePos.x < 720 - 35 ? _mousePos.x + 5 : _mousePos.x - textExtent - 5, _mousePos.y < 35 ? _mousePos.y + 35 : _mousePos.y - 35);
    dc.DrawText(text, textPos);

    // draw mouse cursor lines
    dc.DrawLine(_mousePos.x, 0, _mousePos.x, 361);
    dc.DrawLine(0, _mousePos.y, 721, _mousePos.y);

    event.Skip();
}


void ConstraintDialog::OnPanelSetConstraintLeftDown(wxMouseEvent& event)
{
    wxPoint pos = event.GetPosition();
    if (pos.y > 360)
    {
        pos.y = 360;
    }

    _points[pos.x] = pos.y;

    // _constraint.addConstraint(0.5f * pos.x, 0.5f);

    auto it = _points.find(pos.x);
    if (it != _points.end())
    {
        _pointIterator = it;
        if (!PanelSetConstraint->HasCapture())
        {
            PanelSetConstraint->CaptureMouse();
        }
    }

    Refresh();
}

void ConstraintDialog::OnPanelSetConstraintRightDown(wxMouseEvent& event)
{
    wxPoint pos = event.GetPosition();
    if (pos.y > 360)
    {
        pos.y = 360;
    }
    // _constraint.removeConstraint(0.5f * pos.x, 0.5f);

    auto it = _points.lower_bound(pos.x - 2);
    if (it != _points.end())
    {
        if (std::abs(pos.x - it->first) < 3 && std::abs(pos.y - it->second) < 3)
        {
            _points.erase(it);
        }
        _constraint = getConstraint();
    }

    Refresh();
}

void ConstraintDialog::OnPanelSetConstraintMouseMove(wxMouseEvent& event)
{
    _mousePos = event.GetPosition();

    if (_mousePos.x < 0)
    {
        _mousePos.x = 0;
    }
    else if (_mousePos.x > 720)
    {
        _mousePos.x = 720;
    }
    if (_mousePos.y < 0)
    {
        _mousePos.y = 0;
    }
    else if (_mousePos.y > 360)
    {
        _mousePos.y = 360;
    }

    if (_pointIterator != _points.end())
    {
        _points.erase(_pointIterator);
        _points[_mousePos.x] = _mousePos.y;
        _pointIterator = _points.find(_mousePos.x);
    }

    Refresh();
}

void ConstraintDialog::OnPanelSetConstraintLeftUp(wxMouseEvent& event)
{
    _pointIterator = _points.end();

    if (PanelSetConstraint->HasCapture())
    {
        PanelSetConstraint->ReleaseMouse();
    }
    _constraint = getConstraint();
    Refresh();
}

void ConstraintDialog::OnPanelSetConstraintMouseCaptureLost(wxMouseCaptureLostEvent& event)
{
    _pointIterator = _points.end();
    if (PanelSetConstraint->HasCapture())
    {
        PanelSetConstraint->ReleaseMouse();
    }
}
