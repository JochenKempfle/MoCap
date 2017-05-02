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


#ifndef CONSTRAINTDIALOG_H
#define CONSTRAINTDIALOG_H

#include "JointConstraint.h"
#include <vector>

#ifndef WX_PRECOMP
	//(*HeadersPCH(ConstraintDialog)
	#include <wx/sizer.h>
	#include <wx/panel.h>
	#include <wx/button.h>
	#include <wx/dialog.h>
	//*)
#endif
//(*Headers(ConstraintDialog)
#include "GLCanvas.h"
//*)

class ConstraintDialog: public wxDialog
{
	public:

		ConstraintDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~ConstraintDialog();

		void setConstraint(const JointConstraint &constraint);
		JointConstraint getConstraint() const;

		//(*Declarations(ConstraintDialog)
		wxButton* Button1;
		wxPanel* Panel1;
		wxButton* Button2;
		wxPanel* PanelSetConstraint;
		GLCanvas* glCanvas;
		//*)

	protected:

		//(*Identifiers(ConstraintDialog)
		static const long ID_GLCANVAS;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		static const long ID_PANELSETCONSTRAINT;
		static const long ID_PANEL1;
		//*)

	private:

		//(*Handlers(ConstraintDialog)
		void OnPanelSetConstraintPaint(wxPaintEvent& event);
		void OnPanelSetConstraintLeftDown(wxMouseEvent& event);
		void OnPanelSetConstraintRightDown(wxMouseEvent& event);
		void OnPanelSetConstraintMouseMove(wxMouseEvent& event);
		void OnPanelSetConstraintLeftUp(wxMouseEvent& event);
		//*)
		void OnPanelSetConstraintMouseCaptureLost(wxMouseCaptureLostEvent& event);

		// returns the interpolated angle value for the start/end point of the constraint canvas
		int getStartEndAngle() const;

		JointConstraint _constraint;

		wxPoint _mousePos;
		int _startEndAngle;

		std::map<int, int> _points;
		std::map<int, int>::iterator _pointIterator;

		DECLARE_EVENT_TABLE()
};

#endif
