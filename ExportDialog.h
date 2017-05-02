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


#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#ifndef WX_PRECOMP
	//(*HeadersPCH(ExportDialog)
	#include <wx/sizer.h>
	#include <wx/stattext.h>
	#include <wx/button.h>
	#include <wx/dialog.h>
	//*)
#endif
//(*Headers(ExportDialog)
#include <wx/filepicker.h>
#include <wx/spinctrl.h>
#include <wx/statline.h>
//*)

class ExportDialog: public wxDialog
{
	public:

		ExportDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~ExportDialog();

		void setValues(uint64_t startMarkerTime, uint64_t endMarkerTime, uint64_t cursorTime, uint64_t endTime);
		float getFrameTime() const;
		uint64_t getStartTime() const;
		uint64_t getEndTime() const;

		wxString getPath() const { return FilePickerCtrlExport->GetPath(); }

		//(*Declarations(ExportDialog)
		wxSpinCtrl* SpinCtrlStartTimeMS;
		wxButton* ButtonEndTimeMarker;
		wxButton* ButtonEndTimeDefault;
		wxButton* ButtonStartTimeCursor;
		wxStaticText* StaticText2;
		wxSpinCtrl* SpinCtrlStartTimeSec;
		wxStaticText* StaticText1;
		wxStaticText* StaticText3;
		wxButton* ButtonStartTimeDefault;
		wxButton* ButtonCancel;
		wxFilePickerCtrl* FilePickerCtrlExport;
		wxSpinCtrl* SpinCtrlEndTimeMS;
		wxSpinCtrl* SpinCtrlEndTimeSec;
		wxButton* ButtonEndTimeCursor;
		wxStaticLine* StaticLine1;
		wxButton* ButtonOk;
		wxButton* ButtonStartTimeMarker;
		wxSpinCtrl* SpinCtrlFrameTime;
		//*)

	protected:

		//(*Identifiers(ExportDialog)
		static const long ID_SPINCTRLSTARTTIMESEC;
		static const long ID_STATICTEXT1;
		static const long ID_SPINCTRLSTARTTIMEMS;
		static const long ID_BUTTONSTARTTIMECURSOR;
		static const long ID_BUTTONSTARTTIMEMARKER;
		static const long ID_BUTTONSTARTTIMEDEFAULT;
		static const long ID_SPINCTRLENDTIMESEC;
		static const long ID_STATICTEXT2;
		static const long ID_SPINCTRLENDTIMEMS;
		static const long ID_BUTTONENDTIMECURSOR;
		static const long ID_BUTTONENDTIMEMARKER;
		static const long ID_BUTTONENDTIMEDEFAULT;
		static const long ID_STATICLINE1;
		static const long ID_STATICTEXT3;
		static const long ID_SPINCTRLFRAMETIME;
		static const long ID_FILEPICKERCTRLEXPORT;
		static const long ID_BUTTONOK;
		static const long ID_BUTTONCANCEL;
		//*)

	private:

		//(*Handlers(ExportDialog)
		void OnButtonOkClick(wxCommandEvent& event);
		void OnButtonCancelClick(wxCommandEvent& event);
		void OnButtonStartTimeCursorClick(wxCommandEvent& event);
		void OnButtonStartTimeMarkerClick(wxCommandEvent& event);
		void OnButtonStartTimeDefaultClick(wxCommandEvent& event);
		void OnButtonEndTimeCursorClick(wxCommandEvent& event);
		void OnButtonEndTimeMarkerClick(wxCommandEvent& event);
		void OnButtonEndTimeDefaultClick(wxCommandEvent& event);
		//*)

		uint64_t _startMarkerTime;
		uint64_t _endMarkerTime;
		uint64_t _cursorTime;
		uint64_t _endTime;

		DECLARE_EVENT_TABLE()
};

#endif
