#include "MinibloqProperties.h"
#include "MainFrame.h"

#include "Bubble.h"

#include <wx/dir.h>


MinibloqProperties::MinibloqProperties( wxWindow* parent,
                                        wxWindowID id,
                                        Bubble *const bubble,
                                        const wxString& languagePath,
                                        const wxColour& colour,
                                        const wxPoint& pos,
                                        const wxSize& size,
                                        long style,
                                        const wxString& name) : BubblePanel(parent,
                                                                            id,
                                                                            colour,
                                                                            true,
                                                                            pos,
                                                                            size,
                                                                            style,
                                                                            name),
                                                                parent(parent),
                                                                bubble(bubble),
                                                                languagePath(languagePath),
                                                                lblLanguage(NULL),
                                                                comboLanguage(NULL),
                                                                buttonReloadLanguages(NULL)
{
    //Hide();
    lblLanguage = new wxStaticText( this,
                                    wxNewId(), //##
                                    //##Future: Make configurable by the user if he want to see these
                                    //shorcuts on screen or not:
                                    _("Language:"), //##Load shorcuts from XML.
                                    //wxString(""),//##Debug.
                                    wxPoint(10, 10),
                                    wxSize(150, 20),
                                    wxALIGN_CENTRE,
                                    wxString("lblLanguage")
                                   );

    comboLanguage = new BubbleCombo(this,
                                    wxNewId(),
                                    wxImage(bubble->getThemePath() + wxString("/ComboSelectDefault.png")),
                                    wxImage(bubble->getThemePath() + wxString("/ComboSelectPressed.png")),
                                    wxImage(bubble->getThemePath() + wxString("/ComboSelectHover.png")),
                                    wxImage(bubble->getThemePath() + wxString("/ComboSelectDisabled.png")),
                                    false,
                                    wxPoint(10, 25), //##Make this automatic.
                                    wxSize(190, 16),
                                    //wxTAB_TRAVERSAL|
                                    //wxNO_BORDER|
                                    //wxBORDER_SIMPLE|
                                    //wxBORDER_RAISED|
                                    wxBORDER_SUNKEN|
                                    wxTRANSPARENT_WINDOW,
                                    //wxDefaultSize,
                                    //##wxALIGN_CENTRE,
                                    wxString("comboLanguage")
                                    );

    if (comboLanguage)
    {
        comboLanguage->setSorted(true);
        loadLanguages();
        selectDefaultLanguage();
        changeLanguage();
        comboLanguage->Connect( wxEVT_COMMAND_TEXT_UPDATED,
                                wxCommandEventHandler(MinibloqProperties::onComboLanguageChanged),
                                NULL,
                                this
                               );
    }

    buttonReloadLanguages = new wxButton(   this,
                                            wxNewId(),
                                            _("Reload languages"),
                                            wxPoint(10, 55), //##Un-hardcode!
                                            //wxDefaultSize,
                                            wxSize(160, 25), //##Future: The autosize (with the label) feature does now work. Make it work!
                                            wxBU_EXACTFIT
                                        );

    if (buttonReloadLanguages)
    {
        buttonReloadLanguages->Connect( wxEVT_LEFT_DOWN,
                                        wxMouseEventHandler(MinibloqProperties::onButtonReloadLanguagesLeftDown),
                                        NULL,
                                        this
                                      );
    }

    //##Patch (2012.04.30) for an unresolved bug: When updateGUI is called the first time, lblLanguage does not
    //load the correct string from the current dictionary, so it appears in English even when the current
    //preselected language is other:
    if (lblLanguage)
        lblLanguage->SetLabel(_("Language:"));

    //##SetBackgroundColourAndRefresh(colour);
}


MinibloqProperties::~MinibloqProperties()
{
}


void MinibloqProperties::onComboLanguageChanged(wxCommandEvent &event)
{
//##Delete this:
//    if (parent)
//    {
//        if ( !((MainFrame *)parent)->addLocaleCatalog("es.mo") ) //##Debug
//            wxLogWarning(_("Error #2: Can't load ")); //##Debug
//
//        //Update the complete GUI:
//        ((MainFrame *)parent)->updateGUI();
//    }

    changeLanguage();
}


int MinibloqProperties::loadLanguages()
{
    if (parent == NULL)
        return -1; //##Future: Standarize errors.

    if (comboLanguage == NULL)
        return -2; //##Future: Standarize errors.

    wxDir dir(languagePath);
    if ( !dir.IsOpened() )
        return -3; //##Future: Standarize errors.
    wxString fileName;

    //Loads the metadata for each language:
    bool result = dir.GetFirst(&fileName, wxEmptyString, wxDIR_FILES | wxDIR_HIDDEN);
    while (result)
    {
        //##:Future: A fastest way of doing all may be adding a metadata field to BubbleCombo, to avoid the use
        //of the hash table (so the language code can be stored there):
        if (fileName.AfterLast('.') == wxString("mo"))
        {
            if (fileName.BeforeLast('.').AfterLast('.') == wxString("Metadata"))
            {
                if ( ((MainFrame *)parent)->addLocaleCatalog(fileName) )
                {
                    wxString lanCode = fileName.BeforeLast('.').BeforeLast('.');
                    wxString lanName = _(lanCode + wxString(".Name"));
                    languageName2Code[_(lanName)] = lanCode;
                    languageCode2Name[_(lanCode)] = lanName;
                    comboLanguage->append(lanName);
                }
                else
                {
                    wxLogWarning(_("Error #2: Can't load ") + fileName);
                }
            }
        }
        result = dir.GetNext(&fileName);
    }

    return 0; //##Future: Standarize errors.
}


void MinibloqProperties::selectDefaultLanguage()
{
    //##Implementar: Es la función que averigua cuál es el lenguaje seleccionado por defecto en el sistema operativo...
    //##Por ahora selecciona el primer lenguaje:
    wxLanguageInfo lanInfo = *wxLocale::GetLanguageInfo(wxLocale::GetSystemLanguage());
    //wxLogWarning(lanInfo.CanonicalName.BeforeLast('_'));
    if ( !comboLanguage->setSelection(languageCode2Name[lanInfo.CanonicalName]) ) //Tries to load with the complete language and country name.
        if ( !comboLanguage->setSelection( languageCode2Name[lanInfo.CanonicalName.BeforeLast('_')]) ) //Tries to match only the lanuage code.
            if ( !comboLanguage->setSelection(wxString("en")) ) //Tries to load English.
                comboLanguage->setSelection(0); //Just loads the first entry from the combo...

}


void MinibloqProperties::changeLanguage()
{
    if (parent)
    {
        if (comboLanguage)
        {
            wxString catalog = languageName2Code[comboLanguage->getText()] + wxString(".mo");
            if ( !((MainFrame *)parent)->addLocaleCatalog(catalog) )
                wxLogWarning(_("Error #2: Can't load ") + catalog);

            //Update the complete GUI:
            ((MainFrame *)parent)->updateGUI();
        }
    }
}


void MinibloqProperties::updateGUI()
{
    if (lblLanguage)
        lblLanguage->SetLabel(_("Language:"));

    if (buttonReloadLanguages)
        buttonReloadLanguages->SetLabel(_("Reload languages"));
}


void MinibloqProperties::onButtonReloadLanguagesLeftDown(wxMouseEvent& event)
{
    if (comboLanguage == NULL)
        return;

    comboLanguage->clear();
    loadLanguages();
    event.Skip();
}


/*##Ver si es necesario este evento...
void MinibloqProperties::onSize(wxSizeEvent& event)
{
    event.Skip();
}
*/
