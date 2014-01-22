#ifndef BubbleCanvas__h
#define BubbleCanvas__h

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/menu.h>
#include <wx/stattext.h>
#include <wx/combobox.h>
#include <wx/textctrl.h>
#include <wx/regex.h>
#include <wx/timer.h>

//##Eliminar este comentario:
//If drag-images are not used, DON'T DELETE the following lines:
//These mayt not be used now, but perhaps in the future will be useful:
//#include "wx/image.h"
//#define wxUSE_GENERIC_DRAGIMAGE 1
//#if wxUSE_GENERIC_DRAGIMAGE
//#include "wx/generic/dragimgg.h"
//#define wxDragImage wxGenericDragImage
//#else
//#include "wx/dragimag.h"
//#endif

#include "BubblePanel.h"
#include "BubbleButton.h"
//#include "IBubbleFileIO.h"
#include "BubbleHardwareManager.h" //##¡Ver si este include va acá...!

#define ZOOM_MAX_INDEX      8

#include <wx/list.h>
class BubbleCanvas;
WX_DECLARE_LIST(BubbleCanvas, listOfCanvas);

class BubbleExpressionPicker;
WX_DEFINE_ARRAY(BubbleExpressionPicker*, arrayOfExpressionPickers);
//##WX_DECLARE_LIST(BubbleExpressionPicker, listOfExpressionPickers);

#include <wx/arrimpl.cpp> //This is a magic incantation which must be done!
class BubbleBlock;
WX_DECLARE_OBJARRAY(BubbleBlock, arrayOfBlocks);
class BubbleParamInfo;
WX_DECLARE_OBJARRAY(BubbleParamInfo, arrayOfParamInfo);
class BubbleParam;
WX_DECLARE_OBJARRAY(BubbleParam, arrayOfParams);
class BubbleInstance;
WX_DECLARE_OBJARRAY(BubbleInstance, arrayOfInstances);


class BubbleInstance
{
    private:
        wxString name;
        wxString type;

    public:
        BubbleInstance() { }
        BubbleInstance(const wxString& name, const wxString& type): name(name), type(type)
        {
        }
        ~BubbleInstance() { }

        inline void setName(const wxString& value) { name = value; }
        inline const wxString &getName() const { return name; }

        inline void setType(const wxString& value) { type = value; }
        inline const wxString &getType() const { return type; }
};


class BubbleInstanceManager
{
    protected:
        arrayOfInstances instances;

    public:
        BubbleInstanceManager() { }
        virtual ~BubbleInstanceManager() { }

        virtual inline void clear() { instances.Clear(); }

        virtual void setInstance(BubbleInstance *const value);
        virtual BubbleInstance *getInstance(const wxString& name) const;
        virtual BubbleInstance *getInstance(int index) const;
        virtual int getInstanceIndex(const wxString &name) const;
        virtual void removeInstance(const wxString& name);
        virtual inline int getInstancesCount() const { return instances.GetCount(); }
        virtual void clearType(const wxString& typeName);
};


class BubbleNumber
{
    private:
        //These 3 values (min, max, decimals) determines the kind of the data
        float min;
        float max;
        unsigned int decimals;
        float defaultValue;

    public:
        BubbleNumber() { }
        BubbleNumber(const float min,
                     const float max,
                     const unsigned int decimals,
                     const float defaultValue)  :   min(min),
                                                    max(max),
                                                    decimals(decimals),
                                                    defaultValue(defaultValue)
        {
        }
        ~BubbleNumber() { }

        inline void setMin(const float value) { min = value; }
        inline float getMin() const { return min; }

        inline void setMax(const float value) { max = value; }
        inline float getMax() const { return max; }

        inline void setDecimals(const unsigned int value) { decimals = value; }
        inline unsigned int getDecimals() const { return decimals; }

        inline void setDefaultValue(const float value) { defaultValue = value; }
        inline float getDefaultValue() const { return defaultValue; }
};


class IBubbleBlockNavigator
{
    protected:
        //BubbleBlock *firstBlock;

        //##Future: Put these on a class which encapsulates all the block managment:
        //##Future: Implement some kind of iterator (such as for_each from STL)...
        arrayOfBlocks blocks;
        arrayOfBlocks blocksToPaste;


    public:
        IBubbleBlockNavigator() //: firstBlock(NULL)
        {
        }
        virtual ~IBubbleBlockNavigator() { }

        virtual BubbleBlock *getFirstBlock() const = 0;
        virtual BubbleBlock *getLastBlock() const = 0;
        virtual BubbleBlock *getPrevBlock(BubbleBlock *block) const = 0;
        virtual BubbleBlock *getNextBlock(BubbleBlock *block) const = 0;
        virtual unsigned int getBlocksCount() const = 0;
};


//##Si tiene cosas en común con BubbleParam, ésta última deberá hereder de BubbleParamInfo:
class BubbleParamInfo
{
    private:
        wxString name;
        wxString dataType;
        wxImage notAssignedImage;
        wxImage defaultImage;
        wxImage hoverImage;
        wxImage pressedImage;
        wxImage disabledImage;

    public:
        BubbleParamInfo(const wxString& name,
                        const wxString& dataType,
                        const wxImage& notAssignedImage,
                        const wxImage& defaultImage,
                        const wxImage& hoverImage,
                        const wxImage& pressedImage,
                        const wxImage& disabledImage
                        ) :
                            name(name),
                            dataType(dataType),
                            notAssignedImage(notAssignedImage),
                            defaultImage(defaultImage),
                            hoverImage(hoverImage),
                            pressedImage(pressedImage),
                            disabledImage(disabledImage)

        {
        }
        virtual ~BubbleParamInfo();

        const wxString &getName() const;
        const wxString &getDataType() const;

        const wxImage &getNotAssignedImage() const;
        const wxImage &getDefaultImage() const;
        const wxImage &getHoverImage() const;
        const wxImage &getPressedImage() const;
        const wxImage &getDisabledImage() const;
};


class Bubble;
class BubbleBlock;
class BubbleParam : public BubbleButton
{
    private:
        wxString dataType;
        BubbleBlock *firstBlock;

        wxImage imageAssigned; //##Will not be initialized.
        wxImage imageNotAssigned; //##Will not be initialized.

        void onParamLeftDown(wxMouseEvent& event);

    public:
        BubbleParam(wxWindow* parent,
                    wxWindowID id,
                    const wxString &dataType,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize,
                    const wxString& name = wxPanelNameStr);
        virtual ~BubbleParam();

        inline void setDataType(const wxString& value) { dataType = value; }
        inline const wxString &getDataType() const { return dataType; }

        inline void setParamFirstBlock(BubbleBlock *block) { firstBlock = block; }
        inline BubbleBlock *getParamFirstBlock() const { return firstBlock; }

        void setImageAssigned(const wxImage& image);
        const wxImage &getImageAssigned() const;
        void setImageNotAssigned(const wxImage& image);
        const wxImage &getImageNotAssigned() const;
};


class BubbleCanvasInfo : public BubbleInstanceManager
{
    private:
        bool _main;
        wxColour backgroundColour;
        unsigned int tabWidth;
        int initialBlockX;
        int initialBlockY;

    public:
        BubbleCanvasInfo(
                            bool _main,
                            wxColour backgroundColour,
                            unsigned int tabWidth,
                            int initialBlockX,
                            int initialBlockY) :_main(_main),
                                                backgroundColour(backgroundColour),
                                                tabWidth(tabWidth),
                                                initialBlockX(initialBlockX),
                                                initialBlockY(initialBlockY)
        {
        }
        ~BubbleCanvasInfo() {}

        inline bool isMain() const { return _main; }

        void setBackgroundColour(const wxColour& colour);
        wxColour getBackgroundColour() const;

        inline void setTabWidth(unsigned int value) { tabWidth = value; }
        inline unsigned int getTabWidth() const { return tabWidth; }

        inline void getInitialBlockX(int value) { initialBlockX = value; }
        inline int getInitialBlockX() const { return initialBlockX; }
        inline void setInitialBlockY(int value) { initialBlockY = value; }
        inline int getInitialBlockY() const { return initialBlockY; }
};


//##This timer is used for visual effects (by now, only for autoPan, but may have more uses in the future)
class BubbleCanvasTicker : public wxTimer
{
	protected:
		wxWindow *owner;
		BubbleCanvas *canvas;
		int _scrollValue;

	public:
		BubbleCanvasTicker(wxWindow *owner, BubbleCanvas* canvas) : wxTimer(owner),
                                                                    owner(owner),
                                                                    canvas(canvas),
                                                                    _scrollValue(0)
		{
		}
		virtual ~BubbleCanvasTicker() { }

		void Notify();
};


class BubbleBlockInfo;
class BubbleCanvas : public BubblePanel, public IBubbleBlockNavigator, public BubbleInstanceManager//, public IBubbleFileIO
{
    private:
        enum
        {
            //Editing:
            ID_Accel_Undo = wxID_HIGHEST+1,
            ID_Accel_Redo,
            ID_Accel_SelectAll,
            ID_Accel_Copy,
            ID_Accel_Cut,
            ID_Accel_Paste,
            ID_Accel_Delete,
            ID_Accel_CommentUncomment,

            //Zooming:
            ID_Accel_RestoreZoomTo75,
            ID_Accel_RestoreZoomTo100,
            ID_Accel_DecZoom,
            ID_Accel_IncZoom,

            //Scrolling:
            ID_Accel_ScrollFirstBackBlock,
            ID_Accel_ScrollHome,
            ID_Accel_ScrollHomeAbsolute,
            ID_Accel_ScrollEnd,
            ID_Accel_ScrollLastParamBlock,
            ID_Accel_DecHScroll,
            ID_Accel_IncHScroll,
            ID_Accel_DecVScroll,
            ID_Accel_IncVScroll,

            //Block navigation:
            ID_Accel_MoveLeft,
            ID_Accel_MoveRight,
            ID_Accel_MoveUp,
            ID_Accel_MoveDown,

            //Edit pop-up menu:
            ID_MenuEditDeleteActions,
            ID_MenuEditCopyActions,
            ID_MenuEditCutActions,
            ID_MenuEditPasteActions,
            ID_MenuEditCommentUncommentActions,

            ID_MenuEditPasteStartBlocks,
            ID_MenuEditDeleteExpressions,
        };

        //##:
        //static const wxWindowID ID_ButtonAddBlock;
        wxWindow *parent;
        Bubble *bubble;

        BubbleCanvasTicker autoPanTimer;
        int autoPanTimerInterval;
        unsigned int autoPanSensibleWidth;
        unsigned int autoPanSensibleHeight;
        int autoPanHStep;
        int autoPanVStep;

        wxMenu *popEditForActions;
        wxMenu *popEditForStartBlocks;
        wxMenu *popEditForExpressions;
        //wxMenuItem *menuEditAddComment;
        wxMenuItem *menuEditDeleteActions;
        wxMenuItem *menuEditCopyActions;
        wxMenuItem *menuEditCutActions;
        wxMenuItem *menuEditPasteActions;
        wxMenuItem *menuEditCommentUncommentActions;
        wxMenuItem *menuEditSep0Actions;

        wxMenuItem *menuEditPasteStartBlocks;
        wxMenuItem *menuEditDeleteExpressions;

        bool focused;
        bool saved;
        bool _main;
        bool lockBlockKeyboardEvents;

        unsigned int tabWidth;
        int initialBlockX;
        int initialBlockY;

        int defaultHScrollInc;
        int defaultVScrollInc;

        //##bool activeWindow;
        BubbleBlock *currentBlock;
        BubbleBlock *draggedBlock;
        bool disableWhileDragging;
        //wxDragImage *nonDraggableImage;

        bool blocksEnabled;
        //##wxCursor cursor;

        bool visibleLabels;

        //##Quizá todo este asuntito del zoom cambie por la posibilidad de que se accedad directamente al
        //zoomFactor en forma de FlotingPoint:
        unsigned int zoomIndex;
        float zoomFactor[ZOOM_MAX_INDEX + 1];

        int scrollX, scrollY;
        bool startCanvasDrag;
        int downX, downY;

        //arrayOfBlocks   blocks;
        //arrayOfBlocks   selectedBlocks; //##

        BubbleButton *addBlockButton;
        int addBlockButton_DefaultWidth;
        int addBlockButton_DefaultHeight;

        wxPoint dragStartPos;

        //void moveAddBlockButton(); //##

        BubbleBlock *createBlock(const BubbleBlockInfo &info);
        void deleteOneBlock(BubbleBlock *block, bool makeCopy);

        BubbleBlock *findBlockUnderDragged(BubbleBlock *const dragged, const wxPoint& point) const;
        bool canPaste(BubbleBlock *const cut, BubbleBlock *const dest) const;
        bool canDrop(BubbleBlock *const dragged, BubbleBlock *const dest) const;

        void onSize(wxSizeEvent &event);
        void onScrollBarThumbRelease(wxScrollWinEvent &event);

        //##void onMouseEvents(wxMouseEvent& event);
        //##void onSetFocus(wxFocusEvent& event);
        void onKillFocus(wxFocusEvent &event);
        void onMouseEnter(wxMouseEvent &event);
        void onMouseLeave(wxMouseEvent &event);
        void onMouseWheel(wxMouseEvent &event);
        void onLeftUp(wxMouseEvent &event);
        //void onLeftDown(wxMouseEvent &event);
        void onRightDown(wxMouseEvent &event);
        void onMiddleDown(wxMouseEvent &event);
        void onMiddleUp(wxMouseEvent &event);
        void onMouseMove(wxMouseEvent &event);
        void onAddBlockButtonLeftDown(wxMouseEvent &event);

    public:
        BubbleCanvas(   wxWindow* parent,
                        Bubble *const bubble,
                        wxWindowID id,
                        int defaultHScrollInc,  //These are not part of the BubbleCanvasInfo because they are
                        int defaultVScrollInc,  //application dependent, NOT target dependent.
                        //const BubbleCanvasInfo &info
                        const BubbleCanvasInfo &info //##
                    );
        BubbleCanvas(const BubbleCanvas &canvas); //##Implementar.
        virtual ~BubbleCanvas();

        //Since it seems to be impossible to hide a wxMenuItem, we need two different pop-up menus here:
        void createMenuEditForActions();
        void updateMenuEditForActionsGUI();
        inline wxMenu *getMenuEditForActions() const { return popEditForActions; }
        void createMenuEditForStartBlocks();
        void updateMenuEditForStartBlocksGUI();
        inline wxMenu *getMenuEditForStartBlocks() const { return popEditForStartBlocks; }
        void createMenuEditForExpressions();
        void updateMenuEditForExpressionsGUI();
        inline wxMenu *getMenuEditForExpressions() const { return popEditForExpressions; }
        bool popupMenu(wxMenu* menu, const wxPoint& pos = wxDefaultPosition);
        bool getEnableMenuPaste() const;
        bool getEnableMenuCut() const;
        bool getEnableMenuCopy() const;
        bool getEnableMenuDelete() const;
        bool getEnableMenuCommentUncomment() const;

        void updateBlocksTextGUI();

        void zoomParams(BubbleBlock *block);

        //void onMenuEditAddComment(wxCommandEvent& evt);
        void onCopyBlock(wxCommandEvent& evt);  //##Must use the clipboard.
        void onCutBlock(wxCommandEvent& evt);   //##Must use the clipboard.
        void onPasteBlock(wxCommandEvent& evt);   //##Must use the clipboard.
        void onDeleteBlock(wxCommandEvent& evt);
        void onCommentUncommentBlock(wxCommandEvent& evt);

        void onUndo(wxCommandEvent& evt);
        void onRedo(wxCommandEvent& evt);
        void onSelectAll(wxCommandEvent& evt);

        void onRestoreZoomTo75(wxCommandEvent& evt);
        void onRestoreZoomTo100(wxCommandEvent& evt);
        void onDecZoom(wxCommandEvent& evt);
        void onIncZoom(wxCommandEvent& evt);

        void onScrollFirstBackBlock(wxCommandEvent& evt);
        void onScrollHome(wxCommandEvent& evt);
        void onScrollHomeAbsolute(wxCommandEvent& evt);
        void onScrollEnd(wxCommandEvent& evt);
        void onScrollLastParamBlock(wxCommandEvent& evt);
        void onDecHScroll(wxCommandEvent& evt);
        void onIncHScroll(wxCommandEvent& evt);
        void onDecVScroll(wxCommandEvent& evt);
        void onIncVScroll(wxCommandEvent& evt);

        void onMoveLeft(wxCommandEvent& evt);
        void onMoveRight(wxCommandEvent& evt);
        void onMoveUp(wxCommandEvent& evt);
        void onMoveDown(wxCommandEvent& evt);

        void OnMouseEvent(wxMouseEvent& event);

        //##Ver qué funciones serán virtual al final (puede que para optimizar les saque el virtual a gran
        //parte, pero no lo sé aún):
        //##Ver si se agregan más notifiers.
        //En ese caso, habrá una función xxxNotify por cada tipo de notificación.
        //Las clases que quiera notificar otras cosas, deberán heredar del BubbleCanvasNotifier y de
        //BubbleCanvas, agregando los métodos de registración y de notificación que sean necesarios.
        //void registerZoomNotifier(IBubbleCanvasNotifier *notifier); //##Ver si paso estas cosas a referencias.

        //##Esta función es por ahora un parche cableado que va a cambiar en un addAcceleratorKey individual
        //al que llamará la clase Bubble desde loadAcceleratorTableFromFile:
        void addAcceleratorKeys();

        void zoom();

        void setZoomIndex(const unsigned int value);  //##Las funciones de zoom en realidad trabajarán con un array,
        unsigned int getZoomIndex() const;       //o algo así, de posibles valores de zoom.
        float getZoomFactor() const;
        void scrollToHomeAbsolute();
        void scrollToHome();
        void scrollToEnd();
        void setHScroll(const int value); //##Ver si se deja así...
        int getHScroll() const;
        void setVScroll(const int value);
        int getVScroll() const;
        //inline bool getScrollBarsVisible() const;
        //void setScrollBarsVisible(bool value);
        void adjustVScrollBar();
        void adjustHScrollBar();

        inline bool areBlocksEnabled() const { return blocksEnabled; }
        void enableAllBlocks(bool value);

        inline bool getVisibleLabels() const  { return visibleLabels; }
        void setVisibleLabels(bool value);

        inline Bubble* getBubble() const { return bubble; }

        inline bool isMain() const { return _main; }
        inline unsigned int getTabWidth() const { return tabWidth; }

        //##Perhaps in the future the value param may be converted into a private variable:
        inline int getAutoPanTimerInterval() const { return autoPanTimerInterval; }
        inline void setAutoPanTimerInterval(int value) { autoPanTimerInterval = value; }

        inline unsigned int getAutoPanSensibleWidth() const { return autoPanSensibleWidth; }
        inline void setAutoPanSensibleWidth(unsigned int value) { autoPanSensibleWidth = value; }
        inline unsigned int getAutoPanSensibleHeight() const { return autoPanSensibleHeight; }
        inline void setAutoPanSensibleHeight(unsigned int value) { autoPanSensibleHeight = value; }

        inline unsigned int getAutoPanHStepAbsoluteValue() const { return abs(autoPanHStep); }
        inline void setAutoPanHStepAbsoluteValue(unsigned int value) { autoPanHStep =  value; }
        inline unsigned int getAutoPanVStepAbsoluteValue() const { return abs(autoPanVStep); }
        inline void setAutoPanVStepAbsoluteValue(unsigned int value) { autoPanVStep =  value; }
        void autoPanTick();
        void startAutoPanV();
        void stopAutoPanV();
        void autoPanV(const wxPoint &cursorPos);

//        inline wxDragImage *getNonDraggableImage() const { return nonDraggableImage; }
//        inline void setNonDraggableImage(wxDragImage *value) { nonDraggableImage = value; }
//        void deleteNonDraggableImage();
        inline void setDragStartPos(const wxPoint &value) { dragStartPos = value; }
        inline const wxPoint &getDragStartPos() const { return dragStartPos; }
        inline void setDraggedBlock(BubbleBlock *const value) { draggedBlock = value; }
        inline BubbleBlock *getDraggedBlock() const { return draggedBlock; }
        inline bool getDisableWhileDragging() const { return disableWhileDragging; }
        inline void setDisableWhileDragging(bool value) { disableWhileDragging = value; }

        inline bool getFocused() const { return focused; }
        inline void setFocused(const bool value) { focused = value; }

        inline bool isSaved() const { return saved; }
        void forceSaved(const bool value);

        void setCurrentBlock(BubbleBlock *const block); //##Ver en el futuro si recordará el estado inclusive de la última selección.
        inline BubbleBlock *getCurrentBlock() const { return  currentBlock; }
        BubbleBlock *getFirstBlock() const;
        BubbleBlock *getLastBlock() const;
        BubbleBlock *getPrevBlock(BubbleBlock *block) const;
        BubbleBlock *getNextBlock(BubbleBlock *block) const;
        unsigned int getBlocksCount() const;

        void scrollHToCurrentBlock();
        void scrollVToCurrentBlock();
        void scrollToCurrentBlock();

        void selectPrevVerticalBlockAndScroll();
        void selectNextVerticalBlockAndScroll();
        void selectPrevHorizontalBlockAndScroll();
        void selectNextHorizontalBlockAndScroll();

        void showAllBlocks(const bool value);
        void showBlocks(BubbleBlock *const firstBlock, BubbleBlock *const lastBlock, const bool value);
        //void showParams(BubbleBlock *block, const bool value); //##Const pointer?

        void updateCodeAndRefresh();

        //These 2 funcions avoid the use of the keyboard while dragging blocks:
        inline void setLockBlockKeyboardEvents(bool value) { lockBlockKeyboardEvents = value; }
        inline bool getLockBlockKeyboardEvents() const { return lockBlockKeyboardEvents; }

        //From here, every method must modify the "saved" field:
        void addBlock(const BubbleBlockInfo &info, const bool showBlock = true);
        void addParam(const BubbleBlockInfo &info, BubbleParam *paramSlot, const bool showParam= true);
        void addParamDeletingParams(const BubbleBlockInfo &info, BubbleParam *paramSlot, const bool showParam);
        void addParamKeepingParams(const BubbleBlockInfo &info, BubbleParam *paramSlot, const bool showParam);

        void undo();
        void redo();
        void selectAll();

        BubbleBlock *getFirstBlockToPaste() const;
        void detachAllBlocks();
        void detachAllBlocksToPaste();
        void copyBlock(BubbleBlock *block);   //Uses the clipboard.
        void cutBlock(BubbleBlock *block, bool makeCopy);    //Uses the clipboard.
        void commentUncommentBlock(BubbleBlock *block);
        void pasteBlock(BubbleBlock *block);  //Uses the clipboard.
        void moveBlocks(BubbleBlock *srcFirstBlock, BubbleBlock *srcLastBlock, BubbleBlock *destBlock);
        void findBlock(const wxString& searchString);   //##Ver el tema del criterio de búsqueda:
                                                        //quizá necesite otros parámetros... Se me ocurre,
                                                        //por ejemplo, que pueda buscar por tipo, etc..
        bool mustRemoveInstance(BubbleBlock *block);
        void setAllInstances();
        void clearNonDeclaredInstances();
        bool varNameIsInUse(const wxString& name, BubbleBlock *sender, bool markWrongBlocks);
        void updateBlocksUsingThisVarName(  const wxString& currentName,
                                            const wxString& newName,
                                            BubbleBlock *sender);

        bool loadFromFile(const wxString& name);
        bool saveToFile(const wxString& name);
        void print();
        wxImage getCanvasCapture(); //##Return an whole image copy? Yes, by now...

        DECLARE_EVENT_TABLE()
};


class BubblePicker;
class BubbleBlockInfo
{
    //##Falta todo el "temita" de los "bloques hermanos" (2 en while, 2 ó 3 en if, N en el futuro -para switch o elseif-)...
    private:
        wxString name;
        wxString function;
        wxString dataType;
        wxArrayString code; //##Deberá ser un wxArrayString
        wxString defaultMethodName; //##Esto se va...
        wxString methodSeparator;
        wxString paramSeparator;
        wxString paramListStart;
        wxString paramListEnd;
        wxString keywords0;
        wxString keywords1;

        bool loading;

        wxString toolTip;
        wxString label;

        bool hasHorizontalParams;

        bool hasInstanceNameField;
        bool instanceNameFieldReadOnly;
        bool instanceNameFieldSorted;
        bool userMustSelectInstanceName;
        wxString instanceType;
        wxString instanceNameFieldDefaultValue;
        bool updateVariables;

        bool componentStart;
        bool blockStart;
        bool variableInit;
        bool blockIsDraggable;
        bool dontDeleteBrothers;

        bool commented;

        wxString loadAction;

        //##BubbleDataType constantFieldType;
        //##bool constantBoolDefaultValue;
        //##BubbleNumber constantNumber;
        //##int constantStringMinLen;
        //##int constantStringMaxLen;
        //##wxString constantStringDefaltValue;
        //##bool constantFieldEmpty;
        bool hasConstantField;
        wxString constantFieldDefaultValue;

        bool constantFieldEnableValidation;
        wxString constantFieldValidatorCharList;
        wxString constantFieldValidatorString;

        arrayOfParamInfo params;
        arrayOfInstances preinstatiatedObjects;

        wxColour    defaultBackgroundColour0,
                    defaultBackgroundColour1,
                    disabledBackgroundColour0,
                    disabledBackgroundColour1,
                    defaultMouseOverColour,
                    defaultErrorColour,
                    defaultEditableFieldsColour,
                    defaultSelectionBorderColour;
        wxSize originalSize;
        float childFactor;

        unsigned int nextBlockIndentationTabs;

        bool hasBlockButton;
        bool allowDragOnMethodButton;
        bool hasMethodButton;
        bool centeredMethodButton;
        //bool hasOperationButton;
        //##Future: The return button is not visible, is just to be copied in the left-block's param button:
        //bool hasReturnButton;
        bool hasAddParamsButton;

        bool _hasBlockHoverImage;
        bool _hasBlockPressedImage;
        bool _hasMethodHoverImage;
        bool _hasMethodPressedImage;

        wxImage pickerDefaultImage;
        wxImage pickerHoverImage;
        wxImage pickerPressedImage;
        wxImage pickerDisabledImage;

        wxImage blockDefaultImage;
        wxImage blockHoverImage;
        wxImage blockPressedImage;
        wxImage blockDisabledImage;

        wxImage methodDefaultImage;
        wxImage methodHoverImage;
        wxImage methodPressedImage;
        wxImage methodDisabledImage;

        wxImage returnDefaultImage;
        wxImage returnHoverImage;
        wxImage returnPressedImage;
        wxImage returnDisabledImage;

        wxImage addParamsDefaultImage;
        wxImage addParamsHoverImage;
        wxImage addParamsPressedImage;
        wxImage addParamsDisabledImage;

        wxImage removeParamsDefaultImage;
        wxImage removeParamsHoverImage;
        wxImage removeParamsPressedImage;
        wxImage removeParamsDisabledImage;

        BubbleBlockInfo *brotherBlockInfo;

        unsigned int minParamsCount;

        //##Horrible, but works nice!
        wxImage emptyDummyImage;
        wxString emptyDummyString;

    public:
        BubbleBlockInfo():
                            name(wxString("")),
                            function(wxString("")),
                            dataType(wxString("")),
                            defaultMethodName(""), //##Esto se va...
                            methodSeparator(""),
                            paramSeparator(wxString("")),
                            paramListStart(wxString("")),
                            paramListEnd(wxString("")),
                            keywords0(wxString("")),
                            keywords1(wxString("")),

                            loading(false),

                            toolTip(wxString("")),
                            label(wxString("")),

                            hasHorizontalParams(false),
                            hasInstanceNameField(false),
                            instanceNameFieldReadOnly(false),
                            instanceNameFieldSorted(false),
                            userMustSelectInstanceName(false),
                            instanceType(wxString("")),
                            instanceNameFieldDefaultValue(wxString("")),
                            updateVariables(false),

                            componentStart(false),
                            blockStart(false),
                            variableInit(false),
                            blockIsDraggable(true),
                            dontDeleteBrothers(false),
                            commented(false),
                            loadAction(wxString("load")), //##descablear

//##:
//                            constantFieldType(bdtVoid),
//                            constantBoolDefaultValue(false),
//                            constantNumber(0.0, 0.0, 0, 0.0),
//                            constantStringMinLen(0),
//                            constantStringMaxLen(0),
//                            constantStringDefaltValue(wxString("")),
//                            constantFieldEmpty(false),
                            hasConstantField(false),
                            constantFieldDefaultValue(wxString("")),
                            //##By default: floting point numbers without exponent:

                            constantFieldEnableValidation(true),
                            constantFieldValidatorCharList(wxString("0123456789.")),
                            constantFieldValidatorString(wxString("(^[0-9]+(.?[0-9]+)?$)")),

                            defaultBackgroundColour0(wxColour(0, 0, 0)),
                            defaultBackgroundColour1(wxColour(0, 0, 0)),

                            disabledBackgroundColour0(wxColour(192, 192, 192)), //##Test
                            disabledBackgroundColour1(wxColour(32, 32, 32)), //##Test

                            defaultMouseOverColour(wxColour(255, 255, 0)),
                            defaultErrorColour(wxColour(255, 0, 0)),
                            defaultEditableFieldsColour(255, 255, 255),
                            defaultSelectionBorderColour(255, 0, 0),

                            originalSize(wxDefaultSize),
                            childFactor(1.0),

                            nextBlockIndentationTabs(0),

                            hasBlockButton(false),
                            allowDragOnMethodButton(false),
                            hasMethodButton(false),
                            centeredMethodButton(false),
                            //##hasReturnButton(false),
                            hasAddParamsButton(false),

                            _hasBlockHoverImage(false),
                            _hasBlockPressedImage(false),
                            _hasMethodHoverImage(false),
                            _hasMethodPressedImage(false),

                            brotherBlockInfo(NULL),

                            minParamsCount(0),

                            //##Horrible, but works nice!
                            emptyDummyString(wxString(""))

        {
            code.Clear();
        }
        //##Ver si se necesita constructor de copia por las dudas, al menos que no haga gran cosa...

        virtual ~BubbleBlockInfo()
        {
        }

        //##Pasar a inline luego todo lo posible a la hora de optimizar:
        //##Esto tiene luego que ser parte de alguna interfaz común, para que la herede también BubbleBlock
        //(podría llamarse algo así como IBubbleBlockFeatures):
        void setName(const wxString& value); //##Estas funciones son comunes a BubbleBlock, BubbleBlockNavigator y a BubbleButton: ¡ABSTRAER!
        const wxString &getName() const;
        inline void setFunction(const wxString& value) { function = value; }
        inline const wxString &getFunction() const { return function; }
        inline void setDataType(const wxString& value) { dataType = value; }
        inline const wxString &getDataType() const { return dataType; }

        //##Por ahora se hace así, pero va a evolucionar mucho todavía:
        void setCode(const wxArrayString& value);
        const wxArrayString &getCode() const;

        inline const wxString &getMethodSeparator() const { return methodSeparator; }
        inline void setMethodSeparator(const wxString &value) { methodSeparator = value; }

        inline const wxString &getDefaultMethodName() const { return defaultMethodName; } //##Cambiará cuando se agregue el soporte completo para métodos...
        inline void setDefaultMethodName(const wxString &value) { defaultMethodName = value; } //##

        inline const wxString &getParamSeparator() const { return paramSeparator; }
        inline void setParamSeparator(const wxString &value) { paramSeparator = value; }

        inline const wxString &getParamListStart() const { return paramListStart; }
        inline void setParamListStart(const wxString &value) { paramListStart = value; }

        inline const wxString &getParamListEnd() const { return paramListEnd; }
        inline void setParamListEnd(const wxString &value) { paramListEnd = value; }

        inline const wxString &getKeywords0() const { return keywords0; }
        inline void setKeywords0(const wxString &value) { keywords0 = value; }

        inline const wxString &getKeywords1() const { return keywords1; }
        inline void setKeywords1(const wxString &value) { keywords1 = value; }

        inline const bool getLoading() const { return loading; }
        inline void setLoading(const bool value) { loading = value; }

        inline const wxString &getToolTip() const { return toolTip; }
        inline void setToolTip(const wxString &value) { toolTip = value; }

        inline const wxString &getLabel() const { return label; }
        inline void setLabel(const wxString &value) { label = value; }

        inline void setHasHorizontalParams(bool value) { hasHorizontalParams = value; }
        inline bool getHasHorizontalParams() const { return hasHorizontalParams; }

        inline void setOriginalSize(const wxSize &value) { originalSize = value; }
        inline const wxSize &getOriginalSize() const { return originalSize; }

        inline void setChildFactor(const float value) { childFactor = value; }
        inline const float getChildFactor() const { return childFactor; }

        inline void setDefaultBackgroundColour0(const wxColour& colour) { defaultBackgroundColour0 = colour; }
        inline wxColour getDefaultBackgroundColour0() const { return defaultBackgroundColour0; }
        inline void setDefaultBackgroundColour1(const wxColour& colour) { defaultBackgroundColour1 = colour; }
        inline wxColour getDefaultBackgroundColour1() const { return defaultBackgroundColour1; }

        inline void setDisabledBackgroundColour0(const wxColour& colour) { disabledBackgroundColour0 = colour; }
        inline wxColour getDisabledBackgroundColour0() const { return disabledBackgroundColour0; }
        inline void setDisabledBackgroundColour1(const wxColour& colour) { disabledBackgroundColour1 = colour; }
        inline wxColour getDisabledBackgroundColour1() const { return disabledBackgroundColour1; }

        inline void setDefaultMouseOverColour(const wxColour& colour) { defaultMouseOverColour = colour; }
        inline wxColour getDefaultMouseOverColour() const { return defaultMouseOverColour; }

        inline void setDefaultErrorColour(const wxColour& colour) { defaultErrorColour = colour; }
        inline wxColour getDefaultErrorColour() const { return defaultErrorColour; }

        inline void setDefaultEditableFieldsColour(const wxColour& colour) { defaultEditableFieldsColour = colour; }
        inline wxColour getDefaultEditableFieldsColour() const { return defaultEditableFieldsColour; }

        inline void setDefaultSelectionBorderColour(const wxColour& colour) { defaultSelectionBorderColour = colour; }
        inline wxColour getDefaultSelectionBorderColour() const { return defaultSelectionBorderColour; }

        inline void setHasBlockButton(const bool value) { hasBlockButton = value; }
        inline bool getHasBlockButton() const { return hasBlockButton; }
        inline void setAllowDragOnMethodButton(const bool value) { allowDragOnMethodButton = value; }
        inline bool getAllowDragOnMethodButton() const { return allowDragOnMethodButton; }
        inline void setHasMethodButton(const bool value) { hasMethodButton = value; }
        inline bool getHasMethodButton() const { return hasMethodButton; }
        inline void setCenteredMethodButton(const bool value) { centeredMethodButton = value; }
        inline bool getCenteredMethodButton() const { return centeredMethodButton; }

        //void setHasOperationButton(const bool value);
        //bool getHasOperationButton() const;
        //##void setHasReturnButton(const bool value);
        //##bool getHasReturnButton() const;
        void setHasAddParamsButton(const bool value);
        bool getHasAddParamsButton() const;

        bool hasBlockHoverImage() const;
        bool hasBlockPressedImage() const;
        bool hasMethodHoverImage() const;
        bool hasMethodPressedImage() const;

        void setPickerDefaultImage(const wxImage& image);
        const wxImage &getPickerDefaultImage() const;
        void setPickerHoverImage(const wxImage& image);
        const wxImage &getPickerHoverImage() const;
        void setPickerPressedImage(const wxImage& image);
        const wxImage &getPickerPressedImage() const;
        void setPickerDisabledImage(const wxImage& image);
        const wxImage &getPickerDisabledImage() const;

        void setBlockDefaultImage(const wxImage& image);
        const wxImage &getBlockDefaultImage() const;
        void setBlockHoverImage(const wxImage& image);
        const wxImage &getBlockHoverImage() const;
        void setBlockPressedImage(const wxImage& image);
        const wxImage &getBlockPressedImage() const;
        void setBlockDisabledImage(const wxImage& image);
        const wxImage &getBlockDisabledImage() const;

        void setMethodDefaultImage(const wxImage& image);
        const wxImage &getMethodDefaultImage() const;
        void setMethodHoverImage(const wxImage& image);
        const wxImage &getMethodHoverImage() const;
        void setMethodPressedImage(const wxImage& image);
        const wxImage &getMethodPressedImage() const;
        void setMethodDisabledImage(const wxImage& image);
        const wxImage &getMethodDisabledImage() const;

        //void setOperationDefaultImage(const wxImage& image);
        //const wxImage &getOperationDefaultImage() const;
        //void setOperationHoverImage(const wxImage& image);
        //const wxImage &getOperationHoverImage() const;
        //void setOperationPressedImage(const wxImage& image);
        //const wxImage &getOperationPressedImage() const;
        //void setOperationDisabledImage(const wxImage& image);
        //const wxImage &getOperationDisabledImage() const;

        //##The return button is not visible, is just to be copied in the left-block's param button:
        void setReturnDefaultImage(const wxImage& image);
        const wxImage &getReturnDefaultImage() const;
        void setReturnHoverImage(const wxImage& image);
        const wxImage &getReturnHoverImage() const;
        void setReturnPressedImage(const wxImage& image);
        const wxImage &getReturnPressedImage() const;
        void setReturnDisabledImage(const wxImage& image);
        const wxImage &getReturnDisabledImage() const;

        void setAddParamsDefaultImage(const wxImage& image);
        const wxImage &getAddParamsDefaultImage() const;
        void setAddParamsHoverImage(const wxImage& image);
        const wxImage &getAddParamsHoverImage() const;
        void setAddParamsPressedImage(const wxImage& image);
        const wxImage &getAddParamsPressedImage() const;
        void setAddParamsDisabledImage(const wxImage& image);
        const wxImage &getAddParamsDisabledImage() const;

        void setRemoveParamsDefaultImage(const wxImage& image);
        const wxImage &getRemoveParamsDefaultImage() const;
        void setRemoveParamsHoverImage(const wxImage& image);
        const wxImage &getRemoveParamsHoverImage() const;
        void setRemoveParamsPressedImage(const wxImage& image);
        const wxImage &getRemoveParamsPressedImage() const;
        void setRemoveParamsDisabledImage(const wxImage& image);
        const wxImage &getRemoveParamsDisabledImage() const;

        void setInstanceNameFieldDefaultValue(const wxString& value);
        const wxString &getInstanceNameFieldDefaultValue() const;
        inline void setHasInstanceNameField(bool value) { hasInstanceNameField = value; }
        inline bool getHasInstanceNameField() const { return hasInstanceNameField; }
        inline void setInstanceNameFieldSorted(bool value) { instanceNameFieldSorted = value; }
        inline bool getInstanceNameFieldSorted() const { return instanceNameFieldSorted; }
        inline void setUserMustSelectInstanceName(bool value) { userMustSelectInstanceName = value; }
        inline bool getUserMustSelectInstanceName() const { return userMustSelectInstanceName; }
        void setInstanceType(const wxString &value) { instanceType = value; }
        const wxString &getInstanceType() const { return instanceType; }
        inline void setUpdateVariables(bool value) { updateVariables = value; }
        inline bool getUpdateVariables() const { return updateVariables; }

        inline void setComponentStart(const bool value) { componentStart = value; }
        inline const bool getComponentStart() const { return componentStart; }
        inline void setBlockStart(const bool value) { blockStart = value; }
        inline const bool getBlockStart() const { return blockStart; }
        inline void setVariableInit(const bool value) { variableInit = value; }
        inline const bool getVariableInit() const { return variableInit; }
        inline void setIsDraggable(const bool value) { blockIsDraggable = value; }
        inline const bool getIsDraggable() const { return blockIsDraggable; }
        inline void setDontDeleteBrothers(const bool value) { dontDeleteBrothers = value; }
        inline const bool getDontDeleteBrothers() const { return dontDeleteBrothers; }

        inline void setCommented(bool value) { commented = value; }
        inline bool getCommented() const { return commented; }

        void setLoadAction(const wxString &value) { loadAction = value; }
        inline const wxString & getLoadAction() const { return loadAction; }

        inline void setHasConstantField(bool value) { hasConstantField = value; }
        inline bool getHasConstantField() const { return hasConstantField; }

        inline bool getConstantFieldEnableValidation() const { return constantFieldEnableValidation; };
        inline void setConstantFieldEnableValidation(bool value) { constantFieldEnableValidation = value; };
        inline void setConstantFieldValidatorCharList(const wxString &value) { constantFieldValidatorCharList = value; }
        inline const wxString & getConstantFieldValidatorCharList() const { return constantFieldValidatorCharList; }
        inline void setConstantFieldValidatorString(const wxString &value) { constantFieldValidatorString = value; }
        inline const wxString & getConstantFieldValidatorString() const { return constantFieldValidatorString; }
        inline void setConstantFieldDefaultValue(const wxString &value) { constantFieldDefaultValue = value;}
        inline const wxString & getConstantFieldDefaultValue() const { return constantFieldDefaultValue; }

//##Ver si todo esto finalmente vuela:
//        inline BubbleDataType getConstantFieldType() const { return constantFieldType; }
//        inline void setConstantFieldType(const BubbleDataType value) { constantFieldType = value; }
//        inline void setConstantField( const bool defaultValue)
//        {
//            constantBoolDefaultValue = defaultValue;
//            constantFieldType = bdtBool;
//        }
//        inline bool getConstantBoolDefaultValue() const {return constantBoolDefaultValue; }
//        inline void setConstantField( const BubbleNumber &value)
//        {
//            constantNumber = value;
//            constantFieldType = bdtNumber;
//        }
//        inline const BubbleNumber &getConstantNumber() const { return constantNumber; }
//        inline void setConstantField(int minLen,
//                              int maxLen,
//                              const wxString &defaultValue)
//        {
//            constantStringMinLen = minLen;
//            constantStringMaxLen = maxLen;
//            constantStringDefaltValue = defaultValue;
//            constantFieldType = bdtText;
//        }
//        inline int getConstantStringMinLen() const { return constantStringMinLen; }
//        inline int getConstantStringMaxLen() const { return constantStringMaxLen; }
//        inline const wxString & getConstantStringDefaltValue() const { return constantStringDefaltValue; }
//        inline void setConstantFieldEmpty( const bool value) { constantFieldEmpty = value;}
//        inline bool getConstantFieldEmpty() const {return constantFieldEmpty; }

        void addParam(  const wxString& name,
                        const wxString& dataType,
                        const wxImage& notAssignedImage,
                        const wxImage& defaultImage,
                        const wxImage& hoverImage,
                        const wxImage& pressedImage,
                        const wxImage& disabledImage
                     );
        wxString getParamName(const unsigned int index) const;
        wxString getParamDataType(const unsigned int index) const;
        const wxImage &getParamNotAssignedImage(const unsigned int index) const;
        const wxImage &getParamDefaultImage(const unsigned int index) const; //##Ver si retornar el wxImage se puede mejorar
        const wxImage &getParamHoverImage(const unsigned int index) const; //##Ver si retornar el wxImage se puede mejorar
        const wxImage &getParamPressedImage(const unsigned int index) const; //##Ver si retornar el wxImage se puede mejorar
        const wxImage &getParamDisabledImage(const unsigned int index) const; //##Ver si retornar el wxImage se puede mejorar
        unsigned int getParamsCount() const;
        inline void setMinParamsCount(const unsigned int value) { minParamsCount = value; }
        inline unsigned int getMinParamsCount() const { return minParamsCount; }

        void setBrother(BubbleBlockInfo *const brother);
        BubbleBlockInfo *const getBrother() const;

        void setNextBlockIndentationTabs(const unsigned int value);
        int getNextBlockIndentationTabs() const;
};


class BubbleBlock : public BubblePanel//##wxControl//##wxWindow//##BubblePanel
{
    private:
        wxWindowID id;
        BubbleCanvas *parent;

        bool enabled;
        bool dragging;
        bool paramsVisible;
        bool commented;
        bool selected;
        unsigned int selectionBorderWidth;

        wxString name;
        wxString function;
        wxString dataType;
        bool componentStart;
        bool blockStart;
        bool variableInit;
        bool blockIsDraggable;
        bool dontDeleteBrothers;
        wxString loadAction;
        wxArrayString code; //##Deberá ser un wxArrayString
        wxString defaultMethodName; //##Esto se va...
        wxString methodSeparator;
        wxString paramSeparator;
        wxString paramListStart;
        wxString paramListEnd;

        BubbleBlock *backBlock;
        BubbleParam *backParamSlot;

        BubbleButton *blockButton;
        BubbleButton *methodButton;
        bool allowDragOnMethodButton;
        bool centeredMethodButton;
        //BubbleButton *operationButton;
        //##BubbleButton *returnButton;
        BubbleButton *addParamsButton;
        BubbleButton *removeParamsButton;
        unsigned int minParamsCount;

        bool constantFieldEnableValidation;
        wxString prevInstanceName;
        wxTextCtrl *constantField;
        wxStaticText *constantFieldLabel;
        wxRegEx constantFieldValidator;
        wxString constantFieldValidatorCharList;
        BubbleCombo *instanceNameField;
        bool instanceNameFieldReadOnly;
        bool instanceNameFieldSorted;
        bool userMustSelectInstanceName;
        wxString instanceType;
        bool updateVariables;

        bool hasConstantField;
        wxString constantFieldDefaultValue;
//        wxComboBox *constantField;

//##
//        BubbleDataType constantFieldType;
//        bool constantBoolDefaultValue;
//        BubbleNumber constantNumber;
//        int constantStringMinLen;
//        int constantStringMaxLen;
//        wxString constantStringDefaltValue;

        bool hasHorizontalParams;
        arrayOfParams params; //##Ver si esto no tendría que llamarse "paramSlots"

        unsigned int nextBlockIndentationTabs;

        BubbleBlockInfo *brotherBlockInfo;
        BubbleBlock *prevBrotherBlock;
        BubbleBlock *nextBrotherBlock;
        BubbleBlock *linkBlock;

        wxSize originalSize;
        wxSize realSize;
        float childFactor;
        wxColour    defaultBackgroundColour0,
                    defaultBackgroundColour1,
                    disabledBackgroundColour0,
                    disabledBackgroundColour1,
                    defaultMouseOverColour,
                    defaultErrorColour,
                    defaultEditableFieldsColour,
                    defaultSelectionBorderColour;

        wxPoint positionBeforeDragging;
        wxPoint dragStartPos;

        void setupButton(BubbleButton *button,
                         const wxImage &defaultImage, //##¿Optimizar?
                         const wxImage &hoverImage,
                         const wxImage &pressedImage,
                         const wxImage &disabledImage);

    protected:
        inline void setDragging(bool value) { dragging = value; }
//##        inline bool getDragging() const { return dragging; }

        void onLeftUp(wxMouseEvent& event);
        void onLeftDown(wxMouseEvent& event);
        void onRightDown(wxMouseEvent& event);
        void onMouseEnter(wxMouseEvent& event);
        //void OnMouseEvent(wxMouseEvent& event);

        void onMethodButtonLeftDown(wxMouseEvent& event);

        void onConstantFieldLabelMouseEnter(wxMouseEvent& event);
        void onConstantFieldLabelMouseLeave(wxMouseEvent& event);

        //void onClick(wxCommandEvent& event);
        void onSize(wxSizeEvent& event);
        void onPaint(wxPaintEvent& event);
        void onEraseBackground(wxEraseEvent & event);

        void onMethodLeftDown(wxMouseEvent& event);
        void onAddParamLeftDown(wxMouseEvent& event);
        void onAddParamsButtonLeftDown(wxMouseEvent& event);
        void onRemoveParamsButtonLeftDown(wxMouseEvent& event);

        void onClickInstanceNameField(wxCommandEvent &event);
        void onInstanceNameSelected(wxCommandEvent &event);
        void onConstantFieldTextUpdated(wxCommandEvent &event);

        wxFont getFontConstantFields() const;
        wxFont getFontEditableFields() const;

        wxPoint getBlockButtonPos() const;
        wxSize getBlockButtonSize() const;

        wxPoint getMethodButtonPos() const;
        wxSize getMethodButtonSize() const;

        //##wxSize getReturnButtonSize() const;
        //##wxPoint getReturnButtonPos() const;

        wxSize getAddParamsButtonSize() const;
        wxPoint getAddParamsButtonPos() const;
        wxPoint getRemoveParamsButtonPos() const;

        wxSize getConstantFieldSize() const;
        wxPoint getConstantFieldLabelPos() const;
        wxPoint getConstantFieldPos() const;

        wxSize getInstanceNameFieldSize() const;
        wxPoint getInstanceNameFieldPos() const;

        wxSize getParamSize() const;

    public:
        BubbleBlock(BubbleCanvas *parent,
                    wxWindowID id,
                    const wxPoint &position,
                    const wxSize &size,
                    const BubbleBlockInfo &info);
        //##El constructor de copia va a ser necesario para trabajar con los arrays dinámicos:
        //##Ver si necesito también un operador = y ver qué hago con el constructor...
        //  ##Esta última nota es válida para todas las clases con constructor de copia: Ver bien qué haré...
        //##No sé si está bien que lo implemente así, sin nada, porque en verdad el wxArray parece necesitarlo,
        //y el wxPanel está declarado con DECLARE_DYNAMIC_CLASS_NO_COPY...
        BubbleBlock(const BubbleBlock &block);

        //##:Luego se verá qué se hace:
        virtual ~BubbleBlock();

        BubbleCanvas* getCanvas() const;

        inline void setBackBlock(BubbleBlock *value) { backBlock = value; }
        inline BubbleBlock *getBackBlock() const { return backBlock; }
        BubbleBlock *getFirstBackBlock() const;
        BubbleBlock *getLastParamFirstBlock() const;

        void enableActionButtons(BubbleBlock *block);

        void select();
        void unselect();

        inline bool getDragging() const { return dragging; } //##Esto va a protected...
        void beginDrag(const wxPoint &startPos);
        void endDrag(bool drop);
        void moveWithParams(int x, int y);
        void moveParamSlot(const unsigned int index, int x, int y);
        void moveWithBrothers(int x, int y);

        inline bool areParamsVisible() const { return paramsVisible; }
        void showParams(bool value);

        void showChildren(bool value);
        void showParamsChildren(bool value);

        void enableChildren(bool value, bool fullEnableDisable);
        void enableParamsChildren(bool value, bool fullEnableDisable = true);
        bool Enable(bool value = true, bool fullEnableDisable = true);
        bool Disable();
        void comment(bool value);
        inline void setCommented(bool value) { commented = value; } //This just changes the variable commented.
        void commentWithBrothers(bool value);
        inline bool isCommented() const { return commented; }

        void showLabel(bool value);
        void showParamsLabels(bool value);

        void updateTextGUI();
        void updateParamsTextGUI();

        void resizeConstantField();
        inline void setOriginalSize(const wxSize &value) { originalSize = value; }
        inline const wxSize &getOriginalSize() const { return originalSize; }

        inline void setRealSize(const wxSize &value) { realSize = value; }
        inline const wxSize &getRealSize() const { return realSize; }

        void changeAllBackBlocksRealSize(wxSize delta, bool changeThis);

        inline void setChildFactor(const float value) { childFactor = value; }
        inline const float getChildFactor() const { return childFactor; }

        void setName(const wxString& value);
        inline const wxString &getName() const { return name; }

        inline  void setFunction(const wxString& value) { function = value; }
        inline const wxString &getFunction() const { return function; }

        inline  void setComponentStart(const bool value) { componentStart = value; }
        inline const bool getComponentStart() const { return componentStart; }
        inline  void setBlockStart(const bool value) { blockStart = value; }
        inline const bool getBlockStart() const { return blockStart; }
        inline void setVariableInit(const bool value) { variableInit = value; }
        inline const bool getVariableInit() const { return variableInit; }
        inline  void setIsDraggable(const bool value) { blockIsDraggable = value; }
        inline const bool getIsDraggable() const { return blockIsDraggable; }
        inline  void setDontDeleteBrothers(const bool value) { dontDeleteBrothers = value; }
        inline const bool getDontDeleteBrothers() const { return dontDeleteBrothers; }

        void setLoadAction(const wxString &value) { loadAction = value; }
        inline const wxString & getLoadAction() const { return loadAction; }

        inline void setDataType(const wxString &value) { dataType = value; }
        inline const wxString &getDataType() const { return dataType; }

        inline void setCode(const wxArrayString& value) { code = value; }
        wxArrayString getCode() const;

        inline const wxString &getMethodSeparator() const { return methodSeparator; }
        inline void setMethodSeparator(const wxString &value) { methodSeparator = value; }

        inline const wxString &getDefaultMethodName() const { return defaultMethodName; } //##Cambiará cuando se agregue el soporte completo para métodos...
        inline void setDefaultMethodName(const wxString &value) { defaultMethodName = value; } //##

        inline const wxString &getParamSeparator() const { return paramSeparator; }
        inline void setParamSeparator(const wxString &value) { paramSeparator = value; }

        inline const wxString &getParamListStart() const { return paramListStart; }
        inline void setParamListStart(const wxString &value) { paramListStart = value; }

        inline const wxString &getParamListEnd() const { return paramListEnd; }
        inline void setParamListEnd(const wxString &value) { paramListEnd = value; }

        unsigned int getIndentation() const;

        inline void setHasHorizontalParams(bool value) { hasHorizontalParams = value; }
        inline bool getHasHorizontalParams() const { return hasHorizontalParams; }

        inline void setDefaultBackgroundColour0(const wxColour& colour) { defaultBackgroundColour0 = colour; }
        inline wxColour getDefaultBackgroundColour0() const { return defaultBackgroundColour0; }
        inline void setDefaultBackgroundColour1(const wxColour& colour) { defaultBackgroundColour1 = colour; }
        inline wxColour getDefaultBackgroundColour1() const { return defaultBackgroundColour1; }

        inline void setDisabledBackgroundColour0(const wxColour& colour) { disabledBackgroundColour0 = colour; }
        inline wxColour getDisabledBackgroundColour0() const { return disabledBackgroundColour0; }
        inline void setDisabledBackgroundColour1(const wxColour& colour) { disabledBackgroundColour1 = colour; }
        inline wxColour getDisabledBackgroundColour1() const { return disabledBackgroundColour1; }

        inline void setDefaultMouseOverColour(const wxColour& colour) { defaultMouseOverColour = colour; }
        inline wxColour getDefaultMouseOverColour() const { return defaultMouseOverColour; }

        inline void setDefaultErrorColour(const wxColour& colour) { defaultErrorColour = colour; }
        inline wxColour getDefaultErrorColour() const { return defaultErrorColour; }

        inline void setDefaultEditableFieldsColour(const wxColour& colour) { defaultEditableFieldsColour = colour; }
        inline wxColour getDefaultEditableFieldsColour() const { return defaultEditableFieldsColour; }

        inline void setDefaultSelectionBorderColour(const wxColour& colour) { defaultSelectionBorderColour = colour; }
        inline wxColour getDefaultSelectionBorderColour() const { return defaultSelectionBorderColour; }

        void setBlockButton(const wxImage& defaultImage,    //The blockButton can be static (but must have
                            const wxImage& disabledImage);  //disabled image.
        void setBlockButton(const wxImage& defaultImage,
                            const wxImage& hoverImage,
                            const wxImage& pressedImage,
                            const wxImage& disabledImage);
        inline BubbleButton * getBlockButton() const { return blockButton; }

        inline void setAllowDragOnMethodButton(const bool value) {  allowDragOnMethodButton = value; }
        inline bool getAllowDragOnMethodButton() const { return allowDragOnMethodButton; }
        void setMethodButton(const wxImage& defaultImage,   //The methodButton must have all it's images.
                             const wxImage& hoverImage,
                             const wxImage& pressedImage,
                             const wxImage& disabledImage);
        //void setOperationButton(const wxImage& defaultImage,   //The operationButton must have all it's images.
        //                        const wxImage& hoverImage,
        //                        const wxImage& pressedImage,
        //                        const wxImage& disabledImage);

        void updateInstanceNameList();
        void addVariableWithInstanceName();
        bool setInstanceNameField(const wxString& value);
        inline bool getHasInstanceNameField() const { return (instanceNameField != NULL); }
        wxString getInstanceNameField() const;
        inline void setInstanceNameFieldSorted(bool value) { instanceNameFieldSorted = value; }
        inline bool getInstanceNameFieldSorted() const { return instanceNameFieldSorted; }
        inline void setUserMustSelectInstanceName(bool value) { userMustSelectInstanceName = value; }
        inline bool getUserMustSelectInstanceName() const { return userMustSelectInstanceName; }
        void setInstanceType(const wxString &value) { instanceType = value; }
        const wxString &getInstanceType() const { return instanceType; }
        inline void setUpdateVariables(bool value) { updateVariables = value; }
        inline bool getUpdateVariables() const { return updateVariables; }
        //bool validateVariableName(const wxString &value, const bool changeBackground = true);

        inline void setHasConstantField(bool value) { hasConstantField = value; }
        inline bool getHasConstantField() const { return hasConstantField; }
        inline void setConstantFieldDefaultValue(const wxString &value) { constantFieldDefaultValue = value;}
        inline const wxString &getConstantFieldDefaultValue() const { return constantFieldDefaultValue; }
        wxString getConstantFieldValue() const;
        void updateVarNameValidation(   const wxString& currentName,
                                        const wxString& newName,
                                        BubbleBlock *sender);
        void validateParamsVarNames(const wxString& currentName,
                                    const wxString& newName,
                                    BubbleBlock *sender);
        bool varNameIsValid(const wxString& value);
        bool validateConstantField(bool testVarNameInUse);
        void setConstantFieldError(bool value);
        void setInstanceNameFieldError(bool value);

        inline BubbleBlockInfo *const getBrotherInfo() const { return brotherBlockInfo; }
        inline void setPrevBrother(BubbleBlock *const brother) { prevBrotherBlock = brother; }
        inline BubbleBlock *const getPrevBrother() const { return prevBrotherBlock; }
        inline void setNextBrother(BubbleBlock *const brother) { nextBrotherBlock = brother; }
        inline BubbleBlock *const getNextBrother() const { return nextBrotherBlock; }
        BubbleBlock *getFirstBrother() const;
        BubbleBlock *getLastBrother() const;
        void showBrothers(bool value);

        inline void setNextBlockIndentationTabs(const unsigned int value) { nextBlockIndentationTabs = value; }
        inline int getNextBlockIndentationTabs() const { return nextBlockIndentationTabs; }

        wxSize getTotalParamsSize() const;
        wxPoint getParamPos(int index) const;

        bool getHasAddParamsButton() const;
        unsigned int getParamsCount() const;
        inline void setMinParamsCount(const unsigned int value) { minParamsCount = value; }
        inline unsigned int getMinParamsCount() const { return minParamsCount; }
        BubbleBlock *getParamFirstBlock(const unsigned int index) const;
        int getParamIndex(BubbleBlock *block) const;
        void addParamSlot(  const wxString& name,
                            const wxString& dataType,
                            const wxImage &assignedImage,
                            const wxImage &notAssignedImage,
                            const wxImage &defaultImage,
                            const wxImage &hoverImage,
                            const wxImage &pressedImage,
                            const wxImage &disabledImage
                          );
        BubbleParam *getParamSlot(const unsigned int index) const;
        void removeParamSlot(const unsigned int index);
        void removeParams();
        BubbleBlock *getParamSlotFirstBlock(const unsigned int index) const;
        inline void setBackParamSlot(BubbleParam *value) { backParamSlot = value; }
        inline BubbleParam *getBackParamSlot() const { return backParamSlot; }

        wxString getAttributes() const;
        //void SetBackgroundColourAndRefresh(const wxColour& colour); //##

        DECLARE_EVENT_TABLE()
};


class BubbleComment : public BubbleBlock
{
    BubbleComment(  BubbleCanvas *parent,
                    wxWindowID id,
                    const wxPoint &position,
                    const wxSize &size,
                    const BubbleBlockInfo &info) : BubbleBlock( parent,
                                                                id,
                                                                position,
                                                                size,
                                                                info
                                                              )
    {
    }

    //##Ver comentarios al respecto del constructor de copia en BubbleBlock:
    BubbleComment(const BubbleComment &comment);

    //##:Luego se verá qué se hace:
    virtual ~BubbleComment() { }
};

#endif
