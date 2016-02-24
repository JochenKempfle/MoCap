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
#include "PostProcessPanel.h"
#include "AnimationManager.h"
#include "MoCapManager.h"
#include "MoCapMain.h"
#include "CustomEvents.h"

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(PostProcessPanel)
	#include <wx/intl.h>
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(PostProcessPanel)
//*)

//(*IdInit(PostProcessPanel)
const long PostProcessPanel::ID_GLCANVAS = wxNewId();
const long PostProcessPanel::ID_SLIDER2 = wxNewId();
const long PostProcessPanel::ID_BUTTONSETSKELETON = wxNewId();
const long PostProcessPanel::ID_BUTTONPLAY = wxNewId();
const long PostProcessPanel::ID_TOGGLEBUTTONTIMELINE = wxNewId();
const long PostProcessPanel::ID_TOGGLEBUTTONPREVIEW = wxNewId();
const long PostProcessPanel::ID_LISTBOXSEQUENCES = wxNewId();
const long PostProcessPanel::ID_GENERICDIRCTRL = wxNewId();
const long PostProcessPanel::ID_TREECTRLSKELETON = wxNewId();
const long PostProcessPanel::ID_STATICTEXT3 = wxNewId();
const long PostProcessPanel::ID_STATICTEXTFRAMES = wxNewId();
const long PostProcessPanel::ID_STATICTEXT4 = wxNewId();
const long PostProcessPanel::ID_STATICTEXTFPS = wxNewId();
const long PostProcessPanel::ID_STATICTEXT5 = wxNewId();
const long PostProcessPanel::ID_STATICTEXTLENGTH = wxNewId();
const long PostProcessPanel::ID_CHECKBOXSELECTALL = wxNewId();
const long PostProcessPanel::ID_PANELDRAGDROPSEQUENCE = wxNewId();
const long PostProcessPanel::ID_BUTTONEXPORT = wxNewId();
const long PostProcessPanel::ID_BUTTONSAVE = wxNewId();
const long PostProcessPanel::ID_BUTTONLOAD = wxNewId();
const long PostProcessPanel::ID_STATICTEXT1 = wxNewId();
const long PostProcessPanel::ID_SPINCTRLYAW = wxNewId();
const long PostProcessPanel::ID_STATICTEXT10 = wxNewId();
const long PostProcessPanel::ID_SPINCTRLPITCH = wxNewId();
const long PostProcessPanel::ID_STATICTEXT11 = wxNewId();
const long PostProcessPanel::ID_SPINCTRLROLL = wxNewId();
const long PostProcessPanel::ID_BUTTONFROMSELECTION = wxNewId();
const long PostProcessPanel::ID_PANELDRAGDROPFRAME = wxNewId();
const long PostProcessPanel::ID_TIMELINE = wxNewId();
//*)
const long PostProcessPanel::ID_TIMER = wxNewId();

BEGIN_EVENT_TABLE(PostProcessPanel,wxPanel)
	//(*EventTable(PostProcessPanel)
	//*)
END_EVENT_TABLE()

PostProcessPanel::PostProcessPanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(PostProcessPanel)
	wxBoxSizer* BoxSizer4;
	wxStaticBoxSizer* StaticBoxSizer2;
	wxBoxSizer* BoxSizer5;
	wxBoxSizer* BoxSizer10;
	wxBoxSizer* BoxSizer8;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer11;
	wxStaticBoxSizer* StaticBoxSizer3;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer9;
	wxStaticBoxSizer* StaticBoxSizer1;
	wxBoxSizer* BoxSizer3;
	wxGridSizer* GridSizer2;

	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer4 = new wxBoxSizer(wxVERTICAL);
	int GLCanvasAttributes_1[] = {
		WX_GL_RGBA,
		WX_GL_DOUBLEBUFFER,
		WX_GL_DEPTH_SIZE,      16,
		WX_GL_STENCIL_SIZE,    0,
		0, 0 };
	glCanvas = new GLCanvas(this, ID_GLCANVAS, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE, _T("ID_GLCANVAS"), GLCanvasAttributes_1);
	glCanvas->SetBackgroundColour(wxColour(0,0,0));
	BoxSizer4->Add(glCanvas, 1, wxALL|wxEXPAND, 5);
	Slider2 = new wxSlider(this, ID_SLIDER2, 0, 0, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER2"));
	BoxSizer4->Add(Slider2, 0, wxEXPAND, 5);
	BoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
	ButtonSetSkeleton = new wxButton(this, ID_BUTTONSETSKELETON, _("Set Skeleton"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONSETSKELETON"));
	BoxSizer9->Add(ButtonSetSkeleton, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer9->Add(-1,-1,1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonPlay = new wxButton(this, ID_BUTTONPLAY, _("Play"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONPLAY"));
	ButtonPlay->SetDefault();
	BoxSizer9->Add(ButtonPlay, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer9->Add(-1,-1,1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ToggleButtonTimeline = new wxToggleButton(this, ID_TOGGLEBUTTONTIMELINE, _("Timeline"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TOGGLEBUTTONTIMELINE"));
	ToggleButtonTimeline->SetValue(true);
	BoxSizer9->Add(ToggleButtonTimeline, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ToggleButtonPreview = new wxToggleButton(this, ID_TOGGLEBUTTONPREVIEW, _("Preview"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TOGGLEBUTTONPREVIEW"));
	BoxSizer9->Add(ToggleButtonPreview, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer4->Add(BoxSizer9, 0, wxEXPAND, 5);
	BoxSizer2->Add(BoxSizer4, 1, wxEXPAND, 5);
	BoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Import"));
	BoxSizer10 = new wxBoxSizer(wxVERTICAL);
	ListBoxSequences = new wxListBox(this, ID_LISTBOXSEQUENCES, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOXSEQUENCES"));
	BoxSizer10->Add(ListBoxSequences, 1, wxALL|wxEXPAND, 5);
	GenericDirCtrl = new wxGenericDirCtrl(this, ID_GENERICDIRCTRL, _T("C:\\Users\\Jochen\\Desktop"), wxDefaultPosition, wxDefaultSize, wxDIRCTRL_SHOW_FILTERS, _T("*.bvh"), 0, _T("ID_GENERICDIRCTRL"));
	BoxSizer10->Add(GenericDirCtrl, 2, wxALL|wxEXPAND, 5);
	StaticBoxSizer1->Add(BoxSizer10, 1, wxEXPAND, 5);
	BoxSizer11 = new wxBoxSizer(wxVERTICAL);
	TreeCtrlSkeleton = new wxTreeCtrl(this, ID_TREECTRLSKELETON, wxDefaultPosition, wxDefaultSize, wxTR_MULTIPLE|wxTR_DEFAULT_STYLE, wxDefaultValidator, _T("ID_TREECTRLSKELETON"));
	BoxSizer11->Add(TreeCtrlSkeleton, 1, wxALL|wxEXPAND, 5);
	GridSizerSequenceInfo = new wxGridSizer(3, 2, 0, 0);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Frames"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	GridSizerSequenceInfo->Add(StaticText3, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
	StaticTextFrames = new wxStaticText(this, ID_STATICTEXTFRAMES, _("1000"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXTFRAMES"));
	GridSizerSequenceInfo->Add(StaticTextFrames, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("FPS"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	GridSizerSequenceInfo->Add(StaticText4, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
	StaticTextFPS = new wxStaticText(this, ID_STATICTEXTFPS, _("100"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXTFPS"));
	GridSizerSequenceInfo->Add(StaticTextFPS, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Length in sec"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	GridSizerSequenceInfo->Add(StaticText5, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
	StaticTextLength = new wxStaticText(this, ID_STATICTEXTLENGTH, _("10"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXTLENGTH"));
	GridSizerSequenceInfo->Add(StaticTextLength, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer11->Add(GridSizerSequenceInfo, 0, wxALL|wxEXPAND, 5);
	CheckBoxSelectAll = new wxCheckBox(this, ID_CHECKBOXSELECTALL, _("add all channels"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXSELECTALL"));
	CheckBoxSelectAll->SetValue(true);
	BoxSizer11->Add(CheckBoxSelectAll, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	PanelDragDropSequence = new wxPanel(this, ID_PANELDRAGDROPSEQUENCE, wxDefaultPosition, wxSize(160,60), wxSIMPLE_BORDER|wxTAB_TRAVERSAL, _T("ID_PANELDRAGDROPSEQUENCE"));
	PanelDragDropSequence->SetBackgroundColour(wxColour(230,230,130));
	BoxSizer11->Add(PanelDragDropSequence, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer1->Add(BoxSizer11, 1, wxEXPAND, 5);
	BoxSizer8->Add(StaticBoxSizer1, 2, wxALL|wxEXPAND, 5);
	BoxSizer5 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer3 = new wxStaticBoxSizer(wxVERTICAL, this, _("Export/Save"));
	ButtonExport = new wxButton(this, ID_BUTTONEXPORT, _("Export"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONEXPORT"));
	StaticBoxSizer3->Add(ButtonExport, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonSave = new wxButton(this, ID_BUTTONSAVE, _("Save"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONSAVE"));
	StaticBoxSizer3->Add(ButtonSave, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonLoad = new wxButton(this, ID_BUTTONLOAD, _("Load"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONLOAD"));
	StaticBoxSizer3->Add(ButtonLoad, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer5->Add(StaticBoxSizer3, 0, wxALL|wxEXPAND, 5);
	StaticBoxSizer2 = new wxStaticBoxSizer(wxVERTICAL, this, _("Custom Frame"));
	GridSizer2 = new wxGridSizer(3, 2, 0, 0);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Yaw"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	GridSizer2->Add(StaticText1, 0, wxALL|wxEXPAND, 5);
	SpinCtrlYaw = new wxSpinCtrl(this, ID_SPINCTRLYAW, _T("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 360, 0, _T("ID_SPINCTRLYAW"));
	SpinCtrlYaw->SetValue(_T("0"));
	GridSizer2->Add(SpinCtrlYaw, 0, wxALL|wxEXPAND, 5);
	StaticText10 = new wxStaticText(this, ID_STATICTEXT10, _("Pitch"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT10"));
	GridSizer2->Add(StaticText10, 0, wxALL|wxEXPAND, 5);
	SpinCtrlPitch = new wxSpinCtrl(this, ID_SPINCTRLPITCH, _T("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 360, 0, _T("ID_SPINCTRLPITCH"));
	SpinCtrlPitch->SetValue(_T("0"));
	GridSizer2->Add(SpinCtrlPitch, 0, wxALL|wxEXPAND, 5);
	StaticText11 = new wxStaticText(this, ID_STATICTEXT11, _("Roll"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT11"));
	GridSizer2->Add(StaticText11, 0, wxALL|wxEXPAND, 5);
	SpinCtrlRoll = new wxSpinCtrl(this, ID_SPINCTRLROLL, _T("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 360, 0, _T("ID_SPINCTRLROLL"));
	SpinCtrlRoll->SetValue(_T("0"));
	GridSizer2->Add(SpinCtrlRoll, 0, wxALL|wxEXPAND, 5);
	StaticBoxSizer2->Add(GridSizer2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonFromSelection = new wxButton(this, ID_BUTTONFROMSELECTION, _("Set from selection"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONFROMSELECTION"));
	StaticBoxSizer2->Add(ButtonFromSelection, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer2->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	PanelDragDropFrame = new wxPanel(this, ID_PANELDRAGDROPFRAME, wxDefaultPosition, wxSize(160,60), wxSIMPLE_BORDER|wxTAB_TRAVERSAL, _T("ID_PANELDRAGDROPFRAME"));
	PanelDragDropFrame->SetBackgroundColour(wxColour(230,230,130));
	StaticBoxSizer2->Add(PanelDragDropFrame, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer5->Add(StaticBoxSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL, 5);
	BoxSizer8->Add(BoxSizer5, 1, wxEXPAND, 5);
	BoxSizer2->Add(BoxSizer8, 1, wxEXPAND, 0);
	BoxSizer1->Add(BoxSizer2, 3, wxEXPAND, 5);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	timelinePanel = new TimelinePanel(this,ID_TIMELINE,wxDefaultPosition,wxDefaultSize);
	BoxSizer3->Add(timelinePanel, 1, wxALL|wxEXPAND, 5);
	BoxSizer1->Add(BoxSizer3, 2, wxEXPAND, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);

	Connect(ID_BUTTONPLAY,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PostProcessPanel::OnButtonPlayClick);
	Connect(ID_TOGGLEBUTTONTIMELINE,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&PostProcessPanel::OnToggleButtonTimelineToggle);
	Connect(ID_TOGGLEBUTTONPREVIEW,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&PostProcessPanel::OnToggleButtonPreviewToggle);
	Connect(ID_LISTBOXSEQUENCES,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&PostProcessPanel::OnListBoxSequencesSelect);
	PanelDragDropSequence->Connect(wxEVT_PAINT,(wxObjectEventFunction)&PostProcessPanel::OnPanelDragDropSequencePaint,0,this);
	PanelDragDropSequence->Connect(wxEVT_LEFT_DOWN,(wxObjectEventFunction)&PostProcessPanel::OnPanelDragDropSequenceLeftDown,0,this);
	PanelDragDropFrame->Connect(wxEVT_PAINT,(wxObjectEventFunction)&PostProcessPanel::OnPanelDragDropFramePaint,0,this);
	PanelDragDropFrame->Connect(wxEVT_LEFT_DOWN,(wxObjectEventFunction)&PostProcessPanel::OnPanelDragDropFrameLeftDown,0,this);
	timelinePanel->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&PostProcessPanel::OnTimelinePanelLeftUp,0,this);
	//*)
	Connect(wxEVT_MOUSE_CAPTURE_LOST, (wxObjectEventFunction)&PostProcessPanel::OnMouseCaptureLost);
	Connect(UpdateEvent, (wxObjectEventFunction)&PostProcessPanel::OnUpdateEvent);
	GenericDirCtrl->Connect(wxEVT_DIRCTRL_FILEACTIVATED,(wxObjectEventFunction)&PostProcessPanel::OnGenericDirCtrlActivated,0,this);
    Connect(ID_TIMER, wxEVT_TIMER, wxTimerEventHandler(PostProcessPanel::OnTimerEvent));
//	Treebook1->AddPage(new TimelinePanel(Treebook1,wxNewId(),wxDefaultPosition,wxDefaultSize), _("text"));
//	Treebook1->AddPage(new TimelinePanel(this,wxNewId(),wxDefaultPosition,wxDefaultSize), _("text2"));

    _timer = new wxTimer(this, ID_TIMER);

    theAnimationManager.setTimelineSkeleton(theMoCapManager.getSkeleton());

    glCanvas->setSkeleton(theAnimationManager.getTimelineSkeleton());
	glCanvas->setStyle(DRAW_LOCAL_COORDINATE_SYSTEM);

    timelinePanel->setGLCanvas(glCanvas);
    _currentFrame = 0;
    _currentProjectSequence = -1;
    _previewMode = false;

    updateListBoxSequences();
    updateSequenceInfo();
}

PostProcessPanel::~PostProcessPanel()
{
	//(*Destroy(PostProcessPanel)
	//*)
    _timer->Stop();
}

void PostProcessPanel::OnUpdateEvent(wxEvent& event)
{

}

void PostProcessPanel::OnMouseCaptureLost(wxMouseCaptureLostEvent& event)
{
    if (HasCapture())
    {
        ReleaseMouse();
    }
    SetCursor(wxCURSOR_ARROW);
}

void PostProcessPanel::OnTimerEvent(wxTimerEvent& event)
{
    if (_previewMode)
    {
        MotionSequence* sequence = theAnimationManager.getProjectSequence(_currentProjectSequence);
        if (_currentFrame >= sequence->getNumFrames())
        {
            _currentFrame = 0;
        }
        sequence->setToFrame(_currentFrame);
        glCanvas->Refresh();
    }
    else
    {
        // TODO(JK#1#): play timeline, use getCurrentCursorPosition as start
        timelinePanel->getCursorPosition();
    }
    ++_currentFrame;
}

void PostProcessPanel::OnPanelDragDropSequenceLeftDown(wxMouseEvent& event)
{
    if (_currentProjectSequence < 0)
    {
        return;
    }
    SetCursor(wxCURSOR_HAND);

    std::vector<int> channelIds;

    if (CheckBoxSelectAll->GetValue())
    {
        channelIds = theAnimationManager.getProjectSequence(_currentProjectSequence)->getChannelIds();
    }
    else
    {
        wxArrayTreeItemIds selections;
        TreeCtrlSkeleton->GetSelections(selections);
        for (size_t i = 0; i < selections.size(); ++i)
        {
            channelIds.push_back(static_cast<TreeItemBoneData*>(TreeCtrlSkeleton->GetItemData(selections[i]))->getBoneId());
        }
    }

    timelinePanel->prepareAddingSequence(_currentProjectSequence, channelIds);
    timelinePanel->CaptureMouse();
}

void PostProcessPanel::OnPanelDragDropFrameLeftDown(wxMouseEvent& event)
{
    SetCursor(wxCURSOR_HAND);

    MotionSequenceFrame frame(Quaternion(SpinCtrlRoll->GetValue() * M_PI / 180.0, SpinCtrlPitch->GetValue() * M_PI / 180.0, SpinCtrlYaw->GetValue() * M_PI / 180.0));

    timelinePanel->prepareAddingFrame(frame);
    timelinePanel->CaptureMouse();
}

void PostProcessPanel::OnTimelinePanelLeftUp(wxMouseEvent& event)
{
    if (timelinePanel->HasCapture())
    {
        timelinePanel->ReleaseMouse();
    }
    SetCursor(wxCURSOR_ARROW);
    event.Skip();
}

void PostProcessPanel::OnGenericDirCtrlActivated(wxTreeEvent& event)
{
    SetCursor(wxCURSOR_ARROWWAIT);
    // TOODO(JK#1#): load file
    MotionSequence* sequence = theAnimationManager.readBVH(GenericDirCtrl->GetPath());

    SetCursor(wxCURSOR_DEFAULT);

    if (sequence == nullptr)
    {
        wxString msg;
        msg << _("Error loading the file: ") << GenericDirCtrl->GetPath();
        wxMessageBox(msg, _("Error"), wxICON_ERROR);
        return;
    }
    _currentProjectSequence = theAnimationManager.addProjectSequence(sequence);

    _currentFrame = 0;

    updateListBoxSequences();
    updateSequenceInfo();

    if (ToggleButtonPreview->GetValue())
    {
        glCanvas->setSkeleton(sequence->getSkeleton());
    }
    Refresh();
}

void PostProcessPanel::OnToggleButtonTimelineToggle(wxCommandEvent& event)
{
    if (_previewMode)
    {
        stop();
    }
    _previewMode = false;
    ToggleButtonPreview->SetValue(false);
    ToggleButtonTimeline->SetValue(true);
    glCanvas->setSkeleton(theAnimationManager.getTimelineSkeleton());
    glCanvas->Refresh();
}

void PostProcessPanel::OnToggleButtonPreviewToggle(wxCommandEvent& event)
{
    if (!_previewMode)
    {
        stop();
    }
    _previewMode = true;
    ToggleButtonPreview->SetValue(true);
    ToggleButtonTimeline->SetValue(false);
    _currentFrame = 0;
    glCanvas->setSkeleton(theAnimationManager.getSequenceSkeleton(_currentProjectSequence));
    glCanvas->Refresh();
}

void PostProcessPanel::OnPanelDragDropSequencePaint(wxPaintEvent& event)
{
    wxPaintDC dc(PanelDragDropSequence);
    if (!dc.IsOk())
    {
        return;
    }
    wxSize size = PanelDragDropSequence->GetSize();
    dc.DrawLabel(_("Drag and drop this panel\nto the timeline to add\ncurrently selected channels"), wxRect(0, 0, size.x, size.y), wxALIGN_CENTER);
}

void PostProcessPanel::OnPanelDragDropFramePaint(wxPaintEvent& event)
{
    wxPaintDC dc(PanelDragDropFrame);
    if (!dc.IsOk())
    {
        return;
    }
    wxSize size = PanelDragDropFrame->GetSize();
    dc.DrawLabel(_("Drag and drop this panel\nto the timeline to add\na custom frame"), wxRect(0, 0, size.x, size.y), wxALIGN_CENTER);
}

void PostProcessPanel::OnButtonPlayClick(wxCommandEvent& event)
{
    if (_timer->IsRunning())
    {
        ButtonPlay->SetLabel(_("Play"));
        stop();
    }
    else
    {
        ButtonPlay->SetLabel(_("Stop"));
        play();
    }
}

void PostProcessPanel::play()
{
    if (_previewMode)
    {
        MotionSequence* sequence = theAnimationManager.getProjectSequence(_currentProjectSequence);
        if (sequence != nullptr)
        {
            sequence->setToFrame(_currentFrame);
            glCanvas->Refresh();
            _timer->Start(sequence->getFrameTime() * 1000);
        }
    }
    else if (ToggleButtonTimeline->GetValue())
    {

    }
}

void PostProcessPanel::stop()
{
    _timer->Stop();
    if (_previewMode)
    {
        _currentFrame = 0;
        MotionSequence* sequence = theAnimationManager.getProjectSequence(_currentProjectSequence);
        if (sequence != nullptr)
        {
            sequence->setToFrame(_currentFrame);
            glCanvas->Refresh();
        }
    }
}

void PostProcessPanel::updateListBoxSequences()
{
    ListBoxSequences->Clear();
    std::vector<MotionSequence*> sequences = theAnimationManager.getProjectSequences();
    for (size_t i = 0; i < sequences.size(); ++i)
    {
        ListBoxSequences->Append(sequences[i]->getName());
    }
    ListBoxSequences->SetSelection(_currentProjectSequence);
    ListBoxSequences->Refresh();
}

void PostProcessPanel::updateSequenceInfo()
{
    // clear the tree control
    TreeCtrlSkeleton->DeleteAllItems();
    _treeItemIdFromBoneId.clear();

    StaticTextFrames->SetLabel(_("0"));
    StaticTextFPS->SetLabel(_("0"));
    StaticTextLength->SetLabel(_("0"));

    MotionSequence* sequence = theAnimationManager.getProjectSequence(_currentProjectSequence);
    if (sequence == nullptr)
    {
        return;
    }
    Skeleton* skeleton = sequence->getSkeleton();
    if (skeleton == nullptr)
    {
        return;
    }
    Bone* root = skeleton->getRoot();
    // when there is no bone in the skeleton, we are done
    if (root == nullptr)
    {
        return;
    }
    // get all children in breadth first sorted order
    std::vector<Bone*> bones = root->getAllChildren();

    // _treeItemIdFromBoneId is only needed when allowing selection in the gl panel
    _treeItemIdFromBoneId[root->getId()] = TreeCtrlSkeleton->AddRoot(sequence->getChannel(root->getId())->getName(), -1, -1, new TreeItemBoneData(root->getId()));

    for (size_t i = 0; i < bones.size(); ++i)
    {
        // append new tree item (tree item parent is item id of parent of current bone)
        _treeItemIdFromBoneId[bones[i]->getId()] = TreeCtrlSkeleton->AppendItem(_treeItemIdFromBoneId[bones[i]->getParent()->getId()], sequence->getChannel(bones[i]->getId())->getName(), -1, -1, new TreeItemBoneData(bones[i]->getId()));
    }
    TreeCtrlSkeleton->ExpandAll();

    wxString label;
    label << sequence->getNumFrames();
    StaticTextFrames->SetLabel(label);

    label.Clear();
    label.Printf(_("%.1f"), 1.0/sequence->getFrameTime());
    StaticTextFPS->SetLabel(label);

    label.Clear();
    label.Printf(_("%.1f"), sequence->getNumFrames() * sequence->getFrameTime());
    StaticTextLength->SetLabel(label);

    GridSizerSequenceInfo->Layout();
    TreeCtrlSkeleton->Refresh();
}

void PostProcessPanel::OnListBoxSequencesSelect(wxCommandEvent& event)
{
    _currentProjectSequence = ListBoxSequences->GetSelection();
    updateSequenceInfo();
}






