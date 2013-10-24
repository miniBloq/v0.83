//##Agregar licencia y avisos de copyright de terceros en TODOS LOS headers y cpp.

#include "Bubble.h"

#include <wx/msgdlg.h> //##Debug, se va en release. Agregar compilación condicional.
#include <wx/dir.h>
#include <wx/filename.h>

#if defined (WIN32)
//##@# Possible non-portable code:
#include <windows.h>
#endif

//##:
//#include <string>
//#include <vector>


//##2010.10.04: Ver que esto no dé error de runtime: Dejar este mensaje interno, aunque pasarlo a
//inglés, por si se reportara un bug al respecto en el futuro. Al menos dejarlo hasta ver bien cómo
//trabaja WX_DEFINE_OBJARRAY() y ver por qué en algunos compiladores, si se usa este macro sin tener
//en el scope la full-declaration de la clase de los elementos del array, ni siquiera dan warning:
//WX_DEFINE_OBJARRAY(arrayOfCanvas);


//##Ver si hay que usar un wxList acá, o si se usará el sistema de tabs, haciendo que Bubble herede
//de las clases para manejar NoteBooks y Tabs:

//We used wxList instead of wxObjArray because the list does not deletes the objects, which is already
//done by de parent-subsystem in the frame or dialog to which those objets belongs too. Anyway, these
//are canvases, and with a wxList is enough.
#include <wx/listimpl.cpp>
WX_DEFINE_LIST(listOfCanvas);


//##Ver cómo agregar chequeo de que si no setean el parent y el notifier, TODO MAL!
/*
Bubble::Bubble(IBubbleNotifier *notifier) : parent(NULL),
                                            currentCanvas(NULL),
                                            notifier(notifier)
*/

//##Más adelante, puede que arme otro constructor, al que se pasen ambos parámetros (parent y notifier), pero
//tengo que asegurarme que no haya problemas en la destrucción, por las cosas que destruye en parent
//automáticamente, cosa que en esta configuración actual, en la que con este constructor es posible crear
//la instancia en el stack (estática) en vez del heap, y luego hacer los "sets", funciona bien.

//El parent muchas veces no se puede pasar en el constructor de la instancia de Bubble (ya que a veces, como
//en Minibloq, el parent se crea DESPUÉS que la instancia de Bubble), por lo que tampoco se pasa el notifier,
//por consistencia.
Bubble::Bubble(wxLocale& locale) :  parent(NULL),
                                    currentCanvas(NULL),
                                    actionPicker(NULL),
                                    bubbleXML(this, locale),
                                    actionDataType(wxString("")),
                                    variableInitName(wxString("")),
                                    notifier(NULL),
                                    bootPortName(wxString("")),

                                    hardwareManager(NULL),
                                    boardName(wxString("")),

                                    includesCodeList(wxString("")),
                                    includesBuildList(wxString("")),
                                    initBoardCode(wxString("")),

                                    blocksEnabled(true),
                                    visibleLabels(false),

                                    componentsRepositoryPath(wxString("")),
                                    toolsPath(wxString("")),
                                    blocksPath(wxString("")),
                                    host(wxString("win.i386")), //Default OS
                                    appPath(wxString("")),
                                    themePath(wxString("")),

                                    //##Will be necessary?:
                                    docPath(wxString("")),

                                    componentPath(wxString("")),
                                    outputPath(wxString("")),

                                    simplifyCode(false)
{
    //##Falan implementar cosas...?
    saved = true;   //##Ver cómo funciona el tema de la herencia con clases abstractas, si se pueden
                    //inicializar las variables en la lista del constructor de la clase abstracta...
}


Bubble::~Bubble()
{
    //##No va: Como los elementos de la lista son descendientes de wxPanel, y tienen sus parents,
    //el frame o dialog al que pertenecen los destruirá. Y aquí, como el objeto canvases es estático,
    //se asegura la llamada a su destructor sin hacer nada:
    //canvases.Clear();

    //##Ver que funcione bien esto:
    if (bootSerialPort.IsOpen())
        bootSerialPort.Close();

    //Destroy all the pickers:
    if (actionPicker)
    {
        actionPicker->Destroy();
        actionPicker = NULL; //Not necessary, but for extra security.
    }
    clearExpressionPickers();
}


#if defined (WIN32)
LPWSTR Bubble::cstrToWChar(LPCSTR value)
{
    LPWSTR result = NULL;
    int valueLen = lstrlenA(value);
    int resultLen = MultiByteToWideChar(CP_ACP, 0, value, valueLen, 0, 0);
    if (resultLen > 0)
    {
        result = SysAllocStringLen(0, resultLen);
        MultiByteToWideChar(CP_ACP, 0, value, valueLen, result, resultLen);
    }
    return result;
}
#endif


wxString Bubble::bool2string(const bool value)
{
    if (value)
        return wxString("true");
    return wxString("false");
}


//NOTE: Only checks for "true", any other string is false:
bool Bubble::string2bool(const wxString &value)
{
    if (value == wxString("true"))
        return true;
    return false;
}


bool Bubble::nonListedCharsInString(const wxString &charList, const wxString &value)
{
    for (unsigned int i=0; i<value.Len(); i++)
    {
        if (charList.Find(value[i]) == wxNOT_FOUND)
            return true;
    }
    return false;
}


#if UNDER_DEVELOPMENT
//##No borrar, este código es muy útil: Es una función DC2Bitmap bastante rápida, aunque los problemas que
//tiene si se la quiere usar para capturar los bitmaps asociados a los bloques son los siguientes:
//
//- Sólo toma los bloques si éstos están visibles en pantalla.
//- Además, no captura el dc de comboboxes y edits.
//
//Se la puede llamar con cosas tipo:
//
//  wxClientDC dc(currentBlock);
//  wxBitmap bmp(dc2Image(&dc));
//
static wxImage dc2Image(wxClientDC *dc) //##Pasar el puntero a constante.
{
    if (dc)
    {
        wxCoord dcW, dcH,
                dcX, dcY;
        dc->GetSize(&dcW, &dcH);
        wxImage tempImage(dcW, dcH);
        wxColour tempColour(0, 0, 0);
        for (dcX = 0; dcX < dcW; dcX++)
        {
            for (dcY = 0; dcY < dcH; dcY++)
            {
                if (dc->GetPixel(dcX, dcY, &tempColour))
                {
                    tempImage.SetRGB(dcX, dcY, tempColour.Red(), tempColour.Green(), tempColour.Blue());
                }
            }
        }
        return tempImage;
    }

    //If something goes wrong, return a dummy:
    return wxImage();
}
#endif


bool Bubble::isSaved() const
{
    //##Futuro: Debe recorrer todos los canvases y devolver el AND de todo eso.
    if (currentCanvas)
        return saved && currentCanvas->isSaved();

    return saved;
}


//##This function MUST be called after the construction (this may be improved in the future):
void Bubble::setCanvasesParent(wxWindow *value)
{
    parent = value;
}


void Bubble::setNotifier(IBubbleNotifier *value)
{
    notifier = value;
}


IBubbleNotifier *Bubble::getNotifier()
{
    return notifier;
}


void Bubble::closeTemporalPickers()
{
    BubbleExpressionPicker* iterator = NULL;
    for (unsigned int i = 0; i<expressionPickers.GetCount(); i++)
    {
        iterator = expressionPickers.Item(i);
        if (iterator)
        {
            if (iterator->getCloseAfterPick())
                iterator->Hide();
        }
    }
};


BubbleExpressionPicker* Bubble::getExpressionPicker(const wxString &dataType)
{
    //##Debug:
    if (getNotifier() == NULL)
        return NULL;
    //getNotifier()->showMessage(dataType + wxString("\n"), false, true, *wxGREEN); //##Debug.

    //##Optimize in the future (binary search, if possible):
    BubbleExpressionPicker* iterator = NULL;
    for (unsigned int i = 0; i<expressionPickers.GetCount(); i++)
    {
        //getNotifier()->showMessage((wxString("\n") << expressionPickers.GetCount()) + wxString("\n"), false, true, *wxRED); //##Debug.
        iterator = expressionPickers.Item(i);
        if (iterator)
        {
            //getNotifier()->showMessage(iterator->getDataType() + wxString("\n"), false, true, *wxRED); //##Debug.
            //getNotifier()->showMessage(dataType + wxString("\n"), false, true, *wxRED); //##Debug.
            if (iterator->getDataType() == dataType)
            {
                //getNotifier()->showMessage(_("Picker FOUND\n"), false, true, *wxRED); //##Debug.
                return iterator;
            }
        }
    }

    return NULL;
}


void Bubble::clearExpressionPickers()
{
    //This does not call expressionPickers.Clear(), because the pickers are dialogs (at least in the)
    //current implementation, and it's better to destroy them calling iterator->Destroy(), not with the
    //delete operator.
    //Then this functions empties the array, but Empty() does not call the delete operator.

    //##If the pickers change to other type of thing instead of dialogs (for example panels), then this will
    //need to be done in a different way, probably calling parent's removeChild() function.

    BubbleExpressionPicker* iterator = NULL;
    for (unsigned int i = 0; i<expressionPickers.GetCount(); i++)
    {
        iterator = expressionPickers.Item(i);
        if (iterator)
        {
            iterator->Destroy();
            iterator = NULL; //For security only.
        }
    }
    expressionPickers.Empty();
}


bool Bubble::addBlockToPicker(BubbleBlockInfo *block, wxWindow *pickersParent)
{
    if (block == NULL)
        return false;
    if (pickersParent == NULL)
        return false;

    //##Debug:
    if (getNotifier() == NULL)
        return false;

    BubblePicker *picker = NULL;
    if (block->getDataType() == getActionDataType())
    {
        picker = actionPicker;
    }
    else
    {
        //##NOTA: No voy a agregar una lista de dataTypes en un XML, sino que ésta se armará aquí automáticamente.
        //Esto no sólo es mucho más amigable para con los desarrolladores de bloques, sino que es la única opción
        //que se me ocurre, realmente compatible con la filosofía "Minibloq" de agregar, y no modificar, ya que
        //para agregar nuevos tipos de datos, basta con agregar los bloques que los utilizan, sin andar listándolos
        //explícitamente por ahí. La contra es que es un poco más lento al cargar los bloques.
        picker = getExpressionPicker(block->getDataType());
        if (picker == NULL)
        {
            //getNotifier()->showMessage(block->getDataType() + wxString("\n"), false, true, *wxRED); //##Debug.
            BubbleExpressionPicker *newPicker = new BubbleExpressionPicker( pickersParent,
                                                                            this,
                                                                            wxNewId(),
                                                                            //##Ver si en el futuro se podrá internacionalizar algo,
                                                                            //o mejorar estas descripciones agregando algún archivo
                                                                            //de documentación, pero por ahora van así:
                                                                            block->getDataType(),
                                                                            block->getDataType(),
                                                                            64, //32, //##
                                                                            wxColour(0, 0, 0), //##Ver tema color.
                                                                            7 //Cols
                                                                          );
            if (newPicker)
            {
                //getNotifier()->showMessage(_("New Picker = ") + block->getDataType() + wxString("\n"), false, true, *wxRED); //##Debug.
                newPicker->setCloseAfterPick(true);
                //getNotifier()->showMessage(newPicker->getDataType() + wxString("\n"), false, true, *wxRED); //##Debug.
                expressionPickers.Add(newPicker);
                //##Falta destruir los pickers al final, o cuando se hace el reload.
                picker = newPicker;
            }
        }
    }

    if (picker)
    {
        picker->addButton(  block->getName(),
                            block->getFunction(),
                            block->getDataType(),
                            block->getToolTip(),
                            block->getDefaultBackgroundColour1(),
                            block->getPickerDefaultImage(),
                            block->getPickerPressedImage(),
                            block->getPickerHoverImage(),
                            block->getPickerDisabledImage()
                         );
    }
    return true;
}


const BubbleBlockInfo& Bubble::getBlockInfo(const wxString& name, const wxString& function)
{
    return bubbleXML.getBlockInfo(name, function);
}


bool Bubble::blockIsValid(const wxString& name, const wxString& type) const
{
    return bubbleXML.blockIsValid(name, type);
}


int Bubble::loadBlocksInfo(wxWindow *pickersParent, bool showPickers)
{
    return bubbleXML.loadBlocksInfo(pickersParent, showPickers);
}


int Bubble::loadHardwareTargets(BubbleHardwareManager *hardwareManager)
{
    return bubbleXML.loadHardwareTargets(hardwareManager);
}


void Bubble::setCurrentCanvas(BubbleCanvas *value)
{
    currentCanvas = value;
    if (currentCanvas)
        currentCanvas->SetFocus();
    if (getNotifier()) //So the notified GUI can show the zoom level for the new currentCanvas.
    {
        getNotifier()->zoomNotify();
        closeTemporalPickers();
    }
}


void Bubble::addCanvas( wxWindowID id,
                        bool mainCanvas,
                        int defaultHScrollInc,
                        int defaultVScrollInc
                      )
{
    //##Implementar esto, y sincronizar todo con el sistema de Tabs:
    //##Recibir el color de fondo como parámetro (para que la aplicación lo levante de un XML de configuración):

    //##Levantar todo esto del XML:

    BubbleCanvas *newCanvas = new BubbleCanvas( parent,
                                                this,
                                                id,
                                                defaultHScrollInc,
                                                defaultVScrollInc,
                                                bubbleXML.getCanvasInfo(mainCanvas)
                                              );
    if (newCanvas)
    {
        canvases.Append(newCanvas);
        currentCanvas = newCanvas;
        saved = false;
        loadAcceleratorTableFromFile(); //##Esto va a cambiar casi seguro: Se agregará el nombre
                                        //del archivo del que se cargan los acceleratorKeys, y
                                        //quizá se agreguen más cosas de cofiguración allí, no sé.
    }
	//##newCanvas->SetBackgroundColourAndRefresh(wxColour(r, g, b));
}


//##Ver qué parámetros recibe:
void Bubble::deleteCanvas(BubbleCanvas *const value)
{
//##Try-catch
    saved = false;

    //##El currentCanvas debería ser el canvas anterior, si existe:
    //Esto no está cotemplado ahora, pero puede generar un crash de la aplicación, ya que podría ocurrir
    //que no quede apuntando a ningún lado:
    //Hay que usar canvases.IndexOf(value), restar 1 y ver que exista, para luego asignar el currentCanvas.
    //##Por ahora, asigno el primer canvas como el current tras el borrado, para evitar el posible cuelque
    //si currentCanvas queda apuntado al que se eliminó:
    listOfCanvas::iterator iter = canvases.begin();
    currentCanvas = *iter;

    //Removes the object from the list, but does not destroy it (the application must destroy it):
    canvases.DeleteObject(value);

    if (canvases.empty())
    {
        currentCanvas = NULL;
    }

    if (currentCanvas)
        currentCanvas->SetFocus();
}


bool Bubble::setBoardName(const wxString& value, wxWindow *pickersParent)
{
    if (pickersParent == NULL)
        return false;

    boardName = value;
    if (getNotifier())
    {
        //VERY IMPORTANT: The notifier MUST destroy the current canvas, and create a new one:
        getNotifier()->changeBoardNotify();
        //wxMessageDialog dialog0(parent, getHardwareManager()->getCurrentBoardProperties()->getObjectExtension(), _("setBoardName:")); //##Debug.
        //dialog0.ShowModal(); //##Debug.
    }
    else
    {
        //If there is no notifier, the current canvas can't be destroyed, and THIS IS AN ERROR:
        return false;
    }
    enableAllBlocks(false);

    bubbleXML.loadBlocksInfo(pickersParent, getCurrentCanvas() != NULL);  //Only shows the Actions picker if there is
                                                                //already a canvas.
    enableAllBlocks(true);

    return true;
}


wxString Bubble::getOutputObjectsList(const wxString &fileExtension)
{
    wxString list("");

    wxDir dir(outputPath);
    if ( !dir.IsOpened() )
        return list;

    wxString fileName;
    //##This filter is not working, apparently due to a wxDir known bug:
    //bool result = dir.GetFirst(&fileName, wxString("*.") + fileExtension, wxDIR_DEFAULT);
    bool result = dir.GetFirst(&fileName, wxEmptyString, wxDIR_DEFAULT);
    while (result)
    {
        if (fileName.AfterLast('.') == fileExtension)
            list += wxString("\"") + outputPath + wxString("/") + fileName + wxString("\" ");
        result = dir.GetNext(&fileName);
    }

    return list; //##Not used by now.
}


int Bubble::loadBoardRelations()
{
    int result = bubbleXML.loadBoardRelations();
    if ( getNotifier() )
    {
        updateCode();
        getNotifier()->refreshGeneratedCodeNotify();
    }
    return result;
}


BubbleBlockInfo Bubble::loadBlockFromXML(wxXmlNode *child)
{
    wxString returnValue("");

    //It doen's matter if there is no function attribute (that's because I don't call HasAttribute here):
    returnValue = child->GetAttribute(wxString("function"), wxString(""));
    BubbleBlockInfo blockInfo(bubbleXML.getBlockInfo(child->GetName(), returnValue));
    blockInfo.setLoading(true); //Very important!

    //The rest of the attributes:
    if (child->HasAttribute(wxString("commented")))
    {
        returnValue = child->GetAttribute(wxString("commented"), wxString(""));
        blockInfo.setCommented(string2bool(returnValue));
    }
    if (child->HasAttribute(wxString("instanceName")))
    {
        returnValue = child->GetAttribute(wxString("instanceName"), wxString(""));
        blockInfo.setInstanceNameFieldDefaultValue(returnValue);
    }
    if (child->HasAttribute(wxString("constantValue")))
    {
        //##Ver si hay que guardar y levantar el tipo de dato también:
        returnValue = child->GetAttribute(wxString("constantValue"), wxString(""));
        blockInfo.setConstantFieldDefaultValue(returnValue);
    }
    if (child->HasAttribute(wxString("hasAddParamsButton")))
    {
        //If the block has addParamsButton, then has to add the possible extra paramSlots:
        returnValue = child->GetAttribute(wxString("hasAddParamsButton"), wxString(""));
        if (returnValue == wxString("true"))
        {
            if (child->HasAttribute(wxString("paramsCount")))
            {
                returnValue = child->GetAttribute(wxString("paramsCount"), wxString(""));
                long paramsCount = 0;
                if (returnValue.ToLong(&paramsCount))
                {
                    while (blockInfo.getParamsCount() < (unsigned int)(paramsCount))
                    {
                        //The addParamSlot in the blocks, always add paramSlots equal
                        //to the first one (thats why I use 0 here as index):
                        blockInfo.addParam( blockInfo.getParamName(0),
                                            blockInfo.getParamDataType(0),
                                            blockInfo.getParamNotAssignedImage(0),
                                            blockInfo.getParamDefaultImage(0),
                                            blockInfo.getParamHoverImage(0),
                                            blockInfo.getParamPressedImage(0),
                                            blockInfo.getParamDisabledImage(0)
                                          );
                    }
                }
            }
        }
    }

    return blockInfo;
}


void Bubble::loadParamsFromXML(wxXmlNode *child)
{
    if (child == NULL)
        return;
    if (currentCanvas == NULL)
        return;

    BubbleBlock *currentBlock = currentCanvas->getCurrentBlock();
    if (currentBlock == NULL)
        return;

    unsigned int i = 0;
    wxXmlNode *paramChild = child->GetChildren();
    while (paramChild)
    {
        //##loadXMLParams
        if (paramChild->GetName() != wxString("NULLParam"))
        {
            currentCanvas->setCurrentBlock(currentBlock);
            currentCanvas->addParam(loadBlockFromXML(paramChild),
                                    currentCanvas->getCurrentBlock()->getParamSlot(i),
                                    false
                                   );
        }
        //If it's a NULL param, just increment the paramSlot index:
        loadParamsFromXML(paramChild);
        i++;
        paramChild = paramChild->GetNext();
    }

    //##loadParamsFromXML(child->GetChildren());
}


bool Bubble::loadComponentFromFile(const wxString& name)
{
    //##Futuro: Agregar robustez, para que no se cuelgue si el XML está mal formateado, etc..

    if (currentCanvas == NULL)
    {
        if (getNotifier())
            getNotifier()->showMessage(_("Internal error: There is no \"currentCanvas\"."), true, true, *wxRED);
        return false;
    }

    //This is to avoid bad painting while loading component, when the open file dialog was maximized before
    //open the file, thus causing the frame to be badly draw for a moment:
    if (parent)
    {
        parent->Refresh();
        parent->Update();
    }

    //This reduces the flicker, specially in the start block:
    currentCanvas->showAllBlocks(false);

    wxXmlDocument componentFile;
    if ( !componentFile.Load(name, wxString("UTF-8")) )
    {
        currentCanvas->showAllBlocks(true);
        return false;
    }
    //componentFile.Save( name + wxString("_") );//##Debug.
    //componentFile.Save( name + wxString("_", wxXML_NO_INDENTATION) );//##Debug.


    wxXmlNode *root = componentFile.GetRoot();
    if (root == NULL)
    {
        currentCanvas->showAllBlocks(true);
        return false;
    }

    if (root->GetName() != wxString("mbqc"))
    {
        currentCanvas->showAllBlocks(true);
        return false;
    }

    //##Ver si hay que agregar seguridades a todo este proceso:
    wxXmlNode *rootChild = root->GetChildren();
    wxXmlNode *child = NULL;

    while (rootChild)
    {
        //Load de properties:
        if (rootChild->GetName() == "properties")
        {
            child = rootChild->GetChildren();
            while (child)
            {
                if (child->GetName() == "blocks")
                {
                    //Reset the progress bar:
                    if (getNotifier())
                    {
                        getNotifier()->clearMessage();
                        if ( child->HasAttribute(wxString("count")) )
                        {
                            wxString blocksCount = child->GetAttribute(wxString("count"), wxString("1"));
                            long blocksCountNumber = 1;
                            getNotifier()->setProgressMax(1);
                            if (blocksCount.ToLong(&blocksCountNumber))
                            {
                                if (blocksCountNumber > 1)
                                    getNotifier()->setProgressMax(blocksCountNumber);
                            }
                            getNotifier()->setProgressPosition(0, false, false);
                        }
                    }
                }
                child = child->GetNext();
            }
        }
        //Load variables:
        else if (rootChild->GetName() == "variables")
        {
            child = rootChild->GetChildren();
            while (child)
            {
                BubbleInstance *var = new BubbleInstance(child->GetName(), wxString("Variable"));
                currentCanvas->setInstance(var);
                child = child->GetNext();
            }
        }
        //Load Blocks:
        else if (rootChild->GetName() == "blocks")
        {
            child = rootChild->GetChildren();
            while (child)
            {
                //##Pasar todos estos strings de los atributos a constantes, para evitar errores de cut & paste
                //con los otros lugares donde se utilizan, en como la clase BubbleBlock, por ejemplo:
                wxString returnValue("");
                if ( child->HasAttribute(wxString("loadAction")) )
                {
                    returnValue = child->GetAttribute(wxString("loadAction"), wxString(""));
                    if ( returnValue == wxString("load") ) //##Ver si hay que descablear, o hacer una función auxiliar...
                    {
                        //##Esto ya parece funcionar con estructuras con múltiples hermanos, tipo
                        //"if-elseif-...-else-endif", así que sólo faltaría implementar el sistema de
                        //addBrother a los blocks y ver si funciona:
                        currentCanvas->addBlock(loadBlockFromXML(child), false);
                        if (currentCanvas->getCurrentBlock())
                        {
                            BubbleBlock *brother = currentCanvas->getCurrentBlock()->getNextBrother();
                            std::stack<BubbleBlock*> invertingStack;
                            while (brother)
                            {
                                invertingStack.push(brother);
                                brother = brother->getNextBrother();
                            }
                            while(!invertingStack.empty())
                            {
                                brothers.push(invertingStack.top());
                                invertingStack.pop();
                            }
                        }
                        loadParamsFromXML(child);
                    }
                    else if ( returnValue == wxString("brother") )
                    {
                        if (!brothers.empty())
                        {
                            BubbleBlock *prevBrother = brothers.top();
                            if (prevBrother)
                            {
                                brothers.pop();
                                currentCanvas->setCurrentBlock(prevBrother);
                                loadParamsFromXML(child);
                            }
                        }
                    }
                    //else if ( returnValue == wxString("noLoad") ) --> Do nothing!
                }
                //Show the loading progress:
                if (getNotifier())
                    getNotifier()->setProgressPosition(getNotifier()->getProgressPosition() + 1, true, false);

                //getNotifier()->showMessage(child->GetName() + wxString(" ") + function + wxString(" ") + instanceName + wxString("\n"), false, true, *wxGREEN); //##Debug
                child = child->GetNext();
            }
        }
        rootChild = rootChild->GetNext();
    }

    currentCanvas->setCurrentBlock(currentCanvas->getFirstBlock());
    currentCanvas->scrollToHomeAbsolute();
    currentCanvas->showAllBlocks(true);
    currentCanvas->forceSaved(true);

    if (getNotifier())
    {
        //getNotifier()->setProgressPosition(getNotifier()->getProgressMax(), true, false);
        getNotifier()->hideMessagesWindow();
    }

    saved = true;
    return true;
}


//##Esto funciona, pero la indentación es un asco. Ver si en el futuro paso al manejo automático de
//wxXMLDocument también en grabación, aunque no sé, porque esto es muy rápido además:
wxString Bubble::generateXMLFromParams(BubbleBlock *block)
{
    wxString tempXML("");
    if (block == NULL)
        return tempXML;
    tempXML +=  wxString("<") +
                block->getName() +
                block->getAttributes() +
                wxString(">\n");

    //Indentation:
    wxString indentation("\t");
    for (unsigned int i = 0; i < block->getIndentation(); i++)
        indentation += wxString("\t");

    unsigned int paramsCount = block->getParamsCount();
    for (unsigned int paramIndex = 0; paramIndex < paramsCount; paramIndex++)
    {
        BubbleBlock *paramBlock = block->getParamFirstBlock(paramIndex);
        if (paramBlock)
        {
            //##Futuro: Mejorar la indentación, para que no haya 2 tabs en los parámetros:
            tempXML += indentation + wxString("\t") + wxString("\t") + generateXMLFromParams(paramBlock);
        }
        else
        {
            //##Pasar estos strings (como "NULLParam") a constantes para evitar errores y para emprolijar:
            //No param attached to the slot:
            tempXML +=  indentation + wxString("\t") +
                        wxString("<NULLParam loadAction=\"noLoad\">\n") +
                        indentation + wxString("\t") +
                        wxString("</NULLParam>\n");
        }
    }
    tempXML +=  indentation + wxString("\t") +
                wxString("</") +
                block->getName() +
                wxString(">\n");

    return tempXML;
}


bool Bubble::saveComponentToFile(const wxString& name, bool format)
{
    //##La indentación funciona mal acá, así que por ahora, se creó la función formatComponentFile, que
    //vuelve a abrir el archivo aquí generado, lo carga en un wxXmlDocument y lo graba indentado. Igual es

    //NOTE: I commented all the lines regarding the progressBar, because this caused a lot of flickering in
    //the in screen, specially if the hardware manager was visible. But they can be easily re-activated.
    //So please DON'T DELETE THE COMMENTED LINES (those with the "getNotifier" string) below:

    try
    {
        //First, reset the progress bar:
//        if (getNotifier())
//        {
//            getNotifier()->setProgressPosition(0, false, false);
//            getNotifier()->clearMessage();
//        }

        //##Nota acerca del tipo de archivo: Por ahora usa el typeDefault, que en teoría debería generar
        //archivos con terminación de línea "DOS". Pero más adelante ser verá, y quizá se pase todo a Unix.
        //Si esto se cambia, ver qué funciones hay que tocar (en principio, al menos hay que llamar a Write()
        //con otro tipo de archivo, porque ahora usan el parámetro con valor por defecto). Algo que sí me
        //gustaría es no usar el default, para que todos los Minibloq generen el mismo tipo de archivo, sin
        //importar el sistema operativo, lo cual dará más uniformidad, pero no estoy seguro...

        //##
        //##Los mensajes de grabación, etc., son también rojos si hay error, y azules si todo fue bien. Los de
        //compilación existosos son verdes... (al menos por ahora me gusta así).

        //Try to create the output dir:
        createDirs(outputPath);

        //Try to create the file:
        wxTextFile componentFile;
        wxRemoveFile(name); //##Ver si en el futuro será así: Esto reemplaza el archivo, si éste existía...
        if (!componentFile.Create(name))
            return false;

        //##Descablear, ver si estas cosas se sacan del algún archivo en run-time:
        componentFile.AddLine("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
        componentFile.AddLine("<!-- Created with Minibloq (http://minibloq.org/) -->");
        componentFile.AddLine("<mbqc"); //##Ver si esto queda así, o si pasa a algo tipo "MinibloqComponent"...
        componentFile.AddLine("\txmlns=\"http://mbqc.namespaces.minibloq.org\""); //##
        componentFile.AddLine("\txmlns:minibloq=\"http://minibloq.org\""); //##
        componentFile.AddLine("\tMinibloqVersion=\"0.82.Beta\">"); //##Un-hardcode!

        //##Falta recorrer todos los canvases para agregar los bloqs de usuario...

        //##Recorre los bloques para serializarlos al archivo del mbqc (por ahora sólo del currentCanvas):
        if (!currentCanvas)
            return false;

        componentFile.AddLine(wxString("\t<properties>")); //##Ver si esto queda así, o si pasa a algo tipo "MinibloqComponent"...
        componentFile.AddLine(wxString("\t\t<blocks count=\"") +
                              (wxString("") << currentCanvas->getBlocksCount()) + wxString("\"") +
                              wxString(">")); //##
        componentFile.AddLine(wxString("\t\t</blocks>"));
        componentFile.AddLine(wxString("\t</properties>")); //##Ver si esto queda así, o si pasa a algo tipo "MinibloqComponent"...

        //##Recorre todas las variables (##Esto va a cambiar en el futuro cuando haya más tipos de datos):
        componentFile.AddLine("\t<variables>"); //##Ver si esto queda así, o si pasa a algo tipo "MinibloqComponent"...

        //This is a small patch to prevent the following bug: If the user is editing a variableInit block, and
        //hi has already changed the var name, and saves the program without unselecting the variableInit
        //block, then two instance names are saved: the old one and the new one. And this is wrong. So now,
        //this unselects the currentBlock (because the currentBlock is the only that the user may be editing):
        BubbleBlock *tempBlock = currentCanvas->getCurrentBlock();
        if (tempBlock)
        {
            if (tempBlock->getVariableInit())
                tempBlock->addVariableWithInstanceName();
        }

        //##Ahora que se aseguró de que las instancias estén completas, las recorre para generar las
        //declaraciones correspondientes:
        for (int i = 0; i<currentCanvas->getInstancesCount(); i++)
        {
            //##Esto va a cambiar cuando se introduzca el manejo de tipos definitivo con las
            //variables, y cuando se introduzcan clases:
            BubbleInstance *tempInstance = currentCanvas->getInstance(i);
            if (tempInstance)
            {
                if ( tempInstance->getType() == "Variable" ) //##Horrible harcoded: Un-hardcode!!
                {
                    componentFile.AddLine(wxString("\t\t<") + tempInstance->getName() + wxString(">"));
                    componentFile.AddLine(wxString("\t\t</") + tempInstance->getName() + wxString(">"));
                }
            }
        }
        componentFile.AddLine("\t</variables>"); //##Ver si esto queda así, o si pasa a algo tipo "MinibloqComponent"...

        componentFile.AddLine("\t<blocks>"); //##Ver si esto queda así, o si pasa a algo tipo "MinibloqComponent"...
        BubbleBlock *iteratorBlock = currentCanvas->getFirstBlock();
//        if (getNotifier())
//            getNotifier()->setProgressMax(currentCanvas->getBlocksCount());
        while (iteratorBlock)
        {
//            if (getNotifier())
//                getNotifier()->setProgressPosition(getNotifier()->getProgressPosition() + 1, true, false);

            //Indentation:
            //##En cosas como el código G, que quizá tengan sólo un encabezado tipo "comentario",
            //se puede poner acá 0 ó 1. Aquí van 2, porque todo va indentado 1 con respecto al
            //InitializationCode. Así que se hará configurable la "indentación con respecto al
            //InitializationCode, pero por ahora está cableada acá en "un tab":
            wxString indentation("\t\t");
            for (unsigned int i = 0; i < iteratorBlock->getIndentation(); i++)
                indentation += wxString("\t");

            //##Más adelante se implementará un patrón de iteración, o algo así seguramente...
            //Params:
            if (iteratorBlock->getParamsCount() > 0)
            {
                //##Esto seguramente devolverá un stringArray, no un string, ya que los parámetros van uno abajo del otro:
                wxString tempXML = indentation + generateXMLFromParams(iteratorBlock);
                componentFile.AddLine(tempXML.BeforeLast('\n')); //##Eliminar el último enter que metió generateParamsXML.
            }
            else
            {
                componentFile.AddLine(indentation + wxString("<") +
                                                    iteratorBlock->getName() +
                                                    iteratorBlock->getAttributes() +
                                                    wxString(">"));
                componentFile.AddLine(indentation + wxString("</") + iteratorBlock->getName() + wxString(">"));
            }
            iteratorBlock = currentCanvas->getNextBlock(iteratorBlock);
        }
        componentFile.AddLine("\t</blocks>");
        componentFile.AddLine("</mbqc>");

        //##Save the changes and closes the file:
        if ( !componentFile.Write() )
        {
//            if (getNotifier())
//                getNotifier()->hideMessagesWindow();
            return false;
        }
        if ( componentFile.Close() )
        {
            saved = true; //##
            currentCanvas->forceSaved(true);
//##Futuro: Dejar esto cuando haya un mejor sistema de "mini-mensajes", de modo que los muestre chiquitos, o grandes
//          según sea conveniente para cada acción (por ejemplo, para la compilación puede ser mejor grandes, etc.)...
//            if ( getNotifier() )
//                getNotifier()->showMessage(_("Component:\n") + name + _("\nSaved ok."), true, false, *wxBLUE);

//            if (getNotifier())
//                getNotifier()->hideMessagesWindow();
            if (format)
                return formatComponentFile(name);
            return true;
        }
//        if (getNotifier())
//            getNotifier()->hideMessagesWindow();
        return false;
    }
    catch(...)
    {
//        if (getNotifier())
//            getNotifier()->hideMessagesWindow();
        return false;
    }
}


//##Patch: Esto debería hacerse más rápido y con menos memoria en el actual save, pero la indentación funcinó mal
//y se implementó así por ahora, para que al menos indente:
bool Bubble::formatComponentFile(const wxString& name)
{
    wxXmlDocument componentFile;
    if ( !componentFile.Load(name, wxString("UTF-8")) )
    {
        currentCanvas->showAllBlocks(true);
        return false;
    }
    return componentFile.Save(name, 2); //##Hacer la indentación configurable, al menos levantarla de un
                                        //archivo de texto (XML también, claro)...
}


bool Bubble::run()
{
    //##Futuro: En los boards que tienen deploy y además una orden extra para ejecutar, Run tiene sentido, y
    //tiene la forma actual (en que primero llama a deploy). Pero en el futuro, cuando se agregue la ejecución
    //en tiempo real con intérprete, run será mucho más complejo, casi con seguridad...
    if (deploy())
    {
        //##Implementar...
    }
    return false;
}


#if defined (WIN32)
//##NO PORTABLE CODE: Only for Windows:
//##Not working yet:
bool Bubble::winInstallINF()
{
    if (parent)
    {
        HINSTANCE instance = ShellExecute(
                                            (HWND)parent->GetHandle(),
                                            cstrToWChar("open"),
                                            cstrToWChar("rundll32.exe"),
                                            wxString(wxString("setupapi,InstallHinfSection DefaultInstall 132 ") +
                                            //##:
                                            wxString("C:/Projects/Multiplo/Soft/Minibloq/v1.0/Soft/Bin/Minibloq-RG.v1.0/Components/Drivers/Arduino/UNO/v1.0/Arduino UNO.inf")).c_str(),
                                            //##wxString("C:/Projects/Multiplo/Soft/Minibloq/v1.0/Soft/Bin/Minibloq-RG.v1.0/Components/Drivers/DuinoBot/v1.0/LUFA_CDC_Bootloader.inf")).c_str(),
                                            NULL,
                                            SW_HIDE
                                         );
        return ((int)instance) > 32;
    }
    return false;
}
#endif


void Bubble::createDirs(const wxString& path)
{
    //Try to create the output dir structure:
    wxFileName aux(path);
    if (!wxDir::Exists(getTempPath()))
        wxMkdir(getTempPath());
    if (!wxDir::Exists(aux.GetPath()))
        wxMkdir(aux.GetPath());
    if (!wxDir::Exists(path))
        wxMkdir(path);
}


//##:
//bool Bubble::executeCmd(const wxString& cmd)
//{
//    //##Implementar...
//    BubbleProcess *const process = new BubbleProcess(notifier); //##Ver si esto es correcto, porque me parece más seguro que dejar al notifier
//                                                                //que libere la memoria, tal como sugiere el ejemplo de wxWidgets...
//    //##Ver qué se hace conel resultado de wxExecute:
//    //long pidLast = wxExecute(cmd, wxEXEC_ASYNC, process);
//    //wxExecute(cmd, wxEXEC_ASYNC, process);
//    wxExecute(cmd, wxEXEC_SYNC, process);
//
//    //##Ver si se hace más algo con esto:
//    return true;
//}


bool Bubble::isSubstringInArrayString(const wxArrayString &value, const wxString& substring)
{
    size_t count = value.GetCount();
    for (size_t n = 0; n < count; n++)
    {
        if (value[n].Find(substring) != wxNOT_FOUND )
            return true;
    }

    return false;
}


//##Esta función determina por ahora, si vale la pena seguir o no el proceso de compilación, ya que si
//detecta el string de error, devuelve false, y las funciones que la llaman deberían detener el proceso:
bool Bubble::findErrorStringAndShow(const wxArrayString &value)
{
    //##unhardcode
    //##Hacer que el substring "error:" sea configurable en el target (e incluso más adelante, puedo usar
    //expresiones regulares):
    if (    isSubstringInArrayString(value, wxString("error")) ||
            //isSubstringInArrayString(value, wxString("error:")) ||
            isSubstringInArrayString(value, wxString("Unable")) ||
            isSubstringInArrayString(value, wxString("no such")) ||
            isSubstringInArrayString(value, wxString("No such")) ||
            isSubstringInArrayString(value, wxString("can't")) ||
            isSubstringInArrayString(value, wxString("cannot")) ||
            isSubstringInArrayString(value, wxString("incorrect")) ||
            isSubstringInArrayString(value, wxString("multiple definition")) ||
            isSubstringInArrayString(value, wxString("not found")) ||
            isSubstringInArrayString(value, wxString("No new port")) ||
            isSubstringInArrayString(value, wxString("undefined"))
       )
    {
        showStream(value, *wxRED); //##Hacer los colores configurables
        if (getNotifier())
            getNotifier()->showMessage(_("\nThere are errors."), false, false, *wxRED); //##
        return true;
    }
    showStream(value);
    return false;
}


void Bubble::showStream(const wxArrayString &value, const wxColour& colour)
{
    size_t count = value.GetCount();
    //if ( !count )
    //    return true; //##?

    wxString str("");
    for (size_t n = 0; n < count; n++)
    {
        str += (value[n]) + wxString("\r\n");
        //str += (value[n]) + wxString("\r");
        //str += value[n];
    }

    if (getNotifier())
    {
        getNotifier()->showMessage(str, false, true, colour); //##
    }

    //wxMessageDialog dialog0(parent, str, _("Debug:")); //##Debug.
    //dialog0.ShowModal(); //##Debug.
}


bool Bubble::verifyPortExistance()
{
    if (getHardwareManager() == NULL)
        return false;
    if (getHardwareManager()->getCurrentBoardProperties() == NULL)
        return false;
    if (getNotifier() == NULL)
        return false;

    getNotifier()->showMessage(_("Verifying port ") + bootPortName, false, false, *wxBLUE);

    if (getBootPortName() != wxString("HID")) ////HID or CDC board? ##Future: unhardcode.
    {
        unsigned int times = 0;
        while ( !(BubbleHardwareManager::serialPortExists(bootPortName)) &&
                (times < getHardwareManager()->getCurrentBoardProperties()->getBootFindPortTries())
              )
        {
            times++;
            getNotifier()->showMessage(_(">"), false, false, *wxBLUE);
            wxMilliSleep(100);
        }
        if (BubbleHardwareManager::serialPortExists(bootPortName))
        {
            getNotifier()->showMessage(wxString("\n") + bootPortName + _(" port found.\n"), false, false, *wxGREEN);
            return true;
        }
        getNotifier()->showMessage(wxString("\n") + bootPortName + _(" port not found.\n"), false, false, *wxRED);
        return false;
    }
    return true; //HID port.
}


bool Bubble::deploy()
{
    if (getNotifier() == NULL)
        return false;
    if (getHardwareManager() == NULL)
        return false;
    if (getHardwareManager()->getCurrentBoardProperties() == NULL)
        return false;

    //If the board does not use timeouts in its bootloader, this will compile and build in parallel with the
    //reset process. For exmaple, like the Multiplo DuinoBot board, which uses a run button, so once
    //it's reset, the board will wait until the run button is pressed, or until the software sends a new
    //program and to the flash and then run it from the comm port.
    if (getHardwareManager()->getCurrentBoardProperties()->getResetBeforeBuild())
    {
        //First, reset the progress bar:
        //##getNotifier()->setProgressPosition(0, false, false);
        getNotifier()->clearMessage();

        //verifyPortExistance(); //##Return value not used by now.
        getNotifier()->showMessage(_("\nReseting the board...\n"), false, false, *wxBLUE);
        getNotifier()->deployStartedNotify();
        resetBoard();
    }

    if (build())
    {
        if ( !(getHardwareManager()->getCurrentBoardProperties()->getResetBeforeBuild()) )
        {
            //verifyPortExistance(); //##Return value not used by now.
            getNotifier()->showMessage(_("\nReseting the board...\n"), false, false, *wxBLUE);
            getNotifier()->deployStartedNotify();
            resetBoard();
        }

        //verifyPortExistance(); //##Return value not used by now.
        getNotifier()->showMessage(_("\n"), false, false, *wxBLUE);

        wxArrayString output, errors, commands;
        wxString cmd("");

        //Load the commands from the .board XML file:
        commands.Clear();
        commands = bubbleXML.loadExternalCommands(wxString("deploy"), getHardwareManager()->getCurrentBoardProperties()->getPath() + wxString("/main.board"));
        //cmd = bubbleXML.parseCmd(cmd);

        //Executes the loaded commands:
        int i = 0, count = commands.GetCount();
        //wxMessageDialog dialog0(parent, wxString("") << count, _("commands:"));
        //dialog0.ShowModal();
        while (i < count)
        {
            cmd = commands[i];
            getNotifier()->showMessage(/*(wxString("") << i) + wxString(": ") + */cmd + wxString("\n"), false, true, *wxGREEN);
            wxExecute(cmd, output, errors);

            //Deploy process ends when a command finds an error:
            if (findErrorStringAndShow(errors))
                return false;
            i++;
        }
        showStream(output, *wxWHITE);
        return true;
    }
    return false;
}


bool Bubble::build()
{
    if (getHardwareManager() == NULL)
        return false;
    if (getHardwareManager()->getCurrentBoardProperties() == NULL)
        return false;
    if (getNotifier() == NULL)
        return false;

    loadBoardRelations();
    if (generateCodeAndSaveToFile())
    {
        //Profiling:
        wxLongLong millis = wxGetLocalTimeMillis();

        //First, reset the progress bar:
        //##getNotifier()->setProgressPosition(0, false, false);
        getNotifier()->clearMessage();

        //Loads and executes the commands from the .rel XML files:
        wxArrayString output, errors;
        wxString cmd("");
        unsigned int i = 0;
        while (i < getHardwareManager()->getCurrentBoardProperties()->getRelCommandsCount())
        {
            cmd = getHardwareManager()->getCurrentBoardProperties()->getRelCommand(i);
            getNotifier()->showMessage(/*(wxString("") << i) + wxString(": ") + */cmd + wxString("\n"), false, true, *wxGREEN);
            wxExecute(cmd, output, errors);

            //Build process ends when a command finds an error:
            if (findErrorStringAndShow(errors))
                return false;
            i++;
        }
        showStream(output, *wxWHITE);

        //Loads and executes the commands from the .board XML file:
        cmd = wxString("");
        wxArrayString commands;
        commands.Clear();
        commands = bubbleXML.loadExternalCommands(wxString("build"), getHardwareManager()->getCurrentBoardProperties()->getPath() + wxString("/main.board"));
        //cmd = bubbleXML.parseCmd(cmd);

        //wxMessageDialog dialog0(parent, wxString("") << count, _("commands:"));
        //dialog0.ShowModal();

        //Executes the loaded commands:
        i = 0;
        while (i < commands.GetCount())
        {
            cmd = commands[i];
            getNotifier()->showMessage(/*(wxString("") << i) + wxString(": ") + */cmd + wxString("\n"), false, true, *wxGREEN);
            wxExecute(cmd, output, errors);

            //Build process ends when a command finds an error:
            if (findErrorStringAndShow(errors))
                return false;
            i++;
        }
        showStream(output, *wxWHITE);

        long millisResult = 0;
        millisResult = wxGetLocalTimeMillis().ToLong() - millis.ToLong();
        getNotifier()->showMessage((_("milliseconds: ") + (wxString("") << millisResult)) + wxString("\n\n"), false, false, *wxWHITE); //##Debug
        return true;
    }
    return false;
}


bool Bubble::runInternalCommand(const wxString& cmd)
{
    if (getNotifier() == NULL)
        return false;

    //Currently, runInternalCommand only supports one param, but loadBoardInternalCommands already
    //supports up to 10 params:
    wxString command = cmd.BeforeFirst(';');
    wxString param0 = cmd.AfterFirst(';');
    param0 = param0.BeforeFirst(';'); //Elimintates the last ';'.

    //Shows the command in the message window:
    getNotifier()->showMessage(wxString(">") + command + wxString(";") + param0 + wxString("\n"), false, true, *wxGREEN);

    if (command == wxString("delay"))
    {
        long ms = 0;
        if (param0.ToLong(&ms))
            wxMilliSleep((unsigned int)(ms));
    }
//##Future: finish this (verifyPortExistance() seems to not be working correctly):
//    if (command == wxString("verifyPort"))
//    {
//        return verifyPortExistance();
//        //if (BubbleHardwareManager::serialPortExists(bootPortName))
//        //    getNotifier()->showMessage(bootPortName + wxString(" port found."), false, true, *wxGREEN);
//        //else
//        //    getNotifier()->showMessage(bootPortName + wxString(" port not found."), false, true, *wxRED);
//        //getNotifier()->showMessage(wxString("\n"), false, true, *wxGREEN);
//    }
    else if (command == wxString("setBootSerialLine"))
    {
        if (bootSerialPort.IsOpen())
        {
            if (param0 == wxString("DTR"))
                bootSerialPort.SetLineState(wxSERIAL_LINESTATE_DTR);
            if (param0 == wxString("RTS"))
                bootSerialPort.SetLineState(wxSERIAL_LINESTATE_RTS);
            if (param0 == wxString("DCD"))
                bootSerialPort.SetLineState(wxSERIAL_LINESTATE_DCD);
            if (param0 == wxString("CTS"))
                bootSerialPort.SetLineState(wxSERIAL_LINESTATE_CTS);
            if (param0 == wxString("RING"))
                bootSerialPort.SetLineState(wxSERIAL_LINESTATE_RING);
        }
    }
    else if (command == wxString("clearBootSerialLine"))
    {
        if (bootSerialPort.IsOpen())
        {
            if (param0 == wxString("DTR"))
                bootSerialPort.ClrLineState(wxSERIAL_LINESTATE_DTR);
            if (param0 == wxString("RTS"))
                bootSerialPort.ClrLineState(wxSERIAL_LINESTATE_RTS);
            if (param0 == wxString("DCD"))
                bootSerialPort.ClrLineState(wxSERIAL_LINESTATE_DCD);
            if (param0 == wxString("CTS"))
                bootSerialPort.ClrLineState(wxSERIAL_LINESTATE_CTS);
            if (param0 == wxString("RING"))
                bootSerialPort.ClrLineState(wxSERIAL_LINESTATE_RING);
        }
    }
    else if (command == wxString("openBootSerialPort"))
    {
        bootSerialPort.Open(bootPortName.char_str());
    }
    else if (command == wxString("closeBootSerialPort"))
    {
        bootSerialPort.Close();
    }
    else if (command == wxString("setBootSerialBaudrate"))
    {
        //Very important: It seems that with the wxSerialPort class, the baudrate must be set with the
        //open port. Doing this with the closed port has no effect:
        if (bootSerialPort.IsOpen())
        {
            if (param0 == wxString("150"))
                bootSerialPort.SetBaudRate(wxBAUD_150);
            if (param0 == wxString("300"))
                bootSerialPort.SetBaudRate(wxBAUD_300);
            if (param0 == wxString("600"))
                bootSerialPort.SetBaudRate(wxBAUD_600);
            if (param0 == wxString("1200"))
                bootSerialPort.SetBaudRate(wxBAUD_1200);
            if (param0 == wxString("2400"))
                bootSerialPort.SetBaudRate(wxBAUD_2400);
            if (param0 == wxString("4800"))
                bootSerialPort.SetBaudRate(wxBAUD_4800);
            if (param0 == wxString("9600"))
                bootSerialPort.SetBaudRate(wxBAUD_9600);
            if (param0 == wxString("19200"))
                bootSerialPort.SetBaudRate(wxBAUD_19200);
            if (param0 == wxString("38400"))
                bootSerialPort.SetBaudRate(wxBAUD_38400);
            if (param0 == wxString("57600"))
                bootSerialPort.SetBaudRate(wxBAUD_57600);
            if (param0 == wxString("115200"))
                bootSerialPort.SetBaudRate(wxBAUD_115200);
            if (param0 == wxString("230400"))
                bootSerialPort.SetBaudRate(wxBAUD_230400);
            if (param0 == wxString("460800"))
                bootSerialPort.SetBaudRate(wxBAUD_460800);
            if (param0 == wxString("921600"))
                bootSerialPort.SetBaudRate(wxBAUD_921600);
        }
    }
    else if (command == wxString("stringWriteToBootSerial"))
    {
        if (bootSerialPort.IsOpen())
        {
            bootSerialPort.Write(param0.char_str(), param0.Len());
        }
    }
    else if (command == wxString("containsStringReadFromBootSerial"))
    {
        if (bootSerialPort.IsOpen())
        {
            //This command can stop the reading of internal commands in this XML section (by now, it's
            //all that it can do) by returning "false":
            char buffer[256]; //Max size of param0 for this command = 255.
            unsigned int readCount = bootSerialPort.Read(buffer, sizeof(buffer)-1);
            if ( (readCount < sizeof(buffer)) && (readCount > 0) )
            {
                getNotifier()->showMessage(wxString(buffer) + wxString("\n"), false, false, *wxBLUE);
                if (bootSerialPort.IsOpen())
                    bootSerialPort.Close();
                return wxString(buffer).Contains(param0);
            }
        }
        return false;
    }
    else if (command == wxString("debug"))
    {
        getNotifier()->showMessage(param0 + wxString("\n"), false, false, *wxGREEN);
    }
//##Future: finish this:
//    else if (command == wxString("findNewPort"))
//    {
//        if (getHardwareManager())
//        {
//            std::vector<std::string> ports;
//            getHardwareManager()->getPorts(ports);
//            getNotifier()->showMessage(_("Current ports: \n"), false, false, *wxWHITE);
//            for (unsigned int i = 0; i < ports.size(); i++)
//                getNotifier()->showMessage(wxString(ports[i]) + wxString("\n"), false, false, *wxWHITE);
//            if (getHardwareManager()->findNewPort())
//            {
//                getNotifier()->showMessage(_("New port: "), false, false, *wxWHITE);
//                getNotifier()->showMessage(getHardwareManager()->getNewPort() + wxString("\n"), false, false, *wxWHITE);
//            }
//        }
//    }

    return true;
}


bool Bubble::resetBoard()
{
    if (getHardwareManager() == NULL)
        return false;
    if (getHardwareManager()->getCurrentBoardProperties() == NULL)
        return false;

    wxString boardFileName = getHardwareManager()->getCurrentBoardProperties()->getPath() + wxString("/main.board");
    bool mustReset =    bubbleXML.sectionExists(boardFileName, wxString("resetInternal")) ||
                        bubbleXML.sectionExists(boardFileName, wxString("resetExternal"));

    if (!mustReset)
        return false;

    if (getNotifier() == NULL)
        return false;

    if (!bootSerialPort.IsOpen())
		bootSerialPort.Open(bootPortName.char_str());
    if (bootSerialPort.IsOpen()) //This is NOT the same as en "else"!
    {
        wxArrayString output, errors, commands;
        wxString cmd("");

        //Executes the internal commands, if any:
        commands.Clear();
        commands = bubbleXML.loadInternalCommands(wxString("resetInternal"), boardFileName);
        int i = 0, count = commands.GetCount();
        while (i < count)
        {
            cmd = commands[i];
            if (!runInternalCommand(cmd))
                break;
            i++;
        }

        //Executes the external commands, if any:
        commands.Clear();
        commands = bubbleXML.loadExternalCommands(wxString("resetExternal"), boardFileName);
        i = 0, count = commands.GetCount();
        while (i < count)
        {
            cmd = commands[i];
            getNotifier()->showMessage(/*(wxString("") << i) + wxString(": ") + */cmd + wxString("\n"), false, true, *wxGREEN);
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
            {
                bootSerialPort.Close();
                return false;
            }
            i++;
        }

        showStream(output, *wxWHITE);
        bootSerialPort.Close();
        return true;
    }
	return false;
}


//##Delete this!
#if 0
bool Bubble::verifyBoard()
{
    //##Esto debe ser diferente para cada placa:
    if (getBoardName() == wxString("DuinoBot.v1.x") ||
        getBoardName() == wxString("DuinoBot.Kids.v1.x") )
    {
        try
        {
            if (!bootSerialPort.IsOpen())
                bootSerialPort.Open(bootPortName.char_str());
            if (bootSerialPort.IsOpen()) //This is NOT the same as en "else"!
            {
                char c = 'S'; //##Pide el string "LUFACDC" que es el identificador del bootloader...
                bootSerialPort.Write(&c, 1);

                char buffer[12]; //##Hacer el tamaño configurable.
                unsigned int readCount = bootSerialPort.Read(buffer, sizeof(buffer)-1);
                if ( (readCount < sizeof(buffer)) && (readCount > 0) )
                {
                    if (getNotifier())
                        getNotifier()->showMessage(wxString(buffer) + wxString("\n"), false, false, *wxGREEN);
                    if (bootSerialPort.IsOpen())
                        bootSerialPort.Close();
                    return wxString(buffer).Contains(wxString("LUFACDC"));
                }
            }
            if (bootSerialPort.IsOpen())
                bootSerialPort.Close();
            return false;
        }
        catch(...)
        {
            if (bootSerialPort.IsOpen())
                bootSerialPort.Close();
            return false;
        }
    }
    else
    {
        return true;
    }
}
#endif


void Bubble::linesFromArrayToBubbleEditor(const wxArrayString &strings, BubbleEditor *editor)
{
    if (editor)
    {
        for (unsigned int i = 0; i < strings.GetCount(); i++)
        {
            editor->AddText(strings[i] + wxString("\n")); //##Testing needed: Is this correct in all platforms?
        }
    }
}


void Bubble::linesFromArrayToFile(const wxArrayString &strings, wxTextFile *file)
{
    if (file)
    {
        for (unsigned int i = 0; i < strings.GetCount(); i++)
        {
            file->AddLine(strings[i]);
        }
    }
}


wxArrayString Bubble::string2Array(const wxString &value)
{
    wxArrayString result;
    for (unsigned int i=0; i<value.length(); i++)
    {
        result.Add(value[i]);
    }
    return result;
}


unsigned int Bubble::strOcurrencesInArray(const wxString &str, const wxSortedArrayString &array)
{
//##wxWidgets bug in wxSortedArray.Index()?: In theory, the Index funcion must return the index of the FIRST
//ocurrence of the string in the array. But according to these tests, it does not. If Index
//    unsigned int result = 0;
//    int index = array.Index(str);
//    if (index == wxNOT_FOUND)
//        return 0;
//
//    while ( (unsigned)index < array.GetCount() )
//    {
//        result++;
//        if (str != array[index])
//            break;
//        index++;
//    }
//    return result;

    //##Ugly (linear search, not binary), but works:
    unsigned int result = 0;
    int index = 0;
    while ( (unsigned)index < array.GetCount() )
    {
        if (str == array[index])
            result++;
        index++;
    }
    return result;
}


void Bubble::addLibrariesToCode()
{
    generatedCode.Add(getIncludesCodeList());
    generatedCode.Add("");
}


void Bubble::addInitCode()
{
    if (getHardwareManager() == NULL)
        return;
    if (getHardwareManager()->getCurrentBoardProperties() == NULL)
        return;

    generatedCode.Add(getHardwareManager()->getCurrentBoardProperties()->getInitCode());
}


void Bubble::addFinalCode()
{
    if (getHardwareManager() == NULL)
        return;
    if (getHardwareManager()->getCurrentBoardProperties() == NULL)
        return;

    generatedCode.Add(getHardwareManager()->getCurrentBoardProperties()->getFinalCode());
}


wxString Bubble::generateParamsCode(BubbleBlock *block)
{
    wxString tempCode("");
    if (block == NULL)
        return tempCode;
    if (block->getCode().IsEmpty())
        return tempCode;

    //##Antes se soportaban números negativos, porque es más rápido escribir código así, pero es redundante
    //(ya que se agregó el bloque numberNegative).
    //Y por lo tanto podría confundir, con respecto al bloque numberNegative, el cual es imprescindible, para
    //poder "negativizar" variables y expresiones completas. Así que puede que tenga que tocar este código.
    //Pero se lo deja (al menos por ahora) porque podría ocurrir que en alguna implementación se soporten
    //constantes negativas directamente, sin que se desee que se armen expresiones con numberNegative, o
    //incluso que se soporten ambas formas (con numberNegative y con constantes numéricas negativas):

    //If the block is a number, and it's negative, add the ParamListStart and the ParamListEnd strings:
    if (block->getDataType() == wxString("number")) //##Future: Now there are run-time loadable types, this must be rewritten...
    {
        double x;
        if (block->getCode()[0].ToDouble(&x))
        {
            if (x < 0)
                //tempCode += block->getParamListStart() + block->getCode()[0] + block->getParamListEnd() +block->getParamListStart();
                tempCode += wxString("(") + block->getCode()[0] + wxString(")") + block->getParamListStart(); //##Descablear el símbolo de "asilación de signo" (en este caso los paréntesis).
            else
                tempCode += block->getCode()[0] + block->getParamListStart();
        }
        else
        {
            //Not necessary, but safe:
            tempCode += block->getCode()[0] + block->getParamListStart();
        }
    }
    else
    {
        tempCode += block->getCode()[0] + block->getParamListStart();
    }

    unsigned int paramsCount = block->getParamsCount();
    for (unsigned int paramIndex = 0; paramIndex < paramsCount; paramIndex++)
    {
        BubbleBlock *paramBlock = block->getParamFirstBlock(paramIndex);
        if (paramBlock)
        {
            if (paramIndex == (paramsCount - 1)) //Is the last param?
                tempCode += generateParamsCode(paramBlock) + block->getParamListEnd();
            else
                tempCode += generateParamsCode(paramBlock) + block->getParamSeparator();
        }
    }

    return tempCode;
}


bool Bubble::simplifyCodeLine(wxString &code)
{
    //##Implementar:
    return true;
}


int Bubble::getCodeFirstModifiedLineNumber() const
{
    for (unsigned int i = 0; i < generatedCode.GetCount(); i++)
    {
        if (i < prevGeneratedCode.GetCount() )
        {
            if (generatedCode[i] != prevGeneratedCode[i])
                return (int)i;
        }
        else
        {
            //If had reached the end of the second array, returns wxNOT_FOUND if the arrays has the same
            //quantity of elements (here this means that the arrays are equal, because it already compares
            //each item):
            if (generatedCode.GetCount() == prevGeneratedCode.GetCount())
                return wxNOT_FOUND;
            return (int)i;
        }
    }
    return wxNOT_FOUND;
}


int Bubble::getCodeLastModifiedLineNumber() const
{
    int j = prevGeneratedCode.GetCount() - 1;
    for (unsigned int i = generatedCode.GetCount() - 1; i >= 0; i--)
    {
        if (j >= 0 )
        {
            if (generatedCode[i] != prevGeneratedCode[j])
                return (int)i;
        }
        else
        {
            //If had reached the begin of the second array, returns wxNOT_FOUND if the arrays has the same
            //quantity of elements (here this means that the arrays are equal, because it already compares
            //each item):
            if (generatedCode.GetCount() == prevGeneratedCode.GetCount())
                return wxNOT_FOUND;
            return (int)i;
        }
        j--;
    }
    return wxNOT_FOUND;
}


bool Bubble::updateCode()
{
    //##In the future, this must be extended to support multiple canvases, but will have to be optimzed to
    //only regenerate those canvases wich has changed since the last generation of code:
    try
    {
        //##Ver si esto se hará así, o si se implementará en una sóla pasada, ya que si se hace de esta forma,
        //el programa verifica los errores pasando por todos los bloques, y luego los recorre nuevamente para
        //generar el código. Casi seguro lo haré en una sóla pasada, pero puede que no para la primer versión...
        //##Update del comentario anterior: Lo más probable es que no utilice esta función porque la verificación
        //de errores se hace en tiempo real. Si esto es así, ##Volar esto:
        //if (!verify())
        //    return false;

        //Generates the code:
        prevGeneratedCode = generatedCode;
        generatedCode.Clear();

        //addHeaderCode();
        //addLibrariesToCode();
        addInitCode();

        //##Falta recorrer todos los canvases para agregar los Blocks de usuario...

        //##Recorre los bloques para generar el código (por ahora sólo del currentCanvas):
        if (!currentCanvas)
            return false;
        if (getHardwareManager() == NULL)
            return false;
        if (getHardwareManager()->getCurrentBoardProperties() == NULL)
            return false;

        //##En ocasiones se me colgó esto, tengo que ver si es cuando el avrdude tiene algún problema, o si persite
        //algún bug en este while: Pareciera que es el avrdude, pero pasó algo muy raro:
        //
        //Generé un programa con un if y adentro sonidos y delays, lo bajé y anduvo bien. Traté deliveradamente
        //de bajarlo con el micro sin resetear (o sea corriendo el mismo programita recién bajado), y el software quedó a la espera,
        //pero lo más raro de todo es que los botones de los métodos ¡pasaron a disabled! Muy pero muy extraño, pareciera ser de memoria,
        //pero puede que corriendo en async, etc. o utilizando la API del SO se pueda hacer algo, porque podría ser un temita con el
        //wxExecute.

        //Igual luego cuando lo corrí, si reseteaba el micro mientras estaba "colgado", resultaba en que el soft "volvía del avrdude".
        //así que puedo poner un timer, o un thread o algo (ver el XDFTerminal), para que si la cosa pasa de cierto time-out, avise al
        //usuario de pasa algo.
        //Además se pueden hacer chequeos extra del puerto hasta que todo sea realmente cómodo para el usuario.

        BubbleBlock *iteratorBlock = currentCanvas->getFirstBlock();
        while (iteratorBlock)
        {
            //##Test "comments" stuff this a lot!
            wxString strCommentBegin("");
            wxString strCommentEnd("");
            if (iteratorBlock->isCommented())
            {
                strCommentBegin = getHardwareManager()->getCurrentBoardProperties()->getCommentBegin();
                strCommentEnd = getHardwareManager()->getCurrentBoardProperties()->getCommentEnd();
            }

            //##En cosas como el código G, que quizá tengan sólo un encabezado tipo "comentario",
            //se puede poner acá 0 ó 1. Aquí van 2, porque todo va indentado 1 con respecto al
            //InitializationCode. Así que se hará configurable la "indentación con respecto al
            //InitializationCode, pero por ahora está cableada acá en "un tab":
            wxString indentation("\t");

            //Indentation:
            for (unsigned int i = 0; i < iteratorBlock->getIndentation(); i++)
                indentation += wxString("\t");

            wxArrayString blockCode = iteratorBlock->getCode();

            //##2011.02.01: Acá falta recorrer toda la lista de parámetros, tal cual se hace para el borrado, etc.
            //##2011.02.01: Quizá los operadores no necesiten ser tratados de forma diferente:
            //  - Quizá con cargar vacíos los Start y End, y colocando al operador en el paramSeparator la cosa esté.
            //  - Falta ver qué hacer cuando aparecen operadores unarios (como en los números negativos, por ejemplo).

            //##Más adelante se implementará un patrón de iteración, o algo así seguramente...

            if (iteratorBlock->getParamsCount() > 0)
            {
                //Params:

                //generateParamsCode returns not only de params themselves, but the getCode[0] too, that's why
                //the indentation string is added first:
                wxString tempCode = indentation + strCommentBegin + generateParamsCode(iteratorBlock);
                generatedCode.Add(tempCode + strCommentEnd);

                //Params follow the first blockCode line (blockCode[0]):
                for (unsigned int i = 1; i < blockCode.GetCount(); i++)
                {
                    if (getSimplifyCode())
                        simplifyCodeLine(blockCode[i]);
                    generatedCode.Add(indentation + strCommentBegin + blockCode[i] + strCommentEnd);
                }
            }
            else
            {
                for (unsigned int i = 0; i < blockCode.GetCount(); i++)
                {
                    if (getSimplifyCode())
                        simplifyCodeLine(blockCode[i]);
                    generatedCode.Add(indentation + strCommentBegin + blockCode[i] + strCommentEnd);
                }
            }

            //##Debug:
            //wxMessageDialog dialog0(parent, iteratorBlock->getCode(), _("generateCode()"));
            //dialog0.ShowModal();

            iteratorBlock = currentCanvas->getNextBlock(iteratorBlock);
        }

        addFinalCode();
        return true;
    }
    catch(...)
    {
        return false;
    }
}


bool Bubble::generateCodeAndSaveToFile()
{
    try
    {
        if (getHardwareManager() == NULL)
            return false;
        if (getHardwareManager()->getCurrentBoardProperties() == NULL)
            return false;
        //##Nota acerca del tipo de archivo: Por ahora usa el typeDefault, que en teoría debería generar archivos
        //con terminación de línea "DOS". Pero más adelante ser verá, y quizá se pase todo a Unix. De todos modos
        //la edición con scintilla en el mismo entorno debería funcionar bien igual. Si esto se cambia, ver
        //qué funciones hay que tocar (en principio, al menos hay que llamar a Write() con otro tipo de
        //archivo, porque ahora usan el parámetro con valor por defecto). Algo que sí me gustaría es no usar
        //el default, para que todos los Minibloq generen el mismo tipo de archivo, sin importar el
        //sistema operativo, lo cual dará más uniformidad, pero no estoy seguro...

        //##Luego avisar de errores, etc.:
        clean();

        //Try to create the output dir structure:
        createDirs(outputPath);

        //In Arduino-compatible systems, this file is used to pass a file with valid extension to the compiler, instead of, for
        //example a .ino file. In the future it's possible that this will become configurable in the backend, specially to support
        //other languajes different than C/C++:
        wxTextFile wrapperOutput;
        if ( !wrapperOutput.Create( getOutputPath() + wxString("/") + getComponentFilesPath().AfterLast('/') + wxString(".") +
                                    getHardwareManager()->getCurrentBoardProperties()->getCodeFileExtension() )
           )
            return false;
        wrapperOutput.AddLine(  getHardwareManager()->getCurrentBoardProperties()->getIncludeCodePrefix() +
                                getComponentFilesPath().AfterLast('/') + wxString(".") +
                                getHardwareManager()->getCurrentBoardProperties()->getOutputMainFileExtension() +
                                getHardwareManager()->getCurrentBoardProperties()->getIncludeCodePostfix()
                             );
        if ( !wrapperOutput.Write() )
            return false;
        if ( !wrapperOutput.Close() )
            return false;

        //This is the global header, to be included by other code files that need to access the board's instances and constants:
        wxTextFile mbqGlogalsHeader;
        wxString mbqGlogalsHeaderName = getComponentFilesPath() + wxString("/mbq.") +
                                        getHardwareManager()->getCurrentBoardProperties()->getHeaderFileExtension();
        wxRemoveFile(mbqGlogalsHeaderName);
        if ( !mbqGlogalsHeader.Create(mbqGlogalsHeaderName) )
            return false;
        mbqGlogalsHeader.AddLine(getHardwareManager()->getCurrentBoardProperties()->getIncludeInitCode());
        mbqGlogalsHeader.AddLine(getIncludesCodeList());
        mbqGlogalsHeader.AddLine(getHardwareManager()->getCurrentBoardProperties()->getDefinesCodeList());
        mbqGlogalsHeader.AddLine(getHardwareManager()->getCurrentBoardProperties()->getInstancesHeaderCodeList());
        mbqGlogalsHeader.AddLine(getHardwareManager()->getCurrentBoardProperties()->getIncludeFinalCode());
        if ( !mbqGlogalsHeader.Write() )
            return false;
        if ( !mbqGlogalsHeader.Close() )
            return false;

        //Creates the initBoard file:
        wxTextFile initBoardFile;
        wxString initBoardFileName =    getComponentFilesPath() + wxString("/initBoard.") +
                                        getHardwareManager()->getCurrentBoardProperties()->getCodeFileExtension();
        wxRemoveFile(initBoardFileName);
        if ( !initBoardFile.Create(initBoardFileName) )
            return false;
        initBoardFile.AddLine(getHardwareManager()->getCurrentBoardProperties()->getInitBoardHeader());
        initBoardFile.AddLine(wxString(""));
        initBoardFile.AddLine(getHardwareManager()->getCurrentBoardProperties()->getInstancesCodeList());
        initBoardFile.AddLine(wxString(""));
        initBoardFile.AddLine(getHardwareManager()->getCurrentBoardProperties()->getInitBoardPrefix());
        initBoardFile.AddLine(getInitBoardCode() +
                              getHardwareManager()->getCurrentBoardProperties()->getInitBoardPostfix()
                             );
        if ( !initBoardFile.Write() )
            return false;
        if ( !initBoardFile.Close() )
            return false;

        //Try to create the main file:
        wxTextFile mainOutput;
        wxString mainOutputName = getComponentFilesPath() + wxString("/") + getComponentFilesPath().AfterLast('/') + wxString(".ino");
        wxRemoveFile(mainOutputName);
        if ( !mainOutput.Create(mainOutputName) )
            return false;

        //Refresh the generated code, and obtains it:
        if (!updateCode())
            return false;
        linesFromArrayToFile(getGeneratedCode(), &mainOutput);

        //##Save the changes and closes the file:
        //##Si hay errores con el archivo, reportarlo en Messages:
        if ( !mainOutput.Write() )
            return false;
        if ( mainOutput.Close() )
        {
            if ( getNotifier() )
                getNotifier()->refreshGeneratedCodeNotify();

            return true;
        }
        return false;
    }
    catch(...)
    {
        return false;
    }
}


bool Bubble::clean()
{
    //##Does not work:
    //return wxRmDir(outputPath);

    //##Perhaps in the future it can be specified which file to delete (or to ignore) in the relation files
    //between boards and langauges:

    wxDir dir(outputPath);
    if ( !dir.IsOpened() )
        return false;

    wxString fileName;
    bool result = dir.GetFirst(&fileName, wxEmptyString, wxDIR_DEFAULT);
    while (result)
    {
        wxRemoveFile(outputPath + wxString("/") + fileName);
        result = dir.GetNext(&fileName);
    }
    return true; //##Not used by now.
}


//##Casi seguro esto se va:
//bool Bubble::verify()
//{
//    //##Esta función debe llamar a todas las funciones verifyXXX, y si alguna falla, retorna falso. ##Ver qué
//    //otras funciones verifyXXX hay que implementar...
//    if (!verifyMissingParams())
//        return false;
//
//    //##Implementar: Aquí se hará el control de errores (por ejemplo, si falta un parámetro en un bloque, etc.):
//    return true;
//}


bool Bubble::verifyMissingParams()
{
    //##Implementar: Recorre los bloques y busca si faltan parámetros, por completar por parte del usuario.
    //Cada vez que encuentra uno, agrega el error a la ventana de mensajes, o a donde corresponda. La idea
    //es que en una rápida pasada, pueda encontrar todos los errores de falta de parámetros.
    return true;
}


bool Bubble::makePortable()
{
#if (!UNDER_DEVELOPMENT)
    //##Implementar...
    return true;
#endif
}


void Bubble::loadAcceleratorTableFromFile()
{
    //##Implementar: Por ahora está cableado, pero hay que levantar todo esto de un archivo:
    //##Recorrer todos los canvas en el futuro:
    if (getCurrentCanvas())
    {
        //##Esto va a cambiar, ahora está cableado, pero va a pasar casi seguro a una función llamada
        //"addAcceleratorKey" olgo así, miembro de BubbleCanvas:
        getCurrentCanvas()->addAcceleratorKeys();
    }
}


BubbleActionPicker *Bubble::createActionPicker(wxWindow *pickerParent, bool show)
{
    if (pickerParent == NULL)
        return NULL;

    wxPoint prevPosition(0, 0);
    if (actionPicker)
    {
        //actionPicker->Close(true);
        prevPosition = actionPicker->GetPosition();
        actionPicker->Destroy();
        actionPicker = NULL;
        //return NULL;//##Debug.
//        wxMessageDialog dialog0(parent, _("Closing action picker"), _("0"));
//        dialog0.ShowModal();
    }

    actionPicker = new BubbleActionPicker(  pickerParent,
                                            this,
                                            wxNewId(),
                                            getActionDataType(),
                                            _("Actions"),
                                            64,
                                            //wxColour(64, 64, 64), //##Ver tema color.
                                            wxColour(0, 0, 0), //##Ver tema color.
                                            2 //3
                                          );
    if (actionPicker == NULL)
        return NULL;

    actionPicker->Move(prevPosition);
    return actionPicker; //##No sé si es prolijo retornarlo, ya que es una variable interna de la instancia...
}


void Bubble::showPicker(BubblePicker* picker, const wxPoint& position, bool value)//##, bool &pickerFirstTime)
{
    if (picker)
    {
        if (value)
        {
            /* ##La idea es ver si entra en la pantalla o no, si no es así, entonces lo corre al borde, como
            ocurre automáticamente al desplegar un menú, pero por ahora se prefirió centrar al picker. En el
            futuro se verá qué se hace...
            if ((position.x + picker->GetSize().GetWidth()) > )
            {
                picker->Move(position);
            }
            else
            {
            }
            */

            //##Esto tiene que cambiar, no me gusta enviar este parámetro así, para cada picker, es desprolijo:

            //##if (pickerFirstTime)
            //{
            //    pickerFirstTime = false;
            //picker->Centre();  //##En una versión futura se mejorará el posicionamiento inicial para que
                                        //resulte más cómodo, y se agregará una "chinche" a la ventana, para que
                                        //se quede donde el usuario la dejó, o para que se autoposicione cada vez.
            //}
            //##Es casi seguro que todo esto se irá cuando se pasen los pickers como paneles para que los
            //administre wxAUI:
/*
            if (parent)
            {
                //##Luego esto sale de algún archivo de configuración, donde recuerda la última posición:
//                picker->Move(parent->GetClientSize().GetWidth() - picker->GetSize().GetWidth(),
//                            //picker->GetSize().GetWidth(),
//                            parent->GetPosition().y - picker->GetSize().GetWidth());
                picker->Move(parent->GetPosition().x + parent->GetClientSize().GetWidth() - picker->GetSize().GetWidth(),
                             parent->GetPosition().y //+ (parent->GetSize().GetHeight() - parent->GetClientSize().GetHeight())
                            );
            }
            else
                picker->Centre(); //##Esto puede que se vaya también.
*/
            picker->Move(position);
            picker->Show();

            //Prevents bad painting (with background elements on the panel's surface):
            picker->Update();
            picker->Refresh();

            picker->SetFocus();//##
            //actionPicker->SetFocusIgnoringChildren();
        }
        else
            picker->Show(false);
    }
}


void Bubble::togglePicker(BubblePicker* picker, const wxPoint& position)
{
    if (picker)
        showPicker(picker, position, !picker->IsShown());
}


void Bubble::enableAllBlocks(bool value)
{
    //##Future: travel all canvases:
    if (getCurrentCanvas())
    {
        getCurrentCanvas()->enableAllBlocks(value);
        getCurrentCanvas()->Update();
        getCurrentCanvas()->Refresh();
        blocksEnabled = value;
    }
}


void Bubble::setVisibleLabels(bool value)
{
    //##Implementar:
    //Recorre todos los canvases y les setea los labels en true...
    if (getCurrentCanvas())
    {
        getCurrentCanvas()->setVisibleLabels(value);
        visibleLabels = getCurrentCanvas()->getVisibleLabels();
    }
}
