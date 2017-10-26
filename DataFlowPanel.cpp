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
#include <wx/dcbuffer.h>
#include "DataFlowPanel.h"
#include "MoCapManager.h"
#include "SensorManager.h"

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(DataFlowPanel)
	#include <wx/intl.h>
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(DataFlowPanel)
//*)

//(*IdInit(DataFlowPanel)
//*)

BEGIN_EVENT_TABLE(DataFlowPanel,wxPanel)
	//(*EventTable(DataFlowPanel)
	//*)
END_EVENT_TABLE()

DataFlowPanel::DataFlowPanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
    // important for double buffered paint dc
    SetBackgroundStyle(wxBG_STYLE_PAINT);
	//(*Initialize(DataFlowPanel)
	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	SetBackgroundColour(wxColour(87,87,87));

	Connect(wxEVT_PAINT,(wxObjectEventFunction)&DataFlowPanel::OnPaint);
	Connect(wxEVT_LEFT_DOWN,(wxObjectEventFunction)&DataFlowPanel::OnLeftDown);
	Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&DataFlowPanel::OnLeftUp);
	Connect(wxEVT_MOTION,(wxObjectEventFunction)&DataFlowPanel::OnMouseMove);
	Connect(wxEVT_MOUSEWHEEL,(wxObjectEventFunction)&DataFlowPanel::OnMouseWheel);
	Connect(wxEVT_SIZE,(wxObjectEventFunction)&DataFlowPanel::OnResize);
	//*)
	Connect(wxEVT_MOUSE_CAPTURE_LOST, (wxObjectEventFunction)&DataFlowPanel::OnMouseCaptureLost);

	_selectedTask = nullptr;
	_taskToAdd = nullptr;
}

DataFlowPanel::~DataFlowPanel()
{
	//(*Destroy(DataFlowPanel)
	//*)
}


void DataFlowPanel::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    if (!dc.IsOk())
    {
        return;
    }
    dc.Clear();

    wxPoint drawPos(0, 0);
    wxSize size = GetSize();

    wxPen pen(wxColour(0,0,0), 1);
    wxBrush brush(wxColour(255, 255, 255));

    dc.SetPen(pen);
    dc.SetBrush(brush);

    // set dc's font size
    wxFont font = dc.GetFont();
    font.SetPointSize(10);
    dc.SetFont(font);

    // highlight current sensor node slot when dragging a new task
    if (_taskToAdd != nullptr)
    {
        int slot = getTaskSlot(_mousePos);
        if (slot >= 0)
        {
            dc.DrawRectangle(wxPoint(getSlotPos(slot), 0), wxSize(_widthTaskSlot, size.y));
        }
    }


    // draw sensor nodes
    drawPos.x = 10;
    drawPos.y = 10;
    std::vector<SensorNode*> sensors = theSensorManager.getSensors();
    for (size_t i = 0; i < sensors.size(); ++i)
    {
        drawSensorNode(&dc, sensors[i], drawPos);
        drawPos.y += _sizeSensorNode.y + 10;
    }

    // draw active tasks
    for (auto it = _taskSlots.begin(); it != _taskSlots.end(); ++it)
    {
        drawPos.x = getSlotPos(it->first) + 10;
        drawPos.y = 10;

        // stop drawing tasks when out of screen
        if (drawPos.x > size.x - _widthTaskPreviewSlot)
        {
            break;
        }

        for (size_t i = 0; i < it->second.size(); ++i)
        {
            drawTask(&dc, it->second[i], drawPos);
            drawPos.y += _sizeTask.y + 10;
            // stop drawing tasks when out of screen
            if (drawPos.y > size.y)
            {
                break;
            }
        }
    }
/*
    // draw wiring
    for (auto it = _taskSlots.begin(); it != _taskSlots.end(); ++it)
    {
        for (size_t i = 0; i < it->second.size(); ++i)
        {
            it->second[i]->getChannels();
        }
    }
*/
    // draw task selection slot
    dc.DrawRectangle(wxPoint(size.x - _widthTaskPreviewSlot, 0), wxSize(_widthTaskPreviewSlot, size.y));

    // draw available task previews for creating a task
    drawPos.x = size.x - _widthTaskPreviewSlot + 10;
    drawPos.y = 10;
    for (size_t i = 0; i < TaskFactory::getNumTasks(); ++i)
    {
        drawTaskPreview(&dc, TaskFactory::getTaskName(i), drawPos);
        drawPos.y += _sizeTaskPreview.y + 10;
    }

    // draw task to add above all other parts
    if (_taskToAdd != nullptr)
    {
        drawTask(&dc, _taskToAdd, _mousePos);
    }
}

void DataFlowPanel::drawSensorNode(wxDC* dc, SensorNode* sensor, wxPoint pos)
{
    dc->SetPen(wxPen(wxColour(0, 0, 0), 1));
    dc->DrawRectangle(pos, _sizeSensorNode);

    wxString name = sensor->getName();
    wxSize textSize = dc->GetTextExtent(name);
    while (dc->GetTextExtent(name).x > _sizeSensorNode.x - 10)
    {
        name.RemoveLast();
    }

    dc->DrawText(name, pos + wxPoint(5, 0));
}

void DataFlowPanel::drawTask(wxDC* dc, TaskBase* task, wxPoint pos)
{
    dc->SetPen(wxPen(wxColour(0, 0, 0)));

    wxFont font = dc->GetFont();
    font.SetPointSize(8);
    dc->SetFont(font);

    // get background colour of task (depends on being selected)
    wxColour bgColour;
    if (task == _selectedTask)
    {
        bgColour = wxColor(200, 200, 100);
    }
    else
    {
        bgColour = wxColor(200, 200, 200);
    }
    dc->SetBrush(wxBrush(bgColour));

    // draw task background and rect around it
    dc->DrawRectangle(pos, _sizeTask);

    std::vector<std::string> typeNames = task->getOutputDataType().getTypeNames();

    for (size_t i = 0; i < typeNames.size(); ++i)
    {
        pos.y += 20;
        dc->DrawRectangle(pos, wxSize(10, 10));
    }

    // draw task name
    int textExtent = dc->GetTextExtent(task->getName()).x;
    if (textExtent + 40 < _sizeTask.x)
    {
        wxPoint textPosition(pos.x + 20, pos.y + 10);

        dc->DrawText(task->getName(), textPosition);
    }

    // draw task info
    textExtent = dc->GetTextExtent(task->getInfo()).x;
    if (textExtent + 40 < _sizeTask.x)
    {
        wxPoint textPosition(pos.x + 20, pos.y + 30);

        dc->DrawText(task->getInfo(), textPosition);
    }
}

void DataFlowPanel::drawTaskPreview(wxDC* dc, wxString name, wxPoint pos)
{
    dc->SetPen(wxPen(wxColour(0, 0, 0), 1));
    dc->DrawRectangle(pos, _sizeTaskPreview);

    wxSize textSize = dc->GetTextExtent(name);
    while (dc->GetTextExtent(name).x > _sizeTaskPreview.x - 10)
    {
        name.RemoveLast();
    }

    dc->DrawText(name, pos + wxPoint(5, 0));
}

int DataFlowPanel::getTaskSlot(wxPoint pos, bool checkY) const
{
    wxSize size = GetSize();
    if (pos.x > size.x - _widthTaskPreviewSlot || pos.x < _widthSensorNodeSlot)
    {
        return -1;
    }
    else if (checkY && (pos.y < 0 || pos.y > size.y))
    {
        return -1;
    }
    return (pos.x - _widthSensorNodeSlot) / int(_widthTaskSlot);
}

int DataFlowPanel::getSlotPos(int slot) const
{
    return _widthSensorNodeSlot + slot * _widthTaskSlot;
}

void DataFlowPanel::OnLeftDown(wxMouseEvent& event)
{
    wxPoint pos = event.GetPosition();
    wxSize size = GetSize();

    if (pos.x < _widthSensorNodeSlot)
    {
        // inside sensor slot
    }
    else if (pos.x > size.x - _widthTaskPreviewSlot)
    {
        // inside task preview region
        pos.x -= (size.x - _widthTaskPreviewSlot);
        //  check if a task preview was clicked
        if (pos.x > 10 && pos.x < _widthSensorNodeSlot - 10)
        {
            if (pos.y % (_sizeTaskPreview.y + 10) > 10)
            {
                int index = pos.y / (_sizeTaskPreview.y + 10);
                if (index < int(TaskFactory::getNumTasks()))
                {
                    _taskToAdd = TaskFactory::createTask(index);
                }
            }
        }
        CaptureMouse();
    }
    else
    {
        // inside task slot -> check within which slot
        int slot = (pos.x - _widthSensorNodeSlot) / _widthTaskSlot;
    }
}

void DataFlowPanel::OnMouseMove(wxMouseEvent& event)
{
    if (_taskToAdd != nullptr)
    {
        _mousePos = event.GetPosition();
        Refresh();
    }
}

void DataFlowPanel::OnLeftUp(wxMouseEvent& event)
{
    if (HasCapture())
    {
        ReleaseMouse();
    }
    if (_taskToAdd != nullptr)
    {
        wxPoint pos = event.GetPosition();

        int slot = getTaskSlot(pos);
        if (slot >= 0)
        {
            _taskSlots[slot].push_back(_taskToAdd);
        }
        else
        {
            delete _taskToAdd;
        }
        _taskToAdd = nullptr;
        Refresh();
    }
}

void DataFlowPanel::OnMouseWheel(wxMouseEvent& event)
{
}

void DataFlowPanel::OnResize(wxSizeEvent& event)
{
    Refresh();
}

void DataFlowPanel::OnMouseCaptureLost(wxMouseCaptureLostEvent& event)
{
    if (HasCapture())
    {
        ReleaseMouse();
    }
    if (_taskToAdd != nullptr)
    {
        delete _taskToAdd;
        _taskToAdd = nullptr;
    }
}

