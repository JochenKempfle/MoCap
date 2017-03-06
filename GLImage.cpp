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


#include "GLImage.h"


GLImage::GLImage()
{
    _id = 0;
    _scaleX = 1.0f;
    _scaleY = 1.0f;
    _visible = true;
}

GLImage::~GLImage()
{
    if (_id != 0)
    {
        glDeleteTextures(1, &_id);
    }
}


void GLImage::render() const
{
    if (!_visible || _id == 0)
    {
        return;
    }

    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _id);

    glPushMatrix();
    glTranslatef(_position.x(), _position.y(), _position.z());
    if (_billboarding)
    {
        float modelview[16];

        // get the current modelview matrix
        glGetFloatv(GL_MODELVIEW_MATRIX , modelview);

        // undo all rotations (all scaling is lost as well!)
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                if (i == j)
                {
                    modelview[i*4 + j] = 1.0f;
                }
                else
                {
                    modelview[i*4 + j] = 0.0f;
                }
            }
        }

        // set the modelview with no rotations and scaling
        glLoadMatrixf(modelview);
    }
    else
    {
        glRotatef(_rotation.x(), 1.0f, 0.0f, 0.0f);
        glRotatef(_rotation.y(), 0.0f, 1.0f, 0.0f);
        glRotatef(_rotation.z(), 0.0f, 0.0f, 1.0f);
    }
    glScalef(_scaleX, _scaleY, 1.0f);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glBegin(GL_QUADS);
      glColor3f(0.0f, 0.0f, 0.0f);

      glTexCoord2f(0.0f, 0.0f);
      glVertex2f(0.0f, 0.0f);

      glTexCoord2f(1.0f, 0.0f);
      glVertex2f(_width, 0.0f);

      glTexCoord2f(1.0f, 1.0f);
      glVertex2f(_width, -_height);

      glTexCoord2f(0.0f, 1.0f);
      glVertex2f(0.0f, -_height);
    glEnd();

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

void GLImage::loadImage(wxString filename)
{
    setImage(wxImage(filename));
}

void GLImage::setImage(const wxBitmap &bmp)
{
    setImage(bmp.ConvertToImage());
}

void GLImage::setImage(const wxImage &image)
{
    if (!image.IsOk())
    {
        return;
    }

    if (_id != 0)
    {
        glDeleteTextures(1, &_id);
        _id = 0;
    }

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &_id);

	glBindTexture(GL_TEXTURE_2D, _id);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    const unsigned int width = image.GetWidth();
    const unsigned int height = image.GetHeight();

    if (width > height)
    {
        _width = 1.0f;
        _height = float(height)/float(width);
    }
    else
    {
        _width = float(width)/float(height);
        _height = 1.0f;
    }

    GLubyte* rgb = image.GetData();
    if (image.HasAlpha())
    {
        GLubyte* alpha = image.GetAlpha();
        GLubyte* imageData = new GLubyte[width*height*4];
        if (imageData == nullptr || alpha == nullptr)
        {
            return;
        }

        GLubyte* pImageData = imageData;
        for (unsigned int h = 0; h < height; ++h)
        {
            for (unsigned int w = 0; w < width; ++w)
            {
                for (unsigned int i = 0; i < 3; ++i)
                {
                    *pImageData = *rgb;
                    ++pImageData;
                    ++rgb;
                }
                *pImageData = *alpha;
                ++pImageData;
                ++alpha;
            }
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

        delete imageData;
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, rgb);
    }

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
}




