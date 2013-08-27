#ifndef ComponentProperties__h
#define ComponentProperties__h

#include "BubblePanel.h"


class ComponentProperties : public BubblePanel
{
    private:
        //##Ver si se agrega constructor de copia privado para evitar la copia accidental.

    public:
        ComponentProperties(    wxWindow* parent,
                                wxWindowID id,
                                const wxColour& colour,
                                const wxPoint& pos = wxDefaultPosition,
                                const wxSize& size = wxDefaultSize,
                                long style = wxTAB_TRAVERSAL,
                                const wxString& name = "ComponentProperties"
                           );
        virtual ~ComponentProperties();

        //##void onSize(wxSizeEvent& event);

        //##DECLARE_EVENT_TABLE()
};
#endif
