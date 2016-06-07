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


#ifndef MOTIONPLAYERPANEL_H
#define MOTIONPLAYERPANEL_H

#include "GLCanvas.h"
#include "TreeItemBoneData.h"


#ifndef WX_PRECOMP
	//(*HeadersPCH(MotionPlayerPanel)
	#include <wx/sizer.h>
	#include <wx/stattext.h>
	#include <wx/listbox.h>
	#include <wx/slider.h>
	#include <wx/panel.h>
	#include <wx/button.h>
	//*)
#endif
//(*Headers(MotionPlayerPanel)
#include <wx/treectrl.h>
#include <wx/glcanvas.h>
//*)

class MotionPlayerPanel: public wxPanel
{
	public:

		MotionPlayerPanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~MotionPlayerPanel();

		//(*Declarations(MotionPlayerPanel)
		wxSlider* SliderFrames;
		wxGridSizer* GridSizerSequenceInfo;
		wxStaticText* StaticTextFPS;
		wxStaticText* StaticTextFrames;
		wxStaticText* StaticTextLength;
		wxStaticText* StaticText1;
		wxStaticText* StaticText3;
		wxTreeCtrl* TreeCtrlSkeleton;
		wxStaticText* StaticText5;
		wxButton* ButtonSave;
		wxListBox* ListBoxSequences;
		wxButton* ButtonLoad;
		wxButton* ButtonPlay;
		GLCanvas* glCanvas;
		//*)

	protected:

		//(*Identifiers(MotionPlayerPanel)
		static const long ID_GLCANVAS;
		static const long ID_SLIDERFRAMES;
		static const long ID_BUTTONPLAY;
		static const long ID_TREECTRLSKELETON;
		static const long ID_STATICTEXT1;
		static const long ID_STATICTEXTFRAMES;
		static const long ID_STATICTEXT3;
		static const long ID_STATICTEXTFPS;
		static const long ID_STATICTEXT5;
		static const long ID_STATICTEXTLENGTH;
		static const long ID_LISTBOXSEQUENCES;
		static const long ID_BUTTONLOAD;
		static const long ID_BUTTONSAVE;
		//*)

        static const long ID_TIMER;

	private:

		//(*Handlers(MotionPlayerPanel)
		void OnButtonPlayClick(wxCommandEvent& event);
		void OnButtonLoadClick(wxCommandEvent& event);
		void OnButtonSaveClick(wxCommandEvent& event);
		void OnListBoxSequencesSelect(wxCommandEvent& event);
		void OnSliderFramesCmdSliderUpdated(wxScrollEvent& event);
		//*)

        void OnTimerEvent(wxTimerEvent& event);
		void OnUpdateEvent(wxEvent& event);

		void play();
		void stop();
		void updateListBoxSequences();
		void updateSequenceInfo();

		int _currentProjectSequence;

		unsigned int _currentFrame;

		wxTimer* _timer;

		std::map<int, wxTreeItemId> _treeItemIdFromBoneId;

		DECLARE_EVENT_TABLE()
};

#endif
