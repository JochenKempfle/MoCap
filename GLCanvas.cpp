/*Copyright (c) 2014, Jochen KempfleAll rights reserved.Redistribution and use in source and binary forms, with or without modification,are permitted provided that the following conditions are met:1. Redistributions of source code must retain the above copyright notice,this list of conditions and the following disclaimer.2. Redistributions in binary form must reproduce the above copyright notice,this list of conditions and the following disclaimer in the documentation and/orother materials provided with the distribution.THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" ANDANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIEDWARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITYOF SUCH DAMAGE.*/#include "wx_pch.h"#include "GLCanvas.h"#include <GL/gl.h>#include <GL/glu.h>#include "MoCapManager.h"#ifndef WX_PRECOMP
	//InternalHeadersPCH(GLCanvas)
	#include <wx/wx.h>
	//
#endifBEGIN_EVENT_TABLE(GLCanvas, wxGLCanvas)
	//EventTable(GLCanvas)
	//
END_EVENT_TABLE()// Constructor to initialize the canvas.GLCanvas::GLCanvas(wxWindow* parent, wxWindowID id, const wxPoint &pos, const wxSize &size, int style, const wxString &name, int* attribList)        : wxGLCanvas(parent, id, attribList, pos, size, style, name){    _GLRC = new wxGLContext(this);    Connect(wxEVT_SIZE, (wxObjectEventFunction)&GLCanvas::OnSize);    Connect(wxEVT_PAINT,(wxObjectEventFunction)&GLCanvas::OnPaint);
	Connect(wxEVT_LEFT_DOWN,(wxObjectEventFunction)&GLCanvas::OnLeftDown);
	Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&GLCanvas::OnLeftUp);
	Connect(wxEVT_MOTION,(wxObjectEventFunction)&GLCanvas::OnMouseMove);    InitGL();    _xRotation = 0.0f;    _yRotation = 0.0f;    _clicked = false;}// DestructorGLCanvas::~GLCanvas(){    delete _GLRC;}// Paint event. This method gets called every time the graphics should be drawn.void GLCanvas::OnPaint(wxPaintEvent &event){	wxPaintDC dc(this);	if (!_GLRC) return;	SetCurrent(*_GLRC);	Render();	// glFlush();	SwapBuffers();	event.Skip();}// This method gets handles all resize events that may occur on// the canvas. It preserves the aspect ratio and defines the// projection matrix (GL_PROJECTION).void GLCanvas::OnSize(wxSizeEvent &event){    // wxMessageBox("resize!");	// this is also necessary to update the context on some platforms - no longer =)	// wxGLCanvas::OnSize(event);	// set GL viewport	wxSize size = event.GetSize();	if (_GLRC)	{		SetCurrent(*_GLRC);		glViewport(0, 0, GLint(size.x), GLint(size.y));	}	glMatrixMode(GL_PROJECTION);	glLoadIdentity();	gluPerspective(45.0, double(size.x) / double(size.y), 0.1, 100.0);	glMatrixMode(GL_MODELVIEW);}// This method is responsible to clear the background. However// it does nothing but consume the event. This is to avoid flashing effects.void GLCanvas::OnEraseBackground(wxEraseEvent &event){	event.Skip();}// Focus handling. This event handler consumes the event.void GLCanvas::OnEnterWindow(wxMouseEvent &event){	SetFocus();	event.Skip();}void GLCanvas::OnLeftDown(wxMouseEvent &event){    SetFocus();    CaptureMouse();    _clicked = true;    _mousePosAtClick = event.GetPosition();}void GLCanvas::OnLeftUp(wxMouseEvent &event){    if (HasCapture())    {        ReleaseMouse();    }    _clicked = false;    Refresh();}void GLCanvas::OnMouseMove(wxMouseEvent &event){    if (_clicked)    {        wxPoint delta = event.GetPosition() - _mousePosAtClick;        _xRotation += float(delta.x);        _yRotation += float(delta.y);        _mousePosAtClick = event.GetPosition();        Refresh();    }}// This method draws the scene.void GLCanvas::Render(){	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	glLoadIdentity();	glTranslatef(0.0, 0.0, -3.0);    glRotatef(_yRotation, 1.f, 0.f, 0.f);/* orbit the X axis */	glTranslatef(0.0, -1.1, 0.0);    glRotatef(_xRotation, 0.f, 1.f, 0.f);/* orbit the Y axis */    theMoCapManager.render();}// Initialization of all OpenGL specific parameters.void GLCanvas::InitGL(){    SetCurrent(*_GLRC);	glClearColor(0.0, 0.0, 0.0, 0.0);    glEnable(GL_DEPTH_TEST);    glEnable(GL_POINT_SMOOTH);	glShadeModel(GL_SMOOTH);}