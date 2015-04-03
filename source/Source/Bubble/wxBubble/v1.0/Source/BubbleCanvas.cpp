#include "BubbleCanvas.h"
#include "Bubble.h"

#include <wx/dcclient.h>
//## #include <wx/print.h>
#include <wx/dcprint.h>
#include <BubblePrintout.h>
#include <wx/msgdlg.h> //##Debug, se va en release. Agregar compilación condicional.

//##:
//const wxWindowID BubbleCanvas::ID_ButtonAddBlock = wxNewId();


//////////////////////////////////////////////////////////////
//
// class BubbleCanvasTicker
//
//////////////////////////////////////////////////////////////
void BubbleCanvasTicker::Notify()
{
    if (canvas)
    {
        canvas->autoPanTick(); //##Descablear este valor
    }
};


//////////////////////////////////////////////////////////////
//
// class BubbleCanvas
//
//////////////////////////////////////////////////////////////
BEGIN_EVENT_TABLE(BubbleCanvas, BubblePanel)
    EVT_SIZE(BubbleCanvas::onSize)
    EVT_SCROLLWIN_THUMBRELEASE(BubbleCanvas::onScrollBarThumbRelease)

    EVT_ENTER_WINDOW(BubbleCanvas::onMouseEnter)
    EVT_LEAVE_WINDOW(BubbleCanvas::onMouseLeave)

    EVT_MOTION(BubbleCanvas::onMouseMove)
    EVT_MIDDLE_UP(BubbleCanvas::onMiddleUp)
    EVT_MIDDLE_DOWN(BubbleCanvas::onMiddleDown)

    //##EVT_SET_FOCUS(BubbleCanvas::onSetFocus)
    EVT_KILL_FOCUS(BubbleCanvas::onKillFocus)

    EVT_MOUSE_EVENTS(BubbleCanvas::OnMouseEvent)
    EVT_LEFT_UP(BubbleCanvas::onLeftUp)
    //EVT_LEFT_DOWN(BubbleCanvas::onLeftDown)
    //##EVT_LEFT_DCLICK(BubbleCanvas::onLeftDown)
    EVT_RIGHT_DOWN(BubbleCanvas::onRightDown)
    EVT_RIGHT_DCLICK(BubbleCanvas::onRightDown)

    //##I'm not sure those edit accelerators will remain here, have still to test a lot (it's possible that
    //the will be here anyway, just not sure):
    EVT_MENU(ID_Accel_Undo, BubbleCanvas::onUndo)
    EVT_MENU(ID_Accel_Redo, BubbleCanvas::onRedo)
    EVT_MENU(ID_Accel_SelectAll, BubbleCanvas::onSelectAll)
    EVT_MENU(ID_Accel_Copy, BubbleCanvas::onCopyBlock)
    EVT_MENU(ID_Accel_Cut, BubbleCanvas::onCutBlock)
    EVT_MENU(ID_Accel_Paste, BubbleCanvas::onPasteBlock)
    EVT_MENU(ID_Accel_Delete, BubbleCanvas::onDeleteBlock)
    EVT_MENU(ID_Accel_CommentUncomment, BubbleCanvas::onCommentUncommentBlock)

    EVT_MENU(ID_Accel_RestoreZoomTo100, BubbleCanvas::onRestoreZoomTo100)
    //EVT_MENU(ID_Accel_RestoreZoomTo75, BubbleCanvas::onRestoreZoomTo75)
    EVT_MENU(ID_Accel_DecZoom, BubbleCanvas::onDecZoom)
    EVT_MENU(ID_Accel_IncZoom, BubbleCanvas::onIncZoom)

    EVT_MENU(ID_Accel_ScrollFirstBackBlock, BubbleCanvas::onScrollFirstBackBlock)
    EVT_MENU(ID_Accel_ScrollHome, BubbleCanvas::onScrollHome)
    EVT_MENU(ID_Accel_ScrollHomeAbsolute, BubbleCanvas::onScrollHomeAbsolute)
    EVT_MENU(ID_Accel_ScrollEnd, BubbleCanvas::onScrollEnd)
    EVT_MENU(ID_Accel_ScrollLastParamBlock, BubbleCanvas::onScrollLastParamBlock)
    EVT_MENU(ID_Accel_DecHScroll, BubbleCanvas::onDecHScroll)
    EVT_MENU(ID_Accel_IncHScroll, BubbleCanvas::onIncHScroll)
    EVT_MENU(ID_Accel_DecVScroll, BubbleCanvas::onDecVScroll)
    EVT_MENU(ID_Accel_IncVScroll, BubbleCanvas::onIncVScroll)

    EVT_MENU(ID_Accel_MoveLeft, BubbleCanvas::onMoveLeft)
    EVT_MENU(ID_Accel_MoveRight, BubbleCanvas::onMoveRight)
    EVT_MENU(ID_Accel_MoveUp, BubbleCanvas::onMoveUp)
    EVT_MENU(ID_Accel_MoveDown, BubbleCanvas::onMoveDown)

    //EVT_MENU(ID_MenuEditAddComment, BubbleCanvas::onMenuEditAddComment)
    EVT_MENU(ID_MenuEditDeleteActions, BubbleCanvas::onDeleteBlock)
    EVT_MENU(ID_MenuEditCopyActions, BubbleCanvas::onCopyBlock)
    EVT_MENU(ID_MenuEditCutActions, BubbleCanvas::onCutBlock)
    EVT_MENU(ID_MenuEditPasteActions, BubbleCanvas::onPasteBlock)
    EVT_MENU(ID_MenuEditCommentUncommentActions, BubbleCanvas::onCommentUncommentBlock)

    EVT_MENU(ID_MenuEditPasteStartBlocks, BubbleCanvas::onPasteBlock)
    EVT_MENU(ID_MenuEditDeleteExpressions, BubbleCanvas::onDeleteBlock)
END_EVENT_TABLE()

/*##Otros eventos que puedo utilizar aquí (ver wx/event.h):
wxEVT_LEFT_DOWN
wxEVT_LEFT_UP
wxEVT_MIDDLE_DOWN
wxEVT_MIDDLE_UP
wxEVT_RIGHT_DOWN
wxEVT_RIGHT_UP
wxEVT_MOTION
wxEVT_ENTER_WINDOW
wxEVT_LEAVE_WINDOW
wxEVT_LEFT_DCLICK
wxEVT_MIDDLE_DCLICK
wxEVT_RIGHT_DCLICK
wxEVT_SET_FOCUS
wxEVT_KILL_FOCUS
wxEVT_CHILD_FOCUS
wxEVT_MOUSEWHEEL
*/

//##Block *lastBlock = NULL;

BubbleCanvas::BubbleCanvas( wxWindow* parent,
                            Bubble *const bubble,
                            wxWindowID id,
                            int defaultHScrollInc,
                            int defaultVScrollInc,
                            //const BubbleCanvasInfo &info):  BubblePanel(parent, id, info.getBackgroundColour()),
                            const BubbleCanvasInfo &info):  BubblePanel(parent, id,
                                                                        info.getBackgroundColour(), true
                                                                        //wxDefaultPosition, wxDefaultSize,
                                                                        //wxTAB_TRAVERSAL | wxSB_VERTICAL //wxVSCROLL
                                                                       ),
                                                            parent(parent),
                                                            bubble(bubble),
                                                            autoPanTimer(this, this),
                                                            autoPanTimerInterval(-1), //milliseconds
                                                            autoPanSensibleWidth(60), //##Not used yet => not tested yet.
                                                            autoPanSensibleHeight(60),
                                                            autoPanHStep(10),
                                                            autoPanVStep(10),

                                                            popEditForActions(NULL),
                                                            popEditForStartBlocks(NULL),
                                                            popEditForExpressions(NULL),

                                                            menuEditDeleteActions(NULL),
                                                            menuEditCopyActions(NULL),
                                                            menuEditCutActions(NULL),
                                                            menuEditPasteActions(NULL),
                                                            menuEditCommentUncommentActions(NULL),

                                                            menuEditPasteStartBlocks(NULL),
                                                            menuEditDeleteExpressions(NULL),

                                                            focused(true),
                                                            saved(true),

                                                            _main(info.isMain()),

                                                            lockBlockKeyboardEvents(false),

                                                            tabWidth(info.getTabWidth()),
                                                            initialBlockX(info.getInitialBlockX()),
                                                            initialBlockY(info.getInitialBlockY()),

                                                            defaultHScrollInc(defaultHScrollInc),
                                                            defaultVScrollInc(defaultVScrollInc),

                                                            currentBlock(NULL),
                                                            draggedBlock(NULL),

                                                            disableWhileDragging(true),
                                                            //nonDraggableImage(NULL),
                                                            blocksEnabled(true),

                                                            visibleLabels(false),

                                                            //##cursor(wxSTANDARD_CURSOR),

                                                            //zoomIndex(4), //##Ver cómo conviene inicializar esto, por ahora es en el 100%
                                                            zoomIndex(3), //##Ver cómo conviene inicializar esto, por ahora es en el 75%
                                                            //activeWindow(false),
                                                            startCanvasDrag(false),
                                                            downX(0),
                                                            downY(0),
                                                            addBlockButton(NULL),
                                                            addBlockButton_DefaultWidth(32),
                                                            addBlockButton_DefaultHeight(32),
                                                            dragStartPos(0, 0)
{
    //##Pasar esto a una función y agregar chequeos:
    zoomFactor[0] = 0.1;
    zoomFactor[1] = 0.25;
    zoomFactor[2] = 0.5;
    zoomFactor[3] = 0.75;
    zoomFactor[4] = 1.0;
    zoomFactor[5] = 1.25;
    zoomFactor[6] = 1.5;
    zoomFactor[7] = 1.75;
    zoomFactor[8] = 2.0;
    //zoomFactor[ZOOM_MAX_INDEX] = -1; //##Ver cómo se implementa el ViewAll en el futuro...
    //zoomFactor[7] = 2.0; //##View All, este valor debería ser dinámico, así está mal, pero es por ahora nomás...

    //##Hacer esto configurable y pasar el color en el constructor, con un valor por defecto, en lo posible:
    //SetBackgroundColourAndRefresh(wxColour(255, 249, 189));
    //SetBackgroundColourAndRefresh(wxColour(0, 0, 0));
    SetBackgroundColourAndRefresh(info.getBackgroundColour());

    //SetWindowStyle(GetWindowStyle() | wxCLIP_CHILDREN); //##Ver si sirve

    //##Ver si hay que implementar algo más...

    //##Por ahora estas inicializaciones van acá, luego se irán a la clase BubbleBlock?:
	scrollX = initialBlockX;
	scrollY = initialBlockY;

    Connect(wxEVT_MOUSEWHEEL,
            wxMouseEventHandler(BubbleCanvas::onMouseWheel));

    //##Pruebita: No compila, se vé que no tengo Bind en mi versión de wxWidgets:
    //Bind(wxEVT_MOUSEWHEEL, &BubbleCanvas::onMouseWheel, this);

//##Quizá deje esto cableado y todo, e imponga como condición de Minibloq que debe existir el ComponentStart
//y llamarse así, lo mismo que el BlockStart. El día de mañana se agregará lo de las classes, etc.:

/////////////////
///////////////

    //##Preinstantiated objects:
    for (int i = 0; i<info.getInstancesCount(); i++)
    {
        //##Esto va a cambiar cuando se introduzca el manejo de tipos definitivo con las
        //variables, y cuando se introduzcan clases:
        BubbleInstance *tempInstance = info.getInstance(i);
        if (tempInstance)
        {
            BubbleInstance *newInstance = new BubbleInstance(tempInstance->getName(), tempInstance->getType());
            setInstance(newInstance);
        }
    }

/////////////////
    //##Los bloques de inicio de block (BlockStart), son bloques con el AddParams, y con el Return Visible
    //(único bloque con return visible, por ahora), etc.:
    //##Esto debe ser descableado, agregando una función en Bubble que sea getBlockStart, y otra que sea getComponentStart,
    //ya que ambos bloques, en la estructura actual de Bubble, son obligatorios, pero esto está cableado y debe ser puesto
    //como dinámico:
    BubbleBlockInfo blockInfo;
    if (isMain())
        blockInfo.setName(wxString("ComponentStart"));  //##Descablear? Por ahora no. Esto será así: el bloque
                                                        //ComponentStart tiene que existir, al igual que BlockStart.
    else
        blockInfo.setName(wxString("BlockStart")); //##Aún no soportada esta función de bloques de usuario. Descablear?
    blockInfo.setFunction(wxString("Flow")); //##Descablear?
    //blockInfo.setDataType(wxString("void")); //##Descablear?

    if (bubble)
    {
        if ( bubble->blockIsValid(blockInfo.getName(), blockInfo.getFunction()) )
            addBlock( bubble->getBlockInfo(blockInfo.getName(), blockInfo.getFunction()) );

        setVisibleLabels(bubble->getVisibleLabels());
    }

    //se le definen la cantidad de parámetros, etc..
    forceSaved(true); //The first block doesn't count.
    //firstBlock = &(blocks[0]);
///////////////

    tabWidth = (int)(tabWidth*zoomFactor[zoomIndex]); //##Probar...

    popEditForActions = new wxMenu;
    createMenuEditForActions();

    popEditForStartBlocks = new wxMenu;
    createMenuEditForStartBlocks();

    popEditForExpressions = new wxMenu;
    createMenuEditForExpressions();
}


//##Completar la lista de inicialización:
BubbleCanvas::BubbleCanvas(const BubbleCanvas &canvas) : autoPanTimer(this, this)
{
    //##Implementar.
}


BubbleCanvas::~BubbleCanvas()
{
    //##Ver si hay que implementar algo...

    //##Buscar algún programita de detección de memory leackages y probar que todo ande bien
    //en esta aplicación, que usa tantos objetos dinámicos.
    //##Idea: ¡¡Se podría integrar dicho programita al XDF!!

//##IMPORTANT NOTE: Don't call here to Clear, Empty or the like for the arrays that contain objects with
//parents. wxWidgets will automatically destroy all the children, so calling Clear for a wxObjArray may cause
//a crahs on application exit:
//    //Don't do this, because this array is never the owner of the blocks: blocksToPaste.Clear();
//    //selectedBlocks.Clear();
//    //blocks.Clear();

    detachAllBlocksToPaste();   //Very important! This avoids destruction of the same blocks by the array
                                //deletion and by the parent.
    detachAllBlocks();

    //deleteNonDraggableImage();

    //##En principio no hay que destruir esto (ver el comentario en el destructor de MainFrame):
    //##Ver que esto no produzca un crash (ver en la docmentación de wxWidgets qué pasa con los parents de
    //los pop-up menus -wxMenu-, que tienen algún temita con la destrucción):
    //if (popEditForActions)
    //    delete popEditForActions;
    //if (popEditForExpressions)
    //    delete popEditForExpressions;
}


void BubbleCanvas::createMenuEditForActions()
{
    //##Descablear todo esto (imágenes, strings, etc.). Más adelante se puede también personalizar qué
    //entries del menú contextual estarán presentes. En el futuro también podría haber menúes contextuales
    //para cada tipo de bloque, si fuera necesario...
    if (bubble)
    {
        if (popEditForActions == NULL)
            return;

        wxImage img;
        int iconW = 16; //##Load from XML? At least make them configurable to improve accesibility.
        int iconH = 16; //##Load from XML?

    //##Pasar esto a la forma del método?:
    // Append(int id, const wxString& item = "", const wxString& helpString = "", wxItemKind kind = wxITEM_NORMAL):
//##Implement Copy as soon as possible:

        menuEditCutActions = new wxMenuItem(popEditForActions, ID_MenuEditCutActions, _("Cut"));
        if (menuEditCutActions)
        {
            img.LoadFile(bubble->getThemePath() + wxString("/Cut.png")); //##Change this image...
            menuEditCutActions->SetBitmap(wxBitmap(img.Scale(iconW, iconH)));
            popEditForActions->Append(menuEditCutActions);
        }

        menuEditPasteActions = new wxMenuItem(popEditForActions, ID_MenuEditPasteActions, _("Paste"));
        if (menuEditPasteActions)
        {
            img.LoadFile(bubble->getThemePath() + wxString("/Paste.png")); //##Change this image...
            menuEditPasteActions->SetBitmap(wxBitmap(img.Scale(iconW, iconH)));
            popEditForActions->Append(menuEditPasteActions);
        }

        menuEditDeleteActions = new wxMenuItem(popEditForActions, ID_MenuEditDeleteActions, _("Delete"));
        if (menuEditDeleteActions)
        {
            img.LoadFile(bubble->getThemePath() + wxString("/Delete.png")); //##Change this image...
            menuEditDeleteActions->SetBitmap(wxBitmap(img.Scale(iconW, iconH)));
            popEditForActions->Append(menuEditDeleteActions);
        }

        //Separator:
        menuEditSep0Actions = new wxMenuItem(popEditForActions);
        if (menuEditSep0Actions)
            popEditForActions->Append(menuEditSep0Actions);

        menuEditCommentUncommentActions = new wxMenuItem(popEditForActions, ID_MenuEditCommentUncommentActions, _("Comment/Uncomment"));
        if (menuEditCommentUncommentActions)
        {
            img.LoadFile(bubble->getThemePath() + wxString("/CommentUncomment.png")); //##Change this image...
            menuEditCommentUncommentActions->SetBitmap(wxBitmap(img.Scale(iconW, iconH)));
            popEditForActions->Append(menuEditCommentUncommentActions);
        }
    }
}


void BubbleCanvas::updateMenuEditForActionsGUI()
{
    if (popEditForActions == NULL)
        return;

    if (menuEditCutActions)
        popEditForActions->Destroy(menuEditCutActions);
    if (menuEditPasteActions)
        popEditForActions->Destroy(menuEditPasteActions);
    if (menuEditDeleteActions)
        popEditForActions->Destroy(menuEditDeleteActions);
    if (menuEditSep0Actions)
        popEditForActions->Destroy(menuEditSep0Actions);
    if (menuEditCommentUncommentActions)
        popEditForActions->Destroy(menuEditCommentUncommentActions);

    createMenuEditForActions();
}


void BubbleCanvas::createMenuEditForStartBlocks()
{
    //##Descablear todo esto (imágenes, strings, etc.). Más adelante se puede también personalizar qué
    //entries del menú contextual estarán presentes. En el futuro también podría haber menúes contextuales
    //para cada tipo de bloque, si fuera necesario...
    if (bubble)
    {
        if (popEditForStartBlocks == NULL)
            return;

        wxImage img;
        int iconW = 16; //##Load from XML? At least make them configurable to improve accesibility.
        int iconH = 16; //##Load from XML?

        menuEditPasteStartBlocks = new wxMenuItem(popEditForStartBlocks, ID_MenuEditPasteStartBlocks, _("Paste"));
        if (menuEditPasteStartBlocks)
        {
            img.LoadFile(bubble->getThemePath() + wxString("/Paste.png")); //##Change this image...
            menuEditPasteStartBlocks->SetBitmap(wxBitmap(img.Scale(iconW, iconH)));
            popEditForStartBlocks->Append(menuEditPasteStartBlocks);
        }
    }
}


void BubbleCanvas::updateMenuEditForStartBlocksGUI()
{
    if (popEditForStartBlocks == NULL)
        return;

    if (menuEditPasteStartBlocks)
        popEditForStartBlocks->Destroy(menuEditPasteStartBlocks);

    createMenuEditForStartBlocks();
}


void BubbleCanvas::createMenuEditForExpressions()
{
    //##Descablear todo esto (imágenes, strings, etc.). Más adelante se puede también personalizar qué
    //entries del menú contextual estarán presentes. En el futuro también podría haber menúes contextuales
    //para cada tipo de bloque, si fuera necesario...
    if (bubble)
    {
        if (popEditForExpressions == NULL)
            return;

        wxImage img;
        int iconW = 16; //##Load from XML? At least make them configurable to improve accesibility.
        int iconH = 16; //##Load from XML?

    //##By now, the only menu item that works for expressions is Delete. The copy, cut and paste subsystem
    //must be implemented in the future:

        menuEditDeleteExpressions = new wxMenuItem(popEditForExpressions, ID_MenuEditDeleteExpressions, _("Delete"));
        if (menuEditDeleteExpressions)
        {
            img.LoadFile(bubble->getThemePath() + wxString("/Delete.png")); //##Change this image...
            menuEditDeleteExpressions->SetBitmap(wxBitmap(img.Scale(iconW, iconH)));
            popEditForExpressions->Append(menuEditDeleteExpressions);
        }
    }
}


void BubbleCanvas::updateMenuEditForExpressionsGUI()
{
    if (popEditForExpressions == NULL)
        return;

    if (menuEditDeleteExpressions)
        popEditForExpressions->Destroy(menuEditDeleteExpressions);

    createMenuEditForExpressions();
}


bool BubbleCanvas::popupMenu(wxMenu* menu, const wxPoint& pos)
{
    //Enables and disables the pop-up menu items according to the situation:
    if (menuEditPasteActions)
    {
        //menuEditPaste->Enable(!blocksToPaste.IsEmpty());
        menuEditPasteActions->Enable(getEnableMenuPaste());
    }
    if (menuEditPasteStartBlocks)
    {
        //menuEditPaste->Enable(!blocksToPaste.IsEmpty());
        menuEditPasteStartBlocks->Enable(getEnableMenuPaste());
    }

    //Shows the menu:
    return PopupMenu(menu, pos);
}


bool BubbleCanvas::getEnableMenuPaste() const
{
    return (!blocksToPaste.IsEmpty()) && canPaste(getFirstBlockToPaste(), getCurrentBlock());
}


bool BubbleCanvas::getEnableMenuCut() const
{
    if (bubble == NULL)
        return false;
    if (getCurrentBlock() == NULL)
        return false;

    return getEnableMenuDelete() && (getCurrentBlock()->getDataType() == bubble->getActionDataType());
}


bool BubbleCanvas::getEnableMenuCopy() const
{
    return getEnableMenuCut();
}


bool BubbleCanvas::getEnableMenuDelete() const
{
    if (getCurrentBlock() == NULL)
        return false;
    if (getFirstBlock() == NULL)
        return false;
    return (getCurrentBlock() != getFirstBlock());
}


bool BubbleCanvas::getEnableMenuCommentUncomment() const
{
    return getEnableMenuCut();
}


void BubbleCanvas::updateBlocksTextGUI()
{
    //This method updates all the tooltips and labels for the blocks on the current program (used when
    //the user changes the current language):
    BubbleBlock *iteratorBlock = NULL;
    for (unsigned int i = 0; i<blocks.GetCount(); i++)
    {
        iteratorBlock = &(blocks.Item(i));
        if (iteratorBlock)
        {
            iteratorBlock->updateParamsTextGUI();
        }
    }
}


//void BubbleCanvas::onMenuEditAddComment(wxCommandEvent& evt)
//{
//
//}


//This function does not evaluates the block that is being dragged:
BubbleBlock *BubbleCanvas::findBlockUnderDragged(BubbleBlock *const dragged, const wxPoint& point) const
{
    if (dragged == NULL)
        return NULL;

    BubbleBlock *iteratorBlock = NULL;
    wxPoint iteratorPos(0, 0);
    wxSize iteratorSize(0, 0);

    //##Por ahora esto es sólo para action blocks, después se puede hacer algo que determine el action (por
    //las coordenadas verticales y entonces determine el parámetro específico):
    for (unsigned int i = 0; i<blocks.GetCount(); i++)
    {
        iteratorBlock = &(blocks.Item(i));
        if (iteratorBlock)
        {
            if (iteratorBlock != dragged)
            {
                iteratorPos = iteratorBlock->GetPosition();
                iteratorSize = iteratorBlock->GetSize();
//                if (bubble) //##Debug.
//                    if (bubble->getNotifier()) //##Debug.
//                        bubble->getNotifier()->showMessage( (wxString("\nx=") << iteratorPos.x) +
//                                                            (wxString("\ty=") << iteratorPos.y),
//                                                            false, true, *wxRED); //##Debug.
                if (dragged && getBubble())
                {
                    //In action blocks, this function only evaluates the current "line" of blocks over which
                    //the dragged block is being dragged (vertical coordinate):
                    if (dragged->getDataType() == getBubble()->getActionDataType())
                    {
                        if ( (iteratorPos.y <= point.y) && (point.y < (iteratorPos.y + iteratorSize.GetHeight())) )
                        {
                            return iteratorBlock;
                        }
                    }
                    else
                    {
                        if (    (iteratorPos.x <= point.x) && (point.x < (iteratorPos.x + iteratorSize.GetWidth())) &&
                                (iteratorPos.y <= point.y) && (point.y < (iteratorPos.y + iteratorSize.GetHeight())) )
                        {
                            return iteratorBlock;
                        }
                    }
                }
                else
                {
                    return NULL;
                }
            }
        }
    }
    //There is no valid block under the point:
    return NULL;
}


bool BubbleCanvas::canPaste(BubbleBlock *const cut, BubbleBlock *const dest) const
{
    if (bubble == NULL)
        return false;
    if (cut == NULL)
        return false;
    if (dest == NULL)
        return false;

    int destIndex = blocks.Index(*dest);

    //Rules that apply when both blocks are actions:
    if ( (cut->getDataType() == bubble->getActionDataType()) && (destIndex != wxNOT_FOUND) )
    {
        //Rule: If the cutBlock is a variableInit block, it can only be pasted to a position after the
        //following block types:
        //* componentStart
        //* blockStart
        //* variableInit
        if (cut->getVariableInit())
        {
            if ( !(dest->getComponentStart() || dest->getBlockStart() || dest->getVariableInit()) )
                return false;
        }
        //Rule: If the cutBlock isn't a variableInit block, it can't be pasted before a variableInit:
        else
        {
            BubbleBlock *nextBlock = getNextBlock(dest);
            if (nextBlock)
                if (nextBlock->getVariableInit())
                    return false;
        }

    }

    return true;
}


bool BubbleCanvas::canDrop(BubbleBlock *const dragged, BubbleBlock *const dest) const
{
    //This function sets the rules for drag&drop and cut&paste operations. More rules may be added in the
    //future. The rules create a big AND function, where each returns false if not passed.

    if (dragged == NULL)
        return false;
    if (dest == NULL)
        return false;

    //First, check the "cut, copy & paste rules", which are a subset of the "drag&drop rules":
    if (!canPaste(dragged, dest))
        return false;

    BubbleBlock *firstBrother = dragged->getFirstBrother();
    BubbleBlock *lastBrother = dragged->getLastBrother();
    int draggedIndex = blocks.Index(*dragged);
    int destIndex = blocks.Index(*dest);

    //Rules that apply when both blocks are actions:
    if ( (draggedIndex != wxNOT_FOUND) && (destIndex != wxNOT_FOUND) )
    {
        //Rule: If the block hasn't got brothers, nothing happens:
        //if ( (firstBrother == NULL) && (lastBrother == NULL) )
        //{
        //}

        //Rule: If the draggedBlock is the first of a bunch of brothers, it can't be dragged to a position
        //between itself and the lastBrother:
        if ( (firstBrother == NULL) && (lastBrother !=NULL) )
        {
            int lastBrotherIndex = blocks.Index(*lastBrother);
            if ( (draggedIndex < destIndex) && (destIndex <= lastBrotherIndex) )
                return false;
        }

        //##These rules are for the future, because right now, brothers are non-draggable blocks:
        //
        ////Rule: If the draggedBlock is the last of a bunch of brothers, it can't be dragged to a position
        ////prior to it's prevBrother:
        //if ( (firstBrother != NULL) && (lastBrother == NULL) )
        //{
        //    BubbleBlock *prevBrother = draggedBlock->getPrevBrother();
        //    if (prevBrother)
        //    {
        //        int prevBrotherIndex = blocks.Index(*prevBrother);
        //        if (prevBrotherIndex == wxNOT_FOUND)
        //            return false; //This should not happend (it would be a very strange error).
        //        if (destIndex < prevBrotherIndex)
        //            return false;
        //    }
        //    else
        //    {
        //        return false; //This should not happend (it would be a very strange error).
        //    }
        //}
        //
        ////Rule: If the draggedBlock is an "intermediate" brother, it can't be dragged outside the brothers:
        //if ( (firstBrother != NULL) && (lastBrother != NULL) )
        //{
        //    int lastBrotherIndex = blocks.Index(*lastBrother);
        //    int firstBrotherIndex = blocks.Index(*firstBrother);
        //    if ( (destIndex < firstBrotherIndex) || (lastBrotherIndex <= destIndex) )
        //        return false;
        //}
    }
    //Rules that apply when the draggedBlock is an param, and the destblock is an action:
    else if ( (draggedIndex == wxNOT_FOUND) && (destIndex != wxNOT_FOUND) )
    {
        //##Implementar...
        return false;
    }
    //Rules that apply when the draggedBlock is an action, and the destblock is a param:
    else if ( (draggedIndex != wxNOT_FOUND) && (destIndex == wxNOT_FOUND) )
    {
        //An action can't be dropped into a param.
        return false;
    }
    //Rules that apply when the draggedBlock is an param, and the destblock is an param:
    else
    {
        //##Implementar...
        return false;
    }

    //Only can drop if the blocks pair passes ALL the previous rules:
    return true;
}


//bool BubbleCanvas::getScrollBarsVisible() const
//{
//    //##Implement.
//    return ##;
//}
//
//
//void BubbleCanvas::setScrollBarsVisible(bool value)
//{
//    //##Implement.
//}


void BubbleCanvas::adjustVScrollBar()
{
//##Without the call to SetScrollbar the V scroll bar is not visible (which is desired as this is not
//finished yet):

//    //##Finish this:
//    BubbleBlock *block0 = getFirstBlock();
//    BubbleBlock *blockN = &(blocks.Last());
//    if (block0 && blockN)
//    {
//        int virtualHeight = blockN->GetPosition().y + blockN->GetSize().GetHeight() - block0->GetPosition().y;
//        SetScrollbar(   wxVERTICAL,
//                        virtualHeight/2 - block0->GetPosition().y, //Position.
//                        virtualHeight/10, //Thumb size.
//                        virtualHeight, //Range.
//                        true
//                    );
//    }
}


void BubbleCanvas::adjustHScrollBar()
{
    //##Implement.
}


void BubbleCanvas::onSize(wxSizeEvent &event)
{
    adjustVScrollBar();
    adjustHScrollBar();
}


void BubbleCanvas::onScrollBarThumbRelease(wxScrollWinEvent &event)
{
    //##
    setVScroll(GetScrollPos(wxVERTICAL));
}


void BubbleCanvas::onMouseWheel(wxMouseEvent &event)
{
//    if (!activeWindow)
//        event.Skip();

    //##Más adelante estará implementado un factor de zoom que funcione aunque no haya bloques, pero
    //por ahora, esto evita un error por referencia a la nada (y obviamente se cuelga la aplicación):
    if (blocks.IsEmpty())
        return;

    if (bubble)
    {
        //If any temporal picker is being showed, it must be closed:
        if (bubble->getNotifier())
            bubble->closeTemporalPickers();
    }

    wxSize size;
    wxPoint position;

    //##Futuro: Agregar funciones de centrado sobre el cursos al Zoom, etc.:
    //
    //##Futuro lejano: Mejorar el flickering con double-buffering o algo así.
    //
    //##Ver que las direcciones de zoom, scroll, etc. con respecto al mouse wheel sean iguales a las del Scintilla, para
    //ganar en coherencia:
        //Actualmente están iguales al Acrobat reader y al Inkscape, y me resultan intuitivas, al menos a mí.
        //El zoom es inverso al del Scintilla utilizado en Code::Blocks, pero espero poder invertir eso en el Scintilla,
        //porque me gusta así como está y además creo que el Acrobat es suficientemente popular como para que haya gente acostumbrada a él.
    if (event.m_controlDown)
    {
        //Zoom:
        //##Futuro: Agregar el zoom rápido cuando están presionados Shift y Ctrl a la vez:
        //##Nota: La dirección del zoom con respecto al giro de la rueda, se seleccionó para que resulte consistente con el zoom del Scintilla, aunque
        //a mí personalmente me resulta más intuitivo al revés, y sería consistente con otras herramientas que usamos (pero que a diferencia del
        //Scintilla no están dentro del nuestro software, como el Inkscape):

        //Configuración compatible con el Scintilla:
        //if (event.m_wheelRotation > 0)
        //
        //##Nota, esta configuración es la que me resulta cómoda a mí, pero es al revés que en el Scintilla. Me parece además más consistente porque
        //con Shift + Wheel se AUMENTA el scroll en X y con Shift + Alt se AUMENTA el scroll en y (con respecto al sistema de coordenadas de Windows),
        //por lo que así me gusta más. Pero si en el Scintilla no logro reconfigurar eso, entonces tendré que cambiarlo acá también para que
        //queden CONSISTENTES:
        //
        //Configuración compatible con los browsers (Chrome, Firefox), con XULRunner, con Inkscape, con Acrobat Reader, etc.:
        if (event.m_wheelRotation > 0)
        {
            if (zoomIndex < ZOOM_MAX_INDEX)
            {
                zoomIndex++;
                zoom(); //This is inside the if to avoid flickering by not trying to zoom once the index is the maximun.
            }
        }
        else
        {
            if (zoomIndex > 0)
            {
                zoomIndex--;
                zoom(); //Same as above but for the lower limit of index.
            }
        }
    }
    //##Ver qué elijo finalmente (ver Scintilla e Inkscape, PERO CASI SEGURO UTILIZARÉ EL SHIFT):
    //##El Shift NO TIENE EL PROBLEMA DE LA PÉRDIDA DE FOCO descripto más abajo.
    //##ADEMÁS: el Alt se utilizará para los comandos contextuales todo el tiempo, así que hay que descargarlo de funciones.
    //else if (event.m_altDown)
    //Por otro lado el Shift es el que usa Inkscape.
    //##Agregar funciones de autocentrado, etc. al scroll:
    else if (event.m_shiftDown)
    {
        //##Acá va a haber que llamar a una función de scroll, a la que también se pueda llamar cuando se usan los otros métodos para scrollar...

        //##Ojo con la pérdida de foco en esta situación (creo que se arregla con un SetFocus):
        //
        //Se hace scroll horizontal con Alt presionado, luego se suelta Alt y SIN DAR CLICK
        //se hace scroll vertical, el cual ya no funciona. Es desagradable, y hay que corregirlo.

        //Scroll horizontal:
        if (event.m_wheelRotation > 0)
        {
            //##Does not make sense to scroll more than the most left-positioned block (which is always the
            //first block):
            if (!blocks.IsEmpty())
            {
                setHScroll(defaultHScrollInc);
            }
        }
        //##Acá la condición de tope es compliada: Hay que agregar un método al Canvas que informe del bloque
        //más a la derecha, de modo que se pueda escrolar hasta un poquito antes de perderlo de vista.
        //Cada vez que se hace un AddBlock, se debe comparar con el último bloque "de más a la derecha" y
        //actualizar el tope de scroll horizontal:
        else
        {
            setHScroll(-defaultHScrollInc);
        }
    }
    else
    {
        //Scroll vertical:
        if (event.m_wheelRotation > 0)
        {
            setVScroll(defaultVScrollInc);
        }
        else
        {
            if (!blocks.IsEmpty())
            {
                setVScroll(-defaultVScrollInc);
            }
        }
    }
}


void BubbleCanvas::onCopyBlock(wxCommandEvent &evt)
{
    if (getLockBlockKeyboardEvents())
        return;

    if (getCurrentBlock())
        copyBlock(getCurrentBlock());
}


void BubbleCanvas::onCutBlock(wxCommandEvent &evt)
{
    if (getLockBlockKeyboardEvents())
        return;

    if (getCurrentBlock())
        cutBlock(getCurrentBlock(), true);

    evt.Skip(); //Is this necessary?
}


void BubbleCanvas::onCommentUncommentBlock(wxCommandEvent &evt)
{
    if (getLockBlockKeyboardEvents())
        return;

    if (getCurrentBlock())
    {
        commentUncommentBlock(getCurrentBlock());
    }

    evt.Skip(); //Is this necessary?
}


void BubbleCanvas::onPasteBlock(wxCommandEvent &evt)
{
    if (getLockBlockKeyboardEvents())
        return;

    if (getCurrentBlock())
    {
        //Now, if the block was a variableInit, finds repeated var names:
        BubbleBlock *blockToPaste = getFirstBlockToPaste();
        if (blockToPaste)
        {
            pasteBlock(getCurrentBlock());
            if (blockToPaste->getVariableInit())
                varNameIsInUse(blockToPaste->getConstantFieldValue(), blockToPaste, true);
        }
    }
}


void BubbleCanvas::onDeleteBlock(wxCommandEvent &evt)
{
    if (getLockBlockKeyboardEvents())
        return;

    if (getCurrentBlock())
        cutBlock(getCurrentBlock(), false);
    evt.Skip(); //Is this necessary?
}


void BubbleCanvas::onUndo(wxCommandEvent &evt)
{
    if (getLockBlockKeyboardEvents())
        return;

    undo();
}


void BubbleCanvas::onRedo(wxCommandEvent &evt)
{
    if (getLockBlockKeyboardEvents())
        return;

    redo();
}


void BubbleCanvas::onSelectAll(wxCommandEvent &evt)
{
    if (getLockBlockKeyboardEvents())
        return;

    selectAll();
}


void BubbleCanvas::onRestoreZoomTo100(wxCommandEvent &evt)
{
    if (getLockBlockKeyboardEvents())
        return;

    setZoomIndex(4); //##This will change by constants (not just indexes!).
}


void BubbleCanvas::onRestoreZoomTo75(wxCommandEvent &evt)
{
    if (getLockBlockKeyboardEvents())
        return;

    setZoomIndex(3); //##This will change by constants (not just indexes!).
}


void BubbleCanvas::onDecZoom(wxCommandEvent &evt)
{
    if (getLockBlockKeyboardEvents())
        return;

    setZoomIndex(getZoomIndex() - 1);
}


void BubbleCanvas::onIncZoom(wxCommandEvent &evt)
{
    if (getLockBlockKeyboardEvents())
        return;

    setZoomIndex(getZoomIndex() + 1);
}


void BubbleCanvas::onScrollFirstBackBlock(wxCommandEvent &evt)
{
    if (getLockBlockKeyboardEvents())
        return;

    BubbleBlock *block = getCurrentBlock();
    if (block)
    {
        setCurrentBlock(block->getFirstBackBlock());
        scrollToCurrentBlock();
    }
}


void BubbleCanvas::onScrollHome(wxCommandEvent &evt)
{
    if (getLockBlockKeyboardEvents())
        return;

    setCurrentBlock(getFirstBlock());
    scrollToHome();
}


void BubbleCanvas::onScrollHomeAbsolute(wxCommandEvent &evt)
{
    if (getLockBlockKeyboardEvents())
        return;

    setCurrentBlock(getFirstBlock());
    scrollToHomeAbsolute();
}


void BubbleCanvas::onScrollEnd(wxCommandEvent &evt)
{
    if (getLockBlockKeyboardEvents())
        return;

    setCurrentBlock(getLastBlock());
    scrollToEnd();
}


void BubbleCanvas::onScrollLastParamBlock(wxCommandEvent &evt)
{
    if (getLockBlockKeyboardEvents())
        return;

    BubbleBlock *block = getCurrentBlock();
    if (block)
    {
        setCurrentBlock(block->getLastParamFirstBlock());
        scrollToCurrentBlock();
    }
}


void BubbleCanvas::onDecHScroll(wxCommandEvent &evt)
{
    if (getLockBlockKeyboardEvents())
        return;

    setHScroll(-defaultHScrollInc);
}


void BubbleCanvas::onIncHScroll(wxCommandEvent &evt)
{
    if (getLockBlockKeyboardEvents())
        return;

    setHScroll(defaultHScrollInc);
}


void BubbleCanvas::onDecVScroll(wxCommandEvent &evt)
{
    if (getLockBlockKeyboardEvents())
        return;

    setVScroll(-defaultVScrollInc);
}


void BubbleCanvas::onIncVScroll(wxCommandEvent &evt)
{
    if (getLockBlockKeyboardEvents())
        return;

    setVScroll(defaultVScrollInc);
}


void BubbleCanvas::onMoveLeft(wxCommandEvent &evt)
{
    if (getLockBlockKeyboardEvents())
        return;

    selectPrevHorizontalBlockAndScroll();
    //evt.Skip(); //Linux version (Juan Pizarro): The Skip() fires multiple undesired key events.
}


void BubbleCanvas::onMoveRight(wxCommandEvent &evt)
{
    if (getLockBlockKeyboardEvents())
        return;

    selectNextHorizontalBlockAndScroll();
    //evt.Skip(); //Linux version (Juan Pizarro): The Skip() fires multiple undesired key events.
}


void BubbleCanvas::onMoveUp(wxCommandEvent &evt)
{
    if (getLockBlockKeyboardEvents())
        return;

    selectPrevVerticalBlockAndScroll();
    //evt.Skip(); //Linux version (Juan Pizarro): The Skip() fires multiple undesired key events.
}


void BubbleCanvas::onMoveDown(wxCommandEvent &evt)
{
    if (getLockBlockKeyboardEvents())
        return;

    selectNextVerticalBlockAndScroll();
    //evt.Skip(); //Linux version (Juan Pizarro): The Skip() fires multiple undesired key events.
}


void BubbleCanvas::OnMouseEvent(wxMouseEvent &event)
{
    if (event.LeftIsDown())
    {
//##:
#if 1
        if (event.Dragging() )
        {
            if (draggedBlock)
            {
                wxPoint draggedBlockPos = draggedBlock->GetPosition();
                int tolerance = 6; //##Hardwired by now, but in the future may be configurable...
                int dx = abs(event.GetPosition().x - (dragStartPos.x + draggedBlockPos.x));
                int dy = abs(event.GetPosition().y - (dragStartPos.y + draggedBlockPos.y));
                if ( (dx <= tolerance) && (dy <= tolerance) )
                {
                    event.Skip();
                    return; //Is this necessary after a call to Skip()?
                }

                //draggedBlock->setDragging(true);

                if (draggedBlock->getIsDraggable())
                {
                    //##This may change in the future (or may be made user-configurable):
                    if (draggedBlock->areParamsVisible())
                    {
                        draggedBlock->showParams(false);
                        draggedBlock->Show(true);
                    }

                    if (getDisableWhileDragging())
                    {
                        //Only disables the blocks once (when the dragging starts):
                        if (areBlocksEnabled())
                        {
                            enableAllBlocks(false); //##Ver si esto se queda así.
                            if (!draggedBlock->isCommented()) //##
                                draggedBlock->enableParamsChildren(true); //##
                        }
                    }
                    draggedBlock->showChildren(false);

                    //##Don't delete this: It does work, but with too much flickering by now:
                    //draggedBlock->moveWithParams(event.GetPosition().x - dragStartPos.x - draggedBlockPos.x,
                    //                                event.GetPosition().y - dragStartPos.y - draggedBlockPos.y);
                    draggedBlock->moveWithBrothers(event.GetPosition().x - dragStartPos.x - draggedBlockPos.x,
                                                   event.GetPosition().y - dragStartPos.y - draggedBlockPos.y);
                    //draggedBlock->Move( event.GetPosition().x - dragStartPos.x, event.GetPosition().y - dragStartPos.y);

                    BubbleBlock *destBlock = findBlockUnderDragged(draggedBlock, event.GetPosition());
                    if (destBlock)
                    {
                        if ( canDrop(draggedBlock, destBlock) )
                        {
                            //##Test de interactividad, se puede resaltar mejor que con el setCurrentBlock, pero por ahora es lo que hay:
                            draggedBlock->Update();
                            draggedBlock->Refresh();
                            setCurrentBlock(destBlock);
                            //scrollToCurrentBlock(); //##Atuopan tests.
                        }
                    }
                }
                autoPanV(draggedBlockPos);
            }
        }
        else
#endif
        {
            if (bubble)
                bubble->setCurrentCanvas(this);

            //SetFocus();
            SetFocusIgnoringChildren();
            setFocused(true);
        }
    }
    event.Skip();
}


void BubbleCanvas::onLeftUp(wxMouseEvent &event)
{
//##:
#if 1
    if (draggedBlock)
    {
        BubbleBlock *destBlock = findBlockUnderDragged(draggedBlock, event.GetPosition());
        if ( (getCurrentBlock() == getLastBlock()) || (getCurrentBlock() == getFirstBlock()) )
            destBlock = getCurrentBlock(); //This is to make drag handier.

//        if (bubble) //##Debug.
//        {
//            if (bubble->getNotifier()) //##Debug.
//            {
//                if (destBlock) //##Debug.
//                    bubble->getNotifier()->showMessage(wxString("\nDragged!") , false, true, *wxGREEN); //##Debug.
//                else
//                    bubble->getNotifier()->showMessage(wxString("\nNot dragged.") , false, true, *wxRED); //##Debug.
//            }
//        }
        stopAutoPanV();
        if ( destBlock && (destBlock != draggedBlock) && draggedBlock->getDragging())
        {
            if ( canDrop(draggedBlock, destBlock) )
            {
                BubbleBlock *lastBrother = draggedBlock->getLastBrother();
                if (lastBrother)
                {
                    moveBlocks(draggedBlock, lastBrother, destBlock);
                    setCurrentBlock(draggedBlock);
                    draggedBlock->endDrag(false);   //Does not make the setPosition (drop) in endDrag() method,
                                                    //because it was already made by moveBlocks().
                    zoom();
                    updateCodeAndRefresh();
                    forceSaved(false);
                }
                else
                {
                    //Detaches the dragged block from it's current position:
                    int index = blocks.Index(*draggedBlock);
//                    int tempIndex = index; //To avoid forcing the saveFlag if the user dragged the block to
//                                                    //is current position.
                    blocks.Detach(index);

                    //Inserts after the destination block:
                    index = blocks.Index(*destBlock);
                    if ( (index + 1) < ((int)blocks.GetCount()) )
                    {
                        //bubble->getNotifier()->showMessage(wxString("\nInserted!") , false, true, *wxGREEN); //##Debug.
                        blocks.Insert(draggedBlock, index + 1);
                        setCurrentBlock(draggedBlock);
                        draggedBlock->endDrag(true);
                        zoom();
                        updateCodeAndRefresh();
                        forceSaved(false);
//##
//                        draggedBlock->endDrag(tempIndex != (index + 1));    //This condition evaluation avoids forcing
//                                                                            //the saveFlag if the user dragged the block
//                                                                            //to is current position.
                    }
                    else
                    {
                        //bubble->getNotifier()->showMessage(wxString("\nAdded!") , false, true, *wxRED); //##Debug.
                        blocks.Add(draggedBlock);
                        setCurrentBlock(draggedBlock);
                        draggedBlock->endDrag(true);
                        zoom();
                        updateCodeAndRefresh();
                        forceSaved(false);
                    }
                }
            }
            else
            {
                if (draggedBlock->getDragging())
                {
                    setCurrentBlock(draggedBlock);
                    zoom();
                    updateCodeAndRefresh();
                    //##forceSaved(false);?
                }
                draggedBlock->endDrag(false);
            }
        }
        else
        {
            if (draggedBlock->getDragging())
            {
                setCurrentBlock(draggedBlock);
                zoom();
                updateCodeAndRefresh();
                //##forceSabed(false);?
            }
            draggedBlock->endDrag(false);
        }
        if (!areBlocksEnabled())
        {
            enableAllBlocks(true);
            zoom(); //If blocks were disabled, implies that there was something dragged, so: zoom again!
        }
    }
#endif
    event.Skip();
}


void BubbleCanvas::onRightDown(wxMouseEvent &event)
{
    if (bubble)
        bubble->setCurrentCanvas(this);
    //SetFocus();
    SetFocusIgnoringChildren();
    setFocused(true);
    event.Skip();
}


/*  //##Parece que hay un bug en wxWidgets que hace que el evento onMiddleUp no llegue inmediatamente cuando
    //el usuario suelta el botón, sino que se dispare al siguiente evento de mouse (cualquiera sea). No sé
    //si hay forma de solucionarlo, pero mientras esto no ocurra, no se puede cambiar el cursor para el scroll
    //de botón del medio, porque es fea la sensación cuando el cursor debe ser restaurado y esto no ocurre.
void BubbleCanvas::onMouseEvents(wxMouseEvent& event)
{
    if (!(event.MiddleIsDown()))
        SetCursor(cursor);
}
*/


void BubbleCanvas::onMiddleUp(wxMouseEvent &event)
{
    //SetCursor(cursor); //##Ver bug documentado en onMouseEvents...
    startCanvasDrag = false;
    event.Skip();
}


void BubbleCanvas::onMiddleDown(wxMouseEvent &event)
{
    //##Ver el comentario de arriba, de por qué no se implementa acá el cambio de cursor:
    //cursor = GetCursor();
    //SetCursor(wxCursor(wxCURSOR_SIZING));

    downY = event.GetY();
    downX = event.GetX();
    startCanvasDrag = true;

    event.Skip();
}


void BubbleCanvas::onMouseMove(wxMouseEvent &event)
{
    if (event.MiddleIsDown() && startCanvasDrag)
    {
        setVScroll(event.GetY() - downY);
        setHScroll(event.GetX() - downX);
        downY = event.GetY();
        downX = event.GetX();
    }

    event.Skip();
}


/*##
void BubbleCanvas::onSetFocus(wxFocusEvent &event)
{
    if (bubble)
        bubble->setCurrentCanvas(this);
    event.Skip();
}
*/


void BubbleCanvas::onKillFocus(wxFocusEvent &event)
{
//    if (bubble)
//    {
//        //If any temporal picker is being showed, it must be closed:
//        if (bubble->getNotifier())
//            bubble->closeTemporalPickers();
//    }

//    wxMessageDialog dialog1(parent, wxString("LostFocus"), _("2")); //##Debug.
//    dialog1.ShowModal();//##Debug

    setFocused(false);
    event.Skip();
}


void BubbleCanvas::onMouseEnter(wxMouseEvent &event)
{
    //##No logro hacer andar el setFocus, así que tomo este evento para esto (quizá haya que hacer un Connect
    //del parent con el setFocus de acá, para que cuando se da click al Notebook, por ejemplo, entre en el
    //setfocus y cambie el currentCanvas del bubble):
//    if (bubble)
//        bubble->setCurrentCanvas(this);

    //##Toda esta truchada es porque el evento del MouseWheel no funciona con el wxPanel por ahora:

    //##Abrá que agregar otro evento de estos a los hijos del BubbleCanvas (en ese caso los
    //BubbleBlocks y sus eventuales hijos también):

    //activeWindow = true;

    //##Esto permite recibir el scroll con el mousewheel sin el problema de la pérdida de foco
    //cuando se va a otro componente (me refiero a ese bug extraño, por el que sí andan el
    //Shift+MouseWheel y el Ctrl+MouseWheel, pero no anda el MouseWheel sólo:

//##2011.02.24: Prueba para que no se pierda el foco de los edits de los bloques con el MouseEnter en el fondo del canvas:
//    if (bubble)
//    {
//        if (bubble->getCurrentCanvas() == this)
//        {
//            //SetFocusIgnoringChildren();
//            SetFocus();
//        }
//    }

    event.Skip();
}


void BubbleCanvas::onMouseLeave(wxMouseEvent &event)
{
    //activeWindow = false;
    event.Skip();
}


void BubbleCanvas::setZoomIndex(const unsigned int value)
{
    //##Las funciones de zoom en realidad trabajarán con un array,
    //o algo así, de posibles valores de zoom.

    if (value <= ZOOM_MAX_INDEX)
    {
        zoomIndex = value;
        zoom();
    }
}


unsigned int BubbleCanvas::getZoomIndex() const
{
    return zoomIndex;
}


float BubbleCanvas::getZoomFactor() const
{
    return zoomFactor[zoomIndex];
}


void BubbleCanvas::scrollToHomeAbsolute()
{
    ScrollWindow(-scrollX + initialBlockX, -scrollY + initialBlockY);
    scrollX = initialBlockX;
    scrollY = initialBlockY;
}


void BubbleCanvas::scrollToHome()
{
//##Ver si se comparar límites en el futuro para esta función también:
//    BubbleBlock *tempBlock = getFirstBlock();
//    if (tempBlock)
//    {
//        if (abs(tempBlock->GetPosition().y + tempBlock->GetSize().GetHeight()) > GetClientSize().GetHeight())
//        {
            ScrollWindow(0, -scrollY + initialBlockY);
            scrollY = initialBlockY;
//        }
//    }
}


void BubbleCanvas::scrollToEnd()
{
    //No checks, because there is always at least one block:
    BubbleBlock *tempBlock = &(blocks.Last());
    if (tempBlock)
    {
        if (abs(tempBlock->GetPosition().y + tempBlock->GetSize().GetHeight()) > GetClientSize().GetHeight())
            setVScroll(-abs(tempBlock->GetPosition().y) + GetClientSize().GetHeight()/2 + initialBlockY);
        //setVScroll(-abs((blocks.Last()).GetPosition().y) + initialBlockY);
    }
}


void BubbleCanvas::setHScroll(const int value)
{
    //##:Agregar el tema de los límites...
    if (value < 0)
    {
//        if (scrollX > initialBlockX)
//        {
            ScrollWindow(value, 0);
            scrollX += value;
//        }
    }
    else
    {
        ScrollWindow(value, 0);
        scrollX += value;
    }

    adjustHScrollBar();
}


int BubbleCanvas::getHScroll() const
{
    //##
    return scrollX;
}


void BubbleCanvas::setVScroll(const int value)
{
    if (value < 0)
    {
        if (blocks.Last().GetPosition().y > 0)
        {
            ScrollWindow(0, value);
            scrollY += value;
        }
    }
    else
    {
        if (getFirstBlock())
        {
            if ( getFirstBlock()->GetPosition().y + getFirstBlock()->GetSize().GetHeight() <
                 GetClientSize().GetHeight() - getFirstBlock()->GetSize().GetHeight() )
            {
                ScrollWindow(0, value);
                scrollY += value;
            }
        }
    }

    adjustVScrollBar(); //##
}


//##El método VScroll debería refactorizarse a setVScroll:
int BubbleCanvas::getVScroll() const
{
    //##¿Pasar esto a inline?
    return scrollY;
}


void BubbleCanvas::addAcceleratorKeys()
{
    //##Implementar: Por ahora está cableado, pero hay que levantar todo esto de un archivo:

    //##Probar bien los accelerators:

    //##Las teclas F1, F2,... pueden subsistir EN PARALELO CON OTRAS TECLAS, ya que no todas las plataformas
    //donde Minibloq deberá correr las tienen (como la OLPC XO, por ejemplo). Pero como en Windows son tan
    //comunes, cosas como el Help deberían tener "doble entrada", siendo accesible con las teclas rápidas que
    //se utilicen, pero también con F1 (creo que lo haré así, aunque puede que esto sea contraproducente porque
    //luego los usuarios se acostumbran a las "F" y no se dan cuenta de que hay otras teclas, no sé, pero como
    //se indican los shortcuts en los menúes, quizá esto esté bastante bien de todos modos...

    //##Completar la tabla (ver si se levantan cosas también del XML, como shotcuts personalizados, y ver
    //también qué se hará con la internacionalización):
//##2011.02.24: Prueba para ver el bug del "2" en las netbooks:

    unsigned int entriesCount = 23; //##Esto vuela, se levantará de XML.

    //Some of these event IDs are not associated with a real menu or control:
    wxAcceleratorEntry kyboardEntries[entriesCount];

    //Edit menu:
    kyboardEntries[0].Set(wxACCEL_CTRL,    (int) 'Z',       ID_Accel_Undo);
    kyboardEntries[1].Set(wxACCEL_CTRL,    (int) 'Y',       ID_Accel_Redo);
    kyboardEntries[2].Set(wxACCEL_CTRL,    (int) 'A',      ID_Accel_SelectAll);
    kyboardEntries[3].Set(wxACCEL_CTRL,    (int) 'C',      ID_Accel_Copy);
    kyboardEntries[4].Set(wxACCEL_CTRL,    (int) 'X',      ID_Accel_Cut);
    kyboardEntries[5].Set(wxACCEL_CTRL,    (int) 'V',      ID_Accel_Paste);
    kyboardEntries[6].Set(wxACCEL_ALT,   WXK_DELETE,     ID_Accel_Delete);

    //Zooming:
    kyboardEntries[7].Set(wxACCEL_CTRL,    (int) '0',      ID_Accel_RestoreZoomTo100);
    //kyboardEntries[7].Set(wxACCEL_CTRL,    (int) '0',      ID_Accel_RestoreZoomTo75);
    kyboardEntries[8].Set(wxACCEL_CTRL,    (int) '-',      ID_Accel_DecZoom);
    kyboardEntries[9].Set(wxACCEL_CTRL,    (int) '+',      ID_Accel_IncZoom);

    //Scrolling:
    kyboardEntries[10].Set(wxACCEL_ALT,     WXK_HOME,       ID_Accel_ScrollFirstBackBlock);
    kyboardEntries[11].Set(wxACCEL_CTRL,    WXK_HOME,       ID_Accel_ScrollHome);
    kyboardEntries[12].Set(wxACCEL_CTRL + wxACCEL_ALT,  WXK_HOME, ID_Accel_ScrollHomeAbsolute);
    kyboardEntries[13].Set(wxACCEL_CTRL,    WXK_END,        ID_Accel_ScrollEnd);
    kyboardEntries[14].Set(wxACCEL_ALT,     WXK_END,        ID_Accel_ScrollLastParamBlock);
    kyboardEntries[15].Set(wxACCEL_CTRL,    WXK_LEFT,       ID_Accel_DecHScroll);
    kyboardEntries[16].Set(wxACCEL_CTRL,    WXK_RIGHT,      ID_Accel_IncHScroll);
    kyboardEntries[17].Set(wxACCEL_CTRL,    WXK_DOWN,       ID_Accel_DecVScroll);
    kyboardEntries[18].Set(wxACCEL_CTRL,    WXK_UP,         ID_Accel_IncVScroll);

    //Block navigation:
    kyboardEntries[19].Set(wxACCEL_ALT,     WXK_LEFT,       ID_Accel_MoveLeft);
    kyboardEntries[20].Set(wxACCEL_ALT,     WXK_RIGHT,      ID_Accel_MoveRight);
    kyboardEntries[21].Set(wxACCEL_ALT,     WXK_UP,         ID_Accel_MoveUp);
    kyboardEntries[22].Set(wxACCEL_ALT,     WXK_DOWN,       ID_Accel_MoveDown);

    wxAcceleratorTable accel(entriesCount, kyboardEntries);
    SetAcceleratorTable(accel);
}


//##Ver si se puede optimizar más (y sobre todo habrá que optimizar el OnSize de los bloques):
void BubbleCanvas::zoom()
{
    //##Optimizar:
    try
    {
        BubbleBlock *prevBlock = NULL;
        BubbleBlock *iteratorBlock = NULL;

        unsigned int i = 0;
//##: Test to avoid flicker:
//        for (i = 0; i<blocks.GetCount(); i++)
//        {
//            iteratorBlock = &(blocks.Item(i)); //##In theory, this is faster than the other index based form, but I'm not sure yet...
//            if (iteratorBlock)
//                iteratorBlock->Show(false);
//        }

        for (i = 0; i<blocks.GetCount(); i++)
        {
            iteratorBlock = &(blocks.Item(i)); //##In theory, this is faster than the other index based form, but I'm not sure yet...
            if (iteratorBlock)
            {
                iteratorBlock->SetSize( (int)(iteratorBlock->getRealSize().GetWidth()*zoomFactor[zoomIndex]),
                                        (int)(iteratorBlock->getRealSize().GetHeight()*zoomFactor[zoomIndex]));

                //Sets the block's position, with it's indentation:
                if (i > 0)
                {
                    if (prevBlock)
                    {
                        wxPoint position = prevBlock->GetPosition();
                        if (iteratorBlock->getPrevBrother())
                            position.x = iteratorBlock->getPrevBrother()->GetPosition().x;
                        else
                            position.x += (int)(prevBlock->getNextBlockIndentationTabs()*(tabWidth*zoomFactor[zoomIndex]));
                        position.y += prevBlock->GetSize().GetHeight();
                        iteratorBlock->SetPosition(position);
                    }
                }

                //Sets the block's params' sizes and positions:
                zoomParams(iteratorBlock);

                //This is for the indentation of the next iteratorBlock:
                prevBlock = iteratorBlock;

                //Notifies the zoom change:
                if (bubble->getNotifier())
                    bubble->getNotifier()->zoomNotify();
            }
        }

//##: Test to avoid flicker:
//        for (i = 0; i<blocks.GetCount(); i++)
//        {
//            iteratorBlock = &(blocks.Item(i)); //##In theory, this is faster than the other index based form, but I'm not sure yet...
//            if (iteratorBlock)
//                iteratorBlock->Show(true);
//        }

        adjustVScrollBar();
        adjustHScrollBar();

        Refresh(); //Show the colors...
        Update();
    }
    catch(...)
    {
    }
}


void BubbleCanvas::zoomParams(BubbleBlock *block)
{
    for (unsigned int j = 0; j<block->getParamsCount(); j++)
    {
        BubbleBlock *paramIteratorBlock = block->getParamFirstBlock(j);
        if (paramIteratorBlock)
        {
            //paramIteratorBlock->resizeConstantField();//##
            int defWidth = paramIteratorBlock->getRealSize().GetWidth();
            int defHeight = paramIteratorBlock->getRealSize().GetHeight();
            paramIteratorBlock->SetSize((int)(defWidth*zoomFactor[zoomIndex]),
                                        (int)(defHeight*zoomFactor[zoomIndex]));

            int posY = 0;
            for (unsigned int k = 0; k < j; k++)
            {
                BubbleBlock *tempBlock = block->getParamFirstBlock(k);
                if (tempBlock)
                    posY += tempBlock->GetSize().GetHeight();
                else
                    posY += (int)(paramIteratorBlock->getOriginalSize().GetHeight()*zoomFactor[zoomIndex]); //##Ojo con considerar a los originalSize todos iguales...
            }
            paramIteratorBlock->SetPosition(wxPoint(block->GetPosition().x + block->GetSize().GetWidth(),
                                                    block->GetPosition().y +
                                                    //##(int)(paramIteratorBlock->getOriginalSize().GetHeight()*zoomFactor[zoomIndex])*j
                                                    posY
                                                    )
                                           );
            paramIteratorBlock->Update();
            paramIteratorBlock->Refresh();
            zoomParams(paramIteratorBlock);
        }
    }
}


void BubbleCanvas::forceSaved(const bool value)
{
    saved = value;

    if (bubble->getNotifier())
        bubble->getNotifier()->canvasChanged(this);
}


void BubbleCanvas::setCurrentBlock(BubbleBlock *block)
{

//##Future: Uncomment this debug dialog and test if the calls to setCurrentBlock are correct (becase
//it's possible that the system is calling this method more times than needed, due to some focus and events
//problems:
//    if (currentBlock)
//    {
//        wxMessageDialog dialog0(parent, currentBlock->getName(), _("0")); //##Debug.
//        dialog0.ShowModal();  //##Debug.
//    }

    //##Falta seleccionar gráficamente mejor:
    if (currentBlock)
    {
        //This avoids flickering and stops recursive calls between BubbleBlock::select
        //and BubbleCanvas::setCurrentBlock:
        if (currentBlock == block)
        {
            //When a block is selected, the currentCanvas must be that block's canvas:
            if (bubble)
                bubble->setCurrentCanvas(this);
            return;
        }
        else
        {
            currentBlock->unselect();
        }
    }

    currentBlock = block;
    if (currentBlock)
    {
        //##Cuidado!! El SetFocusIgnoringChildren genera un bug horrible cuando se trabaja con los comboboxes
        //de los instanceName, por ejemplo...
        //currentBlock->SetFocusIgnoringChildren();
        currentBlock->SetFocus(); //This sets the focus correctly if there are editable fields.
        currentBlock->select();
    }

    if (bubble)
    {
        //When a block is selected, the currentCanvas must be that block's canvas:
        bubble->setCurrentCanvas(this);

        //If any temporal picker is being showed, it must be closed:
        if (bubble->getNotifier())
            bubble->closeTemporalPickers();
    }
}


void BubbleCanvas::undo()
{
    //##Implementar...
}


void BubbleCanvas::redo()
{
    //##Implementar...
}


void BubbleCanvas::selectAll()
{
}


//Empties the wxObjArray without destroying the content:
//##Future: put this as an static function and make it generic, to be used with any wxObjArray:
void BubbleCanvas::detachAllBlocks()
{
    unsigned int count = blocks.GetCount();
    for (unsigned int i = 0; i<count; i++)
    {
        if (!blocks.IsEmpty())
            blocks.Detach(0);
    }
}


void BubbleCanvas::detachAllBlocksToPaste()
{
    unsigned int count = blocksToPaste.GetCount();
    for (unsigned int i = 0; i<count; i++)
    {
        //wxMessageDialog dialog0(parent, wxString("") << i, _("Paste")); //##Debug.
        //dialog0.ShowModal(); //##Debug,
        if (!blocksToPaste.IsEmpty())
            blocksToPaste.Detach(0);
    }
}


void BubbleCanvas::copyBlock(BubbleBlock *block)
{
//##Implement this:
////##Try-catch
//    //##In the future: This must use the clipboard too, in order to let copy to other applications and
//    //program instances...
//
//    //wxMessageDialog dialog1(parent, wxString("Copy"), _("1")); //##Debug.
//    //dialog1.ShowModal();//##Debug
//
//    if (block)
//    {
////        if (block->getFirstBrother()
////            firstBlockToPaste = block->getFirstBrother();
////        else
////            firstBlockToPaste = block;
////
////        //##Future: lastBlockToPaste will be used when mutiple selection becomes alive:
////        if (block->getLastBrother())
////            lastBlockToPaste = block->getLastBrother();
////        else
////            lastBlockToPaste = block;
//
//
//    }
//
//    forceSaved(false);
}


void BubbleCanvas::pasteBlock(BubbleBlock *block)
{
    //##Try-catch
    //##Future: Use the clipboard, so can paste fron other Minibloq instances.

    if (blocksToPaste.IsEmpty())
        return;
    if (block == NULL)
        return;

    //Pastes the blockToPaste AFTER the block:
    if ( canPaste(getFirstBlockToPaste(), block) )
    {
        int destIndex = blocks.Index(*block);
        BubbleBlock *iteratorBlock = NULL;
        for (unsigned int i = 0; i<blocksToPaste.GetCount(); i++)
        {
            iteratorBlock = &(blocksToPaste.Item(i));
            if (iteratorBlock)
            {
                if (destIndex != wxNOT_FOUND)
                {
                    //If it's a variableInit, probably must add the instance:
                    if (iteratorBlock->getVariableInit())
                    {
                        if (iteratorBlock->validateConstantField(false))
                        {
                            wxString varName = iteratorBlock->getConstantFieldValue();
                            if (!varNameIsInUse(varName, NULL, false))
                            {
                                BubbleInstance *var = new BubbleInstance(varName, wxString("Variable")); //##Un-hardcode.
                                if (var)
                                    setInstance(var);
                            }
                        }
                    }

                    if ( (destIndex + 1) < (int)blocks.GetCount() )
                        blocks.Insert(iteratorBlock, destIndex + 1);
                    else
                        blocks.Add(iteratorBlock);
                    iteratorBlock->Show(true);
                    iteratorBlock->showParams(true);

                    destIndex = blocks.Index(*iteratorBlock);

                    //wxMessageDialog dialog0(parent, iteratorBlock->getName(), _("Paste")); //##Debug.
                    //dialog0.ShowModal(); //##Debug,
                }
                else
                {
                    break;
                }
            }
        }
        //This does not free the memory as calling Empty() will do for an wxObjArray. Juust detach the items,
        //leaving the array empty without destroying content:
        detachAllBlocksToPaste();

        //##Test if this is handy:
        //setCurrentBlock(firstBlockToPaste);

        zoom();
        updateCodeAndRefresh();
        forceSaved(false);
    }
}


void BubbleCanvas::moveBlocks(BubbleBlock *srcFirstBlock, BubbleBlock *srcLastBlock, BubbleBlock *destBlock)
{
    int srcFirstIndex = blocks.Index(*srcFirstBlock);
    int srcLastIndex = blocks.Index(*srcLastBlock);
    int destIndex = blocks.Index(*destBlock);

    if ((srcFirstIndex == wxNOT_FOUND) ||
        (srcLastIndex == wxNOT_FOUND) ||
        (destIndex == wxNOT_FOUND)
       )
    {
        //All indexes must be in the blocks array:
        return;
    }
    if (srcFirstIndex > srcLastIndex)
    {
        //First source index can't be greater that last:
        return;
    }
    if ((srcFirstIndex <= destIndex) && (destIndex <= srcLastIndex) )
    {
        //Destination index can't be between the first and the last source blocks:
        return;
    }

    BubbleBlock *tempBlock = NULL;
    if (srcFirstIndex >= destIndex)
    {
        for (int i = srcFirstIndex; i <= srcLastIndex; i++)
        {
            tempBlock = &(blocks[i]);
            if (tempBlock) //Not really necessary.
            {
                blocks.Detach(i);
                blocks.Insert(tempBlock, destIndex + 1);

                //This is not necessary because here srcFirstIndex >= destIndex, and this function only moves
                //more than one block (so the destIndex is never the last block):
                //if ( (destIndex + 1) < ((int)blocks.GetCount()) )
                //{
                //    blocks.Insert(tempBlock, destIndex + 1);
                //}
                //else
                //{
                //    blocks.Add(tempBlock);
                //}
                destIndex++;
            }
        }
    }
    else
    {
        for (int i = srcLastIndex; i >= srcFirstIndex; i--)
        {
            tempBlock = &(blocks[i]);
            if (tempBlock) //Not really necessary.
            {
                blocks.Detach(i);
                if ( (destIndex) < ((int)blocks.GetCount()) )
                {
                    blocks.Insert(tempBlock, destIndex);
                }
                else
                {
                    blocks.Add(tempBlock);
                }
                destIndex--;
            }
        }
    }
}


void BubbleCanvas::deleteOneBlock(BubbleBlock *block, bool makeCopy)
{
    //##Empty catch: Improve this.
    try
    {
        if (!blocks.IsEmpty())
        {
            if (!block) //Redundant, but more secure.
                return;

            int index = blocks.Index(*block);
            if (index == wxNOT_FOUND)
                return;

//            if (makeCopy)
//            {
                //wxMessageDialog dialog0(parent, block->getName(), _("deleteOneBlock0")); //##Debug.
                //dialog0.ShowModal(); //##Debug,
                blocksToPaste.Add(block);
                block->showParams(false);
//            }
//            else
//                block->removeParams();

            if (index > 0) //Not the first block.
            {
                if (block != getFirstBlock()) //Reduntant, but more secure.
                {
                    //##: 2011.07.05: Test the whole thing, searching for memory-leaks (and remember the
                    //automatic delete made by the wxWindows of it's children):
                    draggedBlock = NULL;  //To avoid interference with the drag&drop subsystem.
                    blocks.Detach(index);
                    block->Hide();

                    //Moves up the blocks behind de one deleted:
                    BubbleBlock *iteratorBlock = NULL;
                    for (unsigned int i = index; i<blocks.GetCount(); i++)
                    {
                        iteratorBlock = &(blocks.Item(i));
                        if (iteratorBlock)
                        {
                            iteratorBlock->Move(iteratorBlock->GetPosition().x,
                                                iteratorBlock->GetPosition().y -
                                                block->GetSize().GetHeight()
                                               );
                        }
                    }

                    forceSaved(false);
                }
            }
        }
    }
    catch(...)
    {
    }
}


//If makeCopy is true, adds the block to the blocksToPaste array, but it's just an add to the end of that
//array, not an insert:
void BubbleCanvas::cutBlock(BubbleBlock *block, bool makeCopy)
{
    try
    {
        //I like the guards here and, this way, I think they are pretty clear:
        if (block == NULL)
            return;
        if (blocks.IsEmpty())
            return;

        int index = blocks.Index(*block);

        //##Ver si hay que llamar a endDrag...
        draggedBlock = NULL;  //To avoid interference with the drag&drop subsystem.

        if (makeCopy)
            detachAllBlocksToPaste();

        if (index == wxNOT_FOUND) //Is block a param?
        {
            //Yes, it's a param:

//##This is not working fine, the program crashes in some circumstances:
//            if (makeCopy)
                block->showParams(false);
//            else
//                block->removeParams();
            block->Hide(); //##By now, only hides deleted blocks. This has to be implemented well soon!

            //If it's a param, has a backParamSlot:
            BubbleBlock *tempBackBlock = block->getBackBlock();

            if (block->getBackParamSlot())
                block->getBackParamSlot()->setParamFirstBlock(NULL);

            if (tempBackBlock)
            {
                //wxSize tempSize = tempBackBlock->getRealSize();
                int delta = -block->getRealSize().GetHeight() + block->getOriginalSize().GetHeight();
                tempBackBlock->changeAllBackBlocksRealSize(wxSize(0, delta), true);

                BubbleParam *paramSlot = block->getBackParamSlot();
                if (paramSlot)
                {
                    //##Futuro, me parece muy piola: Implementar una ventana de "incompletos", o "errores", o
                    //lo que sea, pero en tiempo real, que vaya mostrando las cosas que están incompletas,
                    //como los parámetros no asignados, o los nombres de variables no asignados. De esta forma
                    //el programador sabe cuándo le faltan cosas antes de perder su tiempo compilando:

                    //##paramSlot->setImageDefault(tempBackBlock->getParamNotAssignedImage());
                    paramSlot->setImageDefault(paramSlot->getImageNotAssigned());
                }
                //wxMessageDialog dialog0(parent, wxString("cutParams"), _("0")); //##Debug.
                //dialog0.ShowModal();//##Debug

                setCurrentBlock(tempBackBlock);

                //wxMessageDialog dialog1(parent, wxString("cutParams"), _("1")); //##Debug.
                //dialog1.ShowModal();//##Debug

//##This is not working, possible because a lack of correct detaching of the pointer (block = NULL is
//pointless in the code below):
                //##Test this a lot! 2011.07.05:
//                if (block)
//                {
//                    RemoveChild(block);
//                    block->Destroy();   //Dont' use the delete operator here (see wxWidgets documentation).
////                    block = NULL;
////                    if (block->getBackParamSlot())
////                        block->getBackParamSlot()->setParamFirstBlock(NULL);
//                }
            }

            zoom();
            updateCodeAndRefresh();
            //SetFocus(); //##By the moment has no effect.
            setFocused(true);
            forceSaved(false);

            return;
        }

        if (index <= 0) //First block can't be deleted.
            return;


        //It's not a param, and it's not the first block, so it may be a variableInit:
        bool validateName = false;
        wxString varName("");
        if (block->getVariableInit())
        {
            validateName = true;
            if (block->getHasConstantField())
                varName = block->getConstantFieldValue();
        }

        BubbleBlock *nextCurrentBlock = &(blocks[index - 1]); //The next current block will be the prevBlock.
        BubbleBlock *iteratorBlock = NULL;
        BubbleBlock *lastBrother = NULL;
//##:
//        if (block->getDontDeleteBrothers())
//        {
//            lastBrother = block;
//            nextCurrentBlock = getPrevBlock(block);
//        }
//        else
//        {
        if (block->getFirstBrother())
        {
            //If there is a firstBrother, the next current block will be the prev block to the first brother:
            iteratorBlock = block->getFirstBrother();
            index = blocks.Index(*iteratorBlock);
            if ( (index != wxNOT_FOUND) && (index > 0) )
                nextCurrentBlock = &(blocks[index - 1]);
        }
        else
        {
            iteratorBlock = block;
        }
        if (block->getLastBrother())
            lastBrother = block->getLastBrother();
        else
            lastBrother = block;
        BubbleBlock *nextBlock = NULL;

        while ( iteratorBlock && lastBrother && (iteratorBlock != lastBrother) )
        {
            nextBlock = getNextBlock(iteratorBlock);
            //wxMessageDialog dialog0(parent, iteratorBlock->getName(), _("Delete")); //##Debug.
            //dialog0.ShowModal(); //##Debug,
            deleteOneBlock(iteratorBlock, makeCopy);
            iteratorBlock = nextBlock;
        }
//        }
        deleteOneBlock(lastBrother, makeCopy);
        setCurrentBlock(nextCurrentBlock);
        zoom(); //This rearranges everything (##but has some flickering by now, which could be corrected
                //creating the blocks in invisible state. This flickering appears with brotherBlocks, due
                //to the repositioning of them once created...).
        updateCodeAndRefresh();

        //SetFocus(); //##By the moment has no effect.
        setFocused(true);

        //If the block was a variableInit block, the variable must be removed from instances:
        if (mustRemoveInstance(block))
        {
            //wxMessageDialog dialog1(parent, block->getInstanceNameField(), _("2")); //##Debug.
            //dialog1.ShowModal();//##Debug

            removeInstance(block->getConstantFieldValue());
            updateBlocksUsingThisVarName(block->getConstantFieldValue(), wxEmptyString, NULL);
        }

        //##Test this (2011.07.05):
//        if (!makeCopy)
//        {
//            if (block)
//            {
//                RemoveChild(block);
//                block->Destroy();   //Dont' use the delete operator here (see wxWidgets documentation).
//                block = NULL;
//                //wxMessageDialog dialog1(parent, wxString("block = NULL"), _("cutBlock")); //##Debug.
//                //dialog1.ShowModal();//##Debug
//            }
//        }

        //Now, if the block was a variableInit, finds repeated var names again (because there are names that
        //now may not be repeated due to the deletion of this block):
        if (validateName)
            varNameIsInUse(wxString(""), NULL, true);

        forceSaved(false);
    }
    catch(...)
    {
    }
}


void BubbleCanvas::commentUncommentBlock(BubbleBlock *block)
{
    if (block)
    {
        block->commentWithBrothers(!block->isCommented());
        updateCodeAndRefresh();

//##The following code is also used in BubbleCanvas::cutBlock: So make a function!:
////////////////
        //If the block was a variableInit block, the variable must be removed from instances:
        if (mustRemoveInstance(block))
        {
            //wxMessageDialog dialog1(parent, block->getInstanceNameField(), _("2")); //##Debug.
            //dialog1.ShowModal();//##Debug

            removeInstance(block->getConstantFieldValue());
            updateBlocksUsingThisVarName(block->getConstantFieldValue(), wxEmptyString, NULL);
        }

        //Now, if the block was a variableInit, finds repeated var names again (because there are names that
        //now may not be repeated due to the deletion of this block):
        if (block->getVariableInit())
            varNameIsInUse(wxString(""), NULL, true);
////////////////

        forceSaved(false);
    }
}


void BubbleCanvas::findBlock(const wxString& searchString)
{
    //##Implementar...
    //##Ver el tema del criterio de búsqueda:
    //quizá necesite otros parámetros...
}


bool BubbleCanvas::mustRemoveInstance(BubbleBlock *block)
{
    if (block == NULL)
        return false;
    //if (block->getUpdateVariables())
    if (block->getVariableInit())
        return !varNameIsInUse(block->getConstantFieldValue(), block, false);
    return false;
}


//Search for at least one setter block with the same variable name:
void BubbleCanvas::setAllInstances()
{
    BubbleBlock *iteratorBlock = NULL;
    for (unsigned int i = 0; i<blocks.GetCount(); i++)
    {
        iteratorBlock = &(blocks.Item(i));
        if (iteratorBlock)
        {
            if (iteratorBlock->getVariableInit())
            {
                //##Descablear: En el futuro, se agregará el getter para el tipo de datos del instance del bloque:
                BubbleInstance *var = new BubbleInstance(iteratorBlock->getInstanceNameField(), wxString("Variable"));
                setInstance(var);
            }
        }
    }
}


//Deltes all instances, iterates over the setters, and only creates again the existent variables:
void BubbleCanvas::clearNonDeclaredInstances()
{
    clearType(wxString("Variable")); //##Descablear...
    setAllInstances();
}


bool BubbleCanvas::varNameIsInUse(const wxString& name, BubbleBlock *sender, bool markWrongBlocks)
{
    ////##Debug begin:
    //if (bubble == NULL)
    //    return false;
    //if (bubble->getNotifier() == NULL)
    //    return false;
    ////##Debug end.

    //Tow passes: First, list all used varnames, and then looks for names used more than once. This may
    //be optimized in the future, because it's temporal complexity is O(n^2):
    bool result = false;

    //First pass: lists all the names, in a sorted array (then uses a binary search to find them):
    wxSortedArrayString varNames;
    int count = blocks.GetCount();
    BubbleBlock *iteratorBlock = NULL;
    for (int i = 0; i<count; i++)
    {
        iteratorBlock = &(blocks.Item(i));
        if (iteratorBlock)
        {
            //Only variableInit blocks can declare variables:
            if (iteratorBlock->getVariableInit())
            {
                //The sender does not adds its own name:
                if (iteratorBlock != sender)
                {
                    if (!iteratorBlock->isCommented())
                        varNames.Add(iteratorBlock->getConstantFieldValue());
                }
            }
        }
    }
    //Index uses a binary search:
    result = ( varNames.Index(name) != wxNOT_FOUND );

    //Only makes a second pass if has to mark (as wrong) other variableInit blocks using a repeated name:
    if (markWrongBlocks)
    {
        //Second pass: iterate over all the variableInit blocks again, looking for already existing names.
        //But first, adds the sender's name also:
        if (sender)
        {
            if (sender->getVariableInit())
            {
                if (!sender->isCommented())
                    varNames.Add(sender->getConstantFieldValue());
            }
        }

        ////##Debug begin:
        //bubble->getNotifier()->showMessage(wxString("markWrongBlocks\n") , true, true, *wxBLUE); //##Debug.
        //for (unsigned int i = 0; i<varNames.GetCount() ; i++)
        //{
        //    bubble->getNotifier()->showMessage(varNames[i] + wxString("\n") , false, false, *wxBLUE); //##Debug.
        //}
        //bubble->getNotifier()->showMessage(wxString("\n") , false, false, *wxBLUE); //##Debug.
        ////##Debug end.

        iteratorBlock = NULL;
        for (int i = 0; i<count; i++)
        {
            iteratorBlock = &(blocks.Item(i));
            if (iteratorBlock)
            {
                //Only variableInit blocks can declare variables, so this tests is limited to them:
                if (iteratorBlock->getVariableInit())
                {
                    if (!iteratorBlock->isCommented())
                    {
                        int ocurrences = Bubble::strOcurrencesInArray(iteratorBlock->getConstantFieldValue(), varNames);
                        //bubble->getNotifier()->showMessage( iteratorBlock->getConstantFieldValue() + wxString(" <- ") +
                        //                                    (wxString("") << ocurrences) +
                        //                                    wxString("\n"),
                        //                                    false, false, *wxGREEN
                        //                                  ); //##Debug.
                        iteratorBlock->setConstantFieldError(ocurrences > 1);
                    }
                }
            }
        }
    }
    return result;
}


void BubbleCanvas::updateBlocksUsingThisVarName(const wxString& currentName,
                                                const wxString& newName,
                                                BubbleBlock *sender)
{
    BubbleBlock *iteratorBlock = NULL;
    for (unsigned int i = 0; i<blocks.GetCount(); i++)
    {
        iteratorBlock = &(blocks.Item(i));
        if (iteratorBlock)
        {
            iteratorBlock->updateVarNameValidation(currentName, newName, sender);
            iteratorBlock->validateParamsVarNames(currentName, newName, sender);
        }
    }
}


bool BubbleCanvas::loadFromFile(const wxString& name)
{
    forceSaved(true);
#if (!UNDER_DEVELOPMENT)
    //##Implementar...
    return true;
#endif
}


bool BubbleCanvas::saveToFile(const wxString& name)
{
    forceSaved(true);
#if (!UNDER_DEVELOPMENT)
    //##Implementar...
    return true;
#endif
}


void BubbleCanvas::print()
{
#if UNDER_DEVELOPMENT
    //##No se hace así, como está acá, ver el edit.cpp del ejemplo estándar "stc" de wxWidgets:
    try
    {
        //##Ver si estas cosas se quedan acá o se van a la clase o al mismo Bubble:
        wxPrinter printer;
        BubblePrintout printout(wxString("File name")); //##
        wxPrinterDC *printerDC = (wxPrinterDC *)printout.GetDC(); //##Ver si el cast es correcto...
        //wxDC *printerDC = printout.GetDC(); //##Ver si el cast es correcto...

        //##Implementar: Recorrer todo el array de bloques y ver cómo generar las páginas a imprimir:
        if (currentBlock)
        {
            wxClientDC dc(currentBlock); //Temporary stack object, as the wxWidgets doc says...
//            printerDC->Blit(    0,                          //wxCoord xdest,
//                                0,                          //wxCoord ydest,
//                                dc.GetSize().GetWidth(),    //wxCoord width,
//                                dc.GetSize().GetHeight(),   //wxCoord height,
//                                &dc,                         //wxDC* source,
//                                0,                          //wxCoord xsrc,
//                                0                           //wxCoord ysrc
//                            );
//            printerDC->DrawLine(0, 0, 10, 10);
//                            //int logicalFunc = wxCOPY,
//                            //bool useMask = false,
//                            //wxCoord xsrcMask = -1,
//                            //wxCoord ysrcMask = -1);
        }

        printer.Print(this, &printout, true);
    }
    catch(...)
    {
        //##
    }
#endif
}


wxImage BubbleCanvas::getCanvasCapture()
{
    //##Esto funciona con el área visible, pero hay que recorrer todos los childs, y posiblemente armar a
    //mano la indentación, etc. para generar esto. Lo bueno es que luego este método puede ser el que se
    //utilice para el print().
    //##Además, con el recorrido de los childs, se va a poder evitar el background color que se genera con
    //la captura de pantalla como se está haciendo ahora:
    //##Tener en cuenta también que cuando se cierra la aplicación, parece que se pierde el contenido del
    //clipboard (ver la doc de wxWidgets al respecto).
    wxWindowDC  dc(this); //According to the wxWidgets documentation, this should be constructed in the stack.
    wxSize size = GetClientSize();
    wxImage img(size);

    try
    {
        wxColour colour;
        if (bubble->getNotifier())
            bubble->getNotifier()->setProgressMax(size.GetWidth());
        for (int x = 0; x < size.GetWidth(); x++)
        {
            if (bubble->getNotifier())
                bubble->getNotifier()->setProgressPosition(x, true, true);
            for (int y = 0; y < size.GetHeight(); y++)
            {
                dc.GetPixel(x, y, &colour);
                img.SetRGB(x, y, colour.Red(), colour.Green(), colour.Blue());
            }
        }
    }
    catch(...)
    {
        //##
    }
    //##return wxBitmap(img);
    return img;
}


void BubbleCanvas::onAddBlockButtonLeftDown(wxMouseEvent& event)
{
/*##Esto no sé si va:
    //##Implementar...
    if (bubble) //##
    {
        //##Terminar acá el tema de la posición:
        //##Agregar al picker el evento lostFocus para que se autoinvisibilice (y probar que no se ejecute con los mousehover de
        //otros objetos).
        //##Ver si se puede mejorar el temita del zorder. o si hay que hacer un Reparent(currentCanvas), o algo así...
        //##También ver el tema del currentCanvas, que al moverse de notebook, se setee el nuevo.
        //##Ver también cómo se trabajará el asunto del currentBlock, porque creo que con el mousehover aparecerán los botoncitos
        //para editar, pero no estoy seguro...
        if (bubble->getNotifier())
        {
            bubble->setCurrentCanvas(this);
            //bubble->getNotifier()->toggleActionPicker();
            bubble->togglePicker(bubble->getNotifier()->getActionPicker());

                //##Agregar acá que si se presiona de nuevo el botón, y el picker se está mostrando, entonces se cierre.
        }

//        showBlockPicker(
//                        wxPoint (
//                                addBlockButton->GetPosition().x,
//                                addBlockButton->GetPosition().y + addBlockButton->GetSize().GetHeight()
//                                )
//                        );
    }
*/
    event.Skip();
}


BubbleBlock *BubbleCanvas::getFirstBlockToPaste() const
{
    if (blocksToPaste.IsEmpty())
        return NULL;
    return &(blocksToPaste[0]);
}


BubbleBlock *BubbleCanvas::getFirstBlock() const
{
    if (blocks.IsEmpty())
        return NULL;
    return &(blocks[0]);
}


BubbleBlock *BubbleCanvas::getLastBlock() const
{
    if (blocks.IsEmpty())
        return NULL;
    return &(blocks.Last()); //##Test this.
}


BubbleBlock *BubbleCanvas::getPrevBlock(BubbleBlock *block) const
{
    try
    {
        if (blocks.IsEmpty())
            return NULL;
        if (block == NULL)
            return NULL;
        if (block == getFirstBlock())
            return NULL;

        int index = blocks.Index(*block);
        if (index != wxNOT_FOUND)
            return &(blocks[index - 1]);
        return NULL;
    }
    catch(...)
    {
        return NULL;
    }
}


BubbleBlock *BubbleCanvas::getNextBlock(BubbleBlock *block) const
{
    try
    {
        if (blocks.IsEmpty())
            return NULL;
        if (block == NULL)
            return NULL;
        if (block == getLastBlock())
            return NULL;

        int index = blocks.Index(*block);
        if (index != wxNOT_FOUND)
            return &(blocks[index + 1]);
        return NULL;
    }
    catch(...)
    {
        return NULL;
    }
}


unsigned int BubbleCanvas::getBlocksCount() const
{
    return blocks.GetCount();
}


BubbleBlock *BubbleCanvas::createBlock(const BubbleBlockInfo &info)
{
    try
    {
        int defWidth = info.getOriginalSize().GetWidth();
        int defHeight = info.getOriginalSize().GetHeight();
        if (info.getParamsCount() > 0)
            defHeight = info.getOriginalSize().GetHeight()*(info.getParamsCount());

        BubbleBlock *newBlock = NULL; //Always initialize.
        if (currentBlock && !blocks.IsEmpty())
        {
            newBlock = new BubbleBlock( this,
                                        wxNewId(),
                                        wxPoint( currentBlock->GetPosition().x + (int)(currentBlock->getNextBlockIndentationTabs()*(tabWidth*zoomFactor[zoomIndex])),
                                                 currentBlock->GetPosition().y + currentBlock->GetSize().GetHeight() ),
                                        //Default size: This is not the same as the SetSize below: This sets the default size for the block:
                                        wxSize(defWidth, defHeight),
                                        info
                                      );

//            wxMessageDialog dialog1(parent, _("4"), _("4")); //##Debug.
//            dialog1.ShowModal();//##Debug

            //This does not set the default size, just adjusts the size to the current zoom level:
            if (newBlock)
            {
                newBlock->SetSize((int)(defWidth*zoomFactor[zoomIndex]),
                                  (int)(defHeight*zoomFactor[zoomIndex])
                                 );
                //20110402: Patch for the bug that inverts the brother blocks creation when a combobox (from
                //another blockhas the focus:
                newBlock->SetFocusIgnoringChildren();
            }

            if (currentBlock == &(blocks.Last()))
            {
                //The currentBlock is the last one, so add one:
                blocks.Add(newBlock);

            }
            else
            {
                //The currentBlock may be a param, so insert the new block after the firstBackBlock (when
                //there is no firstBackBlock, getFirstBackBlock() returns the block itself (in this case,
                //the currenBlock)):
                BubbleBlock *tempCurrentBlock = currentBlock->getFirstBackBlock();
                if (tempCurrentBlock)
                {
                    int index = blocks.Index(*tempCurrentBlock);
                    blocks.Insert(newBlock, index + 1); //##Ver si hay que hacer algún chequeo (como ver si Index devolvió wxNOT_FOUND)...
                    BubbleBlock *iteratorBlock = NULL;
                    //Moves up the blocks behind de one created:
                    for (int i = index + 2; i<((int)blocks.GetCount()); i++)
                    {
                        iteratorBlock = &(blocks.Item(i));
                        if (iteratorBlock)
                        {
                            iteratorBlock->Move(iteratorBlock->GetPosition().x,
                                                iteratorBlock->GetPosition().y +
                                                newBlock->GetSize().GetHeight()
                                               );   //##Por ahora, si se borra un ciclo o un if, se borra todo su contenido, cuando se soporte
                                                    //el borrado de éstos sin su contenido, entonces habrá que mover en y, para desindentar.
                        }
                    }
                }
            }
        }
        else
        {
            //There is no block, so this will be the first one:
            newBlock = new BubbleBlock( this,
                                        wxNewId(),
                                        wxPoint(
                                                scrollX,
                                                scrollY
                                               ),
                                        wxSize(
                                                (int)(defWidth*zoomFactor[zoomIndex]),
                                                (int)(defHeight*zoomFactor[zoomIndex])
                                              ),
                                        info);
            blocks.Add(newBlock);
        }

//##Sacar esto:
//        if (newBlock)
//        {
//            //currentBlock = newBlock;
//            setCurrentBlock(newBlock);
//            //newBlock->Show(true); //The blocks are created invisible, to avoid flickering:
//            //zoom();
//            //scrollToCurrentBlock();
//        }

        return newBlock;
    }
    catch(...)
    {
        return NULL;
    }
}


void BubbleCanvas::showBlocks(BubbleBlock *const firstBlock, BubbleBlock *const lastBlock, const bool value)
{
    int firstIndex = blocks.Index(*firstBlock);
    int lastIndex = blocks.Index(*lastBlock);

    if ( (firstIndex == wxNOT_FOUND) || (lastIndex == wxNOT_FOUND) )
    {
        //All indexes must be in the blocks array:
        return;
    }
    if (firstIndex > lastIndex)
    {
        //##Future: Invert the indexes if this is the case, AND TEST IT!
        return;
    }

    BubbleBlock *iteratorBlock = NULL;
    for (int i = firstIndex; i <= lastIndex; i++)
    {
        iteratorBlock = &(blocks[i]);
        if (iteratorBlock) //Not really necessary.
        {
            iteratorBlock->Show(value);
            //showParams(iteratorBlock, value);
            iteratorBlock->showParams(value);
        }
    }
}


void BubbleCanvas::showAllBlocks(const bool value)
{
    BubbleBlock *iteratorBlock = NULL;
    for (unsigned int i = 0; i<blocks.GetCount(); i++)
    {
        iteratorBlock = &(blocks.Item(i));
        if (iteratorBlock)
        {
            iteratorBlock->Show(value);
            iteratorBlock->showParams(value);
            //showParams(iteratorBlock, value);
        }
    }
}


//void BubbleCanvas::showParams(BubbleBlock *block, const bool value)
//{
//    for (unsigned int j = 0; j<block->getParamsCount(); j++)
//    {
//        BubbleBlock *paramIteratorBlock = block->getParamFirstBlock(j);
//        if (paramIteratorBlock)
//        {
//            paramIteratorBlock->Show(value);
//            showParams(paramIteratorBlock, value);
//        }
//    }
//}


//IMPORTANT NOTE: When altering the canvas, call this method ALLWAYS AFTER THE CALL TO zoom(), in order to
//avoid bad indentation in the generated code:
void BubbleCanvas::updateCodeAndRefresh()
{
    //Real-time code generation:
    if (bubble)
    {
        bubble->updateCode();
        //If any temporal picker is being showed, it must be closed:
        if (bubble->getNotifier())
            bubble->getNotifier()->refreshGeneratedCodeNotify();
    }
}


void BubbleCanvas::addBlock(const BubbleBlockInfo &info, const bool showBlock)
{
    try
    {
        BubbleBlock *firstNewBlock = NULL;
        BubbleBlock *prevBlock = NULL;
        BubbleBlock *newBlock = createBlock(info);
        if (!newBlock)
            return;

        firstNewBlock = newBlock;

        setCurrentBlock(newBlock);
        //##newBlock->Show(true); //The blocks are created invisible, to avoid flickering:
        //##newBlock->SetFocusIgnoringChildren();

//        wxMessageDialog dialog1(parent, _("1"), _("1")); //##Debug.
//        dialog1.ShowModal();//##Debug

        //Brother blocks:
        while (newBlock->getBrotherInfo())
        {
//            wxMessageDialog dialog1(parent, _("2"), _("2")); //##Debug.
//            dialog1.ShowModal();//##Debug

            prevBlock = newBlock;
            newBlock = createBlock(*(newBlock->getBrotherInfo()));
            if (newBlock)
            {
                //##2011.10.11: Test if this way of comment brothers works with the future brothers with params (like elseif):
                newBlock->comment(prevBlock->isCommented());

                prevBlock->setNextBrother(newBlock);
                newBlock->setPrevBrother(prevBlock);
                setCurrentBlock(newBlock); //##Patch for blocks like the if, with multiple brothers... Must be improved!

                //##Future: Save an array of these blocks, and then AFTER the zoom() call, call their show() method...
                //##20110417: newBlock->Show(true); //The blocks are created invisible, to avoid flickering.

//                wxMessageDialog dialog1(parent, _("3"), _("3")); //##Debug.
//                dialog1.ShowModal();//##Debug
            }
            else
                break;
        }

        setCurrentBlock(firstNewBlock);
        zoom(); //This rearranges everything (##but has some flickering by now, which could be corrected
                //creating the blocks in invisible state. This flickering appears with brotherBlocks, due
                //to the repositioning of them once created...).
        //The blocks are created invisible. To avoid flickering, this makes them visible AFTER the zoom() call:
        //Next brother blocks:
        BubbleBlock *nextBlock = firstNewBlock;
        if (showBlock)
        {
            while(nextBlock)
            {
                nextBlock->Show(true);
                nextBlock = nextBlock->getNextBrother();
            }
        }
        scrollToCurrentBlock();
        SetFocusIgnoringChildren(); //##By the moment has no effect.
        setFocused(true);
        updateCodeAndRefresh();
        forceSaved(false);
    }
    catch(...)
    {
    }
}


void BubbleCanvas::addParam(const BubbleBlockInfo &info, BubbleParam *paramSlot, const bool showParam)
{
    if (paramSlot)
    {
        BubbleBlock * firstBlock = paramSlot->getParamFirstBlock();

        //Was there already a param in this paramSlot?
        if (firstBlock)
        {
            //How many params will have the new block?
            if (info.getParamsCount() == 0)
            {
                addParamDeletingParams(info, paramSlot, showParam);
            }
            else if (info.getParamsCount() == 1)
            {
                //wxMessageDialog dialog1(parent, wxString("1 param"), _("0")); //##Debug.
                //dialog1.ShowModal(); //##Debug
                addParamDeletingParams(info, paramSlot, showParam); //##
            }
            else //if (info.getParamsCount() > 1)
            {
                addParamKeepingParams(info, paramSlot, showParam);
            }
        }
        else
        {
            //Here we are not replacing a preexistent block, but adding a new one:
            addParamDeletingParams(info, paramSlot, showParam);
        }
    }
}


void BubbleCanvas::addParamDeletingParams(const BubbleBlockInfo &info, BubbleParam *paramSlot, const bool showParam)
{
    if (paramSlot)
    {
        if (paramSlot->getParamFirstBlock() != NULL)
        {
//            wxMessageDialog dialog0(parent, wxString("addParam"), _("0")); //##Debug.
//            dialog0.ShowModal(); //##Debug

            //##Param already asigned, delete it first:
            cutBlock(paramSlot->getParamFirstBlock(), false);
//## Debug:
//            if (paramSlot->getParamFirstBlock())
//            {
//                wxMessageDialog dialog1(parent, wxString("NOT NULL"), _("1")); //##Debug.
//                dialog1.ShowModal(); //##Debug
//            }
        }
        if (paramSlot->GetParent())
        {
            int defWidth = info.getOriginalSize().GetWidth();
            int defHeight = info.getOriginalSize().GetHeight();
            if (info.getParamsCount() > 0)
                defHeight = info.getOriginalSize().GetHeight()*(info.getParamsCount());
//##2011.02.17:
//            if (info.getHasInstanceNameField())
//                defHeight += 24; //##Test: Parece que quiere, hay que definir mejor el size que se agrega con el label del name.
            BubbleBlock *newParamBlock = new BubbleBlock(   this,
                                                            wxNewId(),
                                                            wxDefaultPosition,  //##Ver si se cambia esto por una función que devuelva
                                                                                //la posición incial correcta, y a la que también se
                                                                                //llame desde zoom()...
                                                            wxSize(defWidth, defHeight),
                                                            info
                                                        );

            paramSlot->setParamFirstBlock(newParamBlock);

            if (newParamBlock)
            {
                newParamBlock->setBackBlock((BubbleBlock*)paramSlot->GetParent());
                if (newParamBlock->getBackBlock())
                {
                    //##paramSlot->setImageDefault(newParamBlock->getBackBlock()->getParamDefaultImage());
                    paramSlot->setImageDefault(paramSlot->getImageAssigned());
                }
                newParamBlock->setBackParamSlot(paramSlot);
                if (newParamBlock->getRealSize().GetHeight() > info.getOriginalSize().GetHeight())
                    newParamBlock->changeAllBackBlocksRealSize(wxSize(0, newParamBlock->getRealSize().GetHeight()-info.getOriginalSize().GetHeight()), false);

                zoom(); //This rearranges everything (##but has some flickering by now, which could possibly be corrected
                        //creating the blocks in it's very correct position. This flickering appears with brotherBlocks, due
                        //to the repositioning of them once created...).

                newParamBlock->SetBackgroundColourAndRefresh(info.getDefaultBackgroundColour1()); //##Esto es imprescindible por ahora, ver si lo mando adentro de los bloque...
                setCurrentBlock(newParamBlock);
                newParamBlock->Show(showParam); //Goes visible AFTER the zoom() call.
            }

            updateCodeAndRefresh();
            forceSaved(false);
        }
    }
}


//This routine keeps the params, but has a horrible bug in the resizing of the blocks, plus a possible memory bug:
void BubbleCanvas::addParamKeepingParams(const BubbleBlockInfo &info, BubbleParam *paramSlot, const bool showParam)
{
    if (paramSlot)
    {
        unsigned int firstBlockParamsCount = 0;
        bool firstBlockHasAddParamsButton = false;
        BubbleParam * firstBlockFirstParam = NULL;
        BubbleBlock * firstBlock = paramSlot->getParamFirstBlock();

        if (firstBlock)
        {
//            wxMessageDialog dialog0(parent, wxString("addParam"), _("0")); //##Debug.
//            dialog0.ShowModal(); //##Debug

            //Saves data belonging to the first block's first param:
            firstBlockParamsCount = firstBlock->getParamsCount();
            firstBlockHasAddParamsButton = firstBlock->getHasAddParamsButton();
            firstBlockFirstParam = firstBlock->getParamSlot(0);

            //##Deletes the current param:
            //cutBlock(firstBlock, true);
            firstBlock->Hide();
            //firstBlock->showParams(false);
        }
        if (paramSlot->GetParent())
        {
            int defWidth = info.getOriginalSize().GetWidth();
            int defHeight = info.getOriginalSize().GetHeight();
            if (info.getParamsCount() > 0)
                defHeight = info.getOriginalSize().GetHeight()*(info.getParamsCount());

            BubbleBlock *newParamBlock = new BubbleBlock(   this,
                                                            wxNewId(),
                                                            wxDefaultPosition,  //##Ver si se cambia esto por una función que devuelva
                                                                                //la posición incial correcta, y a la que también se
                                                                                //llame desde zoom()...
                                                            wxSize(defWidth, defHeight),
                                                            info
                                                        );

            paramSlot->setParamFirstBlock(newParamBlock);

            if (newParamBlock)
            {
                newParamBlock->setBackBlock((BubbleBlock*)paramSlot->GetParent());
                if (newParamBlock->getBackBlock())
                {
                    //##paramSlot->setImageDefault(newParamBlock->getBackBlock()->getParamDefaultImage());
                    paramSlot->setImageDefault(paramSlot->getImageAssigned());
                }
                newParamBlock->setBackParamSlot(paramSlot);

//                if (newParamBlock->getRealSize().GetHeight() > info.getOriginalSize().GetHeight())
//                    newParamBlock->changeAllBackBlocksRealSize(wxSize(0, newParamBlock->getRealSize().GetHeight()-info.getOriginalSize().GetHeight()), false);

                //If the param's first block had addParamsButton, completes the paramSlots added to it in the newParamBlock:
                if (newParamBlock->getHasAddParamsButton() && firstBlockHasAddParamsButton)
                {
                    for (unsigned int i=newParamBlock->getParamsCount(); i<firstBlockParamsCount; i++)
                    {
                        if (firstBlockFirstParam)
                        {
                            newParamBlock->addParamSlot(    firstBlockFirstParam->getName(),
                                                            firstBlockFirstParam->getDataType(),
                                                            firstBlockFirstParam->getImageAssigned(),
                                                            firstBlockFirstParam->getImageNotAssigned(),
                                                            firstBlockFirstParam->getImageDefault(),
                                                            firstBlockFirstParam->getImageHover(),
                                                            firstBlockFirstParam->getImagePressed(),
                                                            firstBlockFirstParam->getImageDisabled()
                                                        );
                        }
                    }
                }

                if (firstBlock)
                {
                    //Now points the original params from the previous block to the newParamBlock:
                    for (unsigned int i=0; i<newParamBlock->getParamsCount(); i++)
                    {
//                        wxMessageDialog dialog2(parent, wxString("i = ") << i, _("2")); //##Debug.
//                        dialog2.ShowModal(); //##Debug

                        if (i < firstBlock->getParamsCount())
                        {
                            BubbleParam *currentOldParam = firstBlock->getParamSlot(i);
                            if (currentOldParam)
                            {
                                BubbleBlock *currentOldParamBlock = currentOldParam->getParamFirstBlock();
                                if (currentOldParamBlock)
                                {
                                    if (currentOldParamBlock->getDataType() == newParamBlock->getParamSlot(i)->getDataType())
                                    {
                                        if (newParamBlock->getParamSlot(i))
                                        {
                                            newParamBlock->getParamSlot(i)->setParamFirstBlock(currentOldParamBlock);
                                            currentOldParamBlock->setBackBlock(newParamBlock);
                                            currentOldParamBlock->changeAllBackBlocksRealSize(wxSize(0, newParamBlock->getRealSize().GetHeight()-info.getOriginalSize().GetHeight()), false);
                                            newParamBlock->getParamSlot(i)->setImageDefault(paramSlot->getImageAssigned());
                                        }
                                        currentOldParamBlock->Show(true);
                                    }
                                    else
                                    {
                                        cutBlock(currentOldParamBlock, false);
                                    }
                                }
                            }
                            else
                            {
                                //No more paramSlots:
                                break;
                            }
                        }
                    }

                    //Finally, deletes the extra params if the new block has less paramslots than the older block:
                    for (unsigned int i = newParamBlock->getParamsCount(); i < firstBlock->getParamsCount(); i++)
                    {
                        BubbleParam *currentOldParam = firstBlock->getParamSlot(i);
                        if (currentOldParam)
                        {
                            BubbleBlock *currentOldParamBlock = currentOldParam->getParamFirstBlock();
                            if (currentOldParamBlock)
                            {
                                cutBlock(currentOldParamBlock, false);
                            }
                        }
                    }
                }

                if (newParamBlock->getRealSize().GetHeight() > info.getOriginalSize().GetHeight())
                {
                    newParamBlock->changeAllBackBlocksRealSize(wxSize(0, newParamBlock->getRealSize().GetHeight()-info.getOriginalSize().GetHeight()), false);
                }
//                else
//                {
//                    float totalParamsSize = 0;
//                    if (getZoomFactor() != 0)
//                    {
//                        totalParamsSize = (float)newParamBlock->getTotalParamsSize().GetHeight()/getZoomFactor();
//                        if (totalParamsSize < newParamBlock->getRealSize().GetHeight())
//                            newParamBlock->changeAllBackBlocksRealSize(wxSize(0, -(int)totalParamsSize + newParamBlock->getRealSize().GetHeight()), false);
//                        //wxMessageDialog dialog0(parent, wxString("0"), _("0")); //##Debug.
//                        //dialog0.ShowModal(); //##Debug
//                    }
//                }

                zoom(); //This rearranges everything (##but has some flickering by now, which could possibly be corrected
                        //creating the blocks in it's very correct position. This flickering appears with brotherBlocks, due
                        //to the repositioning of them once created...).

                newParamBlock->SetBackgroundColourAndRefresh(info.getDefaultBackgroundColour1()); //##Esto es imprescindible por ahora, ver si lo mando adentro de los bloque...
                setCurrentBlock(newParamBlock);
                newParamBlock->Show(showParam); //Goes visible AFTER the zoom() call.
            }

            updateCodeAndRefresh();
            forceSaved(false);
        }
    }
}


void BubbleCanvas::autoPanTick()
{
//##
//    if (draggedBlock)
//    {
//        draggedBlock->Update();
//        draggedBlock->Refresh();
//    }

    //##Only vertical by now:
    setVScroll(autoPanVStep);
}


//##By now, only vertical:
void BubbleCanvas::startAutoPanV()
{
    if (draggedBlock)
    {
        //draggedBlock->Show(false); //To avoid scroll of this block too!
        draggedBlock->showBrothers(false); //To avoid scroll of these blocks too!
    }
    autoPanTimer.Start(getAutoPanTimerInterval());
}


void BubbleCanvas::stopAutoPanV()
{
    if (draggedBlock)
    {
        //draggedBlock->Show(true);
        draggedBlock->showBrothers(true);
        draggedBlock->Update();
        draggedBlock->Refresh();
    }
    autoPanTimer.Stop();
}


void BubbleCanvas::autoPanV(const wxPoint &cursorPos)
{
    //##Descablear el área de autopan:
    if ( cursorPos.y > (int)(GetClientSize().GetHeight() - getAutoPanSensibleHeight()) )
    {
        autoPanVStep = -getAutoPanVStepAbsoluteValue();
        startAutoPanV();
    }
    else if (cursorPos.y < 0)
    {
        autoPanVStep = getAutoPanVStepAbsoluteValue();
        startAutoPanV();
    }
    else
    {
        stopAutoPanV();
    }
}


void BubbleCanvas::scrollHToCurrentBlock()
{
    if (currentBlock)
    {
        //This if() is a small patch to avoid a non desired scroll for the first block, when the canvas
        //is being created:
        if (currentBlock != &(blocks.Item(0)))
        {
            //Scrolls the canvas if the new block will not be visible (autoscroll):
            if (abs(currentBlock->GetPosition().x + currentBlock->GetSize().GetWidth()) > GetClientSize().GetWidth())
            {
                setHScroll(-abs(currentBlock->GetPosition().x + currentBlock->GetSize().GetWidth() - GetClientSize().x));
            }
            else
            {
                if (currentBlock->GetPosition().x < 0)
                {
                    setHScroll(abs(currentBlock->GetPosition().x));
                }
            }
        }
        else
        {
            if (currentBlock->GetPosition().x < 0)
            {
                setHScroll(abs(currentBlock->GetPosition().x));
            }
        }
    }
}


void BubbleCanvas::scrollVToCurrentBlock()
{
    if (currentBlock)
    {
        //This if() is a small patch to avoid a non desired scroll for the first block, when the canvas
        //is being created:
        if (currentBlock != &(blocks.Item(0)))
        {
            //Scrolls the canvas if the new block will not be visible (autoscroll):
            //if ((currentBlock->GetPosition().y + currentBlock->GetSize().GetHeight()) > GetClientSize().GetHeight())
            if (abs(currentBlock->GetPosition().y + currentBlock->GetSize().GetHeight()) > GetClientSize().GetHeight())
            {
//                wxMessageDialog dialog0(parent, _("1"), _("Not visible")); //##Debug.
//                dialog0.ShowModal(); //##Debug,
                setVScroll(-abs(currentBlock->GetPosition().y + currentBlock->GetSize().GetHeight() - GetClientSize().y));
            }
            else
            {
                if (currentBlock->GetPosition().y < 0)
                {
                    setVScroll(abs(currentBlock->GetPosition().y));
                }
            }
        }
        else
        {
            if (currentBlock->GetPosition().y < 0)
            {
                setVScroll(abs(currentBlock->GetPosition().y));
            }
        }
    }
}


//void BubbleCanvas::deleteNonDraggableImage()
//{
//    if (nonDraggableImage)
//    {
//        delete nonDraggableImage;
//        nonDraggableImage = NULL; //Not necessary, but more secure.
//    }
//}


void BubbleCanvas::scrollToCurrentBlock()
{
    scrollHToCurrentBlock();
    scrollVToCurrentBlock();
}


void BubbleCanvas::selectPrevVerticalBlockAndScroll()
{
    BubbleBlock *block = getCurrentBlock();
    if (!block)
        return;
    if (block->getBackBlock())
    {
        //It's a param, so go to the previous not empty param:
        int index = block->getBackBlock()->getParamIndex(block);
        if (index != wxNOT_FOUND)
        {
            BubbleBlock *firstNotEmptyBlock = NULL;
            for (int i = index - 1; i >= 0; i--)
            {
                firstNotEmptyBlock = block->getBackBlock()->getParamFirstBlock(i);
                if (firstNotEmptyBlock)
                {
                    block = firstNotEmptyBlock;
                    break;
                }
            }
            //If there are no other "brother" params, go to the next block from the blocks array:
            if (firstNotEmptyBlock == NULL)
                block = getPrevBlock(block->getFirstBackBlock());
        }
    }
    else
    {
        //It's a block belonging to the blocks main array:
        block = getPrevBlock(block);
    }
    if (block)
        setCurrentBlock(block);
    scrollToCurrentBlock();
}


void BubbleCanvas::selectNextVerticalBlockAndScroll()
{
    BubbleBlock *block = getCurrentBlock();
    if (!block)
        return;
    if (block->getBackBlock())
    {
        //It's a param, so go to the next not empty param:
        int index = block->getBackBlock()->getParamIndex(block);
        if (index != wxNOT_FOUND)
        {
//                wxString str; //##Debug.
//                str << index; //##Debug.
//                wxMessageDialog dialog0(this, str, _("1")); //##Debug.
//                dialog0.ShowModal(); //##Debug.
            BubbleBlock *firstNotEmptyBlock = NULL;
            for (unsigned int i = index + 1; i < block->getBackBlock()->getParamsCount(); i++)
            {
                firstNotEmptyBlock = block->getBackBlock()->getParamFirstBlock(i);
                if (firstNotEmptyBlock)
                {
                    block = firstNotEmptyBlock;
                    break;
                }
            }
            //If there are no other "brother" params, go to the next block from the blocks array:
            if (firstNotEmptyBlock == NULL)
                block = getNextBlock(block->getFirstBackBlock());
        }
    }
    else
    {
        //It's a block belonging to the blocks main array:
        block = getNextBlock(block);
    }
    if (block)
        setCurrentBlock(block);
    scrollToCurrentBlock();
}


void BubbleCanvas::selectPrevHorizontalBlockAndScroll()
{
    BubbleBlock *block = getCurrentBlock();
    if (block)
    {
        block = block->getBackBlock();
        if (block)
        {
            setCurrentBlock(block);
            scrollToCurrentBlock();
        }
    }
}


void BubbleCanvas::selectNextHorizontalBlockAndScroll()
{
    BubbleBlock *block = getCurrentBlock();
    if (block)
    {
        BubbleBlock *firstNotEmptyBlock = block->getParamFirstBlock(0);
        for (unsigned int i = 0; i < block->getParamsCount(); i++)
        {
            if (firstNotEmptyBlock)
                break;
            firstNotEmptyBlock = block->getParamFirstBlock(i);
        }
        if (firstNotEmptyBlock)
        {
            setCurrentBlock(firstNotEmptyBlock);
            scrollToCurrentBlock();
        }
    }
}


void BubbleCanvas::enableAllBlocks(bool value)
{
    BubbleBlock *iteratorBlock = NULL;
    for (unsigned int i = 0; i<blocks.GetCount(); i++)
    {
        iteratorBlock = &(blocks.Item(i));
        if (iteratorBlock)
        {
            //##This call (to isCommented) must be encapsulated inside the block in the future (in fact, the whole "enable subsystem"
            //myst be separated from the "comment subsystem"):
            if (!iteratorBlock->isCommented())
                iteratorBlock->enableParamsChildren(value);
        }
    }
    blocksEnabled = value;
}


void BubbleCanvas::setVisibleLabels(bool value)
{
    //##Implementar:
    //##Recorre todos los bloques (al menos los setters, y enciende los labels en cada uno de ellos):
    BubbleBlock *iteratorBlock = NULL;
    for (unsigned int i = 0; i<blocks.GetCount(); i++)
    {
        iteratorBlock = &(blocks.Item(i));
        if (iteratorBlock)
        {
            iteratorBlock->showParamsLabels(value);
        }
    }
    visibleLabels = value;
}
