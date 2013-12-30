#ifndef MinibloqProperties__h
#define MinibloqProperties__h

#include "BubblePanel.h"
#include "BubbleCombo.h"

#include <wx/hashmap.h>


WX_DECLARE_STRING_HASH_MAP(wxString, LanguagesHash);


class Bubble;
class MinibloqProperties : public BubblePanel
{
    private:
        //##See if have to add a private copy constructor to avoid accidental copying.
        wxWindow* parent;
        Bubble *bubble;

        wxString languagePath;
        LanguagesHash languageName2Code;
        LanguagesHash languageCode2Name;

        wxStaticText *lblLanguage;
        BubbleCombo *comboLanguage;
        wxButton *buttonReloadLanguages;

        void onButtonReloadLanguagesLeftDown(wxMouseEvent& event);


    protected:
        void onComboLanguageChanged(wxCommandEvent &event);
        int loadLanguages();
        void selectDefaultLanguage();
        void changeLanguage();


    public:
        MinibloqProperties( wxWindow* parent,
                            wxWindowID id,
                            Bubble *const bubble,
                            const wxString& languagePath,
                            const wxColour& colour,
                            const wxPoint& pos = wxDefaultPosition,
                            const wxSize& size = wxDefaultSize,
                            long style = wxTAB_TRAVERSAL,
                            const wxString& name = "MinibloqProperties"
                          );
        virtual ~MinibloqProperties();

        void updateGUI();
        inline void setLanguageSelection(const wxString& value)
        {
            if (comboLanguage)
                comboLanguage->setSelection(value);
        }
        inline wxString getLanguageSelection() const
        {
            if (comboLanguage)
                return comboLanguage->getText();
            return wxEmptyString;
        }


        //##void onSize(wxSizeEvent& event);

        //##DECLARE_EVENT_TABLE()
};
#endif
