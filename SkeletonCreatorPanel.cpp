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
#include <gl/glu.h>
#include "SkeletonCreatorPanel.h"

#include "MoCapManager.h"

#ifndef WX_PRECOMP
    //(*InternalHeadersPCH(SkeletonCreatorPanel)
    #include <wx/intl.h>
    #include <wx/string.h>
    //*)
#endif
//(*InternalHeaders(SkeletonCreatorPanel)
//*)

//(*IdInit(SkeletonCreatorPanel)
const long SkeletonCreatorPanel::ID_BUTTONLOAD = wxNewId();
const long SkeletonCreatorPanel::ID_BUTTONSAVE = wxNewId();
const long SkeletonCreatorPanel::ID_BUTTONRESET = wxNewId();
const long SkeletonCreatorPanel::ID_GLCANVAS = wxNewId();
const long SkeletonCreatorPanel::ID_TREECTRLSKELETON = wxNewId();
const long SkeletonCreatorPanel::ID_STATICTEXT1 = wxNewId();
const long SkeletonCreatorPanel::ID_TEXTCTRLNAME = wxNewId();
const long SkeletonCreatorPanel::ID_STATICTEXT2 = wxNewId();
const long SkeletonCreatorPanel::ID_CHOICEPARENT = wxNewId();
const long SkeletonCreatorPanel::ID_STATICTEXT9 = wxNewId();
const long SkeletonCreatorPanel::ID_TEXTCTRLLENGTH = wxNewId();
const long SkeletonCreatorPanel::ID_STATICTEXT6 = wxNewId();
const long SkeletonCreatorPanel::ID_STATICTEXT7 = wxNewId();
const long SkeletonCreatorPanel::ID_STATICTEXT8 = wxNewId();
const long SkeletonCreatorPanel::ID_STATICTEXT3 = wxNewId();
const long SkeletonCreatorPanel::ID_TEXTCTRL3 = wxNewId();
const long SkeletonCreatorPanel::ID_TEXTCTRL6 = wxNewId();
const long SkeletonCreatorPanel::ID_TEXTCTRL9 = wxNewId();
const long SkeletonCreatorPanel::ID_STATICTEXT4 = wxNewId();
const long SkeletonCreatorPanel::ID_TEXTCTRL4 = wxNewId();
const long SkeletonCreatorPanel::ID_TEXTCTRL7 = wxNewId();
const long SkeletonCreatorPanel::ID_TEXTCTRL10 = wxNewId();
const long SkeletonCreatorPanel::ID_STATICTEXT5 = wxNewId();
const long SkeletonCreatorPanel::ID_TEXTCTRL5 = wxNewId();
const long SkeletonCreatorPanel::ID_TEXTCTRL8 = wxNewId();
const long SkeletonCreatorPanel::ID_TEXTCTRL11 = wxNewId();
const long SkeletonCreatorPanel::ID_BUTTONADDBONE = wxNewId();
const long SkeletonCreatorPanel::ID_BUTTONREMOVEBONE = wxNewId();
//*)

BEGIN_EVENT_TABLE(SkeletonCreatorPanel,wxPanel)
    //(*EventTable(SkeletonCreatorPanel)
    //*)
END_EVENT_TABLE()

SkeletonCreatorPanel::SkeletonCreatorPanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
    //(*Initialize(SkeletonCreatorPanel)
    wxBoxSizer* BoxSizer4;
    wxBoxSizer* BoxSizer6;
    wxBoxSizer* BoxSizer5;
    wxBoxSizer* BoxSizer7;
    wxBoxSizer* BoxSizer8;
    wxBoxSizer* BoxSizer2;
    wxGridSizer* GridSizer1;
    wxBoxSizer* BoxSizer1;
    wxBoxSizer* BoxSizer9;
    wxBoxSizer* BoxSizer3;

    Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    BoxSizer2 = new wxBoxSizer(wxVERTICAL);
    BoxSizer2->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ButtonLoad = new wxButton(this, ID_BUTTONLOAD, _("Load"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONLOAD"));
    BoxSizer2->Add(ButtonLoad, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ButtonSave = new wxButton(this, ID_BUTTONSAVE, _("Save"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONSAVE"));
    BoxSizer2->Add(ButtonSave, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ButtonReset = new wxButton(this, ID_BUTTONRESET, _("Reset"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONRESET"));
    BoxSizer2->Add(ButtonReset, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer1->Add(BoxSizer2, 0, wxEXPAND, 5);
    BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    int GLCanvasAttributes_1[] = {
    	WX_GL_RGBA,
    	WX_GL_DOUBLEBUFFER,
    	WX_GL_DEPTH_SIZE,      16,
    	WX_GL_STENCIL_SIZE,    0,
    	0, 0 };
    glCanvas = new GLCanvas(this, ID_GLCANVAS, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE, _T("ID_GLCANVAS"), GLCanvasAttributes_1);
    glCanvas->SetBackgroundColour(wxColour(0,0,0));
    BoxSizer3->Add(glCanvas, 1, wxALL|wxEXPAND, 5);
    BoxSizer1->Add(BoxSizer3, 1, wxEXPAND, 5);
    BoxSizer4 = new wxBoxSizer(wxVERTICAL);
    TreeCtrlSkeleton = new wxTreeCtrl(this, ID_TREECTRLSKELETON, wxDefaultPosition, wxDefaultSize, wxTR_EDIT_LABELS|wxTR_DEFAULT_STYLE, wxDefaultValidator, _T("ID_TREECTRLSKELETON"));
    BoxSizer4->Add(TreeCtrlSkeleton, 1, wxALL|wxEXPAND, 5);
    BoxSizer5 = new wxBoxSizer(wxVERTICAL);
    BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Name"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    BoxSizer6->Add(StaticText1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrlName = new wxTextCtrl(this, ID_TEXTCTRLNAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, wxDefaultValidator, _T("ID_TEXTCTRLNAME"));
    BoxSizer6->Add(TextCtrlName, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer5->Add(BoxSizer6, 0, wxEXPAND, 5);
    BoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
    StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Parent"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    BoxSizer7->Add(StaticText2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ChoiceParent = new wxChoice(this, ID_CHOICEPARENT, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICEPARENT"));
    BoxSizer7->Add(ChoiceParent, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer5->Add(BoxSizer7, 0, wxEXPAND, 5);
    BoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
    StaticText9 = new wxStaticText(this, ID_STATICTEXT9, _("Length"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT9"));
    BoxSizer8->Add(StaticText9, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrlLength = new wxTextCtrl(this, ID_TEXTCTRLLENGTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, wxFloatingPointValidator<float>(), _T("ID_TEXTCTRLLENGTH"));
    BoxSizer8->Add(TextCtrlLength, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer5->Add(BoxSizer8, 0, wxEXPAND, 5);
    GridSizer1 = new wxGridSizer(4, 4, 0, 0);
    GridSizer1->Add(-1,-1,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("Yaw"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
    GridSizer1->Add(StaticText6, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _("Pitch"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
    GridSizer1->Add(StaticText7, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _("Roll"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
    GridSizer1->Add(StaticText8, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Default"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    GridSizer1->Add(StaticText3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl3 = new wxTextCtrl(this, ID_TEXTCTRL3, _("0"), wxDefaultPosition, wxSize(60,20), 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
    GridSizer1->Add(TextCtrl3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl6 = new wxTextCtrl(this, ID_TEXTCTRL6, _("0"), wxDefaultPosition, wxSize(60,20), 0, wxDefaultValidator, _T("ID_TEXTCTRL6"));
    GridSizer1->Add(TextCtrl6, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl9 = new wxTextCtrl(this, ID_TEXTCTRL9, _("0"), wxDefaultPosition, wxSize(60,20), 0, wxDefaultValidator, _T("ID_TEXTCTRL9"));
    GridSizer1->Add(TextCtrl9, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Min"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    GridSizer1->Add(StaticText4, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl4 = new wxTextCtrl(this, ID_TEXTCTRL4, _("0"), wxDefaultPosition, wxSize(60,20), 0, wxDefaultValidator, _T("ID_TEXTCTRL4"));
    GridSizer1->Add(TextCtrl4, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl7 = new wxTextCtrl(this, ID_TEXTCTRL7, _("0"), wxDefaultPosition, wxSize(60,20), 0, wxDefaultValidator, _T("ID_TEXTCTRL7"));
    GridSizer1->Add(TextCtrl7, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl10 = new wxTextCtrl(this, ID_TEXTCTRL10, _("0"), wxDefaultPosition, wxSize(60,20), 0, wxDefaultValidator, _T("ID_TEXTCTRL10"));
    GridSizer1->Add(TextCtrl10, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Max"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
    GridSizer1->Add(StaticText5, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl5 = new wxTextCtrl(this, ID_TEXTCTRL5, _("0"), wxDefaultPosition, wxSize(60,20), 0, wxDefaultValidator, _T("ID_TEXTCTRL5"));
    GridSizer1->Add(TextCtrl5, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl8 = new wxTextCtrl(this, ID_TEXTCTRL8, _("0"), wxDefaultPosition, wxSize(60,20), 0, wxDefaultValidator, _T("ID_TEXTCTRL8"));
    GridSizer1->Add(TextCtrl8, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl11 = new wxTextCtrl(this, ID_TEXTCTRL11, _("0"), wxDefaultPosition, wxSize(60,20), 0, wxDefaultValidator, _T("ID_TEXTCTRL11"));
    GridSizer1->Add(TextCtrl11, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer5->Add(GridSizer1, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
    ButtonAddBone = new wxButton(this, ID_BUTTONADDBONE, _("Add Bone"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONADDBONE"));
    BoxSizer9->Add(ButtonAddBone, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ButtonRemoveBone = new wxButton(this, ID_BUTTONREMOVEBONE, _("Remove"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONREMOVEBONE"));
    BoxSizer9->Add(ButtonRemoveBone, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer5->Add(BoxSizer9, 0, wxEXPAND, 5);
    BoxSizer4->Add(BoxSizer5, 0, wxEXPAND, 5);
    BoxSizer1->Add(BoxSizer4, 0, wxEXPAND, 5);
    SetSizer(BoxSizer1);
    BoxSizer1->Fit(this);
    BoxSizer1->SetSizeHints(this);

    glCanvas->Connect(wxEVT_LEFT_DOWN,(wxObjectEventFunction)&SkeletonCreatorPanel::OnGlCanvasLeftDown,0,this);
    Connect(ID_TREECTRLSKELETON,wxEVT_COMMAND_TREE_END_LABEL_EDIT,(wxObjectEventFunction)&SkeletonCreatorPanel::OnTreeCtrlSkeletonEndLabelEdit);
    Connect(ID_TREECTRLSKELETON,wxEVT_COMMAND_TREE_DELETE_ITEM,(wxObjectEventFunction)&SkeletonCreatorPanel::OnTreeCtrlSkeletonDeleteItem);
    Connect(ID_TREECTRLSKELETON,wxEVT_COMMAND_TREE_SEL_CHANGED,(wxObjectEventFunction)&SkeletonCreatorPanel::OnTreeCtrlSkeletonSelectionChanged);
    Connect(ID_TREECTRLSKELETON,wxEVT_COMMAND_TREE_KEY_DOWN,(wxObjectEventFunction)&SkeletonCreatorPanel::OnTreeCtrlSkeletonKeyDown);
    Connect(ID_TEXTCTRLNAME,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&SkeletonCreatorPanel::OnTextCtrlNameTextEnter);
    Connect(ID_CHOICEPARENT,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&SkeletonCreatorPanel::OnChoiceParentSelect);
    Connect(ID_TEXTCTRLLENGTH,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&SkeletonCreatorPanel::OnTextCtrlLengthTextEnter);
    //*)
    glCanvas->Connect(wxEVT_MOUSE_CAPTURE_LOST, (wxObjectEventFunction)&SkeletonCreatorPanel::OnMouseCaptureLost,0,this);

	glCanvas->setSkeleton(theMoCapManager.getSkeleton());
	glCanvas->setStyle(DRAW_SPIN_ARROWS | HIGHLIGHT_SELECTED_BONE | DRAW_LOCAL_COORDINATE_SYSTEM);

	static_cast<wxFloatingPointValidator<float>*>(TextCtrlLength->GetValidator())->SetMin(0.0f);
    // _skeleton = theMoCapManager.getSkeleton();
    updateTreeCtrlSkeleton();
    // updateBoneInfo();
    // ChoiceParent->SetSelection(ChoiceParent->Append(_("root")));
}

SkeletonCreatorPanel::~SkeletonCreatorPanel()
{
    //(*Destroy(SkeletonCreatorPanel)
    //*)
    // unselect any bone
    theMoCapManager.selectBone(-1);
}


void SkeletonCreatorPanel::OnTreeCtrlSkeletonEndLabelEdit(wxTreeEvent& event)
{
}

void SkeletonCreatorPanel::OnTreeCtrlSkeletonDeleteItem(wxTreeEvent& event)
{
    wxTreeItemId item = event.GetItem();
    if (item.IsOk())
    {
        //TreeCtrlSkeleton->
    }
    // wxMessageBox(_("delete"));
    // Refresh();
    event.Skip();
}

void SkeletonCreatorPanel::OnTreeCtrlSkeletonSelectionChanged(wxTreeEvent& event)
{
    wxTreeItemId item = event.GetItem();
    TreeItemBoneData* boneData = static_cast<TreeItemBoneData*>(TreeCtrlSkeleton->GetItemData(item));

    int boneId = -1;
    if (boneData != nullptr)
    {
        boneId = boneData->getBoneId();
    }
    theMoCapManager.selectBone(boneId);
    updateBoneInfo();
    Refresh();
}

void SkeletonCreatorPanel::OnTreeCtrlSkeletonKeyDown(wxTreeEvent& event)
{
    int boneId = theMoCapManager.getSelectedBoneId();

    switch (event.GetKeyCode())
    {
        case WXK_DELETE:
            if (TreeCtrlSkeleton->GetSelection().IsOk())
            {
                // TreeCtrlSkeleton->Delete(TreeCtrlSkeleton->GetSelection());
                theMoCapManager.getSkeleton()->eraseBone(boneId, false);
                theMoCapManager.selectBone(-1);
                //TreeCtrlSkeleton->Delete(TreeCtrlSkeleton->GetSelection());
                //TreeCtrlSkeleton->DeleteAllItems();
            }
            break;
        default:
            return;
    }
    theMoCapManager.getSkeleton()->update();
    updateTreeCtrlSkeleton();
    Refresh();
    //event.Skip();
}

void SkeletonCreatorPanel::updateTreeCtrlSkeleton()
{
    // clear the tree control
    TreeCtrlSkeleton->DeleteAllItems();
    _treeItemIdFromBoneId.clear();
    Bone* root = theMoCapManager.getSkeleton()->getRoot();
    // when there is no bone in the skeleton, we are done
    if (root == nullptr)
    {
        return;
    }
    // get all children in breadth first sorted order
    std::vector<Bone*> bones = root->getAllChildren();

    _treeItemIdFromBoneId[root->getId()] = TreeCtrlSkeleton->AddRoot(theMoCapManager.getBoneName(root->getId()), -1, -1, new TreeItemBoneData(root->getId()));

    for (size_t i = 0; i < bones.size(); ++i)
    {
        // append new tree item (tree item parent is item id of parent of current bone)
        _treeItemIdFromBoneId[bones[i]->getId()] = TreeCtrlSkeleton->AppendItem(_treeItemIdFromBoneId[bones[i]->getParent()->getId()], theMoCapManager.getBoneName(bones[i]->getId()), -1, -1, new TreeItemBoneData(bones[i]->getId()));
    }
    TreeCtrlSkeleton->ExpandAll();
    TreeCtrlSkeleton->Refresh();
}

void SkeletonCreatorPanel::updateBoneInfo()
{
    Bone* bone = theMoCapManager.getSelectedBone();
    if (bone == nullptr)
    {
        // no bone selected
        TextCtrlName->Clear();
        ChoiceParent->Clear();
        return;
    }

    TextCtrlName->ChangeValue(bone->getName());
    ChoiceParent->Clear();
    wxString lengthString;
    lengthString << bone->getLength();
    TextCtrlLength->ChangeValue(lengthString);

    if (bone->getParent() != nullptr)
    {
        // get all children in breadth first sorted order
        std::vector<Bone*> bones = theMoCapManager.getSkeleton()->getRoot()->getAllChildren();
        // also insert the root at the beginning
        bones.insert(bones.begin(), 1, theMoCapManager.getSkeleton()->getRoot());
        for (size_t i = 0; i < bones.size(); ++i)
        {
            // only append bones different than current bone
            if (bone != bones[i])
            {
                int id = ChoiceParent->Append(theMoCapManager.getBoneName(bones[i]->getId()), bones[i]);
                // set selection to bones parent
                if (bone->getParent() == bones[i])
                {
                    ChoiceParent->SetSelection(id);
                }
            }
        }
    }
}

void SkeletonCreatorPanel::OnGlCanvasLeftDown(wxMouseEvent& event)
{
    int id = glCanvas->getObjectIdAt(event.GetPosition());
    int nextFreeId = theMoCapManager.getSkeleton()->getNextFreeId();
    // if the id is smaller than the next free id, a bone (id > 0) or free space (id == -1) was clicked. Set selection to clicked object (unselects if id < 0)
    if (id < nextFreeId)
    {
        selectBone(id);
        // theMoCapManager.selectBone(id);
    }
    else if (id < nextFreeId + 3 && theMoCapManager.getSelectedBoneId() >= 0)
    {
        id -= nextFreeId;
        // TODO(JK#5#): catch mouse move events and process them to rotate bones
        // rotation around x-axis
        if (id == 0)
        {
            // currently fixed to rotations about ~5°, change this later on!
            theMoCapManager.getSelectedBone()->rotate(0.085, 0.0, 0.0);
        }
        // rotation around z-axis
        else if (id == 1)
        {
            theMoCapManager.getSelectedBone()->rotate(0.0, 0.0, 0.085);
        }
        // rotation around y-axis
        else if (id == 2)
        {
            theMoCapManager.getSelectedBone()->rotate(0.0, -0.085, 0.0);
        }
        theMoCapManager.getSkeleton()->update();
        //theMoCapManager.getSkeleton()->setCurrentAsDefault();
    }
    if (id < 0)
    {
        event.Skip();
    }
}

void SkeletonCreatorPanel::selectBone(int id)
{
    if (id < 0)
    {
        return;
    }
    auto it = _treeItemIdFromBoneId.find(id);
    if (it != _treeItemIdFromBoneId.end())
    {
        if (TreeCtrlSkeleton->GetSelection() != it->second)
        {
            TreeCtrlSkeleton->SelectItem(it->second);
            TreeCtrlSkeleton->EnsureVisible(it->second);
        }
    }

    theMoCapManager.selectBone(id);

    TextCtrlName->SetValue(theMoCapManager.getBoneName(id));
    //ChoiceParent->SetSelection(theMoCapManager.getSkeleton()->getBone(id)->getParent());
}


void SkeletonCreatorPanel::OnMouseCaptureLost(wxMouseCaptureLostEvent& event)
{
    if (glCanvas->HasCapture())
    {
        glCanvas->ReleaseMouse();
    }
    event.Skip();
}



void SkeletonCreatorPanel::OnChoiceParentSelect(wxCommandEvent& event)
{
    int boneId = theMoCapManager.getSelectedBoneId();
    if (boneId < 0)
    {
        return;
    }
    Bone* parentBone = static_cast<Bone*>(event.GetClientData());
    if (theMoCapManager.getSkeleton()->reparent(boneId, parentBone->getId(), true))
    {
        theMoCapManager.getSkeleton()->update();
        updateTreeCtrlSkeleton();
        updateBoneInfo();
        glCanvas->Refresh();
    }
    else
    {
        wxString msg;
        if (parentBone == nullptr)
        {
            // this should never happen as only existing bones are displayed, but who knows. This way at least an unknown bug gets correctly reported
            msg << _("Failed to set the selected bone as parent.\nCould not find the new parent bone or the bone does not exist.");
        }
        else
        {
            msg << _("Failed to set ") << parentBone->getName() << _(" as new parent of ") << theMoCapManager.getSelectedBone()->getName();
        }
        wxMessageBox(msg, _("Error"), wxICON_EXCLAMATION);
    }
}

void SkeletonCreatorPanel::OnTextCtrlNameTextEnter(wxCommandEvent& event)
{
    Bone* bone = theMoCapManager.getSelectedBone();
    if (bone != nullptr)
    {
        bone->setName(event.GetString().ToStdString());
    }
    updateTreeCtrlSkeleton();
    updateBoneInfo();
}

void SkeletonCreatorPanel::OnTextCtrlLengthTextEnter(wxCommandEvent& event)
{
    Bone* bone = theMoCapManager.getSelectedBone();
    if (bone != nullptr)
    {
        double length;
        if(event.GetString().ToDouble(&length))
        {
            if (length == 0.0f)
            {
                length = 0.000001f;
            }
            bone->setLength(length);
            bone->update();
        }
        updateBoneInfo();
        glCanvas->Refresh();
    }
}
