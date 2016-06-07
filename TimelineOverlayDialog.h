#ifndef TIMELINEOVERLAYDIALOG_H
#define TIMELINEOVERLAYDIALOG_H

#ifndef WX_PRECOMP
	//(*HeadersPCH(TimelineOverlayDialog)
	#include <wx/sizer.h>
	#include <wx/textctrl.h>
	#include <wx/listbox.h>
	#include <wx/button.h>
	#include <wx/dialog.h>
	//*)
#endif
//(*Headers(TimelineOverlayDialog)
#include <wx/spinctrl.h>
//*)

#include "TimelineOverlay.h"

class TimelineOverlayDialog: public wxDialog
{
	public:

		TimelineOverlayDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~TimelineOverlayDialog();

		// TODO(JK#5#): draw info about tracks in TimelineOverlayDialog
		void setOverlay(const TimelineOverlay* overlay);
		int getPriority() const;
		OverlayType getType() const;

		//(*Declarations(TimelineOverlayDialog)
		wxTextCtrl* TextCtrlPriority;
		wxTextCtrl* TextCtrlOverlayType;
		wxListBox* ListBoxType;
		wxButton* ButtonCancel;
		wxButton* ButtonOk;
		wxSpinCtrl* SpinCtrlPriority;
		//*)

	protected:

		//(*Identifiers(TimelineOverlayDialog)
		static const long ID_SPINCTRLPRIORITY;
		static const long ID_TEXTCTRLPRIORITY;
		static const long ID_LISTBOXTYPE;
		static const long ID_TEXTCTRLOVERLAYTYPE;
		static const long ID_BUTTONOK;
		static const long ID_BUTTONCANCEL;
		//*)

	private:

		//(*Handlers(TimelineOverlayDialog)
		void OnButtonOkClick(wxCommandEvent& event);
		void OnListBoxTypeSelect(wxCommandEvent& event);
		void OnButtonCancelClick(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
