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


#include "wx_pch.h"
#include "TimelinePanel.h"
#include "AnimationManager.h"
#include "MoCapManager.h"
#include "TimelineOverlayDialog.h"
#include <wx/dcbuffer.h>
#include <wx/richtext/richtextbuffer.h>
#include <algorithm>

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(TimelinePanel)
	#include <wx/intl.h>
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(TimelinePanel)
//*)

//(*IdInit(TimelinePanel)
const long TimelinePanel::ID_BUTTONCLEAR = wxNewId();
const long TimelinePanel::ID_BUTTONREMOVE = wxNewId();
const long TimelinePanel::ID_BUTTONCUT = wxNewId();
const long TimelinePanel::ID_BUTTONZOOMIN = wxNewId();
const long TimelinePanel::ID_BUTTONZOOMOUT = wxNewId();
//*)

BEGIN_EVENT_TABLE(TimelinePanel,wxPanel)
	//(*EventTable(TimelinePanel)
	//*)
END_EVENT_TABLE()

TimelinePanel::TimelinePanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
    SetBackgroundStyle(wxBG_STYLE_PAINT);
	//(*Initialize(TimelinePanel)
	wxBoxSizer* BoxSizer1;

	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxFULL_REPAINT_ON_RESIZE, _T("id"));
	SetBackgroundColour(wxColour(120,120,120));
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer1->Add(100,10,0, wxALL|wxALIGN_TOP, 0);
	ButtonClear = new wxButton(this, ID_BUTTONCLEAR, _("Clear All"), wxDefaultPosition, wxSize(-1,26), 0, wxDefaultValidator, _T("ID_BUTTONCLEAR"));
	BoxSizer1->Add(ButtonClear, 0, wxALL|wxALIGN_TOP, 2);
	ButtonRemove = new wxButton(this, ID_BUTTONREMOVE, _("Remove"), wxDefaultPosition, wxSize(-1,26), 0, wxDefaultValidator, _T("ID_BUTTONREMOVE"));
	BoxSizer1->Add(ButtonRemove, 0, wxALL|wxALIGN_TOP, 2);
	ButtonCut = new wxButton(this, ID_BUTTONCUT, _("Cut"), wxDefaultPosition, wxSize(-1,26), 0, wxDefaultValidator, _T("ID_BUTTONCUT"));
	BoxSizer1->Add(ButtonCut, 0, wxALL|wxALIGN_TOP, 2);
	BoxSizer1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonZoomIn = new wxButton(this, ID_BUTTONZOOMIN, _("Zoom +"), wxDefaultPosition, wxSize(-1,26), 0, wxDefaultValidator, _T("ID_BUTTONZOOMIN"));
	BoxSizer1->Add(ButtonZoomIn, 0, wxALL|wxALIGN_TOP, 2);
	ButtonZoomOut = new wxButton(this, ID_BUTTONZOOMOUT, _("Zoom -"), wxDefaultPosition, wxSize(-1,26), 0, wxDefaultValidator, _T("ID_BUTTONZOOMOUT"));
	BoxSizer1->Add(ButtonZoomOut, 0, wxALL|wxALIGN_TOP, 2);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);

	Connect(ID_BUTTONCUT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&TimelinePanel::OnButtonCutClick);
	Connect(ID_BUTTONZOOMIN,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&TimelinePanel::OnButtonZoomInClick);
	Connect(ID_BUTTONZOOMOUT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&TimelinePanel::OnButtonZoomOutClick);
	Connect(wxEVT_PAINT,(wxObjectEventFunction)&TimelinePanel::OnPaint);
	Connect(wxEVT_KILL_FOCUS,(wxObjectEventFunction)&TimelinePanel::OnKillFocus);
	Connect(wxEVT_LEFT_DOWN,(wxObjectEventFunction)&TimelinePanel::OnLeftDown);
	Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&TimelinePanel::OnLeftUp);
	Connect(wxEVT_RIGHT_DOWN,(wxObjectEventFunction)&TimelinePanel::OnRightDown);
	Connect(wxEVT_RIGHT_UP,(wxObjectEventFunction)&TimelinePanel::OnRightUp);
	Connect(wxEVT_MOTION,(wxObjectEventFunction)&TimelinePanel::OnMouseMove);
	Connect(wxEVT_ENTER_WINDOW,(wxObjectEventFunction)&TimelinePanel::OnMouseEnter);
	Connect(wxEVT_LEAVE_WINDOW,(wxObjectEventFunction)&TimelinePanel::OnMouseLeave);
	Connect(wxEVT_MOUSEWHEEL,(wxObjectEventFunction)&TimelinePanel::OnMouseWheel);
	//*)
	Connect(wxEVT_MOUSE_CAPTURE_LOST, (wxObjectEventFunction)&TimelinePanel::OnMouseCaptureLost);

	_ysPerTimeUnit = 1000000;
	_timeOffset = 0;
	_channelOffset = 0;
	_cursorPosition = 0;
	_clickedTime = false;
	_clickedChannel = -1;
	_selectedTrack = -1;
	_editSelectedTrack = false;
	_resizeTrack = false;
	_dragging = false;
    _draggedTrackPos = 0;
    _draggedTrackChannel = 0;
    _draggedTrackLength = 0;
    _dragIsValid = true;
    _sequenceToAdd = -1;
    _glCanvas = nullptr;

    _interpolationPossible = false;
    _interpolationStartTime = -1;
    _interpolationEndTime = -1;
}

TimelinePanel::~TimelinePanel()
{
	//(*Destroy(TimelinePanel)
	//*)
}

void TimelinePanel::prepareAddingSequence(int sequence, const std::vector<int> &channels)
{
    if (sequence < 0)
    {
        return;
    }
    _dragging = true;
    // initially the drag must not be valid as dragging starts outside the timeline
    _dragIsValid = false;
    _draggedTrackLength = theAnimationManager.getProjectSequence(sequence)->getLength() * 1000;
    _mouseToTrackOffset = 30;
    _sequenceToAdd = sequence;
    _channelsToAdd = channels;
}

void TimelinePanel::prepareAddingFrame(const MotionSequenceFrame &frame, float frameTime)
{
    _dragging = true;
    _dragIsValid = false;
    // convert frame time given as a fraction of seconds to time in ys
    _draggedTrackLength = frameTime * 1000 * 1000;
    _mouseToTrackOffset = 5;
    _trackToAdd.clear();
    _trackToAdd.appendFrame(frame);
    _trackToAdd.setFrameTime(frameTime);
    _trackToAdd.setName("Custom Frame");
    //_frameToAdd = frame;
}

void TimelinePanel::setCursorPosition(uint64_t time)
{
    _cursorPosition = time;
    update();
}

void TimelinePanel::update()
{
    Refresh();
    Update();
    if (_glCanvas != nullptr)
    {
        theAnimationManager.getTimeline()->setSkeletonToTime(_cursorPosition);
        _glCanvas->Refresh();
        _glCanvas->Update();
    }
}

void TimelinePanel::OnMouseCaptureLost(wxMouseCaptureLostEvent& event)
{
    if (HasCapture())
    {
        ReleaseMouse();
    }
    SetCursor(wxCURSOR_ARROW);
    _dragging = false;
    _clickedTime = false;
    endDragDrop();
}

void TimelinePanel::OnPopupBonesClick(wxCommandEvent &event)
{
    int id = event.GetId();
    if (id == theAnimationManager.getTimelineSkeleton()->getNextFreeId())
    {
        // set channel to have no bone
        theAnimationManager.getTimeline()->setChannelAffiliation(_clickedChannel, -1);
    }
    else if (id == theAnimationManager.getTimelineSkeleton()->getNextFreeId() + 1)
    {
        // set as position channel
    }
    else
    {
        theAnimationManager.getTimeline()->setChannelAffiliation(_clickedChannel, id);
    }
}

void TimelinePanel::OnPopupSkeletonsClick(wxCommandEvent &event)
{
    int id = event.GetId();

    std::vector<MotionSequence*> sequences = theAnimationManager.getProjectSequences();
    if (id < int(sequences.size()))
    {
        theAnimationManager.setTimelineSkeleton(sequences[id]->getSkeleton());
    }
    else
    {
        int i = id - sequences.size();
        if (i == 0)
        {
            // TODO(JK#2#): load default skeleton for timeline
        }
        else
        {
            // TODO(JK#2#): load a skeleton from file for timeline
        }
    }
    theAnimationManager.getTimelineSkeleton()->setToDefault();
    _glCanvas->Refresh();
    // TODO(JK#2#): when a new skeleton is set for the timeline, all channels may become invalid
}

void TimelinePanel::showPopUpBones()
{
    auto bones = theAnimationManager.getTimelineSkeleton()->getBoneIdsWithName();
    wxMenu menu;
    menu.Append(theAnimationManager.getTimelineSkeleton()->getNextFreeId(), _("None"));
    menu.Append(theAnimationManager.getTimelineSkeleton()->getNextFreeId() + 1, _("Position"));
    for (size_t i = 0; i < bones.size(); ++i)
    {
        wxString name = bones[i].second;
        if (name.size() == 0)
        {
            // if the bone has no name, simply show "Bone" + id
            name << _("Bone ") << bones[i].first;
        }
        menu.Append(bones[i].first, name);
    }
 	menu.Connect(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&TimelinePanel::OnPopupBonesClick, NULL, this);
 	PopupMenu(&menu);
}

void TimelinePanel::showPopUpSkeletons()
{
    std::vector<MotionSequence*> sequences = theAnimationManager.getProjectSequences();
    wxMenu menu;
    menu.Append(sequences.size(), _("Default Skeleton"));
    menu.Append(sequences.size() + 1, _("Open From File"));
    for (size_t i = 0; i < sequences.size(); ++i)
    {
        wxString name = sequences[i]->getSkeleton()->getName();
        if (name.size() == 0)
        {
            // if the skeleton has no name, simply show the motion sequence name
            name = sequences[i]->getName();
        }
        menu.Append(i, name);
    }
 	menu.Connect(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&TimelinePanel::OnPopupSkeletonsClick, NULL, this);
 	PopupMenu(&menu);
}

void TimelinePanel::endDragDrop()
{
    // check if currently dragging
    if (!_dragging)
    {
        _dragIsValid = true;
        _sequenceToAdd = -1;
        _channelsToAdd.clear();
        _trackToAdd.clear();
        return;
    }

    if (_dragIsValid)
    {
        int64_t time = getTimeFromPosition(wxPoint(_draggedTrackPos, 0));
        // time should normally not be smaller than zero, but to be sure, better check if time is valid
        if (time < 0)
        {
            time = 0;
        }

        // check if a sequence is about to be inserted or if a track is moved
        // do not swap the order of the if statements, as there can be a track selected while inserting a new track
        if (_sequenceToAdd >= 0)
        {
            theAnimationManager.addChannelsToTimeline(_sequenceToAdd, _channelsToAdd, _draggedTrackChannel, time);
        }
        else if (_trackToAdd.getNumFrames() > 0)
        {
            theAnimationManager.addTrackToTimeline(_trackToAdd, _draggedTrackChannel, time);
        }
        else if (_selectedTrack >= 0)
        {
            Timeline* timeline = theAnimationManager.getTimeline();
            timeline->moveTrack(_selectedTrack, _draggedTrackChannel, time);
        }
    }
    else
    {
        if (_sequenceToAdd >= 0)
        {
            // don't care, especially do not insert the channels of the sequence ;)
        }
        else if (_trackToAdd.getNumFrames() > 0)
        {
            // don't care
        }
        else if (_selectedTrack >= 0)
        {
            // dragging a track out of the timeline means erasing it
            Timeline* timeline = theAnimationManager.getTimeline();
            timeline->erase(_selectedTrack);
            _selectedTrack = -1;
        }
    }

    _dragging = false;
    _dragIsValid = true;
    _sequenceToAdd = -1;
    _channelsToAdd.clear();
    _trackToAdd.clear();
    update();
}

int TimelinePanel::getLengthFromTime(uint64_t time) const
{
    // add  + _ysPerTimeUnit/2 to avoid rounding errors, otherwise one gets always floor(value)
    return (_numPixelsPerTimeUnit * time + _ysPerTimeUnit/2) / _ysPerTimeUnit;
}

int TimelinePanel::getLengthFromTime(float time) const
{
    return (_numPixelsPerTimeUnit * time) / _ysPerTimeUnit;
}

int TimelinePanel::getPositionFromTime(int64_t time) const
{
    // add  + _ysPerTimeUnit/2 in the division to avoid rounding errors, otherwise one gets always floor(value)
    return _timelineStartX + (_numPixelsPerTimeUnit * (time - _timeOffset) + _ysPerTimeUnit/2) / _ysPerTimeUnit;
}

int64_t TimelinePanel::getTimeFromPosition(wxPoint pos) const
{
    return (_ysPerTimeUnit * (pos.x - _timelineStartX)) / _numPixelsPerTimeUnit + _timeOffset;
}

int TimelinePanel::getChannelFromPosition(wxPoint pos) const
{
    return (pos.y - _timelineStartY)/_channelHeight + _channelOffset;
}

int TimelinePanel::getPositionFromChannel(int channel) const
{
    return _timelineStartY + (channel - _channelOffset) * _channelHeight;
}

void TimelinePanel::drawTrack(wxDC* dc, TimelineTrack* track, wxPoint pos) const
{
    if (dc == nullptr || track == nullptr)
    {
        return;
    }

    dc->SetPen(wxPen(wxColour(0, 0, 0)));

    wxFont font = dc->GetFont();
    font.SetPointSize(8);
    dc->SetFont(font);

    // get background colour of track (depends on being selected)
    wxColour bgColour;
    if (track->getId() == _selectedTrack)
    {
        bgColour = wxColor(200, 200, 100);
    }
    else
    {
        bgColour = wxColor(200, 200, 200);
    }
    dc->SetBrush(wxBrush(bgColour));
    int length = getLengthFromTime(track->getLength());
    // draw track background and rect around it
    dc->DrawRectangle(pos, wxSize(length + 1, _channelHeight + 1));

    // draw track info
    int textExtent = dc->GetTextExtent(track->getName()).x;
    if (textExtent + 40 < length)
    {
        wxPoint textPosition(pos.x + 20, pos.y + 10);
        if (textPosition.x < _timelineStartX + 20)
        {
            textPosition.x = _timelineStartX + 20;
            if (textPosition.x + textExtent + 20 > pos.x + length)
            {
                textPosition.x = pos.x + length - textExtent - 20;
            }
        }
        dc->DrawText(track->getName(), textPosition);
    }

    // draw overlapping and weighting only when not dragging
    // TODO(JK#9#): track content of selected track disappears while adding new sequence or track (_dragging and _selectedTrack are set in this case!)
    if (!(_dragging && _selectedTrack == track->getId()))
    {
        // draw overlapping
        std::vector<TimelineOverlay*> overlays = theAnimationManager.getTimeline()->getOverlays(track);
        std::sort(overlays.begin(), overlays.end(), TimelineOverlay::compChannelDistHigh);

        for (size_t i = 0; i < overlays.size(); ++i)
        {
            wxPoint start(pos);
            wxSize overlapLength(0, _channelHeight/3);
            wxDirection gradientDirection = wxDOWN;

            TimelineTrack* overlayTrack = overlays[i]->getFirstTrack() != track ? overlays[i]->getFirstTrack() : overlays[i]->getSecondTrack();
            if (track->getChannel() < overlayTrack->getChannel())
            {
                start.y += _channelHeight - overlapLength.y + 1;
                gradientDirection = wxUP;
            }

            start.x = getPositionFromTime(overlays[i]->getStartTime());
            overlapLength.x = getLengthFromTime(overlays[i]->getLength()) + 1;

            wxColour overlapColour;

            if (overlays[i]->getType() == OverlayType::ADDITIVE)
            {
                overlapColour = wxColour(100, 200, 100);
            }
            else if (overlays[i]->getType() == OverlayType::SUBTRACTIVE)
            {
                overlapColour = wxColour(200, 100, 100);
            }
            else if (overlays[i]->getType() == OverlayType::LINEAR_INTERPOLATION)
            {
                overlapColour = wxColour(100, 100, 200);
            }
            else if (overlays[i]->getType() == OverlayType::IGNORING)
            {
                overlapColour = wxColour(50, 50, 50);
            }
            else if (overlays[i]->getType() == OverlayType::OVERWRITE)
            {
                overlapColour = wxColour(200, 100, 200);
            }
            else if (overlays[i]->getType() == OverlayType::STATIC_INTERPOLATION)
            {
                overlapColour = wxColour(50, 100, 200);
            }
            else if (overlays[i]->getType() == OverlayType::WEIGHTED_INTERPOLATION)
            {
                overlapColour = wxColour(100, 200, 200);
            }
            // TODO(JK#2#): add colors for other overlay types

            dc->GradientFillLinear(wxRect(start, overlapLength), overlapColour, bgColour, gradientDirection);

            // draw priority of overlay
            wxString roman = wxRichTextDecimalToRoman(overlays[i]->getPriority());
            wxSize prioritySize = dc->GetTextExtent(roman);
            if (prioritySize.x + 3 < overlapLength.x)
            {
                dc->DrawText(roman, start.x + 2, start.y);
            }
        }

        // draw weighting
        std::vector<std::pair<unsigned int, float> > weightPoints = track->getWeightPoints();

        dc->SetPen(wxPen(wxColour(255, 120, 0)));

        if (weightPoints.size() == 0)
        {
            dc->DrawLine(wxPoint(getPositionFromTime(track->getStartTime()), pos.y + 1), wxPoint(getPositionFromTime(track->getEndTime()), pos.y + 1));
        }
        else
        {
            int bottomY = pos.y + _channelHeight - 1;
            int height = _channelHeight - 2;
            // in order to draw the weight points in the middle of the frames instead of at the beginning, add an offset
            uint64_t frameTimeOffset = track->getFrameTime() * 1000000 / 2;

            wxPoint startWeightPoint = wxPoint(pos.x + getLengthFromTime(frameTimeOffset), bottomY - track->getWeight(0) * height);
            wxPoint endWeightPoint = wxPoint(getPositionFromTime(track->getAbsTimeFromFrame(weightPoints[0].first) + frameTimeOffset), bottomY - weightPoints[0].second * height);

            // draw horizontal line from beginning of the track to the center of first frame
            dc->DrawLine(wxPoint(pos.x, startWeightPoint.y), startWeightPoint);
            // draw line from center of first frame to center of frame where the first weight point occurs
            dc->DrawLine(startWeightPoint, endWeightPoint);

            for (size_t i = 1; i < weightPoints.size(); ++i)
            {
                startWeightPoint = wxPoint(getPositionFromTime(track->getAbsTimeFromFrame(weightPoints[i-1].first) + frameTimeOffset), bottomY - weightPoints[i-1].second * height);
                endWeightPoint = wxPoint(getPositionFromTime(track->getAbsTimeFromFrame(weightPoints[i].first) + frameTimeOffset), bottomY - weightPoints[i].second * height);

                dc->DrawLine(startWeightPoint, endWeightPoint);
            }
            startWeightPoint = wxPoint(getPositionFromTime(track->getAbsTimeFromFrame(weightPoints.back().first) + frameTimeOffset), bottomY - weightPoints.back().second * height);
            endWeightPoint = wxPoint(getPositionFromTime(track->getEndTime() - frameTimeOffset), bottomY - track->getWeight(track->getNumFrames() - 1) * height);

            // draw line from center of last frame where a weight point occurs to the center of the very last frame
            dc->DrawLine(startWeightPoint, endWeightPoint);
            // draw horizontal line from center of last frame to the end of the track
            dc->DrawLine(wxPoint(getPositionFromTime(track->getEndTime()), endWeightPoint.y), endWeightPoint);
        }
    }

    dc->SetPen(wxPen(wxColour(0, 0, 0)));

    // draw lines between single frames
    int startFrame = (_timeOffset - getTimeFromPosition(pos)) / (1000000.0f * track->getFrameTime());
    unsigned int endFrame = (getTimeFromPosition(wxPoint(GetSize().x, GetSize().y)) - getTimeFromPosition(pos)) / (1000000.0f * track->getFrameTime()) + 1;
    if (startFrame < 0)
    {
        startFrame = 0;
    }
    if (endFrame > track->getNumFrames())
    {
        endFrame = track->getNumFrames();
    }

    // TODO(JK#3#): how to draw 10* 100* ... frames? bold or large or other color?
    int frameLength = getLengthFromTime(1000000 * track->getFrameTime());
    if (getLengthFromTime(100 * 1000000 * track->getFrameTime()) > 10)
    {
        //for (int i = pos.x + frameLength; k < pos.x + length; k += frameLength)
        for (unsigned int i = 100*(startFrame/100); i <= endFrame; i += 100)
        {
            int framePos = pos.x + getLengthFromTime(i * 1000000.0f * track->getFrameTime());
            dc->DrawLine(wxPoint(framePos, pos.y), wxPoint(framePos, pos.y + 4));
            dc->DrawLine(wxPoint(framePos, pos.y + _channelHeight), wxPoint(framePos, pos.y + _channelHeight - 9));
        }
    }
    if (getLengthFromTime(10 * 1000000 * track->getFrameTime()) > 10)
    {
        //for (int k = pos.x + frameLength; i < pos.x + length; i += frameLength)
        for (unsigned int i = 10*(startFrame/10); i <= endFrame; i += 10)
        {
            int framePos = pos.x + getLengthFromTime(i * 1000000.0f * track->getFrameTime());
            dc->DrawLine(wxPoint(framePos, pos.y), wxPoint(framePos, pos.y + 4));
            dc->DrawLine(wxPoint(framePos, pos.y + _channelHeight), wxPoint(framePos, pos.y + _channelHeight - 6));
        }
    }
    if (frameLength > 10)
    {
        //for (int i = pos.x + frameLength; i < pos.x + length; i += frameLength)
        for (unsigned int i = startFrame; i <= endFrame; ++i)
        {
            int framePos = pos.x + getLengthFromTime(i * 1000000.0f * track->getFrameTime());
            dc->DrawLine(wxPoint(framePos, pos.y), wxPoint(framePos, pos.y + 4));
            dc->DrawLine(wxPoint(framePos, pos.y + _channelHeight), wxPoint(framePos, pos.y + _channelHeight - 3));
        }
    }

    // finally draw again a transparent rect around track (e.g. overlays are drawn above the outer rect)
    dc->SetBrush(*wxTRANSPARENT_BRUSH);
    dc->DrawRectangle(pos, wxSize(length + 1, _channelHeight + 1));
}

void TimelinePanel::drawEditSelectedTrack(wxDC* dc, TimelineTrack* track, wxPoint pos) const
{
    wxSize size(getLengthFromTime(track->getLength()) + 1, _channelHeight*2 + 1);
    dc->SetBrush(wxBrush(wxColour(200, 200, 200)));
    dc->DrawRectangle(pos, size);

    std::vector<std::pair<unsigned int, float> > weightPoints = track->getWeightPoints();

    wxPen blackPen = wxPen(wxColour(0, 0, 0));
    wxPen orangePen = wxPen(wxColour(255, 120, 0));

    if (weightPoints.size() == 0)
    {
        dc->SetPen(orangePen);
        dc->DrawLine(wxPoint(getPositionFromTime(track->getStartTime()), pos.y + 1), wxPoint(getPositionFromTime(track->getEndTime()), pos.y + 1));
    }
    else
    {
        int bottomY = pos.y + size.y - 2;
        int height = _channelHeight * 2 - 2;
        // in order to draw the weight points in the middle of the frames instead of at the beginning, add an offset
        uint64_t frameTimeOffset = track->getFrameTime() * 1000000 / 2;

        wxPoint startWeightPoint = wxPoint(pos.x + getLengthFromTime(frameTimeOffset), bottomY - track->getWeight(0) * height);
        wxPoint endWeightPoint = wxPoint(getPositionFromTime(track->getAbsTimeFromFrame(weightPoints[0].first) + frameTimeOffset), bottomY - weightPoints[0].second * height);

        dc->SetPen(orangePen);
        // draw horizontal line from beginning of the track to the center of first frame
        dc->DrawLine(wxPoint(pos.x, startWeightPoint.y), startWeightPoint);
        // draw line from center of first frame to center of frame where the first weight point occurs
        dc->DrawLine(startWeightPoint, endWeightPoint);

        for (size_t i = 1; i < weightPoints.size(); ++i)
        {
            startWeightPoint = wxPoint(getPositionFromTime(track->getAbsTimeFromFrame(weightPoints[i-1].first) + frameTimeOffset), bottomY - weightPoints[i-1].second * height);
            endWeightPoint = wxPoint(getPositionFromTime(track->getAbsTimeFromFrame(weightPoints[i].first) + frameTimeOffset), bottomY - weightPoints[i].second * height);

            dc->SetPen(orangePen);
            dc->DrawLine(startWeightPoint, endWeightPoint);
            // draw circles to visualize weight points currently set
            dc->SetPen(blackPen);
            dc->DrawCircle(startWeightPoint, 2);
        }
        startWeightPoint = wxPoint(getPositionFromTime(track->getAbsTimeFromFrame(weightPoints.back().first) + frameTimeOffset), bottomY - weightPoints.back().second * height);
        endWeightPoint = wxPoint(getPositionFromTime(track->getEndTime() - frameTimeOffset), bottomY - track->getWeight(track->getNumFrames() - 1) * height);

        dc->SetPen(orangePen);
        // draw line from center of last frame where a weight point occurs to the center of the very last frame
        dc->DrawLine(startWeightPoint, endWeightPoint);
        // draw horizontal line from center of last frame to the end of the track
        dc->DrawLine(wxPoint(getPositionFromTime(track->getEndTime()), endWeightPoint.y), endWeightPoint);

        dc->SetPen(blackPen);
        dc->DrawCircle(startWeightPoint, 2);
    }

    // get and draw all interpolated weight points (only for debugging)
    /*
    int bottomY = pos.y + size.y;
    int height = _channelHeight * 2;
    int frameTimeOffset = track->getFrameTime() * 1000000 / 2;
    for (size_t i = 0; i < track->getNumFrames(); ++i)
    {
        dc->SetPen(blackPen);
        dc->DrawCircle(wxPoint(getPositionFromTime(track->getStartTime() + frameTimeOffset + track->getFrameTime() * 1000000 * i), bottomY - track->getWeight(i) * height), 2);
    }
    */

    dc->SetPen(blackPen);

    // draw lines between single frames
    int startFrame = (_timeOffset - getTimeFromPosition(pos)) / (1000000.0f * track->getFrameTime());
    unsigned int endFrame = (getTimeFromPosition(wxPoint(GetSize().x, GetSize().y)) - getTimeFromPosition(pos)) / (1000000.0f * track->getFrameTime()) + 1;
    if (startFrame < 0)
    {
        startFrame = 0;
    }
    if (endFrame > track->getNumFrames())
    {
        endFrame = track->getNumFrames();
    }

    // TODO(JK#3#): how to draw 10* 100* ... frames? bold or large or other color?
    int frameLength = getLengthFromTime(1000000 * track->getFrameTime());
    if (getLengthFromTime(100 * 1000000 * track->getFrameTime()) > 10)
    {
        //for (int i = pos.x + frameLength; k < pos.x + length; k += frameLength)
        for (unsigned int i = 100*(startFrame/100); i <= endFrame; i += 100)
        {
            int framePos = pos.x + getLengthFromTime(i * 1000000.0f * track->getFrameTime());
            dc->DrawLine(wxPoint(framePos, pos.y), wxPoint(framePos, pos.y + 4));
            dc->DrawLine(wxPoint(framePos, pos.y + _channelHeight * 2), wxPoint(framePos, pos.y + _channelHeight * 2 - 9));
        }
    }
    if (getLengthFromTime(10 * 1000000 * track->getFrameTime()) > 10)
    {
        //for (int k = pos.x + frameLength; i < pos.x + length; i += frameLength)
        for (unsigned int i = 10*(startFrame/10); i <= endFrame; i += 10)
        {
            int framePos = pos.x + getLengthFromTime(i * 1000000.0f * track->getFrameTime());
            dc->DrawLine(wxPoint(framePos, pos.y), wxPoint(framePos, pos.y + 4));
            dc->DrawLine(wxPoint(framePos, pos.y + _channelHeight * 2), wxPoint(framePos, pos.y + _channelHeight * 2 - 6));
        }
    }
    if (frameLength > 10)
    {
        //for (int i = pos.x + frameLength; i < pos.x + length; i += frameLength)
        for (unsigned int i = startFrame; i <= endFrame; ++i)
        {
            int framePos = pos.x + getLengthFromTime(i * 1000000.0f * track->getFrameTime());
            dc->DrawLine(wxPoint(framePos, pos.y), wxPoint(framePos, pos.y + 4));
            dc->DrawLine(wxPoint(framePos, pos.y + _channelHeight * 2), wxPoint(framePos, pos.y + _channelHeight * 2 - 3));
        }
    }
}

void TimelinePanel::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    if (!dc.IsOk())
    {
        return;
    }
    dc.Clear();
    wxPen pen;
    wxBrush brush;

    pen.SetColour(0, 0, 0);
    pen.SetWidth(1);
    dc.SetPen(pen);
    dc.SetBrush(*wxTRANSPARENT_BRUSH);

    wxFont font = dc.GetFont();
    font.SetPointSize(8);
    dc.SetFont(font);

    wxSize size = GetSize();
    wxPoint pos;

    // the number of currently visible channels
    unsigned int numChannels = size.y/_channelHeight + 1;
    // the number of currently visible time units
    int numTimeUnits = size.x/_numPixelsPerTimeUnit + 1;
    uint64_t endTime = getTimeFromPosition(wxPoint(size.x, size.y));

    Timeline* timeline = theAnimationManager.getTimeline();

    // TODO(JK#1#): draw content of timeline


    // first of all draw the content (the tracks) of the timeline. This way there is no special routine needed which
    // draws only the visible parts of the content, as everything else is simply drawn above.
    brush.SetColour(200, 200, 200);
    dc.SetBrush(brush);

    int channelPos = _channelOffset;
    for (unsigned int i = 0; i < numChannels; ++i)
    {
        pos.y = getPositionFromChannel(channelPos);
        // get visible tracks
        std::vector<TimelineTrack*> tracks = timeline->getInRange(channelPos, _timeOffset, endTime);
        for (size_t j = 0; j < tracks.size(); ++j)
        {
            // do not draw tracks being dragged
            if (_dragging && _sequenceToAdd < 0 && _trackToAdd.getNumFrames() == 0 && tracks[j]->getId() == _selectedTrack)
            {
                continue;
            }
            pos.x = getPositionFromTime(tracks[j]->getStartTime());
            drawTrack(&dc, tracks[j], pos);
        }
        ++channelPos;
    }


    // draw dragged tracks

    if (_dragging && _dragIsValid)
    {
        brush.SetColour(200, 100, 0);
        dc.SetBrush(brush);

        channelPos = _draggedTrackChannel - _channelOffset;
        uint64_t length = getLengthFromTime(_draggedTrackLength);

        pos.x = _draggedTrackPos;

        // is a sequence about to be inserted?
        if (_sequenceToAdd >= 0)
        {
            for (size_t i = 0; i < _channelsToAdd.size(); ++i)
            {
                pos.y = _timelineStartY + channelPos * _channelHeight;
                dc.DrawRectangle(pos, wxSize(length + 1, _channelHeight + 1));
                ++channelPos;

                if (i >= numChannels)
                {
                    break;
                }
            }
        }
        else if (_trackToAdd.getNumFrames() > 0)
        {
            pos.y = _timelineStartY + channelPos * _channelHeight;
            dc.DrawRectangle(pos, wxSize(length + 1, _channelHeight + 1));
        }
        else if (_selectedTrack >= 0)
        {
            pos.y = getPositionFromChannel(_draggedTrackChannel);
            drawTrack(&dc, timeline->getTrack(_selectedTrack), pos);
        }
    }

    // draw interpolation stuff
    if (_interpolationPossible)
    {
        brush.SetColour(120, 120, 200);
        dc.SetBrush(brush);
        pos.x = getPositionFromTime(_interpolationStartTime);
        pos.y = getPositionFromChannel(_interpolationChannel);
        dc.DrawRectangle(pos, wxSize(getLengthFromTime(_interpolationEndTime - _interpolationStartTime) + 1, _channelHeight + 1));
    }


    // draw horizontal lines separating the channels
    for (unsigned int i = 0; i < numChannels; ++i)
    {
        dc.DrawLine(wxPoint(0, _timelineStartY + i*_channelHeight), wxPoint(size.x, _timelineStartY + i*_channelHeight));
    }

    // draw edit selected track
    if (_editSelectedTrack)
    {
        TimelineTrack* track = theAnimationManager.getTimeline()->getTrack(_selectedTrack);
        if (track != nullptr)
        {
            wxPoint pos(getPositionFromTime(track->getStartTime()), getPositionFromChannel(track->getChannel()));
            drawEditSelectedTrack(&dc, track, pos);
        }
    }


    // draw set skeleton area
    brush.SetColour(160, 160, 160);
    dc.SetBrush(brush);
    dc.DrawRectangle(wxPoint(0, 0), wxSize(_channelOptionsWidth + 1, _timelineStartY + 1));
    dc.GradientFillLinear(wxRect(wxPoint(1, 0), wxSize(_channelOptionsWidth - 1, 7)), wxColour(230, 230, 230), wxColour(160, 160, 160), wxUP);
    dc.GradientFillLinear(wxRect(wxPoint(1, 7), wxSize(_channelOptionsWidth - 1, 20)), wxColour(230, 230, 230), wxColour(160, 160, 160), wxDOWN);

    dc.DrawText(_("Set Skeleton"), wxPoint(20, 5));
    dc.DrawLine(wxPoint(20, 20), wxPoint(20 + dc.GetTextExtent(_("Set Skeleton")).x, 20));

    wxString skeletonName = theAnimationManager.getTimelineSkeleton()->getName();
    // trim the string to fit into
    while (dc.GetTextExtent(skeletonName).x > _channelOptionsWidth - 10)
    {
        skeletonName.RemoveLast();
    }
    dc.DrawText(skeletonName, wxPoint(5, 25));


    // draw option panel on top of the timeline
    brush.SetColour(200, 200, 200);
    dc.SetBrush(brush);
    dc.DrawRectangle(wxPoint(_channelOptionsWidth, 0), wxSize(size.x, _optionsHeight + 1));

    font.SetPointSize(8);
    dc.SetFont(font);

    // draw time bar
    brush.SetColour(220, 220, 220);
    dc.SetBrush(brush);
    dc.DrawRectangle(wxPoint(_channelOptionsWidth, _optionsHeight), wxSize(size.x, _timeHeight + 1));

    dc.SetBrush(*wxTRANSPARENT_BRUSH);

    for (int i = 0; i < numTimeUnits; ++i)
    {
        uint64_t time = i * _ysPerTimeUnit + _timeOffset;
        int min = time / (60 * 1000000);
        int sec = (time / 1000000) % 60;
        int ms = (time / 1000) % 1000;

        wxString timeLabel;
        timeLabel << (min < 10 ? _("0") : _("")) << min << (sec < 10 ? _(":0") : _(":")) << sec;
        timeLabel << (ms < 100 ? (ms < 10 ? _(":00") : _(":0")) : _(":")) << ms;

        int timeUnitPos = _timelineStartX + i * _numPixelsPerTimeUnit;
        dc.DrawText(timeLabel, wxPoint(timeUnitPos + 2, _timelineStartY - 15));
        // main time units (long line)
        dc.DrawLine(wxPoint(timeUnitPos, _timelineStartY - 12), wxPoint(_timelineStartX + i*100, _timelineStartY));
        // sub time units at every half time unit (medium line)
        dc.DrawLine(wxPoint(timeUnitPos + 50, _timelineStartY - 5), wxPoint(timeUnitPos + 50, _timelineStartY));
        // sub time units at every 1/10 time units (short lines)
        for (int j = 0; j < 10; ++j)
        {
            dc.DrawLine(wxPoint(timeUnitPos + j*10, _timelineStartY - 2), wxPoint(timeUnitPos + j*10, _timelineStartY));
        }
    }

    // draw channel options (channel name etc)
    for (unsigned int i = 0; i < numChannels; ++i)
    {
        dc.GradientFillLinear(wxRect(0, _timelineStartY + i*_channelHeight, _channelOptionsWidth, _channelHeight), wxColour(50, 50, 50), wxColour(200, 200, 200), wxUP);
        dc.DrawLine(wxPoint(0, _timelineStartY + i*_channelHeight), wxPoint(_timelineStartX, _timelineStartY + i*_channelHeight));

        // draw arrows above and below channelName (to swap channels)
        pen.SetWidth(2);
        dc.SetPen(pen);

        // upper arrows
        dc.DrawLine(wxPoint(_channelOptionsWidth - 12, _timelineStartY + i*_channelHeight + 8), wxPoint(_channelOptionsWidth - 8, _timelineStartY + i*_channelHeight + 4));
        dc.DrawLine(wxPoint(_channelOptionsWidth - 4, _timelineStartY + i*_channelHeight + 8), wxPoint(_channelOptionsWidth - 8, _timelineStartY + i*_channelHeight + 4));

        // lower arrows
        dc.DrawLine(wxPoint(_channelOptionsWidth - 12, _timelineStartY + (i+1)*_channelHeight - 8), wxPoint(_channelOptionsWidth - 8, _timelineStartY + (i+1)*_channelHeight - 4));
        dc.DrawLine(wxPoint(_channelOptionsWidth - 4, _timelineStartY + (i+1)*_channelHeight - 8), wxPoint(_channelOptionsWidth - 8, _timelineStartY + (i+1)*_channelHeight - 4));

        pen.SetWidth(1);
        dc.SetPen(pen);

        // draw the channel name (i.e. its number)
        wxString channelName;
        channelName << _channelOffset + i + 1;
        font.SetPointSize(14);
        dc.SetFont(font);

        wxSize textExtent = dc.GetTextExtent(channelName);

        dc.DrawText(channelName, wxPoint(_channelOptionsWidth - textExtent.x - 2, _timelineStartY + i*_channelHeight + 6));

        font.SetPointSize(8);
        dc.SetFont(font);

        // draw the associated bone name
        wxString boneName = theAnimationManager.getTimeline()->getChannelAffiliationName(_channelOffset + i);
        if (boneName.size() == 0)
        {
            boneName << _("Bone ") << theAnimationManager.getTimeline()->getChannelAffiliation(_channelOffset + i);
        }
        // trim the string to fit into
        while (dc.GetTextExtent(boneName).x + 8 > _channelOptionsWidth - textExtent.x)
        {
            boneName.RemoveLast();
        }
        dc.DrawText(boneName, wxPoint(4, _timelineStartY + i*_channelHeight + 5));

        // draw a plus sign for inserting a new channel
        font.SetPointSize(10);
        dc.SetFont(font);

        wxPoint channelButtonStart(0, _timelineStartY + (i+1)*_channelHeight - _channelButtonsSize.y);

        pen.SetColour(0, 0, 0);
        dc.SetPen(pen);

        brush.SetColour(90, 150, 40);
        dc.SetBrush(brush);
        dc.DrawRectangle(channelButtonStart, _channelButtonsSize);
        dc.DrawLine(wxPoint(channelButtonStart.x + _channelButtonsSize.x/2, channelButtonStart.y + 3),
                    wxPoint(channelButtonStart.x + _channelButtonsSize.x/2, channelButtonStart.y + _channelButtonsSize.y - 3));
        dc.DrawLine(wxPoint(channelButtonStart.x + 3, channelButtonStart.y + _channelButtonsSize.y/2),
                    wxPoint(channelButtonStart.x + _channelButtonsSize.x - 3, channelButtonStart.y + _channelButtonsSize.y/2));

        dc.SetBrush(*wxTRANSPARENT_BRUSH);
    }
    // draw line between channel options and timeline
    dc.DrawLine(wxPoint(_channelOptionsWidth, 0), wxPoint(_channelOptionsWidth, size.y));


    // draw the time bar cursor
    int cursorPosition = getPositionFromTime(_cursorPosition);
    if (cursorPosition >= _timelineStartX)
    {
        pen.SetWidth(1);
        pen.SetColour(255, 10, 0);
        brush.SetColour(255, 10, 0);
        dc.SetPen(pen);
        dc.SetBrush(brush);

        dc.DrawLine(wxPoint(cursorPosition, _optionsHeight + 5), wxPoint(cursorPosition, size.y));
        // dc.DrawLine(wxPoint(_channelOptionsWidth + 275, _optionsHeight + 5), wxPoint(_channelOptionsWidth + 275, _optionsHeight + _timeHeight));


        wxPoint points[4] = {wxPoint(cursorPosition - 5, _timelineStartY - 12),
                             wxPoint(cursorPosition, _timelineStartY),
                             wxPoint(cursorPosition + 5, _timelineStartY - 12),
                             wxPoint(cursorPosition - 5, _timelineStartY - 12)};
        dc.DrawPolygon(4, points);
    }

    // draw rectangle around whole control
    pen.SetWidth(1);
    pen.SetColour(0, 0, 0);
    dc.SetPen(pen);
    dc.SetBrush(*wxTRANSPARENT_BRUSH);

    dc.DrawRectangle(wxPoint(0, 0), size);
}



void TimelinePanel::OnLeftDown(wxMouseEvent& event)
{
    wxPoint pos = event.GetPosition();

    if (pos.x > _timelineStartX && pos.y > _optionsHeight && !HasCapture())
    {
        CaptureMouse();
    }

    // reset all variables
    _clickedChannel = -1;
    _clickedTime = false;
    _dragging = false;

    if (pos.x < _timelineStartX)
    {
        _editSelectedTrack = false;
        if (pos.y < _timelineStartY)
        {
            // select a skeleton
            showPopUpSkeletons();
        }
        else
        {
            _clickedChannel = getChannelFromPosition(pos);
            int clickedChannelOffset = (pos.y - _timelineStartY) % _channelHeight;
            if (pos.x < _channelOptionsWidth - 18)
            {
                if (clickedChannelOffset < _channelHeight * 0.5f)
                {
                    showPopUpBones();
                }
                else if (clickedChannelOffset > _channelHeight - _channelButtonsSize.y && pos.x < _channelButtonsSize.x)
                {
                    theAnimationManager.getTimeline()->insertChannelAfter(_clickedChannel);
                }
            }
            else
            {
                if (clickedChannelOffset < _channelHeight * 0.33f)
                {
                    theAnimationManager.getTimeline()->swapChannels(_clickedChannel, _clickedChannel - 1);
                }
                else if (clickedChannelOffset > _channelHeight * 0.66f)
                {
                    theAnimationManager.getTimeline()->swapChannels(_clickedChannel, _clickedChannel + 1);
                }
            }
        }
    }
    else
    {
        if (pos.y < _optionsHeight)
        {
        }
        else if (pos.y < _timelineStartY)
        {
            setCursorPosition(getTimeFromPosition(pos));
            _clickedTime = true;
        }
        else
        {
            _clickedChannel = getChannelFromPosition(pos);

            // edit selected track, set weights etc
            if (_editSelectedTrack && _selectedTrack >= 0)
            {
                TimelineTrack* track = theAnimationManager.getTimeline()->getTrack(_selectedTrack);
                if (track == nullptr)
                {
                    _editSelectedTrack = false;
                    _selectedTrack = -1;
                }
                else if ((track->getChannel() == _clickedChannel || track->getChannel() == _clickedChannel - 1) && track->isInside(getTimeFromPosition(pos)))
                {
                    int clickedChannelOffset = (pos.y - _timelineStartY + (track->getChannel() - _channelOffset)%2 * _channelHeight) % (2 * _channelHeight);
                    float weight = float(2 * _channelHeight - clickedChannelOffset) / (2 * _channelHeight);
                    track->setWeightPoint(track->getFrameNumFromAbsTime(getTimeFromPosition(pos)), weight);
                    _oldWeightPointFrame = track->getFrameNumFromAbsTime(getTimeFromPosition(pos));
                }
                else
                {
                    _editSelectedTrack = false;
                    _selectedTrack = -1;
                }
            }
            // get the track at click position
            TimelineTrack* track = theAnimationManager.getTimeline()->getTrack(_clickedChannel, getTimeFromPosition(pos));
            if (!_editSelectedTrack && track != nullptr)
            {
                _selectedTrack = track->getId();
                _mouseToTrackOffset = pos.x - getPositionFromTime(track->getStartTime());
                _draggedTrackLength = track->getLength();
                _draggedTrackPos = pos.x - _mouseToTrackOffset;
                _draggedTrackChannel = getChannelFromPosition(pos);
                _dragging = true;

                if (getPositionFromTime(track->getEndTime()) - pos.x < 3 && getLengthFromTime(track->getLength()) >= 10)
                {
                    _resizeTrack = true;
                    _dragging = false;
                    // wxSetCursor(wxCURSOR_SIZEWE);
                }
            }
            else if (!_editSelectedTrack)
            {
                _selectedTrack = -1;
            }
        }
    }
    update();
}

void TimelinePanel::OnLeftUp(wxMouseEvent& event)
{
    if (HasCapture())
    {
        ReleaseMouse();
    }
    _clickedTime = false;
    _clickedChannel = -1;

    _resizeTrack = false;

    endDragDrop();
    update();
}

void TimelinePanel::OnRightDown(wxMouseEvent& event)
{
    wxPoint pos = event.GetPosition();

    if (pos.x > _timelineStartX && pos.y > _timelineStartY)
    {
        if (_interpolationPossible)
        {
            uint64_t time = getTimeFromPosition(pos);
            wxSetCursor(wxCURSOR_ARROWWAIT);
            theAnimationManager.getTimeline()->addInterpolation(_interpolationChannel, time, 0.005f);

            wxSetCursor(wxCURSOR_ARROW);
            // TODO(JK#5#): maybe move the interpolation stuff in own function
            /*
            _interpolationStartTime = theAnimationManager.getTimeline()->getTrackBefore(_interpolationChannel, time)->getEndTime();
            _interpolationEndTime = theAnimationManager.getTimeline()->getTrackAfter(_interpolationChannel, time)->getStartTime();
            uint64_t length = _interpolationEndTime - _interpolationStartTime;
            // we want a frame time of ~ 5 ms, so divide delta time given in us by 5*1000 us to get the num of frames fitting into the window
            uint64_t numFrames = length / 5000;
            if (numFrames == 0)
            {
                numFrames = 1;
            }
            // now find the exact frame time, which must be given in a fraction of seconds
            float frameTime = double(length) / double(numFrames * 1000 * 1000);
            Quaternion q = theAnimationManager.getTimeline()->getTrackBefore(_interpolationChannel, time)->getLastFrame().getOrientation();
            Quaternion p = theAnimationManager.getTimeline()->getTrackAfter(_interpolationChannel, time)->getFirstFrame().getOrientation();
            TimelineTrack newTrack;
            newTrack.setFrameTime(frameTime);
            float step = 1.0f / float(numFrames + 1);
            wxSetCursor(wxCURSOR_ARROWWAIT);
            for (unsigned int i = 1; i < numFrames + 1; ++i)
            {
                newTrack.appendFrame(MotionSequenceFrame(q.slerp(p, i*step)));
            }
            theAnimationManager.getTimeline()->insert(newTrack, _interpolationChannel, _interpolationStartTime);
            wxSetCursor(wxCURSOR_ARROW);
            */
            SetToolTip(_(""));
            _interpolationPossible = false;
        }
        else
        {
            uint64_t time = getTimeFromPosition(pos);
            int channel = getChannelFromPosition(pos);
            TimelineTrack* track = theAnimationManager.getTimeline()->getTrack(channel, time);
            if (track == nullptr)
            {
                _selectedTrack = -1;
                _editSelectedTrack = false;
            }
            else
            {
                std::vector<TimelineOverlay*> overlays = theAnimationManager.getTimeline()->getOverlays(track, time);
                TimelineOverlay* clickedOverlay = nullptr;

                if (overlays.size() > 0)
                {
                    std::sort(overlays.begin(), overlays.end(), TimelineOverlay::compChannelDistSmall);
                    int channelClickOffset = (pos.y - _timelineStartY) % _channelHeight;

                    if (channelClickOffset < _channelHeight/3)
                    {
                        // find the first overlay with smaller channel on the first track
                        for (size_t i = 0; i < overlays.size(); ++i)
                        {
                            if (overlays[i]->getFirstTrack()->getChannel() < track->getChannel())
                            {
                                clickedOverlay = overlays[i];
                                break;
                            }
                        }
                    }
                    else if (channelClickOffset > (_channelHeight * 2)/3)
                    {
                        // find the first overlay with greater channel on the second track
                        for (size_t i = 0; i < overlays.size(); ++i)
                        {
                            if (overlays[i]->getSecondTrack()->getChannel() > track->getChannel())
                            {
                                clickedOverlay = overlays[i];
                                break;
                            }
                        }
                    }
                }

                if (clickedOverlay != nullptr)
                {
                    TimelineOverlayDialog* dialog = new TimelineOverlayDialog(this);
                    dialog->setOverlay(clickedOverlay);
                    if (dialog->ShowModal() == wxID_OK)
                    {
                        clickedOverlay->setPriority(dialog->getPriority());
                        clickedOverlay->setType(dialog->getType());
                    }
                    dialog->Destroy();
                }
                else
                {
                    _selectedTrack = track->getId();
                    _editSelectedTrack = true;
                }
            }
        }
    }
    update();
}

void TimelinePanel::OnRightUp(wxMouseEvent& event)
{
    update();
}

void TimelinePanel::OnMouseMove(wxMouseEvent& event)
{
    wxPoint pos = event.GetPosition();
    wxSize size = GetSize();

    if (_dragging)
    {
        if (pos.x < _timelineStartX || pos.x > size.x || pos.y < _timelineStartY || pos.y > size.y)
        {
            _dragIsValid = false;
        }
        else
        {
            _dragIsValid = true;
            _draggedTrackPos = pos.x - _mouseToTrackOffset;
            // if the position would end up at a time point smaller than 0, set it to the start of the timeline
            if (getTimeFromPosition(wxPoint(_draggedTrackPos, 0)) <= 0)
            {
                _draggedTrackPos = getPositionFromTime(0);
            }
            // check if the track is beyond the maximum time and clamp it to the max time
            else if (int64_t(_draggedTrackLength) + getTimeFromPosition(wxPoint(_draggedTrackPos, 0)) > _maxTime)
            {
                _draggedTrackPos = getPositionFromTime(_maxTime - _draggedTrackLength);
            }
            else if (abs(getPositionFromTime(_cursorPosition) - _draggedTrackPos) < 3)
            {
                // clamp dragged track to the time cursor if nearby
                _draggedTrackPos = getPositionFromTime(_cursorPosition);
            }
            // TODO(JK#1#): add a routine to clamp a track to other tracks and to don't allow them to overlap
            _draggedTrackChannel = getChannelFromPosition(pos);
        }
        Refresh();
    }
    else if (event.LeftIsDown() && _editSelectedTrack && _selectedTrack >= 0 && _clickedChannel >= 0)
    {
        TimelineTrack* track = theAnimationManager.getTimeline()->getTrack(_selectedTrack);
        if (track != nullptr)
        {
            unsigned int frame = track->getFrameNumFromAbsTime(getTimeFromPosition(pos));
            if (pos.x < _timelineStartX)
            {
                frame = 0;
            }
            if (frame >= track->getNumFrames())
            {
                frame = track->getNumFrames() - 1;
            }
            float weight;
            if (getChannelFromPosition(pos) < track->getChannel() || pos.y <= _timelineStartY)
            {
                weight = 1.0f;
            }
            else if (getChannelFromPosition(pos) > track->getChannel() + 1)
            {
                weight = 0.0f;
            }
            else
            {
                int channelOffset = (pos.y - _timelineStartY + (track->getChannel() - _channelOffset)%2 * _channelHeight) % (2 * _channelHeight);
                weight = float(2 * _channelHeight - channelOffset) / (2 * _channelHeight);
            }
            track->moveWeightPoint(_oldWeightPointFrame, frame, weight);
            _oldWeightPointFrame = frame;
            Refresh();
        }
    }
    else if (!_editSelectedTrack)
    {
        if (pos.x > _timelineStartX && pos.y > _timelineStartY)
        {
            // TODO(JK#1#): rename _interpolationChannel to _currentChannel, also make create something like _currentMouseTime, _curreentMouseChannel etc
            _interpolationChannel = getChannelFromPosition(pos);
            uint64_t time = getTimeFromPosition(pos);
            if (theAnimationManager.getTimeline()->isBetweenTwoTracks(_interpolationChannel, time))
            {
                SetToolTip(_("Right click for interpolation"));
                _interpolationPossible = true;
                _interpolationStartTime = theAnimationManager.getTimeline()->getTrackBefore(_interpolationChannel, time)->getEndTime();
                _interpolationEndTime = theAnimationManager.getTimeline()->getTrackAfter(_interpolationChannel, time)->getStartTime();
                Refresh();
            }
            // check if _interpolationPossible was set to avoid unnecessary refreshes
            else if (_interpolationPossible)
            {
                SetToolTip(_(""));
                _interpolationPossible = false;
                Refresh();
            }
            TimelineTrack* track = theAnimationManager.getTimeline()->getTrack(_interpolationChannel, time);
            if (track != nullptr)
            {
                if (getPositionFromTime(track->getEndTime()) - pos.x < 3 && getLengthFromTime(track->getLength()) >= 10)
                {
                    wxSetCursor(wxCURSOR_SIZEWE);
                }
                else
                {
                    wxSetCursor(wxCURSOR_ARROW);
                }
            }
            else
            {
                wxSetCursor(wxCURSOR_ARROW);
            }
        }
        // check if _interpolationPossible was set to avoid unnecessary refreshes
        else if (_interpolationPossible)
        {
            SetToolTip(_(""));
            _interpolationPossible = false;
            Refresh();
        }
    }

    // resize track (adjust frame time)
    if (_resizeTrack && _selectedTrack >= 0)
    {
        TimelineTrack* track = theAnimationManager.getTimeline()->getTrack(_selectedTrack);

        int64_t time = getTimeFromPosition(pos) - track->getStartTime();
        if (time < 1000)
        {
            time = 1000;
        }
        float newFrameTime = double(time) / double(track->getNumFrames() * 1000000);
        // track->setFrameTime(newFrameTime);
        theAnimationManager.getTimeline()->changeFrameTime(_selectedTrack, newFrameTime);
        Refresh();
    }


    if (_clickedChannel >= 0)
    {
        // TODO(JK#1#): use _clickedChannel for swapping channels
        // Refresh();
    }
    else if (_clickedTime)
    {
        if (pos.x < _timelineStartX)
        {
            setCursorPosition(_timeOffset);
        }
        else
        {
            setCursorPosition(getTimeFromPosition(pos));
        }
    }
}

void TimelinePanel::OnMouseWheel(wxMouseEvent& event)
{
    wxPoint pos = event.GetPosition();
    if (pos.y < 0)
    {
        return;
    }
    if (pos.x < _channelOptionsWidth)
    {
        _channelOffset -= event.GetWheelRotation()/event.GetWheelDelta();
        if (_channelOffset <= 0)
        {
            _channelOffset = 0;
        }
    }
    else
    {
        _timeOffset -= _ysPerTimeUnit * (event.GetWheelRotation()/event.GetWheelDelta());
        if (_timeOffset <= 0)
        {
            _timeOffset = 0;
        }
        else if (_timeOffset + _ysPerTimeUnit * (GetSize().x - _timelineStartX) / _numPixelsPerTimeUnit > _maxTime)
        {
            // if out of time boundary, undo
            _timeOffset += _ysPerTimeUnit * (event.GetWheelRotation()/event.GetWheelDelta());
        }
    }
    Refresh();
}

void TimelinePanel::OnMouseEnter(wxMouseEvent& event)
{
    SetFocus();
    Refresh();
}

void TimelinePanel::OnMouseLeave(wxMouseEvent& event)
{
    Refresh();
}

void TimelinePanel::OnKillFocus(wxFocusEvent& event)
{
    _clickedTime = false;
    _clickedChannel = -1;
}

void TimelinePanel::OnButtonCutClick(wxCommandEvent& event)
{
    if (_selectedTrack < 0)
    {
        theAnimationManager.getTimeline()->cut(_cursorPosition);
    }
    else
    {
        theAnimationManager.getTimeline()->cut(_selectedTrack, _cursorPosition);
    }
    Refresh();
}

void TimelinePanel::OnButtonZoomInClick(wxCommandEvent& event)
{
    _ysPerTimeUnit /= 1000;
    switch (_ysPerTimeUnit)
    {
        case 20:
            _ysPerTimeUnit = 10;
            break;
        case 50:
            _ysPerTimeUnit = 20;
            break;
        case 100:
            _ysPerTimeUnit = 50;
            break;
        case 200:
            _ysPerTimeUnit = 100;
            break;
        case 500:
            _ysPerTimeUnit = 200;
            break;
        case 1000:
            _ysPerTimeUnit = 500;
            break;
        case 2000:
            _ysPerTimeUnit = 1000;
            break;
        case 5000:
            _ysPerTimeUnit = 2000;
            break;
        case 10000:
            _ysPerTimeUnit = 5000;
            break;
        case 20000:
            _ysPerTimeUnit = 10000;
            break;
        case 30000:
            _ysPerTimeUnit = 20000;
            break;
        case 60000:
            _ysPerTimeUnit = 30000;
            break;
        case 120000:
            _ysPerTimeUnit = 60000;
            break;
        case 300000:
            _ysPerTimeUnit = 120000;
            break;
        case 600000:
            _ysPerTimeUnit = 300000;
            break;
        default:
            break;
    }
    _ysPerTimeUnit *= 1000;
    Refresh();
}

void TimelinePanel::OnButtonZoomOutClick(wxCommandEvent& event)
{
    _ysPerTimeUnit /= 1000;
    switch (_ysPerTimeUnit)
    {
        case 10:
            _ysPerTimeUnit = 20;
            _timeOffset /= 20 * 1000;
            _timeOffset *= 20 * 1000;
            break;
        case 20:
            _ysPerTimeUnit = 50;
            _timeOffset /= 50 * 1000;
            _timeOffset *= 50 * 1000;
            break;
        case 50:
            _ysPerTimeUnit = 100;
            _timeOffset /= 100 * 1000;
            _timeOffset *= 100 * 1000;
            break;
        case 100:
            _ysPerTimeUnit = 200;
            _timeOffset /= 200 * 1000;
            _timeOffset *= 200 * 1000;
            break;
        case 200:
            _ysPerTimeUnit = 500;
            _timeOffset /= 500 * 1000;
            _timeOffset *= 500 * 1000;
            break;
        case 500:
            _ysPerTimeUnit = 1000;
            _timeOffset /= 1000 * 1000;
            _timeOffset *= 1000 * 1000;
            break;
        case 1000:
            _ysPerTimeUnit = 2000;
            _timeOffset /= 2000 * 1000;
            _timeOffset *= 2000 * 1000;
            break;
        case 2000:
            _ysPerTimeUnit = 5000;
            _timeOffset /= 5000 * 1000;
            _timeOffset *= 5000 * 1000;
            break;
        case 5000:
            _ysPerTimeUnit = 10000;
            _timeOffset /= 10000 * 1000;
            _timeOffset *= 10000 * 1000;
            break;
        case 10000:
            _ysPerTimeUnit = 20000;
            _timeOffset /= 20000 * 1000;
            _timeOffset *= 20000 * 1000;
            break;
        case 20000:
            _ysPerTimeUnit = 30000;
            _timeOffset /= 30000 * 1000;
            _timeOffset *= 30000 * 1000;
            break;
        case 30000:
            _ysPerTimeUnit = 60000;
            _timeOffset /= 60000 * 1000;
            _timeOffset *= 60000 * 1000;
            break;
        case 60000:
            _ysPerTimeUnit = 120000;
            _timeOffset /= 120000 * 1000;
            _timeOffset *= 120000 * 1000;
            break;
        case 120000:
            _ysPerTimeUnit = 300000;
            _timeOffset /= 300000 * 1000;
            _timeOffset *= 300000 * 1000;
            break;
        case 300000:
            _ysPerTimeUnit = 600000;
            _timeOffset /= 600000 * 1000;
            _timeOffset *= 600000 * 1000;
            break;
        default:
            break;
    }
    _ysPerTimeUnit *= 1000;
    // adjust time offset to time boundary
    while (_timeOffset + _ysPerTimeUnit * (GetSize().x - _timelineStartX) / _numPixelsPerTimeUnit >= _maxTime)
    {
        _timeOffset -= _ysPerTimeUnit;
    }
    Refresh();
}





