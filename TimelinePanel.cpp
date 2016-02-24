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
#include <wx/dcbuffer.h>

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
	Connect(wxEVT_MOTION,(wxObjectEventFunction)&TimelinePanel::OnMouseMove);
	Connect(wxEVT_ENTER_WINDOW,(wxObjectEventFunction)&TimelinePanel::OnMouseEnter);
	Connect(wxEVT_LEAVE_WINDOW,(wxObjectEventFunction)&TimelinePanel::OnMouseLeave);
	Connect(wxEVT_MOUSEWHEEL,(wxObjectEventFunction)&TimelinePanel::OnMouseWheel);
	//*)
	Connect(wxEVT_MOUSE_CAPTURE_LOST, (wxObjectEventFunction)&TimelinePanel::OnMouseCaptureLost);

	_msPerTimeUnit = 1000;
	_timeOffset = 0;
	_channelOffset = 0;
	_cursorPosition = 0;
	_clickedTime = false;
	_clickedChannel = -1;
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
    _draggedTrackLength = theAnimationManager.getProjectSequence(sequence)->getLength();
    _mouseToTrackOffset = 30;
    _sequenceToAdd = sequence;
    _channelsToAdd = channels;
}

void TimelinePanel::prepareAddingFrame(const MotionSequenceFrame &frame)
{
    _dragging = true;
    _dragIsValid = false;
    _draggedTrackLength = 25;
    _mouseToTrackOffset = 2;
    _trackToAdd.clear();
    _trackToAdd.appendFrame(frame);
    _trackToAdd.setFrameTime(0.025);
    _trackToAdd.setName("Custom Frame");
    //_frameToAdd = frame;
}

void TimelinePanel::setCursorPosition(int time)
{
    if (time < 0)
    {
        time = 0;
    }
    _cursorPosition = time;
    theAnimationManager.getTimeline()->setSkeletonToTime(time);
    update();
}

void TimelinePanel::update()
{
    Refresh();
    Update();
    if (_glCanvas != nullptr)
    {
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
}

void TimelinePanel::OnPopupClick(wxCommandEvent &event)
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

void TimelinePanel::showPopUp()
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
 	menu.Connect(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&TimelinePanel::OnPopupClick, NULL, this);
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
        int time = getTimeFromPosition(wxPoint(_draggedTrackPos, 0));
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

int TimelinePanel::getLengthFromTime(int time) const
{
    return (_numPixelsPerTimeUnit * time) / _msPerTimeUnit;
}

int TimelinePanel::getLengthFromTime(float time) const
{
    return (_numPixelsPerTimeUnit * time) / _msPerTimeUnit;
}

int TimelinePanel::getPositionFromTime(int time) const
{
    return _timelineStartX + (_numPixelsPerTimeUnit * (time - _timeOffset)) / _msPerTimeUnit;
}

int TimelinePanel::getTimeFromPosition(wxPoint pos) const
{
    return (_msPerTimeUnit * (pos.x - _timelineStartX)) / _numPixelsPerTimeUnit + _timeOffset;
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
    // draw track background and rect around it
    if (track->getId() == _selectedTrack)
    {
        dc->SetBrush(wxBrush(wxColor(200, 200, 100)));
    }
    else
    {
        dc->SetBrush(wxBrush(wxColor(200, 200, 200)));
    }
    int length = getLengthFromTime(track->getLength());
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

    // draw lines between single frames
    int startFrame = (_timeOffset - getTimeFromPosition(pos)) / (1000.0f * track->getFrameTime());
    unsigned int endFrame = (getTimeFromPosition(wxPoint(GetSize().x, GetSize().y)) - getTimeFromPosition(pos)) / (1000.0f * track->getFrameTime()) + 1;
    if (startFrame < 0)
    {
        startFrame = 0;
    }
    if (endFrame > track->getNumFrames())
    {
        endFrame = track->getNumFrames();
    }

    // TODO(JK#3#): how to draw 10* 100* ... frames? bold or large or other color?
    int frameLength = getLengthFromTime(1000 * track->getFrameTime());
    if (getLengthFromTime(100 * 1000 * track->getFrameTime()) > 10)
    {
        //for (int i = pos.x + frameLength; k < pos.x + length; k += frameLength)
        for (unsigned int i = 100*(startFrame/100); i <= endFrame; i += 100)
        {
            int framePos = pos.x + getLengthFromTime(i * 1000.0f * track->getFrameTime());
            dc->DrawLine(wxPoint(framePos, pos.y), wxPoint(framePos, pos.y + 4));
            dc->DrawLine(wxPoint(framePos, pos.y + _channelHeight), wxPoint(framePos, pos.y + _channelHeight - 9));
        }
    }
    if (getLengthFromTime(10 * 1000 * track->getFrameTime()) > 10)
    {
        //for (int k = pos.x + frameLength; i < pos.x + length; i += frameLength)
        for (unsigned int i = 10*(startFrame/10); i <= endFrame; i += 10)
        {
            int framePos = pos.x + getLengthFromTime(i * 1000.0f * track->getFrameTime());
            dc->DrawLine(wxPoint(framePos, pos.y), wxPoint(framePos, pos.y + 4));
            dc->DrawLine(wxPoint(framePos, pos.y + _channelHeight), wxPoint(framePos, pos.y + _channelHeight - 6));
        }
    }
    if (frameLength > 10)
    {
        //for (int i = pos.x + frameLength; i < pos.x + length; i += frameLength)
        for (unsigned int i = startFrame; i <= endFrame; ++i)
        {
            int framePos = pos.x + getLengthFromTime(i * 1000.0f * track->getFrameTime());
            dc->DrawLine(wxPoint(framePos, pos.y), wxPoint(framePos, pos.y + 4));
            dc->DrawLine(wxPoint(framePos, pos.y + _channelHeight), wxPoint(framePos, pos.y + _channelHeight - 3));
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
    int endTime = getTimeFromPosition(wxPoint(size.x, size.y));

    Timeline* timeline = theAnimationManager.getTimeline();

    // TODO(JK#1#): draw content of timeline


    // first of all draw the content of the timeline. This way there is no special routine needed which
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
        int length = getLengthFromTime(_draggedTrackLength);

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
        brush.SetColour(110, 110, 210);
        dc.SetBrush(brush);
        pos.x = getPositionFromTime(_interpolationStartTime);
        pos.y = getPositionFromChannel(_interpolationChannel);
        dc.DrawRectangle(pos, wxSize(getLengthFromTime(_interpolationEndTime - _interpolationStartTime) + 1, _channelHeight + 1));
        if (getLengthFromTime(_interpolationEndTime - _interpolationStartTime) > dc.GetTextExtent(_("Right click for")).x + 10)
        {
            pos += wxPoint(5, 5);
            dc.DrawText(_("Right click for"), pos);
            pos += wxPoint(0, 15);
            dc.DrawText(_("interpolation"), pos);
        }
    }


    // draw sorting area
    brush.SetColour(200, 200, 200);
    dc.SetBrush(brush);
    dc.DrawRectangle(wxPoint(0, 0), wxSize(_channelOptionsWidth + 1, _timelineStartY + 1));
    dc.DrawText(_("Sort by"), wxPoint(5, 5));

    // draw option panel above the timeline
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
        int time = i * _msPerTimeUnit + _timeOffset;
        int min = time / (60 * 1000);
        int sec = (time / 1000) % 60;
        int ms = time % 1000;

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
        dc.DrawLine(wxPoint(0, _timelineStartY + i*_channelHeight), wxPoint(size.x, _timelineStartY + i*_channelHeight));

        // TODO(JK#1#): draw arrows above and below channelName (to swap channels)

        wxString channelName;
        channelName << _channelOffset + i + 1;
        font.SetPointSize(14);
        dc.SetFont(font);
        dc.DrawText(channelName, wxPoint(_channelOptionsWidth - dc.GetTextExtent(channelName).x - 2, _timelineStartY + i*_channelHeight + 6));

        int channelNameExtent = dc.GetTextExtent(channelName).x;

        font.SetPointSize(8);
        dc.SetFont(font);

        wxString boneName = theAnimationManager.getTimeline()->getChannelAffiliationName(_channelOffset + i);
        if (boneName.size() == 0)
        {
            boneName << _("Bone ") << theAnimationManager.getTimeline()->getChannelAffiliation(_channelOffset + i);
        }
        // trim the string to fit into
        while (dc.GetTextExtent(boneName).x + 8 > _channelOptionsWidth - channelNameExtent)
        {
            boneName.RemoveLast();
        }
        dc.DrawText(boneName, wxPoint(4, _timelineStartY + i*_channelHeight + 5));
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
    if (!HasCapture())
    {
        CaptureMouse();
    }

    wxPoint pos = event.GetPosition();

    // reset all variables
    _clickedChannel = -1;
    _clickedTime = false;
    _dragging = false;

    if (pos.x < _timelineStartX)
    {
        if (pos.y < _timelineStartY)
        {

        }
        else
        {
            _clickedChannel = getChannelFromPosition(pos);
            showPopUp();
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
            _selectedTrack = -1;
            _clickedChannel = getChannelFromPosition(pos);
            if (theAnimationManager.getTimeline()->isBetweenTwoTracks(_clickedChannel, getTimeFromPosition(pos)))
            {
                // wxMessageBox(_("A"));
            }
            // get the track at click position
            TimelineTrack* track = theAnimationManager.getTimeline()->getTrack(_clickedChannel, getTimeFromPosition(pos));
            if (track != nullptr)
            {
                _selectedTrack = track->getId();
                _mouseToTrackOffset = pos.x - getPositionFromTime(track->getStartTime());
                _draggedTrackLength = track->getLength();
                _draggedTrackPos = pos.x - _mouseToTrackOffset;
                _draggedTrackChannel = getChannelFromPosition(pos);
                _dragging = true;
            }
        }
    }
    Refresh();
}

void TimelinePanel::OnLeftUp(wxMouseEvent& event)
{
    if (HasCapture())
    {
        ReleaseMouse();
    }
    _clickedTime = false;
    _clickedChannel = -1;

    endDragDrop();
}

void TimelinePanel::OnRightDown(wxMouseEvent& event)
{
    wxPoint pos = event.GetPosition();

    if (pos.x > _timelineStartX && pos.y < _timelineStartY)
    {
        if (_interpolationPossible)
        {
            // TODO(JK#1#): do the interpolation stuff
        }
    }
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
            // TODO(JK#1#): check if track is beyond _maxTime
            if (getTimeFromPosition(wxPoint(_draggedTrackPos, 0)) <= 0)
            {
                _draggedTrackPos = getPositionFromTime(0);
            }
            else if (getTimeFromPosition(wxPoint(_draggedTrackPos, 0)) + _draggedTrackLength > _maxTime)
            {
                _draggedTrackPos = getPositionFromTime(_maxTime - _draggedTrackLength);
            }
            else if (abs(getPositionFromTime(_cursorPosition) - _draggedTrackPos) < 3)
            {
                // clamp dragged track to the time cursor if nearby
                _draggedTrackPos = getPositionFromTime(_cursorPosition);
            }
            _draggedTrackChannel = getChannelFromPosition(pos);
        }
        Refresh();
    }
    else
    {
        if (pos.x > _timelineStartX && pos.y > _timelineStartY)
        {
            _interpolationChannel = getChannelFromPosition(pos);
            unsigned int time = getTimeFromPosition(pos);
            if (theAnimationManager.getTimeline()->isBetweenTwoTracks(_interpolationChannel, time))
            {
                _interpolationPossible = true;
                _interpolationStartTime = theAnimationManager.getTimeline()->getTrackBefore(_interpolationChannel, time)->getEndTime();
                _interpolationEndTime = theAnimationManager.getTimeline()->getTrackAfter(_interpolationChannel, time)->getStartTime();
                Refresh();
            }
            // check if _interpolationPossible was set to avoid unnecessary refreshes
            else if (_interpolationPossible)
            {
                _interpolationPossible = false;
                Refresh();
            }
        }
        // check if _interpolationPossible was set to avoid unnecessary refreshes
        else if (_interpolationPossible)
        {
            _interpolationPossible = false;
            Refresh();
        }
    }

    if (_clickedChannel >= 0)
    {
        // TODO(JK#1#): use _clickedChannel for swapping channels
        // Refresh();
    }
    else if (_clickedTime)
    {
        setCursorPosition(getTimeFromPosition(pos));
        if (_cursorPosition < _timeOffset)
        {
            setCursorPosition(_timeOffset);
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
        _timeOffset -= _msPerTimeUnit * (event.GetWheelRotation()/event.GetWheelDelta());
        if (_timeOffset <= 0)
        {
            _timeOffset = 0;
        }
        else if (_timeOffset + _msPerTimeUnit * (GetSize().x - _timelineStartX) / _numPixelsPerTimeUnit > _maxTime)
        {
            // if out of time boundary, undo
            _timeOffset += _msPerTimeUnit * (event.GetWheelRotation()/event.GetWheelDelta());
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
    switch (_msPerTimeUnit)
    {
        case 20:
            _msPerTimeUnit = 10;
            break;
        case 50:
            _msPerTimeUnit = 20;
            break;
        case 100:
            _msPerTimeUnit = 50;
            break;
        case 200:
            _msPerTimeUnit = 100;
            break;
        case 500:
            _msPerTimeUnit = 200;
            break;
        case 1000:
            _msPerTimeUnit = 500;
            break;
        case 2000:
            _msPerTimeUnit = 1000;
            break;
        case 5000:
            _msPerTimeUnit = 2000;
            break;
        case 10000:
            _msPerTimeUnit = 5000;
            break;
        case 20000:
            _msPerTimeUnit = 10000;
            break;
        case 30000:
            _msPerTimeUnit = 20000;
            break;
        case 60000:
            _msPerTimeUnit = 30000;
            break;
        case 120000:
            _msPerTimeUnit = 60000;
            break;
        case 300000:
            _msPerTimeUnit = 120000;
            break;
        case 600000:
            _msPerTimeUnit = 300000;
            break;
        default:
            break;
    }
    Refresh();
}

void TimelinePanel::OnButtonZoomOutClick(wxCommandEvent& event)
{
    switch (_msPerTimeUnit)
    {
        case 10:
            _msPerTimeUnit = 20;
            _timeOffset /= 20;
            _timeOffset *= 20;
            break;
        case 20:
            _msPerTimeUnit = 50;
            _timeOffset /= 50;
            _timeOffset *= 50;
            break;
        case 50:
            _msPerTimeUnit = 100;
            _timeOffset /= 100;
            _timeOffset *= 100;
            break;
        case 100:
            _msPerTimeUnit = 200;
            _timeOffset /= 200;
            _timeOffset *= 200;
            break;
        case 200:
            _msPerTimeUnit = 500;
            _timeOffset /= 500;
            _timeOffset *= 500;
            break;
        case 500:
            _msPerTimeUnit = 1000;
            _timeOffset /= 1000;
            _timeOffset *= 1000;
            break;
        case 1000:
            _msPerTimeUnit = 2000;
            _timeOffset /= 2000;
            _timeOffset *= 2000;
            break;
        case 2000:
            _msPerTimeUnit = 5000;
            _timeOffset /= 5000;
            _timeOffset *= 5000;
            break;
        case 5000:
            _msPerTimeUnit = 10000;
            _timeOffset /= 10000;
            _timeOffset *= 10000;
            break;
        case 10000:
            _msPerTimeUnit = 20000;
            _timeOffset /= 20000;
            _timeOffset *= 20000;
            break;
        case 20000:
            _msPerTimeUnit = 30000;
            _timeOffset /= 30000;
            _timeOffset *= 30000;
            break;
        case 30000:
            _msPerTimeUnit = 60000;
            _timeOffset /= 60000;
            _timeOffset *= 60000;
            break;
        case 60000:
            _msPerTimeUnit = 120000;
            _timeOffset /= 120000;
            _timeOffset *= 120000;
            break;
        case 120000:
            _msPerTimeUnit = 300000;
            _timeOffset /= 300000;
            _timeOffset *= 300000;
            break;
        case 300000:
            _msPerTimeUnit = 600000;
            _timeOffset /= 600000;
            _timeOffset *= 600000;
            break;
        default:
            break;
    }
    // adjust time offset to time boundary
    while (_timeOffset + _msPerTimeUnit * (GetSize().x - _timelineStartX) / _numPixelsPerTimeUnit >= _maxTime)
    {
        _timeOffset -= _msPerTimeUnit;
    }
    Refresh();
}




