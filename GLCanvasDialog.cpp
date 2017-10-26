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
#include "GLCanvasDialog.h"
#include "GLCanvas.h"

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(GLCanvasDialog)
	#include <wx/intl.h>
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(GLCanvasDialog)
//*)

//(*IdInit(GLCanvasDialog)
const long GLCanvasDialog::ID_COLOURPICKERCTRL = wxNewId();
const long GLCanvasDialog::ID_SLIDERLINEWIDTH = wxNewId();
const long GLCanvasDialog::ID_CHECKLISTBOXTRACEBONES = wxNewId();
const long GLCanvasDialog::ID_SLIDERTRACELENGTH = wxNewId();
const long GLCanvasDialog::ID_BUTTONOK = wxNewId();
const long GLCanvasDialog::ID_BUTTONABORT = wxNewId();
//*)

BEGIN_EVENT_TABLE(GLCanvasDialog,wxDialog)
	//(*EventTable(GLCanvasDialog)
	//*)
END_EVENT_TABLE()

GLCanvasDialog::GLCanvasDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(GLCanvasDialog)
	wxStaticBoxSizer* StaticBoxSizer2;
	wxBoxSizer* BoxSizer2;
	wxStaticBoxSizer* StaticBoxSizer3;
	wxBoxSizer* BoxSizer1;
	wxStaticBoxSizer* StaticBoxSizer1;

	Create(parent, id, _("Canvas Settings"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxSize(400,400));
	Move(wxDefaultPosition);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Background Color"));
	ColourPickerCtrl = new wxColourPickerCtrl(this, ID_COLOURPICKERCTRL, wxColour(0,0,0), wxDefaultPosition, wxDefaultSize, wxCLRP_USE_TEXTCTRL, wxDefaultValidator, _T("ID_COLOURPICKERCTRL"));
	ColourPickerCtrl->SetToolTip(_("Click or enter text to change background color"));
	StaticBoxSizer1->Add(ColourPickerCtrl, 1, wxALL, 5);
	BoxSizer1->Add(StaticBoxSizer1, 0, wxALL|wxEXPAND, 5);
	StaticBoxSizer2 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Line Width"));
	SliderLineWidth = new wxSlider(this, ID_SLIDERLINEWIDTH, 1, 1, 5, wxDefaultPosition, wxDefaultSize, wxSL_LABELS|wxSL_TOP, wxDefaultValidator, _T("ID_SLIDERLINEWIDTH"));
	SliderLineWidth->SetTickFreq(1);
	StaticBoxSizer2->Add(SliderLineWidth, 1, wxALL, 5);
	BoxSizer1->Add(StaticBoxSizer2, 0, wxALL|wxEXPAND, 5);
	StaticBoxSizer3 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Draw Trace / Trace Length"));
	CheckListBoxTraceBones = new wxCheckListBox(this, ID_CHECKLISTBOXTRACEBONES, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHECKLISTBOXTRACEBONES"));
	StaticBoxSizer3->Add(CheckListBoxTraceBones, 1, wxALL|wxEXPAND, 5);
	SliderTraceLength = new wxSlider(this, ID_SLIDERTRACELENGTH, 0, 0, 1000, wxDefaultPosition, wxDefaultSize, wxSL_LABELS|wxSL_TOP, wxDefaultValidator, _T("ID_SLIDERTRACELENGTH"));
	StaticBoxSizer3->Add(SliderTraceLength, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(StaticBoxSizer3, 1, wxALL|wxEXPAND, 5);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	ButtonOK = new wxButton(this, ID_BUTTONOK, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONOK"));
	ButtonOK->SetDefault();
	BoxSizer2->Add(ButtonOK, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonAbort = new wxButton(this, ID_BUTTONABORT, _("Abort"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONABORT"));
	BoxSizer2->Add(ButtonAbort, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxFIXED_MINSIZE, 5);
	SetSizer(BoxSizer1);
	SetSizer(BoxSizer1);
	Layout();
	Center();

	Connect(ID_BUTTONOK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&GLCanvasDialog::OnButtonOKClick);
	Connect(ID_BUTTONABORT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&GLCanvasDialog::OnButtonAbortClick);
	//*)
}

GLCanvasDialog::~GLCanvasDialog()
{
	//(*Destroy(GLCanvasDialog)
	//*)
}

void GLCanvasDialog::setTraceLength(int length)
{
    if (length < 0)
    {
        length = 0;
    }
    else if (length > SliderTraceLength->GetMax())
    {
        length = SliderTraceLength->GetMax();
    }
    SliderTraceLength->SetValue(length);
}

int GLCanvasDialog::getTraceLength() const
{
    return SliderTraceLength->GetValue();
}

void GLCanvasDialog::setSkeleton(const Skeleton* skeleton)
{
    _boneIdsAndNames.clear();
    CheckListBoxTraceBones->Clear();
	if (skeleton == nullptr)
    {
        return;
    }
    _boneIdsAndNames = skeleton->getBoneIdsWithName();
    for (size_t i = 0; i < _boneIdsAndNames.size(); ++i)
    {
        CheckListBoxTraceBones->Append(_boneIdsAndNames[i].second);
    }
}

void GLCanvasDialog::setBonesWithTrace(std::vector<int> boneIds)
{
    CheckListBoxTraceBones->DeselectAll();
    size_t j = 0;
    for (size_t i = 0; i < _boneIdsAndNames.size(); ++i)
    {
        if (j >= boneIds.size())
        {
            break;
        }
        else if (_boneIdsAndNames[i].first != boneIds[j])
        {
            continue;
        }
        CheckListBoxTraceBones->Check(i);
        ++j;
    }
}

std::vector<int> GLCanvasDialog::getBonesWithTrace() const
{
    std::vector<int> boneIds;

    for (size_t i = 0; i < CheckListBoxTraceBones->GetCount(); ++i)
    {
        if (CheckListBoxTraceBones->IsChecked(i))
        {
            boneIds.push_back(_boneIdsAndNames[i].first);
        }
    }
    return boneIds;
}


void GLCanvasDialog::OnButtonOKClick(wxCommandEvent& event)
{
    EndModal(wxID_OK);
}

void GLCanvasDialog::OnButtonAbortClick(wxCommandEvent& event)
{
    EndModal(wxID_ABORT);
}
