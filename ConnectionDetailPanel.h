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


#ifndef CONNECTIONDETAILPANEL_H
#define CONNECTIONDETAILPANEL_H

#ifndef WX_PRECOMP
	//(*HeadersPCH(ConnectionDetailPanel)
	#include <wx/sizer.h>
	#include <wx/panel.h>
	#include <wx/button.h>
	//*)
#endif
//(*Headers(ConnectionDetailPanel)
//*)

#include "ReceiverBase.h"


class ConnectionDetailPanel: public wxPanel
{
	public:

		ConnectionDetailPanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~ConnectionDetailPanel();

		bool connect();
		void disconnect();

		void select(bool selection = true) { _selected = selection; }
		void unselect() { select(false); }

		void setReceiver(ReceiverBase* recv);

		//(*Declarations(ConnectionDetailPanel)
		wxButton* ButtonConnection;
		//*)

	protected:

		//(*Identifiers(ConnectionDetailPanel)
		static const long ID_BUTTONCONNECTION;
		//*)

	private:

		//(*Handlers(ConnectionDetailPanel)
		void OnButtonConnectionClick(wxCommandEvent& event);
		void OnMouseEnter(wxMouseEvent& event);
		void OnMouseLeave(wxMouseEvent& event);
		void OnPaint(wxPaintEvent& event);
		//*)

		bool _connected;
		bool _mouseInside;
		bool _selected;

		wxString _name;
		wxString _info;

		ReceiverBase* _receiver;

		DECLARE_EVENT_TABLE()
};

#endif
