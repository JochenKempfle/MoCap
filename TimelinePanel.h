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
#include "AnimationManager.h"
#include "GLCanvas.h"

#ifndef WX_PRECOMP
	//(*HeadersPCH(TimelinePanel)
	#include <wx/sizer.h>
	#include <wx/panel.h>
	#include <wx/button.h>
	//*)
#endif
//(*Headers(TimelinePanel)
#include <wx/tglbtn.h>
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
		wxToggleButton* ToggleButtonStickyEnds;
		wxButton* ButtonClear;
		wxButton* ButtonCut;
		wxButton* ButtonDecompose;
		//*)

		// TODO(JK#8#2017-02-24): Maybe add moveToCursor/navigation (front, back) function in timeline panel

		void prepareAddingSequence(int sequence, const std::vector<int> &channels);
		void prepareAddingFrame(const MotionSequenceFrame &frame, float frameTime);

		void setCursorPosition(uint64_t time);
		uint64_t getCursorPosition() const { return _cursorPosition; }

		uint64_t getStartMarkerTime() const { return _startMarkerTime; }
		uint64_t getEndMarkerTime() const { return _endMarkerTime; }
		uint64_t getEndTime() const { return theAnimationManager.getTimeline()->getMaxTime(); }

		void update();

		void setGLCanvas(GLCanvas* glCanvas) { _glCanvas = glCanvas; }


	protected:

		//(*Identifiers(TimelinePanel)
		static const long ID_BUTTONCLEAR;
		static const long ID_BUTTONREMOVE;
		static const long ID_BUTTONCUT;
		static const long ID_BUTTONDECOMPOSE;
		static const long ID_TOGGLEBUTTONSTICKYENDS;
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
		void OnButtonCutClick(wxCommandEvent& event);
		void OnRightDown(wxMouseEvent& event);
		void OnRightUp(wxMouseEvent& event);
		void OnToggleButtonStickyEndsToggle(wxCommandEvent& event);
		void OnButtonClearClick(wxCommandEvent& event);
		void OnButtonRemoveClick(wxCommandEvent& event);
		void OnButtonDecomposeClick(wxCommandEvent& event);
		//*)
        void OnMouseCaptureLost(wxMouseCaptureLostEvent& event);
        void OnPopupBonesClick(wxCommandEvent& event);
        void OnPopupSkeletonsClick(wxCommandEvent& event);

        void showPopUpBones();
        void showPopUpSkeletons();

		void endDragDrop();

		GLCanvas* _glCanvas;

        int getLengthFromTime(uint64_t time) const;
        int getLengthFromTime(float time) const;
        int getPositionFromTime(int64_t time) const;
        // returns the time point at which the given point will be in current setting (zoom factor, time offset)
        // always check if time is negative, this means the point is before the beginning and therefore invalid
        int64_t getTimeFromPosition(wxPoint pos) const;
        int64_t getTimeFromPosition(int posX) const;
        int getChannelFromPosition(wxPoint pos) const;
        int getPositionFromChannel(int channel) const;

		void drawTrack(wxDC* dc, TimelineTrack* track, wxPoint pos) const;
        void drawEditSelectedTrack(wxDC* dc, TimelineTrack* track, wxPoint pos) const;

		int64_t _ysPerTimeUnit;
		int64_t _timeOffset;
		int _channelOffset;
		uint64_t _cursorPosition;

		// the x offset of the mouse to the beginning of a track (when clicked and dragged)
		int _mouseToTrackOffset;

        bool _clickedTime;
        // TODO(JK#3#): remove _clickedChannel if not needed
        int _clickedChannel;

        int _selectedTrack;
        bool _editSelectedTrack;

        unsigned int _oldWeightPointFrame;

        bool _resizeTrack;

        bool _dragging;
        bool _dragIsValid;
        int _draggedTrackPos;
        int _draggedTrackChannel;
        int64_t _draggedTrackStartTime;
        uint64_t _draggedTrackLength;

        bool _interpolationPossible;
        uint64_t _interpolationStartTime;
        uint64_t _interpolationEndTime;
        int _interpolationChannel;

        int _sequenceToAdd;
        TimelineTrack _trackToAdd;
        std::vector<int> _channelsToAdd;

        bool _hasStickyEnds;
        int _stickyEndPixelSize;

        int64_t _startMarkerTime;
        int64_t _endMarkerTime;

        bool _cursorOnStartMarker;
        bool _cursorOnEndMarker;
        bool _clickedStartMarker;
        bool _clickedEndMarker;

		// std::vector<std::vector<MotionTrack> > _channelContent;

		const wxSize _channelButtonsSize = wxSize(13, 13);

		// rendering constants
        const int _optionsHeight = 30;
        const int _timeHeight = 17;
        const int _channelHeight = 40;
        const int _channelOptionsWidth = 100;
        const int _numPixelsPerTimeUnit = 100;

        const int _timelineStartX = _channelOptionsWidth + 1;
        const int _timelineStartY = _optionsHeight + _timeHeight;

        // const int timeResolution = 1000;
        const int64_t _maxTime = 42000000000;

		DECLARE_EVENT_TABLE()
};

#endif
