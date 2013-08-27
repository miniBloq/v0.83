#include "BubbleHardwareManager.h"
#include "Bubble.h"

#if defined (WIN32)
#include <windows.h>
#else
#include <string>
#include <vector>
#include "include/portscan.h"
#endif

BEGIN_EVENT_TABLE(BubbleHardwareManager, BubblePanel)
    EVT_SIZE(BubbleHardwareManager::onSize)
END_EVENT_TABLE()


BubbleHardwareManager::BubbleHardwareManager(   wxWindow* parent,
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
                                                                                    name
                                                                                   ),
                                                                        parent(parent),
                                                                        bubble(bubble),
                                                                        lblBootPortName(NULL),
                                                                        comboBootPortName(NULL),
                                                                        lblBoardName(NULL),
                                                                        comboBoardName(NULL),
                                                                        buttonReloadBlocks(NULL),
                                                                        buttonMainImage(NULL),
                                                                        emptyDummyString(wxString(""))
{
    if (bubble == NULL)
        return; //Nothing to do.

    //Hide();

    //##The following line crashes the application when the BubbleHardwareManager is closed by the user, so
    //##don't delete it, because it may be possibly a wxAUI's bug, and I would like to document it:
    //SetSize(400, 450);

    lblBoardName = new wxStaticText(   this,
                                        wxNewId(), //##
                                        //##Future: Make configurable by the user if he want to see these
                                        //shorcuts on screen or not:
                                        _("Hardware (Alt+W):"), //##Load shorcuts from XML.
                                        wxPoint(10, 10),
                                        wxSize(150, 20),
                                        wxALIGN_CENTRE,
                                        wxString("lblBoardName")
                                   );

    //##Esto no es el target completo, sino el "target hardware". Los targets tienen 2 partes:
    //1. Hardware.
    //2. API/Lenguaje.
    //En el futuro cercano, ambos se podrán seleccionar en Minibloq.
    comboBoardName = new BubbleCombo(   this,
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
                                        wxString("comboBoardName")
                                    );
    if (comboBoardName)
    {
        //##Un-hardcode everything (images, strings...):
        comboBoardName->setSorted(false);
        comboBoardName->append("DuinoBot.v1.x.HID",
                                new wxImage(bubble->getLibPath() + wxString("/CPP/Targets/DuinoBot.v1.x.HID_Arduino.v1.0/Images/DuinoBot.v1.x.HID.Thumb.20120113a.png")));
        comboBoardName->append("DuinoBot.v1.x",
                                new wxImage(bubble->getLibPath() + wxString("/CPP/Targets/DuinoBot.v1.x_Arduino.v1.0/Images/DuinoBot.v1.x.Thumb.20110920a.png")));
        comboBoardName->append("DuinoBot.Kids.v1.x",
                                new wxImage(bubble->getLibPath() + wxString("/CPP/Targets/DuinoBot.Kids.v1.x_Arduino.v1.0/Images/DuinoBot.Kids.v1.1.Thumb.20110920a.png")));
        comboBoardName->append("Seeeduino v2.2x Mega328",
                                new wxImage(bubble->getLibPath() + wxString("/CPP/Targets/Seeeduino.v2.2x.Mega328_Arduino.v1.0/Images/Seeeduino.v2.2x.Mega328.Thumb.20110920a.png")));
        comboBoardName->append("Seeeduino Mega 1280",
                                new wxImage(bubble->getLibPath() + wxString("/CPP/Targets/SeeeduinoMega1280_Arduino.v1.0/Images/SeeeduinoMega1280.Thumb.20110920a.png")));
        comboBoardName->append("Arduino Uno",
                                new wxImage(bubble->getLibPath() + wxString("/CPP/Targets/ArduinoUNO_Arduino.v1.0/Images/ArduinoUNO.Thumb.20110920a.png")));
        comboBoardName->append( "Arduino Duemilanove Mega328",
                                new wxImage(bubble->getLibPath() + wxString("/CPP/Targets/ArduinoDuemilanove328_Arduino.v1.0/Images/ArduinoDuemilanove328.Thumb.20110920a.png")));
        comboBoardName->append( "Arduino Duemilanove Mega168",
                                new wxImage(bubble->getLibPath() + wxString("/CPP/Targets/ArduinoDuemilanove168_Arduino.v1.0/Images/ArduinoDuemilanove168.Thumb.20110920a.png")));
        comboBoardName->append( "Arduino Mega 2560",
                                new wxImage(bubble->getLibPath() + wxString("/CPP/Targets/ArduinoMega2560_Arduino.v1.0/Images/ArduinoMega2560.Thumb.20110920a.png")));
        comboBoardName->append( "Arduino Mega 1280",
                                new wxImage(bubble->getLibPath() + wxString("/CPP/Targets/ArduinoMega1280_Arduino.v1.0/Images/ArduinoMega1280.Thumb.20110920a.png")));
        comboBoardName->append("ATTiny25 (with ArduinoISP)",
                                new wxImage(bubble->getLibPath() + wxString("/CPP/Targets/Tiny25.45.85_Arduino.v1.0/Images/Tiny25.Thumb.20110930a.png")));
        comboBoardName->append("ATTiny45 (with ArduinoISP)",
                                new wxImage(bubble->getLibPath() + wxString("/CPP/Targets/Tiny25.45.85_Arduino.v1.0/Images/Tiny45.Thumb.20110930a.png")));
        comboBoardName->append("ATTiny85 (with ArduinoISP)",
                                new wxImage(bubble->getLibPath() + wxString("/CPP/Targets/Tiny25.45.85_Arduino.v1.0/Images/Tiny85.Thumb.20110930a.png")));
        comboBoardName->append("ATTiny25 (with Doper)",
                                new wxImage(bubble->getLibPath() + wxString("/CPP/Targets/Tiny25.45.85_Arduino.v1.0/Images/Tiny25.Thumb.20110930a.png")));
        comboBoardName->append("ATTiny45 (with Doper)",
                                new wxImage(bubble->getLibPath() + wxString("/CPP/Targets/Tiny25.45.85_Arduino.v1.0/Images/Tiny45.Thumb.20110930a.png")));
        comboBoardName->append("ATTiny85 (with Doper)",
                                new wxImage(bubble->getLibPath() + wxString("/CPP/Targets/Tiny25.45.85_Arduino.v1.0/Images/Tiny85.Thumb.20110930a.png")));

        comboBoardName->Connect(wxEVT_COMMAND_TEXT_UPDATED,
                                wxCommandEventHandler(BubbleHardwareManager::onComboBoardNameChanged),
                                NULL,
                                this
                               );
        comboBoardName->setSelection(0); //##Load this from the "last config" file.
        //This is not necessary because the setSelection call triggers the onComboBoardNameChanged event,
        //which does set the Board Name (in fact, those lines will make the program startup slower):
        //if (bubble)
        //{
        //    bubble->setBoardName(comboBoardName->getText(), parent);
        //}
    }

    lblBootPortName = new wxStaticText( this,
                                        wxNewId(),
                                        //In the future there may be IP sockets, and other comm
                                        //channels, so here it's just "Port", not "Serial port":

                                        //##Future: Make configurable by the user if he want to see these
                                        //shorcuts on screen or not:
                                        _("Port (Alt+R):"), //##Load shorcuts from XML.
                                        wxPoint(210, 10),   //##Un-hardcode!
                                        wxSize(150, 20),    //##Un-hardcode!
                                        wxALIGN_CENTRE,
                                        wxString("lblBootSerialPortName")
                                       );

    comboBootPortName = new BubbleCombo(this,
                                        wxNewId(),
                                        wxImage(bubble->getThemePath() + wxString("/ComboSelectDefault.png")),
                                        wxImage(bubble->getThemePath() + wxString("/ComboSelectPressed.png")),
                                        wxImage(bubble->getThemePath() + wxString("/ComboSelectHover.png")),
                                        wxImage(bubble->getThemePath() + wxString("/ComboSelectDisabled.png")),
                                        false,
                                        wxPoint(210, 25),   //##Un-hardcode!
                                        wxSize(100, 16),    //##Un-hardcode!
                                        wxBORDER_SUNKEN|
                                        wxTRANSPARENT_WINDOW,
                                        wxString("comboBootSerialPortName")
                                       );
    if (comboBootPortName)
    {
        comboBootPortName->Connect( wxEVT_COMMAND_TOOL_CLICKED,
                                    wxCommandEventHandler(BubbleHardwareManager::onUpdatePorts),
                                    NULL,
                                    this
                                  );
        comboBootPortName->Connect( wxEVT_COMMAND_TEXT_UPDATED,
                                    wxCommandEventHandler(BubbleHardwareManager::onComboBootPortNameChanged),
                                    NULL,
                                    this
                                  );
        updatePorts();
        comboBootPortName->setSelection(0); //##Un-hardcode and get the port from the config file...
        wxString strCommRealName = wxString("//./") + comboBootPortName->getText();
        bubble->setBootPortName(strCommRealName);
    }

    buttonReloadBlocks = new wxButton(  this,
                                        wxNewId(),
                                        _("Reload blocks"),
                                        wxPoint(10, 55), //##Un-hardcode!
                                        //wxDefaultSize,
                                        wxSize(160, 25), //##Future: The autosize (with the label) feature does now work. Make it work!
                                        wxBU_EXACTFIT
                                     ); //##

    if (buttonReloadBlocks)
    {
        buttonReloadBlocks->Connect(wxEVT_LEFT_UP,
                                    wxMouseEventHandler(BubbleHardwareManager::onButtonReloadBlocksLeftUp),
                                    NULL,
                                    this
                                   );
        //buttonReloadBlocks->Hide(); //##Testing.
    }

    buttonMainImage = new BubbleButton( this,
                                        wxNewId(),
                                        wxPoint(0, 0),  //##Un-hardcode!
                                        wxSize(32, 32)  //##Un-hardcode!
                                      );
    //##buttonIcon->SetValidator(wxDefaultValidator);

    changeImage();

    //SetBackgroundColour(colour);
    SetBackgroundColourAndRefresh(colour);
}


BubbleHardwareManager::~BubbleHardwareManager()
{
}


void BubbleHardwareManager::updateGUI()
{
    if (lblBoardName)
        lblBoardName->SetLabel(_("Hardware (Alt+W):"));
    if (lblBootPortName)
        lblBootPortName->SetLabel(_("Port (Alt+R):"));

    if (buttonReloadBlocks)
        buttonReloadBlocks->SetLabel(_("Reload blocks"));
}


void BubbleHardwareManager::changeImage()
{
    if (bubble == NULL)
        return;
    if (buttonMainImage == NULL)
        return;

    //##Un-hardcode ALL OF THIS (both conditions and image paths)!
    if (bubble->getBoardName() == wxString("DuinoBot.v1.x.HID"))
    {
        buttonMainImage->setImageDefault(wxImage(bubble->getLibPath() + wxString("/CPP/Targets/DuinoBot.v1.x.HID_Arduino.v1.0/Images/DuinoBot.v1.x.HID.20120113a.png")));
    }
    else if (bubble->getBoardName() == wxString("DuinoBot.v1.x"))
    {
        buttonMainImage->setImageDefault(wxImage(bubble->getLibPath() + wxString("/CPP/Targets/DuinoBot.v1.x_Arduino.v1.0/Images/DuinoBot.v1.x.20110724a.png")));
    }
    else if (bubble->getBoardName() == wxString("DuinoBot.Kids.v1.x"))
    {
        buttonMainImage->setImageDefault(wxImage(bubble->getLibPath() + wxString("/CPP/Targets/DuinoBot.Kids.v1.x_Arduino.v1.0/Images/DuinoBot.Kids.v1.1.20110327a.png")));
    }

    else if (bubble->getBoardName() == wxString("Seeeduino v2.2x Mega328"))
    {
        buttonMainImage->setImageDefault(wxImage(bubble->getLibPath() + wxString("/CPP/Targets/Seeeduino.v2.2x.Mega328_Arduino.v1.0/Images/Seeeduino.v2.2x.Mega328.20110508b.png")));
    }
    else if (bubble->getBoardName() == wxString("Seeeduino Mega 1280"))
    {
        buttonMainImage->setImageDefault(wxImage(bubble->getLibPath() + wxString("/CPP/Targets/SeeeduinoMega1280_Arduino.v1.0/Images/SeeeduinoMega1280.20110916a.png")));
    }

    else if (bubble->getBoardName() == wxString("Arduino Uno"))
    {
        buttonMainImage->setImageDefault(wxImage(bubble->getLibPath() + wxString("/CPP/Targets/ArduinoUNO_Arduino.v1.0/Images/ArduinoUNO.20110325a.png")));
    }
    else if (bubble->getBoardName() == wxString("Arduino Duemilanove Mega168"))
    {
        buttonMainImage->setImageDefault(wxImage(bubble->getLibPath() + wxString("/CPP/Targets/ArduinoDuemilanove168_Arduino.v1.0/Images/ArduinoDuemilanove168.20110724a.png")));
    }
    else if (bubble->getBoardName() == wxString("Arduino Duemilanove Mega328"))
    {
        buttonMainImage->setImageDefault(wxImage(bubble->getLibPath() + wxString("/CPP/Targets/ArduinoDuemilanove328_Arduino.v1.0/Images/ArduinoDuemilanove328.20110724a.png")));
    }

    else if (bubble->getBoardName() == wxString("Arduino Mega 2560"))
    {
        buttonMainImage->setImageDefault(wxImage(bubble->getLibPath() + wxString("/CPP/Targets/ArduinoMega2560_Arduino.v1.0/Images/ArduinoMega2560.20110916a.png")));
    }
    else if (bubble->getBoardName() == wxString("Arduino Mega 1280"))
    {
        buttonMainImage->setImageDefault(wxImage(bubble->getLibPath() + wxString("/CPP/Targets/ArduinoMega1280_Arduino.v1.0/Images/ArduinoMega1280.20110916a.png")));
    }

    else if (bubble->getBoardName() == wxString("ATTiny25 (with ArduinoISP)"))
    {
        buttonMainImage->setImageDefault(wxImage(bubble->getLibPath() + wxString("/CPP/Targets/Tiny25.45.85_Arduino.v1.0/Images/Tiny25.ArduinoISP.20110930a.png")));
    }
    else if (bubble->getBoardName() == wxString("ATTiny45 (with ArduinoISP)"))
    {
        buttonMainImage->setImageDefault(wxImage(bubble->getLibPath() + wxString("/CPP/Targets/Tiny25.45.85_Arduino.v1.0/Images/Tiny45.ArduinoISP.20110930a.png")));
    }
    else if (bubble->getBoardName() == wxString("ATTiny85 (with ArduinoISP)"))
    {
        buttonMainImage->setImageDefault(wxImage(bubble->getLibPath() + wxString("/CPP/Targets/Tiny25.45.85_Arduino.v1.0/Images/Tiny85.ArduinoISP.20110930a.png")));
    }

    else if (bubble->getBoardName() == wxString("ATTiny25 (with Doper)"))
    {
        buttonMainImage->setImageDefault(wxImage(bubble->getLibPath() + wxString("/CPP/Targets/Tiny25.45.85_Arduino.v1.0/Images/Tiny25.Doper.20110930a.png")));
    }
    else if (bubble->getBoardName() == wxString("ATTiny45 (with Doper)"))
    {
        buttonMainImage->setImageDefault(wxImage(bubble->getLibPath() + wxString("/CPP/Targets/Tiny25.45.85_Arduino.v1.0/Images/Tiny45.Doper.20110930a.png")));
    }
    else if (bubble->getBoardName() == wxString("ATTiny85 (with Doper)"))
    {
        buttonMainImage->setImageDefault(wxImage(bubble->getLibPath() + wxString("/CPP/Targets/Tiny25.45.85_Arduino.v1.0/Images/Tiny85.Doper.20110930a.png")));
    }

    fit(GetSize());

    //This works better, because just refreshing the buttonMainImage sometimes redraws the combos without
    //their border:
    Update();
    Refresh();
    //buttonMainImage->Update();
    //buttonMainImage->Refresh();
}


void BubbleHardwareManager::onButtonReloadBlocksLeftUp(wxMouseEvent& event)
{
    if (bubble)
    {
        if (parent)
        {
//            if (buttonReloadBlocks)
//            {
//                buttonReloadBlocks->Update();
//                buttonReloadBlocks->Refresh();
//            }
            bubble->enableAllBlocks(false);
            bubble->loadBlocksInfo(parent, true);
            bubble->enableAllBlocks(true);
        }
    }
}


void BubbleHardwareManager::onSize(wxSizeEvent& event)
{
    fit(event.GetSize());
    Update();
    Refresh();
//    if (comboBootPortName)
//    {
//        //comboBootPortName->Raise();
//    }
    event.Skip();
}

void BubbleHardwareManager::fit(const wxSize& size)
{
    //##Crear un panel para alojar al comboBox del port, y que la imagen quede abajo: Además, armar todo esto
    //con un flexGrid o algo por el estilo:
    if (buttonMainImage)
    {
        //Margins:
        float margin = 0.025;
        int iconX = (int)(margin*size.GetWidth());
        int iconY = (int)(margin*size.GetHeight());
        if (comboBootPortName)
            iconY += comboBootPortName->GetSize().GetHeight()*2;

        int iconW = size.GetWidth() - 2*iconX;;
        int iconH = 0;

        //First, it tries to maximize witdh (including margins), and then calculates the corresponding height:
        if (buttonMainImage->getImageDefault().GetHeight() > 0)
        {
            float ratio = ((float)buttonMainImage->getImageDefault().GetHeight()/(float)buttonMainImage->getImageDefault().GetWidth());
            iconH = (int)(ratio*iconW);
        }

        //If the height is too large, maximizes height (including margins) to the available space, and recalculates the width:
        if ((iconH + 2*iconY) > (size.GetHeight() - 75)) //##
        {
            iconH = size.GetHeight() - 75 - 2*iconY;
            if (buttonMainImage->getImageDefault().GetWidth() > 0)
            {
                float ratio = ((float)buttonMainImage->getImageDefault().GetWidth()/(float)buttonMainImage->getImageDefault().GetHeight());
                iconW = (int)(ratio*iconH);
            }
        }

        //Finally, centers and scales the image:
        iconX = (size.GetWidth() / 2) - (iconW / 2);
        iconY = (size.GetHeight() / 2) - (iconH / 2);
        buttonMainImage->Move(iconX, iconY); //First moves the buttonMethod.
        buttonMainImage->SetSize(iconW, iconH);
        //buttonMainImage->Lower();
    }
}


//##@# Possible non-portable code:
bool BubbleHardwareManager::serialPortExists(const wxString& strPort)
{
#if defined (WIN32)
    HANDLE hFile = ::CreateFile(strPort.c_str(), GENERIC_READ |
                                GENERIC_WRITE, 0, NULL,
                                OPEN_EXISTING, 0, NULL);
    bool result = (hFile != INVALID_HANDLE_VALUE);
    CloseHandle(hFile);
    return result;
#else
    return TRUE;
#endif
}


void BubbleHardwareManager::updatePorts()
{
    if (comboBootPortName == NULL)
        return;
    comboBootPortName->clear();
#if defined (WIN32)
    //##Make the limit (by now 256) configurable, from an XML file (and see the API documetation too):
    //##Thanks to Alan Kharsansky for this code!:
    for (int portNumber = 1; portNumber < 256; ++portNumber)
    {
        wxString strCommRealName = wxString("//./COM");
        wxString strCommScreenReal = wxString("COM");
        strCommRealName << portNumber;
        strCommScreenReal << portNumber;
        if (serialPortExists(strCommRealName))
        {
            comboBootPortName->append(strCommScreenReal);
        }
    }
#else
    //##Thanks to Juan Pizarro for this code!:
    std::vector<std::string> ports;
    if( ctb::GetAvailablePorts(ports) )
    {
        for(int i=0; i < ports.size(); i++)
        {
            comboBootPortName->append(ports[i]);
        }
    }
#endif
}


void BubbleHardwareManager::setPortSelectorEnabled(bool value)
{
    if (comboBootPortName)
        comboBootPortName->Enable(value);
}


bool BubbleHardwareManager::getPortSelectorEnabled()
{
    if (comboBootPortName)
        return comboBootPortName->IsEnabled();
    return false;
}


wxString BubbleHardwareManager::getPortNameString()
{
    if (comboBootPortName)
        return comboBootPortName->getText();
    return emptyDummyString;
}


void BubbleHardwareManager::setPortNameString(const wxString& value)
{
    if (comboBootPortName)
        comboBootPortName->setText(value);
}


void BubbleHardwareManager::popUpPortList()
{
    if (IsShownOnScreen())
    {
        updatePorts();
        if (comboBootPortName)
            comboBootPortName->popUpList();
    }
}


void BubbleHardwareManager::popUpBoardList()
{
    if (IsShownOnScreen())
    {
        if (comboBoardName)
            comboBoardName->popUpList();
    }
}


void BubbleHardwareManager::onUpdatePorts(wxCommandEvent &event)
{
    updatePorts();
    event.Skip();
}


void BubbleHardwareManager::onComboBootPortNameChanged(wxCommandEvent &event)
{
    if (comboBootPortName)
    {
        if (bubble)
        {
            if (comboBootPortName->getText() == "HID") //##Unhardcode
            {
                bubble->setBootPortName("HID");
            }
            else
            {
                wxString strCommRealName = wxString("//./") + comboBootPortName->getText();
                bubble->setBootPortName(strCommRealName);
            }
        }
    }
}


void BubbleHardwareManager::onComboBoardNameChanged(wxCommandEvent &event)
{
    if (comboBoardName)
    {
        if (bubble)
        {
            bubble->setBoardName(event.GetString(), parent);
            changeImage();
        }
    }
}
