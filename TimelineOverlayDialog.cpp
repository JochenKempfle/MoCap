#include "wx_pch.h"
#include "TimelineOverlayDialog.h"

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(TimelineOverlayDialog)
	#include <wx/intl.h>
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(TimelineOverlayDialog)
//*)

//(*IdInit(TimelineOverlayDialog)
const long TimelineOverlayDialog::ID_SPINCTRLPRIORITY = wxNewId();
const long TimelineOverlayDialog::ID_TEXTCTRLPRIORITY = wxNewId();
const long TimelineOverlayDialog::ID_LISTBOXTYPE = wxNewId();
const long TimelineOverlayDialog::ID_TEXTCTRLOVERLAYTYPE = wxNewId();
const long TimelineOverlayDialog::ID_BUTTONOK = wxNewId();
const long TimelineOverlayDialog::ID_BUTTONCANCEL = wxNewId();
//*)

BEGIN_EVENT_TABLE(TimelineOverlayDialog,wxDialog)
	//(*EventTable(TimelineOverlayDialog)
	//*)
END_EVENT_TABLE()

TimelineOverlayDialog::TimelineOverlayDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(TimelineOverlayDialog)
	wxStaticBoxSizer* StaticBoxSizer2;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	wxStaticBoxSizer* StaticBoxSizer1;
	wxBoxSizer* BoxSizer3;

	Create(parent, id, _("Overlay Properties"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	StaticBoxSizer2 = new wxStaticBoxSizer(wxVERTICAL, this, _("Priority"));
	SpinCtrlPriority = new wxSpinCtrl(this, ID_SPINCTRLPRIORITY, _T("1"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 20, 1, _T("ID_SPINCTRLPRIORITY"));
	SpinCtrlPriority->SetValue(_T("1"));
	StaticBoxSizer2->Add(SpinCtrlPriority, 0, wxALL, 5);
	TextCtrlPriority = new wxTextCtrl(this, ID_TEXTCTRLPRIORITY, _("Set the priority of the overlay. A smaller value indicates a higher priority."), wxDefaultPosition, wxDefaultSize, wxTE_NO_VSCROLL|wxTE_MULTILINE|wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_TEXTCTRLPRIORITY"));
	StaticBoxSizer2->Add(TextCtrlPriority, 0, wxALL|wxEXPAND, 5);
	StaticBoxSizer2->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2->Add(StaticBoxSizer2, 1, wxALL|wxEXPAND, 5);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Overlay Type"));
	ListBoxType = new wxListBox(this, ID_LISTBOXTYPE, wxDefaultPosition, wxDefaultSize, 0, 0, wxLB_SINGLE, wxDefaultValidator, _T("ID_LISTBOXTYPE"));
	ListBoxType->SetSelection( ListBoxType->Append(_("Additive")) );
	ListBoxType->Append(_("Subtractive"));
	ListBoxType->Append(_("Ignore"));
	ListBoxType->Append(_("Overwrite"));
	ListBoxType->Append(_("Linear Interpolation"));
	ListBoxType->Append(_("Static Interpolation"));
	ListBoxType->Append(_("Weighted Interpolation"));
	StaticBoxSizer1->Add(ListBoxType, 1, wxALL|wxEXPAND, 5);
	TextCtrlOverlayType = new wxTextCtrl(this, ID_TEXTCTRLOVERLAYTYPE, _("Add the rotation of the lower track\'s frame to the respective frame of the track above."), wxDefaultPosition, wxDefaultSize, wxTE_NO_VSCROLL|wxTE_MULTILINE|wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_TEXTCTRLOVERLAYTYPE"));
	StaticBoxSizer1->Add(TextCtrlOverlayType, 1, wxALL|wxEXPAND, 5);
	BoxSizer2->Add(StaticBoxSizer1, 2, wxALL|wxEXPAND, 5);
	BoxSizer1->Add(BoxSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	ButtonOk = new wxButton(this, ID_BUTTONOK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONOK"));
	ButtonOk->SetDefault();
	BoxSizer3->Add(ButtonOk, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonCancel = new wxButton(this, ID_BUTTONCANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONCANCEL"));
	BoxSizer3->Add(ButtonCancel, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();

	Connect(ID_LISTBOXTYPE,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&TimelineOverlayDialog::OnListBoxTypeSelect);
	Connect(ID_BUTTONOK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&TimelineOverlayDialog::OnButtonOkClick);
	Connect(ID_BUTTONCANCEL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&TimelineOverlayDialog::OnButtonCancelClick);
	//*)
}

TimelineOverlayDialog::~TimelineOverlayDialog()
{
	//(*Destroy(TimelineOverlayDialog)
	//*)
}

void TimelineOverlayDialog::setOverlay(const TimelineOverlay* overlay)
{
    SpinCtrlPriority->SetValue(overlay->getPriority());
    ListBoxType->SetSelection(int(overlay->getType()));
}

int TimelineOverlayDialog::getPriority() const
{
    return SpinCtrlPriority->GetValue();
}

OverlayType TimelineOverlayDialog::getType() const
{
    return OverlayType(ListBoxType->GetSelection());
}

void TimelineOverlayDialog::OnButtonOkClick(wxCommandEvent& event)
{
    EndModal(wxID_OK);
}

void TimelineOverlayDialog::OnButtonCancelClick(wxCommandEvent& event)
{
    EndModal(wxID_CANCEL);
}

void TimelineOverlayDialog::OnListBoxTypeSelect(wxCommandEvent& event)
{
    int selection = event.GetSelection();

    if (selection == 0)
    {
        TextCtrlOverlayType->SetValue(_("Add the rotation of the lower track's frame to the respective frame of the track above."));
    }
    else if (selection == 1)
    {
        TextCtrlOverlayType->SetValue(_("Subtract the rotation of the lower track's frame from the respective frame of the track above."));
    }
    else if (selection == 2)
    {
        TextCtrlOverlayType->SetValue(_("Use the upper track's frames, discard the lower track's frames."));
    }
    else if (selection == 3)
    {
        TextCtrlOverlayType->SetValue(_("Use the lower track's frames, discard the upper track's frames."));
    }
    else if (selection == 4)
    {
        TextCtrlOverlayType->SetValue(_("Linearly interpolate from upper track to lower track."));
    }
    else if (selection == 5)
    {
        TextCtrlOverlayType->SetValue(_("Achieve a static interpolation of 50% between both tracks."));
    }
    else if (selection == 6)
    {
        TextCtrlOverlayType->SetValue(_("Interpolate between both tracks with the upper track's weights as interpolation parameter."));
    }
}






