#include "AGE_TextControls.h"

#ifndef AGE_ComboBoxes_h
#define AGE_ComboBoxes_h

class AGEComboBox: public wxOwnerDrawnComboBox, public AGELinkedBox
{
public:
    AGEComboBox(wxWindow *parent):
    wxOwnerDrawnComboBox(parent, wxID_ANY, "", wxDefaultPosition, wxSize(0, 20), 0, NULL, wxCB_READONLY){}

    virtual void OnUpdate(wxCommandEvent&)=0;
    void enable(bool yes){Enable(yes);}

    AGETextCtrl *TextBox;
};

class ComboBox_Plus1: public AGEComboBox
{
public:
    ComboBox_Plus1(wxWindow *parent, AGETextCtrl *Pointer):
    AGEComboBox(parent)
    {
        TextBox = Pointer;
        TextBox->LinkedBoxes.push_front(this);
        Bind(wxEVT_COMMAND_COMBOBOX_SELECTED, &ComboBox_Plus1::OnUpdate, this);
    }

    void OnUpdate(wxCommandEvent &Event);
    void update(int value);
};

class ComboBox_EffectType: public AGEComboBox
{
public:
    ComboBox_EffectType(wxWindow *parent, AGETextCtrl *Pointer):
    AGEComboBox(parent)
    {
        TextBox = Pointer;
        TextBox->LinkedBoxes.push_front(this);
        Bind(wxEVT_COMMAND_COMBOBOX_SELECTED, &ComboBox_EffectType::OnUpdate, this);
    }

    void OnUpdate(wxCommandEvent &Event);
    void update(int value);
};

class ComboBox_EffectAttribute: public AGEComboBox
{
public:
    ComboBox_EffectAttribute(wxWindow *parent, AGETextCtrl *Pointer):
    AGEComboBox(parent)
    {
        TextBox = Pointer;
        TextBox->LinkedBoxes.push_front(this);
        Bind(wxEVT_COMMAND_COMBOBOX_SELECTED, &ComboBox_EffectAttribute::OnUpdate, this);
    }

    void OnUpdate(wxCommandEvent &Event);
    void update(int value);
};

#endif
