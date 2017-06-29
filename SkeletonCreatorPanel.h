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
#include <wx/clrpicker.h>
//*)

class SkeletonCreatorPanel: public wxPanel
{
	public:

		SkeletonCreatorPanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~SkeletonCreatorPanel();

		//(*Declarations(SkeletonCreatorPanel)
		wxStaticText* StaticText10;
		wxStaticText* StaticText9;
		wxButton* ButtonExport;
		wxButton* ButtonConstraint;
		wxTextCtrl* TextCtrlLength;
		wxColourPickerCtrl* ColourPickerCtrl;
		wxTextCtrl* TextCtrlZ;
		wxStaticText* StaticText13;
		wxStaticText* StaticText2;
		wxStaticText* StaticText14;
		wxStaticText* StaticText6;
		wxButton* ButtonLoadDefault;
		wxStaticText* StaticText8;
		wxStaticText* StaticText11;
		wxButton* ButtonImport;
		wxStaticText* StaticText1;
		wxTextCtrl* TextCtrlYaw;
		wxStaticText* StaticText3;
		wxTreeCtrl* TreeCtrlSkeleton;
		wxTextCtrl* TextCtrlRoll;
		wxStaticText* StaticText7;
		wxChoice* ChoiceParent;
		wxButton* ButtonRemoveBone;
		wxTextCtrl* TextCtrlName;
		wxTextCtrl* TextCtrlPitch;
		wxStaticText* StaticText12;
		wxTextCtrl* TextCtrlY;
		wxButton* ButtonAddBone;
		GLCanvas* glCanvas;
		wxTextCtrl* TextCtrlX;
		//*)

		void updateTreeCtrlSkeleton();
		void updateBoneInfo();
		void selectBone(int id);

	protected:

		//(*Identifiers(SkeletonCreatorPanel)
		static const long ID_BUTTONIMPORT;
		static const long ID_BUTTONEXPORT;
		static const long ID_BUTTONLOADDEFAULT;
		static const long ID_GLCANVAS;
		static const long ID_TREECTRLSKELETON;
		static const long ID_STATICTEXT1;
		static const long ID_TEXTCTRLNAME;
		static const long ID_STATICTEXT2;
		static const long ID_CHOICEPARENT;
		static const long ID_STATICTEXT9;
		static const long ID_TEXTCTRLLENGTH;
		static const long ID_STATICTEXT14;
		static const long ID_COLOURPICKERCTRL;
		static const long ID_STATICTEXT8;
		static const long ID_STATICTEXT7;
		static const long ID_STATICTEXT6;
		static const long ID_STATICTEXT3;
		static const long ID_TEXTCTRLROLL;
		static const long ID_TEXTCTRLPITCH;
		static const long ID_TEXTCTRLYAW;
		static const long ID_BUTTONCONSTRAINT;
		static const long ID_BUTTONADDBONE;
		static const long ID_BUTTONREMOVEBONE;
		static const long ID_STATICTEXT10;
		static const long ID_STATICTEXT11;
		static const long ID_STATICTEXT12;
		static const long ID_STATICTEXT13;
		static const long ID_TEXTCTRLX;
		static const long ID_TEXTCTRLY;
		static const long ID_TEXTCTRLZ;
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
		void OnButtonImportClick(wxCommandEvent& event);
		void OnButtonExportClick(wxCommandEvent& event);
		void OnButtonLoadDefaultClick(wxCommandEvent& event);
		void OnButtonAddBoneClick(wxCommandEvent& event);
		void OnButtonRemoveBoneClick(wxCommandEvent& event);
		void OnTextCtrlOrientationTextEnter(wxCommandEvent& event);
		void OnTextCtrlPositionTextEnter(wxCommandEvent& event);
		void OnglCanvasMouseMove(wxMouseEvent& event);
		void OnglCanvasLeftUp(wxMouseEvent& event);
		void OnButton1Click(wxCommandEvent& event);
		void OnButtonConstraintClick(wxCommandEvent& event);
		//*)
        void OnMouseCaptureLost(wxMouseCaptureLostEvent& event);

		DECLARE_EVENT_TABLE()

		int _clickedArrow;
		wxPoint _mousePos;
		// Skeleton* _skeleton;
		std::map<int, wxTreeItemId> _treeItemIdFromBoneId;
		// TODO(JK#2#2017-06-29): application crashes when recording (or simulating?) a motion with a custom skeleton
};

#endif
