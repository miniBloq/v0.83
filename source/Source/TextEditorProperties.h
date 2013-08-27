#ifndef TextEditorProperties__h
#define TextEditorProperties__h

#include "BubblePanel.h"


class TextEditorProperties : public BubblePanel
{
    private:
        //##Ver si se agrega constructor de copia privado para evitar la copia accidental.

    public:
        TextEditorProperties(   wxWindow* parent,
                                wxWindowID id,
                                const wxColour& colour,
                                const wxPoint& pos = wxDefaultPosition,
                                const wxSize& size = wxDefaultSize,
                                long style = wxTAB_TRAVERSAL,
                                const wxString& name = "TextEditorProperties"
                            );
        virtual ~TextEditorProperties();

        //##void onSize(wxSizeEvent& event);

        //##DECLARE_EVENT_TABLE()
};
#endif
