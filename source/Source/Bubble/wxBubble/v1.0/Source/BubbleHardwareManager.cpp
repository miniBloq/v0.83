#include "BubbleHardwareManager.h"
#include "Bubble.h"

#if defined (WIN32)
    #include <windows.h>
#endif


//##Make the limit configurable, from an XML file (and see the Windows and Linux API documetation too
//to avoid defining it smaller than possibly needed):
const int maxPorts = 256;


WX_DEFINE_OBJARRAY(arrayOfBoardProperties);


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
                                                                        currentBoardProperties(NULL),
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

    currentBoardProperties = new BubbleBoardProperties();

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
        comboBoardName->setSorted(false);
        bubble->loadHardwareTargets(this);

        BubbleBoardProperties *firstBoard = NULL;
        firstBoard = &(boardsProperties.Item(0));
        currentBoardProperties->set(firstBoard);

        comboBoardName->setSelection(0); //##Load this from the "last config" file.

        comboBoardName->Connect(wxEVT_COMMAND_TEXT_UPDATED,
                                wxCommandEventHandler(BubbleHardwareManager::onComboBoardNameChanged),
                                NULL,
                                this
                               );

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
        setPortType();
//        comboBootPortName->setSelection(0); //##Un-hardcode and get the port from the config file...
//        wxString strCommRealName = wxString("//./") + comboBootPortName->getText();
//        bubble->setBootPortName(strCommRealName);
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

    buttonReloadHardware = new wxButton(  this,
                                        wxNewId(),
                                        _("Reload hardware"),
                                        wxPoint(175, 55), //##Un-hardcode!
                                        //wxDefaultSize,
                                        wxSize(160, 25), //##Future: The autosize (with the label) feature does now work. Make it work!
                                        wxBU_EXACTFIT
                                     ); //##

    if (buttonReloadHardware)
    {
        buttonReloadHardware->Connect(wxEVT_LEFT_UP,
                                    wxMouseEventHandler(BubbleHardwareManager::onButtonButtonReloadHardwareLeftUp),
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


void BubbleHardwareManager::addBoard(BubbleBoardProperties *boardProperties)
{
    if (comboBoardName)
    {
        if (boardProperties)
        {
            boardsProperties.Add(boardProperties);
            comboBoardName->append( boardProperties->getName(),
                                    new wxImage(boardProperties->getPath() + wxString("/img/") +
                                                boardProperties->getImgThumb())
                                  );
        }
    }
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
    if (buttonMainImage == NULL)
        return;
    if (currentBoardProperties == NULL)
        return;

    buttonMainImage->setImageDefault(wxImage(currentBoardProperties->getPath() +
                                             wxString("/img/") + currentBoardProperties->getImgMain())
                                    );
    fit(GetSize());

    //This works better, because just refreshing the buttonMainImage sometimes redraws the combos without
    //their border:
    Update();
    Refresh();
    //buttonMainImage->Update();
    //buttonMainImage->Refresh();
}



void BubbleHardwareManager::onButtonButtonReloadHardwareLeftUp(wxMouseEvent& event)
{
    if (bubble)
    {
        if (parent)
        {
            bubble->loadHardwareTargets(this);
            comboBoardName->setSelection(0);
        }
    }
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
    return true;
#endif
}


void BubbleHardwareManager::updatePorts()
{
    if (comboBootPortName == NULL)
        return;
    comboBootPortName->clear();
    ports.clear();

#if defined (WIN32)
    //Thanks to Alan Kharsansky for this code!:
    for (int portNumber = 1; portNumber < maxPorts; ++portNumber)
    {
        wxString strCommRealName = wxString("//./COM");
        wxString strCommScreenReal = wxString("COM");
        strCommRealName << portNumber;
        strCommScreenReal << portNumber;
        if (serialPortExists(strCommRealName))
        {
            comboBootPortName->append(strCommScreenReal);
            ports.push_back(std::string(strCommRealName.mb_str()));
        }
    }
#else
    //Thanks to Juan Pizarro for this code!:
    if( ctb::GetAvailablePorts(ports) )
    {
        for(int i=0; i < ports.size(); i++)
        {
            comboBootPortName->append(ports[i]);
        }
    }
#endif
}


bool BubbleHardwareManager::getAvailablePorts(std::vector<std::string>& result)
{
#if defined (WIN32)
    //Thanks to Alan Kharsansky for this code!:
    for (int portNumber = 1; portNumber < maxPorts; ++portNumber)
    {
        wxString strCommRealName = wxString("//./COM");
        wxString strCommScreenReal = wxString("COM");
        strCommRealName << portNumber;
        strCommScreenReal << portNumber;
        if (serialPortExists(strCommRealName))
        {
            result.push_back(std::string(strCommScreenReal.mb_str()));
        }
    }
    return true;
#else
    //Thanks to Juan Pizarro for this code!:
    return ctb::GetAvailablePorts(result);
#endif
}


bool BubbleHardwareManager::findNewPort()
{
    //##Future: code this:

    newPort = wxString("");
    return false;
}


wxString BubbleHardwareManager::getNewPort()
{
    return newPort;
}


//##
//void BubbleHardwareManager::getPorts(std::vector<std::string>& result)
//{
//    result = ports;
//}


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


void BubbleHardwareManager::setPortType()
{
    if (comboBootPortName)
    {
        if (bubble)
        {
            //##In the future this will be more flexible, allowing for TCP communications, among other:
            if (comboBootPortName->getText() == wxString("HID"))
            {
                bubble->setBootPortName(wxString("HID"));
            }
            else
            {
                wxString strCommRealName = wxString("//./") + comboBootPortName->getText();
                bubble->setBootPortName(strCommRealName);
            }
        }
    }
}


void BubbleHardwareManager::onComboBootPortNameChanged(wxCommandEvent &event)
{
    setPortType();
}


void BubbleHardwareManager::onComboBoardNameChanged(wxCommandEvent &event)
{
    if (comboBoardName)
    {
        if (bubble)
        {
            //Find new seleted board's properties:
            BubbleBoardProperties *iterator = NULL;
            for (unsigned int i = 0; i < boardsProperties.GetCount(); i++)
            {
                iterator = &(boardsProperties.Item(i)); //##In theory, this is faster than the other index based form, but I'm not sure yet...
                if (iterator)
                {
                    if (iterator->getName() == event.GetString())
                    {
                        currentBoardProperties->set(iterator);
                    }
                }
            }
            bubble->setBoardName(event.GetString(), parent);
            changeImage();
            //##Debug:
            //wxMessageDialog dialog0(bubble->getParent(), getCurrentBoardProperties()->getPortType(),
            //                                             getCurrentBoardProperties()->getName()); //##Debug.
            //dialog0.ShowModal(); //##Debug.

            if ((getCurrentBoardProperties())->getPortType() == wxString("HID"))
            {
                setPortSelectorEnabled(false);
                setPortNameString((getCurrentBoardProperties())->getPortType());
            }
            else
            {
                setPortSelectorEnabled(true);
                setPortNameString(wxString(""));
                updatePorts();
            }

            //Updates the generated code (for example, with the include files):
            bubble->loadBoardRelations();
        }
    }
}
