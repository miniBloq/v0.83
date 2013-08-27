#ifndef BlockEditorProperties__h
#define BlockEditorProperties__h

#include "BubblePanel.h"


class BlockEditorProperties : public BubblePanel
{
    private:
        //##Ver si se agrega constructor de copia privado para evitar la copia accidental.

    public:
        BlockEditorProperties(  wxWindow* parent,
                                wxWindowID id,
                                const wxColour& colour,
                                const wxPoint& pos = wxDefaultPosition,
                                const wxSize& size = wxDefaultSize,
                                long style = wxTAB_TRAVERSAL,
                                const wxString& name = "BlockEditorProperties"
                             );
        virtual ~BlockEditorProperties();

        //##void onSize(wxSizeEvent& event);

        //##DECLARE_EVENT_TABLE()
};
#endif
