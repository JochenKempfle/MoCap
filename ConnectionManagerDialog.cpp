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
#include "ConnectionManagerDialog.h"
#include "ConnectionDetailPanel.h"
#include "MoCapManager.h"

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(ConnectionManagerDialog)
	#include <wx/intl.h>
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(ConnectionManagerDialog)
//*)

//(*IdInit(ConnectionManagerDialog)
const long ConnectionManagerDialog::ID_CONTAINERPANEL = wxNewId();
const long ConnectionManagerDialog::ID_LISTBOXCONNECTIONS = wxNewId();
const long ConnectionManagerDialog::ID_BUTTON1 = wxNewId();
const long ConnectionManagerDialog::ID_BUTTONREMOVE = wxNewId();
//*)

BEGIN_EVENT_TABLE(ConnectionManagerDialog,wxDialog)
	//(*EventTable(ConnectionManagerDialog)
	//*)
END_EVENT_TABLE()

ConnectionManagerDialog::ConnectionManagerDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(ConnectionManagerDialog)
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer3;

	Create(parent, id, _("Connection Manager"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxSize(500,400));
	Move(wxDefaultPosition);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizerContainer = new wxBoxSizer(wxHORIZONTAL);
	ContainerPanel = new ScrolledContainerPanel(this,ID_CONTAINERPANEL,wxDefaultPosition,wxDefaultSize);
	BoxSizerContainer->Add(ContainerPanel, 1, wxALL|wxEXPAND, 5);
	BoxSizer1->Add(BoxSizerContainer, 2, wxALL|wxEXPAND, 5);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	ListBoxConnections = new wxListBox(this, ID_LISTBOXCONNECTIONS, wxDefaultPosition, wxDefaultSize, 0, 0, wxLB_SINGLE|wxLB_NEEDED_SB, wxDefaultValidator, _T("ID_LISTBOXCONNECTIONS"));
	ListBoxConnections->SetSelection( ListBoxConnections->Append(_("UDP")) );
	ListBoxConnections->Append(_("Kinect"));
	ListBoxConnections->Append(_("File"));
	BoxSizer3->Add(ListBoxConnections, 2, wxALL|wxEXPAND, 5);
	BoxSizer2 = new wxBoxSizer(wxVERTICAL);
	ButtonCreate = new wxButton(this, ID_BUTTON1, _("Create"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
	BoxSizer2->Add(ButtonCreate, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonRemove = new wxButton(this, ID_BUTTONREMOVE, _("Remove"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONREMOVE"));
	BoxSizer2->Add(ButtonRemove, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer3->Add(BoxSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer3, 1, wxALL|wxEXPAND, 5);
	SetSizer(BoxSizer1);
	SetSizer(BoxSizer1);
	Layout();
	Center();

	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ConnectionManagerDialog::OnButtonCreateClick);
	Connect(ID_BUTTONREMOVE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ConnectionManagerDialog::OnButtonRemoveClick);
	//*)
	_selectedPanel = -1;
	ListBoxConnections->Clear();
	for (size_t i = 0; i < theMoCapManager.getNumRegisteredReceivers(); ++i)
    {
        ListBoxConnections->Append(theMoCapManager.getReceiverName(i));
    }
    // add panels of existing receivers
    for (size_t i = 0; i < theMoCapManager.getNumReceivers(); ++i)
    {
        ConnectionDetailPanel* panel = new ConnectionDetailPanel(ContainerPanel->ScrolledWindow);
        panel->setReceiver(theMoCapManager.getReceiver(i));
        ContainerPanel->add(panel);
    }
}

ConnectionManagerDialog::~ConnectionManagerDialog()
{
	//(*Destroy(ConnectionManagerDialog)
	//*)
}


void ConnectionManagerDialog::OnButtonCreateClick(wxCommandEvent& event)
{
    int selection = ListBoxConnections->GetSelection();

    if (selection < 0 || selection > static_cast<int>(theMoCapManager.getNumRegisteredReceivers()))
    {
        return;
    }

    ReceiverBase* recv = theMoCapManager.createReceiver(selection);
    if (recv == nullptr || !recv->setup())
    {
        theMoCapManager.removeReceiver(recv);
        wxMessageBox(_("Unable to create receiver"), _("Error"), wxICON_EXCLAMATION);
        return;
    }
    ConnectionDetailPanel* panel = new ConnectionDetailPanel(ContainerPanel->ScrolledWindow);
    panel->setReceiver(recv);
    ContainerPanel->add(panel);
    BoxSizerContainer->Layout();
	// panel->Connect(panel->getRemoveButtonID(), wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&OnRemoveConnectionDetailPanel);
	panel->Connect(wxEVT_LEFT_DOWN,(wxObjectEventFunction)&ConnectionManagerDialog::OnConnectionDetailPanelLeftDown, NULL, this);

	_detailPanels.push_back(panel);

    Refresh();
}

void ConnectionManagerDialog::OnConnectionDetailPanelLeftDown(wxMouseEvent& event)
{
    // wxMessageBox(_("haha"));
    ConnectionDetailPanel* panel = static_cast<ConnectionDetailPanel*>(event.GetEventObject());

    // unselect previous selection
    if (_selectedPanel >= 0 && _selectedPanel < static_cast<int>(_detailPanels.size()))
    {
        _detailPanels[_selectedPanel]->unselect();
    }

    _selectedPanel = -1;
    // find panel position
    for (size_t i = 0; i < _detailPanels.size(); ++i)
    {
        if (_detailPanels[i] == panel)
        {
            _selectedPanel = i;
            break;
        }
    }
    //  select current panel
    if (_selectedPanel >= 0)
    {
        panel->select();
    }

    event.Skip();
    Refresh();
}

void ConnectionManagerDialog::OnButtonRemoveClick(wxCommandEvent& event)
{
    if (_selectedPanel < 0 || _detailPanels.size() == 0)
    {
        return;
    }
    _detailPanels[_selectedPanel]->Destroy();
    _detailPanels.erase(_detailPanels.begin() + _selectedPanel);
    theMoCapManager.removeReceiver(_selectedPanel);

    ContainerPanel->layout();
    BoxSizerContainer->Layout();

    _selectedPanel = -1;

    Refresh();
}
