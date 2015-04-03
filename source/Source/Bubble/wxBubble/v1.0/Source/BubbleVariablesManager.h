#ifndef BubbleVariablesManager__h
#define BubbleVariablesManager__h


#include "BubblePanel.h"


class BubbleVariablesManager : public BubblePanel
{
    private:
        //##Ver si se agrega constructor de copia privado para evitar la copia accidental.

    public:
        BubbleVariablesManager( wxWindow* parent,
                                wxWindowID id,
                                const wxColour& colour,
                                const wxPoint& pos = wxDefaultPosition,
                                const wxSize& size = wxDefaultSize,
                                long style = wxTAB_TRAVERSAL,
                                const wxString& name = "BubbleVariablesManager"
                                );
        virtual ~BubbleVariablesManager();

        //##void onSize(wxSizeEvent& event);

        //##DECLARE_EVENT_TABLE()
};
#endif
