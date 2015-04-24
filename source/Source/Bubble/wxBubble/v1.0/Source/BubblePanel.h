#ifndef BubblePanel__h
#define BubblePanel__h

#include <wx/panel.h>

//##Esta clase irá incorporando las cosas comunes a todas las clases descendientes:
class BubblePanel : public wxPanel
{
//    protected:
//        //##Ver si se agrega constructor de copia privado para evitar la copia accidental.
//        wxColour backgroundColour;

    public:
        BubblePanel() : wxPanel() {} //##¿Es necesario llamar acá al constructor por defecto?
        BubblePanel(wxWindow* parent,
                    wxWindowID id,
                    const wxColour& colour,
                    bool visible,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize,
                    long style = wxTAB_TRAVERSAL,
                    const wxString& name = "BubblePanel"
                    );
        virtual ~BubblePanel();

        //##Este método se repite en casi todas las clases Bubble, así que pasará
        //a una clase intermedia cuando se termine la implementación de Bubble:
        void SetBackgroundColourAndRefresh(const wxColour& colour);
};
#endif
