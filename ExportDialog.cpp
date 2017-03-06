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
#include "ExportDialog.h"

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(ExportDialog)
	#include <wx/intl.h>
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(ExportDialog)
//*)

//(*IdInit(ExportDialog)
const long ExportDialog::ID_SPINCTRL1 = wxNewId();
const long ExportDialog::ID_STATICTEXT1 = wxNewId();
const long ExportDialog::ID_SPINCTRL3 = wxNewId();
const long ExportDialog::ID_BUTTONSTARTTIMECURSOR = wxNewId();
const long ExportDialog::ID_BUTTONSTARTTIMEMARKER = wxNewId();
const long ExportDialog::ID_BUTTONSTARTTIMEDEFAULT = wxNewId();
const long ExportDialog::ID_SPINCTRL2 = wxNewId();
const long ExportDialog::ID_STATICTEXT2 = wxNewId();
const long ExportDialog::ID_SPINCTRL4 = wxNewId();
const long ExportDialog::ID_BUTTONENDTIMECURSOR = wxNewId();
const long ExportDialog::ID_BUTTONENDTIMEMARKER = wxNewId();
const long ExportDialog::ID_BUTTONENDTIMEDEFAULT = wxNewId();
const long ExportDialog::ID_STATICLINE1 = wxNewId();
const long ExportDialog::ID_STATICTEXT3 = wxNewId();
const long ExportDialog::ID_SPINCTRLFRAMETIME = wxNewId();
const long ExportDialog::ID_CHECKBOXINTERPOLATE = wxNewId();
const long ExportDialog::ID_FILEPICKERCTRLEXPORT = wxNewId();
const long ExportDialog::ID_BUTTONOK = wxNewId();
const long ExportDialog::ID_BUTTONCANCEL = wxNewId();
//*)

BEGIN_EVENT_TABLE(ExportDialog,wxDialog)
	//(*EventTable(ExportDialog)
	//*)
END_EVENT_TABLE()

ExportDialog::ExportDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(ExportDialog)
	wxBoxSizer* BoxSizer4;
	wxStaticBoxSizer* StaticBoxSizer2;
	wxBoxSizer* BoxSizer6;
	wxBoxSizer* BoxSizer5;
	wxBoxSizer* BoxSizer7;
	wxBoxSizer* BoxSizer8;
	wxBoxSizer* BoxSizer2;
	wxStaticBoxSizer* StaticBoxSizer3;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer9;
	wxStaticBoxSizer* StaticBoxSizer1;
	wxBoxSizer* BoxSizer3;

	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxVERTICAL, this, _("Start Time (sec : ms)"));
	BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	SpinCtrl1 = new wxSpinCtrl(this, ID_SPINCTRL1, _T("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100000, 0, _T("ID_SPINCTRL1"));
	SpinCtrl1->SetValue(_T("0"));
	BoxSizer6->Add(SpinCtrl1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _(":"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	BoxSizer6->Add(StaticText1, 0, wxTOP|wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SpinCtrl3 = new wxSpinCtrl(this, ID_SPINCTRL3, _T("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 999, 0, _T("ID_SPINCTRL3"));
	SpinCtrl3->SetValue(_T("0"));
	BoxSizer6->Add(SpinCtrl3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer1->Add(BoxSizer6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	ButtonStartTimeCursor = new wxButton(this, ID_BUTTONSTARTTIMECURSOR, _("Cursor"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONSTARTTIMECURSOR"));
	BoxSizer5->Add(ButtonStartTimeCursor, 0, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonStartTimeMarker = new wxButton(this, ID_BUTTONSTARTTIMEMARKER, _("Start Marker"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONSTARTTIMEMARKER"));
	BoxSizer5->Add(ButtonStartTimeMarker, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonStartTimeDefault = new wxButton(this, ID_BUTTONSTARTTIMEDEFAULT, _(" Default"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONSTARTTIMEDEFAULT"));
	BoxSizer5->Add(ButtonStartTimeDefault, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer1->Add(BoxSizer5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer8->Add(StaticBoxSizer1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer2 = new wxStaticBoxSizer(wxVERTICAL, this, _("End Time (sec : ms)"));
	BoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
	SpinCtrl2 = new wxSpinCtrl(this, ID_SPINCTRL2, _T("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100000, 0, _T("ID_SPINCTRL2"));
	SpinCtrl2->SetValue(_T("0"));
	BoxSizer9->Add(SpinCtrl2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _(":"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	BoxSizer9->Add(StaticText2, 0, wxTOP|wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SpinCtrl4 = new wxSpinCtrl(this, ID_SPINCTRL4, _T("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 999, 0, _T("ID_SPINCTRL4"));
	SpinCtrl4->SetValue(_T("0"));
	BoxSizer9->Add(SpinCtrl4, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer2->Add(BoxSizer9, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
	ButtonEndTimeCursor = new wxButton(this, ID_BUTTONENDTIMECURSOR, _("Cursor"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONENDTIMECURSOR"));
	BoxSizer7->Add(ButtonEndTimeCursor, 0, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonEndTimeMarker = new wxButton(this, ID_BUTTONENDTIMEMARKER, _("End Marker"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONENDTIMEMARKER"));
	BoxSizer7->Add(ButtonEndTimeMarker, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonEndTimeDefault = new wxButton(this, ID_BUTTONENDTIMEDEFAULT, _("Default"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONENDTIMEDEFAULT"));
	BoxSizer7->Add(ButtonEndTimeDefault, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer2->Add(BoxSizer7, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer8->Add(StaticBoxSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer8, 0, wxALL|wxEXPAND, 5);
	StaticLine1 = new wxStaticLine(this, ID_STATICLINE1, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE1"));
	BoxSizer1->Add(StaticLine1, 1, wxALL|wxEXPAND, 5);
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	StaticBoxSizer3 = new wxStaticBoxSizer(wxVERTICAL, this, _("Export Options"));
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Frame Time (ms)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	BoxSizer2->Add(StaticText3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SpinCtrlFrameTime = new wxSpinCtrl(this, ID_SPINCTRLFRAMETIME, _T("10"), wxDefaultPosition, wxSize(100,-1), 0, 1, 1000000, 10, _T("ID_SPINCTRLFRAMETIME"));
	SpinCtrlFrameTime->SetValue(_T("10"));
	BoxSizer2->Add(SpinCtrlFrameTime, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer3->Add(BoxSizer2, 0, wxALL|wxALIGN_LEFT, 5);
	CheckBoxInterpolate = new wxCheckBox(this, ID_CHECKBOXINTERPOLATE, _("Interpolate Frames"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXINTERPOLATE"));
	CheckBoxInterpolate->SetValue(false);
	StaticBoxSizer3->Add(CheckBoxInterpolate, 0, wxALL|wxALIGN_LEFT, 5);
	BoxSizer4->Add(StaticBoxSizer3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FilePickerCtrlExport = new wxFilePickerCtrl(this, ID_FILEPICKERCTRLEXPORT, wxEmptyString, _("Select a file"), _T("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_OVERWRITE_PROMPT|wxFLP_SAVE|wxFLP_USE_TEXTCTRL, wxDefaultValidator, _T("ID_FILEPICKERCTRLEXPORT"));
	BoxSizer4->Add(FilePickerCtrlExport, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer4, 0, wxALL|wxEXPAND, 5);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	ButtonOk = new wxButton(this, ID_BUTTONOK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONOK"));
	ButtonOk->SetDefault();
	BoxSizer3->Add(ButtonOk, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonCancel = new wxButton(this, ID_BUTTONCANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONCANCEL"));
	BoxSizer3->Add(ButtonCancel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);

	Connect(ID_BUTTONOK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ExportDialog::OnButtonOkClick);
	Connect(ID_BUTTONCANCEL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ExportDialog::OnButtonCancelClick);
	//*)
}

ExportDialog::~ExportDialog()
{
	//(*Destroy(ExportDialog)
	//*)
}


void ExportDialog::OnButtonOkClick(wxCommandEvent& event)
{
    if (FilePickerCtrlExport->GetPath() == _(""));
    {
        wxMessageBox(_("No file selected.\nPlease first select a file to export to!"), _("Error!"), wxICON_EXCLAMATION);
        return;
    }
    if (IsModal())
    {
        EndModal(wxID_OK);
    }
    Show(false);
}

void ExportDialog::OnButtonCancelClick(wxCommandEvent& event)
{
    if (IsModal())
    {
        EndModal(wxID_CANCEL);
    }
    Show(false);
}

