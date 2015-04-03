#ifndef BubbleComponentsManager__h
#define BubbleComponentsManager__h


#include "BubblePanel.h"


class BubbleComponentsManager: public BubblePanel
{
    private:
        //##Ver si se agrega constructor de copia privado para evitar la copia accidental.

    public:
        BubbleComponentsManager(wxWindow* parent,
                                wxWindowID id,
                                const wxColour& colour,
                                const wxPoint& pos = wxDefaultPosition,
                                const wxSize& size = wxDefaultSize,
                                long style = wxTAB_TRAVERSAL,
                                const wxString& name = "BubbleComponentsManager"
                                );
        virtual ~BubbleComponentsManager();

        //##void onSize(wxSizeEvent& event);

        //##DECLARE_EVENT_TABLE()
};
#endif
