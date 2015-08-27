/*Copyright (c) 2014, Jochen KempfleAll rights reserved.Redistribution and use in source and binary forms, with or without modification,are permitted provided that the following conditions are met:1. Redistributions of source code must retain the above copyright notice,this list of conditions and the following disclaimer.2. Redistributions in binary form must reproduce the above copyright notice,this list of conditions and the following disclaimer in the documentation and/orother materials provided with the distribution.THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" ANDANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIEDWARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITYOF SUCH DAMAGE.*/#include "wx_pch.h"
#include "ConnectionDialog.h"#include <wx/utils.h>#include <wx/socket.h>

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(ConnectionDialog)
	#include <wx/intl.h>
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(ConnectionDialog)
//*)

//(*IdInit(ConnectionDialog)
const long ConnectionDialog::ID_TEXTCTRLIP1 = wxNewId();
const long ConnectionDialog::ID_STATICTEXT3 = wxNewId();
const long ConnectionDialog::ID_TEXTCTRLIP2 = wxNewId();
const long ConnectionDialog::ID_STATICTEXT4 = wxNewId();
const long ConnectionDialog::ID_TEXTCTRLIP3 = wxNewId();
const long ConnectionDialog::ID_STATICTEXT5 = wxNewId();
const long ConnectionDialog::ID_TEXTCTRLIP4 = wxNewId();
const long ConnectionDialog::ID_STATICTEXT1 = wxNewId();
const long ConnectionDialog::ID_SPINCTRLPORT = wxNewId();
const long ConnectionDialog::ID_STATICTEXT2 = wxNewId();
const long ConnectionDialog::ID_BUTTONOK = wxNewId();
const long ConnectionDialog::ID_BUTTONCANCEL = wxNewId();
//*)

BEGIN_EVENT_TABLE(ConnectionDialog,wxDialog)
	//(*EventTable(ConnectionDialog)
	//*)
END_EVENT_TABLE()

ConnectionDialog::ConnectionDialog(wxWindow* parent, wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(ConnectionDialog)
	wxBoxSizer* BoxSizer4;
	wxStaticBoxSizer* StaticBoxSizer2;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	wxStaticBoxSizer* StaticBoxSizer1;
	wxBoxSizer* BoxSizer3;

	Create(parent, wxID_ANY, _("UDP Connection Properties"), wxDefaultPosition, wxDefaultSize, wxCAPTION, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxVERTICAL, this, _("IP address"));
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	TextCtrlIP1 = new wxTextCtrl(this, ID_TEXTCTRLIP1, _("0"), wxDefaultPosition, wxSize(30,20), 0, wxDefaultValidator, _T("ID_TEXTCTRLIP1"));
	TextCtrlIP1->SetMaxLength(3);
	TextCtrlIP1->SetFocus();
	BoxSizer4->Add(TextCtrlIP1, 0, wxTOP|wxBOTTOM|wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("."), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	BoxSizer4->Add(StaticText3, 0, wxTOP|wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	TextCtrlIP2 = new wxTextCtrl(this, ID_TEXTCTRLIP2, _("0"), wxDefaultPosition, wxSize(30,20), 0, wxDefaultValidator, _T("ID_TEXTCTRLIP2"));
	TextCtrlIP2->SetMaxLength(3);
	BoxSizer4->Add(TextCtrlIP2, 0, wxTOP|wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("."), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	BoxSizer4->Add(StaticText4, 0, wxTOP|wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	TextCtrlIP3 = new wxTextCtrl(this, ID_TEXTCTRLIP3, _("0"), wxDefaultPosition, wxSize(30,20), 0, wxDefaultValidator, _T("ID_TEXTCTRLIP3"));
	TextCtrlIP3->SetMaxLength(3);
	BoxSizer4->Add(TextCtrlIP3, 0, wxTOP|wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("."), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	BoxSizer4->Add(StaticText5, 0, wxTOP|wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	TextCtrlIP4 = new wxTextCtrl(this, ID_TEXTCTRLIP4, _("0"), wxDefaultPosition, wxSize(30,20), 0, wxDefaultValidator, _T("ID_TEXTCTRLIP4"));
	TextCtrlIP4->SetMaxLength(3);
	BoxSizer4->Add(TextCtrlIP4, 0, wxTOP|wxBOTTOM|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer1->Add(BoxSizer4, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Insert the IP address of\nthe network your sensors\nare connected to"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	StaticBoxSizer1->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2->Add(StaticBoxSizer1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer2 = new wxStaticBoxSizer(wxVERTICAL, this, _("Port"));
	SpinCtrlPort = new wxSpinCtrl(this, ID_SPINCTRLPORT, _T("5050"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1024, 65535, 5050, _T("ID_SPINCTRLPORT"));
	SpinCtrlPort->SetValue(_T("5050"));
	StaticBoxSizer2->Add(SpinCtrlPort, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Select the port (service)\nyour sensors are using"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	StaticBoxSizer2->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2->Add(StaticBoxSizer2, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	ButtonOk = new wxButton(this, ID_BUTTONOK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONOK"));
	ButtonOk->SetDefault();
	BoxSizer3->Add(ButtonOk, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonCancel = new wxButton(this, ID_BUTTONCANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONCANCEL"));
	BoxSizer3->Add(ButtonCancel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer3, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();

	Connect(ID_TEXTCTRLIP1,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ConnectionDialog::OnTextCtrlIPText);
	Connect(ID_TEXTCTRLIP2,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ConnectionDialog::OnTextCtrlIPText);
	Connect(ID_TEXTCTRLIP3,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ConnectionDialog::OnTextCtrlIPText);
	Connect(ID_TEXTCTRLIP4,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ConnectionDialog::OnTextCtrlIPText);
	Connect(ID_BUTTONOK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ConnectionDialog::OnButtonOkClick);
	Connect(ID_BUTTONCANCEL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ConnectionDialog::OnButtonCancelClick);
	//*)	TextCtrlIP1->SelectAll();
}

ConnectionDialog::~ConnectionDialog()
{
	//(*Destroy(ConnectionDialog)
	//*)
}


void ConnectionDialog::OnButtonOkClick(wxCommandEvent& event)
{//    wxIPV4address address;//    _address.Hostname(_("192.168.2.3")); // TP-LINK IP    //addr.Hostname(_("192.168.2.4")); // belkin IP//    _address.Service(5050);    EndModal(wxID_OK);
}

void ConnectionDialog::OnButtonCancelClick(wxCommandEvent& event)
{
    EndModal(wxID_CANCEL);
}
void ConnectionDialog::setIP(wxString ip){    size_t pos = ip.find_first_of('.');    TextCtrlIP1->ChangeValue(ip.substr(0, pos));    size_t pos2 = ip.find_first_of('.', pos + 1);    TextCtrlIP2->ChangeValue(ip.substr(pos + 1, pos2 - pos - 1));    pos = pos2;    pos2 = ip.find_first_of('.', pos + 1);    TextCtrlIP3->ChangeValue(ip.substr(pos + 1, pos2 - pos - 1));    pos = pos2;    pos2 = ip.find_first_of('.', pos + 1);    TextCtrlIP4->ChangeValue(ip.substr(pos + 1, pos2 - pos - 1));    TextCtrlIP1->SelectAll();}

wxString ConnectionDialog::getIP(){    wxString ip;    ip << TextCtrlIP1->GetValue() << _(".") << TextCtrlIP2->GetValue() << _(".") << TextCtrlIP3->GetValue() << _(".") << TextCtrlIP4->GetValue();    return ip;}

void ConnectionDialog::OnTextCtrlIPText(wxCommandEvent& event)
{    wxTextCtrl* ctrl = static_cast<wxTextCtrl*>(event.GetEventObject());    wxString value = ctrl->GetValue();    if (value.Length() == 0)    {        ctrl->ChangeValue(_("0"));        ctrl->SetInsertionPoint(1);        return;    }    else if (value.IsNumber())    {        int num = wxAtoi(value);        if (num < 0)        {            ctrl->ChangeValue(value.erase(0, 1));            ctrl->SetInsertionPoint(value.Length());        }        else if (num == 0)        {            ctrl->ChangeValue(_("0"));            ctrl->SetInsertionPoint(1);        }        else if (num > 255)        {            ctrl->ChangeValue(_("255"));            ctrl->SetInsertionPoint(3);        }        else        {            if (value.at(0) == '0')            {                ctrl->ChangeValue(value.erase(0, 1));                ctrl->SetInsertionPoint(value.Length());            }        }    }    else    {        ctrl->Remove(ctrl->GetInsertionPoint() - 1, ctrl->GetInsertionPoint());    }
}
