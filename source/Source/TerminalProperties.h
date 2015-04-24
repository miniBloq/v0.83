#ifndef TerminalProperties__h
#define TerminalProperties__h

#include "BubblePanel.h"


class TerminalProperties : public BubblePanel
{
    private:
        //##Ver si se agrega constructor de copia privado para evitar la copia accidental.

    public:
        TerminalProperties( wxWindow* parent,
                            wxWindowID id,
                            const wxColour& colour,
                            const wxPoint& pos = wxDefaultPosition,
                            const wxSize& size = wxDefaultSize,
                            long style = wxTAB_TRAVERSAL,
                            const wxString& name = "TerminalProperties"
                          );
        virtual ~TerminalProperties();

        //##void onSize(wxSizeEvent& event);

        //##DECLARE_EVENT_TABLE()
};
#endif
