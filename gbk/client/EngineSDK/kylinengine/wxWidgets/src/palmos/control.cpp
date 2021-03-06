/////////////////////////////////////////////////////////////////////////////
// Name:        src/palmos/control.cpp
// Purpose:     wxControl class
// Author:      William Osborne - minimal working wxPalmOS port
// Modified by: Wlodzimierz ABX Skiba - native implementation
// Created:     10/13/04
// RCS-ID:      $Id: control.cpp,v 1.14 2005/02/24 10:51:11 ABX Exp $
// Copyright:   (c) William Osborne, Wlodzimierz Skiba
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "control.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_CONTROLS

#ifndef WX_PRECOMP
    #include "wx/event.h"
    #include "wx/app.h"
    #include "wx/dcclient.h"
    #include "wx/log.h"
    #include "wx/settings.h"
#endif

#include "wx/control.h"
#include "wx/toplevel.h"
#include "wx/button.h"
#include "wx/checkbox.h"
#include "wx/tglbtn.h"
#include "wx/radiobut.h"
#include "wx/slider.h"

// ----------------------------------------------------------------------------
// wxWin macros
// ----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(wxControl, wxWindow)

BEGIN_EVENT_TABLE(wxControl, wxWindow)
    EVT_ERASE_BACKGROUND(wxControl::OnEraseBackground)
END_EVENT_TABLE()

// ============================================================================
// wxControl implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxControl ctor/dtor
// ----------------------------------------------------------------------------

void wxControl::Init()
{
    m_palmControl = false;
    m_palmField = false;
}

wxControl::~wxControl()
{
    SetLabel(wxEmptyString);
    m_isBeingDeleted = true;

    DestroyChildren();

    uint16_t index;
    FormType* form = GetObjectFormIndex(index);
    if(form!=NULL && index!=frmInvalidObjectId)
    {
        FrmRemoveObject((FormType **)&form,index);
    }
}

// ----------------------------------------------------------------------------
// control window creation
// ----------------------------------------------------------------------------

bool wxControl::Create(wxWindow *parent,
                       wxWindowID id,
                       const wxPoint& pos,
                       const wxSize& size,
                       long style,
                       const wxValidator& wxVALIDATOR_PARAM(validator),
                       const wxString& name)
{
    if ( !wxWindow::Create(parent, id, pos, size, style, name) )
        return false;

#if wxUSE_VALIDATORS
    SetValidator(validator);
#endif

    return true;
}

bool wxControl::PalmCreateControl(ControlStyleType style,
                                  const wxString& label,
                                  const wxPoint& pos,
                                  const wxSize& size,
                                  uint8_t groupID)
{
    FormType* form = GetParentForm();
    if(form==NULL)
        return false;


    wxCoord x = pos.x == wxDefaultCoord ? 0 : pos.x,
            y = pos.y == wxDefaultCoord ? 0 : pos.y,
            w = size.x == wxDefaultCoord ? 1 : size.x,
            h = size.y == wxDefaultCoord ? 1 : size.y;

    wxWindow *win = this;
    while(win->GetParent())
    {
        win = win->GetParent();
        wxPoint pt(win->GetClientAreaOrigin());
        x += pt.x;
        y += pt.y;
    }

    ControlType *control = CtlNewControl(
                               (void **)&form,
                               GetId(),
                               style,
                               wxEmptyString,
                               x,
                               y,
                               w,
                               h,
                               stdFont,
                               groupID,
                               true
                           );

    if(control==NULL)
        return false;

    m_palmControl = true;

    SetInitialBestSize(size);
    SetLabel(label);
    Show();
    return true;
}

bool wxControl::PalmCreateField(const wxString& label,
                                const wxPoint& pos,
                                const wxSize& size,
                                bool editable,
                                bool underlined,
                                JustificationType justification)
{
    FormType* form = GetParentForm();
    if(form==NULL)
        return false;

    m_label = label;

    wxCoord x = pos.x == wxDefaultCoord ? 0 : pos.x,
            y = pos.y == wxDefaultCoord ? 0 : pos.y,
            w = size.x == wxDefaultCoord ? 1 : size.x,
            h = size.y == wxDefaultCoord ? 1 : size.y;

    AdjustForParentClientOrigin(x, y);

    FieldType *field = FldNewField(
                           (void **)&form,
                           GetId(),
                           x,
                           y,
                           w,
                           h,
                           stdFont,
                           10,
                           editable,
                           underlined,
                           false,
                           false,
                           justification,
                           false,
                           false,
                           false
                       );

    if(field==NULL)
        return false;

    m_palmField = true;

    SetInitialBestSize(size);
    SetLabel(label);
    Show();
    return true;
}

// ----------------------------------------------------------------------------
// various accessors
// ----------------------------------------------------------------------------

FormType* wxControl::GetParentForm() const
{
    wxWindow* parentTLW = GetParent();
    while ( parentTLW && !parentTLW->IsTopLevel() )
    {
        parentTLW = parentTLW->GetParent();
    }
    wxTopLevelWindowPalm* tlw = wxDynamicCast(parentTLW, wxTopLevelWindowPalm);
    if(!tlw)
        return NULL;
    return tlw->GetForm();
}

FormType* wxControl::GetObjectFormIndex(uint16_t& index) const
{
    FormType* form = GetParentForm();
    if(form!=NULL)
        index = FrmGetObjectIndex(form, GetId());
    else
        index = frmInvalidObjectId;
    return form;
}

void* wxControl::GetObjectPtr() const
{
    uint16_t index;
    FormType* form = GetObjectFormIndex(index);
    if(form==NULL || index==frmInvalidObjectId)return NULL;
    return FrmGetObjectPtr(form,index);
}

wxBorder wxControl::GetDefaultBorder() const
{
    // we want to automatically give controls a sunken style (confusingly,
    // it may not really mean sunken at all as we map it to WS_EX_CLIENTEDGE
    // which is not sunken at all under Windows XP -- rather, just the default)
    return wxBORDER_SUNKEN;
}

void wxControl::SetIntValue(int val)
{
    FormType* form = GetParentForm();
    if(form==NULL)
        return;
    uint16_t index = FrmGetObjectIndex(form, GetId());
    if(index==frmInvalidObjectId)
        return;
    FrmSetControlValue(form, index, val);
}

void wxControl::SetBoolValue(bool val)
{
    SetIntValue(val?1:0);
}

bool wxControl::GetBoolValue() const
{
    FormType* form = GetParentForm();
    if(form==NULL)
        return false;
    uint16_t index = FrmGetObjectIndex(form, GetId());
    if(index==frmInvalidObjectId)
        return false;
    return ( FrmGetControlValue(form, index) == 1 );
}

wxSize wxControl::DoGetBestSize() const
{
    return wxSize(16, 16);
}

void wxControl::DoGetBounds( RectangleType &rect ) const
{
    FormType* form = GetParentForm();
    if(form==NULL)
        return;
    uint16_t index = FrmGetObjectIndex(form,GetId());
    if(index==frmInvalidObjectId)
        return;
    FrmGetObjectBounds(form,index,&rect);
}

void wxControl::DoSetBounds( RectangleType &rect )
{
    FormType* form = GetParentForm();
    if(form==NULL)
        return;
    uint16_t index = FrmGetObjectIndex(form,GetId());
    if(index==frmInvalidObjectId)
        return;
    FrmSetObjectBounds(form,index,&rect);
}

void wxControl::DoGetPosition( int *x, int *y ) const
{
    int ox = 0, oy = 0;
    AdjustForParentClientOrigin(ox, oy);

    RectangleType rect;
    DoGetBounds(rect);

    if(x)
        *x = rect.topLeft.x - ox;
    if(y)
        *y = rect.topLeft.y - oy;
}

void wxControl::DoGetSize( int *width, int *height ) const
{
    RectangleType rect;
    DoGetBounds(rect);

    if(width)
        *width = rect.extent.x;
    if(height)
        *height = rect.extent.y;
}

void wxControl::DoMoveWindow(int x, int y, int width, int height)
{
    wxRect area = GetRect();
    RectangleType rect;
    rect.topLeft.x = x;
    rect.topLeft.y = y;
    rect.extent.x = width;
    rect.extent.y = height;
    DoSetBounds(rect);
    GetParent()->Refresh(true, &area);
}

bool wxControl::Enable(bool enable)
{
    ControlType *control = (ControlType *)GetObjectPtr();
    if( !IsPalmControl() || (control == NULL))
        return false;
    if( CtlEnabled(control) == enable)
        return false;
    CtlSetEnabled( control, enable);
    return true;
}

bool wxControl::IsEnabled() const
{
    ControlType *control = (ControlType *)GetObjectPtr();
    if( !IsPalmControl() || (control == NULL))
        return false;
    return CtlEnabled(control);
}

bool wxControl::IsShown() const
{
    return StatGetAttribute ( statAttrBarVisible , NULL );
}

bool wxControl::Show( bool show )
{
    FormType* form = GetParentForm();
    if(form==NULL)
        return false;
    uint16_t index = FrmGetObjectIndex(form,GetId());
    if(index==frmInvalidObjectId)
        return false;
    if(show)
        FrmShowObject(form,index);
    else
        FrmHideObject(form,index);
    return true;
}

void wxControl::SetFieldLabel(const wxString& label)
{
    FieldType* field = (FieldType*)GetObjectPtr();
    if(field==NULL)
        return;

    uint16_t newSize = label.Length() + 1;
    MemHandle strHandle = FldGetTextHandle(field);
    FldSetTextHandle(field, NULL );
    if (strHandle)
    {
        if(MemHandleResize(strHandle, newSize)!=errNone)
            strHandle = 0;
    }
    else
    {
        strHandle = MemHandleNew( newSize );
    }
    if(!strHandle)
        return;

    char* str = (char*) MemHandleLock( strHandle );
    if(str==NULL)
        return;

    strcpy(str, label.c_str());
    MemHandleUnlock(strHandle);
    FldSetTextHandle(field, strHandle);
    FldRecalculateField(field, true);
}

void wxControl::SetControlLabel(const wxString& label)
{
    ControlType* control = (ControlType*)GetObjectPtr();
    if(control==NULL)
        return;
    CtlSetLabel(control,wxEmptyString);
    m_label = label;
    if(!m_label.empty())
        CtlSetLabel(control,m_label.c_str());
}

void wxControl::SetLabel(const wxString& label)
{
    if(IsPalmField())
        SetFieldLabel(label);

    // unlike other native controls, slider has no label
    if(IsPalmControl() && !wxDynamicCast(this,wxSlider))
        SetControlLabel(label);
}

wxString wxControl::GetFieldLabel()
{
    FieldType* field = (FieldType*)GetObjectPtr();
    if(field==NULL)
        return wxEmptyString;
    return FldGetTextPtr(field);
}

wxString wxControl::GetControlLabel()
{
    ControlType* control = (ControlType*)GetObjectPtr();
    if(control==NULL)
        return wxEmptyString;
    return CtlGetLabel(control);
}

wxString wxControl::GetLabel()
{
    if(IsPalmField())
        return GetFieldLabel();

    // unlike other native controls, slider has no label
    if(IsPalmControl() && !wxDynamicCast(this,wxSlider))
        return GetControlLabel();

    return wxEmptyString;
}

/* static */ wxVisualAttributes
wxControl::GetClassDefaultAttributes(wxWindowVariant WXUNUSED(variant))
{
    wxVisualAttributes attrs;

    // old school (i.e. not "common") controls use the standard dialog font
    // by default
    attrs.font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);

    // most, or at least many, of the controls use the same colours as the
    // buttons -- others will have to override this (and possibly simply call
    // GetCompositeControlsDefaultAttributes() from their versions)
    attrs.colFg = wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT);
    attrs.colBg = wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE);

    return attrs;
}

// another version for the "composite", i.e. non simple controls
/* static */ wxVisualAttributes
wxControl::GetCompositeControlsDefaultAttributes(wxWindowVariant WXUNUSED(variant))
{
    wxVisualAttributes attrs;
    attrs.font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    attrs.colFg = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT);
    attrs.colBg = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW);

    return attrs;
}

// ----------------------------------------------------------------------------
// message handling
// ----------------------------------------------------------------------------

bool wxControl::ProcessCommand(wxCommandEvent& event)
{
    return GetEventHandler()->ProcessEvent(event);
}

void wxControl::OnEraseBackground(wxEraseEvent& event)
{
}

#endif // wxUSE_CONTROLS
