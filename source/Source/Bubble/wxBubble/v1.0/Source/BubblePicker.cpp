#include "BubblePicker.h"

//##Todo esto cambiará cuando implemente los tipos de datos definidos por el target, en runtime:

const wxWindowID BubblePicker::ID_Buttons = wxNewId();


BEGIN_EVENT_TABLE(BubblePicker, wxDialog)
    EVT_CLOSE(BubblePicker::onClose)
    //EVT_LEAVE_WINDOW(BubblePicker::onMouseLeave)
    //EVT_KILL_FOCUS(BubblePicker::onKillFocus)
    //EVT_ACTIVATE(BubblePicker::onActivate)
END_EVENT_TABLE()


//////////////////////////////////////////////////////////////////////////////////
//
// BubblePicker
//
//////////////////////////////////////////////////////////////////////////////////

//##Implementar:
//Manejo de teclado, para salir con ESC, y para poder agregar bloques con sus teclas (y que muestre las teclas,
//de los bloques (que serán secuencias de varias teclas, seguramente) como por ejemplo algunos programas ahora
//que al presionar ALT muestran las teclas de cada cosa).

//##Optimizar: Creería que el parámetro "colour" finalmente será inútil, así que no habrá que pasarlo más:
BubblePicker::BubblePicker(wxWindow* parent,
                           Bubble *const bubble,
                           wxWindowID id,
                           const wxString& dataType,
                           const wxString& caption,
                           const unsigned int buttonsSide,
                           const wxColour& colour,
                           const unsigned int cols) : wxDialog( parent,
                                                                id,
                                                                wxString(caption),
                                                                wxDefaultPosition,
                                                                wxDefaultSize,
                                                                wxCAPTION
                                                                //##:
                                                                //| wxRESIZE_BORDER //wxCAPTION //wxDEFAULT_DIALOG_STYLE //wxRESIZE_BORDER
                                                             ), //##Traducir...
                                                    /*##Ver si en el futuro se puede "resizear":
                                                    wxDialog( parent,
                                                              id,
                                                              wxString("Blocks"), //##Traducciones...
                                                              wxDefaultPosition,
                                                              wxDefaultSize,
                                                              wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER,
                                                              wxString("BlockPicker")
                                                            ),
                                                    */
                                                    parent(parent),
                                                    bubble(bubble),
                                                    dataType(dataType),
                                                    buttonsCount(0),
                                                    buttonsWidth(buttonsSide), //##Ver estos parámetros, si pasan a constantes...
                                                    buttonsHeight(buttonsSide),
                                                    buttonsHGap(4), //##
                                                    buttonsVGap(4), //##
                                                    sizerMain(NULL),
                                                    buttonPressed(false),
                                                    closeAfterPick(false),
                                                    cols(cols)
                                                    //activated(false)
{
    Hide();

    SetBackgroundColourAndRefresh(colour);

    sizerMain = new wxFlexGridSizer(1, 1, buttonsVGap, buttonsHGap);
    SetSizer(sizerMain);
}


BubblePicker::~BubblePicker()
{
    //##No va: Como los elementos de la lista son descendientes de wxPanel, y tienen sus parents,
    //el frame o dialog al que pertenecen los destruirá. Y aquí, como el objeto canvases es estático,
    //se asegura la llamada a su destructor sin hacer nada:
    //canvases.Clear();
}


BubblePanel *BubblePicker::getFunctionPanel(const wxString& function)
{
    //Finds a child window with the function as name:
    wxWindow *window = wxWindow::FindWindowByName(function, this);
    if (window)
    {
        if ( window->IsKindOf(CLASSINFO(BubblePanel)) ) //This is not strictly necessary, but it's safer.
            return (BubblePanel *)window; //##Improve casts with predefined C++ casts.
    }
    return NULL;
}


void BubblePicker::addButton(const wxString& name,
                             const wxString& function,
                             const wxString& returnType,
                             const wxString& tooltip,
                             const wxColour& backgroundColor,
                             const wxImage& imgDefault,
                             const wxImage& imgPressed,
                             const wxImage& imgHover,
                             const wxImage& imgDisabled)
{
    long buttonId = wxNewId();
    wxGridSizer *functionPanelSizer = NULL;
    BubblePanel *functionPanel = getFunctionPanel(function);

    //If a functionPanel for this function doesn't exist yet, create one:
    if (functionPanel == NULL)
    {
        //wxMessageDialog dialog0(parent, function, _("0")); //##Debug
        //dialog0.ShowModal(); //##Debug

        functionPanelSizer = new wxGridSizer(cols, buttonsVGap, buttonsHGap);
        functionPanel = new BubblePanel(this,
                                        wxNewId(),
                                        backgroundColor,    //Function panels adopt the colour passed for
                                                            //the first button created for this function.
                                        true,
                                        wxDefaultPosition,
                                        wxDefaultSize,
                                        wxTAB_TRAVERSAL,
                                        function            //VERY IMPORTANT: The function is stored in
                                                            //the panel's name!
                                       );
        if (functionPanel)
        {
            if (functionPanelSizer)
                functionPanel->SetSizer(functionPanelSizer);
            if (sizerMain)
            {
                sizerMain->Add(functionPanel, wxSizerFlags().Expand().Bottom());
            }
        }
    }
    //This is not an else!:
    if (functionPanel)
    {
        functionPanelSizer = (wxGridSizer *)functionPanel->GetSizer(); //##Future: use KindOf to protect the cast!
        BubbleButton *button = new BubbleButton(functionPanel,
                                                buttonId,
                                                wxPoint(0, 0),
                                                wxSize(buttonsWidth, buttonsHeight));
        if (button)
        {
            button->setName(name);
            button->SetToolTip(tooltip);
            button->setFunction(function);
            button->setReturnDataType(returnType);
            button->setBackgroundColor(backgroundColor);
            button->setImageDefault(imgDefault);
            button->setImagePressed(imgPressed);
            button->setImageHover(imgHover);
            button->setImageDisabled(imgDisabled);

            //All the buttons share the same event handlers:
            button->Connect(wxEVT_LEFT_DOWN,
                            wxMouseEventHandler(BubblePicker::onLeftDown),
                            NULL,
                            this);
            button->Connect(wxEVT_LEFT_UP,
                            wxMouseEventHandler(BubblePicker::onLeftUp),
                            NULL,
                            this);
            button->Connect(wxEVT_LEFT_DCLICK,
                            wxMouseEventHandler(BubblePicker::onLeftDoubleClick),
                            NULL,
                            this);
            button->Connect(wxEVT_LEAVE_WINDOW,
                            wxMouseEventHandler(BubblePicker::onLeaveWindow),
                            NULL,
                            this);
            buttonsCount++;

            //##Test:
            //button->SetLabel(wxString("Button"));

            if (functionPanelSizer)
            {
                functionPanelSizer->Add(button, 1, wxSHAPED | wxFIXED_MINSIZE);
                functionPanelSizer->Fit(functionPanel);
            }
        }
    }
    if (sizerMain)
        SetSizerAndFit(sizerMain);
}


void BubblePicker::enableAllExceptTheseButtons(const wxSortedArrayString& buttonNames, bool value)
{
    wxWindowList& panels = GetChildren();
    wxWindowList::iterator iterPanels = panels.begin();
    for (iterPanels = panels.begin(); iterPanels != panels.end(); ++iterPanels)
    {
        wxWindow *panel = *iterPanels;
        if (panel)
        {
            if ( panel->IsKindOf(CLASSINFO(BubblePanel)) ) //This is not strictly necessary, but it's safer.
            {
                wxWindowList& buttons = panel->GetChildren();
                wxWindowList::iterator iterButtons = buttons.begin();
                for (iterButtons = buttons.begin(); iterButtons != buttons.end(); ++iterButtons)
                {
                    wxWindow *button = *iterButtons;
                    if (button)
                    {
                        if ( button->IsKindOf(CLASSINFO(BubbleButton)) ) //This is not strictly necessary, but it's safer.
                        {
                            //##Future: If a tests detects flickr in a platform, add an if here to test if the
                            //button is already in the state to which this function will be changing it:
                            if (value)
                                button->Enable( buttonNames.Index(((BubbleButton *)button)->getName()) == wxNOT_FOUND );
                            else
                                button->Enable( buttonNames.Index(((BubbleButton *)button)->getName()) != wxNOT_FOUND );
                        }
                    }
                }
            }
        }
    }
}


void BubblePicker::onLeftUp(wxMouseEvent& event)
{
    if (closeAfterPick)
    {
        Show(false);
        buttonPressed = false;
        if (parent)
        {
            parent->Show(true);
//##Doesn't work by now:
//            if (bubble)
//            {
//                if (bubble->getCurrentCanvas())
//                {
//                    if ( bubble->getCurrentCanvas()->getCurrentBlock() )
//                        bubble->getCurrentCanvas()->getCurrentBlock()->SetFocus();
//                }
//            }
        }
    }
    //event.Skip();
}


void BubblePicker::onLeftDoubleClick(wxMouseEvent& event)
{
    //In the base class, do nothing:
    event.Skip();
}


void BubblePicker::onLeftDown(wxMouseEvent& event)
{
    if (bubble)
    {
        if (bubble->getCurrentCanvas())
        {
            //##Acá se puede agregar un nivel más de seguridad, con un KindOf, como se hizo en el evento
            //de  mainFrame (buscar "kindof" en el mainFrame y verlo):
            BubbleButton *button = (BubbleButton *) event.GetEventObject();
            if (button)
            {
                if ( bubble->blockIsValid(button->getName(), button->getFunction()) )
                {
                    bubble->getCurrentCanvas()->addBlock( bubble->getBlockInfo(button->getName(), button->getFunction()) );
                }
            }
        }
    }
    buttonPressed = true;
    event.Skip();
}


void BubblePicker::onLeaveWindow(wxMouseEvent& event)
{
    //This is the case when the user adds a block by pressing the left button, but, with the button still
    //pressed, leaves the button area (I do not want to do the Show(false) in the LeftUp event because the
    //visual effect of the button being pressed is not complete, so this way looks nicer):
    if (buttonPressed)
    {
        if (closeAfterPick)
        {
            buttonPressed = false;
            Show(false);
            if (parent)
            {
                parent->Show(true);
                //parent->SetFocus();
            }
        }
    }
    event.Skip();
}


void BubblePicker::SetBackgroundColourAndRefresh(const wxColour& colour)
{
    SetBackgroundColour(colour);

    //This is to do the repainting inmediatly:
    Refresh();
    Update();
}


void BubblePicker::onKillFocus(wxFocusEvent& event)
{
    //Show(false);
//    SetBackgroundColourAndRefresh(wxColour(255, 0, 0));
    event.Skip();
}


void BubblePicker::onActivate(wxActivateEvent& event)
{
/*
    if (event.GetActive())
    {
        activated = true;
    }
    else if (activated)
    {
        activated = false;
        Show(false);
        //SetBackgroundColourAndRefresh(wxColour(255, 0, 0));
    }
*/
    event.Skip();
}


void BubblePicker::onClose(wxCloseEvent &event)
{
    //##Más adelante, se puede hacer que si se trata de cerrar este picker (lo cual sólo puede ocurrir por un
    //Alt+F4, ya que no tiene botón de cierre, pase estas teclas al Frame, para cerrar la aplicación, porque
    //a veces pasa que el usuario presiona Alt+F4 cuando recién usó al picker y la intención es precisamente
    //cerrar la aplicación, pero el picker captura el evento acá y por lo tanto no ocurre nada.
    event.Veto();
}


/*No effect:
void BubblePicker::onMouseLeave(wxMouseEvent& event)
{
    if (bubble)
    {
        if (bubble->getCurrentCanvas())
            bubble->getCurrentCanvas()->SetFocus();
    }
    event.Skip();
}
*/


//////////////////////////////////////////////////////////////////////////////////
//
// BubbleActionPicker
//
//////////////////////////////////////////////////////////////////////////////////
void BubbleActionPicker::onLeftUp(wxMouseEvent& event)
{
    if (buttonPressed)
    {
        if (bubble)
        {
            if (bubble->getCurrentCanvas())
            {
                //##Acá se puede agregar un nivel más de seguridad, con un KindOf, como se hizo en el evento
                //de  mainFrame (buscar "kindof" en el mainFrame y verlo):
                BubbleButton *button = (BubbleButton *) event.GetEventObject();
                if ( bubble->blockIsValid(button->getName(), button->getFunction()) )
                {
                    bubble->getCurrentCanvas()->addBlock( bubble->getBlockInfo(button->getName(), button->getFunction()) );
                }
            }
        }
        if (closeAfterPick)
        {
            Show(false);
        }
    }
    buttonPressed = false;
    event.Skip();
}


void BubbleActionPicker::onLeftDown(wxMouseEvent& event)
{
//    if (bubble)
//    {
//        if (bubble->getCurrentCanvas())
//        {
//            //##Acá se puede agregar un nivel más de seguridad, con un KindOf, como se hizo en el evento
//            //de  mainFrame (buscar "kindof" en el mainFrame y verlo):
//            BubbleButton *button = (BubbleButton *) event.GetEventObject();
//            if ( bubble->blockIsValid(button->getName(), button->getFunction()) )
//            {
//                bubble->getCurrentCanvas()->addBlock( bubble->getBlockInfo(button->getName(), button->getFunction()) );
//            }
//        }
//    }

    //##Reimplementar esto de alguna manera:
    //##No me gusta esto, porque depende de lo heredado (osea que hay que colocarlo en true siempre que se sobrecargue esta función):
    buttonPressed = true;

    event.Skip();
}


void BubbleActionPicker::onLeftDoubleClick(wxMouseEvent& event)
{
    onLeftUp(event);
    buttonPressed = true; //Accelerates the speed loose due to the buttonPressed logic? Apparently not...
    event.Skip();
}


//////////////////////////////////////////////////////////////////////////////////
//
// BubbleExpressionPicker
//
//////////////////////////////////////////////////////////////////////////////////
//void BubbleExpressionPicker::onLeftUp(wxMouseEvent& event)
//{
//    if (bubble)
//    {
//        if (bubble->getCurrentCanvas())
//        {
//            //##Acá se puede agregar un nivel más de seguridad, con un KindOf, como se hizo en el evento
//            //de  mainFrame (buscar "kindof" en el mainFrame y verlo):
//            BubbleButton *button = (BubbleButton *) event.GetEventObject();
//            if ( bubble->blockIsValid(button->getName(), button->getFunction()) )
//            {
//                bubble->getCurrentCanvas()->addParam( bubble->getBlockInfo(button->getName(), button->getFunction()), paramSlot );
//            }
//        }
//    }
//    if (closeAfterPick)
//    {
//        Show(false);
//        buttonPressed = false;
//    }
//    event.Skip();
//}

//##Testint to fix the cause for the event sent to the actionPicker when the button is released in an expression picker:
//void BubbleExpressionPicker::onLeftUp(wxMouseEvent& event)
//{
//    event.StopPropagation();
//}

void BubbleExpressionPicker::onLeftDown(wxMouseEvent& event)
{
    //##En los pickers de expresiones (parámetros), me gusta más que agregue acá en el down y no en el up, porque
    //en el up da la sensación de ser muy lento:
    if (bubble)
    {
        if (bubble->getCurrentCanvas())
        {
            //##Acá se puede agregar un nivel más de seguridad, con un KindOf, como se hizo en el evento
            //de  mainFrame (buscar "kindof" en el mainFrame y verlo):
            BubbleButton *button = (BubbleButton *) event.GetEventObject();
            if ( bubble->blockIsValid(button->getName(), button->getFunction()) )
            {
                bubble->getCurrentCanvas()->addParam( bubble->getBlockInfo(button->getName(), button->getFunction()), paramSlot );
            }
        }
    }

    //##Reimplementar esto de alguna manera:
    //##No me gusta esto, porque depende de lo heredado (osea que hay que colocarlo en true siempre que se sobrecargue esta función):
    buttonPressed = true;

    //event.Skip();
}
