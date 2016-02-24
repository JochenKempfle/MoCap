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


#ifndef POSTPROCESSPANEL_H
#define POSTPROCESSPANEL_H

#include "GLCanvas.h"
#include "TimelinePanel.h"
#include "TreeItemBoneData.h"

#ifndef WX_PRECOMP
	//(*HeadersPCH(PostProcessPanel)
	#include <wx/sizer.h>
	#include <wx/stattext.h>
	#include <wx/checkbox.h>
	#include <wx/listbox.h>
	#include <wx/slider.h>
	#include <wx/panel.h>
	#include <wx/button.h>
	//*)
#endif
//(*Headers(PostProcessPanel)
#include <wx/treectrl.h>
#include <wx/glcanvas.h>
#include <wx/spinctrl.h>
#include <wx/tglbtn.h>
#include <wx/dirctrl.h>
//*)

class PostProcessPanel: public wxPanel
{
	public:

		PostProcessPanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~PostProcessPanel();

		//(*Declarations(PostProcessPanel)
		wxStaticText* StaticText10;
		wxGenericDirCtrl* GenericDirCtrl;
		wxButton* ButtonExport;
		wxButton* ButtonSetSkeleton;
		wxSpinCtrl* SpinCtrlRoll;
		wxSlider* Slider2;
		wxPanel* PanelDragDropSequence;
		wxGridSizer* GridSizerSequenceInfo;
		wxSpinCtrl* SpinCtrlYaw;
		wxStaticText* StaticTextFPS;
		wxPanel* PanelDragDropFrame;
		wxStaticText* StaticText11;
		wxStaticText* StaticTextFrames;
		wxButton* ButtonFromSelection;
		wxStaticText* StaticTextLength;
		wxStaticText* StaticText1;
		wxCheckBox* CheckBoxSelectAll;
		wxStaticText* StaticText3;
		wxTreeCtrl* TreeCtrlSkeleton;
		wxSpinCtrl* SpinCtrlPitch;
		wxStaticText* StaticText5;
		wxButton* ButtonSave;
		TimelinePanel* timelinePanel;
		wxListBox* ListBoxSequences;
		wxToggleButton* ToggleButtonTimeline;
		wxButton* ButtonLoad;
		wxButton* ButtonPlay;
		wxStaticText* StaticText4;
		wxToggleButton* ToggleButtonPreview;
		GLCanvas* glCanvas;
		//*)

	protected:

		//(*Identifiers(PostProcessPanel)
		static const long ID_GLCANVAS;
		static const long ID_SLIDER2;
		static const long ID_BUTTONSETSKELETON;
		static const long ID_BUTTONPLAY;
		static const long ID_TOGGLEBUTTONTIMELINE;
		static const long ID_TOGGLEBUTTONPREVIEW;
		static const long ID_LISTBOXSEQUENCES;
		static const long ID_GENERICDIRCTRL;
		static const long ID_TREECTRLSKELETON;
		static const long ID_STATICTEXT3;
		static const long ID_STATICTEXTFRAMES;
		static const long ID_STATICTEXT4;
		static const long ID_STATICTEXTFPS;
		static const long ID_STATICTEXT5;
		static const long ID_STATICTEXTLENGTH;
		static const long ID_CHECKBOXSELECTALL;
		static const long ID_PANELDRAGDROPSEQUENCE;
		static const long ID_BUTTONEXPORT;
		static const long ID_BUTTONSAVE;
		static const long ID_BUTTONLOAD;
		static const long ID_STATICTEXT1;
		static const long ID_SPINCTRLYAW;
		static const long ID_STATICTEXT10;
		static const long ID_SPINCTRLPITCH;
		static const long ID_STATICTEXT11;
		static const long ID_SPINCTRLROLL;
		static const long ID_BUTTONFROMSELECTION;
		static const long ID_PANELDRAGDROPFRAME;
		static const long ID_TIMELINE;
		//*)
        static const long ID_TIMER;

	private:

		//(*Handlers(PostProcessPanel)
		void OnPanelDragDropPaint(wxPaintEvent& event);
		void OnToggleButtonTimelineToggle(wxCommandEvent& event);
		void OnToggleButtonPreviewToggle(wxCommandEvent& event);
		void OnPanelDragDropSequencePaint(wxPaintEvent& event);
		void OnPanelDragDropFramePaint(wxPaintEvent& event);
		void OnButtonPlayClick(wxCommandEvent& event);
		void OnListBoxSequencesSelect(wxCommandEvent& event);
		void OnPanelDragDropSequenceLeftDown(wxMouseEvent& event);
		void OnPanelDragDropFrameLeftDown(wxMouseEvent& event);
		void OnTimelinePanelLeftUp(wxMouseEvent& event);
		//*)
        void OnMouseCaptureLost(wxMouseCaptureLostEvent& event);
        void OnTimerEvent(wxTimerEvent& event);

		void OnUpdateEvent(wxEvent& event);
		void OnGenericDirCtrlActivated(wxTreeEvent& event);

		void play();
		void stop();
		void updateListBoxSequences();
		void updateSequenceInfo();

		int _currentProjectSequence;

		unsigned int _currentFrame;
		bool _previewMode;

		wxTimer* _timer;

		std::map<int, wxTreeItemId> _treeItemIdFromBoneId;

		DECLARE_EVENT_TABLE()
};

#endif
