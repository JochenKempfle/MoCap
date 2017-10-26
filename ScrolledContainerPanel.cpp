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
#include "ScrolledContainerPanel.h"

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(ScrolledContainerPanel)
	#include <wx/intl.h>
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(ScrolledContainerPanel)
//*)

//(*IdInit(ScrolledContainerPanel)
const long ScrolledContainerPanel::ID_SCROLLEDWINDOW = wxNewId();
//*)

BEGIN_EVENT_TABLE(ScrolledContainerPanel,wxPanel)
	//(*EventTable(ScrolledContainerPanel)
	//*)
END_EVENT_TABLE()

ScrolledContainerPanel::ScrolledContainerPanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(ScrolledContainerPanel)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(87,87,87));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	ScrolledWindow = new wxScrolledWindow(this, ID_SCROLLEDWINDOW, wxDefaultPosition, wxDefaultSize, wxVSCROLL|wxALWAYS_SHOW_SB, _T("ID_SCROLLEDWINDOW"));
	BoxSizer2 = new wxBoxSizer(wxVERTICAL);
	ScrolledWindow->SetSizer(BoxSizer2);
	BoxSizer2->Fit(ScrolledWindow);
	BoxSizer2->SetSizeHints(ScrolledWindow);
	BoxSizer1->Add(ScrolledWindow, 1, wxEXPAND, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	//*)
	ScrolledWindow->SetScrollRate(0, 10);
}

ScrolledContainerPanel::~ScrolledContainerPanel()
{
	//(*Destroy(ScrolledContainerPanel)
	//*)
}

void ScrolledContainerPanel::add(wxPanel* panel)
{
    BoxSizer2->Add(panel, 0, wxALL|wxEXPAND, 5);
    layout();
}

void ScrolledContainerPanel::add(std::vector<wxPanel*> panels)
{
    for (size_t i = 0; i < panels.size(); ++i)
    {
        BoxSizer2->Add(panels[i], 0, wxALL|wxEXPAND, 5);
    }
    layout();
}

void ScrolledContainerPanel::remove(wxWindow* window)
{
    BoxSizer2->Detach(window);
    // window->Destroy();
    // BoxSizer2->Layout();
}

void ScrolledContainerPanel::layout()
{
	BoxSizer2->Fit(ScrolledWindow);
	BoxSizer2->SetSizeHints(ScrolledWindow);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Refresh();
}
