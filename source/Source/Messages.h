#ifndef Messages__h
#define Messages__h


#include "BubblePanel.h"
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/gauge.h>

class Messages : public BubblePanel
{
    private:
        wxFlexGridSizer *sizer;
        //##wxBoxSizer *sizer;
        wxTextCtrl* txtMsg;
        wxGauge *progress;

        //##Ver si se agrega constructor de copia privado para evitar la copia accidental.

    public:
        Messages(   wxWindow* parent,
                    wxWindowID id,
                    const wxColour& colour,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize,
                    long style = wxTAB_TRAVERSAL,
                    const wxString& name = "Messages"
                );
        virtual ~Messages();

        void updateGUI();

        //##Por ahora, se accede directamente al text control y otros elementos:
        inline wxTextCtrl* getTxtMsg() const
        {
            return txtMsg;
        }

        inline wxGauge* getProgress() const
        {
            return progress;
        }

        void setTextColour(const wxColour& colour);

        //##void onSize(wxSizeEvent& event);

        //##DECLARE_EVENT_TABLE()
};
#endif
