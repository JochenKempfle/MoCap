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


#ifndef DATAFLOWPANEL_H
#define DATAFLOWPANEL_H

#ifndef WX_PRECOMP
	//(*HeadersPCH(DataFlowPanel)
	#include <wx/panel.h>
	//*)
#endif
//(*Headers(DataFlowPanel)
//*)

#include "TaskBase.h"
#include "SensorNode.h"

class DataFlowPanel: public wxPanel
{
	public:

		DataFlowPanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~DataFlowPanel();

		//(*Declarations(DataFlowPanel)
		//*)

	protected:

		//(*Identifiers(DataFlowPanel)
		//*)

	private:

		//(*Handlers(DataFlowPanel)
		void OnPaint(wxPaintEvent& event);
		void OnLeftDown(wxMouseEvent& event);
		void OnMouseMove(wxMouseEvent& event);
		void OnLeftUp(wxMouseEvent& event);
		void OnMouseWheel(wxMouseEvent& event);
		void OnResize(wxSizeEvent& event);
		//*)
        void OnMouseCaptureLost(wxMouseCaptureLostEvent& event);

		void drawSensorNode(wxDC* dc, SensorNode* sensor, wxPoint pos);

		void drawTask(wxDC* dc, TaskBase* task, wxPoint pos);
		void drawTaskPreview(wxDC* dc, wxString name, wxPoint pos);

		size_t numSlots() const { return _taskSlots.size(); }
		int getTaskSlot(wxPoint pos, bool checkY = true) const;
		int getSlotPos(int slot) const;

		TaskBase* _selectedTask;

		const wxSize _sizeSensorNode = wxSize(150, 30);
		const wxSize _sizeTask = wxSize(200, 150);
		const wxSize _sizeTaskPreview = wxSize(150, 40);

		const int _widthSensorNodeSlot = _sizeSensorNode.x + 20;
		const int _widthTaskSlot = _sizeTask.x + 20;
		const int _widthTaskPreviewSlot = _sizeTaskPreview.x + 20;

		TaskBase* _taskToAdd;

		wxPoint _mousePos;

		std::map<int, std::vector<TaskBase*> > _taskSlots;

		DECLARE_EVENT_TABLE()
};

#endif
