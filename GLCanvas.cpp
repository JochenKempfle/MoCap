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
#include "GLCanvasDialog.h"

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
	Connect(wxEVT_ENTER_WINDOW, (wxObjectEventFunction)&GLCanvas::OnEnterWindow);
	Connect(wxEVT_LEAVE_WINDOW, (wxObjectEventFunction)&GLCanvas::OnLeaveWindow);
    InitGL();
    _xRotation = 0.0f;
    _yRotation = M_PI;
    _cameraPosition = Vector3(0.0f, 1.0f, 3.0f);
    _cameraFront = Vector3(0.0f, 0.0f, -1.0f);
    _cameraUp = Vector3(0.0f, 1.0f, 0.0f);
    _cameraRight = Vector3(1.0f, 0.0f, 0.0f);
    _showUI = false;
    _backgroundColor = wxColour(0, 0, 0);
    _lClicked = false;
    _rClicked = false;
    _skeleton = nullptr;
    _constraint = nullptr;
    _style = STANDARD;
	SetFocus();
	// _image.loadImage(_("C:/Users/Jochen/Desktop/Katana.png"));
	_sphereQuad = gluNewQuadric();
}

GLCanvas::~GLCanvas()
{
    gluDeleteQuadric(_sphereQuad);
    delete _GLRC;
}

void GLCanvas::OnPaint(wxPaintEvent &event)
{
	wxPaintDC dc(this);

	if (!_GLRC)
    {
        return;
    }

	SetCurrent(*_GLRC);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	if (_style & SINGLE_SENSOR_MODE)
    {
        renderSingleSensor();
    }
    else if (_style & SINGLE_JOINT_MODE)
    {
        renderSingleJoint();
    }
    else
    {
        renderSkeleton();
    }
	// _image.render();
	// glFlush();
	SwapBuffers();
	if (_showUI && !(_style & SELECTION_MODE))
    {
        drawUserInterface(dc);
    }
	event.Skip();
}

void GLCanvas::OnSize(wxSizeEvent &event)
{
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
	//SetFocus();
	if (_style & (SINGLE_JOINT_MODE|SINGLE_SENSOR_MODE))
    {
        _showUI = false;
    }
    else
    {
        _showUI = true;
    }
	event.Skip();
	Refresh();
}

void GLCanvas::OnLeaveWindow(wxMouseEvent &event)
{
	_showUI = false;
	event.Skip();
	Refresh();
}

void GLCanvas::OnLeftDown(wxMouseEvent &event)
{
    SetFocus();

    wxSize size = GetSize();
    wxPoint pos = event.GetPosition();

    bool showMenu = false;

    if (pos.x > size.x - _buttonSize && pos.y < _numButtons * _buttonSize)
    {
        int index = pos.y / _buttonSize;
        switch (index)
        {
            case 0:
                _style = _style & DRAW_GRID ? _style & ~DRAW_GRID : _style | DRAW_GRID;
                break;

            case 1:
                _style = _style & DRAW_AABB ? _style & ~DRAW_AABB : _style | DRAW_AABB;
                break;

            case 2:
                _style = _style & DRAW_LOCAL_COORDINATE_SYSTEM ? _style & ~DRAW_LOCAL_COORDINATE_SYSTEM : _style | DRAW_LOCAL_COORDINATE_SYSTEM;
                break;

            case 3:
                _style = _style & DRAW_JOINT_CONSTRAINTS ? _style & ~DRAW_JOINT_CONSTRAINTS : _style | DRAW_JOINT_CONSTRAINTS;
                break;

            case 4:
                _style = _style & DRAW_ROTATION_AXIS ? _style & ~DRAW_ROTATION_AXIS : _style | DRAW_ROTATION_AXIS;
                break;

            case 5:
                _style = _style & DRAW_LABEL ? _style & ~DRAW_LABEL : _style | DRAW_LABEL;
                break;

            case 6:
                if (_skeleton != nullptr)
                {
                    _cameraPosition = _skeleton->getPosition() - 3.0f*_cameraFront;
                }
                else
                {
                    _cameraPosition = Vector3(0.0f, 1.0f, 3.0f);
                }
                break;

            case 7:
                showMenu = true;
                break;

            default:
                break;
        }

        if (showMenu)
        {
            GLCanvasDialog dialog(this);
            dialog.setBackgroundColor(_backgroundColor);
            if (dialog.ShowModal() == wxID_OK)
            {
                _backgroundColor = dialog.getBackgroundColor();
                glClearColor(float(_backgroundColor.Red())/255.0f, float(_backgroundColor.Green())/255.0f, float(_backgroundColor.Blue())/255.0f, 0.0);
                Refresh();
            }
        }

        return;
    }

    if (!HasCapture())
    {
        CaptureMouse();
    }
    _lClicked = true;
    _prevMousePos = pos;

    SetCursor(wxCURSOR_BLANK);

    event.Skip();
}

void GLCanvas::OnLeftUp(wxMouseEvent &event)
{
    // release mouse (only if not also captured by right click)
    if (HasCapture() && !_rClicked)
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
    _prevMousePos = event.GetPosition();

    SetCursor(wxCURSOR_BLANK);

    event.Skip();
}

void GLCanvas::OnRightUp(wxMouseEvent &event)
{
    // release mouse (only if not also captured by left click)
    if (HasCapture() && !_lClicked)
    {
        ReleaseMouse();
    }
    _rClicked = false;
    SetCursor(wxCURSOR_ARROW);
    Refresh();
}

void GLCanvas::OnMouseMove(wxMouseEvent &event)
{
    wxSize size = GetSize();
    wxPoint pos = event.GetPosition();

    if (pos.x > size.x - _buttonSize && pos.y < _numButtons * _buttonSize)
    {
        wxString tooltip;
        int index = pos.y / _buttonSize;
        switch (index)
        {
            case 0:
                tooltip = _("Show grid");
                break;

            case 1:
                tooltip = _("Show bounding box");
                break;

            case 2:
                tooltip = _("Show coordinate system");
                break;

            case 3:
                tooltip = _("Show joint constraints");
                break;

            case 4:
                tooltip = _("Show rotation axis");
                break;

            case 5:
                tooltip = _("Show label");
                break;

            case 6:
                tooltip = _("Move to skeleton");
                break;

            case 7:
                tooltip = _("Settings");
                break;

            default:
                tooltip = _("");
                break;
        }
        wxToolTip* prevToolTip = GetToolTip();
        if (prevToolTip == nullptr || prevToolTip->GetTip() != tooltip)
        {
            SetToolTip(tooltip);
        }
    }
    else if (GetToolTip() != nullptr)
    {
        UnsetToolTip();
    }

    if (_style & SINGLE_SENSOR_MODE)
    {
        return;
    }

    // get the delta of the mouse movement
    wxPoint delta = pos - _prevMousePos;
    _prevMousePos = pos;

    if (_rClicked)
    {
        _xRotation -= 0.1f * M_PI/180.0f * float(delta.y);
        if (_xRotation > M_PI/2.0)
        {
            _xRotation = M_PI/2.0;
        }
        else if (_xRotation < -M_PI/2.0)
        {
            _xRotation = -M_PI/2.0;
        }
        _yRotation -= 0.1f * M_PI/180.0f * float(delta.x);
        _cameraFront = Vector3(cos(_xRotation) * sin(_yRotation), sin(_xRotation), cos(_xRotation) * cos(_yRotation)).normalized();
        _cameraUp = Vector3(sin(_yRotation - M_PI/2.0f), 0.0f, cos(_yRotation - M_PI/2.0f)).cross(_cameraFront).normalized();
        _cameraRight = _cameraFront.cross(_cameraUp).normalized();
        //_prevMousePos = event.GetPosition();
        // wxPoint cursorPos = ClientToScreen(_prevMousePos);
        // WarpPointer(_prevMousePos.x, _prevMousePos.y);
        // SetCursorPos(cursorPos.x, cursorPos.y);
        Refresh();
    }
    if (_lClicked)
    {
        _cameraPosition -= _cameraSpeed * float(delta.x) * _cameraRight;
        _cameraPosition += _cameraSpeed * float(delta.y) * _cameraUp;
        //_cameraPosition.y() += _cameraSpeed * float(delta.y);
        // _prevMousePos = event.GetPosition();

        // warp pointer causes glcanvas to freeze (under linux). Consider not hiding the cursor
        // WarpPointer(_prevMousePos.x, _prevMousePos.y);
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
    Vector3 lookAt = _cameraPosition + _cameraFront;
    gluLookAt(_cameraPosition.x(), _cameraPosition.y(), _cameraPosition.z(),
              lookAt.x()         , lookAt.y()         , lookAt.z(),
              _cameraUp.x()      , _cameraUp.y()      , _cameraUp.z());

    if (_style & DRAW_GRID)
    {
        drawGrid();
    }
    if (_skeleton == nullptr)
    {
        return;
    }

    for (auto it = _skeleton->beginBones(); it != _skeleton->endBones(); ++it)
    {
        glPushMatrix();
        Bone bone = *(it->second);

        int boneId = bone.getId();

        // don't draw any bone without valid id
        if (boneId < 0)
        {
            continue;
        }

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

        glTranslatef(startPos.x(), startPos.y(), startPos.z());

        float length = bone.getLength();

        Vector3 dir = bone.getDirection();
        Vector3 up = bone.getUpDirection();
        Vector3 right = bone.getRightDirection();

        Vector3 endPos = dir*length;

        startPos = Vector3(0, 0, 0);

        float length_10 = length * 0.1f;
        Vector3 endPos_10 = endPos * 0.1f;

        Vector3 upPoint = up*length_10 + endPos_10;
        Vector3 downPoint = - up*length_10 + endPos_10;
        Vector3 rightPoint = right*length_10 + endPos_10;
        Vector3 leftPoint = - right*length_10 + endPos_10;

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

                glVertex3f(endPos.x(), endPos.y(), endPos.z());
                glVertex3f(endPos.x() + dir.x()*0.1f, endPos.y() + dir.y()*0.1f, endPos.z() + dir.z()*0.1f);

                glColor4ubv(green);

                glVertex3f(endPos.x(), endPos.y(), endPos.z());
                glVertex3f(endPos.x() + up.x()*0.1f, endPos.y() + up.y()*0.1f, endPos.z() + up.z()*0.1f);

                glColor4ubv(blue);

                glVertex3f(endPos.x(), endPos.y(), endPos.z());
                glVertex3f(endPos.x() + right.x()*0.1f, endPos.y() + right.y()*0.1f, endPos.z() + right.z()*0.1f);
            glEnd();
        }

        if (_style & DRAW_ROTATION_AXIS)
        {
            Vector3 rotAxis = bone.getRelOrientation().getRotationAxis();

            glBegin(GL_LINES);
                glColor4ubv(yellow);

                glVertex3f(- rotAxis.x()*0.2f, - rotAxis.y()*0.2f, - rotAxis.z()*0.2f);
                glVertex3f(rotAxis.x()*0.2f, rotAxis.y()*0.2f, rotAxis.z()*0.2f);
            glEnd();
        }

        // draw bone
        glPolygonMode(GL_FRONT, GL_FILL);

        glColor4ubv(boneColor1);

        glBegin(GL_TRIANGLE_FAN);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(upPoint.x(), upPoint.y(), upPoint.z());
            glVertex3f(leftPoint.x(), leftPoint.y(), leftPoint.z());
            glVertex3f(downPoint.x(), downPoint.y(), downPoint.z());
            glVertex3f(rightPoint.x(), rightPoint.y(), rightPoint.z());
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
                glVertex3f(0.0f, 0.0f, 0.0f);
                glVertex3f(upPoint.x(), upPoint.y(), upPoint.z());
                glVertex3f(leftPoint.x(), leftPoint.y(), leftPoint.z());
                glVertex3f(downPoint.x(), downPoint.y(), downPoint.z());
                glVertex3f(rightPoint.x(), rightPoint.y(), rightPoint.z());
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

            // draw labels
            if (_style & DRAW_LABEL)
            {
                // glDisable(GL_DEPTH_TEST);
                GLImage* image = _labels.find(boneId)->second;
                image->setPosition(0.5f * bone.getLength() * dir - 0.06f * bone.getLength() * _cameraFront);
                image->render();
                // glEnable(GL_DEPTH_TEST);
            }
        }

        if (_style & DRAW_SPIN_ARROWS && bone.getId() == _skeleton->getSelectedBoneId())
        {
            drawSpinArrows(endPos - 0.2 * bone.getLength() * dir, dir, up, right);
        }
        glPopMatrix();
    }

    // draw joints (after everything else, as they are transparent and need everything else be rendered)
    if (_style & DRAW_JOINT_CONSTRAINTS)
    {
        for (auto it = _skeleton->beginBones(); it != _skeleton->endBones(); ++it)
        {
            glPushMatrix();
            Bone bone = *(it->second);

            Vector3 startPos = bone.getStartPos();
            glTranslatef(startPos.x(), startPos.y(), startPos.z());

            JointConstraint constraint = bone.getJointConstraint();

            drawJoint(constraint);

            glPopMatrix();
        }
    }

    if (!(_style & SELECTION_MODE) && _style & DRAW_AABB)
    {
        drawAABB(_skeleton->getAABB());
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
        glVertex3f(frontUpLeft.x(), frontUpLeft.y(), frontUpLeft.z());
        glVertex3f(frontUpRight.x(), frontUpRight.y(), frontUpRight.z());
        glVertex3f(frontDownRight.x(), frontDownRight.y(), frontDownRight.z());
        glVertex3f(frontDownLeft.x(), frontDownLeft.y(), frontDownLeft.z());

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


void GLCanvas::renderSingleJoint() const
{
    Vector3 lookAt = _cameraPosition + _cameraFront;
    gluLookAt(_cameraPosition.x(), _cameraPosition.y(), _cameraPosition.z(),
              lookAt.x()         , lookAt.y()         , lookAt.z(),
              _cameraUp.x()      , _cameraUp.y()      , _cameraUp.z());

    glBegin(GL_LINES);
        glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(1.0f, 0.0f, 0.0f);

        glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 1.0f, 0.0f);

        glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 1.0f);
    glEnd();

    if (_constraint != nullptr)
    {
        drawJoint(*_constraint, 1.0f);
    }
}

// Initialization of all OpenGL specific parameters.
void GLCanvas::InitGL()
{
    SetCurrent(*_GLRC);
	glClearColor(0.0, 0.0, 0.0, 0.0);
    //glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glShadeModel(GL_SMOOTH);
}

int GLCanvas::getObjectIdAt(const wxPoint& pos)
{
    if (pos.x > GetSize().x - _buttonSize && pos.y < _numButtons * _buttonSize)
    {
        return -1;
    }

    int id = -1;
    if (_GLRC)
    {
        SetCurrent(*_GLRC);
        glDisable(GL_BLEND);
        _style |= SELECTION_MODE;

        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

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

void GLCanvas::setSkeleton(Skeleton* skeleton)
{
    _skeleton = skeleton;
    for (auto it = _labels.begin(); it != _labels.end(); ++it)
    {
        delete it->second;
    }
    _labels.clear();

    if (_skeleton == nullptr)
    {
        return;
    }

    // create labels
    wxMemoryDC dc;
    dc.SetPen(*wxWHITE_PEN);
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.SetTextForeground(*wxWHITE);
    for (auto it = _skeleton->beginBones(); it != _skeleton->endBones(); ++it)
    {
        wxString name = it->second->getName();
        wxSize textSize = dc.GetTextExtent(name);
        textSize.x += 4;
        textSize.y += 1;
        wxBitmap bmp(textSize);
        dc.SelectObject(bmp);
        dc.DrawRoundedRectangle(0, 0, textSize.x, textSize.y, 5);
        dc.DrawText(name, 2, 0);

        GLImage* image = new GLImage();

        image->setImage(bmp);
        image->setBillboard();
        image->scale(0.003f * textSize.x, 0.003f * textSize.x);

        _labels[it->first] = image;
    }
}

void GLCanvas::drawUserInterface(wxDC &dc) const
{
    wxSize size = GetSize();
    wxPoint pos(0, 0);
    pos.x = size.x - _buttonSize;

    wxSize buttonSize(_buttonSize, _buttonSize);

    wxPen whitePen(wxColour(255, 255, 255));
    whitePen.SetWidth(1);
    wxPen blackPen(wxColour(0, 0, 0));
    blackPen.SetWidth(1);
    wxPen greenPen(wxColour(0, 255, 0));
    wxBrush brush;

	dc.SetPen(blackPen);

    brush.SetColour(wxColour(0, 0, 0));
	dc.SetBrush(brush);

	// draw black background behind all buttons
	dc.DrawRectangle(pos.x - 1, pos.y, _buttonSize + 1, _numButtons * _buttonSize + 1);

	// draw grit
	if (_style & DRAW_GRID)
    {
        dc.SetPen(greenPen);
    }
    else
    {
        dc.SetPen(whitePen);
    }

	dc.DrawRectangle(pos.x + 1, pos.y + 1, _buttonSize - 2, _buttonSize - 2);

	dc.DrawLine(pos.x + _buttonSizeHalf, pos.y + 1, pos.x + _buttonSizeHalf, pos.y + _buttonSize - 1);
	dc.DrawLine(pos.x + 1, pos.y + _buttonSizeHalf, size.x - 1, pos.y + _buttonSizeHalf);

	pos.y += _buttonSize;

	// draw a cube for showing AABB
	if (_style & DRAW_AABB)
    {
        dc.SetPen(greenPen);
    }
    else
    {
        dc.SetPen(whitePen);
    }

	dc.DrawRectangle(pos.x + _buttonSizeHalf - 3, pos.y + 3, _buttonSizeHalf, _buttonSizeHalf);
	dc.DrawRectangle(pos.x + 2, pos.y + _buttonSizeHalf - 2, _buttonSizeHalf, _buttonSizeHalf);

	dc.DrawLine(pos.x + 2, pos.y + _buttonSizeHalf - 2, pos.x + _buttonSizeHalf - 3, pos.y + 3);
	dc.DrawLine(pos.x + 2 + _buttonSizeHalf, pos.y + _buttonSizeHalf - 2, pos.x + _buttonSize - 3, pos.y + 3);
	dc.DrawLine(pos.x + 2 + _buttonSizeHalf, pos.y + _buttonSize - 3, pos.x + _buttonSize - 3, pos.y + _buttonSizeHalf + 2);

	pos.y += _buttonSize;

	// draw coordinate system
	if (_style & DRAW_LOCAL_COORDINATE_SYSTEM)
    {
        dc.SetPen(greenPen);
    }
    else
    {
        dc.SetPen(whitePen);
    }

	dc.DrawLine(pos.x + 2, pos.y + 3, pos.x + 2, pos.y + _buttonSize - 2);
	dc.DrawLine(pos.x + 2, pos.y + _buttonSize - 2, size.x - 3, pos.y + _buttonSize - 2);

	dc.DrawLine(pos.x + 2, pos.y + _buttonSize - 2, pos.x + _buttonSize - 6, pos.y + _buttonSizeHalf + 2);

	pos.y += _buttonSize;

	// draw joint constraints
	if (_style & DRAW_JOINT_CONSTRAINTS)
    {
        dc.SetPen(greenPen);
    }
    else
    {
        dc.SetPen(whitePen);
    }
	//dc.DrawCircle(pos.x + _buttonSizeHalf, pos.y + _buttonSizeHalf, _buttonSizeHalf - 3);
    //dc.DrawArc(pos.x + 4, pos.y + 5, pos.x + 4, pos.y + _buttonSize - 5, pos.x + _buttonSizeHalf + 2, pos.y + _buttonSizeHalf);
	//dc.DrawCircle(pos.x + 6, pos.y + _buttonSizeHalf, 3);
	dc.DrawEllipse(pos.x + 4, pos.y + 3, _buttonSizeHalf - 5, _buttonSizeHalf + 4);
	dc.DrawLine(pos.x + 8, pos.y + 3, pos.x + _buttonSizeHalf + 5, pos.y + _buttonSizeHalf);
	dc.DrawLine(pos.x + 8, pos.y + _buttonSizeHalf + 7, pos.x + _buttonSizeHalf + 5, pos.y + _buttonSizeHalf);

	pos.y += _buttonSize;

	// draw rotation axis
	if (_style & DRAW_ROTATION_AXIS)
    {
        dc.SetPen(greenPen);
    }
    else
    {
        dc.SetPen(whitePen);
    }

    dc.DrawArc(pos.x + _buttonSizeHalf + 2, pos.y + _buttonSize - 2, pos.x + _buttonSizeHalf, pos.y + _buttonSizeHalf + 1, pos.x + 2, pos.y + _buttonSize - 2);
    //dc.DrawEllipse(pos.x + 6, pos.y + 5, _buttonSize - 12, _buttonSize - 10);
    //dc.DrawCircle(pos.x + _buttonSizeHalf + 2, pos.y + _buttonSizeHalf - 2, 3);
	dc.DrawLine(pos.x + 2, pos.y + _buttonSize - 2, pos.x + _buttonSize - 2, pos.y + 2);
	dc.DrawLine(pos.x + 2, pos.y + _buttonSize - 2, pos.x + _buttonSize - 2, pos.y + _buttonSize - 2);

	pos.y += _buttonSize;

	// draw label
	if (_style & DRAW_LABEL)
    {
        dc.SetPen(greenPen);
    }
    else
    {
        dc.SetPen(whitePen);
    }

	dc.DrawRoundedRectangle(pos.x + 4, pos.y + 3, _buttonSize - 5, _buttonSizeHalf, 3.0);
	dc.DrawSpline(pos.x + 4, pos.y + _buttonSizeHalf - 3, pos.x + 1, pos.y + _buttonSizeHalf + 4, pos.x + _buttonSizeHalf + 4, pos.y + _buttonSizeHalf + 6);

	pos.y += _buttonSize;

	// draw locate skeleton button
    dc.SetPen(whitePen);

	dc.DrawLine(pos.x + _buttonSizeHalf, pos.y + 2, pos.x + _buttonSizeHalf, pos.y + _buttonSize - 1);
	dc.DrawLine(pos.x + 2, pos.y + _buttonSizeHalf, size.x - 1, pos.y + _buttonSizeHalf);

	dc.DrawCircle(pos.x + _buttonSizeHalf, pos.y + _buttonSizeHalf, _buttonSizeHalf - 4);
	brush.SetColour(wxColour(255, 255, 255));
	dc.SetBrush(brush);
	dc.DrawCircle(pos.x + _buttonSizeHalf, pos.y + _buttonSizeHalf, _buttonSizeHalf - 8);


	pos.y += _buttonSize;

	// draw settings button
	dc.DrawCircle(pos.x + _buttonSizeHalf, pos.y + _buttonSizeHalf, _buttonSizeHalf - 3);

	dc.SetPen(blackPen);

	brush.SetColour(wxColour(0, 0, 0));
	dc.SetBrush(brush);

	dc.DrawCircle(pos.x + _buttonSizeHalf + 6, pos.y + _buttonSizeHalf, 2);
	dc.DrawCircle(pos.x + _buttonSizeHalf - 6, pos.y + _buttonSizeHalf, 2);

	dc.DrawCircle(pos.x + _buttonSizeHalf, pos.y + _buttonSizeHalf + 6, 2);
	dc.DrawCircle(pos.x + _buttonSizeHalf, pos.y + _buttonSizeHalf - 6, 2);

	dc.SetPen(whitePen);

	dc.DrawCircle(pos.x + _buttonSizeHalf, pos.y + _buttonSizeHalf, _buttonSizeHalf - 7);
}

void GLCanvas::drawGrid() const
{
    int x = int(_cameraPosition.x());
    int z = int(_cameraPosition.z());

    glBegin(GL_LINES);
        glColor3d(0.95, 0.95, 0.95);

        for (int i = -20; i < 21; ++i)
        {
            glVertex3i(x + i, 0, z - 20);
            glVertex3i(x + i, 0, z + 20);

            glVertex3i(x - 20, 0, z + i);
            glVertex3i(x + 20, 0, z + i);
        }
    glEnd();
}

void GLCanvas::drawJoint(const JointConstraint &constraint, float radius) const
{
    // auto points = constraint.getAxisAngleToAngleKeyPoints();
    std::vector<Vector3> vectors = constraint.getKeyPointsInterpolated(radius, 2.0f);

    glBegin(GL_LINE_LOOP);
        glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
        for (size_t i = 0; i < vectors.size(); ++i)
        {
            glVertex3f(vectors[i].x(), vectors[i].y(), vectors[i].z());
        }
    glEnd();

    vectors = constraint.getKeyPoints(radius);

    glBegin(GL_LINE_LOOP);
        glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
        for (size_t i = 0; i < vectors.size(); ++i)
        {
            glVertex3f(vectors[i].x(), vectors[i].y(), vectors[i].z());
        }
    glEnd();

    glBegin(GL_LINE_LOOP);
        glColor4f(1.0f, 0, 0, 1.0f);
        for (size_t i = 0; i < 360; ++i)
        {
            float angle = constraint.getMaxAngleAt(i);
            glVertex3f(float(i)/60.0f - 2, angle/60.0f + 1.5, 1.0f);
        }
    glEnd();

    glRenderMode(GL_SMOOTH);
    glPolygonMode(GL_FRONT, GL_FILL);
    //glColor3f(0.5f, 0.5f, 0.5f);
    glColor4f(0.5f, 0.5f, 0.5f, 0.5f);

    gluSphere(_sphereQuad, radius, 10, 8);
}

void GLCanvas::drawAABB(const AABB &box) const
{
    float minX = box.minX();
    float minY = box.minY();
    float minZ = box.minZ();

    float maxX = box.maxX();
    float maxY = box.maxY();
    float maxZ = box.maxZ();

    /* Uncomment if always a real cube shall be displayed
    const float delta = 0.002f;

    if (std::abs(maxX - minX) < 2.0f*delta)
    {
        minX -= delta;
        maxX += delta;
    }
    if (std::abs(maxY - minY) < delta)
    {
        minY -= delta;
        maxY += delta;
    }
    if (std::abs(maxZ - minZ) < delta)
    {
        minZ -= delta;
        maxZ += delta;
    }
    */

    glColor4ubv(white);

    glBegin(GL_LINES);
        glVertex3f(minX, minY, minZ);
        glVertex3f(maxX, minY, minZ);

        glVertex3f(minX, minY, minZ);
        glVertex3f(minX, maxY, minZ);

        glVertex3f(minX, minY, minZ);
        glVertex3f(minX, minY, maxZ);

        glVertex3f(maxX, maxY, maxZ);
        glVertex3f(minX, maxY, maxZ);

        glVertex3f(maxX, maxY, maxZ);
        glVertex3f(maxX, minY, maxZ);

        glVertex3f(maxX, maxY, maxZ);
        glVertex3f(maxX, maxY, minZ);

        glVertex3f(maxX, minY, minZ);
        glVertex3f(maxX, maxY, minZ);

        glVertex3f(maxX, minY, minZ);
        glVertex3f(maxX, minY, maxZ);

        glVertex3f(minX, maxY, minZ);
        glVertex3f(maxX, maxY, minZ);

        glVertex3f(minX, maxY, minZ);
        glVertex3f(minX, maxY, maxZ);

        glVertex3f(minX, minY, maxZ);
        glVertex3f(maxX, minY, maxZ);

        glVertex3f(minX, minY, maxZ);
        glVertex3f(minX, maxY, maxZ);
    glEnd();
}

/* void drawSphere(double r, int lats, int longs)
{
    for(int i = 0; i <= lats; ++i)
    {
        double lat0 = M_PI * (-0.5 + double(i - 1) / lats);
        double z0 = std::sin(lat0);
        double zr0 = std::cos(lat0);

        double lat1 = M_PI * (-0.5 + double(i) / lats);
        double z1 = std::sin(lat1);
        double zr1 = std::cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for(int j = 0; j <= longs; ++j)
        {
            double lng = 2 * M_PI * (double) (j - 1) / longs;
            double x = std::cos(lng);
            double y = std::sin(lng);

            glNormal3f(x * zr0, y * zr0, z0);
            glVertex3f(x * zr0, y * zr0, z0);
            glNormal3f(x * zr1, y * zr1, z1);
            glVertex3f(x * zr1, y * zr1, z1);
        }
        glEnd();
    }
}
*/
