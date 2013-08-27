#ifndef BubbleEditor__h
#define BubbleEditor__h


#include <wx/stc/stc.h>


class BubbleEditor : public wxStyledTextCtrl
{
    private:
        //##Ver si se agrega constructor de copia privado para evitar la copia accidental.

    public:
        BubbleEditor(   wxWindow *parent,
                        wxWindowID id = wxID_ANY,
                        const wxPoint &pos = wxDefaultPosition,
                        const wxSize &size = wxDefaultSize,
                        long style = wxVSCROLL
                    );
        virtual ~BubbleEditor();

        inline void clear() { SetText(wxString("")); };
        void OnMouseWheel(wxMouseEvent& evt);

        DECLARE_EVENT_TABLE()
};
#endif
