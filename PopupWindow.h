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


#ifndef POPUPWINDOW_H
#define POPUPWINDOW_H

#ifndef WX_PRECOMP
	//(*HeadersPCH(PopupWindow)
	#include <wx/panel.h>
	//*)
#endif
//(*Headers(SensorDataPanel)
//*)

#include <wx/popupwin.h>
#include <type_traits>


template <class T>
class PopupWindow : public wxPopupWindow
{
  public:
    PopupWindow() = delete;

    PopupWindow(wxWindow* parent, int flags = wxBORDER_NONE)
    {
        static_assert(std::is_base_of<wxPanel, T>::value, "Template parameter must be a wxPanel or have inherited from wxPanel");
        Create(parent, flags);
        _panel = new T(this);
        SetSize(_panel->GetSize());
    }

    virtual ~PopupWindow()
    {
        if (_panel != nullptr)
        {
            _panel->Destroy();
            _panel = nullptr;
        }
    }

    void setSize(int width, int height)
    {
        _panel->SetSize(width, height);
        SetSize(width, height);
    }

    T* getPanel() { return _panel; }
  protected:

  private:
    T* _panel;
};


template <class T>
class PopupTransientWindow : public wxPopupTransientWindow
{
  public:
    PopupTransientWindow() = delete;

    PopupTransientWindow(wxWindow* parent, int flags = wxBORDER_NONE)
    {
        static_assert(std::is_base_of<wxPanel, T>::value, "Template parameter must be a wxPanel or have inherited from wxPanel");
        Create(parent, flags);
        _panel = new T(this);
        SetSize(_panel->GetSize());
    }

    virtual ~PopupTransientWindow()
    {
        if (_panel != nullptr)
        {
            _panel->Destroy();
            _panel = nullptr;
        }
    }

    void setSize(int width, int height)
    {
        _panel->SetSize(width, height);
        SetSize(width, height);
    }

    T* getPanel() { return _panel; }
  protected:

  private:
    T* _panel;
};

#endif // POPUPWINDOW_H
