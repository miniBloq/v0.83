/*
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
*/
#include <wx/app.h>
#include <wx/stdpaths.h>
#include "MainFrame.h"


//////////////////////////////////////////////////////////////////////
// Application
//////////////////////////////////////////////////////////////////////
class Minibloq : public wxApp
{
    protected:
        wxLocale locale;

	public:
		//##Constructor? It seems that right now the compiler uses the default constructor.

        bool OnInit()
        {
            //i18n/l10n:
            //20120603: Change suggested by Juan Pizarro to solve the i18n bug in some Windows and Linux machines:
            //if ( !locale.Init(wxLANGUAGE_DEFAULT, wxLOCALE_CONV_ENCODING) )
#if defined (linux)
            wxLogNull logNo;
#endif
            if ( !locale.Init(wxLANGUAGE_ENGLISH, wxLOCALE_CONV_ENCODING) )
                wxLogWarning(_("Error #1: This language is not supported by the system."));

            wxString lanPath(wxStandardPaths::Get().GetExecutablePath().BeforeLast(wxFileName::GetPathSeparator()) +
                             wxFileName::GetPathSeparator() + wxString("GUI") +
                             wxFileName::GetPathSeparator() + wxString("Texts") //##Unhardcode this in the future? I'm not sure...
                            );
            wxLocale::AddCatalogLookupPathPrefix(lanPath);
            wxString initialCatalogName("en.mo"); //This basic English catalog must exist.
            //wxString initialCatalogName("es.mo"); //##Debug...
            if (!locale.AddCatalog(initialCatalogName))
            {
                //##Future: See if future wxLocale implementations solve this problem:
                wxLogWarning(   _("Error #1: The installation path\n\n\"") + wxStandardPaths::Get().GetExecutablePath() +
                                _("\"\n\ncontains non-English chars.\n") +
                                _("Please try to install Minibloq on a location without this kind of chars. ") +
                                _("Otherwise, the program will run, but the translation system will not work properly.")
                            );
                //wxLogWarning(_("Error #2: Can't load ") + initialCatalogName);
            }

            /*## Future: Full screen:
            wxFrame* frame = new MainFrame(NULL,
                                         wxID_ANY,
                                         wxString("Minibloq v1.0"),
                                         wxDefaultPosition,
                                         wxSize(1280, 800),
                                         wxCLIP_CHILDREN);
            */

            //Código original, pero pasado a 1280 x 800:

            MainFrame* frame = NULL;
            frame = new MainFrame(  NULL,
                                    wxID_ANY,
                                    locale,
                                    lanPath,
                                    initialCatalogName,
                                    wxString("miniBloq v0.82.Beta"),
                                    wxDefaultPosition,
                                    //wxDefaultSize,
                                    wxSize(1280, 800), //##Load from XML config file...
                                    wxDEFAULT_FRAME_STYLE //|| wxMAXIMIZE
                                  );

            //frame->Centre(); //##Load from XML config file...
            //##This works, and can establish from the app start the size and position of the MainFrame:
            /*
            wxFrame* frame = new MainFrame(NULL,
                                         wxID_ANY,
                                         wxString("miniBloq v0.82"),
                                         wxPoint(100, 100),
                                         wxSize(500, 500));
            */

            if (frame)
            {
                SetTopWindow(frame);
                frame->Show();
                frame->Maximize(); //##Load from XML config file...
                if (argc > 0)
                {
                    wxString fileName = argv[1];
                    if (wxFile::Exists(fileName))
                    {
                        frame->loadFileComponent(fileName);
                    }
                }
            }

            return true;
        }
};

DECLARE_APP(Minibloq)
IMPLEMENT_APP(Minibloq)
