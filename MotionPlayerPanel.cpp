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
#include "MotionPlayerPanel.h"
#include "AnimationManager.h"
#include "FileHandler.h"
#include "CustomEvents.h"

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(MotionPlayerPanel)
	#include <wx/intl.h>
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(MotionPlayerPanel)
//*)

//(*IdInit(MotionPlayerPanel)
const long MotionPlayerPanel::ID_GLCANVAS = wxNewId();
const long MotionPlayerPanel::ID_SLIDERFRAMES = wxNewId();
const long MotionPlayerPanel::ID_BUTTONPLAY = wxNewId();
const long MotionPlayerPanel::ID_TREECTRLSKELETON = wxNewId();
const long MotionPlayerPanel::ID_STATICTEXT1 = wxNewId();
const long MotionPlayerPanel::ID_STATICTEXTFRAMES = wxNewId();
const long MotionPlayerPanel::ID_STATICTEXT3 = wxNewId();
const long MotionPlayerPanel::ID_STATICTEXTFPS = wxNewId();
const long MotionPlayerPanel::ID_STATICTEXT5 = wxNewId();
const long MotionPlayerPanel::ID_STATICTEXTLENGTH = wxNewId();
const long MotionPlayerPanel::ID_LISTBOXSEQUENCES = wxNewId();
const long MotionPlayerPanel::ID_BUTTONLOAD = wxNewId();
const long MotionPlayerPanel::ID_BUTTONSAVE = wxNewId();
//*)
const long MotionPlayerPanel::ID_TIMER = wxNewId();

BEGIN_EVENT_TABLE(MotionPlayerPanel,wxPanel)
	//(*EventTable(MotionPlayerPanel)
	//*)
END_EVENT_TABLE()

MotionPlayerPanel::MotionPlayerPanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(MotionPlayerPanel)
	wxBoxSizer* BoxSizer4;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer3;

	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer2 = new wxBoxSizer(wxVERTICAL);
	int GLCanvasAttributes_1[] = {
		WX_GL_RGBA,
		WX_GL_DOUBLEBUFFER,
		WX_GL_DEPTH_SIZE,      16,
		WX_GL_STENCIL_SIZE,    0,
		0, 0 };
	glCanvas = new GLCanvas(this, ID_GLCANVAS, wxDefaultPosition, wxDefaultSize, 0, _T("ID_GLCANVAS"), GLCanvasAttributes_1);
	glCanvas->SetBackgroundColour(wxColour(0,0,0));
	BoxSizer2->Add(glCanvas, 1, wxALL|wxEXPAND, 5);
	SliderFrames = new wxSlider(this, ID_SLIDERFRAMES, 0, 0, 1, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDERFRAMES"));
	SliderFrames->SetTickFreq(1);
	BoxSizer2->Add(SliderFrames, 0, wxALL|wxEXPAND, 5);
	ButtonPlay = new wxButton(this, ID_BUTTONPLAY, _("Play"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONPLAY"));
	BoxSizer2->Add(ButtonPlay, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer2, 1, wxALL|wxEXPAND, 5);
	BoxSizer3 = new wxBoxSizer(wxVERTICAL);
	TreeCtrlSkeleton = new wxTreeCtrl(this, ID_TREECTRLSKELETON, wxDefaultPosition, wxDefaultSize, wxTR_EDIT_LABELS|wxTR_DEFAULT_STYLE, wxDefaultValidator, _T("ID_TREECTRLSKELETON"));
	BoxSizer3->Add(TreeCtrlSkeleton, 2, wxALL|wxEXPAND, 5);
	GridSizerSequenceInfo = new wxGridSizer(3, 2, 0, 0);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Frames"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	GridSizerSequenceInfo->Add(StaticText1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	StaticTextFrames = new wxStaticText(this, ID_STATICTEXTFRAMES, _("0"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXTFRAMES"));
	GridSizerSequenceInfo->Add(StaticTextFrames, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("FPS"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	GridSizerSequenceInfo->Add(StaticText3, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	StaticTextFPS = new wxStaticText(this, ID_STATICTEXTFPS, _("0"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXTFPS"));
	GridSizerSequenceInfo->Add(StaticTextFPS, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Length in sec"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	GridSizerSequenceInfo->Add(StaticText5, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	StaticTextLength = new wxStaticText(this, ID_STATICTEXTLENGTH, _("0"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXTLENGTH"));
	GridSizerSequenceInfo->Add(StaticTextLength, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer3->Add(GridSizerSequenceInfo, 0, wxALL|wxEXPAND, 5);
	ListBoxSequences = new wxListBox(this, ID_LISTBOXSEQUENCES, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOXSEQUENCES"));
	BoxSizer3->Add(ListBoxSequences, 1, wxALL|wxEXPAND, 5);
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	ButtonLoad = new wxButton(this, ID_BUTTONLOAD, _("Load"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONLOAD"));
	BoxSizer4->Add(ButtonLoad, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonSave = new wxButton(this, ID_BUTTONSAVE, _("Save as..."), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONSAVE"));
	BoxSizer4->Add(ButtonSave, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer3->Add(BoxSizer4, 0, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 25);
	BoxSizer1->Add(BoxSizer3, 0, wxALL|wxEXPAND, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);

	Connect(ID_SLIDERFRAMES,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&MotionPlayerPanel::OnSliderFramesCmdSliderUpdated);
	Connect(ID_BUTTONPLAY,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MotionPlayerPanel::OnButtonPlayClick);
	Connect(ID_LISTBOXSEQUENCES,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&MotionPlayerPanel::OnListBoxSequencesSelect);
	Connect(ID_BUTTONLOAD,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MotionPlayerPanel::OnButtonLoadClick);
	Connect(ID_BUTTONSAVE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MotionPlayerPanel::OnButtonSaveClick);
	//*)

    _timer = new wxTimer(this, ID_TIMER);

	Connect(UpdateEvent, (wxObjectEventFunction)&MotionPlayerPanel::OnUpdateEvent);
    Connect(ID_TIMER, wxEVT_TIMER, wxTimerEventHandler(MotionPlayerPanel::OnTimerEvent));

    glCanvas->setSkeleton(theAnimationManager.getTimelineSkeleton());
	glCanvas->setStyle(DRAW_LOCAL_COORDINATE_SYSTEM);

    _currentFrame = 0;
    _currentProjectSequence = -1;

    glCanvas->setSkeleton(nullptr);

    updateListBoxSequences();
    updateSequenceInfo();
}

MotionPlayerPanel::~MotionPlayerPanel()
{
	//(*Destroy(MotionPlayerPanel)
	//*)
}

void MotionPlayerPanel::OnUpdateEvent(wxEvent& event)
{

}

void MotionPlayerPanel::OnTimerEvent(wxTimerEvent& event)
{
    MotionSequence* sequence = theAnimationManager.getProjectSequence(_currentProjectSequence);
    if (_currentFrame >= sequence->getNumFrames())
    {
        _currentFrame = 0;
    }
    sequence->setToFrame(_currentFrame);
    SliderFrames->SetValue(_currentFrame);
    glCanvas->Refresh();
    ++_currentFrame;
}

void MotionPlayerPanel::OnButtonPlayClick(wxCommandEvent& event)
{
    if (_timer->IsRunning() || _currentProjectSequence < 0)
    {
        ButtonPlay->SetLabel(_("Play"));
        stop();
    }
    else
    {
        ButtonPlay->SetLabel(_("Pause"));
        play();
    }
}

void MotionPlayerPanel::OnButtonLoadClick(wxCommandEvent& event)
{
    // show the file dialog
    wxFileDialog* fileDialog = new wxFileDialog(this, _("Open bvh file"), _(""), _(""), _("bvh files (*.bvh)|*.bvh"), wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if (fileDialog->ShowModal() == wxID_CANCEL)
    {
        fileDialog->Destroy();
        return;
    }
    SetCursor(wxCURSOR_ARROWWAIT);
    MotionSequence* sequence = FileHandler::readBVH(fileDialog->GetPath());
    fileDialog->Destroy();

    SetCursor(wxCURSOR_DEFAULT);

    if (sequence == nullptr)
    {
        wxString msg;
        msg << _("Error loading the file: ") << fileDialog->GetPath();
        wxMessageBox(msg, _("Error"), wxICON_ERROR);
        return;
    }
    _currentProjectSequence = theAnimationManager.addProjectSequence(sequence);
    _currentFrame = 0;
    sequence->setToFrame(_currentFrame);

    SliderFrames->SetMax(sequence->getNumFrames());

    updateListBoxSequences();
    updateSequenceInfo();

//    glCanvas->setSkeleton(theAnimationManager.getSequenceSkeleton(_currentProjectSequence));
    glCanvas->setSkeleton(sequence->getSkeleton());
    glCanvas->Refresh();
}

void MotionPlayerPanel::OnButtonSaveClick(wxCommandEvent& event)
{
    MotionSequence* sequence = theAnimationManager.getProjectSequence(_currentProjectSequence);
    if (sequence == nullptr)
    {
        return;
    }
    // show the file dialog
    wxFileDialog* fileDialog = new wxFileDialog(this, _("Save bvh file"), _(""), _(""), _("bvh files (*.bvh)|*.bvh"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
    if (fileDialog->ShowModal() == wxID_CANCEL)
    {
        fileDialog->Destroy();
        return;
    }

    wxString path = fileDialog->GetPath();

    fileDialog->Destroy();

    FileHandler::writeBVH(path, sequence);
}

void MotionPlayerPanel::play()
{
    MotionSequence* sequence = theAnimationManager.getProjectSequence(_currentProjectSequence);
    if (sequence != nullptr)
    {
        sequence->setToFrame(_currentFrame);
        glCanvas->Refresh();
        _timer->Start(sequence->getFrameTime() * 1000.0f);
    }
}

void MotionPlayerPanel::stop()
{
    _timer->Stop();

    SliderFrames->SetValue(_currentFrame);
    MotionSequence* sequence = theAnimationManager.getProjectSequence(_currentProjectSequence);
    if (sequence != nullptr)
    {
        sequence->setToFrame(_currentFrame);
        glCanvas->Refresh();
    }
}

void MotionPlayerPanel::updateListBoxSequences()
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

void MotionPlayerPanel::updateSequenceInfo()
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

void MotionPlayerPanel::OnListBoxSequencesSelect(wxCommandEvent& event)
{
    stop();
    _currentProjectSequence = ListBoxSequences->GetSelection();
    glCanvas->setSkeleton(theAnimationManager.getSequenceSkeleton(_currentProjectSequence));
    glCanvas->Refresh();

    MotionSequence* sequence = theAnimationManager.getProjectSequence(_currentProjectSequence);
    SliderFrames->SetMax(sequence->getNumFrames());
    updateSequenceInfo();
}

void MotionPlayerPanel::OnSliderFramesCmdSliderUpdated(wxScrollEvent& event)
{
    MotionSequence* sequence = theAnimationManager.getProjectSequence(_currentProjectSequence);
    if (sequence == nullptr)
    {
        return;
    }
    _currentFrame = SliderFrames->GetValue();
    sequence->setToFrame(_currentFrame);
    glCanvas->Refresh();
}
