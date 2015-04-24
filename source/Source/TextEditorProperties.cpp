#include "TextEditorProperties.h"
#include "Bubble.h"

TextEditorProperties::TextEditorProperties( wxWindow* parent,
                                            wxWindowID id,
                                            Bubble *const bubble,
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
                                                                    //AERobot's calibration screen:
                                                                    sizerButtons(NULL),
                                                                    //panelButtons(NULL),
                                                                    buttonFwd(NULL),
                                                                    buttonFwdClear(NULL),
                                                                    buttonBack(NULL),
                                                                    buttonBackClear(NULL),
                                                                    buttonCW(NULL),
                                                                    buttonCWClear(NULL),
                                                                    buttonCCW(NULL),
                                                                    buttonCCWClear(NULL),
                                                                    buttonLine(NULL),
                                                                    buttonLineClear(NULL),
                                                                    buttonTest(NULL),
                                                                    buttonClearAll(NULL)

{
    const int cols = 5;
    const int rows = 8;
#if defined (WIN32)
    sizerButtons = new wxFlexGridSizer(cols, rows);
#else
    //##Future: Test this on Mac OS X:
    sizerButtons = new wxFlexGridSizer(rows, cols, 0, 0);
#endif
    if (sizerButtons)
    {
        SetSizer(sizerButtons);

        sizerButtons->AddGrowableRow(0);
        sizerButtons->AddGrowableRow(1);
        sizerButtons->AddGrowableRow(2);
        sizerButtons->AddGrowableRow(3);
        sizerButtons->AddGrowableRow(4);
        //sizerButtons->AddGrowableRow(5);

        sizerButtons->AddGrowableCol(0);
        sizerButtons->AddGrowableCol(1);
        sizerButtons->AddGrowableCol(2);
        sizerButtons->AddGrowableCol(3);
        sizerButtons->AddGrowableCol(4);
        //sizerButtons->AddGrowableCol(5);

        //Row 0:
        sizerButtons->AddStretchSpacer();
        sizerButtons->AddStretchSpacer();
        buttonFwd = new wxButton(this, wxNewId(), _("Forward"));
        if (buttonFwd)
        {
            sizerButtons->Add(buttonFwd, wxSizerFlags().Expand().Bottom());
            buttonFwd->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(TextEditorProperties::onButtonFwdClick), NULL, this);
        }
        sizerButtons->AddStretchSpacer();
        sizerButtons->AddStretchSpacer();

        //Row 1:
        sizerButtons->AddStretchSpacer();
        sizerButtons->AddStretchSpacer();
        buttonFwdClear = new wxButton(this, wxNewId(), _("Forward Clear"));
        if (buttonFwdClear)
        {
            sizerButtons->Add(buttonFwdClear, wxSizerFlags().Expand().Bottom());
            //##buttonFwdClear->Show(false);
            buttonFwdClear->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(TextEditorProperties::onButtonFwdClearClick), NULL, this);
        }
        sizerButtons->AddStretchSpacer();
        sizerButtons->AddStretchSpacer();

//        sizerButtons->AddStretchSpacer();
//        sizerButtons->AddStretchSpacer();
//        sizerButtons->AddStretchSpacer();
//        sizerButtons->AddStretchSpacer();
//        sizerButtons->AddStretchSpacer();

        //Row 2:
        buttonCCW = new wxButton(this, wxNewId(), _("CCW"));
        if (buttonCCW)
        {
            sizerButtons->Add(buttonCCW, wxSizerFlags().Expand().Bottom());
            buttonCCW->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(TextEditorProperties::onButtonCCWClick), NULL, this);
        }
        sizerButtons->AddStretchSpacer();
        sizerButtons->AddStretchSpacer();
        sizerButtons->AddStretchSpacer();
        buttonCW = new wxButton(this, wxNewId(), _("CW"));
        if (buttonCW)
        {
            sizerButtons->Add(buttonCW, wxSizerFlags().Expand().Bottom());
            buttonCW->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(TextEditorProperties::onButtonCWClick), NULL, this);
        }

        //Row 3:
        buttonCCWClear = new wxButton(this, wxNewId(), _("CCW Clear"));
        if (buttonCCWClear)
        {
            sizerButtons->Add(buttonCCWClear, wxSizerFlags().Expand().Bottom());
            buttonCCWClear->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(TextEditorProperties::onButtonCCWClearClick), NULL, this);
        }
        sizerButtons->AddStretchSpacer();
        sizerButtons->AddStretchSpacer();
        sizerButtons->AddStretchSpacer();
        buttonCWClear = new wxButton(this, wxNewId(), _("CW Clear"));
        if (buttonCWClear)
        {
            sizerButtons->Add(buttonCWClear, wxSizerFlags().Expand().Bottom());
            buttonCWClear->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(TextEditorProperties::onButtonCWClearClick), NULL, this);
        }

//        sizerButtons->AddStretchSpacer();
//        sizerButtons->AddStretchSpacer();
//        sizerButtons->AddStretchSpacer();
//        sizerButtons->AddStretchSpacer();
//        sizerButtons->AddStretchSpacer();

        //Row 4:
        sizerButtons->AddStretchSpacer();
        sizerButtons->AddStretchSpacer();
        buttonBack = new wxButton(this, wxNewId(), _("Back"));
        if (buttonBack)
        {
            sizerButtons->Add(buttonBack, wxSizerFlags().Expand().Bottom());
            buttonBack->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(TextEditorProperties::onButtonBackClick), NULL, this);
        }
        sizerButtons->AddStretchSpacer();
        sizerButtons->AddStretchSpacer();

        //Row 5:
        sizerButtons->AddStretchSpacer();
        sizerButtons->AddStretchSpacer();
        buttonBackClear = new wxButton(this, wxNewId(), _("Back Clear"));
        if (buttonBackClear)
        {
            sizerButtons->Add(buttonBackClear, wxSizerFlags().Expand().Bottom());
            buttonBackClear->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(TextEditorProperties::onButtonBackClearClick), NULL, this);
        }
        sizerButtons->AddStretchSpacer();
        sizerButtons->AddStretchSpacer();

        //Row 6:
        sizerButtons->AddStretchSpacer();
        sizerButtons->AddStretchSpacer();
        sizerButtons->AddStretchSpacer();
        sizerButtons->AddStretchSpacer();
        sizerButtons->AddStretchSpacer();

        //Row 7:
        buttonTest = new wxButton(this, wxNewId(), _("Test"));
        if (buttonTest)
        {
            sizerButtons->Add(buttonTest, wxSizerFlags().Expand().Bottom());
            buttonTest->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(TextEditorProperties::onButtonTestClick), NULL, this);
        }
        buttonClearAll = new wxButton(this, wxNewId(), _("Clear All"));
        if (buttonClearAll)
        {
            sizerButtons->Add(buttonClearAll, wxSizerFlags().Expand().Bottom());
            buttonClearAll->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(TextEditorProperties::onButtonClearAllClick), NULL, this);
        }
        sizerButtons->AddStretchSpacer();
        buttonLine = new wxButton(this, wxNewId(), _("Line"));
        if (buttonLine)
        {
            sizerButtons->Add(buttonLine, wxSizerFlags().Expand().Bottom());
            buttonLine->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(TextEditorProperties::onButtonLineClick), NULL, this);
        }
        buttonLineClear = new wxButton(this, wxNewId(), _("Line Clear"));
        if (buttonLineClear)
        {
            sizerButtons->Add(buttonLineClear, wxSizerFlags().Expand().Bottom());
            buttonLineClear->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(TextEditorProperties::onButtonLineClearClick), NULL, this);
        }

        sizerButtons->Fit(this);
    }
}


TextEditorProperties::~TextEditorProperties()
{
}

void TextEditorProperties::updateGUI()
{
    //##Future: For internationalization!
    /*
    if (lblLanguage)
        lblLanguage->SetLabel(_("Language:"));

    if (buttonReloadLanguages)
        buttonReloadLanguages->SetLabel(_("Reload languages"));
    */
}

/*##Ver si es necesario este evento...
void TextEditorProperties::onSize(wxSizeEvent& event)
{
    event.Skip();
}
*/

void TextEditorProperties::flashFile(wxString fileName)
{
    if (!bubble)
        return;

    wxArrayString output, errors;

    wxString cmd = "/avrdude.209130919.exe -p m168 -P usb -c usbasp -U \"flash:w:" +
                    bubble->getComponentsRepositoryPath() +
                    "/drivers/AERobot/v1.0/calibrate/bin/" +
                    fileName;

    wxExecute(bubble->getToolsPath() + cmd, output, errors);
    bubble->findErrorStringAndShow(errors);
    bubble->showStream(output, *wxWHITE);
}

void TextEditorProperties::onButtonFwdClick(wxMouseEvent& event)
{
    if (!bubble)
        return;
    if (bubble->getNotifier())
        bubble->getNotifier()->showMessage(wxString("Fwd..."), true, true, *wxBLUE); //##Debug.
    flashFile("calibrate_forward.hex:i\"");
    event.Skip();
}


void TextEditorProperties::onButtonFwdClearClick(wxMouseEvent& event)
{
    if (!bubble)
        return;
    if (bubble->getNotifier())
        bubble->getNotifier()->showMessage(wxString("Fwd Clear..."), true, true, *wxBLUE); //##Debug.
    flashFile("clear_forward.hex:i\"");
    event.Skip();
}


void TextEditorProperties::onButtonBackClick(wxMouseEvent& event)
{
    if (!bubble)
        return;
    if (bubble->getNotifier())
        bubble->getNotifier()->showMessage(wxString("Back..."), true, true, *wxBLUE); //##Debug.
    flashFile("calibrate_backwards.hex:i\"");
    event.Skip();
}


void TextEditorProperties::onButtonBackClearClick(wxMouseEvent& event)
{
    if (!bubble)
        return;
    if (bubble->getNotifier())
        bubble->getNotifier()->showMessage(wxString("Back Clear..."), true, true, *wxBLUE); //##Debug.
    flashFile("clear_backwards.hex:i\"");
    event.Skip();
}


void TextEditorProperties::onButtonCWClick(wxMouseEvent& event)
{
    if (!bubble)
        return;
    if (bubble->getNotifier())
        bubble->getNotifier()->showMessage(wxString("CW..."), true, true, *wxBLUE); //##Debug.
    flashFile("calibrate_cw.hex:i\"");
    event.Skip();
}

void TextEditorProperties::onButtonCWClearClick(wxMouseEvent& event)
{
    if (!bubble)
        return;
    if (bubble->getNotifier())
        bubble->getNotifier()->showMessage(wxString("CW Clear..."), true, true, *wxBLUE); //##Debug.
    flashFile("clear_cw.hex:i\"");
    event.Skip();
}

void TextEditorProperties::onButtonCCWClick(wxMouseEvent& event)
{
    if (!bubble)
        return;
    if (bubble->getNotifier())
        bubble->getNotifier()->showMessage(wxString("CCW..."), true, true, *wxBLUE); //##Debug.
    flashFile("calibrate_ccw.hex:i\"");
    event.Skip();
}


void TextEditorProperties::onButtonCCWClearClick(wxMouseEvent& event)
{
    if (!bubble)
        return;
    if (bubble->getNotifier())
        bubble->getNotifier()->showMessage(wxString("CCW Clear..."), true, true, *wxBLUE); //##Debug.
    flashFile("clear_ccw.hex:i\"");
    event.Skip();
}


void TextEditorProperties::onButtonLineClick(wxMouseEvent& event)
{
    if (!bubble)
        return;
    if (bubble->getNotifier())
        bubble->getNotifier()->showMessage(wxString("Line..."), true, true, *wxBLUE); //##Debug.
    flashFile("line_calibrate.hex:i\"");
    event.Skip();
}


void TextEditorProperties::onButtonLineClearClick(wxMouseEvent& event)
{
    if (!bubble)
        return;
    if (bubble->getNotifier())
        bubble->getNotifier()->showMessage(wxString("Line Clear..."), true, true, *wxBLUE); //##Debug.
    flashFile("clear_line.hex:i\"");
    event.Skip();
}


void TextEditorProperties::onButtonTestClick(wxMouseEvent& event)
{
    if (!bubble)
        return;
    if (bubble->getNotifier())
        bubble->getNotifier()->showMessage(wxString("Test..."), true, true, *wxBLUE); //##Debug.
    flashFile("test_calibration.hex:i\"");
    event.Skip();
}


void TextEditorProperties::onButtonClearAllClick(wxMouseEvent& event)
{
    if (!bubble)
        return;
    if (bubble->getNotifier())
        bubble->getNotifier()->showMessage(wxString("Clear All..."), true, true, *wxBLUE); //##Debug.
    flashFile("clear_calibration.hex:i\"");
    event.Skip();
}
