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
//#include <GL/glu.h>
#include "SkeletonCreatorPanel.h"
#include "FileHandler.h"
#include "MotionSequence.h"
#include "ConstraintDialog.h"

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
const long SkeletonCreatorPanel::ID_BUTTONIMPORT = wxNewId();
const long SkeletonCreatorPanel::ID_BUTTONEXPORT = wxNewId();
const long SkeletonCreatorPanel::ID_BUTTONLOADDEFAULT = wxNewId();
const long SkeletonCreatorPanel::ID_GLCANVAS = wxNewId();
const long SkeletonCreatorPanel::ID_TREECTRLSKELETON = wxNewId();
const long SkeletonCreatorPanel::ID_STATICTEXT1 = wxNewId();
const long SkeletonCreatorPanel::ID_TEXTCTRLNAME = wxNewId();
const long SkeletonCreatorPanel::ID_STATICTEXT2 = wxNewId();
const long SkeletonCreatorPanel::ID_CHOICEPARENT = wxNewId();
const long SkeletonCreatorPanel::ID_STATICTEXT9 = wxNewId();
const long SkeletonCreatorPanel::ID_TEXTCTRLLENGTH = wxNewId();
const long SkeletonCreatorPanel::ID_STATICTEXT14 = wxNewId();
const long SkeletonCreatorPanel::ID_COLOURPICKERCTRL = wxNewId();
const long SkeletonCreatorPanel::ID_STATICTEXT8 = wxNewId();
const long SkeletonCreatorPanel::ID_STATICTEXT7 = wxNewId();
const long SkeletonCreatorPanel::ID_STATICTEXT6 = wxNewId();
const long SkeletonCreatorPanel::ID_STATICTEXT3 = wxNewId();
const long SkeletonCreatorPanel::ID_TEXTCTRLROLL = wxNewId();
const long SkeletonCreatorPanel::ID_TEXTCTRLPITCH = wxNewId();
const long SkeletonCreatorPanel::ID_TEXTCTRLYAW = wxNewId();
const long SkeletonCreatorPanel::ID_BUTTONCONSTRAINT = wxNewId();
const long SkeletonCreatorPanel::ID_BUTTONADDBONE = wxNewId();
const long SkeletonCreatorPanel::ID_BUTTONREMOVEBONE = wxNewId();
const long SkeletonCreatorPanel::ID_STATICTEXT10 = wxNewId();
const long SkeletonCreatorPanel::ID_STATICTEXT11 = wxNewId();
const long SkeletonCreatorPanel::ID_STATICTEXT12 = wxNewId();
const long SkeletonCreatorPanel::ID_STATICTEXT13 = wxNewId();
const long SkeletonCreatorPanel::ID_TEXTCTRLX = wxNewId();
const long SkeletonCreatorPanel::ID_TEXTCTRLY = wxNewId();
const long SkeletonCreatorPanel::ID_TEXTCTRLZ = wxNewId();
//*)

BEGIN_EVENT_TABLE(SkeletonCreatorPanel,wxPanel)
    //(*EventTable(SkeletonCreatorPanel)
    //*)
END_EVENT_TABLE()

SkeletonCreatorPanel::SkeletonCreatorPanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
    //(*Initialize(SkeletonCreatorPanel)
    wxBoxSizer* BoxSizer4;
    wxStaticBoxSizer* StaticBoxSizer2;
    wxBoxSizer* BoxSizer6;
    wxBoxSizer* BoxSizer5;
    wxBoxSizer* BoxSizer10;
    wxBoxSizer* BoxSizer7;
    wxBoxSizer* BoxSizer8;
    wxBoxSizer* BoxSizer2;
    wxBoxSizer* BoxSizer11;
    wxGridSizer* GridSizer1;
    wxBoxSizer* BoxSizer1;
    wxBoxSizer* BoxSizer9;
    wxStaticBoxSizer* StaticBoxSizer1;
    wxBoxSizer* BoxSizer3;
    wxGridSizer* GridSizer2;

    Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    BoxSizer2 = new wxBoxSizer(wxVERTICAL);
    BoxSizer2->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ButtonImport = new wxButton(this, ID_BUTTONIMPORT, _("Import"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONIMPORT"));
    BoxSizer2->Add(ButtonImport, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ButtonExport = new wxButton(this, ID_BUTTONEXPORT, _("Export"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONEXPORT"));
    BoxSizer2->Add(ButtonExport, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ButtonLoadDefault = new wxButton(this, ID_BUTTONLOADDEFAULT, _("Load Default"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONLOADDEFAULT"));
    BoxSizer2->Add(ButtonLoadDefault, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
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
    StaticBoxSizer1 = new wxStaticBoxSizer(wxVERTICAL, this, _("Bone"));
    BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Name"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    BoxSizer6->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrlName = new wxTextCtrl(this, ID_TEXTCTRLNAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, wxDefaultValidator, _T("ID_TEXTCTRLNAME"));
    BoxSizer6->Add(TextCtrlName, 3, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticBoxSizer1->Add(BoxSizer6, 0, wxEXPAND, 5);
    BoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
    StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Parent"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    BoxSizer7->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ChoiceParent = new wxChoice(this, ID_CHOICEPARENT, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICEPARENT"));
    BoxSizer7->Add(ChoiceParent, 3, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticBoxSizer1->Add(BoxSizer7, 0, wxEXPAND, 5);
    BoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
    StaticText9 = new wxStaticText(this, ID_STATICTEXT9, _("Length"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT9"));
    BoxSizer8->Add(StaticText9, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrlLength = new wxTextCtrl(this, ID_TEXTCTRLLENGTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, wxFloatingPointValidator<float>(), _T("ID_TEXTCTRLLENGTH"));
    BoxSizer8->Add(TextCtrlLength, 3, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticBoxSizer1->Add(BoxSizer8, 0, wxEXPAND, 5);
    BoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
    StaticText14 = new wxStaticText(this, ID_STATICTEXT14, _("Color"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT14"));
    BoxSizer10->Add(StaticText14, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ColourPickerCtrl = new wxColourPickerCtrl(this, ID_COLOURPICKERCTRL, wxColour(180,180,180), wxDefaultPosition, wxDefaultSize, wxCLRP_USE_TEXTCTRL, wxDefaultValidator, _T("ID_COLOURPICKERCTRL"));
    BoxSizer10->Add(ColourPickerCtrl, 3, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticBoxSizer1->Add(BoxSizer10, 0, wxEXPAND, 5);
    GridSizer1 = new wxGridSizer(2, 4, 0, 0);
    GridSizer1->Add(-1,-1,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _("Roll"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
    GridSizer1->Add(StaticText8, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _("Pitch"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
    GridSizer1->Add(StaticText7, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("Yaw"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
    GridSizer1->Add(StaticText6, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Orientation"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    GridSizer1->Add(StaticText3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrlRoll = new wxTextCtrl(this, ID_TEXTCTRLROLL, _("0"), wxDefaultPosition, wxSize(60,20), wxTE_PROCESS_ENTER, wxFloatingPointValidator<float>(), _T("ID_TEXTCTRLROLL"));
    GridSizer1->Add(TextCtrlRoll, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrlPitch = new wxTextCtrl(this, ID_TEXTCTRLPITCH, _("0"), wxDefaultPosition, wxSize(60,20), wxTE_PROCESS_ENTER, wxFloatingPointValidator<float>(), _T("ID_TEXTCTRLPITCH"));
    GridSizer1->Add(TextCtrlPitch, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrlYaw = new wxTextCtrl(this, ID_TEXTCTRLYAW, _("0"), wxDefaultPosition, wxSize(60,20), wxTE_PROCESS_ENTER, wxFloatingPointValidator<float>(), _T("ID_TEXTCTRLYAW"));
    GridSizer1->Add(TextCtrlYaw, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticBoxSizer1->Add(GridSizer1, 0, wxEXPAND, 5);
    BoxSizer11 = new wxBoxSizer(wxVERTICAL);
    ButtonConstraint = new wxButton(this, ID_BUTTONCONSTRAINT, _("Set Constraint"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONCONSTRAINT"));
    BoxSizer11->Add(ButtonConstraint, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticBoxSizer1->Add(BoxSizer11, 1, wxEXPAND, 5);
    BoxSizer4->Add(StaticBoxSizer1, 0, wxALL|wxEXPAND, 5);
    BoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
    ButtonAddBone = new wxButton(this, ID_BUTTONADDBONE, _("Add Bone"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONADDBONE"));
    BoxSizer9->Add(ButtonAddBone, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ButtonRemoveBone = new wxButton(this, ID_BUTTONREMOVEBONE, _("Remove Bone"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONREMOVEBONE"));
    BoxSizer9->Add(ButtonRemoveBone, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer4->Add(BoxSizer9, 0, wxALIGN_CENTER_HORIZONTAL, 5);
    StaticBoxSizer2 = new wxStaticBoxSizer(wxVERTICAL, this, _("Skeleton"));
    GridSizer2 = new wxGridSizer(0, 4, 0, 0);
    GridSizer2->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText10 = new wxStaticText(this, ID_STATICTEXT10, _("X"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT10"));
    GridSizer2->Add(StaticText10, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText11 = new wxStaticText(this, ID_STATICTEXT11, _("Y"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT11"));
    GridSizer2->Add(StaticText11, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText12 = new wxStaticText(this, ID_STATICTEXT12, _("Z"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT12"));
    GridSizer2->Add(StaticText12, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText13 = new wxStaticText(this, ID_STATICTEXT13, _("Position"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT13"));
    GridSizer2->Add(StaticText13, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrlX = new wxTextCtrl(this, ID_TEXTCTRLX, _("0"), wxDefaultPosition, wxSize(60,20), wxTE_PROCESS_ENTER, wxFloatingPointValidator<float>(), _T("ID_TEXTCTRLX"));
    GridSizer2->Add(TextCtrlX, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrlY = new wxTextCtrl(this, ID_TEXTCTRLY, _("0"), wxDefaultPosition, wxSize(60,20), wxTE_PROCESS_ENTER, wxFloatingPointValidator<float>(), _T("ID_TEXTCTRLY"));
    GridSizer2->Add(TextCtrlY, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrlZ = new wxTextCtrl(this, ID_TEXTCTRLZ, _("0"), wxDefaultPosition, wxSize(60,20), wxTE_PROCESS_ENTER, wxFloatingPointValidator<float>(), _T("ID_TEXTCTRLZ"));
    GridSizer2->Add(TextCtrlZ, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticBoxSizer2->Add(GridSizer2, 0, wxEXPAND, 5);
    BoxSizer4->Add(StaticBoxSizer2, 0, wxALL|wxEXPAND, 5);
    BoxSizer5 = new wxBoxSizer(wxVERTICAL);
    BoxSizer4->Add(BoxSizer5, 0, wxEXPAND, 5);
    BoxSizer1->Add(BoxSizer4, 0, wxEXPAND, 5);
    SetSizer(BoxSizer1);
    BoxSizer1->Fit(this);
    BoxSizer1->SetSizeHints(this);

    Connect(ID_BUTTONIMPORT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SkeletonCreatorPanel::OnButtonImportClick);
    Connect(ID_BUTTONEXPORT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SkeletonCreatorPanel::OnButtonExportClick);
    Connect(ID_BUTTONLOADDEFAULT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SkeletonCreatorPanel::OnButtonLoadDefaultClick);
    glCanvas->Connect(wxEVT_LEFT_DOWN,(wxObjectEventFunction)&SkeletonCreatorPanel::OnGlCanvasLeftDown,0,this);
    glCanvas->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&SkeletonCreatorPanel::OnglCanvasLeftUp,0,this);
    glCanvas->Connect(wxEVT_MOTION,(wxObjectEventFunction)&SkeletonCreatorPanel::OnglCanvasMouseMove,0,this);
    Connect(ID_TREECTRLSKELETON,wxEVT_COMMAND_TREE_END_LABEL_EDIT,(wxObjectEventFunction)&SkeletonCreatorPanel::OnTreeCtrlSkeletonEndLabelEdit);
    Connect(ID_TREECTRLSKELETON,wxEVT_COMMAND_TREE_DELETE_ITEM,(wxObjectEventFunction)&SkeletonCreatorPanel::OnTreeCtrlSkeletonDeleteItem);
    Connect(ID_TREECTRLSKELETON,wxEVT_COMMAND_TREE_SEL_CHANGED,(wxObjectEventFunction)&SkeletonCreatorPanel::OnTreeCtrlSkeletonSelectionChanged);
    Connect(ID_TREECTRLSKELETON,wxEVT_COMMAND_TREE_KEY_DOWN,(wxObjectEventFunction)&SkeletonCreatorPanel::OnTreeCtrlSkeletonKeyDown);
    Connect(ID_TEXTCTRLNAME,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&SkeletonCreatorPanel::OnTextCtrlNameTextEnter);
    Connect(ID_CHOICEPARENT,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&SkeletonCreatorPanel::OnChoiceParentSelect);
    Connect(ID_TEXTCTRLLENGTH,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&SkeletonCreatorPanel::OnTextCtrlLengthTextEnter);
    Connect(ID_TEXTCTRLROLL,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&SkeletonCreatorPanel::OnTextCtrlOrientationTextEnter);
    Connect(ID_TEXTCTRLPITCH,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&SkeletonCreatorPanel::OnTextCtrlOrientationTextEnter);
    Connect(ID_TEXTCTRLYAW,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&SkeletonCreatorPanel::OnTextCtrlOrientationTextEnter);
    Connect(ID_BUTTONCONSTRAINT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SkeletonCreatorPanel::OnButtonConstraintClick);
    Connect(ID_BUTTONADDBONE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SkeletonCreatorPanel::OnButtonAddBoneClick);
    Connect(ID_BUTTONREMOVEBONE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SkeletonCreatorPanel::OnButtonRemoveBoneClick);
    Connect(ID_TEXTCTRLX,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&SkeletonCreatorPanel::OnTextCtrlPositionTextEnter);
    Connect(ID_TEXTCTRLY,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&SkeletonCreatorPanel::OnTextCtrlPositionTextEnter);
    Connect(ID_TEXTCTRLZ,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&SkeletonCreatorPanel::OnTextCtrlPositionTextEnter);
    //*)
    glCanvas->Connect(wxEVT_MOUSE_CAPTURE_LOST, (wxObjectEventFunction)&SkeletonCreatorPanel::OnMouseCaptureLost,0,this);

    theMoCapManager.getSkeleton()->setToDefault();
	glCanvas->setSkeleton(theMoCapManager.getSkeleton());
	glCanvas->setStyle(DRAW_SPIN_ARROWS | HIGHLIGHT_SELECTED_BONE | DRAW_LOCAL_COORDINATE_SYSTEM);

	static_cast<wxFloatingPointValidator<float>*>(TextCtrlLength->GetValidator())->SetMin(0.0f);
    // _skeleton = theMoCapManager.getSkeleton();
    updateTreeCtrlSkeleton();
    updateBoneInfo();
    // ChoiceParent->SetSelection(ChoiceParent->Append(_("root")));
    _clickedArrow = -1;
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
                selectBone(-1);
                //TreeCtrlSkeleton->Delete(TreeCtrlSkeleton->GetSelection());
                //TreeCtrlSkeleton->DeleteAllItems();
            }
            break;
        default:
            return;
    }
    theMoCapManager.getSkeleton()->update();
    updateTreeCtrlSkeleton();
    updateBoneInfo();
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
        TreeCtrlSkeleton->Refresh();
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

    wxString value;
    Vector3 position = theMoCapManager.getSkeleton()->getPosition();

    value << position.x();
    TextCtrlX->ChangeValue(value);
    value.Clear();

    value << position.y();
    TextCtrlY->ChangeValue(value);
    value.Clear();

    value << position.z();
    TextCtrlZ->ChangeValue(value);
    value.Clear();

    if (bone == nullptr)
    {
        // no bone selected
        TextCtrlName->Clear();
        ChoiceParent->Clear();
        TextCtrlLength->ChangeValue(_("0"));
        TextCtrlYaw->ChangeValue(_("0"));
        TextCtrlPitch->ChangeValue(_("0"));
        TextCtrlRoll->ChangeValue(_("0"));
        return;
    }

    TextCtrlName->ChangeValue(bone->getName());
    ChoiceParent->Clear();

    value << bone->getLength();
    TextCtrlLength->ChangeValue(value);
    value.Clear();

    Vector3 euler = bone->getAbsOrientation().toEuler();

    value << euler.yaw() * 180.0/M_PI;
    TextCtrlYaw->ChangeValue(value);
    value.Clear();

    value << euler.pitch() * 180.0/M_PI;
    TextCtrlPitch->ChangeValue(value);
    value.Clear();

    value << euler.roll() * 180.0/M_PI;
    TextCtrlRoll->ChangeValue(value);
    value.Clear();

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
    // event.Skip();
    int id = glCanvas->getObjectIdAt(event.GetPosition());
    int nextFreeId = theMoCapManager.getSkeleton()->getNextFreeId();
    // if the id is smaller than the next free id, a bone (id > 0) or free space (id == -1) was clicked. Set selection to clicked object (unselects if id < 0)
    if (id < nextFreeId)
    {
        selectBone(id);
        // updateBoneInfo();
        // theMoCapManager.selectBone(id);
    }
    else if (id < nextFreeId + 3 && theMoCapManager.getSelectedBoneId() >= 0)
    {
        _clickedArrow = -1;
        id -= nextFreeId;
        // TODO(JK#5#): catch mouse move events and process them to rotate bones
        // rotation around x-axis
        if (id == 0)
        {
            _clickedArrow = 0;
            // currently fixed to rotations about ~5°, change this later on!
            // theMoCapManager.getSelectedBone()->rotate(0.085, 0.0, 0.0);
        }
        // rotation around z-axis
        else if (id == 1)
        {
            _clickedArrow = 1;
            // theMoCapManager.getSelectedBone()->rotate(0.0, 0.0, 0.085);
        }
        // rotation around y-axis
        else if (id == 2)
        {
            _clickedArrow = 2;
            // theMoCapManager.getSelectedBone()->rotate(0.0, -0.085, 0.0);
        }
        _mousePos = event.GetPosition();
        // theMoCapManager.getSkeleton()->update();
        //theMoCapManager.getSkeleton()->setCurrentAsDefault();
    }
    if (id < 0)
    {
        event.Skip();
    }
}

void SkeletonCreatorPanel::OnglCanvasMouseMove(wxMouseEvent& event)
{
    if (_clickedArrow < 0 || _clickedArrow > 2)
    {
        event.Skip();
        return;
    }
    wxPoint mousePos = event.GetPosition();
    int delta = _mousePos.x - mousePos.x;
    if (_clickedArrow == 0)
    {
        theMoCapManager.getSelectedBone()->rotate(delta * 0.01, 0.0, 0.0);
    }
    // rotation around z-axis
    else if (_clickedArrow == 1)
    {
        theMoCapManager.getSelectedBone()->rotate(0.0, 0.0, delta * 0.01);
    }
    // rotation around y-axis
    else if (_clickedArrow == 2)
    {
        theMoCapManager.getSelectedBone()->rotate(0.0, delta * -0.01, 0.0);
    }
    _mousePos = mousePos;
    theMoCapManager.getSkeleton()->update();
    glCanvas->Refresh();
}

void SkeletonCreatorPanel::OnglCanvasLeftUp(wxMouseEvent& event)
{
    _clickedArrow = -1;
    event.Skip();
    updateBoneInfo();
}

void SkeletonCreatorPanel::selectBone(int id)
{
    if (id < 0)
    {
        theMoCapManager.selectBone(-1);
        TreeCtrlSkeleton->Unselect();
        updateBoneInfo();
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

void SkeletonCreatorPanel::OnButtonImportClick(wxCommandEvent& event)
{
    // show the file dialog
    // TODO(JK#5#): import skeleton corrections
    wxFileDialog* fileDialog = new wxFileDialog(this, _("Import skeleton from file"), _(""), _(""), _("bvh files (*.bvh)|*.bvh"), wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if (fileDialog->ShowModal() == wxID_CANCEL)
    {
        fileDialog->Destroy();
        return;
    }
    SetCursor(wxCURSOR_ARROWWAIT);
    Skeleton* skeleton = FileHandler::readBVHSkeleton(fileDialog->GetPath());
    fileDialog->Destroy();

    SetCursor(wxCURSOR_DEFAULT);

    if (skeleton == nullptr)
    {
        wxString msg;
        msg << _("Error loading the file: ") << fileDialog->GetPath();
        wxMessageBox(msg, _("Error"), wxICON_ERROR);
        return;
    }
    theMoCapManager.setSkeleton(*skeleton);
    delete skeleton;

    selectBone(-1);
    updateBoneInfo();
    updateTreeCtrlSkeleton();
    Refresh();
}

void SkeletonCreatorPanel::OnButtonExportClick(wxCommandEvent& event)
{
    Skeleton* skeleton = theMoCapManager.getSkeleton();
    if (skeleton == nullptr)
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

    // TODO(JK#4#): export skeleton corrections
    MotionSequence sequence;
    sequence.createFromSkeleton(*skeleton);
    FileHandler::writeBVH(path, &sequence);
}

void SkeletonCreatorPanel::OnButtonLoadDefaultClick(wxCommandEvent& event)
{
    theMoCapManager.createAndSetDefaultSkeleton();
    theMoCapManager.getSkeleton()->update();
    selectBone(-1);
    updateTreeCtrlSkeleton();
    updateBoneInfo();
    Refresh();
}

void SkeletonCreatorPanel::OnButtonAddBoneClick(wxCommandEvent& event)
{
    Bone* bone = theMoCapManager.getSelectedBone();
    int id;
    Bone newBone;
    newBone.setName("new bone");
    newBone.setLength(0.5f);
    if (bone != nullptr)
    {
        id = theMoCapManager.getSkeleton()->createBone(newBone, bone->getId());
    }
    else
    {
        id = theMoCapManager.getSkeleton()->createBone(newBone, theMoCapManager.getSkeleton()->getRootId());
    }
    theMoCapManager.getSkeleton()->update();
    selectBone(id);
    updateTreeCtrlSkeleton();
    updateBoneInfo();
    Refresh();
}

void SkeletonCreatorPanel::OnButtonRemoveBoneClick(wxCommandEvent& event)
{
    int boneId = theMoCapManager.getSelectedBoneId();

    theMoCapManager.getSkeleton()->eraseBone(boneId, false);
    selectBone(-1);

    theMoCapManager.getSkeleton()->update();
    updateTreeCtrlSkeleton();
    updateBoneInfo();
    Refresh();
}

void SkeletonCreatorPanel::OnTextCtrlOrientationTextEnter(wxCommandEvent& event)
{
    Bone* bone = theMoCapManager.getSelectedBone();
    if (bone == nullptr)
    {
        return;
    }
    double roll, pitch, yaw;
    TextCtrlRoll->GetValue().ToDouble(&roll);
    TextCtrlPitch->GetValue().ToDouble(&pitch);
    TextCtrlYaw->GetValue().ToDouble(&yaw);
    bone->setRelOrientation(Quaternion(roll * M_PI/180.0, pitch * M_PI/180.0, yaw * M_PI/180.0));
    theMoCapManager.getSkeleton()->update();
    updateBoneInfo();
    glCanvas->Refresh();
}

void SkeletonCreatorPanel::OnTextCtrlPositionTextEnter(wxCommandEvent& event)
{
    double x, y, z;
    TextCtrlX->GetValue().ToDouble(&x);
    TextCtrlY->GetValue().ToDouble(&y);
    TextCtrlZ->GetValue().ToDouble(&z);
    theMoCapManager.getSkeleton()->setPosition(x, y, z);
    updateBoneInfo();
    glCanvas->Refresh();
}

void SkeletonCreatorPanel::OnButtonConstraintClick(wxCommandEvent& event)
{
    ConstraintDialog dialog(this);
    if (dialog.ShowModal() == wxID_OK)
    {

    }
}



