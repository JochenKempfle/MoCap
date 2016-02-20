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


#ifndef GLIMAGE_H
#define GLIMAGE_H

#include "Vector3.h"
#include <GL/gl.h>
#include <wx/image.h>
#include <wx/bitmap.h>


class GLImage
{
  public:
    GLImage();
    virtual ~GLImage();

    void render() const;
    void show(bool visible = true) { _visible = visible; }
    bool isVisible() const { return _visible; }

    void loadImage(wxString filename);
    void setImage(const wxBitmap &bmp);
    void setImage(const wxImage &image);

    void setPosition(Vector3 position) { _position = position; }
    void setPosition(float x, float y, float z) { _position = Vector3(x, y, z); }
    Vector3 getPosition() const { return _position; }

    void setRotation(Vector3 rotation) { _rotation = rotation; }
    void setRotation(float roll, float pitch, float yaw) { _rotation = Vector3(roll, pitch, yaw); }
    Vector3 getRotation() const { return _rotation; }

    void scale(float x, float y) { _scaleX = x; _scaleY = y; }
    float getScaleX() const { return _scaleX; }
    float getScaleY() const { return _scaleY; }

  protected:

  private:
    GLuint _id;
    Vector3 _position;
    Vector3 _rotation;
    float _scaleX;
    float _scaleY;
    float _width;
    float _height;
    bool _visible;
};

#endif // GLIMAGE_H
