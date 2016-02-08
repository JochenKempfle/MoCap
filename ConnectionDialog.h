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


#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#ifndef WX_PRECOMP
	//(*HeadersPCH(ConnectionDialog)
	#include <wx/sizer.h>
	#include <wx/stattext.h>
	#include <wx/textctrl.h>
	#include <wx/button.h>
	#include <wx/dialog.h>
	//*)
#endif
//(*Headers(ConnectionDialog)
#include <wx/spinctrl.h>
//*)

class ConnectionDialog: public wxDialog
{
	public:

		ConnectionDialog(wxWindow* parent, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~ConnectionDialog();

		void setPort(unsigned short port) { SpinCtrlPort->SetValue(port); }
		unsigned short getPort() { return SpinCtrlPort->GetValue(); }
		void setIP(wxString IP);
		wxString getIP();
		//(*Declarations(ConnectionDialog)
		wxStaticText* StaticText2;
		wxTextCtrl* TextCtrlIP2;
		wxTextCtrl* TextCtrlIP3;
		wxTextCtrl* TextCtrlIP4;
		wxStaticText* StaticText1;
		wxStaticText* StaticText3;
		wxSpinCtrl* SpinCtrlPort;
		wxButton* ButtonCancel;
		wxStaticText* StaticText5;
		wxButton* ButtonOk;
		wxStaticText* StaticText4;
		wxTextCtrl* TextCtrlIP1;
		//*)

	protected:

		//(*Identifiers(ConnectionDialog)
		static const long ID_TEXTCTRLIP1;
		static const long ID_STATICTEXT3;
		static const long ID_TEXTCTRLIP2;
		static const long ID_STATICTEXT4;
		static const long ID_TEXTCTRLIP3;
		static const long ID_STATICTEXT5;
		static const long ID_TEXTCTRLIP4;
		static const long ID_STATICTEXT1;
		static const long ID_SPINCTRLPORT;
		static const long ID_STATICTEXT2;
		static const long ID_BUTTONOK;
		static const long ID_BUTTONCANCEL;
		//*)

	private:

		//(*Handlers(ConnectionDialog)
		void OnButtonOkClick(wxCommandEvent& event);
		void OnButtonCancelClick(wxCommandEvent& event);
		void OnTextCtrlIPText(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
