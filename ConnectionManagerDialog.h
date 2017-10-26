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


#ifndef CONNECTIONMANAGERDIALOG_H
#define CONNECTIONMANAGERDIALOG_H

#ifndef WX_PRECOMP
	//(*HeadersPCH(ConnectionManagerDialog)
	#include <wx/sizer.h>
	#include <wx/listbox.h>
	#include <wx/button.h>
	#include <wx/dialog.h>
	//*)
#endif
//(*Headers(ConnectionManagerDialog)
#include "ScrolledContainerPanel.h"
//*)

#include "ConnectionDetailPanel.h"
#include <vector>

class ConnectionManagerDialog: public wxDialog
{
	public:

		ConnectionManagerDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~ConnectionManagerDialog();

		//(*Declarations(ConnectionManagerDialog)
		wxListBox* ListBoxConnections;
		wxButton* ButtonRemove;
		wxButton* ButtonCreate;
		ScrolledContainerPanel* ContainerPanel;
		wxBoxSizer* BoxSizerContainer;
		//*)

	protected:

		//(*Identifiers(ConnectionManagerDialog)
		static const long ID_CONTAINERPANEL;
		static const long ID_LISTBOXCONNECTIONS;
		static const long ID_BUTTON1;
		static const long ID_BUTTONREMOVE;
		//*)

	private:

		//(*Handlers(ConnectionManagerDialog)
		void OnButtonCreateClick(wxCommandEvent& event);
		void OnButtonRemoveClick(wxCommandEvent& event);
		//*)

		void OnConnectionDetailPanelLeftDown(wxMouseEvent& event);

		std::vector<ConnectionDetailPanel*> _detailPanels;
		int _selectedPanel;

		DECLARE_EVENT_TABLE()
};

#endif
