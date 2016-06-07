#include "wx_pch.h"
#include "FilterDialog.h"

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(FilterDialog)
	#include <wx/intl.h>
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(FilterDialog)
//*)

//(*IdInit(FilterDialog)
const long FilterDialog::ID_LISTBOXFILTER = wxNewId();
const long FilterDialog::ID_SPINCTRLFRAMETIME = wxNewId();
const long FilterDialog::ID_STATICTEXT1 = wxNewId();
const long FilterDialog::ID_BUTTONOK = wxNewId();
const long FilterDialog::ID_BUTTONCANCEL = wxNewId();
//*)

BEGIN_EVENT_TABLE(FilterDialog,wxDialog)
	//(*EventTable(FilterDialog)
	//*)
END_EVENT_TABLE()

FilterDialog::FilterDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(FilterDialog)
	wxStaticBoxSizer* StaticBoxSizer2;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	wxStaticBoxSizer* StaticBoxSizer1;
	wxBoxSizer* BoxSizer3;

	Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Filter type"));
	ListBoxFilter = new wxListBox(this, ID_LISTBOXFILTER, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOXFILTER"));
	ListBoxFilter->SetMinSize(wxSize(-1,100));
	StaticBoxSizer1->Add(ListBoxFilter, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2->Add(StaticBoxSizer1, 1, wxALL|wxEXPAND, 5);
	StaticBoxSizer2 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Frame Time"));
	SpinCtrlFrameTime = new wxSpinCtrl(this, ID_SPINCTRLFRAMETIME, _T("10"), wxDefaultPosition, wxDefaultSize, 0, 1, 1000, 10, _T("ID_SPINCTRLFRAMETIME"));
	SpinCtrlFrameTime->SetValue(_T("10"));
	StaticBoxSizer2->Add(SpinCtrlFrameTime, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("ms"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	StaticBoxSizer2->Add(StaticText1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2->Add(StaticBoxSizer2, 1, wxALL|wxEXPAND, 5);
	BoxSizer1->Add(BoxSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	ButtonOK = new wxButton(this, ID_BUTTONOK, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONOK"));
	BoxSizer3->Add(ButtonOK, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonCancel = new wxButton(this, ID_BUTTONCANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONCANCEL"));
	BoxSizer3->Add(ButtonCancel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);

	Connect(ID_BUTTONOK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FilterDialog::OnButtonOKClick);
	Connect(ID_BUTTONCANCEL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FilterDialog::OnButtonCancelClick);
	//*)
}

FilterDialog::~FilterDialog()
{
	//(*Destroy(FilterDialog)
	//*)
}


void FilterDialog::OnButtonOKClick(wxCommandEvent& event)
{
    EndModal(wxID_OK);
}

void FilterDialog::OnButtonCancelClick(wxCommandEvent& event)
{
    EndModal(wxID_CANCEL);
}

void FilterDialog::setFilter(std::vector<MotionFilterBase*> filters, unsigned int selectedFilter)
{
    ListBoxFilter->Clear();
    if (filters.size() == 0)
    {
        return;
    }
    for (size_t i = 0; i < filters.size(); ++i)
    {
        ListBoxFilter->AppendAndEnsureVisible(filters[i]->getName());
    }
    if (selectedFilter >= filters.size())
    {
        selectedFilter = filters.size() - 1;
    }
    ListBoxFilter->SetSelection(selectedFilter);
}

void FilterDialog::setFrameTime(unsigned int frameTime)
{
    if (frameTime == 0)
    {
        frameTime = 1;
    }
    else if (frameTime > 1000)
    {
        frameTime = 1000;
    }
    SpinCtrlFrameTime->SetValue(frameTime);
}

int FilterDialog::getSelectedFilter() const
{
    return ListBoxFilter->GetSelection();
}

unsigned int FilterDialog::getFrameTime() const
{
    return SpinCtrlFrameTime->GetValue();
}
