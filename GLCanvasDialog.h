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


#ifndef GLCANVASDIALOG_H
#define GLCANVASDIALOG_H

#ifndef WX_PRECOMP
	//(*HeadersPCH(GLCanvasDialog)
	#include <wx/sizer.h>
	#include <wx/checklst.h>
	#include <wx/slider.h>
	#include <wx/button.h>
	#include <wx/dialog.h>
	//*)
#endif
//(*Headers(GLCanvasDialog)
#include <wx/clrpicker.h>
//*)

#include <vector>
#include <utility>
#include "Skeleton.h"


class GLCanvasDialog: public wxDialog
{
	public:

		GLCanvasDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~GLCanvasDialog();

		//(*Declarations(GLCanvasDialog)
		wxColourPickerCtrl* ColourPickerCtrl;
		wxSlider* SliderLineWidth;
		wxSlider* SliderTraceLength;
		wxButton* ButtonOK;
		wxButton* ButtonAbort;
		wxCheckListBox* CheckListBoxTraceBones;
		//*)

		void setBackgroundColor(wxColour color) { ColourPickerCtrl->SetColour(color); }
		wxColour getBackgroundColor() const { return ColourPickerCtrl->GetColour(); }

		void setLineWidth(float len) { SliderLineWidth->SetValue(int(len < 1.0f ? 1.0f : len > 5.0f ? 5.0f : len)); }
		float getLineWidth() const { return float(SliderLineWidth->GetValue()); }

		void setTraceLength(int length);
		int getTraceLength() const;

		void setSkeleton(const Skeleton* skeleton);
		void setBonesWithTrace(std::vector<int> boneIdsWithTrace);
		std::vector<int> getBonesWithTrace() const;

	protected:

		//(*Identifiers(GLCanvasDialog)
		static const long ID_COLOURPICKERCTRL;
		static const long ID_SLIDERLINEWIDTH;
		static const long ID_CHECKLISTBOXTRACEBONES;
		static const long ID_SLIDERTRACELENGTH;
		static const long ID_BUTTONOK;
		static const long ID_BUTTONABORT;
		//*)

	private:

		//(*Handlers(GLCanvasDialog)
		void OnButtonOKClick(wxCommandEvent& event);
		void OnButtonAbortClick(wxCommandEvent& event);
		//*)

		std::vector<std::pair<int, std::string> > _boneIdsAndNames;

		DECLARE_EVENT_TABLE()
};

#endif
