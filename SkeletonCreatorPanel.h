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


#ifndef SKELETONCREATORPANEL_H
#define SKELETONCREATORPANEL_H


#include "GLCanvas.h"
#include "TreeItemBoneData.h"
#include <map>
#include <wx/valnum.h>

#ifndef WX_PRECOMP
	//(*HeadersPCH(SkeletonCreatorPanel)
	#include <wx/sizer.h>
	#include <wx/stattext.h>
	#include <wx/textctrl.h>
	#include <wx/panel.h>
	#include <wx/choice.h>
	#include <wx/button.h>
	//*)
#endif
//(*Headers(SkeletonCreatorPanel)
#include <wx/treectrl.h>
#include <wx/glcanvas.h>
//*)

class SkeletonCreatorPanel: public wxPanel
{
	public:

		SkeletonCreatorPanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~SkeletonCreatorPanel();

		//(*Declarations(SkeletonCreatorPanel)
		wxStaticText* StaticText9;
		wxTextCtrl* TextCtrl4;
		wxTextCtrl* TextCtrlLength;
		wxStaticText* StaticText2;
		wxStaticText* StaticText6;
		wxTextCtrl* TextCtrl6;
		wxStaticText* StaticText8;
		wxStaticText* StaticText1;
		wxStaticText* StaticText3;
		wxTreeCtrl* TreeCtrlSkeleton;
		wxStaticText* StaticText5;
		wxStaticText* StaticText7;
		wxButton* ButtonSave;
		wxChoice* ChoiceParent;
		wxTextCtrl* TextCtrl8;
		wxButton* ButtonRemoveBone;
		wxTextCtrl* TextCtrl7;
		wxTextCtrl* TextCtrlName;
		wxButton* ButtonReset;
		wxTextCtrl* TextCtrl9;
		wxTextCtrl* TextCtrl5;
		wxButton* ButtonLoad;
		wxButton* ButtonAddBone;
		wxStaticText* StaticText4;
		wxTextCtrl* TextCtrl3;
		wxTextCtrl* TextCtrl10;
		GLCanvas* glCanvas;
		wxTextCtrl* TextCtrl11;
		//*)

		void updateTreeCtrlSkeleton();
		void updateBoneInfo();
		void selectBone(int id);

	protected:

		//(*Identifiers(SkeletonCreatorPanel)
		static const long ID_BUTTONLOAD;
		static const long ID_BUTTONSAVE;
		static const long ID_BUTTONRESET;
		static const long ID_GLCANVAS;
		static const long ID_TREECTRLSKELETON;
		static const long ID_STATICTEXT1;
		static const long ID_TEXTCTRLNAME;
		static const long ID_STATICTEXT2;
		static const long ID_CHOICEPARENT;
		static const long ID_STATICTEXT9;
		static const long ID_TEXTCTRLLENGTH;
		static const long ID_STATICTEXT6;
		static const long ID_STATICTEXT7;
		static const long ID_STATICTEXT8;
		static const long ID_STATICTEXT3;
		static const long ID_TEXTCTRL3;
		static const long ID_TEXTCTRL6;
		static const long ID_TEXTCTRL9;
		static const long ID_STATICTEXT4;
		static const long ID_TEXTCTRL4;
		static const long ID_TEXTCTRL7;
		static const long ID_TEXTCTRL10;
		static const long ID_STATICTEXT5;
		static const long ID_TEXTCTRL5;
		static const long ID_TEXTCTRL8;
		static const long ID_TEXTCTRL11;
		static const long ID_BUTTONADDBONE;
		static const long ID_BUTTONREMOVEBONE;
		//*)

	private:

		//(*Handlers(SkeletonCreatorPanel)
		void OnTreeCtrlSkeletonEndLabelEdit(wxTreeEvent& event);
		void OnTreeCtrlSkeletonDeleteItem(wxTreeEvent& event);
		void OnTreeCtrlSkeletonSelectionChanged(wxTreeEvent& event);
		void OnTreeCtrlSkeletonKeyDown(wxTreeEvent& event);
		void OnGlCanvasLeftDown(wxMouseEvent& event);
		void OnChoiceParentSelect(wxCommandEvent& event);
		void OnTextCtrlNameTextEnter(wxCommandEvent& event);
		void OnTextCtrlLengthTextEnter(wxCommandEvent& event);
		//*)
        void OnMouseCaptureLost(wxMouseCaptureLostEvent& event);

		DECLARE_EVENT_TABLE()

		// Skeleton* _skeleton;
		std::map<int, wxTreeItemId> _treeItemIdFromBoneId;
};

#endif
