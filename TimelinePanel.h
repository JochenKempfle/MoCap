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


#ifndef TIMELINEPANEL_H
#define TIMELINEPANEL_H

#include <vector>
#include "TimelineTrack.h"

#ifndef WX_PRECOMP
	//(*HeadersPCH(TimelinePanel)
	#include <wx/sizer.h>
	#include <wx/panel.h>
	#include <wx/button.h>
	//*)
#endif
//(*Headers(TimelinePanel)
//*)

class TimelinePanel: public wxPanel
{
	public:

		TimelinePanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~TimelinePanel();

		//(*Declarations(TimelinePanel)
		wxButton* ButtonZoomOut;
		wxButton* ButtonZoomIn;
		wxButton* ButtonRemove;
		wxButton* ButtonClear;
		wxButton* ButtonCut;
		//*)

	protected:

		//(*Identifiers(TimelinePanel)
		static const long ID_BUTTONCLEAR;
		static const long ID_BUTTONREMOVE;
		static const long ID_BUTTONCUT;
		static const long ID_BUTTONZOOMIN;
		static const long ID_BUTTONZOOMOUT;
		//*)

	private:

		//(*Handlers(TimelinePanel)
		void OnPaint(wxPaintEvent& event);
		void OnLeftDown(wxMouseEvent& event);
		void OnLeftUp(wxMouseEvent& event);
		void OnMouseMove(wxMouseEvent& event);
		void OnMouseWheel(wxMouseEvent& event);
		void OnButtonZoomInClick(wxCommandEvent& event);
		void OnButtonZoomOutClick(wxCommandEvent& event);
		void OnMouseEnter(wxMouseEvent& event);
		void OnMouseLeave(wxMouseEvent& event);
		void OnKillFocus(wxFocusEvent& event);
		//*)

		void prepareAddingSequence(MotionSequence* sequence);
		void prepareAddingSequence(MotionSequenceChannel* channel);
		void prepareAddingFrame(MotionSequenceFrame frame);

		int _msPerTimeUnit;
		int _timeOffset;
		int _channelOffset;
		int _cursorPosition;

        bool _clickedTime;
        int _clickedChannel;

		// std::vector<std::vector<MotionTrack> > _channelContent;

		// rendering constants
        const int _optionsHeight = 30;
        const int _timeHeight = 17;
        const int _channelHeight = 40;
        const int _channelOptionsWidth = 100;
        const int _numPixelsPerTimeUnit = 100;

		DECLARE_EVENT_TABLE()
};

#endif
