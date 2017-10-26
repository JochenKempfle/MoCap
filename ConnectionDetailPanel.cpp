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
#include "ConnectionDetailPanel.h"

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(ConnectionDetailPanel)
	#include <wx/intl.h>
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(ConnectionDetailPanel)
//*)

//(*IdInit(ConnectionDetailPanel)
const long ConnectionDetailPanel::ID_BUTTONCONNECTION = wxNewId();
//*)

BEGIN_EVENT_TABLE(ConnectionDetailPanel,wxPanel)
	//(*EventTable(ConnectionDetailPanel)
	//*)
END_EVENT_TABLE()

ConnectionDetailPanel::ConnectionDetailPanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(ConnectionDetailPanel)
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer3;

	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	ButtonConnection = new wxButton(this, ID_BUTTONCONNECTION, _("Connect"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONCONNECTION"));
	BoxSizer3->Add(ButtonConnection, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);

	Connect(ID_BUTTONCONNECTION,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ConnectionDetailPanel::OnButtonConnectionClick);
	Connect(wxEVT_PAINT,(wxObjectEventFunction)&ConnectionDetailPanel::OnPaint);
	Connect(wxEVT_ENTER_WINDOW,(wxObjectEventFunction)&ConnectionDetailPanel::OnMouseEnter);
	Connect(wxEVT_LEAVE_WINDOW,(wxObjectEventFunction)&ConnectionDetailPanel::OnMouseLeave);
	//*)

	_connected = true;
    _mouseInside = false;
    _selected = false;

    _name = _("UDP");
    _info = _("invalid IP : invalid Service");
    _receiver = nullptr;
}

ConnectionDetailPanel::~ConnectionDetailPanel()
{
	//(*Destroy(ConnectionDetailPanel)
	//*)
}


void ConnectionDetailPanel::setReceiver(ReceiverBase* recv)
{
    _receiver = recv;
    _name = _receiver->getName();
    _info = _receiver->getInfo();
    _connected = _receiver->isConnected();
    if (_connected)
    {
        ButtonConnection->SetLabel(_("Disconnect"));
    }
    Refresh();
}

void ConnectionDetailPanel::OnButtonConnectionClick(wxCommandEvent& event)
{
    if (_connected)
    {
        disconnect();
    }
    else
    {
        connect();
    }

    if (_connected)
    {
        connect();
    }
    else
    {
    }

    Refresh();
}

bool ConnectionDetailPanel::connect()
{
    if (_connected)
    {
        return true;
    }
    else if (_receiver == nullptr)
    {
        return false;
    }
    else if (!_receiver->connect())
    {
        wxString msg;
        msg << _("Unable to connect to ") << _receiver->getName() << _(" ") << _receiver->getInfo();
        wxMessageBox(msg);
        return false;
    }
    ButtonConnection->SetLabel(_("Disconnect"));
    _connected = true;
    Refresh();
    return true;
}

void ConnectionDetailPanel::disconnect()
{
    if (_receiver == nullptr)
    {
        return;
    }
    _receiver->disconnect();
    ButtonConnection->SetLabel(_("Connect"));
    _connected = false;
    Refresh();
}

void ConnectionDetailPanel::OnMouseEnter(wxMouseEvent& event)
{
    _mouseInside = true;
    Refresh();
}

void ConnectionDetailPanel::OnMouseLeave(wxMouseEvent& event)
{
    _mouseInside = false;
    Refresh();
}

void ConnectionDetailPanel::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);

    wxSize size = GetSize();

    wxPen pen;

    if (_connected)
    {
        if (_mouseInside || _selected)
        {
            dc.GradientFillLinear(wxRect(wxPoint(0, 0), size), wxColour(0, 180, 0), wxColour(0, 120, 0), wxDOWN);
        }
        else
        {
            dc.GradientFillLinear(wxRect(wxPoint(0, 0), size), wxColour(0, 160, 0), wxColour(0, 100, 0), wxDOWN);
        }
    }
    else
    {
        if (_mouseInside || _selected)
        {
            dc.GradientFillLinear(wxRect(wxPoint(0, 0), size), wxColour(190, 0, 0), wxColour(140, 0, 0), wxDOWN);
        }
        else
        {
            dc.GradientFillLinear(wxRect(wxPoint(0, 0), size), wxColour(170, 0, 0), wxColour(120, 0, 0), wxDOWN);
        }
    }

    dc.SetBrush(*wxTRANSPARENT_BRUSH);

    if (_selected)
    {
        pen.SetColour(wxColour(180, 180, 230));
    }
    else
    {
        pen.SetColour(wxColour(0, 0, 0));
    }
    dc.SetPen(pen);

    dc.DrawRectangle(wxPoint(0, 0), size);

    dc.DrawText(_name, wxPoint(5, 5));
    dc.DrawText(_info, wxPoint(5, 25));

}


