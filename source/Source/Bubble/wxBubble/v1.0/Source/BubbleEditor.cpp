#include "BubbleEditor.h"

#include <wx/stc/stc.h>
#include "Bubble.h"

//##:
//#include <SString.h>
//#include <ScintillaWX.h>

BEGIN_EVENT_TABLE(BubbleEditor, wxStyledTextCtrl)
    //##EVT_SIZE(BubbleEditor::onSize)
    EVT_MOUSEWHEEL(BubbleEditor::OnMouseWheel)
    EVT_STC_CHANGE(wxID_ANY, BubbleEditor::OnChange)
END_EVENT_TABLE()


BubbleEditor::BubbleEditor( wxWindow *parent,
                            Bubble *bubble,
                            wxWindowID id,
                            const wxPoint &pos,
                            const wxSize &size,
                            long style
                          ): wxStyledTextCtrl(  parent,
                                                                id,
                                                                pos,
                                                                size,
                                                                style),
                                            bubble(bubble)
{
}


BubbleEditor::~BubbleEditor()
{
}


//##Ver si en la clase base este método es virtual o no, si no lo es, ver cómo hacer:
void BubbleEditor::OnMouseWheel(wxMouseEvent& evt)
{
    //##Completar esto:
    //  - Hacer que el zoom sea similar al del BubbleCanvas en incremento y límites.
    //  - Sincronizar esto con los notifiers de Bubble para actualizar el frame.
    //  - Luego agregar los comandos para tomar las teclas rápidas de zoom, etc..
    if (evt.m_controlDown)
    {
        wxMouseEvent newEvent = evt;
        newEvent.m_wheelRotation = -evt.m_wheelRotation;
        wxStyledTextCtrl::OnMouseWheel(newEvent);
    }
    else
    {
        wxStyledTextCtrl::OnMouseWheel(evt);
    }

    //Don't do this!!!:
    //evt.Skip();
}


void BubbleEditor::OnChange(wxStyledTextEvent& evt)
{
    if (bubble)
    {
        if (bubble->getNotifier())
        {
            bubble->getNotifier()->textChanged(this);
        }
    }
}
