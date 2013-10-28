#ifndef MainFrame__h
#define MainFrame__h

//##Ver cuáles se pueden pasar al cpp, para que no molesten acá:
//##Sólo dejar las que se necesiten en la aplicación final:
#include <wx/frame.h>
#include <wx/aui/aui.h>
#include <wx/treectrl.h>
#include <wx/artprov.h>
#include <wx/wxhtml.h>

#include "Bubble.h"
//##Ver si alguno se puede ir:
#include "BubbleEditor.h"
#include "BubbleVariablesManager.h" //##Ver si hay que sacar cosas del header para hacer los include sólo acá:
#include "BubbleComponentsManager.h"
#include "Messages.h"
#include "Terminal.h"

#include "MinibloqProperties.h"
#include "BlockEditorProperties.h"
#include "TextEditorProperties.h"
#include "ComponentProperties.h"
#include "TerminalProperties.h"


//////////////////////////////////////////////////////////////////////
// MainFrame
//////////////////////////////////////////////////////////////////////
class MainFrame : public wxFrame, public IBubbleNotifier
{
    private:
        //##Filtrar esto, hay cosas de más:
        enum
        {
//##Ver desde acá hasta...
            ID_CreateTree = wxID_HIGHEST+1,
            //ID_CreateHTML,
            //ID_CreateNotebook, //##
            //ID_GridContent,
            //ID_TextContent,
            //ID_TreeContent,
            //ID_HTMLContent,
            //ID_NotebookContent,
            ID_AllowFloating,
            ID_AllowActivePane,
            ID_TransparentHint,
            ID_VenetianBlindsHint,
            ID_RectangleHint,
            ID_NoHint,
            ID_HintFade,
            ID_NoVenetianFade,
            ID_TransparentDrag,
            ID_NoGradient,
            ID_VerticalGradient,
            ID_HorizontalGradient,
            ID_CustomizeToolbar,

            ID_NotebookNoCloseButton,
            ID_NotebookCloseButton,
            ID_NotebookCloseButtonAll,
            ID_NotebookCloseButtonActive,
            ID_NotebookAllowTabMove,
            ID_NotebookAllowTabExternalMove,
            ID_NotebookAllowTabSplit,
            ID_NotebookWindowList,
            ID_NotebookScrollButtons,
            ID_NotebookTabFixedWidth,
            ID_NotebookArtGloss,
            ID_NotebookArtSimple,
            ID_NotebookAlignTop,
            ID_NotebookAlignBottom,
//##...acá

            ID_Notebook,
            ID_Hardware,
            ID_LocalVariables,
            ID_HelpAndResourceCenter,
            ID_Properties,
            ID_Messages,
            ID_Terminal,
            ID_Components,

            ID_Help,
            ID_About,
            ID_License,

            ID_MinibloqProperties,
            ID_ComponentProperties,
            ID_TerminalProperties,
            ID_BlockEditorProperties,
            ID_TextEditorProperties,

            //##Estos son definitivos:
            ID_QuickFileCreate,
            ID_QuickFileAdd,
            ID_QuickFileOpen,
            ID_QuickFileSaveAll,
            ID_QuickComponentRun,
            ID_QuickComponentDeploy,
            ID_QuickViewTerminal,
            ID_QuickViewHelpAndResourceCenter,

            ID_MenuFile,
            ID_MenuFileCreate,
            ID_MenuFileCreateComponent,
            ID_MenuFileCreateBlock,
            ID_MenuFileAdd,
            ID_MenuFileRemove,
            ID_MenuFileOpen,
            ID_MenuFileExamples,
            ID_MenuFileSave,
            ID_MenuFileSaveAs,
            ID_MenuFileSaveAll,
            ID_MenuFileClose,
            ID_MenuFileCloseAll,
            ID_MenuFilePrint,
            ID_MenuFileExportAsImageToClipboard,

            ID_MenuEdit,
            ID_MenuEditUndo,
            ID_MenuEditRedo,
            ID_MenuEditSelectAll,
            ID_MenuEditCopy,
            ID_MenuEditCut,
            ID_MenuEditPaste,
            ID_MenuEditDelete,
            ID_MenuCommentUncomment,
            ID_MenuEditFind,
            ID_MenuEditFindNext,

            ID_MenuComponent,
            ID_MenuComponentRun,
            ID_MenuComponentBuild,
            ID_MenuComponentDeploy,
            ID_MenuComponentGenerateCode,
            ID_MenuComponentMakePortable,
            ID_MenuComponentOpenFolder,

            ID_MenuView,
            ID_MenuViewHelpAndResourceCenter,
            ID_MenuViewLabels,
            ID_MenuViewComponents,
            ID_MenuViewHardware,
            ID_MenuViewLocalVariables,
            ID_MenuViewProperties,
            ID_MenuViewMessages,
            ID_MenuViewTerminal,
            ID_MenuViewQuickToolbar,
            ID_MenuViewGeneratedCode,
            ID_MenuViewFullScreen,
            ID_MenuViewPrevView,
            ID_MenuViewNextView,

            ID_QuickZoom,
            ID_QuickZoom10,
            ID_QuickZoom25,
            ID_QuickZoom50,
            ID_QuickZoom75,
            ID_QuickZoom100,
            ID_QuickZoom125,
            ID_QuickZoom150,
            ID_QuickZoom175,
            ID_QuickZoom200,
            ID_QuickZoomViewAll,

            ID_ShortCut_DecZoom,
            ID_ShortCut_IncZoom,

            ID_ShortCut_CloseCurrentWindow,
            ID_ShortCut_CloseMessages,

            ID_HardwareManagerPopUpPort,
            ID_HardwareManagerPopUpBoard
        };

/*##Future:
        wxAuiToolBar*   toolMainMenu;
        wxAuiToolBar*   toolFile;
        wxAuiToolBar*   toolEdit;
        wxAuiToolBar*   toolComponent;
        wxAuiToolBar*   toolView;
*/
        wxAuiManager auiManager;
        long m_notebook_style;
        //long m_notebook_theme;

        //wxString initialFile;

        //##Bubble related:
        Bubble bubble;

        wxLocale& locale;
        wxString languagePath;
        wxString lastAddedLocaleCatalog;

        TerminalCommManager *commManager;
        SingleTerminalGUI *singleTerminalGUI;
        SplitTerminalGUI *splitTerminalGUI;

        wxAuiNotebook *notebook;
        wxAuiNotebook *helpAndResourceCenter;
        wxAuiNotebook *properties;
        wxAuiNotebook *terminal;

        BubbleHardwareManager *hardware;
        BubbleVariablesManager *localVariables;
        Messages *messages;
        BubbleComponentsManager *components;

        wxAuiToolBarItem *toolZoom;
        wxAuiToolBar *toolQuick;

        wxHtmlWindow *help;
        wxHtmlWindow *about;
        wxHtmlWindow *license;

        MinibloqProperties *minibloqProperties;
        ComponentProperties *componentProperties;
        TerminalProperties *terminalProperties;
        TextEditorProperties *textEditorProperties;
        BlockEditorProperties *blockEditorProperties;

        wxAuiToolBar *toolMainMenu;

        wxMenu *popFile;
        wxMenu *popFileCreate;
        wxMenu *popEdit;
        wxMenu *popComponent;
        wxMenu *popView;
        wxMenu *popZoom;

        wxMenuItem *menuFileCreate;
        wxMenuItem *menuFileOpen;
        wxMenuItem *menuFileExamples;
        wxMenuItem *menuFileSave;
        wxMenuItem *menuFileSaveAs;
        wxMenuItem *menuFileExit;
        wxMenuItem *menuFileSep0;
        wxMenuItem *menuFileSep1;

        wxMenuItem *menuFileCreateComponent;
        wxMenuItem *menuFileCreateBlock;

        wxMenuItem *menuEditUndo;
        wxMenuItem *menuEditRedo;
        wxMenuItem *menuEditSelectAll;
        wxMenuItem *menuEditCopy;
        wxMenuItem *menuEditCut;
        wxMenuItem *menuEditPaste;
        wxMenuItem *menuEditDelete;
        wxMenuItem *menuCommentUncomment;
        wxMenuItem *menuEditFind;
        wxMenuItem *menuEditFindNext;
        wxMenuItem* menuEditSep0;
        wxMenuItem* menuEditSep1;

        wxMenuItem* menuComponentRun;
        wxMenuItem* menuComponentBuild;
        wxMenuItem* menuComponentOpenFolder;
        wxMenuItem* menuComponentSep0;

        wxMenuItem *menuViewHelpAndResourceCenter;
        wxMenuItem *menuViewLabels;
        wxMenuItem *menuViewComponents;
        wxMenuItem *menuViewHardware;
        wxMenuItem *menuViewLocalVariables;
        wxMenuItem *menuViewProperties;
        wxMenuItem *menuViewMessages;
        wxMenuItem *menuViewTerminal;
        wxMenuItem *menuViewQuickToolbar;
        wxMenuItem *menuViewGeneratedCode;
        wxMenuItem *menuViewPrevView;
        wxMenuItem *menuViewNextView;
        wxMenuItem *menuViewSep0;
        wxMenuItem *menuViewSep1;

        wxMenuItem *menuZoom10;
        wxMenuItem *menuZoom25;
        wxMenuItem *menuZoom50;
        wxMenuItem *menuZoom75;
        wxMenuItem *menuZoom100;
        wxMenuItem *menuZoom125;
        wxMenuItem *menuZoom150;
        wxMenuItem *menuZoom175;
        wxMenuItem *menuZoom200;
        wxMenuItem *menuZoomViewAll;

        BubbleEditor *editCode;

        bool useExternalHelpCenter;

        unsigned int tempComponentNameCounter;
        wxString tempComponentName;

        bool showingMessageArea;
        bool componentAlreadySaved;
        void onDropDownToolbarItem(wxAuiToolBarEvent& evt);

        //##void dropDownMenuEdit(wxAuiToolBar* tb, int id);
        //##void dropDownQuickZoom(wxAuiToolBar* tb, int id);

        void onCustomizeToolbar(wxCommandEvent& evt);

        void onAllowNotebookDnD(wxAuiNotebookEvent& evt);
        void onNotebookPageClose(wxAuiNotebookEvent& evt);
        void onNotebookPageChanged(wxAuiNotebookEvent& evt);
        //void OnNotebookFlag(wxCommandEvent& evt);

        //##void onTabAlignment(wxCommandEvent &evt);
        //##void onGradient(wxCommandEvent& evt);
        //##void onManagerFlag(wxCommandEvent& evt);
        //##void onUpdateUI(wxUpdateUIEvent& evt);
        void onPaneClose(wxAuiManagerEvent& evt);

        void createKeyboardAcceleratorTable();

        void popUpMenu(wxCommandEvent& evt, wxMenu *popMenu);
        void addToolsToMainMenu();
        void deleteToolsFromMainMenu();
        void createMainMenu();
        void createMenuFile();
        void createMenuFileCreate();
        void createMenuEdit();
        void createMenuComponent();
        void createMenuView();
        void createQuickToolbar();
        void createMenuZoom();

        void updateMainMenuGUI();
        void updateMenuZoomGUI();
        void updateMenuFileGUI();
        void updateMenuCreateGUI();
        void updateMenuEditGUI();
        void updateMenuComponentGUI();
        void updateMenuViewGUI();
        void updateQuickToolbarGUI();

        void createNotebook();
        void createHardwareManager();
        void createLocalVariablesManager();
        void createTerminal();
        void loadHelpAndResourceCenterFiles();
        void createHelpAndResourceCenter();
        void createProperties();
        void createFileBlock(bool mainCanvas, const wxString &newTabName);

        bool openFileComponent(const wxString &defaultDir = wxEmptyString);
        void createComponent(bool canCancel);
        void saveComponent();
        void saveComponentAs();

        void onMenuFile(wxCommandEvent& evt);
        void onMenuFileCreateComponent(wxCommandEvent& evt);
        void onMenuFileCreateBlock(wxCommandEvent& evt);
        void onMenuFileAdd(wxCommandEvent& evt);
        void onMenuFileRemove(wxCommandEvent& evt);
        void onMenuFileOpen(wxCommandEvent& evt);
        void onMenuFileExamples(wxCommandEvent& evt);
        void onMenuFileSave(wxCommandEvent& evt);
        void onMenuFileSaveAs(wxCommandEvent& evt);
        void onMenuFileSaveAll(wxCommandEvent& evt);
        void onMenuFileClose(wxCommandEvent& evt);
        void onMenuFileCloseAll(wxCommandEvent& evt);
        void onMenuFilePrint(wxCommandEvent& evt);
        void onMenuFileExportAsImageToClipboard(wxCommandEvent& evt);
        void onMenuFileExit(wxCommandEvent& evt);

        void onMenuEdit(wxCommandEvent& evt);
        void onMenuEditUndo(wxCommandEvent& evt);
        void onMenuEditRedo(wxCommandEvent& evt);
        void onMenuEditSelectAll(wxCommandEvent& evt);
        void onMenuEditCopy(wxCommandEvent& evt);
        void onMenuEditCut(wxCommandEvent& evt);
        void onMenuEditPaste(wxCommandEvent& evt);
        void onMenuEditDelete(wxCommandEvent& evt);
        void onMenuCommentUncomment(wxCommandEvent& evt);
        void onMenuEditFind(wxCommandEvent& evt);
        void onMenuEditFindNext(wxCommandEvent& evt);

        void onMenuComponent(wxCommandEvent& evt);
        void onMenuComponentRun(wxCommandEvent& evt);
        void onMenuComponentBuild(wxCommandEvent& evt);
        void onMenuComponentDeploy(wxCommandEvent& evt);
        void onMenuComponentMakePortable(wxCommandEvent& evt);
        void onMenuComponentOpenFolder(wxCommandEvent& evt);

        void onMenuView(wxCommandEvent& evt);

        void showWindow(const wxString& name, wxMenuItem *menuItem);
        void closeWindow(const wxString& name, wxMenuItem *menuItem, bool update = true);
        void toggleWindow(const wxString& name, wxMenuItem *menuItem);
        void onMenuViewHelpAndResourceCenter(wxCommandEvent& evt);
        void onMenuViewLabels(wxCommandEvent& evt);
        void onMenuViewComponents(wxCommandEvent& evt);
        void onMenuViewHardware(wxCommandEvent& evt);
        void onMenuViewProperties(wxCommandEvent& evt);
        void onMenuViewLocalVariables(wxCommandEvent& evt);
        void onMenuViewMessages(wxCommandEvent& evt);
        void onMenuViewTerminal(wxCommandEvent& evt);
        void onMenuViewQuickToolbar(wxCommandEvent& evt);
        void onMenuViewGeneratedCode(wxCommandEvent& evt);
        void refreshGeneratedCode();
        void toggleGeneratedCode();
        void onMenuViewFullScreen(wxCommandEvent& evt);
        void onMenuViewPrevView(wxCommandEvent& evt);
        void onMenuViewNextView(wxCommandEvent& evt);

        //void OnQuickFileCreate(wxCommandEvent& evt);

        void onQuickZoom(wxCommandEvent& evt);
        void onQuickZoom10(wxCommandEvent& evt);
        void onQuickZoom25(wxCommandEvent& evt);
        void onQuickZoom50(wxCommandEvent& evt);
        void onQuickZoom75(wxCommandEvent& evt);
        void onQuickZoom100(wxCommandEvent& evt);
        void onQuickZoom125(wxCommandEvent& evt);
        void onQuickZoom150(wxCommandEvent& evt);
        void onQuickZoom175(wxCommandEvent& evt);
        void onQuickZoom200(wxCommandEvent& evt);
        void onQuickZoomViewAll(wxCommandEvent& evt);

        void onHardwareManagerPopUpPort(wxCommandEvent& evt);
        void onHardwareManagerPopUpBoard(wxCommandEvent& evt);

        void onEraseBackground(wxEraseEvent& evt);
        void onActivate(wxActivateEvent& event);
        void onSize(wxSizeEvent& evt);
        void onClose(wxCloseEvent& event);
        void onExit(wxCommandEvent& evt);

        void onDecZoom(wxCommandEvent& evt);
        void onIncZoom(wxCommandEvent& evt);

        void onCloseCurrentWindow(wxCommandEvent& evt);
        void onCloseMessages(wxCommandEvent& evt);

        void OnHtmlLinkClicked(wxHtmlLinkEvent &event);

        //Bubble related:
        void zoomNotify();
        //##void processTerminationNotify(BubbleProcess *process);
        void changeBoardNotify();
        void processTerminationNotify();
        void refreshGeneratedCodeNotify();
        void buildFinishedNotify() { }
        void deployStartedNotify();
        void deployFinishedNotify() { }
        void runFinishedNotify() { }
        void makePortableFinishedNotify() { }

        void clearMessage();
        void showMessage(   const wxString &msg, bool clear, bool showWindow,
                            const wxColour& colour, bool clearProgressBar = true);
        void showMessagesWindow();
        void hideMessagesWindow();
        virtual void showMessageArea(bool value);

        void setProgressMax(unsigned int value);
        unsigned int getProgressMax() const;
        void setProgressPosition(unsigned int value, bool showWindow, bool showMessages);
        unsigned int getProgressPosition() const;

        DECLARE_EVENT_TABLE()

    public:
        MainFrame(  wxWindow* parent,
                    wxWindowID id,
                    wxLocale& locale,
                    const wxString& languagePath,
                    const wxString& initialCatalogName,
                    const wxString& title,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize,

                    //##Esto da pantalla normal:
                    long style = wxDEFAULT_FRAME_STYLE | wxSUNKEN_BORDER
                );
                    //##Esto da pantalla completa, es como el F11:
                    //long style = wxMAXIMIZE | wxSUNKEN_BORDER);

        ~MainFrame();

        //##2010.10.10: This event was moved to the class BubbleCanvas, but there the event capture does
        //not work, (not even with Connect, neither with the event table). So the event is captured in the
        //frame and forwarded to the current bubbleCanvas:
        //void WxPanelBlocksMouseWheel(wxMouseEvent& event);

        //i8n/i10n:
        bool addLocaleCatalog(const wxString& value);
        inline const wxString &getLastAddedLocaleCatalog() const { return lastAddedLocaleCatalog; }
        //inline const wxString &getLanguagePath() const { return languagePath; }
        void updateGUI();

        wxAuiDockArt* getDockArt();
        void doUpdate();

//        inline const void setInitialFile(const wxString& value) { initialFile = value; }
//        inline wxString getInitialFile() { return initialFile; }

        void loadFileComponent(const wxString &value);
};
#endif
