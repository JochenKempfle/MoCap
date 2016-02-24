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
#include "GLCanvas.h"
#include "MoCapManager.h"

#if defined(__WXMAC__) || defined(__WXCOCOA__)
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#else
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif


#ifndef WX_PRECOMP
	//InternalHeadersPCH(GLCanvas)
	#include <wx/wx.h>
	//
#endif

BEGIN_EVENT_TABLE(GLCanvas, wxGLCanvas)
	//EventTable(GLCanvas)
	//
END_EVENT_TABLE()

// Constructor to initialize the canvas.
GLCanvas::GLCanvas(wxWindow* parent, wxWindowID id, const wxPoint &pos, const wxSize &size, int style, const wxString &name, int* attribList)
        : wxGLCanvas(parent, id, attribList, pos, size, style, name)
{
    _GLRC = new wxGLContext(this);
    Connect(wxEVT_SIZE, (wxObjectEventFunction)&GLCanvas::OnSize);
    Connect(wxEVT_PAINT,(wxObjectEventFunction)&GLCanvas::OnPaint);
	Connect(wxEVT_LEFT_DOWN,(wxObjectEventFunction)&GLCanvas::OnLeftDown);
	Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&GLCanvas::OnLeftUp);
	Connect(wxEVT_RIGHT_DOWN,(wxObjectEventFunction)&GLCanvas::OnRightDown);
	Connect(wxEVT_RIGHT_UP,(wxObjectEventFunction)&GLCanvas::OnRightUp);
	Connect(wxEVT_MOTION,(wxObjectEventFunction)&GLCanvas::OnMouseMove);
	Connect(wxEVT_MOUSEWHEEL,(wxObjectEventFunction)&GLCanvas::OnMouseWheel);
	Connect(wxEVT_MOUSE_CAPTURE_LOST, (wxObjectEventFunction)&GLCanvas::OnMouseCaptureLost);
    InitGL();
    _xRotation = 0.0f;
    _yRotation = M_PI;
    _cameraPosition = Vector3(0.0f, 1.0f, 3.0f);
    _cameraFront = Vector3(0.0f, 0.0f, -1.0f);
    _cameraUp = Vector3(0.0f, 1.0f, 0.0f);
    _lClicked = false;
    _rClicked = false;
    _skeleton = nullptr;
    _style = STANDARD;
	SetFocus();
	// _image.loadImage(_("C:/Users/Jochen/Desktop/Katana.png"));
}

GLCanvas::~GLCanvas()
{
    delete _GLRC;
}

void GLCanvas::OnPaint(wxPaintEvent &event)
{
	wxPaintDC dc(this);

	if (!_GLRC) return;

	SetCurrent(*_GLRC);

	if (_style & SINGLE_SENSOR_MODE)
    {
        renderSingleSensor();
    }
    else
    {
        renderSkeleton();
    }
	// _image.render();
	// glFlush();
	SwapBuffers();
	event.Skip();
}

void GLCanvas::OnSize(wxSizeEvent &event)
{
	// this is also necessary to update the context on some platforms - no longer =)
	// wxGLCanvas::OnSize(event);

	// set GL viewport
	wxSize size = event.GetSize();
	if (_GLRC)
	{
		SetCurrent(*_GLRC);
		glViewport(0, 0, GLint(size.x), GLint(size.y));
	}
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, double(size.x) / double(size.y), 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

void GLCanvas::OnEraseBackground(wxEraseEvent &event)
{
    // consume the event to avoid flashing effects
	event.Skip();
}

void GLCanvas::OnEnterWindow(wxMouseEvent &event)
{
	SetFocus();
	event.Skip();
}


void GLCanvas::OnLeftDown(wxMouseEvent &event)
{
    SetFocus();
    if (!HasCapture())
    {
        CaptureMouse();
    }
    _lClicked = true;
    _mousePosAtClick = event.GetPosition();

    SetCursor(wxCURSOR_BLANK);

    event.Skip();
}

void GLCanvas::OnLeftUp(wxMouseEvent &event)
{
    if (HasCapture())
    {
        ReleaseMouse();
    }
    _lClicked = false;
    SetCursor(wxCURSOR_ARROW);
    Refresh();
}

void GLCanvas::OnRightDown(wxMouseEvent &event)
{
    SetFocus();
    if (!HasCapture())
    {
        CaptureMouse();
    }
    _rClicked = true;
    _mousePosAtClick = event.GetPosition();

    SetCursor(wxCURSOR_BLANK);

    event.Skip();
}

void GLCanvas::OnRightUp(wxMouseEvent &event)
{
    if (HasCapture())
    {
        ReleaseMouse();
    }
    _rClicked = false;
    SetCursor(wxCURSOR_ARROW);
    Refresh();
}

void GLCanvas::OnMouseMove(wxMouseEvent &event)
{
    if (_style & SINGLE_SENSOR_MODE)
    {
        return;
    }
    if (_rClicked)
    {
        wxPoint delta = event.GetPosition() - _mousePosAtClick;
        _xRotation -= 0.1f * M_PI/180.0f * float(delta.y);
        _yRotation -= 0.1f * M_PI/180.0f * float(delta.x);
        _cameraFront = Vector3(cos(_xRotation) * sin(_yRotation), sin(_xRotation), cos(_xRotation) * cos(_yRotation)).normalized();
        _cameraUp = Vector3(sin(_yRotation - M_PI/2.0f), 0.0f, cos(_yRotation - M_PI/2.0f)).cross(_cameraFront).normalized();
        //_mousePosAtClick = event.GetPosition();
        //wxPoint cursorPos = ClientToScreen(_mousePosAtClick);
        WarpPointer(_mousePosAtClick.x, _mousePosAtClick.y);
        // SetCursorPos(cursorPos.x, cursorPos.y);
        Refresh();
    }
    else if (_lClicked)
    {
        wxPoint delta = event.GetPosition() - _mousePosAtClick;
        _cameraPosition -= _cameraSpeed * float(delta.x) * _cameraFront.cross(_cameraUp).normalized();
        _cameraPosition += _cameraSpeed * float(delta.y) * _cameraUp;
        //_cameraPosition.y() += _cameraSpeed * float(delta.y);
        // _mousePosAtClick = event.GetPosition();
        WarpPointer(_mousePosAtClick.x, _mousePosAtClick.y);
        Refresh();
    }
}

void GLCanvas::OnMouseWheel(wxMouseEvent &event)
{
    _cameraPosition += 30.0f * _cameraSpeed * _cameraFront * float(event.GetWheelRotation()/event.GetWheelDelta());

    if (_style & SINGLE_SENSOR_MODE)
    {
        if (_cameraPosition.z() <= 1.0f)
        {
            _cameraPosition.z() = 1.0f;
        }
    }

    Refresh();
}

void GLCanvas::OnMouseCaptureLost(wxMouseCaptureLostEvent& event)
{
    if (HasCapture())
    {
        ReleaseMouse();
    }
    _lClicked = false;
    _rClicked = false;
    SetCursor(wxCURSOR_ARROW);
    Refresh();
}

void GLCanvas::renderSkeleton() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

    Vector3 lookAt = _cameraPosition + _cameraFront;
    gluLookAt(_cameraPosition.x(), _cameraPosition.y(), _cameraPosition.z(),
              lookAt.x()         , lookAt.y()         , lookAt.z(),
              _cameraUp.x()      , _cameraUp.y()      , _cameraUp.z());

    if (_skeleton == nullptr)
    {
        return;
    }

    for (auto it = _skeleton->beginBones(); it != _skeleton->endBones(); ++it)
    {
        Bone bone = *(it->second);

        int boneId = bone.getId();

        // don't draw any bone without valid id
        if (boneId < 0)
        {
            continue;
        }

        GLubyte red[4] = {255, 0, 0, 255};
        GLubyte green[4] = {0, 255, 0, 255};
        GLubyte blue[4] = {0, 0, 255, 255};
        GLubyte black[4] = {0, 0, 0, 255};

        GLubyte pointColor[4] = {5, 15, 160, 255};
        GLubyte boneStandardColor1[4] = {128, 128, 128, 255};
        GLubyte boneStandardColor2[4] = {180, 180, 180, 255};
        GLubyte boneHighlightedColor1[4] = {100, 200, 100, 255};
        GLubyte boneHighlightedColor2[4] = {160, 255, 160, 255};

        // get the bone id as color for SELECTION_MODE
        GLubyte boneIdColor[4] = {GLubyte((boneId >> 8) & 255), GLubyte((boneId >> 8) & 255), GLubyte(boneId & 255), 255};

        const GLubyte* boneColor1 = boneStandardColor1;
        const GLubyte* boneColor2 = boneStandardColor2;

        if (_style & SELECTION_MODE)
        {
            boneColor1 = boneIdColor;
            boneColor2 = boneIdColor;
        }
        else if (_style & HIGHLIGHT_SELECTED_BONE && bone.getId() == _skeleton->getSelectedBoneId())
        {
            boneColor1 = boneHighlightedColor1;
            boneColor2 = boneHighlightedColor2;
        }

        Vector3 startPos = bone.getStartPos();
        Vector3 endPos = bone.getEndPos();

        float length = bone.getLength();

        Vector3 dir = bone.getDirection();
        Vector3 up = bone.getUpDirection();
        Vector3 right = bone.getRightDirection();

        Vector3 upPoint = startPos + up*length*0.1 + dir*length*0.1;
        Vector3 downPoint = startPos - up*length*0.1 + dir*length*0.1;
        Vector3 rightPoint = startPos + right*length*0.1 + dir*length*0.1;
        Vector3 leftPoint = startPos - right*length*0.1 + dir*length*0.1;

        if (!(_style & SELECTION_MODE))
        {
            //set point size to 10 pixels
            glPointSize(10.0f);
            glColor4ubv(pointColor);

            // TODO(JK#9#): maybe don't draw points for bones (or add render style flag)
            glBegin(GL_POINTS);
                glVertex3f(endPos.x(), endPos.y(), endPos.z());
            glEnd();
        }

        // draw local coordinate system
        if (_style & DRAW_LOCAL_COORDINATE_SYSTEM)
        {
            glBegin(GL_LINES);
                glColor4ubv(red);
                // glVertex3f(startPos.x(), startPos.y(), startPos.z());
                glVertex3f(endPos.x(), endPos.y(), endPos.z());
                glVertex3f(endPos.x() + dir.x()*0.1, endPos.y() + dir.y()*0.1, endPos.z() + dir.z()*0.1);

                glColor4ubv(green);

                glVertex3f(endPos.x(), endPos.y(), endPos.z());
                glVertex3f(endPos.x() + up.x()*0.1, endPos.y() + up.y()*0.1, endPos.z() + up.z()*0.1);

                glColor4ubv(blue);

                glVertex3f(endPos.x(), endPos.y(), endPos.z());
                glVertex3f(endPos.x() + right.x()*0.1, endPos.y() + right.y()*0.1, endPos.z() + right.z()*0.1);
            glEnd();
        }

        // draw bone
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glColor4ubv(boneColor1);

        glBegin(GL_TRIANGLE_FAN);
            glVertex3f(startPos.x(), startPos.y(), startPos.z());
            glVertex3f(upPoint.x(), upPoint.y(), upPoint.z());
            glVertex3f(rightPoint.x(), rightPoint.y(), rightPoint.z());
            glVertex3f(downPoint.x(), downPoint.y(), downPoint.z());
            glVertex3f(leftPoint.x(), leftPoint.y(), leftPoint.z());
            glVertex3f(upPoint.x(), upPoint.y(), upPoint.z());
        glEnd();


        glColor4ubv(boneColor2);

        glBegin(GL_TRIANGLE_FAN);
            glVertex3f(endPos.x(), endPos.y(), endPos.z());
            glVertex3f(upPoint.x(), upPoint.y(), upPoint.z());
            glVertex3f(rightPoint.x(), rightPoint.y(), rightPoint.z());
            glVertex3f(downPoint.x(), downPoint.y(), downPoint.z());
            glVertex3f(leftPoint.x(), leftPoint.y(), leftPoint.z());
            glVertex3f(upPoint.x(), upPoint.y(), upPoint.z());
        glEnd();

        if (!(_style & SELECTION_MODE))
        {
            // draw black mesh lines around bones
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glColor4ubv(black);

            glBegin(GL_TRIANGLE_FAN);
                glVertex3f(startPos.x(), startPos.y(), startPos.z());
                glVertex3f(upPoint.x(), upPoint.y(), upPoint.z());
                glVertex3f(rightPoint.x(), rightPoint.y(), rightPoint.z());
                glVertex3f(downPoint.x(), downPoint.y(), downPoint.z());
                glVertex3f(leftPoint.x(), leftPoint.y(), leftPoint.z());
                glVertex3f(upPoint.x(), upPoint.y(), upPoint.z());
            glEnd();

            glBegin(GL_TRIANGLE_FAN);
                glVertex3f(endPos.x(), endPos.y(), endPos.z());
                glVertex3f(upPoint.x(), upPoint.y(), upPoint.z());
                glVertex3f(rightPoint.x(), rightPoint.y(), rightPoint.z());
                glVertex3f(downPoint.x(), downPoint.y(), downPoint.z());
                glVertex3f(leftPoint.x(), leftPoint.y(), leftPoint.z());
                glVertex3f(upPoint.x(), upPoint.y(), upPoint.z());
            glEnd();
        }

        if (_style & DRAW_SPIN_ARROWS && bone.getId() == _skeleton->getSelectedBoneId())
        {
            drawSpinArrows(endPos - 0.2 * bone.getLength() * dir, dir, up, right);
        }
    }
}

void GLCanvas::drawSpinArrows(Vector3 pos, Vector3 dir, Vector3 up, Vector3 right) const
{
    float length = 0.03f;
    float offset = 0.05f;

    GLubyte red[4] = {255, 0, 0, 255};
    GLubyte green[4] = {0, 255, 0, 255};
    GLubyte blue[4] = {0, 0, 255, 255};
    GLubyte black[4] = {0, 0, 0, 255};

    // set arroe ids to next available free ids
    int idArrowDir = _skeleton->getNextFreeId();
    int idArrowUp = _skeleton->getNextFreeId() + 1;
    int idArrowRight = _skeleton->getNextFreeId() + 2;

    GLubyte idArrowDirColor[4] = {GLubyte((idArrowDir >> 8) & 255), GLubyte((idArrowDir >> 8) & 255), GLubyte(idArrowDir & 255), 255};
    GLubyte idArrowUpColor[4] = {GLubyte((idArrowUp >> 8) & 255), GLubyte((idArrowUp >> 8) & 255), GLubyte(idArrowUp & 255), 255};
    GLubyte idArrowRightColor[4] = {GLubyte((idArrowRight >> 8) & 255), GLubyte((idArrowRight >> 8) & 255), GLubyte(idArrowRight & 255), 255};

    const GLubyte* arrowDirColor = red;
    const GLubyte* arrowUpColor = green;
    const GLubyte* arrowRightColor = blue;

    // base points of the arrow pyramids
    Vector3 basePointUp = pos + up * offset;
    Vector3 basePointRight = pos + right * offset;
    Vector3 basePointDown = pos - up * offset;
    Vector3 basePointLeft = pos - right * offset;

    dir *= length;
    up *= length;
    right *= length;

    Vector3 dirHalf = dir * 0.5;
    Vector3 rightHalf = right * 0.5;

    // base vertices of the circle like arrow
    Vector3 upperCircle[4];
    upperCircle[0] = basePointDown;
    upperCircle[1] = basePointDown - right;
    upperCircle[2] = basePointLeft - up;
    upperCircle[3] = basePointLeft;

    Vector3 lowerCircle[4];
    lowerCircle[0] = basePointDown - up;
    lowerCircle[1] = basePointDown - right*1.4 - up;
    lowerCircle[2] = basePointLeft - right - up*1.4;
    lowerCircle[3] = basePointLeft - right;

    // the arrow rendering is done twice, one iteration for filling color, one for having black corner lines
    int numIterations = 2;
    if (_style & SELECTION_MODE)
    {
        // do not draw the corner lines in selection mode
        numIterations = 1;
        // draw the arrows with their id color
        arrowDirColor = idArrowDirColor;
        arrowUpColor = idArrowUpColor;
        arrowRightColor = idArrowRightColor;
    }
    // draw vertices twice, one run with filling and one with black lines
    for (int j = 0; j < numIterations; ++j)
    {
        if (j == 0)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glColor4f(0.0, 0.0, 0.0, 1.0);
            arrowDirColor = arrowUpColor = arrowRightColor = black;
        }

        if (j == 0)
        {
            glColor4ubv(arrowUpColor);
        }
        glColor4ubv(arrowUpColor);

        // arrow pointing upwards
        glBegin(GL_TRIANGLE_FAN);
            glVertex3f(basePointUp.x() + up.x()*2.0, basePointUp.y() + up.y()*2.0, basePointUp.z() + up.z()*2.0);
            glVertex3f(basePointUp.x() + dir.x(), basePointUp.y() + dir.y(), basePointUp.z() + dir.z());
            glVertex3f(basePointUp.x() + right.x(), basePointUp.y() + right.y(), basePointUp.z() + right.z());
            glVertex3f(basePointUp.x() - dir.x(), basePointUp.y() - dir.y(), basePointUp.z() - dir.z());
            glVertex3f(basePointUp.x() - right.x(), basePointUp.y() - right.y(), basePointUp.z() - right.z());
            glVertex3f(basePointUp.x() + dir.x(), basePointUp.y() + dir.y(), basePointUp.z() + dir.z());
        glEnd();

        // arrow pointing downwards
    /*    glBegin(GL_TRIANGLE_FAN);
            glVertex3f(basePointDown.x() - up.x(), basePointDown.y() - up.y(), basePointDown.z() - up.z());
            glVertex3f(basePointDown.x() + dir.x(), basePointDown.y() + dir.y(), basePointDown.z() + dir.z());
            glVertex3f(basePointDown.x() + right.x(), basePointDown.y() + right.y(), basePointDown.z() + right.z());
            glVertex3f(basePointDown.x() - dir.x(), basePointDown.y() - dir.y(), basePointDown.z() - dir.z());
            glVertex3f(basePointDown.x() - right.x(), basePointDown.y() - right.y(), basePointDown.z() - right.z());
            glVertex3f(basePointDown.x() + dir.x(), basePointDown.y() + dir.y(), basePointDown.z() + dir.z());
        glEnd();
    */
        if (j == 0)
        {
            glColor4f(0.0, 0.0, 1.0, 1.0);
        }
        glColor4ubv(arrowRightColor);

        // arrow pointing to the right
        glBegin(GL_TRIANGLE_FAN);
            glVertex3f(basePointRight.x() + right.x()*2.0, basePointRight.y() + right.y()*2.0, basePointRight.z() + right.z()*2.0);
            glVertex3f(basePointRight.x() + dir.x(), basePointRight.y() + dir.y(), basePointRight.z() + dir.z());
            glVertex3f(basePointRight.x() - up.x(), basePointRight.y() - up.y(), basePointRight.z() - up.z());
            glVertex3f(basePointRight.x() - dir.x(), basePointRight.y() - dir.y(), basePointRight.z() - dir.z());
            glVertex3f(basePointRight.x() + up.x(), basePointRight.y() + up.y(), basePointRight.z() + up.z());
            glVertex3f(basePointRight.x() + dir.x(), basePointRight.y() + dir.y(), basePointRight.z() + dir.z());
        glEnd();

        // arrow pointing to the left
        /*    glBegin(GL_TRIANGLE_FAN);
                glVertex3f(basePointLeft.x() - right.x(), basePointLeft.y() - right.y(), basePointLeft.z() - right.z());
                glVertex3f(basePointLeft.x() + dir.x(), basePointLeft.y() + dir.y(), basePointLeft.z() + dir.z());
                glVertex3f(basePointLeft.x() - up.x(), basePointLeft.y() - up.y(), basePointLeft.z() - up.z());
                glVertex3f(basePointLeft.x() - dir.x(), basePointLeft.y() - dir.y(), basePointLeft.z() - dir.z());
                glVertex3f(basePointLeft.x() + up.x(), basePointLeft.y() + up.y(), basePointLeft.z() + up.z());
                glVertex3f(basePointLeft.x() + dir.x(), basePointLeft.y() + dir.y(), basePointLeft.z() + dir.z());
            glEnd();
        */

        if (j == 0)
        {
            glColor4f(1.0, 0.0, 0.0, 1.0);
        }
        glColor4ubv(arrowDirColor);

        glBegin(GL_TRIANGLE_FAN);
            // top of arrow
            glVertex3f(basePointLeft.x() - rightHalf.x() + up.x()*2.0, basePointLeft.y() - rightHalf.y() + up.y()*2.0, basePointLeft.z() - rightHalf.z() + up.z()*2.0);
            glVertex3f(basePointLeft.x() + rightHalf.x(), basePointLeft.y() + rightHalf.y(), basePointLeft.z() + rightHalf.z());
            glVertex3f(basePointLeft.x() - rightHalf.x() + dir.x(), basePointLeft.y() - rightHalf.y() + dir.y(), basePointLeft.z() - rightHalf.z() + dir.z());
            glVertex3f(basePointLeft.x() - right.x() - rightHalf.x(), basePointLeft.y() - right.y() - rightHalf.y(), basePointLeft.z() - right.z() - rightHalf.z());
            glVertex3f(basePointLeft.x() - rightHalf.x() - dir.x(), basePointLeft.y() - rightHalf.y() - dir.y(), basePointLeft.z() - rightHalf.z() - dir.z());
            glVertex3f(basePointLeft.x() + rightHalf.x(), basePointLeft.y() + rightHalf.y(), basePointLeft.z() + rightHalf.z());
        glEnd();

        // draw arrows base
        glBegin(GL_QUAD_STRIP);
            for (int i = 0; i < 4; ++i)
            {
                glVertex3f(upperCircle[i].x() + dirHalf.x(), upperCircle[i].y() + dirHalf.y(), upperCircle[i].z() + dirHalf.z());
                glVertex3f(upperCircle[i].x() - dirHalf.x(), upperCircle[i].y() - dirHalf.y(), upperCircle[i].z() - dirHalf.z());
            }

            for (int i = 3; i >= 0; --i)
            {
                glVertex3f(lowerCircle[i].x() + dirHalf.x(), lowerCircle[i].y() + dirHalf.y(), lowerCircle[i].z() + dirHalf.z());
                glVertex3f(lowerCircle[i].x() - dirHalf.x(), lowerCircle[i].y() - dirHalf.y(), lowerCircle[i].z() - dirHalf.z());
            }

            glVertex3f(upperCircle[0].x() + dirHalf.x(), upperCircle[0].y() + dirHalf.y(), upperCircle[0].z() + dirHalf.z());
            glVertex3f(upperCircle[0].x() - dirHalf.x(), upperCircle[0].y() - dirHalf.y(), upperCircle[0].z() - dirHalf.z());
        glEnd();

        glBegin(GL_QUAD_STRIP);
            for (int i = 0; i < 4; ++i)
            {
                glVertex3f(lowerCircle[i].x() + dirHalf.x(), lowerCircle[i].y() + dirHalf.y(), lowerCircle[i].z() + dirHalf.z());
                glVertex3f(upperCircle[i].x() + dirHalf.x(), upperCircle[i].y() + dirHalf.y(), upperCircle[i].z() + dirHalf.z());
            }

            for (int i = 3; i >= 0; --i)
            {
                glVertex3f(lowerCircle[i].x() - dirHalf.x(), lowerCircle[i].y() - dirHalf.y(), lowerCircle[i].z() - dirHalf.z());
                glVertex3f(upperCircle[i].x() - dirHalf.x(), upperCircle[i].y() - dirHalf.y(), upperCircle[i].z() - dirHalf.z());
            }
        glEnd();
    }
/*    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glColor4f(0.0, 0.0, 0.0, 1.0);
    // arrow pointing upwards
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(basePointUp.x() + up.x()*2.0, basePointUp.y() + up.y()*2.0, basePointUp.z() + up.z()*2.0);
        glVertex3f(basePointUp.x() + dir.x(), basePointUp.y() + dir.y(), basePointUp.z() + dir.z());
        glVertex3f(basePointUp.x() + right.x(), basePointUp.y() + right.y(), basePointUp.z() + right.z());
        glVertex3f(basePointUp.x() - dir.x(), basePointUp.y() - dir.y(), basePointUp.z() - dir.z());
        glVertex3f(basePointUp.x() - right.x(), basePointUp.y() - right.y(), basePointUp.z() - right.z());
        glVertex3f(basePointUp.x() + dir.x(), basePointUp.y() + dir.y(), basePointUp.z() + dir.z());
    glEnd();

    // arrow pointing to the right
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(basePointRight.x() + right.x()*2.0, basePointRight.y() + right.y()*2.0, basePointRight.z() + right.z()*2.0);
        glVertex3f(basePointRight.x() + dir.x(), basePointRight.y() + dir.y(), basePointRight.z() + dir.z());
        glVertex3f(basePointRight.x() - up.x(), basePointRight.y() - up.y(), basePointRight.z() - up.z());
        glVertex3f(basePointRight.x() - dir.x(), basePointRight.y() - dir.y(), basePointRight.z() - dir.z());
        glVertex3f(basePointRight.x() + up.x(), basePointRight.y() + up.y(), basePointRight.z() + up.z());
        glVertex3f(basePointRight.x() + dir.x(), basePointRight.y() + dir.y(), basePointRight.z() + dir.z());
    glEnd();*/
}

void GLCanvas::renderSingleSensor() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

//    Vector3 lookAt = Vector3(0.0f, 0.0f, 0.0f);
    // Vector3 lookAt = _cameraPosition + _cameraFront;
//    gluLookAt(_cameraPosition.x(), _cameraPosition.y(), _cameraPosition.z(),
//              lookAt.x()         , lookAt.y()         , lookAt.z(),
//              _cameraUp.x()      , _cameraUp.y()      , _cameraUp.z());

    gluLookAt(0.0f, 0.0f, _cameraPosition.z(),
              0.0f, 0.0f, 0.0f,
              0.0f, 1.0f, 0.0f);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    Vector3 dir = _sensorOrientation.rotate(Vector3(1.0f, 0.0f, 0.0f));
    Vector3 up = 0.25 * _sensorOrientation.rotate(Vector3(0.0f, 1.0f, 0.0f));
    Vector3 right = 0.5 * _sensorOrientation.rotate(Vector3(0.0f, 0.0f, 1.0f));

    Vector3 frontUpRight = dir + up + right;
    Vector3 frontDownRight = dir - up + right;
    Vector3 frontUpLeft = dir + up - right;
    Vector3 frontDownLeft = dir - up - right;
    Vector3 backUpRight = -dir + up + right;
    Vector3 backDownRight = -dir - up + right;
    Vector3 backUpLeft = -dir + up - right;
    Vector3 backDownLeft = -dir - up - right;


    glBegin(GL_QUADS);
        glColor3f(0.8, 0.5, 0.5);
        glVertex3f(frontUpRight.x(), frontUpRight.y(), frontUpRight.z());
        glVertex3f(frontUpLeft.x(), frontUpLeft.y(), frontUpLeft.z());
        glVertex3f(frontDownLeft.x(), frontDownLeft.y(), frontDownLeft.z());
        glVertex3f(frontDownRight.x(), frontDownRight.y(), frontDownRight.z());

        glColor3f(0.5, 0.7, 0.5);
        glVertex3f(backUpRight.x(), backUpRight.y(), backUpRight.z());
        glVertex3f(frontUpRight.x(), frontUpRight.y(), frontUpRight.z());
        glVertex3f(frontUpLeft.x(), frontUpLeft.y(), frontUpLeft.z());
        glVertex3f(backUpLeft.x(), backUpLeft.y(), backUpLeft.z());

        glColor3f(0.5, 0.5, 0.7);
        glVertex3f(backDownRight.x(), backDownRight.y(), backDownRight.z());
        glVertex3f(frontDownRight.x(), frontDownRight.y(), frontDownRight.z());
        glVertex3f(frontUpRight.x(), frontUpRight.y(), frontUpRight.z());
        glVertex3f(backUpRight.x(), backUpRight.y(), backUpRight.z());

        glColor3f(0.7, 0.7, 0.7);
        glVertex3f(backUpRight.x(), backUpRight.y(), backUpRight.z());
        glVertex3f(backUpLeft.x(), backUpLeft.y(), backUpLeft.z());
        glVertex3f(backDownLeft.x(), backDownLeft.y(), backDownLeft.z());
        glVertex3f(backDownRight.x(), backDownRight.y(), backDownRight.z());

        glVertex3f(backDownLeft.x(), backDownLeft.y(), backDownLeft.z());
        glVertex3f(frontDownLeft.x(), frontDownLeft.y(), frontDownLeft.z());
        glVertex3f(frontDownRight.x(), frontDownRight.y(), frontDownRight.z());
        glVertex3f(backDownRight.x(), backDownRight.y(), backDownRight.z());

        glVertex3f(backUpLeft.x(), backUpLeft.y(), backUpLeft.z());
        glVertex3f(frontUpLeft.x(), frontUpLeft.y(), frontUpLeft.z());
        glVertex3f(frontDownLeft.x(), frontDownLeft.y(), frontDownLeft.z());
        glVertex3f(backDownLeft.x(), backDownLeft.y(), backDownLeft.z());
    glEnd();

    // scale slightly to ensure the lines are visible
    glScalef(1.001f, 1.001f, 1.001f);

    glLineWidth(1.0f);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_STRIP);
        glVertex3f(backUpRight.x(), backUpRight.y(), backUpRight.z());
        glVertex3f(backUpLeft.x(), backUpLeft.y(), backUpLeft.z());
        glVertex3f(backDownLeft.x(), backDownLeft.y(), backDownLeft.z());
        glVertex3f(backDownRight.x(), backDownRight.y(), backDownRight.z());
        glVertex3f(backUpRight.x(), backUpRight.y(), backUpRight.z());

        glVertex3f(frontUpRight.x(), frontUpRight.y(), frontUpRight.z());
        glVertex3f(frontUpLeft.x(), frontUpLeft.y(), frontUpLeft.z());
        glVertex3f(frontDownLeft.x(), frontDownLeft.y(), frontDownLeft.z());
        glVertex3f(frontDownRight.x(), frontDownRight.y(), frontDownRight.z());
        glVertex3f(frontUpRight.x(), frontUpRight.y(), frontUpRight.z());
    glEnd();

    glBegin(GL_LINES);
        glVertex3f(backUpLeft.x(), backUpLeft.y(), backUpLeft.z());
        glVertex3f(frontUpLeft.x(), frontUpLeft.y(), frontUpLeft.z());

        glVertex3f(backDownLeft.x(), backDownLeft.y(), backDownLeft.z());
        glVertex3f(frontDownLeft.x(), frontDownLeft.y(), frontDownLeft.z());

        glVertex3f(backDownRight.x(), backDownRight.y(), backDownRight.z());
        glVertex3f(frontDownRight.x(), frontDownRight.y(), frontDownRight.z());
    glEnd();

    glLineWidth(2.0f);
    // draw global coordinate system
    glBegin(GL_LINES);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(-1.5, -1.0, -1.0);
        glVertex3f(1.0, -1.0, -1.0);

        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(-1.5, -1.0, -1.0);
        glVertex3f(-1.5, 1.0, -1.0);

        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(-1.5, -1.0, -1.0);
        glVertex3f(-1.5, -1.0, 1.0);
    glEnd();

    // draw global coordinate system
//    glBegin(GL_LINES);
//        glColor3f(1.0, 0.0, 0.0);
//        glVertex3f(0.0, 0.0, 0.0);
//        glVertex3f(1.5, 00.0, 0.0);
//
//        glColor3f(0.0, 1.0, 0.0);
//        glVertex3f(0.0, 0.0, 0.0);
//        glVertex3f(0.0, 1.5, 0.0);
//
//        glColor3f(0.0, 0.0, 1.0);
//        glVertex3f(0.0, 0.0, 0.0);
//        glVertex3f(0.0, 0.0, 1.5);
//    glEnd();

    glLineWidth(1.0f);
}

// Initialization of all OpenGL specific parameters.
void GLCanvas::InitGL()
{
    SetCurrent(*_GLRC);
	glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glShadeModel(GL_SMOOTH);
}

int GLCanvas::getObjectIdAt(const wxPoint& pos)
{
    int id = -1;
    if (_GLRC)
    {
        SetCurrent(*_GLRC);
        glDisable(GL_BLEND);
        _style |= SELECTION_MODE;
        glClearColor(1.0, 1.0, 1.0, 1.0);
        renderSkeleton();
        glFlush();
        glClearColor(0.0, 0.0, 0.0, 0.0);
        _style &= ~SELECTION_MODE;
        glEnable(GL_BLEND);

        // read the color at pos and convert it to an id
        GLubyte color[3];
        glReadPixels(pos.x, GetClientSize().y - pos.y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, color);
        id = static_cast<unsigned int>(((color[0]) << 16) + ((color[1]) << 8) + (color[2]));

        // if all color values are set to 0xFF, the background (or no clickable object) was clicked. Set id to -1
        if (id >= 0xFFFFFF)
        {
            id = -1;
        }
    }
    return id;
}


