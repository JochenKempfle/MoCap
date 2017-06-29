#ifndef GLCANVASDIALOG_H
#define GLCANVASDIALOG_H

#ifndef WX_PRECOMP
	//(*HeadersPCH(GLCanvasDialog)
	#include <wx/sizer.h>
	#include <wx/button.h>
	#include <wx/dialog.h>
	//*)
#endif
//(*Headers(GLCanvasDialog)
#include <wx/clrpicker.h>
//*)

class GLCanvasDialog: public wxDialog
{
	public:

		GLCanvasDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~GLCanvasDialog();

		//(*Declarations(GLCanvasDialog)
		wxColourPickerCtrl* ColourPickerCtrl;
		wxButton* ButtonOK;
		wxButton* ButtonAbort;
		//*)

		void setBackgroundColor(wxColour color) { ColourPickerCtrl->SetColour(color); }
		wxColour getBackgroundColor() const { return ColourPickerCtrl->GetColour(); }

	protected:

		//(*Identifiers(GLCanvasDialog)
		static const long ID_COLOURPICKERCTRL;
		static const long ID_BUTTONOK;
		static const long ID_BUTTONABORT;
		//*)

	private:

		//(*Handlers(GLCanvasDialog)
		void OnButtonOKClick(wxCommandEvent& event);
		void OnButtonAbortClick(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
