#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#ifndef WX_PRECOMP
	//(*HeadersPCH(FilterDialog)
	#include <wx/sizer.h>
	#include <wx/stattext.h>
	#include <wx/listbox.h>
	#include <wx/button.h>
	#include <wx/dialog.h>
	//*)
#endif
//(*Headers(FilterDialog)
#include <wx/spinctrl.h>
//*)

#include <vector>
#include "MotionFilterBase.h"

class FilterDialog: public wxDialog
{
	public:

		FilterDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~FilterDialog();

		void setFilter(std::vector<MotionFilterBase*> filters, unsigned int selectedFilter);
		void setFrameTime(unsigned int frameTime);

		int getSelectedFilter() const;
		unsigned int getFrameTime() const;

		//(*Declarations(FilterDialog)
		wxListBox* ListBoxFilter;
		wxButton* ButtonOK;
		wxStaticText* StaticText1;
		wxButton* ButtonCancel;
		wxSpinCtrl* SpinCtrlFrameTime;
		//*)

	protected:

		//(*Identifiers(FilterDialog)
		static const long ID_LISTBOXFILTER;
		static const long ID_SPINCTRLFRAMETIME;
		static const long ID_STATICTEXT1;
		static const long ID_BUTTONOK;
		static const long ID_BUTTONCANCEL;
		//*)

	private:

		//(*Handlers(FilterDialog)
		void OnButtonOKClick(wxCommandEvent& event);
		void OnButtonCancelClick(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
