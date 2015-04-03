#include "BubbleCanvas.h"

#include "Bubble.h"

#include <wx/dcclient.h>
#include <wx/valtext.h>
//#include <wx/regex.h>
#include <wx/tooltip.h>

#include <wx/msgdlg.h> //##Debug, se va en release. Agregar compilación condicional.


////////////////////////////////////////////////////////////////////////////////////////////////
//
// BubbleBlock
//
////////////////////////////////////////////////////////////////////////////////////////////////

//##2010.10.04: Ver que esto no dé error de runtime: Dejar este mensaje interno, aunque pasarlo a
//inglés, por si se reportara un bug al respecto en el futuro. Al menos dejarlo hasta ver bien cómo
//trabaja WX_DEFINE_OBJARRAY() y ver por qué en algunos compiladores, si se usa este macro sin tener
//en el scope la full-declaration de la clase de los elementos del array, ni siquiera dan warning:
WX_DEFINE_OBJARRAY(arrayOfBlocks);


//BEGIN_EVENT_TABLE(BubbleBlock, BubblePanel)
BEGIN_EVENT_TABLE(BubbleBlock, wxWindow)
    EVT_PAINT(BubbleBlock::onPaint) //##
    EVT_ERASE_BACKGROUND(BubbleBlock::onEraseBackground) //##
    EVT_LEFT_DOWN(BubbleBlock::onLeftDown)
    EVT_LEFT_UP(BubbleBlock::onLeftUp)
    EVT_RIGHT_DOWN(BubbleBlock::onRightDown)
    EVT_SIZE(BubbleBlock::onSize)

//    EVT_MOUSE_EVENTS(BubbleCanvas::OnMouseEvent)
END_EVENT_TABLE()


//##3D-Tests:
//wxImage *backImage = NULL;

BubbleBlock::BubbleBlock(   BubbleCanvas *parent,
                            wxWindowID id,
                            const wxPoint &position,
                            const wxSize &size,
                            const BubbleBlockInfo &info):
//                                                        wxControl(  parent,
//                                                                    id,
//                                                                    position,
//                                                                    size,
//                                                                    //##wxBORDER_SIMPLE
//                                                                    //wxTRANSPARENT_WINDOW //##
//                                                                    wxBORDER_NONE
//                                                                    //|wxCLIP_CHILDREN //##
//                                                                    //|wxFULL_REPAINT_ON_RESIZE  //##
//                                                                 ),
//                                                        wxWindow(   parent,
//                                                                    id,
//                                                                    position,
//                                                                    size,
//                                                                    //##wxBORDER_SIMPLE
//                                                                    //wxTRANSPARENT_WINDOW //##
//                                                                    wxBORDER_NONE
//                                                                    //|wxCLIP_CHILDREN //##
//                                                                    //|wxFULL_REPAINT_ON_RESIZE  //##
//                                                                 ),
                                                        BubblePanel(parent,
                                                                    id,
                                                                    info.getDefaultBackgroundColour1(),
                                                                    false, //This helps avoiding flickering.
                                                                    position,
                                                                    size,
                                                                    //##wxBORDER_SIMPLE
                                                                    wxBORDER_NONE
                                                                    //|wxCLIP_CHILDREN
                                                                    //|wxTRANSPARENT_WINDOW //##
                                                                   ),
                                                        id(id),
                                                        parent(parent),

                                                        enabled(true),
                                                        dragging(false),
                                                        paramsVisible(true),
                                                        commented(info.getCommented()),
                                                        selected(false),
                                                        selectionBorderWidth(4),

                                                        name(info.getName()),
                                                        function(info.getFunction()),
                                                        dataType(info.getDataType()),
                                                        componentStart(info.getComponentStart()),
                                                        blockStart(info.getBlockStart()),
                                                        variableInit(info.getVariableInit()),

                                                        blockIsDraggable(info.getIsDraggable()),
                                                        dontDeleteBrothers(info.getDontDeleteBrothers()),

                                                        loadAction(info.getLoadAction()),
                                                        code(info.getCode()),

                                                        defaultMethodName(info.getDefaultMethodName()), //##Esto va a cambiar cuando se agregue el soporte completo para métodos...
                                                        methodSeparator(info.getMethodSeparator()),

                                                        paramSeparator(info.getParamSeparator()),
                                                        paramListStart(info.getParamListStart()),
                                                        paramListEnd(info.getParamListEnd()),

                                                        backBlock(NULL),
                                                        backParamSlot(NULL),

                                                        blockButton(NULL),
                                                        methodButton(NULL),
                                                        allowDragOnMethodButton(info.getAllowDragOnMethodButton()),
                                                        centeredMethodButton(info.getCenteredMethodButton()),
                                                        //##returnButton(NULL),
                                                        addParamsButton(NULL),
                                                        removeParamsButton(NULL),

                                                        minParamsCount(info.getMinParamsCount()),

                                                        constantFieldEnableValidation(info.getConstantFieldEnableValidation()),
                                                        prevInstanceName(wxString("")),
                                                        constantField(NULL),
                                                        constantFieldLabel(NULL),
                                                        //##Esto se podría optimizar, compilando el string de validadión sólo cuando se carga el target, y no
                                                        //en la creación de cada bloque, de modo que se pueda mejorar la performance:
                                                        constantFieldValidator(info.getConstantFieldValidatorString(), wxRE_ADVANCED|wxRE_ICASE|wxRE_NOSUB),
                                                        constantFieldValidatorCharList(info.getConstantFieldValidatorCharList()),

                                                        instanceNameField(NULL),
                                                        instanceNameFieldSorted(info.getInstanceNameFieldSorted()),
                                                        userMustSelectInstanceName(info.getUserMustSelectInstanceName()),
                                                        instanceType(info.getInstanceType()),
                                                        updateVariables(info.getUpdateVariables()),

                                                        hasConstantField(info.getHasConstantField()),
                                                        constantFieldDefaultValue(info.getConstantFieldDefaultValue()),
//##
//                                                        constantFieldType(info.getConstantFieldType()),
//                                                        constantBoolDefaultValue(info.getConstantBoolDefaultValue()),
//                                                        constantNumber( info.getConstantNumber().getMin(), //##Use the copy constructor instead...
//                                                                        info.getConstantNumber().getMax(),
//                                                                        info.getConstantNumber().getDecimals(),
//                                                                        info.getConstantNumber().getDefaultValue()
//                                                                      ),
//                                                        constantStringMinLen(info.getConstantStringMinLen()),
//                                                        constantStringMaxLen(info.getConstantStringMaxLen()),
//                                                        constantStringDefaltValue(info.getConstantStringDefaltValue()),

                                                        hasHorizontalParams(info.getHasHorizontalParams()),

                                                        nextBlockIndentationTabs(info.getNextBlockIndentationTabs()),

                                                        brotherBlockInfo(NULL),
                                                        prevBrotherBlock(NULL),
                                                        nextBrotherBlock(NULL),
                                                        linkBlock(NULL),

                                                        originalSize(info.getOriginalSize()),
                                                        realSize(info.getOriginalSize()),
                                                        childFactor(info.getChildFactor()),

                                                        defaultBackgroundColour0(info.getDefaultBackgroundColour0()),
                                                        defaultBackgroundColour1(info.getDefaultBackgroundColour1()),

                                                        disabledBackgroundColour0(info.getDisabledBackgroundColour0()),
                                                        disabledBackgroundColour1(info.getDisabledBackgroundColour1()),

                                                        defaultMouseOverColour(info.getDefaultMouseOverColour()),
                                                        defaultErrorColour(info.getDefaultErrorColour()),
                                                        defaultEditableFieldsColour(info.getDefaultEditableFieldsColour()),
                                                        defaultSelectionBorderColour(info.getDefaultSelectionBorderColour()),

                                                        positionBeforeDragging(0, 0),
                                                        dragStartPos(0, 0)
{
    //To avoid flickering on creation:
    //Hide();

    //##Futuro: Optimizar para que no haya tantas copias temporales en memoria (aunque usando el stack en vez del heap es
    //más seguro todo, quizá lo deje así, sobre todo si funciona de manera robusta...

    //NOTES:
    //  1.  The initialization here is more secure with the default initialization before, plus we can check if
    //      the info param is not NULL.
    //  2.  To use less memory, we do not make a local non-temporal copy the info structure, it's uses only here, in
    //      the constructor.

    //##:
    //If there is a blockButton, a methodButton and less than 2 params, the existance of an instanceName modifies
    //the block's height:
//    if ( info.getHasInstanceNameField() && info.getHasBlockButton() && info.getHasMethodButton() && (info.getParamsCount() < 2) )

    if (info.getHasBlockButton())
    {
        blockButton = new BubbleButton( this,
                                        wxNewId(), //##
                                        getBlockButtonPos(),
                                        getBlockButtonSize()
                                      );

        //The blockButton is different: It may lack the Hover and Pressed images:
        if (blockButton)
        {
            //##Esta clase de operaciones se pueden luego hacer con constructores de copia, o algo por el estilo:
            //##Ver si esto es seguro, o si además tengo que verificar las images:
#if defined (linux)
            blockButton->setBackgroundColor(getDefaultBackgroundColour1());
#endif
            blockButton->setImageDefault(info.getBlockDefaultImage());
            if (info.hasBlockHoverImage())
                blockButton->setImageHover(info.getBlockHoverImage());
            if (info.hasBlockPressedImage())
                blockButton->setImagePressed(info.getBlockPressedImage());
            blockButton->setImageDisabled(info.getBlockDisabledImage());

            //##blockButton->SetToolTip(name);
            blockButton->SetToolTip(info.getToolTip());

            //¡This way of calling Connect() finally overcomes the Connect() bug!
            //¡ Thanks to http://www.zytrax.com/tech/survival/wxwidgets.html#connect !
            blockButton->Connect(   wxEVT_LEFT_DOWN,
                                    wxMouseEventHandler(BubbleBlock::onLeftDown),
                                    NULL,
                                    this
                                );
            blockButton->Connect(   wxEVT_RIGHT_DOWN,
                                    wxMouseEventHandler(BubbleBlock::onRightDown),
                                    NULL,
                                    this
                                );
        }
    }

    if (info.getHasMethodButton())
    {
        methodButton = new BubbleButton(this,
                                        wxNewId(), //##
                                        getMethodButtonPos(),
                                        getMethodButtonSize()
                                       );

        if (methodButton)
        {
            //##Esta clase de operaciones se pueden luego hacer con constructores de copia, o algo por el estilo:
            //##Ver si esto es seguro, o si además tengo que verificar las images:
#if defined (linux)
            methodButton->setBackgroundColor(getDefaultBackgroundColour1());
#endif
            methodButton->setImageDefault(info.getMethodDefaultImage());
            if (info.hasMethodHoverImage())
                methodButton->setImageHover(info.getMethodHoverImage());
            if (info.hasMethodPressedImage())
                methodButton->setImagePressed(info.getMethodPressedImage());
            methodButton->setImageDisabled(info.getMethodDisabledImage());

            //##Va el nombre del método, ya se verá cómo se hace esa parte:
            //methodButton->SetToolTip(name);

            setDefaultMethodName(getDefaultMethodName()); //##Esto va a cambiar cuando se agrege el soporte completo para métodos...

            if (getAllowDragOnMethodButton())
            {
                methodButton->Connect(  wxEVT_LEFT_DOWN,
                                        wxMouseEventHandler(BubbleBlock::onLeftDown),
                                        NULL,
                                        this
                                     );
            }
            else
            {
                methodButton->Connect(  wxEVT_LEFT_DOWN,
                                        wxMouseEventHandler(BubbleBlock::onMethodButtonLeftDown),
                                        NULL,
                                        this
                                     );
            }
            methodButton->Connect(  wxEVT_RIGHT_DOWN,
                                    wxMouseEventHandler(BubbleBlock::onRightDown),
                                    NULL,
                                    this
                                 );
            methodButton->Connect(  wxEVT_ENTER_WINDOW,
                                    wxMouseEventHandler(BubbleBlock::onMouseEnter),
                                    NULL,
                                    this
                                 );
        }
    }

//##:
//    if (info.getHasReturnButton())
//    {
//        returnButton = new BubbleButton(this,
//                                        wxNewId(),
//                                        getReturnButtonPos(),
//                                        getReturnButtonSize()
//                                       );
//        returnButton->setVisible(false); //##
//        setupButton(returnButton,
//                    info.getReturnDefaultImage(),
//                    info.getReturnHoverImage(),
//                    info.getReturnPressedImage(),
//                    info.getReturnDisabledImage()
//                   );
//    }

    if (info.getHasAddParamsButton())
    {
        addParamsButton = new BubbleButton( this,
                                            wxNewId(), //##
                                            getAddParamsButtonPos(),
                                            getAddParamsButtonSize()
                                            //wxSize(iconW/2, iconH/2) //##
                                           );
#if defined (linux)
//        addParamsButton->setBackgroundColor(getDefaultBackgroundColour1());
#endif
        addParamsButton->setImageDefault(info.getAddParamsDefaultImage());
        addParamsButton->setImageHover(info.getAddParamsHoverImage());
        addParamsButton->setImagePressed(info.getAddParamsPressedImage());
        addParamsButton->setImageDisabled(info.getAddParamsDisabledImage());
        addParamsButton->Connect(   wxEVT_LEFT_DOWN,
                                    wxMouseEventHandler(BubbleBlock::onAddParamsButtonLeftDown),
                                    NULL,
                                    this
                                );

        removeParamsButton = new BubbleButton(  this,
                                                wxNewId(), //##
                                                getRemoveParamsButtonPos(),
                                                addParamsButton->GetSize()
                                             );
#if defined (linux)
//        removeParamsButton->setBackgroundColor(getDefaultBackgroundColour1());
#endif
        removeParamsButton->setImageDefault(info.getRemoveParamsDefaultImage());
        removeParamsButton->setImageHover(info.getRemoveParamsHoverImage());
        removeParamsButton->setImagePressed(info.getRemoveParamsPressedImage());
        removeParamsButton->setImageDisabled(info.getRemoveParamsDisabledImage());
        removeParamsButton->Connect(wxEVT_LEFT_DOWN,
                                    wxMouseEventHandler(BubbleBlock::onRemoveParamsButtonLeftDown),
                                    NULL,
                                    this
                                   );
    }

    unsigned int index = 0;
    while (index < info.getParamsCount())
    {
        //##Testing:
        addParamSlot(   info.getParamName(index), //##Esto se va todo, pasa a dinámico con número de parámetros variable...
                        //wxString(""),
                        info.getParamDataType(index),
                        info.getParamDefaultImage(index),
                        info.getParamNotAssignedImage(index),
                        info.getParamDefaultImage(index), //##
                        info.getParamHoverImage(index),
                        info.getParamPressedImage(index),
                        info.getParamDisabledImage(index)
                    );
        index++;
    }


    if (getHasConstantField())
    {
        wxArrayString constantFieldIncludeList = Bubble::string2Array(constantFieldValidatorCharList);
        wxTextValidator constantFieldEventValidator (wxFILTER_INCLUDE_CHAR_LIST);  //##Por ahora se filtran así los caracteres no autorizados.
                                                                    //##Luego habría que construir un validator más sofisticado, con
                                                                    //wxRegEx para filtrar realmente los formatos no válidos en
                                                                    //tiempo real.
        constantFieldEventValidator.SetIncludes(constantFieldIncludeList);
//        if (constantFieldEnableValidation) //##
//        {
            constantField = new wxTextCtrl(
                                            this,
                                            wxNewId(),
                                            constantFieldDefaultValue,
                                            //This is faster than this getConstantFieldPos(iconW, iconH), so by the moment will use it:
                                            //##Optimize:
                                            wxPoint(GetClientSize().GetWidth()/2 - (int)(getOriginalSize().GetHeight()*childFactor/2),
                                                    GetClientSize().GetHeight()/2 - (int)(getOriginalSize().GetHeight()*childFactor/2)),
                                            //getConstantFieldSize(label, true),
                                            wxDefaultSize,
                                            //##Luego haré que el color de fondo del control Y DEL TEXTO se unifiquen con el del block
                                            //y además voy a centrar el texto verticalmente. De esta forma va a quedar mucho mejor...
                                            //##También se van agregar formas de validación y ayuda a la edición para guiar al usuario en
                                            //los formatos válidos...
                                            //wxTE_CENTRE | wxBORDER_NONE, //| wxBORDER_STATIC,
                                            wxTE_CENTRE,
                                            constantFieldEventValidator,
                                            wxString("constantField")
                                          );
//        }
//        else
//        {
//            constantField = new wxTextCtrl(
//                                            this,
//                                            wxNewId(),
//                                            constantFieldDefaultValue,
//                                            //##Optimize:
//                                            wxPoint(GetClientSize().GetWidth()/2 - (int)(getOriginalSize().GetHeight()*childFactor/2),
//                                                    GetClientSize().GetHeight()/2 - (int)(getOriginalSize().GetHeight()*childFactor/2)),
//                                            wxDefaultSize,
//                                            wxTE_CENTRE,
//                                            wxDefaultValidator,
//                                            wxString("constantField")
//                                          );
//        }

        if (constantField)
        {
            constantField->Hide(); //##
            constantField->SetFont(getFontConstantFields());
            constantField->SetPosition( getConstantFieldPos());

            constantField->Connect( wxEVT_LEFT_DOWN,
                                    wxMouseEventHandler(BubbleBlock::onLeftDown),
                                    NULL,
                                    this
                                  );
            //##I'm not sure that the constantField must show the canvas' pop-up edit menu (which is made in
            //the onRightDown() method):
            constantField->Connect( wxEVT_RIGHT_DOWN,
                                    wxMouseEventHandler(BubbleBlock::onRightDown),
                                    NULL,
                                    this
                                  );

            constantField->Connect( wxEVT_COMMAND_TEXT_UPDATED,
                                    wxCommandEventHandler(BubbleBlock::onConstantFieldTextUpdated),
                                    NULL,
                                    this
                                  );

            constantFieldLabel = new wxStaticText(
                                                    this,
                                                    wxNewId(), //##
                                                    constantFieldDefaultValue,
                                                    //This is faster than this getConstantFieldPos(iconW, iconH), so by the moment will use it:
                                                    //##Optimize:
                                                    wxPoint(GetClientSize().GetWidth()/2 - (int)(getOriginalSize().GetHeight()*childFactor/2),
                                                            GetClientSize().GetHeight()/2 - (int)(getOriginalSize().GetHeight()*childFactor/2)),
                                                    getConstantFieldSize(),
                                                    wxALIGN_CENTRE,
                                                    wxString("constantFieldLabel")
                                                 );
            if (constantFieldLabel)
            {
                constantFieldLabel->SetFont(getFontConstantFields());
                constantFieldLabel->SetPosition(getConstantFieldLabelPos());

                if (constantFieldLabel->GetLabel() == wxString(""))
                    constantFieldLabel->SetLabel(wxString("  ")); //##Descablear...

                resizeConstantField();
                SetSize((int)(getRealSize().GetWidth()*((BubbleCanvas *)parent)->getZoomFactor()), wxDefaultCoord);

                constantFieldLabel->Connect(wxEVT_ENTER_WINDOW,
                                            wxMouseEventHandler(BubbleBlock::onConstantFieldLabelMouseEnter),
                                            NULL,
                                            this
                                           );
                constantFieldLabel->Connect(wxEVT_LEAVE_WINDOW,
                                            wxMouseEventHandler(BubbleBlock::onConstantFieldLabelMouseLeave),
                                            NULL,
                                            this
                                           );
                constantFieldLabel->Connect(wxEVT_LEFT_DOWN,
                                            wxMouseEventHandler(BubbleBlock::onLeftDown),
                                            NULL,
                                            this
                                           );
                constantFieldLabel->Connect(wxEVT_RIGHT_DOWN,
                                            wxMouseEventHandler(BubbleBlock::onRightDown),
                                            NULL,
                                            this
                                           );
            }
        }
    }

    if (info.getHasInstanceNameField())
    {
        wxString initialSelection(info.getInstanceNameFieldDefaultValue()); //##Esto cambiará por los valores de instances.
        if ( getUserMustSelectInstanceName() && (!info.getLoading()) ) //##!info.getLoading() has no effect, but it's correct.
            initialSelection = wxString("");

        //##Estos if son temporales, para ver que bubble no sea NULL, pero se van en cuanto descablee la carga
        //de imágenes para el combo:
        if (getCanvas())
        {
            if (getCanvas()->getBubble())
            instanceNameField = new BubbleCombo(this,
                                                wxNewId(),
                                                wxImage(getCanvas()->getBubble()->getThemePath() + wxString("/ComboSelectDefault.png")),
                                                wxImage(getCanvas()->getBubble()->getThemePath() + wxString("/ComboSelectPressed.png")),
                                                wxImage(getCanvas()->getBubble()->getThemePath() + wxString("/ComboSelectHover.png")),
                                                wxImage(getCanvas()->getBubble()->getThemePath() + wxString("/ComboSelectDisabled.png")),
                                                false,
                                                wxDefaultPosition,
                                                wxDefaultSize,
                                                //wxTAB_TRAVERSAL|
                                                //wxNO_BORDER|
                                                wxBORDER_SIMPLE,
                                                //wxFULL_REPAINT_ON_RESIZE,
                                                //wxBORDER_RAISED|
                                                //wxBORDER_SUNKEN|
                                                //wxTRANSPARENT_WINDOW,
                                                //wxDefaultSize,
                                                //##wxALIGN_CENTRE,
                                                wxString("instanceNameField")
                                            );
        }
        if (instanceNameField)
        {
            instanceNameField->setSorted(info.getInstanceNameFieldSorted());

            instanceNameField->Connect( wxEVT_COMMAND_TOOL_CLICKED,
                                        wxCommandEventHandler(BubbleBlock::onClickInstanceNameField),
                                        NULL,
                                        this
                                      );
            instanceNameField->Connect( wxEVT_COMMAND_TEXT_UPDATED,
                                        wxCommandEventHandler(BubbleBlock::onInstanceNameSelected),
                                        NULL,
                                        this
                                      );
            instanceNameField->setFont(getFontEditableFields());
            instanceNameField->SetPosition(getInstanceNameFieldPos());

            updateInstanceNameList();

            instanceNameField->SetSize(getInstanceNameFieldSize()); //2011.12.20

            //if (info.getLoading())
            instanceNameField->setSelection(info.getInstanceNameFieldDefaultValue());
//2011.08.08:   I think that is better to force the user to select an instance, rather tham autoselect the .
//              first option, because this caused run-time errors in a lot of programs:
//                if (!getUserMustSelectInstanceName())
//                {
//                    if (instanceNameField->getCount() > 0)
//                        instanceNameField->setSelection(0);
//                }
//                else
//                {
//                    //Shows the incomplete field in real time:
//                    validateConstantField(instanceNameField->getText());
//                }
        }
    }

    if ((info.getHasInstanceNameField() && info.getHasBlockButton()) ||
        (info.getHasInstanceNameField() && info.getHasMethodButton())
       )
    {
        //##This code was wrong, because getInstanceNameFieldSize() has a zoomFactor inside that makes this works bad
        //precisely with the Zoom:
        //        setRealSize(wxSize(getRealSize().GetWidth(),
        //                           getRealSize().GetHeight() + getInstanceNameFieldSize().GetHeight()*2
        //                          )
        //                   );
        //##Optimize:
        //##Un-hardcode:
        //This height correction is only necessary if there are more than one paramSlot:
        if (getParamsCount() <= 1)
        {
            setRealSize(wxSize( getRealSize().GetWidth(),
                                //##Un-hardcode:
                                getRealSize().GetHeight() + (int)( getOriginalSize().GetHeight()*childFactor*0.6) //##
                              )
                       );
        }
    }

    //If there aren't removable params, makes the removeParamsButton invisible:
    if (getParamsCount() <= getMinParamsCount())
    {
        if (removeParamsButton)
            removeParamsButton->Hide();
    }

    validateConstantField(true);

    brotherBlockInfo = info.getBrother();   //##Verificar, sobre todo que el constructor de copia de BlockInfo
                                            //haya mantenido la dirección del puntero al brother...

    //##This is just by now, becase only params are dragable in the current implementation:
    if (getCanvas())
    {
        if (getCanvas()->getBubble())
        {
            setIsDraggable( getIsDraggable() &&
                            (getDataType() == getCanvas()->getBubble()->getActionDataType())
                          );
        }
    }

    if (commented)
    {
        comment(true);
        //if (brotherBlockInfo)
        //    brotherBlockInfo->setCommented(true);
    }
}


BubbleBlock::BubbleBlock(const BubbleBlock &block)
{
    //##Implementar el constructor de copia
}


BubbleBlock::~BubbleBlock()
{
    //##Must params be destroyed here? Apparently yes (the parent is not destroying them!).

    //wxMessageDialog dialog0(parent, getName(), _("Destroy")); //##Debug.
    //dialog0.ShowModal(); //##Debug,


//##Esto vuela!:
    //This is not the best way, and probably will be implemented different in the future, but currently, the
    //Bubble created brotherBlockInfo is destroyed here, because it passed through the BubbleBlockInfo up to
    //this instance as a pointer:
//    if (brotherBlockInfo)
//    {
//        delete brotherBlockInfo; //##¡¡Verificar!!
//        brotherBlockInfo = NULL; //Not necessary.
//    }
    //##Forgetting something?
}


//##Esto probablemente se cambie por algo más prolijo:
void BubbleBlock::setupButton(  BubbleButton *button,
                                const wxImage &defaultImage, //##¿Optimizar?
                                const wxImage &hoverImage,
                                const wxImage &pressedImage,
                                const wxImage &disabledImage)
{
    if (button)
    {
        //##Esta clase de operaciones se pueden luego hacer con constructores de copia, o algo por el estilo:
        //##Ver si esto es seguro, o si además tengo que verificar las images:
#if defined (linux)
//        button->setBackgroundColor(getDefaultBackgroundColour1());
#endif
        button->setImageDefault(defaultImage);
        button->setImageHover(hoverImage);
        button->setImagePressed(pressedImage);
        button->setImageDisabled(disabledImage);

        //¡This way of calling Connect() finally overcomes the Connect() bug!
        //¡ Thanks to http://www.zytrax.com/tech/survival/wxwidgets.html#connect !
        button->Connect(wxEVT_LEFT_DOWN,
                        wxMouseEventHandler(BubbleBlock::onLeftDown),
                        NULL,
                        this
                       );
        button->Connect(wxEVT_RIGHT_DOWN,
                        wxMouseEventHandler(BubbleBlock::onRightDown),
                        NULL,
                        this
                       );
    }
}


BubbleCanvas *BubbleBlock::getCanvas() const
{
    return parent;
}


BubbleBlock *BubbleBlock::getFirstBackBlock() const
{
    BubbleBlock *tempBlock = backBlock;
    BubbleBlock *firstBackBlock = tempBlock;
    while (tempBlock)
    {
        firstBackBlock = tempBlock;
        tempBlock = tempBlock->getBackBlock();
    }
    if (firstBackBlock)
        return firstBackBlock;

    //If ther is no firstBackBlock, the current block IS the firstBackBlock itself:
    return (BubbleBlock *)this;
}


BubbleBlock *BubbleBlock::getLastParamFirstBlock() const
{
    BubbleBlock *block = getParamFirstBlock(0);
    BubbleBlock *prevBlock = NULL;
    while (block)
    {
        prevBlock = block;
        block = block->getParamFirstBlock(0);
    }
    if (block)
        return block;
    if (prevBlock)
        return prevBlock;   //The last block has the first paramSlot empty, so, this returns the last block
                            //itself, not a param.
    return (BubbleBlock *)this; //If there is no Block and no prevBlock, it's because the current Block has
                                //an empty first paramSlot.
}


void BubbleBlock::onConstantFieldLabelMouseEnter(wxMouseEvent& event)
{
    //##Implementar, e implementar el mouse leave:
    //Que se ponga en color (por ahora amarillo, pero deberá ser pasado en el info)...

    if (constantFieldLabel)
    {
        //Compares to avoid flickering:
        if (constantFieldLabel->GetBackgroundColour() != defaultMouseOverColour)
        {
            constantFieldLabel->SetBackgroundColour(defaultMouseOverColour);
            constantFieldLabel->Update();
            constantFieldLabel->Refresh();
        }
    }

    event.Skip();
}


void BubbleBlock::onConstantFieldLabelMouseLeave(wxMouseEvent& event)
{
    //##Implementar, e implementar el mouse leave:
    //Que se ponga en color (por ahora amarillo, pero deberá ser pasado en el info)...

    if (constantFieldLabel)
    {
        //Compares to avoid flickering:
        if (constantFieldLabel->GetBackgroundColour() == defaultMouseOverColour)
        {
            validateConstantField(true);
            constantFieldLabel->Update();
            constantFieldLabel->Refresh();
        }
    }

    event.Skip();
}


void BubbleBlock::beginDrag(const wxPoint &startPos)
{
    if (getCanvas())
    {
        if (getIsDraggable())
        {
            positionBeforeDragging = GetPosition();
            dragStartPos = startPos;
            getCanvas()->setDragStartPos(startPos);
            getCanvas()->setDraggedBlock(this);
            getCanvas()->CaptureMouse();

            getCanvas()->setLockBlockKeyboardEvents(true);

            //showChildren(false);
            //showParamsChildren(false);
        }
//        else
//        {
//            positionBeforeDragging = this->GetPosition();
//            getCanvas()->setDragStartPos(startPos);
//            getCanvas()->setDraggedBlock(this);
//            //When a block is not draggable, it shows an wxDragImage that shows this situation.
//            if (getCanvas())
//            {
//                wxDragImage *nonDraggableImage = new wxDragImage(wxString(_("Block")), wxCursor(wxCURSOR_HAND));
//                if (nonDraggableImage)
//                {
//    //                wxMessageDialog dialog0(parent, wxString("dragImage"), _("1")); //##Debug.
//    //                dialog0.ShowModal(); //##Debug,
//
//                    getCanvas()->setNonDraggableImage(nonDraggableImage);
//                    dragging = true;
//                }
//            }
//        }
        setDragging(true);
    }
}


void BubbleBlock::endDrag(bool drop)
{
    if (getCanvas())
    {
        if (getCanvas()->HasCapture())
            getCanvas()->ReleaseMouse();
        getCanvas()->setLockBlockKeyboardEvents(false);
        if (getIsDraggable())
        {
            if (getDragging())
            {
                getCanvas()->setDraggedBlock(NULL);
                setDragging(false);
                if (!drop)
                {
                    SetPosition(positionBeforeDragging);
                }
                showChildren(true);
                //showParamsChildren(true);

                if (!areParamsVisible())
                    showParams(true);

                //##Si se hace un zoom() después del dragging, esto no será necesario:
    //                Update();
    //                Refresh();
            }
        }
    }
//    else
//    {
//        if (getCanvas())
//        {
//            if (getCanvas()->getNonDraggableImage())
//                getCanvas()->getNonDraggableImage()->EndDrag();
//            getCanvas()->deleteNonDraggableImage();
//            getCanvas()->setDraggedBlock(NULL);
//            dragging = false;
//        }
//    }
}


void BubbleBlock::moveWithParams(int x, int y)
{
    unsigned int paramsCount = getParamsCount();
    for (unsigned int j = 0; j < paramsCount; j++)
    {
        moveParamSlot(j, x, y);
    }
    wxPoint pos = GetPosition();
    Move(x + pos.x, y + pos.y);
//    Update();
    Refresh();
}


void BubbleBlock::moveParamSlot(const unsigned int index, int x, int y)
{
    BubbleParam *tempParam = NULL;
    if (index < params.GetCount())
    {
        tempParam = &(params.Item(index));
        if (tempParam)
        {
            if (tempParam->getParamFirstBlock())
            {
                tempParam->getParamFirstBlock()->moveWithParams(x, y);
            }
        }
    }
}


void BubbleBlock::showBrothers(bool value)
{
    BubbleBlock *iteratorBlock = getPrevBrother();

    //Prev brothers:
    while(iteratorBlock)
    {
        iteratorBlock->Show(value);
        iteratorBlock = iteratorBlock->getPrevBrother();
    }

    //Next brothers:
    iteratorBlock = getNextBrother();
    while(iteratorBlock)
    {
        iteratorBlock->Show(value);
        iteratorBlock = iteratorBlock->getNextBrother();
    }

    Show(value);
}


void BubbleBlock::moveWithBrothers(int x, int y)
{
    BubbleBlock *iteratorBlock = getPrevBrother();
    wxPoint pos(0, 0);

    //Prev brothers:
    while(iteratorBlock)
    {
        pos = iteratorBlock->GetPosition();
        iteratorBlock->Move(pos.x + x, pos.y + y);
        iteratorBlock->Refresh();
        iteratorBlock = iteratorBlock->getPrevBrother();
    }

    //Next brothers:
    iteratorBlock = getNextBrother();
    while(iteratorBlock)
    {
        pos = iteratorBlock->GetPosition();
        iteratorBlock->Move(pos.x + x, pos.y + y);
        iteratorBlock->Refresh();
        iteratorBlock = iteratorBlock->getNextBrother();
    }

    pos = GetPosition();
    Move(x + pos.x, y + pos.y);
//    Update();
    Refresh();
}


void BubbleBlock::showParams(bool value)
{
    unsigned int paramsCount = getParamsCount();
    for (unsigned int j = 0; j < paramsCount; j++)
    {
        BubbleParam *tempParam = NULL;
        if (j < params.GetCount())
        {
            tempParam = &(params.Item(j));
            if (tempParam)
            {
                if (tempParam->getParamFirstBlock())
                {
                    tempParam->getParamFirstBlock()->showParams(value);
                }
            }
        }
    }
    Show(value);
    paramsVisible = value;
}


void BubbleBlock::showChildren(bool value)
{
    wxWindowList &childrenList = GetChildren();
    wxWindowList::iterator iter;
    for (iter = childrenList.begin(); iter != childrenList.end(); ++iter)
    {
        wxWindow *currentChild = *iter;
        if (currentChild)
        {
            //If this is the current block, show the constant editor, else, just show the label:
            if (currentChild == constantField)
            {
                if (getCanvas())
                    currentChild->Show(getCanvas()->getCurrentBlock() == this);
            }
            else if (currentChild == constantFieldLabel)
            {
                if (getCanvas())
                    currentChild->Show(getCanvas()->getCurrentBlock() != this);
            }
            else if (currentChild == removeParamsButton)
            {
                if (value)
                {
                    if (getParamsCount() <= getMinParamsCount())
                        currentChild->Hide();
                    else
                        currentChild->Show(value);
                        //currentChild->Show(value != currentChild->IsShown());
                }
                else
                    currentChild->Hide();
            }
            else if ( (currentChild != blockButton) && (currentChild != methodButton) )
            {
                //When dragging, the blockButton and method buttos are visible:
                currentChild->Show(value);
                //currentChild->Show(value != currentChild->IsShown());
            }
//            if (value)
//            {
//                currentChild->Update();
//                currentChild->Refresh();
//            }
        }
    }
}


void BubbleBlock::showParamsChildren(bool value)
{
    unsigned int paramsCount = getParamsCount();
    for (unsigned int j = 0; j < paramsCount; j++)
    {
        BubbleParam *tempParam = NULL;
        if (j < params.GetCount())
        {
            tempParam = &(params.Item(j));
            if (tempParam)
            {
                if (tempParam->getParamFirstBlock())
                {
                    tempParam->getParamFirstBlock()->showParamsChildren(value);
                }
            }
        }
    }
    showChildren(value);
}


//The param fullEnableDisable decides if the block will be fully disabled (true) or just grayed (false)
void BubbleBlock::enableChildren(bool value, bool fullEnableDisable)
{
    wxWindowList &childrenList = GetChildren();
    wxWindowList::iterator iter;
    for (iter = childrenList.begin(); iter != childrenList.end(); ++iter)
    {
        wxWindow *currentChild = *iter;
        if (currentChild)
        {
            if (currentChild == constantFieldLabel)
            {
                //Decides the colour of the constantFieldLabel control:
                if (value)
                    validateConstantField(true);
                else
                    constantFieldLabel->SetBackgroundColour(getDisabledBackgroundColour0());
            }
            currentChild->Enable(value);
        }
    }

    if (value)
    {
        if (selected)
        {
            if (constantFieldLabel)
                constantFieldLabel->Hide();
            if (constantField)
            {
                resizeConstantField();  //Patch: When the label is an empty string, and the block is deselected,
                                        //does not resize without this call.
                constantField->Show(true);
            }
        }
    }
    else
    {
        if (constantField)
            constantField->Hide();
        if (constantFieldLabel)
        {
            resizeConstantField();  //Patch: When the label is an empty string, and the block is deselected,
                                    //does not resize without this call.
            constantFieldLabel->Show(true);
        }
    }

    Enable(value, fullEnableDisable);
}


void BubbleBlock::enableParamsChildren(bool value, bool fullEnableDisable)
{
    unsigned int paramsCount = getParamsCount();
    for (unsigned int j = 0; j < paramsCount; j++)
    {
        BubbleParam *tempParam = NULL;
        if (j < params.GetCount())
        {
            tempParam = &(params.Item(j));
            if (tempParam)
            {
                if (tempParam->getParamFirstBlock())
                {
                    tempParam->getParamFirstBlock()->enableParamsChildren(value, fullEnableDisable);
                    tempParam->getParamFirstBlock()->setCommented(!value); //In this case, commented is the opposite to enable.
                }
            }
        }
    }
    enableChildren(value, fullEnableDisable);
}


bool BubbleBlock::Enable(bool value, bool fullEnableDisable)
{
    enabled = value;
    Update();
    Refresh();
    if (fullEnableDisable)
        return BubblePanel::Enable(value);
    else
        return value;
}


bool BubbleBlock::Disable()
{
    return Enable(false);
}


void BubbleBlock::comment(bool value)
{
    commented = value;
    enableParamsChildren(!commented, false);
}


void BubbleBlock::commentWithBrothers(bool value)
{
    BubbleBlock *iteratorBlock = getPrevBrother();
    wxPoint pos(0, 0);

    //Prev brothers:
    while(iteratorBlock)
    {
        iteratorBlock->comment(value);
        iteratorBlock = iteratorBlock->getPrevBrother();
    }

    //Next brothers:
    iteratorBlock = getNextBrother();
    while(iteratorBlock)
    {
        iteratorBlock->comment(value);
        iteratorBlock = iteratorBlock->getNextBrother();
    }

    comment(value);
}


void BubbleBlock::showLabel(bool value)
{
    //##Implementar:
}


void BubbleBlock::showParamsLabels(bool value)
{
    unsigned int paramsCount = getParamsCount();
    for (unsigned int j = 0; j < paramsCount; j++)
    {
        BubbleParam *tempParam = NULL;
        if (j < params.GetCount())
        {
            tempParam = &(params.Item(j));
            if (tempParam)
            {
                if (tempParam->getParamFirstBlock())
                {
                    tempParam->getParamFirstBlock()->showParamsLabels(value);
                }
            }
        }
    }
    showLabel(value);
}


void BubbleBlock::updateParamsTextGUI()
{
    unsigned int paramsCount = getParamsCount();
    for (unsigned int j = 0; j < paramsCount; j++)
    {
        BubbleParam *tempParam = NULL;
        if (j < params.GetCount())
        {
            tempParam = &(params.Item(j));
            if (tempParam)
            {
                if (tempParam->getParamFirstBlock())
                {
                    tempParam->getParamFirstBlock()->updateParamsTextGUI();
                }
            }
        }
    }
    updateTextGUI();
}


void BubbleBlock::updateTextGUI()
{
    if (blockButton == NULL)
        return;

    if (getCanvas())
    {
        Bubble *bubble = getCanvas()->getBubble();
        if (bubble == NULL)
            return;

        blockButton->SetToolTip((bubble->getBlockInfo(getName(), getFunction())).getToolTip());
    }
}


void BubbleBlock::onMethodButtonLeftDown(wxMouseEvent& event)
{
    //Now this is the current block:
    if (getCanvas())
        getCanvas()->setCurrentBlock(this);

    event.Skip();
}


void BubbleBlock::onLeftDown(wxMouseEvent& event)
{
    if (getCanvas())
    {
        //Now this is the current block:
        getCanvas()->setCurrentBlock(this);

        //##This code is obsolte in the sense that the code in the onRightDown event does something very similar
        //but in a better way. So put all this stuff of getting the click position on a new method and unify
        //both:

        //Drag&Drop subsystem:
        wxPoint pos = event.GetPosition();
        if (event.GetEventObject())
        {
            //If the click was on a subcontrol, this corrects the dragging offset:
//            if ( event.GetEventObject()->IsKindOf(CLASSINFO(BubbleButton)) )
//            {
                //##Ver si esto permanece así, pero en teoría sí (incluso el constant field, funciona excepto
                //cuando se lo está editando, lo cual es un comportamiento deseable o al menos esperable,
                //documentar esto también):
                if (event.GetEventObject() == blockButton)
                {
                    if (blockButton)
                        pos += blockButton->GetPosition();
                }
                //##Es posible que no permita drag en los métodos, o que tenga que hacer que el picker de método
                //aparezca en el onMouseLeftUp, y no en el down como con los paramSlots, para que se pueda draggear
                //desde el botón de método (que en cosas como las constantes booleanes es importante):
                else if (event.GetEventObject() == methodButton)
                {
                    if (methodButton)
                        pos += methodButton->GetPosition();
                }
                else if (event.GetEventObject() == constantField)
                {
                    if (constantField)
                        pos += constantField->GetPosition();
                }
                else if (event.GetEventObject() == constantFieldLabel)
                {
                    if (constantFieldLabel)
                        pos += constantFieldLabel->GetPosition();
                }

                //##Por ahora esto no está implementado, porque el click del BubbleCombo se maneja en el evento
                //onClickInstanceNameField():
                //else if (event.GetEventObject() == instanceNameField)
                //{
                //    if (instanceNameField)
                //        pos += instanceNameField->GetPosition();
                //}
//            }
        }
        beginDrag(pos);

//        if (instanceNameField)
//            instanceNameField->SetFocus();
//        else
//            SetFocusIgnoringChildren();
        //##SetFocus();

        //##Implementar acá el dragging:
        //dragStartPos = event.GetPosition();
    }
    event.Skip();
}


void BubbleBlock::onLeftUp(wxMouseEvent& event)
{
//    wxMessageDialog dialog0(parent, wxString("endDrag"), _("0")); //##Debug.
//    dialog0.ShowModal();  //##Debug.

    //If the dragging ends over the same block, does not move it:
    endDrag(false);
    event.Skip();
}

//##¡¡ELMINAR ESTO EN CUANTO IMPLEMENTE LOS MENÚES QUE VAN A SACAR A LOS ESPANTOSOSO COMBOBOXES!!:
//##:Esto es para corregir el bug por el cual en los bloques con elementos que reciben foco (tales
//como comboboxes, cuando está el foco en dichos controles, y se presiona un botón DEL MISMO BLOQUE
//que despliega un picker (como por ejemplo un paramSlot), entonces hay que presionar 2 veces dicho
//botón para ver el picker. Esto es sumamente incómodo, y con este setfocus se recibe antes el foco
//en el botón en cuestión, por lo que al recibir luego el evento onLeft se vé el picker
//inmediatamente:
void BubbleBlock::onMouseEnter(wxMouseEvent& event)
{
    //##¡¡ELIMINAR ESTO EN CUANTO IMPLEMENTE LOS MENÚES QUE VAN A SACAR A LOS ESPANTOSOS COMBOBOXES!!

//    BubbleButton *button = (BubbleButton *) event.GetEventObject();
//    if (button)
//    {
//        if ( button->IsKindOf(CLASSINFO(BubbleButton)) )
//        {
//            button->SetFocus();
//        }
//    }
    event.Skip();
}


void BubbleBlock::onRightDown(wxMouseEvent& event)
{
    //##Implementar:
    //Seguramente acá se hará que se despliegue un menú contextual para copiar, pegar, etc..
    //Todos los buttons del block tendrán mapeado este evento, para que el despliegue del menú sea
    //independiente del área del block sobre la que se da el click derecho.
    BubbleCanvas *canvas = getCanvas();
    if (canvas)
    {
        //Now this is the current block:
        canvas->setCurrentBlock(this); //This must be done before calling the pop-up menu.
        SetFocusIgnoringChildren();
        //##SetFocus();

        wxPoint pos = GetPosition() + event.GetPosition();
        wxObject *obj = event.GetEventObject();
        if (obj)
        {
            if ( obj->IsKindOf(CLASSINFO(wxWindow)) )
            {
                if (obj != this)
                    pos += ((wxWindow *)obj)->GetPosition();

                if (canvas->getBubble())
                {
                    if ( getDataType() == canvas->getBubble()->getActionDataType() )
                    {
                        if ( getComponentStart() || getBlockStart() )
                        {
                            if (canvas->getMenuEditForStartBlocks())
                                canvas->popupMenu(canvas->getMenuEditForStartBlocks(), pos);
                        }
                        else
                        {
                            if (canvas->getMenuEditForActions())
                                canvas->popupMenu(canvas->getMenuEditForActions(), pos);
                        }
                    }
                    else
                    {
                        if (canvas->getMenuEditForExpressions())
                            canvas->popupMenu(canvas->getMenuEditForExpressions(), pos);
                    }
                    //Nothing from here, because the user may select the delete option wich will actually
                    //destroy this block...
                }
            }
        }
    }
}


void BubbleBlock::addVariableWithInstanceName()
{
    if (!getUpdateVariables())
        return;

    //##Por ahora esto está así, pero habrá que trabajarlo bastante a medida que se mejore el manejo de
    //variables e instancias:
    if (constantField)
    {
        BubbleInstance *var = new BubbleInstance(constantField->GetValue(), wxString("Variable")); //##Un-hardcode.
        if (var)
        {
            //wxMessageDialog dialog0(parent, var->getName(), _("2")); //##Debug.
            //dialog0.ShowModal();  //##Debug.
            if (getCanvas())
            {
                //If the var name has changed and there is no other varInit block declaring it,
                //deletes the instance:
                if (var->getName() != prevInstanceName)
                {
                    if  (!getCanvas()->varNameIsInUse(prevInstanceName, this, false))
                    {
                        getCanvas()->removeInstance(prevInstanceName);
                        getCanvas()->updateBlocksUsingThisVarName(prevInstanceName, wxEmptyString, NULL);
                    }
                }
                if (validateConstantField(false)) //Passes false because it's not necessary to eval varNameIsInUse.
                {
                    getCanvas()->setInstance(var);
                    getCanvas()->updateBlocksUsingThisVarName(prevInstanceName, var->getName(), this);
                }
            }
        }
    }
}


void BubbleBlock::updateInstanceNameList()
{
    if (getCanvas() == NULL)
        return;
    if (instanceNameField == NULL)
        return;

    instanceNameField->clear();
    for (int i = 0; i<getCanvas()->getInstancesCount(); i++)
    {
        //##Will this change when classes become ready?:
        BubbleInstance *tempInstance = getCanvas()->getInstance(i);
        if (tempInstance)
        {
            if (tempInstance->getType() == getInstanceType())
                instanceNameField->append(tempInstance->getName());
        }
    }
}


void BubbleBlock::onClickInstanceNameField(wxCommandEvent &event)
{
    //##This causes a bit of flick, see if it can be improved:
    if (getCanvas())
        getCanvas()->setCurrentBlock(this);

    updateInstanceNameList();
    event.Skip();
}


wxString BubbleBlock::getConstantFieldValue() const
{
    wxString result("");
    if (constantField)
        result = constantField->GetValue();
    return result;
}


void BubbleBlock::setConstantFieldError(bool value)
{
    if (constantField && constantFieldLabel)
    {
        if (value)
        {
            constantField->SetBackgroundColour(defaultErrorColour);
            constantFieldLabel->SetBackgroundColour(defaultErrorColour);
        }
        else
        {
            validateConstantField(false);
            //constantField->SetBackgroundColour(defaultEditableFieldsColour);
            //constantFieldLabel->SetBackgroundColour(getDefaultBackgroundColour1());
        }
        constantField->Update();
        constantField->Refresh();
        constantFieldLabel->Update();
        constantFieldLabel->Refresh();
    }
}


void BubbleBlock::setInstanceNameFieldError(bool value)
{
    if (instanceNameField)
    {
        if (value)
            instanceNameField->setBackgroundColour(defaultErrorColour);
        else
            instanceNameField->setBackgroundColour(defaultEditableFieldsColour);
        //instanceNameField->Update();
        //instanceNameField->Refresh();
    }
}


void BubbleBlock::resizeConstantField()
{
    if (constantField)
    {
        if (constantFieldLabel)
        {
            //##Patch to make the sizes look better:
            constantFieldLabel->SetLabel(wxString(" ") + constantField->GetValue() + wxString(" "));
            if (constantFieldLabel->GetLabel() == wxString("  "))
                constantFieldLabel->SetLabel(wxString("   "));

            //Computes the constantField textCtrl size based on the text len:
            constantField->SetSize(getConstantFieldSize());
        }
        if (getCanvas())
        {
            if ( getCanvas()->getZoomFactor() != 0)
            {
                int newWidth = 0;
                if (blockButton)
                    newWidth = getBlockButtonPos().x + getBlockButtonSize().GetWidth();
                if (methodButton)
                    newWidth = getMethodButtonPos().x + getMethodButtonSize().GetWidth();
                if ( !(blockButton && methodButton) )
                    newWidth += (int)(getOriginalSize().GetWidth()*childFactor/6); //##Optimize / Unhardcode.
                if (getParamsCount() > 0)
                    newWidth += getParamSize().GetWidth();
                newWidth += (int)(constantField->GetSize().GetWidth());
                setRealSize(wxSize( (int)(newWidth/((BubbleCanvas *)parent)->getZoomFactor()),
                                    getRealSize().GetHeight() )
                           );
                getCanvas()->zoomParams(this);
            }
        }
    }
}


void BubbleBlock::updateVarNameValidation(  const wxString& currentName,
                                            const wxString& newName,
                                            BubbleBlock *sender)
{
    if (getCanvas() == NULL)
        return;

    if (getHasInstanceNameField())
    {
        //Only will modify the blocks using this name:
        if (getInstanceNameField() == currentName)
        {
            if (newName == wxEmptyString)
            {
                //If the newName is empty, this means that the name (and it's corresponding instance)
                //was deleted. So, this just marks the block in red, but does not deletes the text from
                //the instanceNameField (this will happend automatically if the user clicks on the combo
                //because the deleted variable will not be listed there). This is because the user may
                //want to restore the variable soon, so this respects it's entered names.
                setInstanceNameFieldError(true);
            }
            else
            {
                //Only updates the blocks using this variable, if there is no other variableInit block
                //already declaring the new name (because the user may want to live those blocks with
                //that name):
                if ( !(getCanvas()->varNameIsInUse(currentName, sender, false)) )
                {
                    updateInstanceNameList();
                    if (setInstanceNameField(newName))
                        setInstanceNameFieldError(false);
                    else
                        setInstanceNameFieldError(true);
                }
            }
        }
        else if (getInstanceNameField() == newName)
        {
            //But there may exist blocks using the new name, that were invalid, and now have a valid
            //name:
            if (sender)
            {
                if  (sender->varNameIsValid(newName))
                    setInstanceNameFieldError(false);
            }
        }
    }
}


void BubbleBlock::validateParamsVarNames(   const wxString& currentName,
                                            const wxString& newName,
                                            BubbleBlock *sender)
{
    unsigned int paramsCount = getParamsCount();
    for (unsigned int j = 0; j < paramsCount; j++)
    {
        BubbleParam *tempParam = NULL;
        if (j < params.GetCount())
        {
            tempParam = &(params.Item(j));
            if (tempParam)
            {
                if (tempParam->getParamFirstBlock())
                {
                    tempParam->getParamFirstBlock()->validateParamsVarNames(currentName, newName, sender);
                }
            }
        }
    }
    updateVarNameValidation(currentName, newName, sender);
}


bool BubbleBlock::varNameIsValid(const wxString& value)
{
    if (constantFieldValidator.IsValid())
        return constantFieldValidator.Matches(value);
    return false;
}


//This may return bool in the future:
bool BubbleBlock::validateConstantField(bool testVarNameInUse)
{
    if (constantField == NULL)
        return false;
    if (constantFieldLabel == NULL)
        return false;

    if (!constantFieldEnableValidation)
    {
        constantField->SetBackgroundColour(defaultEditableFieldsColour);
        constantFieldLabel->SetBackgroundColour(getDefaultBackgroundColour1());
        return true;
    }

    if (constantField->GetValue() == wxEmptyString)
    {
        constantField->SetBackgroundColour(defaultErrorColour);
        constantFieldLabel->SetBackgroundColour(defaultErrorColour);
        return false;
    }

    if (constantFieldValidator.IsValid())
    {
        //If the constantField is used to init variables, then, the validation includes testing if
        //the instance name already exists:
        if (getUpdateVariables() && testVarNameInUse)
        {
            if (getCanvas())
            {
                if  (!getCanvas()->varNameIsInUse(constantField->GetValue(), this, true))
                {
                    if (constantFieldValidator.Matches(constantField->GetValue()))
                    {
                        constantField->SetBackgroundColour(defaultEditableFieldsColour);
//##Commented to solve bug #20111015.01:
//                        if (isCommented())
//                            constantFieldLabel->SetBackgroundColour(getDisabledBackgroundColour0());
//                        else
                            constantFieldLabel->SetBackgroundColour(getDefaultBackgroundColour1());
                        return true;
                    }
                }
            }
        }
        else
        {
            if (constantFieldValidator.Matches(constantField->GetValue()))
            {
                constantField->SetBackgroundColour(defaultEditableFieldsColour);
//##Commented to solve bug #20111015.01:
//                if (isCommented())
//                    constantFieldLabel->SetBackgroundColour(getDisabledBackgroundColour0());
//                else
                    constantFieldLabel->SetBackgroundColour(getDefaultBackgroundColour1());
                return true;
            }
        }
    }
    constantField->SetBackgroundColour(defaultErrorColour);
//##Commented to solve bug #20111015.01:
//    if (isCommented())
//        constantFieldLabel->SetBackgroundColour(getDisabledBackgroundColour0());
//    else
        constantFieldLabel->SetBackgroundColour(defaultErrorColour);
    //Update();
    //Refresh();
    return false;
}


void BubbleBlock::onConstantFieldTextUpdated(wxCommandEvent &event)
{
    if (getCanvas() == NULL)
        return;

    if (constantField)
    {
        if (constantFieldLabel)
        {
            resizeConstantField();
            SetSize((int)(getRealSize().GetWidth()*getCanvas()->getZoomFactor()), wxDefaultCoord);

            //Finally centers the textCtrl:
            constantField->SetPosition(getConstantFieldPos());

            validateConstantField(true);

            //Needed because of the size change:
            Update();
            Refresh();
    //        constantField->Update();
    //        constantField->Refresh();

            if (getCanvas())
            {
                getCanvas()->updateCodeAndRefresh();
                getCanvas()->forceSaved(false);
            }
        }
    }
    event.Skip();
}


void BubbleBlock::onInstanceNameSelected(wxCommandEvent &event)
{
    if (getCanvas())
    {
        if (instanceNameField)
        {
            //if ((getUserMustSelectInstanceName()) //Is this necessary?
            if  (getCanvas()->getInstanceIndex(instanceNameField->getText()) == wxNOT_FOUND)
                instanceNameField->setBackgroundColour(defaultEditableFieldsColour);
            else
                instanceNameField->setBackgroundColour(getDefaultBackgroundColour1());
        }
        getCanvas()->updateCodeAndRefresh();
        getCanvas()->forceSaved(false);
    }

    event.Skip();
}


void BubbleBlock::onAddParamsButtonLeftDown(wxMouseEvent& event)
{
    if (blockStart)
    {
        //##Implementar (ver comentario del "else")...
    }
    else
    {
        //##Por ahora agrega un parámetro del mismo tipo que el primer parámetro de la lista. Esto está bien para
        //las operaciones, pero no está implementada la cosa de los procedimientos (bloques de usuario), donde
        //el usuario debe poder elegir el tipo de parámetro:
        if (getParamsCount() > 0)
        {
            BubbleParam *param = &(params.Item(0));
            if (param)
            {
                wxString strToolTip("");
                wxToolTip *tempToolTip = param->GetToolTip();
                if (tempToolTip)
                {
                    strToolTip = (tempToolTip->GetTip()).Left((tempToolTip->GetTip()).Len() - 1);
                }
                strToolTip << (getParamsCount() + 1);
                addParamSlot(   //##This line takes the first param's tooltip, which always ends in a one digit
                                //number (ie. "0" or "1" if there are multiple symmetrical params:
                                strToolTip,
                                param->getDataType(),
                                param->getImageAssigned(),
                                param->getImageNotAssigned(),
                                param->getImageDefault(),
                                param->getImageHover(),
                                param->getImagePressed(),
                                param->getImageDisabled()
                            );
                if (getCanvas())
                {
                    getCanvas()->setCurrentBlock(this);
                    getCanvas()->zoom();
                }
            }
        }
//        //If there are removable params, makes the removeParamsButton visible:
//        if (getParamsCount() > getMinParamsCount())
//        {
//            if (removeParamsButton)
//                removeParamsButton->Show(true);
//        }
    }
    event.Skip();
}


void BubbleBlock::onRemoveParamsButtonLeftDown(wxMouseEvent& event)
{
    if (blockStart)
    {
        //##Quizá acá en el remove no sea necesario distinguir entre blockStart y bloques comunes...
    }
    else
    {
        if (getParamsCount() > getMinParamsCount())
        {
            int index = params.Index(params.Last());
            if (index != wxNOT_FOUND)
            {
                int newHeight = 0;
                BubbleBlock *lastParamFirstBlock = getParamSlotFirstBlock(index);
                if (lastParamFirstBlock)
                {
                    newHeight = getRealSize().GetHeight() - lastParamFirstBlock->getRealSize().GetHeight();
                    setRealSize(wxSize(getRealSize().GetWidth(), newHeight));
                    changeAllBackBlocksRealSize(wxSize(0, -lastParamFirstBlock->getRealSize().GetHeight()), false);
                }
                else
                {
                    newHeight = getRealSize().GetHeight() - getOriginalSize().GetHeight();
                    setRealSize(wxSize(getRealSize().GetWidth(), newHeight));
                    changeAllBackBlocksRealSize(wxSize(0, -getOriginalSize().GetHeight()), false);
                }
                if (getCanvas())
                {
                    getCanvas()->setCurrentBlock(this);
                    getCanvas()->zoom();
                }
                removeParamSlot(index);
            }
            //After removing the param, makes this button invisible if there are no more removable params:
            if (getParamsCount() <= getMinParamsCount())
            {
                if (removeParamsButton)
                    removeParamsButton->Hide();
            }
        }
    }
    event.Skip();
}


wxFont BubbleBlock::getFontConstantFields() const
{
    int side = (int)(getOriginalSize().GetHeight()*childFactor);
    if (getCanvas())
        side = (int) ( side * getCanvas()->getZoomFactor());
    //##Ver de descablear las constantes, como el "/3":
    return wxFont (
                    side/3, //##Tamaño del font: Analizar si la proporcionalidad está bien!
                    wxFONTFAMILY_DEFAULT,
                    wxFONTSTYLE_NORMAL,
                    wxFONTWEIGHT_BOLD
                   );
}


//##Este nombre quizá debería cambiar por getFontInstanceNameField, o algo así:
wxFont BubbleBlock::getFontEditableFields() const
{
    int side = (int)(getOriginalSize().GetHeight()*childFactor);
    if (getCanvas())
        side = (int) ( side * getCanvas()->getZoomFactor());
    //##Ver de descablear las constantes, como el "/5":
    return wxFont (
                    side/5, //##Tamaño del font: Analizar si la proporcionalidad está bien!
                    wxFONTFAMILY_DEFAULT,
                    wxFONTSTYLE_NORMAL,
                    wxFONTWEIGHT_BOLD
                   );
}


wxPoint BubbleBlock::getBlockButtonPos() const
{
    //##Optimizar:
    //##Descablear el 120 (y los otros factores?
//    if (instanceNameField && methodButton)
#if defined (WIN32)
    if (instanceNameField)
    {
        return wxPoint( GetClientSize().GetWidth()/120, //##Correr el blockButton para que no se superponga con los bordes redondeados.
                        GetClientSize().GetHeight()/2 - (getBlockButtonSize().GetHeight() + getInstanceNameFieldSize().GetHeight()) /2);
    }
    return wxPoint( GetClientSize().GetWidth()/120,
                    GetClientSize().GetHeight()/2 - getBlockButtonSize().GetHeight()/2);
#else
    if (instanceNameField)
    {
        return wxPoint( GetClientSize().GetWidth()/120+3, //##Correr el blockButton para que no se superponga con los bordes redondeados.
                        GetClientSize().GetHeight()/2 - (getBlockButtonSize().GetHeight() + getInstanceNameFieldSize().GetHeight()) /2);
    }
    return wxPoint( GetClientSize().GetWidth()/120+3,
                    GetClientSize().GetHeight()/2 - getBlockButtonSize().GetHeight()/2);
#endif
}


wxSize BubbleBlock::getBlockButtonSize() const
{
    int side = (int)(getOriginalSize().GetHeight()*childFactor);
    if (getCanvas())
        side = (int) ( side * getCanvas()->getZoomFactor());
#if defined (WIN32)
    return wxSize(side, side);
#else
    return wxSize(side, side*0.9);
#endif
}


wxPoint BubbleBlock::getMethodButtonPos() const
{
    if (blockButton)
    {
        if (instanceNameField)
        {
            return wxPoint( getMethodButtonSize().GetWidth(),
                            GetClientSize().GetHeight()/2 - (getMethodButtonSize().GetHeight() + getInstanceNameFieldSize().GetHeight()) /2);
        }
        return wxPoint( getMethodButtonSize().GetWidth(),
                        GetClientSize().GetHeight()/2 - getMethodButtonSize().GetHeight()/2);
    }
    if (centeredMethodButton)
    {
        return wxPoint( GetClientSize().GetWidth()/2 - getMethodButtonSize().GetWidth()/2,
                        GetClientSize().GetHeight()/2 - getMethodButtonSize().GetHeight()/2
                     );
    }
    return getBlockButtonPos();
}


wxSize BubbleBlock::getMethodButtonSize() const
{
    //##This function may be eliminated, It's here for consistency.
    return getBlockButtonSize();
}


wxSize BubbleBlock::getAddParamsButtonSize() const
{
    int side = (int)(getOriginalSize().GetHeight()*childFactor/2);
    if (getCanvas())
        side = (int) ( side * getCanvas()->getZoomFactor());
    return wxSize(side, side);
}


wxPoint BubbleBlock::getAddParamsButtonPos() const
{
    float border = selectionBorderWidth;
    if (getCanvas())
        border = border*getCanvas()->getZoomFactor();
    int y = GetClientSize().GetHeight() - getAddParamsButtonSize().GetHeight();
    if (blockButton)
        return wxPoint( blockButton->GetPosition().x +
                        blockButton->GetSize().GetWidth() + (int)border,
                        y - (int)border);
    return wxPoint( getBlockButtonPos().x + (int)border,
                    GetClientSize().GetHeight() - getAddParamsButtonSize().GetHeight() - (int)border
                  );
}


wxPoint BubbleBlock::getRemoveParamsButtonPos() const
{
    wxPoint pos = getAddParamsButtonPos();
    if (addParamsButton)
        pos = wxPoint(  pos.x + addParamsButton->GetSize().GetWidth(), pos.y);
    return pos;
}


wxSize BubbleBlock::getConstantFieldSize() const
{
    if (getCanvas())
    {
        if (constantFieldLabel)
        {
            return wxSize(  (int)(  constantFieldLabel->GetSize().GetWidth() +
                                    //##Correction factor, 2011.07.15:
                                    (int)(getRealSize().GetHeight()*childFactor*getCanvas()->getZoomFactor()*0.25)
                                 ),
                            (int)(constantFieldLabel->GetSize().GetHeight()*1.2) //##Optimize / Un-hardcode.
                         );
        }
    }
    //Error (no parent!):
    return wxSize(
                    (int)(getRealSize().GetWidth()*childFactor),
                    (int)(getRealSize().GetHeight()*childFactor/2)
                 );
}


//##Duplicated code with getConstantFieldPos(), improve this:
wxPoint BubbleBlock::getConstantFieldLabelPos() const
{
    if (constantFieldLabel)
    {
        int xPos = 0;
        if (blockButton)
            xPos = getBlockButtonPos().x + getBlockButtonSize().GetWidth();
        if (methodButton)
            xPos = getMethodButtonPos().x + getMethodButtonSize().GetWidth();
        if ( !(blockButton && methodButton) )
            xPos = GetClientSize().GetWidth()/2 - constantFieldLabel->GetSize().GetWidth()/2;
        return wxPoint (xPos, GetClientSize().GetHeight()/2 - constantFieldLabel->GetSize().GetHeight()/2);
    }
    return wxPoint (GetClientSize().GetWidth()/2 - (int)(getOriginalSize().GetHeight()*childFactor/2),
                    GetClientSize().GetHeight()/2 - (int)(getOriginalSize().GetHeight()*childFactor/2));
}


wxPoint BubbleBlock::getConstantFieldPos() const
{
    if (constantField)
    {
        int xPos = 0;
        if (blockButton)
            xPos = getBlockButtonPos().x + getBlockButtonSize().GetWidth();
        if (methodButton)
            xPos = getMethodButtonPos().x + getMethodButtonSize().GetWidth();
        if ( !(blockButton && methodButton) )
            xPos = GetClientSize().GetWidth()/2 - constantField->GetSize().GetWidth()/2;
        return wxPoint (xPos, GetClientSize().GetHeight()/2 - constantField->GetSize().GetHeight()/2);
    }
    return wxPoint (GetClientSize().GetWidth()/2 - (int)(getOriginalSize().GetHeight()*childFactor/2),
                    GetClientSize().GetHeight()/2 - (int)(getOriginalSize().GetHeight()*childFactor/2));
}


wxSize BubbleBlock::getInstanceNameFieldSize() const
{
//##
    //int side = (int)(getOriginalSize().GetHeight()*childFactor/3); //##
//    int side = (int)(GetClientSize().GetHeight()*childFactor/3); //##
//    return wxSize(side, side);

    if (getCanvas())
    {
        return wxSize(
                        GetClientSize().GetWidth() - 2*(int)(GetClientSize().GetWidth()*(1-childFactor)),
//                        getMethodButtonSize().GetWidth()*2 -
//                        (int)( (GetClientSize().GetWidth()/120)*getCanvas()->getZoomFactor() ),

//                        (int)(getRealSize().GetWidth()*childFactor*(getCanvas()->getZoomFactor()) -
//                        (int)(getParamSize().GetWidth()),
                        //(int)( getRealSize().GetHeight()*childFactor*(getCanvas()->getZoomFactor()/3 )
                        //This has still some problems, specially when zooming very small:

                        //##(int)( getRealSize().GetHeight()*childFactor*getCanvas()->getZoomFactor()*0.3 )
                        (int)( getOriginalSize().GetHeight()*childFactor*getCanvas()->getZoomFactor()*0.5 )

                        //(int)( getOriginalSize().GetHeight()*childFactor*getCanvas()->getZoomFactor()*0.6 )//##
                        //getRealSize().GetHeight() + (int)( getRealSize().GetHeight()*childFactor*0.6)
                        //##(int)( getRealSize().GetHeight()*childFactor/3 )
                        //##(int)( GetClientSize().GetHeight()*childFactor*getCanvas()->getZoomFactor()/3 )
                        //##(int)( getOriginalSize().GetHeight()*childFactor*getCanvas()->getZoomFactor()/3 )
                     );
    }
    return wxSize(
                    (int)(getRealSize().GetWidth()*childFactor) - (int)(getParamSize().GetWidth()),
                    (int)(getRealSize().GetHeight()*childFactor/3)
                 );

//    return wxSize(
//                    (int)(getOriginalSize().GetWidth()*childFactor),
//                    (int)(getOriginalSize().GetHeight()*childFactor/3)
//                 );
}


wxPoint BubbleBlock::getInstanceNameFieldPos() const
{
    //##Optimize this!!
    float zoomFactor = 1.0;
    if (getCanvas())
        zoomFactor = getCanvas()->getZoomFactor();

    //##Ver si está bien trabajar acá con el ClientSize:
    int gap = GetClientSize().GetWidth()/120; //##Un-hardcode.
    if (blockButton)
    {
        //int gap = 0; //##
        if (instanceNameField)
        {
            return wxPoint( blockButton->GetPosition().x + (int)(selectionBorderWidth*zoomFactor),
                            //GetClientSize().GetWidth()/2 - instanceNameField->GetSize().GetWidth()/2,
                            blockButton->GetPosition().y + blockButton->GetSize().GetHeight() + gap);
        }
        return wxPoint( blockButton->GetPosition().y, //gap,
                        blockButton->GetPosition().y + blockButton->GetSize().GetHeight() + gap);
    }
    if (methodButton)
    {
        //int gap = 0; //##
        if (instanceNameField)
        {
            return wxPoint( methodButton->GetPosition().x + (int)(selectionBorderWidth*zoomFactor),
                            //GetClientSize().GetWidth()/2 - instanceNameField->GetSize().GetWidth()/2,
                            methodButton->GetPosition().y + methodButton->GetSize().GetHeight() + gap);
        }
        return wxPoint( methodButton->GetPosition().y, //gap,
                        methodButton->GetPosition().y + methodButton->GetSize().GetHeight() + gap);
    }
    return  wxPoint(gap, //##
                    GetClientSize().GetHeight()/2);
}


//##Eliminar el index que no se utiliza:
wxSize BubbleBlock::getParamSize() const
{
    int side = (int)(getOriginalSize().GetHeight());
    if (getCanvas())
        side = (int) (side * getCanvas()->getZoomFactor());
    return wxSize(side/2, side);
}


wxPoint BubbleBlock::getParamPos(int index) const
{
    //##Optimizar:
    int side = (int)(getOriginalSize().GetHeight());
    if (getCanvas())
        side = (int) ( side * getCanvas()->getZoomFactor());

    int posY = 0;
//##:
//    BubbleBlock *tempBlock = getParamFirstBlock(0);
//    if ( (tempBlock == NULL) && (getParamsCount() < 2) )
//        posY = GetClientSize().GetHeight()/2 - getParamSize().GetHeight()/2;
    BubbleBlock *tempBlock = NULL;
    for (int k = 0; k < index; k++)
    {
        tempBlock = getParamFirstBlock(k);
        if (tempBlock)
        {
            if (getCanvas())
                posY += (int)(tempBlock->getRealSize().GetHeight()*getCanvas()->getZoomFactor());
        }
        else
        {
            if (getCanvas())
                posY += (int)(getOriginalSize().GetHeight()*getCanvas()->getZoomFactor()); //##Ojo con considerar a los originalSize todos iguales...
        }
    }
    //##Traducir este comentario: Suma la mitad del último bloque, para que quede "al medio":
    tempBlock = getParamFirstBlock(index);
    if (tempBlock && getCanvas())
    {
        if (tempBlock->getRealSize().GetHeight() != tempBlock->getOriginalSize().GetHeight())
            posY += (int)( (tempBlock->getRealSize().GetHeight()/2)*getCanvas()->getZoomFactor() ) - side/2;
    }
    return wxPoint(GetClientSize().GetWidth() - side/2, posY);
}


void BubbleBlock::onSize(wxSizeEvent& event)
{
    //Square icons, based on the block height (to optimize, their are computed here and passed to the
    //getXXXXSize() and getXXXXSize() functions:

    //##Optimizar / Mejorar todo esto:
    //- Descablear los factores de división internos de los métodos getXXXPos y getXXXSize.
    //- Si hay divisiones por 2, optimizarlas con shifts, igual que luego en el BubbleCanvas (zoom, etc.).
    if (blockButton)
    {
        blockButton->Move(getBlockButtonPos());
        blockButton->SetSize(getBlockButtonSize());
    }

    if (methodButton)
    {
        methodButton->Move(getMethodButtonPos());
        methodButton->SetSize(getMethodButtonSize());
    }

    if (addParamsButton)
    {
        addParamsButton->SetSize(getAddParamsButtonSize());
        addParamsButton->Move(getAddParamsButtonPos());
        removeParamsButton->SetSize(addParamsButton->GetSize());
        removeParamsButton->Move(getRemoveParamsButtonPos());
    }
    if (constantField)
    {
        //These must be called in this specific order (##Este comentario no va más: due to the interna Static control behavior):
        if (constantFieldLabel)
        {
            constantFieldLabel->SetFont(getFontConstantFields());
            constantField->SetFont(getFontConstantFields());
            resizeConstantField();
            constantField->SetPosition(getConstantFieldPos());
            constantFieldLabel->Move(getConstantFieldLabelPos());
        }
    }
    if (instanceNameField)
    {
        instanceNameField->SetSize(getInstanceNameFieldSize());

        instanceNameField->setFont(getFontEditableFields());
        instanceNameField->Move(getInstanceNameFieldPos());
    }

    BubbleParam *iteratorParam = NULL;
    unsigned int index = 0;
    while (index < params.GetCount())
    {
        iteratorParam = &(params.Item(index));
        if (iteratorParam)
        {
            iteratorParam->SetSize(getParamSize());
            iteratorParam->SetPosition(getParamPos(index));
            //iteratorParam->Update();
            //iteratorParam->Refresh();
        }
        index ++;
    }

    event.Skip();
}


//##Prueba para los fondos redondeados, se agrega este método y se deja de heredar de BubblePanel (y por lo
//tanto de wxPanel), para pasar a heredar de wxWindow o de wxControl (según lo que las pruebas indiquen),
//de modo de que el OnPaint sea enteramente el escrito en esta clase, y no tenga nada que ver con el de una
//clase estándar (ya que según la documentación del wxPaintEvent no debería sobreescribirse el OnPaint de
//clases estándar):
//void BubbleBlock::SetBackgroundColourAndRefresh(const wxColour& colour)
//{
//    SetBackgroundColour(colour);
//
//    //This is to do the repainting inmediatly:
//    Refresh();
//    Update();
//}


void BubbleBlock::onPaint(wxPaintEvent& event)
{
    //##Futuro: Se puede agregar opcionalmente (configurable en el entorno, claro) dibujado "3D" de los
    //bloques (me refiero a borde 3D)...

    //##Para todo este método: ¡¡DESCABLEAR, COMENTAR BIEN, Y OPTIMIZAR!!

//##2011.03.18: Prueba de implementación de bloques redondeados:
//    //##Ver si esto no tiene que integrarse con el sistema estándar de visibilidad del wxWindow del que hereda:
//  if (visible)
//  {
        //##A esto le faltaría hacer que los controles hijos realmente puedan "ver" en sus secciones
        //transparente lo pintado aquí, cosa que no ocurre. Una posibilidad es que esta clase hereda de
        //wxPanel, la cual es estándar, y según creo está entre las que el help de wxWidgets recomienda no
        //reescribiles el evento OnPaint... Creo que, ya que estoy dibujando al panel aquí de cero, se podría
        //prescindir de la herencia de wxPanel, para heredar directo de wxControl, o algo de esto...

        wxSize size = GetClientSize();
        wxPaintDC dc(this); //This must be created here, inside the onPaint event, and on the stack.
        //##wxWindowDC dc(this); //This must be created here, inside the onPaint event, and on the stack.
        //##dc.SetBackgroundMode(wxSOLID);

        //dc.DestroyClippingRegion(); //##Test: Works, but has to be improved...
        //dc.SetClippingRegion(wxPoint(0, 0), wxSize(10, 10)); //##Test...

//        if (dragging)
//        {
//            //##Here it will be possible to differentiate the painting during dragging...
//        }
//        else
//        {
            if (enabled)
            {
#if defined (WIN32)
                dc.GradientFillLinear(  wxRect(0, 0, size.GetWidth(), size.GetHeight()),
                                        getDefaultBackgroundColour0(),
                                        getDefaultBackgroundColour1(),
                                        wxSOUTH);
#else
                //##Since the PNG's transparent colour don't work, in Linux the gradient looks really bad, so
                //it's disabled by using the same colour as src and dest colour:
                dc.GradientFillLinear(  wxRect(0, 0, size.GetWidth(), size.GetHeight()),
                                        getDefaultBackgroundColour1(),
                                        getDefaultBackgroundColour1(),
                                        wxSOUTH);
#endif
            }
            else
            {
#if defined (WIN32)
                dc.GradientFillLinear(  wxRect(0, 0, size.GetWidth(), size.GetHeight()),
                                        getDisabledBackgroundColour0(),
                                        getDisabledBackgroundColour1(),
                                        wxSOUTH);
#else
                //##Same as above: gradient disabled under Linux:
                dc.GradientFillLinear(  wxRect(0, 0, size.GetWidth(), size.GetHeight()),
                                        getDisabledBackgroundColour1(),
                                        getDisabledBackgroundColour1(),
                                        wxSOUTH);
#endif
            }

            //Rectángulo redondeado para corregir los bordes:
            dc.SetBrush( wxBrush(wxColour(0, 0, 0), wxTRANSPARENT) );
            float zoomFactor = 1.0;
            if (getCanvas())
                zoomFactor = getCanvas()->getZoomFactor();
#if defined (linux)
            if (getCanvas()){
                dc.SetPen( wxPen(getCanvas()->GetBackgroundColour(), (int)(1)) ); //Solid.
                dc.DrawRectangle(0, 0, size.GetWidth(), size.GetHeight());
            }
#endif
    //##Mejorar la selección de bloques, con un borde:
            wxColour selectionBorderColor(0, 0, 0);
            if (selected)
            {
                selectionBorderColor = defaultSelectionBorderColour;
                dc.SetPen( wxPen(selectionBorderColor, (int)(selectionBorderWidth*zoomFactor)) ); //Solid.
            }
            else
            {
                dc.SetPen( wxPen(selectionBorderColor, 1) ); //Solid.
                //dc.SetPen( wxPen(wxColour(0, 0, 0), 2) ); //Solid.

                //dc.SetBrush( wxBrush(GetBackgroundColour()) );

                //##Test: dc.SetBrush( wxBrush(wxColour(100, 100, 100)) );
                //##Descablear: Ver si se deja la constante multiplicativa en el parámetro de "redondeado":
    //            dc.DrawRoundedRectangle(0, 0,
    //                                    size.GetWidth(), size.GetHeight(),
    //                                    //-1
    //                                    (int)(getOriginalSize().GetHeight()*childFactor*zoomFactor*0.15)
    //                                    );
                //##Corrige las puntas, borrando el gradiente de las mismas:
            }
#if defined (linux)
if (selected)
#endif
            dc.DrawRoundedRectangle(0, 0,
                                    size.GetWidth(), size.GetHeight(),
                                    //-1
                                    (int)(getOriginalSize().GetHeight()*childFactor*zoomFactor*0.15)
                                    );
#if defined (WIN32)
            dc.SetBrush( wxBrush(getCanvas()->GetBackgroundColour()) );
            dc.FloodFill(   0,
                            0,
                            selectionBorderColor,
                            wxFLOOD_BORDER
                        );
            dc.FloodFill(   0,
                            size.GetHeight()-1,
                            selectionBorderColor,
                            wxFLOOD_BORDER
                        );
            dc.FloodFill(   size.GetWidth()-1,
                            0,
                            selectionBorderColor,
                            wxFLOOD_BORDER
                        );
            dc.FloodFill(   size.GetWidth()-1,
                            size.GetHeight()-1,
                            selectionBorderColor,
                            wxFLOOD_BORDER
                        );
#endif
//        }
//  }
    //This is necessary in order to avoid event problems:
    event.Skip();
}

void BubbleBlock::onEraseBackground(wxEraseEvent & event)
{
    //Do nothing, to prevent background erasing...
    //event.Skip();
}


//void BubbleBlock::onPaint(wxPaintEvent& event)
//{
////##Esto no funciona por ahora, debido a que los buttons con imágenes transparentes, llenan las partes
////transparentes con el background del parent del block (osea del Canvas), no con lo que se dibuja aquí:
////    //##Ver si esto no tiene que integrarse con el sistema estándar de visibilidad del wxWindow del que hereda:
////    if (visible)
////    {
//        if (backImage)
//        {
//            wxPaintDC dc(this); //This must be created here, inside the onPaint event, and on the stack.
//            wxSize size = GetClientSize();
//            wxBitmap bmp(backImage->Scale(size.GetWidth(), size.GetHeight()));
//            dc.DrawBitmap(bmp, 0, 0, true);
//        }
////    }
//    //This is necessary in order to avoid event problems:
//    event.Skip();
//}


void BubbleBlock::onMethodLeftDown(wxMouseEvent& event)
{
    if (getCanvas())
    {
        getCanvas()->setCurrentBlock(this);
    }
    event.Skip();
}


void BubbleBlock::onAddParamLeftDown(wxMouseEvent& event)
{
    //##Implementar...
    event.Skip();
}


void BubbleBlock::setName(const wxString& value)
{
    name = value;
//##:
//    if (blockButton)
//        blockButton->SetToolTip(name);
}


void BubbleBlock::enableActionButtons(BubbleBlock *block)
{
    //Enable / disable actionPicker's buttons:
    if (block == NULL)
        return;

    BubbleCanvas *canvas = block->getCanvas();
    if (canvas == NULL)
        return;

    Bubble *bubble = canvas->getBubble();
    if (bubble == NULL)
        return;

    if (bubble->getActionPicker() == NULL)
        return;

    BubbleBlock *nextBlock = canvas->getNextBlock(block);
    wxSortedArrayString tempArray;
    if ( !(block->getComponentStart() || block->getBlockStart() || block->getVariableInit()) )
    {
        //Only enables variableInit blocks (in the actionPicker) if this (the selected) block
        //is one of the following (because variableInits only can be placed at the start of
        //a block or component):
        tempArray.Add(bubble->getVariableInitName());
        bubble->getActionPicker()->enableAllExceptTheseButtons(tempArray, true);
    }
    else if (nextBlock)
    {
        if (nextBlock->getVariableInit())
        {
            //If the the current block is either a variableInit or a start block (both component
            //or block), AND the next block is a variableInit, then disables everything but
            //variableInits in the actionsPicker:
            tempArray.Add(bubble->getVariableInitName());
            bubble->getActionPicker()->enableAllExceptTheseButtons(tempArray, false);
        }
        else
        {
            //Passes an empty array to enable all blocks:
            bubble->getActionPicker()->enableAllExceptTheseButtons(tempArray, true);
        }
    }
    else
    {
        //Passes an empty array to enable all blocks:
        bubble->getActionPicker()->enableAllExceptTheseButtons(tempArray, true);
    }
}


void BubbleBlock::select()
{
    //##Esto se pisa con el evento onSetFocus de los combos:
    //SetFocusIgnoringChildren();
    //SetFocus();

    //##Por ahora, el select() me pareció un buen lugar para actualizar la lista del combo si éste existe:
    //updateInstanceNameList();

    //##Ver si hay que realizar otras acciones:
    //SetWindowStyle(wxBORDER_DOUBLE);
    //SetBackgroundColourAndRefresh(wxColour(192, 192, 255));
    //SetBackgroundColourAndRefresh(wxColour(100, 100, 255));


//##2011.03.20:
    selected = true;
    Refresh();
    Update();

    if (constantField)
        prevInstanceName = constantField->GetValue();

    if (getCanvas())
    {
        getCanvas()->setCurrentBlock(this);
        if (IsEnabled())
        {
            BubbleBlock *block = this;
            if (getCanvas())
            {
                if (getCanvas()->getBubble())
                {
                    //Is this block a param?
                    if ( getCanvas()->getBubble()->getActionDataType() != block->getDataType() )
                    {
                        //Yes, it's a param, so the block to evaluate for the enableActionButtons is the
                        //first action block of the current "line":
                        block = getFirstBackBlock();
                    }
                    enableActionButtons(block);
                }
            }

            if (constantField)
            {
                if (constantFieldLabel)
                    constantFieldLabel->Hide();
                constantField->Show(true);
                validateConstantField(true);
                constantField->SetFocus();
            }
            else if (instanceNameField)
                instanceNameField->SetFocus(); //Sets de focus in the editable fields. //##Esto se va cuando venga el nuevo sistema de menúes.
        }
    }
}


void BubbleBlock::unselect()
{
//    wxMessageDialog dialog0(parent, wxString("unselect"), _("0")); //##Debug.
//    dialog0.ShowModal();//##Debug

    //##Ver si hay que realizar otras acciones:
    //SetWindowStyle(wxBORDER_SIMPLE);
    //SetBackgroundColourAndRefresh(defaultBackgroundColor);
    if (IsEnabled())
    {
//        wxMessageDialog dialog1(parent, wxString("unselect"), _("1")); //##Debug.
//        dialog1.ShowModal();//##Debug

        if (constantField)
        {
//            wxMessageDialog dialog2(parent, wxString("unselect"), _("2")); //##Debug.
//            dialog2.ShowModal();//##Debug

            constantField->Hide();
        }
        if (constantFieldLabel)
        {
//            wxMessageDialog dialog3(parent, wxString("unselect"), _("3")); //##Debug.
//            dialog3.ShowModal();//##Debug

            resizeConstantField();  //Patch: When the label is an empty string, and the block is deselected,
                                    //does not resize without this call.

//            wxMessageDialog dialog4(parent, wxString("unselect"), _("4")); //##Debug.
//            dialog4.ShowModal();//##Debug

            //validateConstantField();
            constantFieldLabel->Show(true);
        }
    }

//    wxMessageDialog dialog5(parent, wxString("unselect"), _("5")); //##Debug.
//    dialog5.ShowModal();//##Debug

    if (!isCommented())
        addVariableWithInstanceName();
    validateConstantField(true);
//##2011.03.20:
    selected = false;
//    SetBackgroundColourAndRefresh(defaultBackgroundColour1);
    Refresh();
    Update();
}


wxSize BubbleBlock::getTotalParamsSize() const
{
    unsigned int totalParamsHeight = 0;
    unsigned int totalParamsWidth = 0;

    BubbleBlock *tempBlock = NULL;
    unsigned int paramsCount = getParamsCount();
    float canvasZoomFactor = getCanvas()->getZoomFactor();
    for (unsigned int i = 0; i < paramsCount; i++)
    {
        tempBlock = getParamFirstBlock(i);
        if (tempBlock == NULL)
        {
            totalParamsHeight += getParamSize().GetHeight();
            totalParamsWidth += getParamSize().GetWidth();
        }
        else
        {
            totalParamsHeight += (unsigned int)(tempBlock->getRealSize().GetHeight()*canvasZoomFactor);
            totalParamsWidth += (unsigned int)(tempBlock->getRealSize().GetWidth()*canvasZoomFactor);
        }
    }
    return wxSize(totalParamsWidth, totalParamsHeight);
}


void BubbleBlock::changeAllBackBlocksRealSize(wxSize delta, bool changeThis)
{
    if (getCanvas() == NULL)
        return;

    if (changeThis)
    {
        if (getCanvas()->getZoomFactor() != 0) //Just another security check (in theory, zoom factor is always != 0).
        {
            if (delta.GetHeight() > 0)
            {
                //The block is growing:
                float totalParamsSize = (float)getTotalParamsSize().GetHeight()/getCanvas()->getZoomFactor();
                if ( (float)(getRealSize() + delta).GetHeight() > totalParamsSize );
                {
                    delta.SetHeight(delta.GetHeight() -
                                    (int)((float)(getRealSize() + delta).GetHeight() - totalParamsSize)
                                   );
                }
            }
            else if (delta.GetHeight() < 0)
            {
                //This recalc is only needed in blocks with instanceNameField, so it's adjusted to take care of the height problems
                //associated with it:
                if (instanceNameField)
                {
                    //The block is shrinking: Here we don't have to compare against the params height, but to the original block's height:
                    float origHeight = ((float)getOriginalSize().GetHeight() +
                                        //##Descablear:
                                       (float)getOriginalSize().GetHeight()*childFactor*0.6);//*getCanvas()->getZoomFactor();
                    if ( (float)(getRealSize() + delta).GetHeight() < origHeight)
                    {
//                        wxMessageDialog dialog0(getCanvas(), _("0"), _("0")); //##Debug.
//                        dialog0.ShowModal();  //##Debug.
                        delta.SetHeight( (int)(origHeight - (float)getRealSize().GetHeight()) );
                    }
                }
            }
        }
//        if ((getRealSize() + delta).GetHeight() > getOriginalSize().GetHeight())
        setRealSize(getRealSize() + delta);
////        else
////            setRealSize(getOriginalSize());
    }
    if (backBlock)
        backBlock->changeAllBackBlocksRealSize(delta, true);
}


//##MUY IMPORTANTE: Ver si puedo optimiar para que no llame tantas veces al constructor de copia de wxImage,
//pasando quizá por referencia no constante la imagen a estas funciones, de modo que la copia la haga
//finalmente el BubbleButton:
void BubbleBlock::setBlockButton(const wxImage& defaultImage,
                                 const wxImage& disabledImage)
{
}


void BubbleBlock::setBlockButton(const wxImage& defaultImage,
                                 const wxImage& hoverImage,
                                 const wxImage& pressedImage,
                                 const wxImage& disabledImage)
{
    setBlockButton(defaultImage, disabledImage);
    if (blockButton)
    {
        blockButton->setImageHover(hoverImage);
        blockButton->setImagePressed(pressedImage);
    }
}


void BubbleBlock::setMethodButton(  const wxImage& defaultImage,
                                    const wxImage& hoverImage,
                                    const wxImage& pressedImage,
                                    const wxImage& disabledImage)
{
    //##
}


bool BubbleBlock::setInstanceNameField(const wxString& value)
{
    //This only sets the field if value param exists in the instances list:
    if (instanceNameField)
        return instanceNameField->setSelection(value);
    return false;
}


wxString BubbleBlock::getInstanceNameField() const
{
    if (instanceNameField)
        return wxString(instanceNameField->getText());
    return wxString("");
}


bool BubbleBlock::getHasAddParamsButton() const
{
    return (addParamsButton != NULL);
}


unsigned int BubbleBlock::getParamsCount() const
{
    return params.GetCount();
}


BubbleBlock *BubbleBlock::getParamFirstBlock(const unsigned int index) const
{
    BubbleParam *tempParam = NULL;
    if (index < params.GetCount())
    {
        tempParam = &(params.Item(index)); //##
    }
    if (tempParam)
        return tempParam->getParamFirstBlock();
    return NULL;
}


int BubbleBlock::getParamIndex(BubbleBlock *block) const
{
    if (block)
    {
        return params.Index(*(block->getBackParamSlot()));
    }
    return wxNOT_FOUND;
}


BubbleParam *BubbleBlock::getParamSlot(const unsigned int index) const
{
    if (getParamsCount() > 0)
        return &(params.Item(index));
    return NULL;
}


 //##Ver si hay otras cosas que no son consistentes con la nomenclautra Param y ParamSlot, y renombrar lo que sea
 //para que quede consistente:
void BubbleBlock::addParamSlot( const wxString& name,
                                const wxString& dataType,
                                const wxImage &assignedImage,
                                const wxImage &notAssignedImage,
                                const wxImage &defaultImage,
                                const wxImage &hoverImage,
                                const wxImage &pressedImage,
                                const wxImage &disabledImage
                              )
{
    try
    {
        //##Esto debe ser COMPLETAMENTE OPTIMIZADO: Como no se borran estos parámetros nuevos que se van
        //creando, se va llenando la memoria cuando el usuario cambia muchas veces de método, así que hay que
        //hacer reasignar a parámetros ya creados cuando se cambie de método, o sea, que hay que tocar ÉSTA
        //función para que si ya hay parámetros creados pero marcados con deleted, o algo así, entonces, se los
        //reasigna y se les cambia el nombre y el tipo, solamente:

        //##Pasar el cálculo de posición y de tamaño también a funciones que se puedan llamar desde este
        //método y desde el OnSize():
        int iconH = (int)( getRealSize().GetHeight() * getCanvas()->getZoomFactor() );
        int iconW = iconH/2;
        BubbleParam *newParam = new BubbleParam(this,
                                                wxNewId(), //##,
                                                dataType,
                                                wxPoint(0, 0),
                                                wxSize(iconW, iconH)
                                              );
        if (newParam)
        {
            //##Esta clase de operaciones se pueden luego hacer con constructores de copia, o algo por el estilo:
            //##Ver si esto es seguro, o si además tengo que verificar las images:
            newParam->setName(name); //##
#if defined (linux)
            newParam->setBackgroundColor(getDefaultBackgroundColour1());
#endif
            newParam->setImageAssigned(assignedImage);
            newParam->setImageNotAssigned(notAssignedImage);
            newParam->setImageDefault(notAssignedImage);
            newParam->setImageHover(hoverImage);
            newParam->setImagePressed(pressedImage);
            newParam->setImageDisabled(disabledImage);
            newParam->SetToolTip(name);
            params.Add(newParam);

            newParam->SetSize(getParamSize());
            newParam->SetPosition(getParamPos(getParamsCount()-1));

            //Each new param has a height equal to "getOriginalSize().GetHeight()":
            if (getParamsCount() > 1)
            {
                //##Cambiar esto para que haga el back y para que tome el último parámetro:
                int newHeight = getRealSize().GetHeight() + getOriginalSize().GetHeight();
                setRealSize(wxSize(getRealSize().GetWidth(), newHeight));
                changeAllBackBlocksRealSize(wxSize(0, getOriginalSize().GetHeight()), false);
            }

            newParam->Connect(  wxEVT_ENTER_WINDOW,
                                wxMouseEventHandler(BubbleBlock::onMouseEnter),
                                NULL,
                                this
                             );

            //If there are removable params, makes the removeParamsButton visible:
            if (getParamsCount() > getMinParamsCount())
            {
                if (removeParamsButton)
                    removeParamsButton->Show(true);
            }

            if (getCanvas())
            {
                //##Not really necessary by now, but may be in the future:
                //getCanvas()->updateCodeAndRefresh();
                getCanvas()->forceSaved(false);
            }
        }
    }
    catch(...)
    {
    }
}


//Rename removeParaSlots?:
void BubbleBlock::removeParams()
{
    unsigned int paramsCount = getParamsCount();
    for (unsigned int j = 0; j < paramsCount; j++)
    {
        //Always delete the first param (which in fact, deletes all of them after the iterations):
        removeParamSlot(0);
    }
}


void BubbleBlock::removeParamSlot(const unsigned int index)
{
    //##Debug:
//    wxString temp;
//    temp << params.GetCount();
//    wxMessageDialog dialog0(parent, temp, _("GetCount:")); //##Debug.
//    dialog0.ShowModal();  //##Debug.

    BubbleParam *tempParamSlot = NULL;
    if (index < params.GetCount())
    {
        //##Borrar de verdad!! Esto es por ahora (probar el DeleteChild o algo similar, O MEJOR: ver qué
        //se hace luego con el subsistema de UnDo/ReDo):
        tempParamSlot = &(params.Item(index));
        if (tempParamSlot)
        {
            BubbleBlock *tempParamSlotFirstBlock = tempParamSlot->getParamFirstBlock();
            if (tempParamSlotFirstBlock)
            {
                //wxMessageDialog dialog0(parent, wxString("paramDestroy"), _("1")); //##Debug.
                //dialog0.ShowModal(); //##Debug,

                //tempParamSlotFirstBlock->Hide();
                tempParamSlotFirstBlock->showParams(false);
                //tempParamSlotFirstBlock->removeParams(); //##
                if (getCanvas())
                {
                    getCanvas()->RemoveChild(tempParamSlotFirstBlock);
                    getCanvas()->setDraggedBlock(NULL);  //To avoid interference with the drag&drop subsystem.
                }
                tempParamSlotFirstBlock->Destroy();
                tempParamSlot->setParamFirstBlock(NULL);
            }
        }
        //tempParam->Hide();
        params.Detach(index);   //##Ojo con esto!!: Ver que este destroy no genere problemas de doble borrado
        if (tempParamSlot)
        {
            RemoveChild(tempParamSlot); //cuando se borra el parent (en este caso el block).
            tempParamSlot->Destroy();   //Dont' use the delete operator here (see wxWidgets documentation).
            tempParamSlot = NULL;
        }
        if (getCanvas())
        {
            getCanvas()->updateCodeAndRefresh();
            getCanvas()->forceSaved(false);
        }
    }
}


BubbleBlock *BubbleBlock::getParamSlotFirstBlock(const unsigned int index) const
{
    BubbleParam *tempParam = &(params.Item(index));
    if (tempParam)
    {
        if (tempParam->getParamFirstBlock())
        {
            return tempParam->getParamFirstBlock();
        }
    }
    return NULL;
}


wxArrayString BubbleBlock::getCode() const
{
    //##See if this logic will remain here:
    wxArrayString tempCode;

    if (getHasConstantField())
    {
        if (constantField)
        {
            wxString firstCodeLine = wxString("");
            if (!code.IsEmpty())
                firstCodeLine = code[0];
            tempCode.Clear();
            if (getUpdateVariables())
            {
//                if (methodButton)
                    tempCode.Add(firstCodeLine + constantField->GetValue() + getMethodSeparator() + getDefaultMethodName());
//                else
//                    tempCode.Add(firstCodeLine + constantField->GetValue() + getMethodSeparator());
                return tempCode;
            }
            else
            {
                tempCode.Add(firstCodeLine + BubbleXML::getXMLEncodedString(constantField->GetValue()));
                return tempCode;
            }
        }
    }
    else if (instanceNameField)
    {
        //##No sé si en el futuro se dejará, pero por ahora, esto permite realizar control de errores, al menos
        //en tiempo de generación de código, y permite encontrar errores tales como que el usuario cambió el
        //nombre de una variable y había getters que la utilizaban...
        //##Por supuesto que luego hay agregar el control en tiempo real, para evitar que el usuario olvide
        //cambiar otros setters potenciales (ají se podría implementar un color de "Warning" que muestre que
        //hubo modificaciones):
        if (getCanvas())
        {
            if  ( getCanvas()->getInstanceIndex(instanceNameField->getText()) == wxNOT_FOUND )
                instanceNameField->setBackgroundColour(defaultErrorColour);
            else
                instanceNameField->setBackgroundColour(defaultEditableFieldsColour);
        }

        //if (methodButton)
        //{
            //##Implemetar:
            //##Si tiene nombre de instancia, y hay método, devolverá el name de instancia + methodSeparator + name de método...
            //##Implementar el subistema de código de métodos (con el getName del methodButton)...

            wxString firstCodeLine = wxString("");
            if (!code.IsEmpty())
                firstCodeLine = code[0];

            tempCode.Clear();

//            if (methodButton)
                tempCode.Add(firstCodeLine + instanceNameField->getText() + getMethodSeparator() + getDefaultMethodName());
//            else
//                tempCode.Add(firstCodeLine + instanceNameField->getText() + getMethodSeparator());

            return tempCode;
        //}
    }

    return code;
}


unsigned int BubbleBlock::getIndentation() const
{
    if (getCanvas())
    {
        BubbleBlock *first = getCanvas()->getFirstBlock();
        if (first)
        {
            //Adds 2 to the numerator and substracts 1 to the denominator to avoid rounding problems:
            return (unsigned int)(((GetPosition().x - first->GetPosition().x) + 2) /
                                  (getCanvas()->getTabWidth()*getCanvas()->getZoomFactor() - 1)
                                 );
        }
    }
    return 0;
}


wxString BubbleBlock::getAttributes() const
{
    //##Descablear estos keywords, o sacarlos al menos de constantes, o algo así, para evitar errores no
    //comprobables en compile-time:
    wxString result(wxString(" function=") + wxString("\"") +function + wxString("\""));
    result += wxString(" commented=") + wxString("\"") + Bubble::bool2string(commented) + wxString("\"");
    if (instanceNameField)
        result += wxString(" instanceName=") + wxString("\"") + instanceNameField->getText() + wxString("\"");
    if (constantField)
        result += wxString(" constantValue=") + wxString("\"") + constantField->GetValue() + wxString("\"");
    result += wxString(" paramsCount=") + wxString("\"") +  (wxString("") << getParamsCount()) + wxString("\"");
    result += wxString(" hasAddParamsButton=") + wxString("\"") + Bubble::bool2string(addParamsButton != NULL) + wxString("\"");
    result += wxString(" loadAction=") + wxString("\"") + getLoadAction() + wxString("\"");

    return result;
}


BubbleBlock *BubbleBlock::getFirstBrother() const
{
    BubbleBlock *returnBlock = NULL;
    BubbleBlock *iteratorBlock = getPrevBrother();
    while(iteratorBlock)
    {
        returnBlock = iteratorBlock;
        iteratorBlock = iteratorBlock->getPrevBrother();
    }
    return returnBlock;
}


BubbleBlock *BubbleBlock::getLastBrother() const
{
    BubbleBlock *returnBlock = NULL;
    BubbleBlock *iteratorBlock = getNextBrother();
    while(iteratorBlock)
    {
        returnBlock = iteratorBlock;
        iteratorBlock = iteratorBlock->getNextBrother();
    }
    return returnBlock;
}
