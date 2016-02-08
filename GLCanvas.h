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


#ifndef GLCANVAS_H
#define GLCANVAS_H

#include <wx/glcanvas.h>
#include "Skeleton.h"
#include "Vector3.h"


enum RenderStyle
{
    STANDARD = 0,
    HIGHLIGHT_SELECTED_BONE = 1,
    DRAW_GRID = 2,
    DRAW_LOCAL_COORDINATE_SYSTEM = 4,
    DRAW_SPIN_ARROWS = 8,
    SELECTION_MODE = 16
};


class GLCanvas : public wxGLCanvas
{
  public:
    GLCanvas(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition,
             const wxSize &size = wxDefaultSize, int style = 0, const wxString &name = _(""), int* attribList = NULL);

    virtual ~GLCanvas();

    void Render() const;
    void drawSpinArrows(Vector3 pos, Vector3 dir, Vector3 up, Vector3 right) const;
    void InitGL();
    int getObjectIdAt(const wxPoint& pos);
    void setSkeleton(Skeleton* skeleton) { _skeleton = skeleton; }

    void setRenderStyle(unsigned char flags) { _renderStyle = flags; }
    void setRenderStyleFlag(unsigned char flags) { _renderStyle |= flags; }
    void unsetRenderStyleFlag(unsigned char flags) { _renderStyle &= ~flags; }
    unsigned char getRenderStyle() const { return _renderStyle; }

  private:
    bool init;
    wxGLContext* _GLRC;

    void OnPaint(wxPaintEvent &event);
    void OnSize(wxSizeEvent &event);
    void OnEraseBackground(wxEraseEvent &event);
    void OnEnterWindow(wxMouseEvent &event);
    void OnLeftDown(wxMouseEvent &event);
    void OnLeftUp(wxMouseEvent &event);
    void OnRightDown(wxMouseEvent &event);
    void OnRightUp(wxMouseEvent &event);
    void OnMouseMove(wxMouseEvent &event);
    void OnMouseWheel(wxMouseEvent &event);
    void OnMouseCaptureLost(wxMouseCaptureLostEvent& event);

    wxPoint _mousePosAtClick;
    bool _lClicked;
    bool _rClicked;
    float _xRotation;
    float _yRotation;
    const float _cameraSpeed = 0.004f;
    Vector3 _cameraPosition;
    Vector3 _cameraFront;
    Vector3 _cameraUp;

    Skeleton* _skeleton;

    unsigned char _renderStyle;

	DECLARE_EVENT_TABLE()
};

#endif // GLCANVAS_H
