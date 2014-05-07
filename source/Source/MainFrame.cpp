#include "MainFrame.h"
#include "Minibloq.xpm"

#include <wx/menu.h>
#include <wx/aui/aui.h>
#include <wx/stdpaths.h>
#include <wx/dir.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h> //##Debug, se va en release. Agregar compilación condicional.
#include <wx/textctrl.h>
#include <wx/uri.h>
#include <wx/artprov.h>
#include <wx/clipbrd.h>
#include <wx/image.h>
#include <wx/toolbar.h>


BEGIN_EVENT_TABLE(MainFrame, wxFrame)
    //EVT_ACTIVATE(MainFrame::onActivate)
	EVT_CLOSE(MainFrame::onClose)
    EVT_ERASE_BACKGROUND(MainFrame::onEraseBackground)
    EVT_SIZE(MainFrame::onSize)

    EVT_MENU(ID_MenuFile, MainFrame::onMenuFile)
    //##:
    EVT_MENU(ID_MenuFileCreate, MainFrame::onMenuFileCreateComponent)
    //##EVT_MENU(ID_MenuFileCreateComponent, MainFrame::onMenuFileCreateComponent)
    EVT_MENU(ID_MenuFileCreateBlock, MainFrame::onMenuFileCreateBlock)
    EVT_MENU(ID_MenuFileRemove, MainFrame::onMenuFileRemove)
    EVT_MENU(ID_MenuFileOpen, MainFrame::onMenuFileOpen)
    EVT_MENU(ID_MenuFileAdd, MainFrame::onMenuFileAdd)
    EVT_MENU(ID_MenuFileExamples, MainFrame::onMenuFileExamples)
    EVT_MENU(ID_MenuFileSave, MainFrame::onMenuFileSave)
    //EVT_MENU(ID_MenuFileSaveAs, MainFrame::onMenuFileSaveAs)
    EVT_MENU(ID_MenuFileSaveAll, MainFrame::onMenuFileSaveAll)
    EVT_MENU(ID_MenuFileClose, MainFrame::onMenuFileClose)
    EVT_MENU(ID_MenuFileCloseAll, MainFrame::onMenuFileCloseAll)
    EVT_MENU(ID_MenuFilePrint, MainFrame::onMenuFilePrint)
    EVT_MENU(ID_MenuFileExportAsImageToClipboard, MainFrame::onMenuFileExportAsImageToClipboard)
    EVT_MENU(wxID_EXIT, MainFrame::onExit)

    EVT_MENU(ID_MenuEdit, MainFrame::onMenuEdit)
    EVT_MENU(ID_MenuEditUndo, MainFrame::onMenuEditUndo)
    EVT_MENU(ID_MenuEditRedo, MainFrame::onMenuEditRedo)
    EVT_MENU(ID_MenuEditSelectAll, MainFrame::onMenuEditSelectAll)
    EVT_MENU(ID_MenuEditCopy, MainFrame::onMenuEditCopy)
    EVT_MENU(ID_MenuEditCut, MainFrame::onMenuEditCut)
    EVT_MENU(ID_MenuEditPaste, MainFrame::onMenuEditPaste)
    EVT_MENU(ID_MenuEditDelete, MainFrame::onMenuEditDelete)
    EVT_MENU(ID_MenuCommentUncomment, MainFrame::onMenuCommentUncomment)
    EVT_MENU(ID_MenuEditFind, MainFrame::onMenuEditFind)
    EVT_MENU(ID_MenuEditFindNext, MainFrame::onMenuEditFindNext)

    EVT_MENU(ID_MenuComponent, MainFrame::onMenuComponent)
    EVT_MENU(ID_MenuComponentRun, MainFrame::onMenuComponentRun)
    EVT_MENU(ID_MenuComponentDeploy, MainFrame::onMenuComponentDeploy)
    EVT_MENU(ID_MenuComponentBuild, MainFrame::onMenuComponentBuild)
    EVT_MENU(ID_MenuComponentMakePortable, MainFrame::onMenuComponentMakePortable)
    EVT_MENU(ID_MenuComponentOpenFolder, MainFrame::onMenuComponentOpenFolder)

    EVT_MENU(ID_MenuView, MainFrame::onMenuView)
    EVT_MENU(ID_MenuViewHelpAndResourceCenter, MainFrame::onMenuViewHelpAndResourceCenter)
    EVT_MENU(ID_MenuViewLabels, MainFrame::onMenuViewLabels)
    EVT_MENU(ID_MenuViewComponents, MainFrame::onMenuViewComponents)
    EVT_MENU(ID_MenuViewHardware, MainFrame::onMenuViewHardware)
    EVT_MENU(ID_MenuViewLocalVariables, MainFrame::onMenuViewLocalVariables)
    EVT_MENU(ID_MenuViewProperties, MainFrame::onMenuViewProperties)
    EVT_MENU(ID_MenuViewMessages, MainFrame::onMenuViewMessages)
    EVT_MENU(ID_MenuViewTerminal, MainFrame::onMenuViewTerminal)
    EVT_MENU(ID_MenuViewQuickToolbar, MainFrame::onMenuViewQuickToolbar)
    EVT_MENU(ID_MenuViewComponentBlocks, MainFrame::onMenuViewComponentBlocks)
    EVT_MENU(ID_MenuViewGeneratedCode, MainFrame::onMenuViewGeneratedCode)
    //##No implementado por ahora:
    //EVT_MENU(ID_MenuViewFullScreen, MainFrame::OnMenuViewFullScreen)
    EVT_MENU(ID_MenuViewPrevView, MainFrame::onMenuViewPrevView)
    EVT_MENU(ID_MenuViewNextView, MainFrame::onMenuViewNextView)

    //##Por ahora, el botón del quick bar es para agregar archivo nuevo de bloques, no despliega
    //el pop, pero si luego se hace este comportamiento configurable, o algo así, se puede usar
    //esta llamada a despliegue de menú aquí comentada:
    //popUpMenu(evt, popFileCreate):
    EVT_MENU(ID_QuickFileCreate, MainFrame::onMenuFileCreateBlock)
    EVT_MENU(ID_QuickFileOpen, MainFrame::onMenuFileOpen)
    EVT_MENU(ID_QuickFileAdd, MainFrame::onMenuFileAdd)
    EVT_MENU(ID_QuickFileSaveAll, MainFrame::onMenuFileSaveAll)
    EVT_MENU(ID_QuickComponentRun, MainFrame::onMenuComponentRun)
    EVT_MENU(ID_QuickComponentDeploy, MainFrame::onMenuComponentDeploy)
    EVT_MENU(ID_QuickViewTerminal, MainFrame::onMenuViewTerminal)
    EVT_MENU(ID_QuickViewHelpAndResourceCenter, MainFrame::onMenuViewHelpAndResourceCenter)

    EVT_MENU(ID_QuickZoom, MainFrame::onQuickZoom)
    EVT_MENU(ID_QuickZoom10, MainFrame::onQuickZoom10)
    EVT_MENU(ID_QuickZoom25, MainFrame::onQuickZoom25)
    EVT_MENU(ID_QuickZoom50, MainFrame::onQuickZoom50)
    EVT_MENU(ID_QuickZoom75, MainFrame::onQuickZoom75)
    EVT_MENU(ID_QuickZoom100, MainFrame::onQuickZoom100)
    EVT_MENU(ID_QuickZoom125, MainFrame::onQuickZoom125)
    EVT_MENU(ID_QuickZoom150, MainFrame::onQuickZoom150)
    EVT_MENU(ID_QuickZoom175, MainFrame::onQuickZoom175)
    EVT_MENU(ID_QuickZoom200, MainFrame::onQuickZoom200)
    EVT_MENU(ID_QuickZoomViewAll, MainFrame::onQuickZoomViewAll)

    EVT_MENU(ID_HardwareManagerPopUpPort, MainFrame::onHardwareManagerPopUpPort)
    EVT_MENU(ID_HardwareManagerPopUpBoard, MainFrame::onHardwareManagerPopUpBoard)

    EVT_AUI_PANE_CLOSE(MainFrame::onPaneClose)
    EVT_AUINOTEBOOK_ALLOW_DND(ID_Notebook, MainFrame::onAllowNotebookDnD)
    EVT_AUINOTEBOOK_PAGE_CLOSE(ID_Notebook, MainFrame::onNotebookPageClose)
    EVT_AUINOTEBOOK_PAGE_CHANGED(ID_Notebook, MainFrame::onNotebookPageChanged)

//##Implement those too?:
//    EVT_AUINOTEBOOK_ALLOW_DND(ID_Terminal, MainFrame::onAllowTerminalDnD)
//    EVT_AUINOTEBOOK_PAGE_CLOSE(ID_Terminal, MainFrame::onTerminalPageClose)
//    EVT_AUINOTEBOOK_PAGE_CHANGED(ID_Terminal, MainFrame::onTerminalPageChanged)

//    EVT_AUINOTEBOOK_ALLOW_DND(ID_Messages, MainFrame::onAllowMessagesDnD)
//    EVT_AUINOTEBOOK_PAGE_CLOSE(ID_Messages, MainFrame::onMessagesPageClose)
//    EVT_AUINOTEBOOK_PAGE_CHANGED(ID_Messages, MainFrame::onMessagesPageChanged)

    EVT_MENU(ID_ShortCut_CloseCurrentWindow, MainFrame::onCloseCurrentWindow)
    EVT_MENU(ID_ShortCut_CloseMessages, MainFrame::onCloseMessages)

    EVT_MENU(ID_ShortCut_DecZoom, MainFrame::onDecZoom)
    EVT_MENU(ID_ShortCut_IncZoom, MainFrame::onIncZoom)

    //##Future: Put this on the HelpAndResourceCenter class:
    EVT_HTML_LINK_CLICKED(wxID_ANY, MainFrame::OnHtmlLinkClicked)
END_EVENT_TABLE()


MainFrame::MainFrame(   wxWindow* parent,
                        wxWindowID id,
                        wxLocale& locale,
                        const wxString& languagePath,
                        const wxString& initialCatalogName,
                        const wxString& title,
                        const wxString& boardName,
                        const wxPoint& pos,
                        const wxSize& size,
                        long style):
                                        wxFrame(parent, id, title, pos, size, style),
                                        showCodeAtStart(false),
                                        centered(true),
                                        boardName(boardName),
                                        bubble(locale),
                                        locale(locale),
                                        languagePath(languagePath),
                                        lastAddedLocaleCatalog(wxString("")),
                                        //##Revisar que esté todo bien inicializado acá:
                                        commManager(NULL),
                                        singleTerminalGUI(NULL),
                                        splitTerminalGUI(NULL),

                                        notebook(NULL),

                                        helpAndResourceCenter(NULL),
                                        properties(NULL),
                                        terminal(NULL),
                                        hardware(NULL),
                                        //localVariables(NULL),
                                        messages(NULL),
                                        //components(NULL),

                                        toolZoom(NULL),
                                        toolQuick(NULL),

                                        help(NULL),
                                        about(NULL),
                                        license(NULL),

                                        minibloqProperties(NULL),
                                        componentProperties(NULL),
                                        terminalProperties(NULL),
                                        textEditorProperties(NULL),
                                        blockEditorProperties(NULL),

                                        toolMainMenu(NULL),

                                        popFile(NULL),
                                        popFileCreate(NULL),
                                        popEdit(NULL),
                                        popComponent(NULL),
                                        popView(NULL),
                                        popZoom(NULL),

                                        menuFileCreate(NULL),
                                        menuFileOpen(NULL),
                                        menuFileAdd(NULL),
                                        menuFileExamples(NULL),
                                        menuFileSave(NULL),
                                        //menuFileSaveAs(NULL),
                                        menuFileExit(NULL),
                                        menuFileSep0(NULL),
                                        menuFileSep1(NULL),

                                        menuFileCreateComponent(NULL),
                                        menuFileCreateBlock(NULL),

                                        menuEditUndo(NULL),
                                        menuEditRedo(NULL),
                                        menuEditSelectAll(NULL),
                                        menuEditCopy(NULL),
                                        menuEditCut(NULL),
                                        menuEditPaste(NULL),
                                        menuEditDelete(NULL),
                                        menuCommentUncomment(NULL),
                                        menuEditFind(NULL),
                                        menuEditFindNext(NULL),
                                        menuEditSep0(NULL),
                                        menuEditSep1(NULL),

                                        menuComponentRun(NULL),
                                        menuComponentBuild(NULL),
                                        menuComponentOpenFolder(NULL),
                                        menuComponentSep0(NULL),

                                        menuViewHelpAndResourceCenter(NULL),
                                        menuViewLabels(NULL),
                                        menuViewComponents(NULL),
                                        menuViewHardware(NULL),
                                        menuViewLocalVariables(NULL),
                                        menuViewProperties(NULL),
                                        menuViewMessages(NULL),
                                        menuViewTerminal(NULL),
                                        menuViewQuickToolbar(NULL),
                                        menuViewComponentBlocks(NULL),
                                        menuViewGeneratedCode(NULL),
                                        menuViewPrevView(NULL),
                                        menuViewNextView(NULL),
                                        menuViewSep0(NULL),
                                        menuViewSep1(NULL),

                                        menuZoom10(NULL),
                                        menuZoom25(NULL),
                                        menuZoom50(NULL),
                                        menuZoom75(NULL),
                                        menuZoom100(NULL),
                                        menuZoom125(NULL),
                                        menuZoom150(NULL),
                                        menuZoom175(NULL),
                                        menuZoom200(NULL),
                                        menuZoomViewAll(NULL),

                                        editCode(NULL),
                                        editCodeZoom(3),

                                        useExternalHelpCenter(false),

                                        tempComponentNameCounter(0),
                                        tempComponentName(wxString("comp1")),  //##Future: This should be initialized after
                                                                                //checking that the comp1 does not exist
                                                                                //in the temp directory yet...
                                        showingMessageArea(true),
                                        componentAlreadySaved(false)
{
    //Tell wxAuiManager to manage this frame:
    auiManager.SetManagedWindow(this);

    //Sets the frame icon:
    SetIcon(wxIcon(Minibloq_xpm));

    //##Un-hardcode this:=
    //Init all the important paths:
    //##Revisar bien el manejo de wxString, para que sea todo correcto, sobre todo en cuanto a memoria:
    //##El último string, osea el número de Theme, hay que sacarlo del archivo de XML configuración:
    //##Además, todo esto es parte de Minibloq, no de Bubble, por lo que en cuanto se pueda hay que descablear
    //este asunto de los path y pasarlos como parámetros a Bubble:
    bubble.setLanguageCode(initialCatalogName.BeforeLast('.'));
    bubble.setAppPath(wxString(wxStandardPaths::Get().GetExecutablePath().BeforeLast(wxFileName::GetPathSeparator()) ));
    bubble.setHost(wxString("win_i386"));
#if defined (linux) && defined(__i386__)
    bubble.setHost(wxString("linux_i386"));
#endif
#if defined (linux) && defined(__x86_64__)
    bubble.setHost(wxString("linux_amd64"));
#endif

    //##2011.10.02: This threshold (of 152 characters) has been determined experimantally, as a WinAVR
    //limitation. But this is just a small patch until we found a better solution (which will be proabley
    //complex), because the real limit is not 152 chars, but the long paths needed to include the libraries
    //deployed with some blocks (whose paths are really really long). This number may be even different for
    //other toolchains, and it must be configurable in the future:
    if (bubble.getAppPath().Len() > 152)
    {
        wxMessageDialog dialog0(this,
                                _("The installation path\n\n\"") + bubble.getAppPath() + _("\"\n\nis too long. ") +
                                _("Please try to install Minibloq on a location with a shorter path."),
                                _("Error #0"));
        dialog0.ShowModal();
    }

    //##Ver si es "/GUI/Images/" o "GUI/Images/" (esto último sería más consistente con el resto de la aplicación):
    bubble.setThemePath(bubble.getAppPath() + wxString("/GUI/Images/") + wxString("Theme0"));

    //##Debug:
    //wxMessageDialog dialog0(this, imagesPath, _("toolsPath"));
    //dialog0.ShowModal();

    //##Esto no es portable, ya que en Linux y Mac seguramente habrá que buscar por '/', pero por ahora
    //se lo deja así, ya que en Windows es como funciona:
    //##En realidad, TODO LO QUE SEA SUMA DE SEPARADORES (me refieron a la barra) Y EXTRACCIÓN DE SEPARADORES,
    //debería hacerse con una variable de tipo char inicializada con la función wxFileName::GetPathSeparator,
    //lo que debería generar independencia del sistema operativo:

    bubble.setComponentsRepositoryPath((bubble.getAppPath().BeforeLast(wxFileName::GetPathSeparator())).BeforeLast(wxFileName::GetPathSeparator()));
    bubble.setBlocksPath(bubble.getComponentsRepositoryPath() + wxString("/blocks"));

    //##Debug:
//    wxMessageDialog dialog0(this, bubble.getComponentsRepositoryPath(), _("componentsRepositoryPath")); //##Debug
//    dialog0.ShowModal(); //##Debug

    bubble.setToolsPath(bubble.getComponentsRepositoryPath() + wxString("/lang/WinAVR/v20090313/bin")); //##Un-hardcode...

    //##Horrible, super temporal:
    wxFileName _tempPath(bubble.getComponentsRepositoryPath() + wxString("/work/_temp")); //##Un-hardcode...
    if (!wxDir::Exists(_tempPath.GetFullPath()))
    {
        //wxMessageDialog dialog0(this, _tempPath.GetFullPath(), _("1")); //##Debug
        //dialog0.ShowModal(); //##Debug
        wxMkdir(_tempPath.GetFullPath());
    }
    //##bubble.setTempPath(_tempPath.GetPath());
    bubble.setTempPath(bubble.getComponentsRepositoryPath() + wxString("/work/_temp")); //##Un-hardcode...

    //At first, the component path is the Temp path:
    bubble.setComponentPath(bubble.getTempPath());
    wxFileName aux(tempComponentName);
    bubble.setComponentFilesPath(bubble.getComponentPath() + wxString("/") + aux.GetName());
    bubble.setOutputPath(bubble.getComponentFilesPath() + wxString("/output"));
    bubble.setTargetsPath(bubble.getComponentsRepositoryPath() + wxString("/hard"));
    bubble.setDocPath(bubble.getAppPath() + wxString("/Doc"));

    //##Debug:
    //wxMessageDialog dialog0(this, _("0"), _("0")); //##Debug
    //dialog0.ShowModal(); //##Debug

    SetMinSize(wxSize(400, 300)); //##

    //This is very important, to be able to load any image supported by the wxImage class:
    ::wxInitAllImageHandlers();

    //Menus and toolbars:
    createMainMenu();
    createQuickToolbar();

    //##Panels: NO BORRAR ESTAS COSAS QUE ESTÁN COMENTADAS:
    /*##
    auiManager.AddPane( CreateTreeCtrl(), wxAuiPaneInfo().
                        Name(wxString("Components")).Caption(_("Components"))
                        .Left().Layer(1).Position(1)
                        .CloseButton(true).MaximizeButton(true)
                      );
    */

    /*##Bug:
        Si Creo el hardwareManager, o aparentemente cualquier cosa que contenga un BubbleButton y lo agrego
        como pane al auiManager, entonces pueden aparecer errores aleatorios de memoria horrendos. Así que
        por ahora, no voy a usar BubbleButtons en panes del AUIManager, ya veré qué hago. La verdad es que
        AUI parece hacer agua por todas partes. Puede obviamente también haber bugs en los botones, pero es
        una clase muy simple, y que evita cosas extrañas, así que no sé qué estará pasando realmente. Por
        ahora lo dejo así. Si tengo que usar la ventana de hardware sin que se cuelgue todo, hay 2 opciones:
        1. Descomentar la línea de arriba que hace el CreateTreeCtrl (con sus funciones asociadas,
        obviamente).
        2. Descomentar las 2 líneas con el camentario de "raro" abajo en el evento OnNotebookPageClose().
    */

    //##Ver si esto queda acá finalmente:
    bubble.setNotifier(this);

    //bubble.loadBlocksInfo(this, false); //Must be called BEFORE the adding of a canvas ( which is done in CreateNotebook() ).
    //##Debug:
    //wxMessageDialog dialog0(this, _("0"), _("0")); //##Debug
    //dialog0.ShowModal(); //##Debug
    createHardwareManager(); //Now This is loading the blocks info (and this must be done BEFORE adding a canvas!).
    bubble.setHardwareManager(hardware);

    //##Esto va a cambiar, porque no me parece seguro el sistema de createNobebook donde devuelve
    //un control que podría ser NULL. Así que el AddPane se hará en el CreateNotebook:
    //como se llame:
    createNotebook();
    //else
    //  ##Reportar error y salir?

#if UNDER_DEVELOPMENT
    createLocalVariablesManager(); //##
#endif

    createHelpAndResourceCenter();
    createProperties();

    messages = new Messages(this, ID_Messages, wxColour(255, 255, 255));
    auiManager.AddPane( messages, wxAuiPaneInfo()
                        .Name(wxString("Messages"))
                        .Caption(_("Messages"))
                        .Bottom().Layer(1).Position(1)
                        .CloseButton(true).Hide()
                        .BestSize(wxSize(200, 200)) //##
                        .MinSize(wxSize(200, 100))
                      );

    createTerminal();

#if UNDER_DEVELOPMENT
    components = new BubbleComponentsManager(this, ID_Components, wxColour(255, 255, 255));
    auiManager.AddPane( components, wxAuiPaneInfo()
                        .Name(wxString("Components"))
                        .Caption(_("Components"))
                        .Left().Layer(1).Position(1)
                        .CloseButton(true).Hide()
                        .BestSize(wxSize(400, 450))
                        .MinSize(wxSize(200, 275))
                      );
#endif

    //##Ver todo este código:
    int i, count;
    wxAuiPaneInfoArray& all_panes = auiManager.GetAllPanes();
    for (i = 0, count = all_panes.GetCount(); i < count; ++i)
        if (!all_panes.Item(i).IsToolbar())
            all_panes.Item(i).Hide();
    //auiManager.GetPane(wxString("Components")).Show().Left().Layer(0).Row(0).Position(0);
    //##auiManager.GetPane(wxString("ActionPicker")).Show().Left().Layer(0).Row(0).Position(0); //##
    auiManager.GetPane(wxString("Hardware")).Show().Left().Layer(0).Row(0).Position(0).PinButton().BestSize(400, 450); //##
    //##auiManager.GetPane(wxString("LocalVariables")).Show().Left().Layer(0).Row(0).Position(1).PinButton().BestSize(400, 450); //##
    //auiManager.GetPane(wxString("CommandWindow")).Show().Bottom().Layer(0).Row(0).Position(0);
    auiManager.GetPane(wxString("Blocks")).Show();

    //"Commit" all changes made to wxAuiManager:
    auiManager.Update();

    BubbleActionPicker *actionPicker = bubble.getActionPicker();
    if (actionPicker)
    {
//##v0.7 (delete if not used!):
//        bubble.showPicker(  actionPicker,
//                            wxPoint(GetPosition().x + GetClientSize().GetWidth() - actionPicker->GetSize().GetWidth(),
//                                    //##GetPosition().y + actionPicker->GetSize().GetHeight()/2
//                                    GetPosition().y + actionPicker->GetSize().GetWidth()/2
//                                    ),
//                            true
//                         );
        bubble.showPicker(  actionPicker,
                            wxPoint(GetPosition().x + GetSize().GetWidth() - actionPicker->GetSize().GetWidth(),
                                    GetPosition().y + GetSize().GetHeight()/2 - actionPicker->GetSize().GetHeight()/2),
                            true
                         );

        //##Future: Integrate ActionPicker with the AUI interface:
        /*
            auiManager.AddPane( createActionPicker(), wxAuiPaneInfo().
            //auiManager.AddPane( new BubblePicker(this, wxID_ANY, wxColour(92, 92, 92)), wxAuiPaneInfo().
                                Name(wxString("ActionPicker")).Caption(_("Action Picker"))
                                .Left().Layer(1).Position(1)
                                .CloseButton(true)
                              );
        */
    }

    //Keyboard managment:
    createKeyboardAcceleratorTable();

    //##Ver si esto queda acá finalmente:
    //bubble.loadBlocks(); //Must be called BEFORE add a canvas ( which is done in CreateNotebook() ).

    changeBoardNotify();

    //This forces to load the includes in the generated code:
    bubble.loadBoardRelations();

    //Read configuration from the config file and initializes everything according to that:
    readConfig();
    if (getShowCodeAtStart())
        toggleGeneratedCode();

//    if (notebook)
//    {
//        if (bubble.getCurrentCanvas())
//        {
//            int index = notebook->GetPageIndex(bubble.getCurrentCanvas());
//            if (index != wxNOT_FOUND)
//            {
//                wxBitmap page_bmp = wxArtProvider::GetBitmap(wxART_WARNING, wxART_OTHER, wxSize(16, 16));
//                notebook->SetPageBitmap(index, page_bmp);
//            }
//        }
//    }
//    bubble.forceSaved(false);
}


bool MainFrame::isShowingGeneratedCode()
{
    return notebook->GetPageIndex(editCode) != wxNOT_FOUND;
}


MainFrame::~MainFrame()
{
    auiManager.UnInit();
    //##No hay que destruir (ver documentación de wxMenu, donde no queda del todo claro), pero si se destruye
    //aquí, la aplicación crashea al salir:
/*
    if (popFile)
        delete popFile;
    //The popFileCreate is destroyed by it's parent. It will be a bug to delete it here:
    //if (popFileCreate)
    //    delete popFileCreate;
    if (popEdit)
        delete popEdit;
    if (popComponent)
        delete popComponent;
    if (popView)
        delete popView;
    if (popZoom)
        delete popZoom;
*/
    //##:Esto producía un fallo cuando la instancia de bubble era dinámica, porque tanto el array como el
    //sistema de "parents" intentan destruir los objetos, y se cuelga el software al finalizar:
    //>> Ahora que se usa wxList, se pasó esto a estático:
    //##delete bubble;

    if (commManager)
        delete commManager;
}


void MainFrame::updateMenu()
{
    if (menuViewHelpAndResourceCenter)
        menuViewHelpAndResourceCenter->Check(auiManager.GetPane("HelpAndResourceCenter").IsShown());
    if (menuViewQuickToolbar)
        menuViewQuickToolbar->Check(auiManager.GetPane("QuickToolBar").IsShown());
    if (notebook)
    {
        int index = notebook->GetPageIndex(bubble.getCurrentCanvas());
        if (menuViewComponentBlocks)
            menuViewComponentBlocks->Check(index != wxNOT_FOUND);
    }
    if (menuViewGeneratedCode)
        menuViewGeneratedCode->Check(auiManager.GetPane("GeneratedCode").IsShown());
    if (menuViewHardware)
        menuViewHardware->Check(auiManager.GetPane("Hardware").IsShown());
    if (menuViewProperties)
        menuViewProperties->Check(auiManager.GetPane("Properties").IsShown());
    if (menuViewMessages)
        menuViewMessages->Check(auiManager.GetPane("Messages").IsShown());
    if (menuViewTerminal)
        menuViewTerminal->Check(auiManager.GetPane("Terminal").IsShown());
}


wxString MainFrame::getConfigFileName()
{
    return  wxStandardPaths::Get().GetExecutablePath().BeforeLast(wxFileName::GetPathSeparator()) +
            wxString("/config/miniBloq.xml");
}


void MainFrame::readConfig()
{
    wxString fileName = getConfigFileName();
    wxXmlDocument xmlFile;

    if ( !xmlFile.Load(fileName, wxString("UTF-8")) )
        return;
    wxXmlNode *root = xmlFile.GetRoot();
    if (root == NULL)
        return;

    wxString tempName("");
    wxXmlNode *rootChild = root->GetChildren();
    while (rootChild)
    {
        tempName = rootChild->GetName();
        if (tempName == wxString("mainFrame"))
        {
            wxXmlNode *child = rootChild->GetChildren();
            while (child)
            {
                if (child->GetName() == "perspective")
                {
                    wxString strPerspective = child->GetNodeContent();
                    auiManager.LoadPerspective(strPerspective, true);
                }
                child = child->GetNext();
            }
        }
        else if (tempName == wxString("hard"))
        {
            wxXmlNode *child = rootChild->GetChildren();
            while (child)
            {
                /*if (child->GetName() == "board")
                {
                    if (bubble.getHardwareManager())
                    {
                        wxString strBoard = child->GetNodeContent();
                        if (bubble.getHardwareManager()->getBoardSelection() != strBoard) //Small optimization for the first board.
                            bubble.getHardwareManager()->setBoardSelection(strBoard);
                    }
                }
                else */if (child->GetName() == "port")
                {
                    if (bubble.getHardwareManager())
                        bubble.getHardwareManager()->setPortSelection(child->GetNodeContent());
                }
                child = child->GetNext();
            }
        }
        else if (tempName == wxString("terminal"))
        {
            wxXmlNode *child = rootChild->GetChildren();
            while (child)
            {
                if (commManager)
                {
                    if (child->GetName() == "tab")
                    {
                        if (commManager->getSingleTerminal() &&
                            commManager->getSplitTerminal())
                        {
                            if (child->GetNodeContent() == wxString("single"))
                                commManager->getSingleTerminal()->SetFocus();
                            else if (child->GetNodeContent() == wxString("split"))
                                commManager->getSplitTerminal()->SetFocus();
                        }
                    }
                    else if (child->GetName() == "showEmoticons")
                    {
                        if (commManager->getSplitTerminal())
                        {
                            commManager->getSplitTerminal()->setEmoticonsEnabled(false);    //This line is a patch due to a possible bug in
                                                                                            //wxWidgets: anyway, it's necessary in order to
                                                                                            //properly show the emoticon screen in the split
                                                                                            //terminal.
                            commManager->getSplitTerminal()->setEmoticonsEnabled(Bubble::string2bool(child->GetNodeContent()));
                        }
                    }
                    else if (child->GetName() == "baudrate")
                    {
                        wxString returnStringValue = child->GetNodeContent();
                        long returnNumericValue = 0;
                        if (returnStringValue.ToLong(&returnNumericValue))
                            commManager->setBaudRate((wxBaud)returnNumericValue);
                    }
                }
                child = child->GetNext();
            }
        }
        else if (tempName == wxString("properties"))
        {
            wxXmlNode *child = rootChild->GetChildren();
            while (child)
            {
                if (child->GetName() == "language")
                {
                    if (minibloqProperties)
                    {
                        wxString strLanguage = child->GetNodeContent();
                        if (minibloqProperties->getLanguageSelection() != strLanguage) //Small optimization.
                            minibloqProperties->setLanguageSelection(strLanguage);
                    }
                }
                if (child->GetName() == "profile")
                {
                    bubble.setProfile(child->GetNodeContent());
                    if (bubble.getHardwareManager())
                        bubble.getHardwareManager()->showDeveloperButtons(bubble.getProfile() == wxString("developer"));
                }
                child = child->GetNext();
            }
        }
        else if (tempName == wxString("canvas"))
        {
            wxXmlNode *child = rootChild->GetChildren();
            while (child)
            {
                if (child->GetName() == "zoom")
                {
                    if (bubble.getCurrentCanvas())
                    {
                        wxString returnStringValue = child->GetNodeContent();
                        long returnNumericValue = 0;
                        if (returnStringValue.ToLong(&returnNumericValue))
                            bubble.getCurrentCanvas()->setZoomIndex((int)returnNumericValue);
                    }
                }
                child = child->GetNext();
            }
        }
        else if (tempName == wxString("code"))
        {
            wxXmlNode *child = rootChild->GetChildren();
            while (child)
            {
                if (child->GetName() == "shown")
                {
                    setShowCodeAtStart(Bubble::string2bool(child->GetNodeContent()));
                }
                if (child->GetName() == "zoom")
                {
                    wxString returnStringValue = child->GetNodeContent();
                    long returnNumericValue = 0;
                    if (returnStringValue.ToLong(&returnNumericValue))
                        setEditCodeZoom((int)returnNumericValue);
                }
                child = child->GetNext();
            }
        }
        else if (tempName == wxString("help"))
        {
            wxXmlNode *child = rootChild->GetChildren();
            while (child)
            {
                if (helpAndResourceCenter && help && about && license)
                {
                    if (child->GetName() == "tab")
                    {
                        if (child->GetNodeContent() == wxString("main"))
                            helpAndResourceCenter->SetSelection(helpAndResourceCenter->GetPageIndex(help));
                        else if (child->GetNodeContent() == wxString("about"))
                            helpAndResourceCenter->SetSelection(helpAndResourceCenter->GetPageIndex(about));
                        else if (child->GetNodeContent() == wxString("license"))
                            helpAndResourceCenter->SetSelection(helpAndResourceCenter->GetPageIndex(license));
                    }
                }
                child = child->GetNext();
            }
        }
//##Future: show recent opened components in the file menu:
//        else if (tempName == wxString("components"))
//        {
//            wxXmlNode *child = rootChild->GetChildren();
//            int i = 0;
//            while (child)
//            {
//                if (child->GetName() == "recent")
//                {
//                    if (i < 10) //##This number could be moved to a constant or the like in the future...
//                    {
//                        addRecentComponent(child->GetNodeContent());
//                    }
//                    else
//                        break;
//                }
//                i++;
//                child = child->GetNext();
//            }
//        }
        rootChild = rootChild->GetNext();
    }

    updateMenu();
    auiManager.Update();
}


void MainFrame::writeConfig()
{
    try
    {
        wxString fileName = getConfigFileName();

        //Replaces the old file by a new one:
        wxTextFile configFile;
        wxRemoveFile(fileName);
        if (!configFile.Create(fileName))
            return;

        configFile.AddLine("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
        configFile.AddLine("<!-- Created with Minibloq (http://minibloq.org/) -->");
        configFile.AddLine("<miniBloq>");

        configFile.AddLine("<mainFrame>");
        configFile.AddLine(wxString("<x>") <<  GetPosition().x << wxString("</x>"));
        configFile.AddLine(wxString("<y>") <<  GetPosition().y << wxString("</y>"));
        configFile.AddLine(wxString("<width>") <<  GetSize().GetWidth() << wxString("</width>"));
        configFile.AddLine(wxString("<height>") <<  GetSize().GetHeight() << wxString("</height>"));
        if (IsMaximized())
            configFile.AddLine(wxString("<maximized>true</maximized>"));
        else
            configFile.AddLine(wxString("<maximized>false</maximized>"));
        configFile.AddLine(wxString("<centered>") << Bubble::bool2string(getCentered()) << wxString("</centered>")); //##Not used by now.
        configFile.AddLine(wxString("<perspective>") <<  auiManager.SavePerspective() << wxString("</perspective>"));
        configFile.AddLine("</mainFrame>");

        configFile.AddLine("<hard>");
        if (bubble.getHardwareManager())
            configFile.AddLine(wxString("<board>") <<  bubble.getHardwareManager()->getBoardSelection() << wxString("</board>"));
        if (bubble.getHardwareManager())
            configFile.AddLine(wxString("<port>") <<  bubble.getHardwareManager()->getPortSelection() << wxString("</port>"));
        configFile.AddLine("</hard>");

        configFile.AddLine("<terminal>");
        if (commManager)
        {
            if (commManager->getSingleTerminal() &&
                commManager->getSplitTerminal() &&
                terminal)
            {
                if (terminal->GetSelection() == terminal->GetPageIndex(singleTerminalGUI))
                    configFile.AddLine(wxString("<tab>") << "single" << wxString("</tab>"));
                else if (terminal->GetSelection() == terminal->GetPageIndex(splitTerminalGUI))
                    configFile.AddLine(wxString("<tab>") << "split" << wxString("</tab>"));

                configFile.AddLine(wxString("<showEmoticons>") << Bubble::bool2string(commManager->getSplitTerminal()->getEmoticonsEnabled()) << wxString("</showEmoticons>"));
            }
            configFile.AddLine(wxString("<baudrate>") << (int)commManager->getBaudRate() << wxString("</baudrate>"));
        }
        configFile.AddLine("</terminal>");

        configFile.AddLine("<properties>");
        if (minibloqProperties)
        {
            configFile.AddLine(wxString("<language>") << minibloqProperties->getLanguageSelection() << wxString("</language>"));
            configFile.AddLine(wxString("<profile>") << bubble.getProfile() << wxString("</profile>"));
        }
        configFile.AddLine("</properties>");

        configFile.AddLine("<canvas>");
        if (bubble.getCurrentCanvas())
        {
            configFile.AddLine(wxString("<zoom>") << bubble.getCurrentCanvas()->getZoomIndex() << wxString("</zoom>"));
        }
        configFile.AddLine("</canvas>");

        configFile.AddLine("<code>");
        if (editCode)
        {
            if (notebook)
            {
                if (notebook->GetPageIndex(editCode) != wxNOT_FOUND )
                {
                    configFile.AddLine(wxString("<shown>true</shown>"));
                    configFile.AddLine(wxString("<zoom>") << editCode->GetZoom() << wxString("</zoom>"));
                }
                else
                {
                    configFile.AddLine(wxString("<shown>false</shown>"));
                    configFile.AddLine(wxString("<zoom>") << getEditCodeZoom() << wxString("</zoom>"));
                }
            }
            else
            {
                configFile.AddLine(wxString("<zoom>") << getEditCodeZoom() << wxString("</zoom>"));
            }
        }
        else
        {
            configFile.AddLine(wxString("<zoom>") << getEditCodeZoom() << wxString("</zoom>"));
        }
        configFile.AddLine("</code>");

        configFile.AddLine("<help>");
        if (helpAndResourceCenter && help && about && license)
        {
            if (helpAndResourceCenter->GetSelection() == helpAndResourceCenter->GetPageIndex(help))
                configFile.AddLine(wxString("<tab>") << "main" << wxString("</tab>"));
            else if (helpAndResourceCenter->GetSelection() == helpAndResourceCenter->GetPageIndex(about))
                configFile.AddLine(wxString("<tab>") << "about" << wxString("</tab>"));
            else if (helpAndResourceCenter->GetSelection() == helpAndResourceCenter->GetPageIndex(license))
                configFile.AddLine(wxString("<tab>") << "license" << wxString("</tab>"));
        }
        configFile.AddLine("</help>");

//##Future:
//        configFile.AddLine("<components>");
//        configFile.AddLine("<recent>");
//        configFile.AddLine("</recent>");
//        configFile.AddLine("</components>");

        configFile.AddLine("</miniBloq>");

        configFile.Write();
        configFile.Close();

        //Format XML file:
        wxXmlDocument componentFile;
        if ( !componentFile.Load(fileName, wxString("UTF-8")) )
            return ;
        componentFile.Save(fileName, 2);
    }
    catch(...)
    {
    }
}


void MainFrame::createKeyboardAcceleratorTable()
{
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
//    unsigned int entriesCount = 54; //##Esto vuela, se levantará de XML casi con seguridad...


    unsigned int entriesCount = 24; //##Esto vuela, se levantará de XML casi con seguridad...

    //Some of these event IDs are not associated with a real menu or control:
    wxAcceleratorEntry kyboardEntries[entriesCount];

////##2011.02.24: Prueba para ver el bug del "2" en las netbooks:
//    //Main menu:
    kyboardEntries[0].Set(wxACCEL_ALT,      (int) 'F',      ID_MenuFile);
    kyboardEntries[1].Set(wxACCEL_ALT,      (int) 'E',      ID_MenuEdit);
    kyboardEntries[2].Set(wxACCEL_ALT,      (int) 'C',      ID_MenuComponent);
    kyboardEntries[3].Set(wxACCEL_ALT,      (int) 'V',      ID_MenuView);
    kyboardEntries[4].Set(wxACCEL_ALT,      (int) 'Z',      ID_QuickZoom);

    //File menu:
    kyboardEntries[5].Set(wxACCEL_CTRL,     (int) 'N',      ID_MenuFileCreateComponent);
//    kyboardEntries[6].Set(wxACCEL_CTRL,     (int) 'B',      ID_MenuFileCreateBlock);
    kyboardEntries[6].Set(wxACCEL_CTRL,     (int) 'D',      ID_MenuFileAdd);
//    kyboardEntries[8].Set(wxACCEL_CTRL,     (int) 'R',      ID_MenuFileRemove);
    kyboardEntries[7].Set(wxACCEL_CTRL,     (int) 'O',      ID_MenuFileOpen);
    kyboardEntries[8].Set(wxACCEL_CTRL,     (int) 'S',      ID_MenuFileSave);
//##@@@@
//    kyboardEntries[11].Set(wxACCEL_CTRL + wxACCEL_SHIFT,    (int) 'S', ID_MenuFileSaveAll);
//    kyboardEntries[12].Set(wxACCEL_CTRL,    (int) 'W',         ID_MenuFileClose);
//    kyboardEntries[13].Set(wxACCEL_CTRL + wxACCEL_SHIFT,    WXK_F4, ID_MenuFileCloseAll);

    //Edit menu:    //Component menu:
    kyboardEntries[9].Set(wxACCEL_CTRL,     (int) 'U',      ID_MenuComponentRun);
//    kyboardEntries[24].Set(wxACCEL_CTRL,    (int) 'P',    ID_MenuComponentDeploy);
    kyboardEntries[10].Set(wxACCEL_CTRL,     (int) 'R',      ID_MenuComponentBuild);
    //##Delete this: kyboardEntries[10].Set(wxACCEL_CTRL,    (int) 'G',      ID_MenuComponentGenerateCode);
                                              //F7 = Future uses.
//    kyboardEntries[26].Set(wxACCEL_CTRL,    WXK_F8,         ID_MenuComponentMakePortable);
    kyboardEntries[11].Set(wxACCEL_ALT,     (int) 'D',      ID_MenuComponentOpenFolder);
                                              //F9 = Stepping Debug?

    //View menu:
    //##Agregar otra tecla más para esto, que permita funcionar sin teclas de función:
    kyboardEntries[12].Set(wxACCEL_NORMAL,  WXK_F1,         ID_MenuViewHelpAndResourceCenter);
    kyboardEntries[13].Set(wxACCEL_CTRL,    (int) 'H',      ID_MenuViewHelpAndResourceCenter);
    kyboardEntries[14].Set(wxACCEL_CTRL,    (int) 'L',      ID_MenuViewLabels);

//    kyboardEntries[29].Set(wxACCEL_ALT,     (int) 'N',      ID_MenuViewComponents);
    kyboardEntries[15].Set(wxACCEL_ALT,     (int) 'H',      ID_MenuViewHardware);
//    kyboardEntries31].Set(wxACCEL_ALT,     (int) 'L',      ID_MenuViewLocalVariables);
    kyboardEntries[16].Set(wxACCEL_ALT,     (int) 'P',      ID_MenuViewProperties);
    kyboardEntries[17].Set(wxACCEL_ALT,     (int) 'M',      ID_MenuViewMessages);
    kyboardEntries[18].Set(wxACCEL_ALT,     (int) 'T',      ID_MenuViewTerminal);
    kyboardEntries[19].Set(wxACCEL_ALT,     (int) 'Q',      ID_MenuViewQuickToolbar);
    kyboardEntries[20].Set(wxACCEL_ALT,     (int) 'G',      ID_MenuViewGeneratedCode);

    kyboardEntries[21].Set(wxACCEL_ALT,     (int) 'R',      ID_HardwareManagerPopUpPort);
    kyboardEntries[22].Set(wxACCEL_ALT,     (int) 'W',      ID_HardwareManagerPopUpBoard);

    kyboardEntries[23].Set(wxACCEL_ALT,     (int) 'B',      ID_MenuViewComponentBlocks);

//##:
//    kyboardEntries[37].Set(wxACCEL_ALT,     WXK_RIGHT,      ID_MenuViewPrevView);
//    kyboardEntries[38].Set(wxACCEL_ALT,     WXK_LEFT,       ID_MenuViewNextView);

    //Window managment:
    //kyboardEntries[53].Set(wxACCEL_NORMAL,  WXK_ESCAPE,     ID_ShortCut_CloseMessages); //##Ver por qué no funciona...
    //##No: kyboardEntries[53].Set(wxACCEL_CTRL,    (int) 'W',      ID_ShortCut_CloseCurrentWindow);

    wxAcceleratorTable accel(entriesCount, kyboardEntries);
    SetAcceleratorTable(accel);
}


void MainFrame::updateGUI()
{
    //##Testing: Delete these lines:
    //wxMessageDialog dialog0(this, locale.GetCanonicalName(), wxString("GetCanonicalName")); //##Debug
    //wxMessageDialog dialog0(this, locale.GetSysName(), wxString("GetSysName")); //##Debug
    //dialog0.ShowModal(); //##Debug

    //Changes bubble's language:
    bubble.setLanguageCode(getLastAddedLocaleCatalog().BeforeLast('.'));

    //Main menu:
    updateMainMenuGUI();

    //Submenues:
    updateMenuFileGUI();
    updateMenuCreateGUI();
    updateMenuEditGUI();
    updateMenuComponentGUI();
    updateMenuViewGUI();

    if (toolMainMenu)
    {
        toolMainMenu->Update();
        toolMainMenu->Refresh();
    }

    //Quick toolbar:
    updateQuickToolbarGUI();
    updateMenuZoomGUI();

    //Help and Resource Center:
    int index = wxNOT_FOUND;
    auiManager.GetPane(wxString("HelpAndResourceCenter")).Caption(_("Help and Resource Center"));
    if (helpAndResourceCenter)
    {
        index = helpAndResourceCenter->GetPageIndex(help);
        if (index != wxNOT_FOUND)
            helpAndResourceCenter->SetPageText(index, _("Help"));
        index = helpAndResourceCenter->GetPageIndex(about);
        if (index != wxNOT_FOUND)
            helpAndResourceCenter->SetPageText(index, _("About Minibloq"));
        index = helpAndResourceCenter->GetPageIndex(license);
        if (index != wxNOT_FOUND)
            helpAndResourceCenter->SetPageText(index, _("License"));

        //Reload HMTLs:
        loadHelpAndResourceCenterFiles();
    }

    //Properties:
    auiManager.GetPane(wxString("Properties")).Caption(_("Properties"));
    if (minibloqProperties)
        minibloqProperties->updateGUI();
    //##Future: Add the other Properties' pages.

    //Terminal:
    auiManager.GetPane(wxString("Terminal")).Caption(_("Terminal"));
    if (terminal)
    {
        index = terminal->GetPageIndex(singleTerminalGUI);
        if (index != wxNOT_FOUND)
            terminal->SetPageText(index, _("Single"));
        index = terminal->GetPageIndex(splitTerminalGUI);
        if (index != wxNOT_FOUND)
            terminal->SetPageText(index, _("Split"));
    }
    if (singleTerminalGUI)
        singleTerminalGUI->updateGUI();
    if (splitTerminalGUI)
        splitTerminalGUI->updateGUI();

    //Hardware:
    auiManager.GetPane(wxString("Hardware")).Caption(_("Hardware"));
    if (hardware)
        hardware->updateGUI();

    //Messages:
    auiManager.GetPane(wxString("Messages")).Caption(_("Messages"));
    if (messages)
        messages->updateGUI();

    auiManager.Update();

    //Reloads blocks:
    bubble.enableAllBlocks(false);
    bubble.loadBlocksInfo(this, true);
    bubble.enableAllBlocks(true);

    //Current canvas:
    if (bubble.getCurrentCanvas())
    {
        bubble.getCurrentCanvas()->updateMenuEditForActionsGUI();
        bubble.getCurrentCanvas()->updateMenuEditForStartBlocksGUI();
        bubble.getCurrentCanvas()->updateMenuEditForExpressionsGUI();
        bubble.getCurrentCanvas()->updateBlocksTextGUI();
    }
}


void MainFrame::loadFileComponent(const wxString& value)
{
    //Used to open initial component from command line (ie: minibloq.exe C:\MyRoboticPrograms\LineFollower.mbqc):
    if (value != wxString(""))
    {
        wxFileName aux(value);
        bubble.setComponentPath(aux.GetPath());
        tempComponentName = aux.GetFullName();
        bubble.setComponentFilesPath(bubble.getComponentPath() + wxString("/") + aux.GetName());
        bubble.setOutputPath(bubble.getComponentFilesPath() + wxString("/output"));

        if (bubble.loadComponentFromFile(value))
        {
            componentAlreadySaved = true;

            //Changes the names of the canvas and the code tabs to the new (loaded) component name:
            aux = value;
            if (notebook)
            {
                if (bubble.getCurrentCanvas())
                    notebook->SetPageText(notebook->GetPageIndex(bubble.getCurrentCanvas()), aux.GetFullName());
                if (editCode && bubble.getHardwareManager())
                {
                    if (bubble.getHardwareManager()->getCurrentBoardProperties())
                    {
                        notebook->SetPageText(  notebook->GetPageIndex(editCode),
                                                aux.GetFullName().BeforeLast('.') + wxString(".") +
                                                bubble.getHardwareManager()->getCurrentBoardProperties()->getOutputMainFileExtension()
                                             );
                    }
                }
            }
        }
    }
}


void MainFrame::addToolsToMainMenu()
{
    if (toolMainMenu == NULL)
        return;

//##Future: Overflow items:
/*
    wxAuiToolBarItemArray prependItems;
    wxAuiToolBarItemArray appendItems;
    wxAuiToolBarItem item;
    item.SetKind(wxITEM_SEPARATOR);
    appendItems.Add(item);

    //##Internacionalizar:
    //##Setear que sólo la barra principal de menú tenga esto:
    item.SetKind(wxITEM_NORMAL);
    item.SetId(ID_CustomizeToolbar);
    item.SetLabel(_("Classic menu"));
    appendItems.Add(item);
    item.SetLabel(_("Reduced menu"));
    appendItems.Add(item);
*/
    //##No bitmaps by now:
    int iconW = 1;
    int iconH = 1;

    wxBitmap bmp0 = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(iconW, iconH)); //##Forma por ahora para hacer que no haya bitmap en el menú
    wxBitmap bmp1 = wxArtProvider::GetBitmap(wxART_TIP, wxART_OTHER, wxSize(iconW, iconH));
    wxBitmap bmp2 = wxArtProvider::GetBitmap(wxART_FIND, wxART_OTHER, wxSize(iconW, iconH));
    wxBitmap bmp3 = wxArtProvider::GetBitmap(wxART_REMOVABLE, wxART_OTHER, wxSize(iconW, iconH));

    //##Luego mejorar esto para eliminar el puntito de 1x1 pixel que aparece en estos menúes de barra (y para
    //agregar funcionamiento con teclas "Alt" estándar); creo que voy a llevar esto a menú estándar:

    toolMainMenu->AddTool(ID_MenuFile, _("File"),  bmp0);
    toolMainMenu->AddTool(ID_MenuEdit, _("Edit"),  bmp1);
    toolMainMenu->AddTool(ID_MenuComponent, _("Component"),  bmp2);
    toolMainMenu->AddTool(ID_MenuView, _("View"),  bmp3);

    toolMainMenu->SetOverflowVisible(false); //##By now there are no overflow items.
    toolMainMenu->Realize();
}


void MainFrame::deleteToolsFromMainMenu()
{
    if (toolMainMenu)
    {
        toolMainMenu->DeleteTool(ID_MenuFile);
        toolMainMenu->DeleteTool(ID_MenuEdit);
        toolMainMenu->DeleteTool(ID_MenuComponent);
        toolMainMenu->DeleteTool(ID_MenuView);
    }
}


void MainFrame::createMainMenu()
{
    //##No bitmaps by now:
    int iconW = 1;
    int iconH = 1;

    //Main menu toolbar:
    //##Cambiar el sistema de generación de IDs:
    //##Probar wxART_TOOLBAR en vez de OTHER:
    toolMainMenu = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                    wxAUI_TB_DEFAULT_STYLE |
                                    wxAUI_TB_OVERFLOW |
                                    wxAUI_TB_TEXT | //##Esto hace que aparezca el texto "estilo menú"
                                    wxAUI_TB_HORZ_TEXT);

    if (toolMainMenu)
    {
        toolMainMenu->SetToolBitmapSize(wxSize(iconW, iconH));
        addToolsToMainMenu();
    /*
        wxMenu *menuItem;
        menuItem = new wxMenuItem(toolMainMenu, ID_MenuFile, _("File"));
        toolMainMenu->Append(menuItem);
        menuItem = new wxMenuItem(toolMainMenu, ID_MenuEdit, _("Edit"));
        toolMainMenu->Append(menuItem);
        menuItem = new wxMenuItem(toolMainMenu, ID_MenuComponent, _("Component"));
        toolMainMenu->Append(menuItem);
        menuItem = new wxMenuItem(toolMainMenu, ID_MenuView, _("View"));
        toolMainMenu->Append(menuItem);
    */
        //Future: toolMainMenu->SetCustomOverflowItems(prependItems, appendItems);

        auiManager.AddPane( toolMainMenu,
                            wxAuiPaneInfo().Name(wxString("toolMainMenu")).Caption(_("Main menu")).
                            ToolbarPane().Top().Row(1).
                            LeftDockable(false).RightDockable(false)
                          );

        //##Creates all the main menu popups:
        popFile = new wxMenu;
        createMenuFile();

        popEdit = new wxMenu;
        createMenuEdit();

        popComponent = new wxMenu;
        createMenuComponent();

        popView = new wxMenu;
        createMenuView();
    }
}


void MainFrame::createMenuFile()
{
    if (popFile == NULL)
        return;

    wxImage img;
    int iconW = 16; //##Load from XML? At least make them configurable to improve accesibility.
    int iconH = 16; //##Load from XML?

#if UNDER_DEVELOPMENT
    popFileCreate = new wxMenu;
    createMenuFileCreate();
    popFile->AppendSubMenu(popFileCreate, _("Create"));
#else
    menuFileCreate = new wxMenuItem(popFile, ID_MenuFileCreate, _("Create"));
    if (menuFileCreate)
    {
        img.LoadFile(bubble.getThemePath() + wxString("/Create.png")); //##Change this image...
        menuFileCreate->SetBitmap(wxBitmap(img.Scale(iconW, iconH)));
        popFile->Append(menuFileCreate);
    }
#endif

    //Separator:
    menuFileSep0 = new wxMenuItem(popFile);
    if (menuFileSep0)
        popFile->Append(menuFileSep0);


    menuFileOpen = new wxMenuItem(popFile, ID_MenuFileOpen, _("Open\tCtrl+O"));
    if (menuFileOpen)
    {
        img.LoadFile(bubble.getThemePath() + wxString("/Open.png")); //##Change this image...
        menuFileOpen->SetBitmap(wxBitmap(img.Scale(iconW, iconH)));
        popFile->Append(menuFileOpen);
    }

    menuFileAdd = new wxMenuItem(popFile, ID_MenuFileAdd, _("Add\tCtrl+D"));
    if (menuFileAdd)
    {
        img.LoadFile(bubble.getThemePath() + wxString("/Add.png")); //##Change this image...
        menuFileAdd->SetBitmap(wxBitmap(img.Scale(iconW, iconH)));
        popFile->Append(menuFileAdd);
    }
////    menuFileRemove = new wxMenuItem(popFile, ID_MenuFileRemove, _("Remove\tCtrl+R"));
////    popFile->Append(menuFileRemove);

    menuFileExamples = new wxMenuItem(popFile, ID_MenuFileExamples, _("Examples"));
    if (menuFileExamples)
    {
        img.LoadFile(bubble.getThemePath() + wxString("/Examples.png")); //##Change this image...
        menuFileExamples->SetBitmap(wxBitmap(img.Scale(iconW, iconH)));
        popFile->Append(menuFileExamples);
    }

    menuFileSave = new wxMenuItem(popFile, ID_MenuFileSave, _("Save all\tCtrl+S"));
    //##These lines may be put together in a new function, passing only the bitmap and the sizes as params:
    if (menuFileSave)
    {
        img.LoadFile(bubble.getThemePath() + wxString("/SaveAll.png")); //##Change this image...
        menuFileSave->SetBitmap(wxBitmap(img.Scale(iconW, iconH)));
        popFile->Append(menuFileSave);
    }

//    menuFileSaveAs = new wxMenuItem(popFile, ID_MenuFileSaveAs, _("Save as"));
//    if (menuFileSaveAs)
//    {
//        img.LoadFile(bubble.getThemePath() + wxString("/SaveAs.png")); //##Change this image...
//        menuFileSaveAs->SetBitmap(wxBitmap(img.Scale(iconW, iconH)));
//        popFile->Append(menuFileSaveAs);
//    }

#if UNDER_DEVELOPMENT
        //Separator:
        menuItem = new wxMenuItem(popFile);
        popFile->Append(menuItem);
        menuItem = new wxMenuItem(popFile, ID_MenuFileClose, _("Close\tCtrl+W"));
        popFile->Append(menuItem);
        menuItem = new wxMenuItem(popFile, ID_MenuFileCloseAll, _("Close all\tCtrl + Shift+W"));
        popFile->Append(menuItem);
        //Separator:
        menuItem = new wxMenuItem(popFile);
        popFile->Append(menuItem);
        menuItem = new wxMenuItem(popFile, ID_MenuFilePrint, _("Print"));
        popFile->Append(menuItem);
        menuItem = new wxMenuItem(popFile, ID_MenuFilePrint, _("Export as image to clipboard")); //##Futuro: ¿Usar tecla rápida?
        popFile->Append(menuItem);
#endif
    //Separator:
    menuFileSep1 = new wxMenuItem(popFile);
    if (menuFileSep1)
        popFile->Append(menuFileSep1);

    //##Ver si elimino esta tecla de función del menú, por ejemplo por "Ctrl+X":
    menuFileExit = new wxMenuItem(popFile, wxID_EXIT, _("Exit\tAlt+F4"));
    if (menuFileExit)
    {
        img.LoadFile(bubble.getThemePath() + wxString("/Exit.png")); //##Change this image...
        menuFileExit->SetBitmap(wxBitmap(img.Scale(iconW, iconH)));
        popFile->Append(menuFileExit);
    }

    //##Future: This may be a toolbar instead of a menu:
/*
    //##No bitmaps by now:
    int iconW = 1;
    int iconH = 1;

    //##Para optimizar, se pueden levantar todos los bitmaps que se utilizarán, y luego irlos pasando
    //como parámetros, al menos en la primera versión:
    //##Hacer un macro o una función para esto:
    menuItem = new wxMenuItem(popFile, 101, _("Create"));
    menuItem->SetBitmap(wxArtProvider::GetBitmap(wxART_NEW, wxART_OTHER, wxSize(iconW, iconH)));
    popFile->Append(menuItem);
    menuItem = new wxMenuItem(popFile, 101, _("Add"));
    menuItem->SetBitmap(wxArtProvider::GetBitmap(wxART_ADD_BOOKMARK, wxART_OTHER, wxSize(iconW, iconH)));
    popFile->Append(menuItem);
    menuItem = new wxMenuItem(popFile, 101, _("Remove"));
    menuItem->SetBitmap(wxArtProvider::GetBitmap(wxART_DEL_BOOKMARK, wxART_OTHER, wxSize(iconW, iconH)));
    popFile->Append(menuItem);
    //Separator:
    menuItem = new wxMenuItem(popFile);
    popFile->Append(menuItem);
    menuItem = new wxMenuItem(popFile, 101, _("Open"));
    menuItem->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_OTHER, wxSize(iconW, iconH)));
    popFile->Append(menuItem);
    menuItem = new wxMenuItem(popFile, 101, _("Save"));
    menuItem->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_OTHER, wxSize(iconW, iconH)));
    popFile->Append(menuItem);
    menuItem = new wxMenuItem(popFile, 101, _("Save as"));
    menuItem->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS, wxART_OTHER, wxSize(iconW, iconH)));
    popFile->Append(menuItem);
    menuItem = new wxMenuItem(popFile, 101, _("Save all"));
    menuItem->SetBitmap(wxArtProvider::GetBitmap(wxART_CDROM, wxART_OTHER, wxSize(iconW, iconH)));
    popFile->Append(menuItem);
    //Separator:
    menuItem = new wxMenuItem(popFile);
    popFile->Append(menuItem);
    menuItem = new wxMenuItem(popFile, 101, _("Close"));
    menuItem->SetBitmap(wxArtProvider::GetBitmap(wxART_DEL_BOOKMARK, wxART_OTHER, wxSize(iconW, iconH)));
    menuItem = new wxMenuItem(popFile, 101, _("Close all"));
    menuItem->SetBitmap(wxArtProvider::GetBitmap(wxART_DELETE, wxART_OTHER, wxSize(iconW, iconH)));
    popFile->Append(menuItem);
    menuItem = new wxMenuItem(popFile); //Separator
    popFile->Append(menuItem);
    menuItem = new wxMenuItem(popFile, 101, _("Print"));
    menuItem->SetBitmap(wxArtProvider::GetBitmap(wxART_PRINT, wxART_OTHER, wxSize(iconW, iconH)));
    popFile->Append(menuItem);
    menuItem = new wxMenuItem(popFile); //Separator
    popFile->Append(menuItem);
    menuItem = new wxMenuItem(popFile, 101, _("Exit"));
    menuItem->SetBitmap(wxArtProvider::GetBitmap(wxART_QUIT, wxART_OTHER, wxSize(iconW, iconH)));
    popFile->Append(menuItem);
*/
}


void MainFrame::createMenuFileCreate()
{
    if (popFileCreate == NULL)
        return;

//##:
//    wxImage img;
//    int iconW = 16; //##Load from XML? At least make them configurable to improve accesibility.
//    int iconH = 16; //##Load from XML?

    //##Pasar esto a la forma del método:
    // Append(int id, const wxString& item = "", const wxString& helpString = "", wxItemKind kind = wxITEM_NORMAL):
    menuFileCreateComponent = new wxMenuItem(popFileCreate, ID_MenuFileCreateComponent, _("Component\tCtrl+N"));
    if (menuFileCreateComponent)
    {
        //img.LoadFile(bubble.getThemePath() + wxString("/Create.png")); //##Change this image...
        //menuFileCreateComponent->SetBitmap(wxBitmap(img.Scale(iconW, iconH)));
        popFileCreate->Append(menuFileCreateComponent);
    }

    menuFileCreateBlock = new wxMenuItem(popFileCreate, ID_MenuFileCreateBlock, _("Block\tCtrl+B"));
    if (menuFileCreateBlock)
    {
        //img.LoadFile(bubble.getThemePath() + wxString("/Create.png")); //##Change this image...
        //menuFileCreateBlock->SetBitmap(wxBitmap(img.Scale(iconW, iconH)));
        popFileCreate->Append(menuFileCreateBlock);
    }
}


void MainFrame::createMenuEdit()
{
    if (popEdit == NULL)
        return;

    wxImage img;
    int iconW = 16; //##Load from XML? At least make them configurable to improve accesibility.
    int iconH = 16; //##Load from XML?

    //##Pasar esto a la forma del método:
    // Append(int id, const wxString& item = "", const wxString& helpString = "", wxItemKind kind = wxITEM_NORMAL):

#if UNDER_DEVELOPMENT
    menuEditUndo = new wxMenuItem(popEdit, ID_MenuEditUndo, _("Undo\tCtrl+Z"));
    if (menuEditUndo)
    {
        img.LoadFile(bubble.getThemePath() + wxString("/Undo.png")); //##Change this image...
        menuEditUndo->SetBitmap(wxBitmap(img.Scale(iconW, iconH)));
        popEdit->Append(menuEditUndo);
    }
    menuEditRedo = new wxMenuItem(popEdit, ID_MenuEditRedo, _("Redo\tCtrl+Y"));
    if (menuEditRedo)
    {
        img.LoadFile(bubble.getThemePath() + wxString("/Redo.png")); //##Change this image...
        menuEditRedo->SetBitmap(wxBitmap(img.Scale(iconW, iconH)));
        popEdit->Append(menuEditRedo);
    }

    //Separator:
    menuEditSep0 = new wxMenuItem(popEdit);
    if (menuEditSep0)
        popEdit->Append(menuEditSep0);
#endif

#if UNDER_DEVELOPMENT
    menuEditSelectAll = new wxMenuItem(popEdit, ID_MenuEditSelectAll, _("Select all\tCtrl+A"));
    popEdit->Append(menuEditSelectAll);
#endif


//##Implement this as soon as possible:
#if UNDER_DEVELOPMENT
    menuEditCopy = new wxMenuItem(popEdit, ID_MenuEditCopy, _("Copy\tCtrl+C"));
    if (menuEditCopy)
    {
        img.LoadFile(bubble.getThemePath() + wxString("/Copy.png")); //##Change this image...
        menuEditCopy->SetBitmap(wxBitmap(img.Scale(iconW, iconH)));
        popEdit->Append(menuEditCopy);
    }
#endif

    menuEditCut = new wxMenuItem(popEdit, ID_MenuEditCut, _("Cut\tCtrl+X"));
#if !UNDER_DEVELOPMENT
#if defined (WIN32)
    if (menuEditCut)
        menuEditCut->Enable(false);
#endif
#endif
    if (menuEditCut)
    {
        img.LoadFile(bubble.getThemePath() + wxString("/Cut.png")); //##Change this image...
        menuEditCut->SetBitmap(wxBitmap(img.Scale(iconW, iconH)));
        popEdit->Append(menuEditCut);
    }

    menuEditPaste = new wxMenuItem(popEdit, ID_MenuEditPaste, _("Paste\tCtrl+V"));
    if (menuEditPaste)
    {
        img.LoadFile(bubble.getThemePath() + wxString("/Paste.png")); //##Change this image...
        menuEditPaste->SetBitmap(wxBitmap(img.Scale(iconW, iconH)));
        popEdit->Append(menuEditPaste);
    }

    menuEditDelete = new wxMenuItem(popEdit, ID_MenuEditDelete, _("Delete\tAlt+Del"));
    if (menuEditDelete)
    {
        img.LoadFile(bubble.getThemePath() + wxString("/Delete.png")); //##Change this image...
        menuEditDelete->SetBitmap(wxBitmap(img.Scale(iconW, iconH)));
        popEdit->Append(menuEditDelete);
    }

    //Separator:
    menuEditSep1 = new wxMenuItem(popEdit);
    if (menuEditSep1)
        popEdit->Append(menuEditSep1);
    //##Futuro:Ver si habrá un replace...

    menuCommentUncomment = new wxMenuItem(popEdit, ID_MenuCommentUncomment, _("Comment/Uncomment"));
    if (menuCommentUncomment)
    {
        img.LoadFile(bubble.getThemePath() + wxString("/CommentUncomment.png")); //##Change this image...
        menuCommentUncomment->SetBitmap(wxBitmap(img.Scale(iconW, iconH)));
        popEdit->Append(menuCommentUncomment);
    }

#if UNDER_DEVELOPMENT
    menuEditFind = new wxMenuItem(popEdit, ID_MenuEditFind, _("Find\tCtrl+F"));
    popEdit->Append(menuEditFind);

//##Ver si elimino esta tecla de función del menú:
    menuEditFindNext = new wxMenuItem(popEdit, ID_MenuEditFindNext, _("Find next\tF3"));
    popEdit->Append(menuEditFindNext);
#endif
}


void MainFrame::createMenuComponent()
{
    if (popComponent == NULL)
        return;

    //##There must be an option on the program settings (here "properties") to enable and disable menu images:

    //##Unifiy these images with the quicktoolbar creation:
    wxImage img;
    int iconW = 16; //##Load from XML? At least make them configurable to improve accesibility.
    int iconH = 16; //##Load from XML?

    if (popComponent == NULL)
        return;

    //##Pasar esto a la forma del método:
    // Append(int id, const wxString& item = "", const wxString& helpString = "", wxItemKind kind = wxITEM_NORMAL):
    menuComponentRun = new wxMenuItem(popComponent, ID_MenuComponentRun, _("Run\tCtrl+U"));
    if (menuComponentRun)
    {
        img.LoadFile(bubble.getThemePath() + wxString("/Run.png"));
        menuComponentRun->SetBitmap(wxBitmap(img.Scale(iconW, iconH)));
        popComponent->Append(menuComponentRun);
    }
#if UNDER_DEVELOPMENT
    menuComponentDeploy = new wxMenuItem(popComponent, ID_MenuComponentDeploy, _("Deploy\tCtrl+P"));
    if (menuComponentDeploy)
    {
        img.LoadFile(bubble.getThemePath() + wxString("/Deploy.png"));
        menuComponentDeploy->SetBitmap(wxBitmap(img.Scale(iconW, iconH)));
        popComponent->Append(menuComponentDeploy);
    }
#endif
    menuComponentBuild = new wxMenuItem(popComponent, ID_MenuComponentBuild, _("Build\tCtrl+R"));
    if (menuComponentBuild)
    {
        img.LoadFile(bubble.getThemePath() + wxString("/Build.png"));
        menuComponentBuild->SetBitmap(wxBitmap(img.Scale(iconW, iconH)));
        popComponent->Append(menuComponentBuild);
    }

    //Separator:
    menuComponentSep0 = new wxMenuItem(popComponent);
    if (menuComponentSep0)
        popComponent->Append(menuComponentSep0);

#if UNDER_DEVELOPMENT
    menuComponentMakePortable = new wxMenuItem(popComponent, ID_MenuComponentMakePortable, _("Make portable\tF8")); //##Eliminar las "Fx" que no están en todas las máquinas (como la OLPC).
    if (menuComponentMakePortable)
        popComponent->Append(menuComponentMakePortable);
#endif
    menuComponentOpenFolder = new wxMenuItem(popComponent, ID_MenuComponentOpenFolder, _("Open folder\tAlt+D"));
    if (menuComponentOpenFolder)
    {
        img.LoadFile(bubble.getThemePath() + wxString("/OpenFolder.png"));
        menuComponentOpenFolder->SetBitmap(wxBitmap(img.Scale(iconW, iconH)));
        popComponent->Append(menuComponentOpenFolder);
    }

/*
    //##No bitmaps by now:
    int iconW = 1;
    int iconH = 1;

    menuItem = new wxMenuItem(popComponent, 101, _("Run"));
    menuItem->SetBitmap(wxArtProvider::GetBitmap(wxART_UNDO, wxART_OTHER, wxSize(iconW, iconH)));
    popComponent->Append(menuItem);
    menuItem = new wxMenuItem(popComponent, 101, _("Deploy"));
    menuItem->SetBitmap(wxArtProvider::GetBitmap(wxART_REDO, wxART_OTHER, wxSize(iconW, iconH)));
    popComponent->Append(menuItem);
    menuItem = new wxMenuItem(popComponent, 101, _("Build"));
    menuItem->SetBitmap(wxArtProvider::GetBitmap(wxART_REDO, wxART_OTHER, wxSize(iconW, iconH)));
    popComponent->Append(menuItem);
    //Separator:
    menuItem = new wxMenuItem(popComponent);
    popComponent->Append(menuItem);
    menuItem = new wxMenuItem(popComponent, 101, _("Make portable"));
    menuItem->SetBitmap(wxArtProvider::GetBitmap(wxART_REPORT_VIEW, wxART_OTHER, wxSize(iconW, iconH)));
    popComponent->Append(menuItem);
*/
}


void MainFrame::createMenuView()
{
    if (popView == NULL)
        return;

    //##Unifiy these images with the quicktoolbar creation:
//    wxBitmap bmp;
//    wxImage img;
//    int iconW = 16; //##Load from XML? At least make them configurable to improve accesibility.
//    int iconH = 16; //##Load from XML?

    //##Los menuItems deben ser private members, no una variable temporal de este método, al menos en este
    //menú, porque hay que mantener sus "checks" sincronizados con la visualización de las ventanas a las
    //que están asociados:

    //##Pasar esto a la forma del método:
    // Append(int id, const wxString& item = "", const wxString& helpString = "", wxItemKind kind = wxITEM_NORMAL):
    menuViewHelpAndResourceCenter = new wxMenuItem( popView, ID_MenuViewHelpAndResourceCenter,
                                                    _("Help and Resource Center\tCtrl+H"),
                                                    wxString(""), wxITEM_CHECK);
    if (menuViewHelpAndResourceCenter)
    {
        popView->Append(menuViewHelpAndResourceCenter);
    }

#if UNDER_DEVELOPMENT
    menuViewLabels = new wxMenuItem(popView, ID_MenuViewLabels, _("Labels\tCtrl+L"),
                                    wxString(""), wxITEM_CHECK);
    if (menuViewLabels)
    {
        //menuViewLabels->Check(true);
        popView->Append(menuViewLabels);
    }
#endif

    //Separator:
    menuViewSep0 = new wxMenuItem(popView);
    if (menuViewSep0)
    {
        popView->Append(menuViewSep0);
    }

    menuViewQuickToolbar = new wxMenuItem(popView, ID_MenuViewQuickToolbar, _("Quick toolbar\tAlt+Q"), wxString(""), wxITEM_CHECK);
    if (menuViewQuickToolbar)
    {
        popView->Append(menuViewQuickToolbar);
        //menuViewQuickToolbar->Check(true);
    }

    //Separator:
    menuViewSep1 = new wxMenuItem(popView);
    if (menuViewSep1)
    {
        popView->Append(menuViewSep1);
    }

#if UNDER_DEVELOPMENT
    menuViewComponents = new wxMenuItem(popView, ID_MenuViewComponents, _("Components\tAlt+N"), wxString(""), wxITEM_CHECK);
    popView->Append(menuViewComponents);
#endif

    menuViewComponentBlocks = new wxMenuItem(popView, ID_MenuViewComponentBlocks, _("Component's blocks\tAlt+B"), wxString(""), wxITEM_CHECK);
    if (menuViewComponentBlocks)
    {
        popView->Append(menuViewComponentBlocks);
    }

    menuViewGeneratedCode = new wxMenuItem(popView, ID_MenuViewGeneratedCode, _("Generated code\tAlt+G"), wxString(""), wxITEM_CHECK);
    if (menuViewGeneratedCode)
    {
        popView->Append(menuViewGeneratedCode);
    }

    menuViewHardware = new wxMenuItem(popView, ID_MenuViewHardware, _("Hardware\tAlt+H"), wxString(""), wxITEM_CHECK);
    if (menuViewHardware)
    {
        //##Optimization: This bitmap is the same for the menu and for the quickbar, so this code must be
        //unified and be executed only once:
//##Needs a checked and an unchecked bitmap to work nice:
//        img.LoadFile(bubble.getThemePath() + wxString("/Hardware.png"));
//        bmp = wxBitmap(img.Scale(iconW, iconH));
//        menuViewHardware->SetBitmap(bmp);
        popView->Append(menuViewHardware);
        //menuViewHardware->Check(true);
    }

    menuViewProperties = new wxMenuItem(popView, ID_MenuViewProperties, _("Properties\tAlt+P"), wxString(""), wxITEM_CHECK);
    if (menuViewProperties)
    {
        popView->Append(menuViewProperties);
    }


#if UNDER_DEVELOPMENT
    menuViewLocalVariables = new wxMenuItem(popView, ID_MenuViewLocalVariables, _("Local variables\tAlt++L"), wxString(""), wxITEM_CHECK);
    popView->Append(menuViewLocalVariables);
#endif


    menuViewMessages = new wxMenuItem(popView, ID_MenuViewMessages, _("Messages\tAlt+M"), wxString(""), wxITEM_CHECK);
    if (menuViewMessages)
    {
        popView->Append(menuViewMessages);
    }

    menuViewTerminal = new wxMenuItem(popView, ID_MenuViewTerminal, _("Terminal\tAlt+T"), wxString(""), wxITEM_CHECK);
    if (menuViewTerminal)
    {
        popView->Append(menuViewTerminal);
        //menuViewTerminal->Check(true);
    }

    //##$: NOTA IMPORTANTE: Ver que coloqué una "L" antes del string constant "Código generado\tAlt+G", que es la que indica
    //que el string es un string de wchar_t, en vez de char, por lo que reconoce los acentos automáticamente (¡SIN ESO NO FUNCIONA!).
    //Probar cómo se hará cuando levante los strings del XML de internacionalización...
    //
#if UNDER_DEVELOPMENT
    /*##No implementado por ahora:
    //Separator:
    menuItem = new wxMenuItem(popView);
    popView->Append(menuItem);
    menuItem = new wxMenuItem(popView, ID_MenuViewFullScreen, _("Full screen\tF11"), wxString(""), wxITEM_CHECK);
    popView->Append(menuItem);
    */
    //Separator:
    //menuItemPop = new wxMenuItem(popView);
    //if (menuItemPop)
        //popView->Append(menuItemPop);
    menuViewPrevView = new wxMenuItem(popView, ID_MenuViewPrevView, _("Prev view\tAlt+Left"));
    if (menuViewPrevView)
    {
        popView->Append(menuViewPrevView);
    }

    menuViewNextView = new wxMenuItem(popView, ID_MenuViewNextView, _("Next view\tAlt+Right"));
    if (menuViewNextView)
    {
        popView->Append(menuViewNextView);
    }
#endif
}


void MainFrame::createQuickToolbar()
{
    //##Futuro: Agregar un dropdown de overflow items (append) donde se pueda configurar rápidamente el tamaño de esta barra en
    //Small (16x16), Medium(32x32), Large (48x48/64x64): IconH y IconW deberían ser propiedades de cada barra y menú, no globales.
    //Quick toolbar:
    //##Esto ahora es para todos los íconos (menú, quicktoolbar, etc., pero hay que pasarlo para que cada toolbar tenga su
    //tamaño configurable):
    int iconW = 16; //##
    int iconH = 16; //##
//##Esto se vé muy bien también:
//    iconW = 32; //##
//    iconH = 32; //##

    //##Optimización futura: Hacer que éstos sean private, para que no los construya cada vez que entra acá:
    wxBitmap bmp;
    wxImage img;

    wxAuiToolBarItem item;

    /*
    wxImage img("C:/Projects/Multiplo/Soft/Minibloq-RG/v1.0/Soft/Bin/Minibloq-RG.v1.0/Components/Minibloq/v1.0/GUI/Images/Add.png",
                 wxBITMAP_TYPE_PNG);
    */
    /*
    //On Windows, GetDataDir() returns the exe dir without the exe name itself. To port this app, here is
    //necessary conditional compilation:
    wxString imagesPath(wxStandardPaths::Get().GetDataDir() + wxString("/GUI/Images"));
    wxImage img(imagesPath + wxString("/Add.png"), wxBITMAP_TYPE_PNG);
    //bmp = wxBitmap(img.Scale(iconW, iconH, wxIMAGE_QUALITY_HIGH)); //Slower but better.
    bmp = wxBitmap(img.Scale(iconW, iconH)); //Faster...
    */
    //On Windows, GetDataDir() returns the exe dir without the exe name itself. To port this app, here is
    //necessary conditional compilation:
    //##Los ID cambiarán, para poder asociar todo esto a eventos
    //(ver http://biolpc22.york.ac.uk/wx/docs/html/wx/wx_eventhandlingoverview.html#windowids):

    //##Ver si se necesitan los ID "Quick" o si todos estos botones mejor se mapean directamente a los IDs de
    //menúes existentes (como se hace más abajo con ViewGeneratedCode y ViewHardware.

    toolQuick = new wxAuiToolBar(   this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                    wxAUI_TB_DEFAULT_STYLE |
                                    wxAUI_TB_OVERFLOW |
                                    wxAUI_TB_HORZ_TEXT);
    if (toolQuick)
    {
        toolQuick->SetToolBitmapSize(wxSize(iconW, iconH));

        img.LoadFile(bubble.getThemePath() + wxString("/Open.png"));
        bmp = wxBitmap(img.Scale(iconW, iconH));
        //##En cuanto esté la funcionalidad SaveAll activada, éste botón será saveAll, no sólo Save, como ahora:
        //toolQuick->AddTool(ID_QuickFileSaveAll, wxString(""), bmp, _("Save all"));
        toolQuick->AddTool(ID_QuickFileOpen, wxString(""), bmp, _("Open")); //##

        img.LoadFile(bubble.getThemePath() + wxString("/Add.png"));
        bmp = wxBitmap(img.Scale(iconW, iconH));
        toolQuick->AddTool(ID_QuickFileAdd, wxString(""), bmp, _("Add"));

        img.LoadFile(bubble.getThemePath() + wxString("/SaveAll.png"));
        bmp = wxBitmap(img.Scale(iconW, iconH));
        toolQuick->AddTool(ID_QuickFileSaveAll, wxString(""), bmp, _("Save all"));

    #if UNDER_DEVELOPMENT
        img.LoadFile(bubble.getThemePath() + wxString("/Create.png"));
        //bmp = wxBitmap(img.Scale(iconW, iconH, wxIMAGE_QUALITY_HIGH)); //Slower but better.
        bmp = wxBitmap(img.Scale(iconW, iconH)); //Faster...
        //toolQuick->AddTool(ID_QuickFileCreate, _("Create"), bmp);
        toolQuick->AddTool(ID_QuickFileCreate, _wxString(""), bmp, _("Create block"));
        //toolQuick->AddSeparator();
    #endif

        img.LoadFile(bubble.getThemePath() + wxString("/Run.png"));
        bmp = wxBitmap(img.Scale(iconW, iconH));
        toolQuick->AddTool(ID_QuickComponentRun, wxString(""), bmp, _("Run"));

    #if UNDER_DEVELOPMENT
        img.LoadFile(bubble.getThemePath() + wxString("/Deploy.png"));
        bmp = wxBitmap(img.Scale(iconW, iconH));
        toolQuick->AddTool(ID_QuickComponentDeploy, wxString(""), bmp, _("Deploy"));
    #endif
        toolQuick->AddSeparator();

    //##This one will be added (together with a menu entry) when the online /real-time execution comes to work:
    //    img.LoadFile(imagesPath + wxString("/Stop.png"));
    //    bmp = wxBitmap(img.Scale(iconW, iconH));
    //    //toolQuick->AddTool(ID_QuickComponentStop, wxString(""), bmp, _("Stop"));
    //    toolQuick->AddSeparator();

        img.LoadFile(bubble.getThemePath() + wxString("/Blocks.png"));
        bmp = wxBitmap(img.Scale(iconW, iconH));
        toolQuick->AddTool(ID_MenuViewComponentBlocks, wxString(""), bmp, _("Component's blocks"));

        img.LoadFile(bubble.getThemePath() + wxString("/GeneratedCode.png"));
        bmp = wxBitmap(img.Scale(iconW, iconH));
        toolQuick->AddTool(ID_MenuViewGeneratedCode, wxString(""), bmp, _("Generated code"));

        img.LoadFile(bubble.getThemePath() + wxString("/Hardware.png"));
        bmp = wxBitmap(img.Scale(iconW, iconH));
        toolQuick->AddTool(ID_MenuViewHardware, wxString(""), bmp, _("Hardware"));

        img.LoadFile(bubble.getThemePath() + wxString("/Terminal.png"));
        bmp = wxBitmap(img.Scale(iconW, iconH));
        toolQuick->AddTool(ID_QuickViewTerminal, wxString(""), bmp, _("Terminal"));

        img.LoadFile(bubble.getThemePath() + wxString("/Help.png"));
        bmp = wxBitmap(img.Scale(iconW, iconH));
        toolQuick->AddTool(ID_QuickViewHelpAndResourceCenter, wxString(""), bmp, _("Help and Resource Center"));

        toolQuick->AddSeparator();

        img.LoadFile(bubble.getThemePath() + wxString("/Zoom.png"));
        bmp = wxBitmap(img.Scale(iconW, iconH));

        //##Guardamos el puntero del toolZoom para cambiarle el label después:
        toolZoom = toolQuick->AddTool(ID_QuickZoom, _("100 %"), bmp, _("Zoom"));

        toolQuick->SetOverflowVisible(false);
        toolQuick->Realize();

        popZoom = new wxMenu;
        createMenuZoom();

        //##Futuro: La barrita rápida será personalizable:
        auiManager.AddPane( toolQuick, wxAuiPaneInfo().
                            Name(wxString("QuickToolBar")).Caption(_("Quick toolbar")).
                            ToolbarPane().Top().Row(1).Position(2).
                            LeftDockable(false).RightDockable(false)
                          );
    }
}


void MainFrame::createMenuZoom()
{
    if (popZoom == NULL)
        return;

    //Ver qué pasa con el Id = 101 de todos estos:
    //##Para optimizar, se pueden levantar todos los bitmaps que se utilizarán, y luego irlos pasando
    //como parámetros, al menos en la primera versión:
    //##Pasar esto a la forma del método:
    // Append(int id, const wxString& item = "", const wxString& helpString = "", wxItemKind kind = wxITEM_NORMAL):
    //##Obtener estos valores de la clase BubbleCanvas (hacer algún tipo de enum que los tenga, y
    //convertir su valor a string):
    menuZoom10 = new wxMenuItem(popZoom, ID_QuickZoom10, _("10 %"), wxString(""), wxITEM_RADIO);
    popZoom->Append(menuZoom10);
    menuZoom25 = new wxMenuItem(popZoom, ID_QuickZoom25, _("25 %"), wxString(""), wxITEM_RADIO);
    popZoom->Append(menuZoom25);
    menuZoom50 = new wxMenuItem(popZoom, ID_QuickZoom50, _("50 %"), wxString(""), wxITEM_RADIO);
    popZoom->Append(menuZoom50);
    menuZoom75 = new wxMenuItem(popZoom, ID_QuickZoom75, _("75 %"), wxString(""), wxITEM_RADIO);
    popZoom->Append(menuZoom75);
    menuZoom100 = new wxMenuItem(popZoom, ID_QuickZoom100, _("100 %"), wxString(""), wxITEM_RADIO);
    popZoom->Append(menuZoom100);
    menuZoom100->Check(true); //##Esto se levanta del archivo de configuración
    menuZoom125 = new wxMenuItem(popZoom, ID_QuickZoom125, _("125 %"), wxString(""), wxITEM_RADIO);
    popZoom->Append(menuZoom125);
    menuZoom150 = new wxMenuItem(popZoom, ID_QuickZoom150, _("150 %"), wxString(""), wxITEM_RADIO);
    popZoom->Append(menuZoom150);
    menuZoom175 = new wxMenuItem(popZoom, ID_QuickZoom175, _("175 %"), wxString(""), wxITEM_RADIO);
    popZoom->Append(menuZoom175);
    menuZoom200 = new wxMenuItem(popZoom, ID_QuickZoom200, _("200 %"), wxString(""), wxITEM_RADIO);
    popZoom->Append(menuZoom200);
//##Futuro:
//    menuZoomViewAll = new wxMenuItem(popZoom, ID_QuickZoomViewAll, _("View all"), wxString(""), wxITEM_RADIO);
//    popZoom->Append(menuZoomViewAll);
}


void MainFrame::updateMainMenuGUI()
{
    deleteToolsFromMainMenu();
    addToolsToMainMenu();

    if (toolMainMenu)
    {
        //Without these calls, the menu is not resized for the new loaded language:
        auiManager.DetachPane(toolMainMenu);
        auiManager.AddPane( toolMainMenu,
                            wxAuiPaneInfo().Name(wxString("toolMainMenu")).Caption(_("Main menu")).
                            ToolbarPane().Top().Row(1).
                            LeftDockable(false).RightDockable(false)
                          );
    }

//    //##Future: Improve this:
//    if (toolMainMenu == NULL)
//        return;
//    wxAuiToolBarItem *toolItem = NULL;
//    toolItem = toolMainMenu->FindTool(ID_MenuFile);
//    if (toolItem)
//        toolItem->SetLabel(_("File"));
//    toolItem = NULL; //Not necessary, but more secure (since FindTool docmentation is poor).
//    toolItem = toolMainMenu->FindTool(ID_MenuEdit);
//    if (toolItem)
//        toolItem->SetLabel(_("Edit"));
//    toolItem = NULL;
//    toolItem = toolMainMenu->FindTool(ID_MenuComponent);
//    if (toolItem)
//        toolItem->SetLabel(_("Component"));
//    toolItem = NULL;
//    toolItem = toolMainMenu->FindTool(ID_MenuView);
//    if (toolItem)
//        toolItem->SetLabel(_("View"));
}


void MainFrame::updateMenuFileGUI()
{
    if (popFile == NULL)
        return;

    if (menuFileCreate)
    {
        popFile->Destroy(menuFileCreate);
        menuFileCreate = NULL;
    }
    if (menuFileOpen)
    {
        popFile->Destroy(menuFileOpen);
        menuFileOpen = NULL;
    }
    if (menuFileAdd)
    {
        popFile->Destroy(menuFileAdd);
        menuFileAdd = NULL;
    }
    if (menuFileExamples)
    {
        popFile->Destroy(menuFileExamples);
        menuFileExamples = NULL;
    }
    if (menuFileSave)
    {
        popFile->Destroy(menuFileSave);
        menuFileSave = NULL;
    }
//    if (menuFileSaveAs)
//    {
//        popFile->Destroy(menuFileSaveAs);
//        menuFileSaveAs = NULL;
//    }
    if (menuFileExit)
    {
        popFile->Destroy(menuFileExit);
        menuFileExit = NULL;
    }
    if (menuFileSep0)
    {
        popFile->Destroy(menuFileSep0);
        menuFileSep0 = NULL;
    }
    if (menuFileSep1)
    {
        popFile->Destroy(menuFileSep1);
        menuFileSep1 = NULL;
    }

    createMenuFile();
}


void MainFrame::updateMenuCreateGUI()
{
    if (popFileCreate == NULL)
        return;

    if (menuFileCreateComponent)
    {
        popFileCreate->Destroy(menuFileCreateComponent);
        menuFileCreateComponent = NULL;
    }
    if (menuFileCreateBlock)
    {
        popFileCreate->Destroy(menuFileCreateBlock);
        menuFileCreateBlock = NULL;
    }

    createMenuFileCreate();
}


void MainFrame::updateMenuEditGUI()
{
    if (popEdit == NULL)
        return;

    if (menuEditUndo)
    {
        popEdit->Destroy(menuEditUndo);
        menuEditUndo = NULL;
    }
#if UNDER_DEVELOPMENT
//##
    if (menuEditRedo)
    {
        popEdit->Destroy(menuEditRedo);
        menuEditRedo = NULL;
    }
    if (menuEditSelectAll)
    {
        popEdit->Destroy(menuEditSelectAll);
        menuEditSelectAll = NULL;
    }
    if (menuEditCopy)
    {
        popEdit->Destroy(menuEditCopy);
        menuEditCopy = NULL;
    }
#endif
    if (menuEditCut)
    {
        popEdit->Destroy(menuEditCut);
        menuEditCut = NULL;
    }
    if (menuEditPaste)
    {
        popEdit->Destroy(menuEditPaste);
        menuEditPaste = NULL;
    }
    if (menuEditDelete)
    {
        popEdit->Destroy(menuEditDelete);
        menuEditDelete = NULL;
    }
    if (menuCommentUncomment)
    {
        popEdit->Destroy(menuCommentUncomment);
        menuCommentUncomment = NULL;
    }
    if (menuEditSep0)
    {
        popEdit->Destroy(menuEditSep0);
        menuEditSep0 = NULL;
    }
    if (menuEditSep1)
    {
        popEdit->Destroy(menuEditSep1);
        menuEditSep1 = NULL;
    }
    //if (menuEditFind)
    //{
        //popEdit->Destroy(menuEditFind); //##
        //menuEditFind = NULL;
    //}

    createMenuEdit();
}


void MainFrame::updateMenuComponentGUI()
{
    if (popComponent == NULL)
        return;

    if (menuComponentRun)
    {
        popComponent->Destroy(menuComponentRun);
        menuComponentRun = NULL;
    }
    if (menuComponentBuild)
    {
        popComponent->Destroy(menuComponentBuild);
        menuComponentBuild = NULL;
    }
    if (menuComponentSep0)
    {
        popComponent->Destroy(menuComponentSep0);
        menuComponentSep0 = NULL;
    }
    if (menuComponentOpenFolder)
    {
        popComponent->Destroy(menuComponentOpenFolder);
        menuComponentOpenFolder = NULL;
    }

    createMenuComponent();
}


void MainFrame::updateMenuViewGUI()
{
    if (popView == NULL)
        return;

    if (menuViewHelpAndResourceCenter)
    {
        popView->Destroy(menuViewHelpAndResourceCenter);
        menuViewHelpAndResourceCenter = NULL;
    }
#if UNDER_DEVELOPMENT
    if (menuViewLabels)
    {
        popView->Destroy(menuViewLabels);
        menuViewLabels = NULL;
    }
#endif
    if (menuViewSep0)
    {
        popView->Destroy(menuViewSep0);
        menuViewSep0 = NULL;
    }
    if (menuViewComponents)
    {
        popView->Destroy(menuViewComponents);
        menuViewComponents = NULL;
    }
    if (menuViewHardware)
    {
        popView->Destroy(menuViewHardware);
        menuViewHardware = NULL;
    }
#if UNDER_DEVELOPMENT
    if (menuViewLocalVariables)
    {
        popView->Destroy(menuViewLocalVariables);
        menuViewLocalVariables = NULL;
    }
#endif
    if (menuViewProperties)
    {
        popView->Destroy(menuViewProperties);
        menuViewProperties = NULL;
    }
    if (menuViewMessages)
    {
        popView->Destroy(menuViewMessages);
        menuViewMessages = NULL;
    }
    if (menuViewTerminal)
    {
        popView->Destroy(menuViewTerminal);
        menuViewTerminal = NULL;
    }
    if (menuViewQuickToolbar)
    {
        popView->Destroy(menuViewQuickToolbar);
        menuViewQuickToolbar = NULL;
    }
    if (menuViewSep1)
    {
        popView->Destroy(menuViewSep1);
        menuViewSep1 = NULL;
    }
    if (menuViewGeneratedCode)
    {
        popView->Destroy(menuViewGeneratedCode);
        menuViewGeneratedCode = NULL;
    }
    if (menuViewComponentBlocks)
    {
        popView->Destroy(menuViewComponentBlocks);
        menuViewComponentBlocks = NULL;
    }
#if UNDER_DEVELOPMENT
    if (menuViewPrevView)
    {
        popView->Destroy(menuViewPrevView);
        menuViewPrevView = NULL;
    }
    if (menuViewNextView)
    {
        popView->Destroy(menuViewNextView);
        menuViewNextView = NULL;
    }
#endif

    createMenuView();
}


void MainFrame::updateQuickToolbarGUI()
{
    //##See if this can be automated in the future (iterating over the tools, for example):
    if (toolQuick == NULL)
        return;
    toolQuick->SetToolShortHelp(ID_QuickFileOpen, _("Open"));
    toolQuick->SetToolShortHelp(ID_QuickFileAdd, _("Add"));
    toolQuick->SetToolShortHelp(ID_QuickFileSaveAll, _("Save all"));

#if UNDER_DEVELOPMENT
    toolQuick->SetToolShortHelp(ID_QuickFileCreate, _("Create block"));
#endif

    toolQuick->SetToolShortHelp(ID_QuickComponentRun, _("Run"));

#if UNDER_DEVELOPMENT
    toolQuick->SetToolShortHelp(ID_QuickComponentDeploy, _("Deploy"));
#endif

    toolQuick->SetToolShortHelp(ID_MenuViewComponentBlocks, _("Component's blocks"));
    toolQuick->SetToolShortHelp(ID_MenuViewGeneratedCode, _("Generated code"));
    toolQuick->SetToolShortHelp(ID_MenuViewHardware, _("Hardware"));
    toolQuick->SetToolShortHelp(ID_QuickViewTerminal, _("Terminal"));
    toolQuick->SetToolShortHelp(ID_QuickViewHelpAndResourceCenter, _("Help and Resource Center"));
    toolQuick->SetToolShortHelp(ID_QuickZoom, _("Zoom"));
}


void MainFrame::updateMenuZoomGUI()
{
    if (popZoom == NULL)
        return;

    if (menuZoom10)
    {
        popZoom->Destroy(menuZoom10);
        menuZoom10 = NULL;
    }
    if (menuZoom25)
    {
        popZoom->Destroy(menuZoom25);
        menuZoom25 = NULL;
    }
    if (menuZoom50)
    {
        popZoom->Destroy(menuZoom50);
        menuZoom50 = NULL;
    }
    if (menuZoom75)
    {
        popZoom->Destroy(menuZoom75);
        menuZoom75 = NULL;
    }
    if (menuZoom100)
    {
        popZoom->Destroy(menuZoom100);
        menuZoom100 = NULL;
    }
    if (menuZoom125)
    {
        popZoom->Destroy(menuZoom125);
        menuZoom125 = NULL;
    }
    if (menuZoom150)
    {
        popZoom->Destroy(menuZoom150);
        menuZoom150 = NULL;
    }
    if (menuZoom175)
    {
        popZoom->Destroy(menuZoom175);
        menuZoom175 = NULL;
    }
    if (menuZoom200)
    {
        popZoom->Destroy(menuZoom200);
        menuZoom200 = NULL;
    }
#if UNDER_DEVELOPMENT
    if (menuZoomViewAll)
    {
        popZoom->Destroy(menuZoomViewAll);
        menuZoomViewAll = NULL;
    }
#endif

    createMenuZoom();
}


void MainFrame::createNotebook()
{
    m_notebook_style =  //wxAUI_NB_DEFAULT_STYLE |
                        wxAUI_NB_TOP |
                        //wxAUI_NB_TAB_SPLIT |
                        wxAUI_NB_TAB_MOVE |
                        wxAUI_NB_SCROLL_BUTTONS |
                        wxAUI_NB_CLOSE_ON_ACTIVE_TAB |
                        wxAUI_NB_MIDDLE_CLICK_CLOSE |
                        //wxAUI_NB_TAB_EXTERNAL_MOVE |
                        wxNO_BORDER |
                        wxAUI_NB_WINDOWLIST_BUTTON;

    //Create the notebook off-window to avoid flicker:
    wxSize client_size = GetClientSize();
    notebook = new wxAuiNotebook(this,
                                ID_Notebook,
                                //##Levantar estas cosas de archivo de configuración, y enviarlas a la función CreateNotebook:
                                wxPoint(client_size.x, client_size.y),
                                wxSize(430,200),
                                m_notebook_style);

    bubble.setCanvasesParent(notebook);
    //bubble.setNotifier(this);
    createFileBlock(true, wxString(""));
    if (notebook)
    {
        auiManager.AddPane( notebook,
                            wxAuiPaneInfo().Name(wxString("Blocks"))
                            .CenterPane()
                            .PaneBorder(false)
                            //.MinSize(wxSize(340, 200))
                          );
    }
}


void MainFrame::createHardwareManager()
{
    //##Error sistemático: Descomentar la creación del panel de "Hardware" cuelga a la aplicación si en el
    //constructor del BubbleHardwareManager se llama a SetSize():

//    wxMessageDialog dialog0(this, getBoardName(), _("board")); //##Debug
//    dialog0.ShowModal(); //##Debug
    hardware = new BubbleHardwareManager(this, ID_Hardware, &bubble, getBoardName(), wxColour(255, 255, 255));
    if (hardware)
    {
        auiManager.AddPane( hardware, wxAuiPaneInfo()
                            .Name(wxString("Hardware")).Caption(_("Hardware"))
                            //.Float().Layer(1).Position(1)
                            //.Float()
                            .Left().Layer(1).Position(0)
                            .CloseButton(true).Hide()
                            .BestSize(wxSize(300, 350))
                            .MinSize(wxSize(340, 275))
                          );
    }
}


//void MainFrame::createLocalVariablesManager()
//{
//    localVariables = new BubbleVariablesManager(this, ID_LocalVariables, wxColour(255, 255, 255));
//    if (localVariables)
//    {
//        auiManager.AddPane( localVariables, wxAuiPaneInfo().
//                            Name(wxString("LocalVariables")).Caption(_("Local variables"))
//                            .Left().Layer(0).Row(0).Position(1)
//                            .CloseButton(true).Hide()
//                            .BestSize(wxSize(400, 450))
//                            .MinSize(wxSize(150, 150))
//                            );
//    }
//}


//##Future: Put this in a class, wich will manage all the HelpAndResourceCenter functions:
void MainFrame::OnHtmlLinkClicked(wxHtmlLinkEvent &event)
{
    //wxMessageDialog dialog0(this, event.GetLinkInfo().GetHref(), _("URL")); //##Debug
    //dialog0.ShowModal(); //##Debug

    wxString link = event.GetLinkInfo().GetHref();

    if (link == wxString("licenses::")) //##Un-hardcode?
    {
        //##Si if this code is portable, and if this will show some kind or error message in case the command fails:
        wxString licensesPath = bubble.getDocPath() + wxString("/_Common");
        if (!wxLaunchDefaultApplication(licensesPath))
        {
            showMessage(wxString("Can't find ") + licensesPath, true, true, *wxRED);
        }
    }
    else if (link == wxString("about::")) //##Un-hardcode?
    {
        if (about)
            about->SetFocus();
    }
    else
    {
        //##Near future: Add support for internal links (to load other help pages stored locally):
        wxLaunchDefaultBrowser(link);
    }
    //Never put a skip here!!!:
    //event.Skip();
}


void MainFrame::loadHelpAndResourceCenterFiles()
{
    if (help)
        help->LoadFile(bubble.getDocPath() + wxString("/") + bubble.getLanguageCode() + wxString("/Help.Main.htm"));
    if (about)
        about->LoadFile(bubble.getDocPath() + wxString("/") + bubble.getLanguageCode() + wxString("/About.htm"));
    if (license)
        license->LoadFile(bubble.getDocPath() + wxString("/") + bubble.getLanguageCode() + wxString("/License.htm"));
}


void MainFrame::createHelpAndResourceCenter()
{
    //##This is a small patch to avoid the problem generated by the bug in the wxHtmlWindow class. The bug
    //consists in not loading pages when there are non-english chars in the absolute page's path. So by now,
    //if there are non-english chars, the html files are loaded with an external browser:
    wxString testName(bubble.getDocPath() + wxString("/") + bubble.getLanguageCode() + wxString("/Help.Main.htm"));
    wxString charList = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 _-.#$%&!°()='¨{}[]^`;,+´~¿¡¬:/\\";
    if (Bubble::nonListedCharsInString(charList, testName))
    {
        //wxMessageDialog dialog0(this, wxString(bubble.getDocPath() + "/ESP/About.htm"), _("Non english chars!")); //##Debug
        //dialog0.ShowModal(); //##Debug

        useExternalHelpCenter = true;
        return;
    }

    //##2011.10.01: Test to see if the bug loading the HTMLs is due to wxURI:
//    wxURI uriAbout(bubble.getDocPath() + "/ESP/About.htm");
//    if (uriAbout.IsReference())
//    if (!wxFileName::FileExists(bubble.getDocPath() + "/ESP/About.htm"))
//    {
//        wxMessageDialog dialog0(this, wxString(bubble.getDocPath() + "/ESP/About.htm"), _("Invalid Reference")); //##Debug
//        dialog0.ShowModal(); //##Debug
//        return;
//    }

//    wxMessageDialog dialog0(this, wxGetCwd(), _("Working dir")); //##Debug
//    dialog0.ShowModal(); //##Debug

    int dx = 550; //##Load this from XML config file.
    int dy = 300; //##Load this from XML config file.
    wxSize client_size = GetClientSize();
    helpAndResourceCenter = new wxAuiNotebook(  this,
                                                ID_HelpAndResourceCenter,
                                                //##Levantar estas cosas de archivo de configuración, y enviarlas a la función CreateNotebook:
                                                wxPoint(client_size.x, client_size.y),
                                                wxSize(dx, dy),
                                                //##Ver por qué se pueden dragear del notebook al properties, y no al revés:
            //##                                    m_notebook_style
                                                wxAUI_NB_WINDOWLIST_BUTTON |
                                                wxAUI_NB_TOP |
                                                wxAUI_NB_TAB_SPLIT |
                                                wxAUI_NB_TAB_MOVE |
                                                wxAUI_NB_SCROLL_BUTTONS |
                                                wxAUI_NB_TAB_EXTERNAL_MOVE |
                                                wxNO_BORDER
                                             );

    if (helpAndResourceCenter)
    {
        int iconW = 16; //##
        int iconH = 16; //##

        auiManager.AddPane( helpAndResourceCenter,
                            wxAuiPaneInfo().Name(wxString("HelpAndResourceCenter"))
                            .Caption(_("Help and Resource Center"))
                            //##.CenterPane()
                            //##.PaneBorder(false)
                            .Right().Layer(1).Position(1)
                            .CloseButton(true).Hide()
                            .BestSize(wxSize(dx, dy)) //##
                            .MinSize(wxSize(dx, dy))
                           );
        help = new wxHtmlWindow(this,
                                ID_Help,
                                wxDefaultPosition,
                                wxSize(dx, dy) //##Meter estos números en constantes...
                               );
        if (help)
        {
            wxBitmap bmp = wxArtProvider::GetBitmap(wxART_HELP_BOOK, wxART_OTHER, wxSize(iconW, iconH)); //##
//            wxImage img;
//            img.LoadFile(imagesPath + wxString("/Minibloq.ico")); //##
//            wxBitmap bmp = wxBitmap(img.Scale(iconW, iconH));

            //##Test (bug: does not load the htm file if there are non-ansi chars in the path string):
            //##May work in the future (but "." points to the XDFRun directory -due to the setting of the
            //working dir made by MinibloqRun.exe-):
            //help->LoadFile(wxString("./Doc/ENG/Help.Main.htm"));
            helpAndResourceCenter->AddPage( help,
                                            _("Help"),
                                            false,
                                            bmp
                                          );

            //##Well, this is clearly a wxWidgets but on the wxHtmlWindow class:
//            wxFileName helpMainFile(bubble.getDocPath() + "/ESP/About.htm");
//            if (helpMainFile.FileExists())
//            {
//                help->LoadFile(helpMainFile);
//            }
        }
        about = new wxHtmlWindow(   this,
                                    ID_About,
                                    wxDefaultPosition,
                                    wxSize(dx, dy)
                                );
        if (about)
        {
            wxImage img;
            img.LoadFile(bubble.getThemePath() + wxString("/Minibloq.ico")); //##
            wxBitmap bmp = wxBitmap(img.Scale(iconW, iconH));
            helpAndResourceCenter->AddPage( about,
                                            _("About Minibloq"),
                                            false,
                                            bmp
                                          );
        }

        license = new wxHtmlWindow( this,
                                    ID_License,
                                    wxDefaultPosition,
                                    wxSize(dx, dy)
                                  );
        if (license)
        {
            wxImage img;
            img.LoadFile(bubble.getThemePath() + wxString("/Minibloq.ico")); //##
            wxBitmap bmp = wxBitmap(img.Scale(iconW, iconH));
            helpAndResourceCenter->AddPage( license,
                                            _("License"),
                                            false,
                                            bmp
                                          );
        }

        loadHelpAndResourceCenterFiles();
    }
}


void MainFrame::createProperties()
{
    //Create the notebook off-window to avoid flicker:
    wxSize client_size = GetClientSize();
    properties = new wxAuiNotebook( this,
                                    ID_Properties,
                                    //##Levantar estas cosas de archivo de configuración, y enviarlas a la función CreateNotebook:
                                    wxPoint(client_size.x, client_size.y),
                                    wxSize(400, 200), //##

                                    //##Ver por qué se pueden dragear del notebook al properties, y no al revés:
//##                                    m_notebook_style
                                    wxAUI_NB_WINDOWLIST_BUTTON |
                                    wxAUI_NB_TOP |
                                    wxAUI_NB_TAB_SPLIT |
                                    wxAUI_NB_TAB_MOVE |
                                    wxAUI_NB_SCROLL_BUTTONS |
                                    wxAUI_NB_TAB_EXTERNAL_MOVE |
                                    wxNO_BORDER
                                    );

    if (properties)
    {
        //##Levantar de disco bien, etc.:
        int iconW = 16; //##
        int iconH = 16; //##
        wxBitmap bmp;
        wxImage img;

        auiManager.AddPane( properties,
                            wxAuiPaneInfo().Name(wxString("Properties"))
                            .Caption(_("Properties"))
                            //##.CenterPane()
                            //##.PaneBorder(false)
                            //.Left().Layer(1).Position(1) //##
                            .Left().Layer(0).Row(0).Position(2) //##
                            .CloseButton(true).Hide()
                            .BestSize(wxSize(200, 200)) //##
                            .MinSize(wxSize(200, 200)) //##
                           );

        minibloqProperties = new MinibloqProperties(this,
                                                    ID_MinibloqProperties,
                                                    &bubble,
                                                    languagePath,
                                                    wxColour(255, 255, 255));
        if (minibloqProperties)
        {
            img.LoadFile(bubble.getThemePath() + wxString("/Minibloq.ico")); //##
            bmp = wxBitmap(img.Scale(iconW, iconH));
            properties->AddPage(minibloqProperties,
                                _("Minibloq"),
                                false,
                                bmp);
        }

#if UNDER_DEVELOPMENT
        componentProperties = new ComponentProperties(  this,
                                                        ID_ComponentProperties,
                                                        wxColour(255, 255, 255));
        if (componentProperties)
        {
            img.LoadFile(bubble.getThemePath() + wxString("/ComponentEditorProperties.png"));//##
            bmp = wxBitmap(img.Scale(iconW, iconH));
            properties->AddPage(componentProperties,
                                _("Component"),
                                false,
                                bmp);
        }

        blockEditorProperties = new BlockEditorProperties(  this,
                                                            ID_BlockEditorProperties,
                                                            wxColour(255, 255, 255));
        if (blockEditorProperties)
        {
            img.LoadFile(bubble.getThemePath() + wxString("/BlockEditorProperties.png"));//##
            bmp = wxBitmap(img.Scale(iconW, iconH));
            properties->AddPage(blockEditorProperties,
                                _("Block editor"),
                                false,
                                bmp);
        }

        textEditorProperties = new TextEditorProperties(this,
                                                        ID_TextEditorProperties,
                                                        wxColour(255, 255, 255));
        if (textEditorProperties)
        {
            img.LoadFile(bubble.getThemePath() + wxString("/TextEditorProperties.png"));//##
            bmp = wxBitmap(img.Scale(iconW, iconH));
            properties->AddPage(textEditorProperties,
                                _("Text editor"),
                                false,
                                bmp);
        }

        terminalProperties = new TerminalProperties(this,
                                                    ID_TerminalProperties,
                                                    wxColour(255, 255, 255)//, //##
                                                    //wxDefaultPosition,
                                                    //wxSize(500, 500)
                                                    );
        if (terminalProperties)
        {
            img.LoadFile(bubble.getThemePath() + wxString("/Terminal.png"));
            bmp = wxBitmap(img.Scale(iconW, iconH));
            properties->AddPage(terminalProperties,
                                _("Terminal"),
                                false,
                                bmp);
            properties->Split(properties->GetPageIndex(terminalProperties), wxBOTTOM);

//            //##No funciona:
//            //##Optimizar:
//            if (properties->GetPage(properties->GetPageIndex(terminalProperties)))
//            {
//                (properties->GetPage(properties->GetPageIndex(terminalProperties)))->SetSize(200, 200);
//            }

        }

#endif
        //##:Esto sale del XML de configuración:
        properties->SetSelection(properties->GetPageIndex(terminalProperties));
        properties->SetFocus();
    }
}


void MainFrame::createTerminal()
{
    //Create the notebook off-window to avoid flicker:
    wxSize client_size = GetClientSize();
    terminal = new wxAuiNotebook(this,
                                 ID_Terminal,
                                //##Levantar estas cosas de archivo de configuración, y enviarlas a la función CreateNotebook:
                                 wxPoint(client_size.x, client_size.y),
                                 wxSize(430,200),
                                 wxAUI_NB_TOP |
                                 //wxAUI_NB_TAB_SPLIT | //This is not desirable.
                                 wxAUI_NB_TAB_MOVE |
                                 wxAUI_NB_SCROLL_BUTTONS |
                                 wxAUI_NB_TAB_EXTERNAL_MOVE |
                                 wxNO_BORDER
                                 );

    if (terminal)
    {
        auiManager.AddPane( terminal,
                            wxAuiPaneInfo().Name(wxString("Terminal"))
                            .Caption(_("Terminal"))
                            //##.CenterPane()
                            //##.PaneBorder(false)
                            .Left().Layer(0).Row(0).Position(2)
                            .CloseButton(true).Hide()
                            .BestSize(wxSize(400, 450))
                            .MinSize(wxSize(250, 275))  //250 is the min value necessary to avoid deformation
                                                        //of the "Clear all" button in the Split terminal.
                           );

        wxColour tempColor(128, 128, 128); //##This must be configurable...
        commManager = new TerminalCommManager();
        singleTerminalGUI = new SingleTerminalGUI(  this,
                                                    wxNewId(),
                                                    &bubble,
                                                    commManager,
                                                    tempColor
                                                 );
        splitTerminalGUI = new SplitTerminalGUI(this,
                                                wxNewId(),
                                                &bubble,
                                                commManager,
                                                tempColor
                                               );
#if defined(linux)
        if(singleTerminalGUI)
            commManager->m_pHandler = singleTerminalGUI;
#endif
        if (singleTerminalGUI && splitTerminalGUI)
        {
            //##Levantar de disco bien, etc.:
            int iconW = 16; //##
            int iconH = 16; //##

            //Single terminal:
            wxBitmap bmp;
            wxImage img;
            img.LoadFile(bubble.getThemePath() + wxString("/Terminal.png"));
            bmp = wxBitmap(img.Scale(iconW, iconH));
            terminal->AddPage(  singleTerminalGUI,
                                _("Single"),
                                false,
                                bmp);

            //Split terminal:
            EmoticonScreen *emoticonScreen = splitTerminalGUI->getRxEmoticon();
            if (emoticonScreen)
            {
                emoticonScreen->setImageEmoticonNothing(wxImage(bubble.getThemePath() + wxString("/Terminal.Nothing.png")));
                emoticonScreen->setImageDefault(emoticonScreen->getImageEmoticonNothing());
                emoticonScreen->setImageEmoticonSmile(wxImage(bubble.getThemePath() + wxString("/Terminal.Smile.png")));
                emoticonScreen->setImageEmoticonAngry(wxImage(bubble.getThemePath() + wxString("/Terminal.Angry.png")));
                emoticonScreen->setImageEmoticonWhatever(wxImage(bubble.getThemePath() + wxString("/Terminal.Whatever.png")));
                emoticonScreen->setImageEmoticonArrowLeft(wxImage(bubble.getThemePath() + wxString("/Terminal.ArrowLeft.png")));
                emoticonScreen->setImageEmoticonArrowRight(wxImage(bubble.getThemePath() + wxString("/Terminal.ArrowRight.png")));
                emoticonScreen->setImageEmoticonStop(wxImage(bubble.getThemePath() + wxString("/Terminal.Stop.png")));
            }
            img.LoadFile(bubble.getThemePath() + wxString("/SplitTerminal.png"));
            iconW = 32; //##
            bmp = wxBitmap(img.Scale(iconW, iconH));
            terminal->AddPage(  splitTerminalGUI,
                                _("Split"),
                                false,
                                bmp);

            terminal->SetSelection(terminal->GetPageIndex(singleTerminalGUI));
            singleTerminalGUI->SetFocus();
        }
    }
}


void MainFrame::onQuickZoom10(wxCommandEvent& evt)
{
    //##Future: Find the object with the focus, and call it's method for this action:
    if (bubble.getCurrentCanvas())
    {
        bubble.getCurrentCanvas()->setZoomIndex(0); //##Cambiará por constantes
    }
}


void MainFrame::onQuickZoom25(wxCommandEvent& evt)
{
    //##Future: Find the object with the focus, and call it's method for this action:
    if (bubble.getCurrentCanvas())
    {
        bubble.getCurrentCanvas()->setZoomIndex(1); //##Cambiará por constantes
    }
}


void MainFrame::onQuickZoom50(wxCommandEvent& evt)
{
    //##Future: Find the object with the focus, and call it's method for this action:
    if (bubble.getCurrentCanvas())
    {
        bubble.getCurrentCanvas()->setZoomIndex(2); //##Cambiará por constantes
    }
}


void MainFrame::onQuickZoom75(wxCommandEvent& evt)
{
    //##Future: Find the object with the focus, and call it's method for this action:
    if (bubble.getCurrentCanvas())
    {
        bubble.getCurrentCanvas()->setZoomIndex(3); //##Cambiará por constantes
    }
}


void MainFrame::onQuickZoom100(wxCommandEvent& evt)
{
    //##Future: Find the object with the focus, and call it's method for this action:
    if (bubble.getCurrentCanvas())
    {
        bubble.getCurrentCanvas()->setZoomIndex(4); //##Cambiará por constantes
    }
}


void MainFrame::onQuickZoom125(wxCommandEvent& evt)
{
    //##Future: Find the object with the focus, and call it's method for this action:
    if (bubble.getCurrentCanvas())
    {
        bubble.getCurrentCanvas()->setZoomIndex(5); //##Cambiará por constantes
    }
}


void MainFrame::onQuickZoom150(wxCommandEvent& evt)
{
    //##Future: Find the object with the focus, and call it's method for this action:
    if (bubble.getCurrentCanvas())
    {
        bubble.getCurrentCanvas()->setZoomIndex(6); //##Cambiará por constantes
    }
}


void MainFrame::onQuickZoom175(wxCommandEvent& evt)
{
    //##Future: Find the object with the focus, and call it's method for this action:
    if (bubble.getCurrentCanvas())
    {
        bubble.getCurrentCanvas()->setZoomIndex(7); //##Cambiará por constantes
    }
}


void MainFrame::onQuickZoom200(wxCommandEvent& evt)
{
    //##Future: Find the object with the focus, and call it's method for this action:
    if (bubble.getCurrentCanvas())
    {
        bubble.getCurrentCanvas()->setZoomIndex(8); //##Cambiará por constantes
    }
}


void MainFrame::onQuickZoomViewAll(wxCommandEvent& evt)
{
    //##Future: Find the object with the focus, and call it's method for this action:
    toolZoom->SetLabel(_("View All"));
    if (bubble.getCurrentCanvas())
    {
        bubble.getCurrentCanvas()->setZoomIndex(ZOOM_MAX_INDEX); //##Cambiará por constantes
    }
}


void MainFrame::onHardwareManagerPopUpPort(wxCommandEvent& evt)
{
    if (hardware)
    {
        hardware->popUpPortList();
    }
}


void MainFrame::onHardwareManagerPopUpBoard(wxCommandEvent& evt)
{
    if (hardware)
    {
        hardware->popUpBoardList();
    }
}


wxAuiDockArt* MainFrame::getDockArt()
{
    return auiManager.GetArtProvider();
}


void MainFrame::doUpdate()
{
    auiManager.Update();
}



void MainFrame::onActivate(wxActivateEvent& event)
{
//##Test: made to if the bug of the edit losing focus could be eliminated, but did not work:
//    SetFocus();
//    event.Skip();
}


void MainFrame::onSize(wxSizeEvent& event)
{
    event.Skip();
}


void MainFrame::onClose(wxCloseEvent& event)
{
    if (notebook)
    {
        if (bubble.getCurrentCanvas())
        {
            if (!(bubble.isSaved()))
            {
                wxString tempName = notebook->GetPageText(notebook->GetPageIndex(bubble.getCurrentCanvas()));
                wxMessageDialog question(   this,
                                            _("The component ") + tempName + _(" was not saved. Do You want to save it?"),
                                            _("Question"),
                                            wxICON_QUESTION | wxYES_NO | wxCANCEL | wxYES_DEFAULT
                                        );
                int answer = question.ShowModal();

                if (answer == wxID_CANCEL)
                {
                    event.Veto();
                    return;
                }
                else if (answer == wxID_YES)
                {
                    if (componentAlreadySaved)
                        bubble.saveComponentToFile(bubble.getComponentPath() + wxString("/") + tempComponentName);
                    else
                        saveComponentAs();
                }
            }
        }

//        wxMessageDialog dialog0(this, wxString("0") , wxString("onClose"));
//        dialog0.ShowModal();

//        FileEditorHash *fileEditorHash = bubble.getFileEditorHash();
//        if (fileEditorHash)
//        {
//            FileEditorHash::iterator it;
//            for (it = fileEditorHash->begin(); it != fileEditorHash->end(); it++)
//            {
//                BubbleEditor *value = it->second;
//                if (value)
//                {
//                    askToSaveEditorContent(value, false);
//                }
//            }
//        }

        closeAllEditorFiles();
    }

    writeConfig();

    //Closes the application (this is necessary if there is defined an OnClose event, like here):
	Destroy();
}


void MainFrame::onEraseBackground(wxEraseEvent& event)
{
    event.Skip();
}


void MainFrame::onAllowNotebookDnD(wxAuiNotebookEvent& evt)
{
    // for the purpose of this test application, explicitly
    // allow all notebook drag and drop events
    evt.Allow();
}


void MainFrame::popUpMenu(wxCommandEvent& evt, wxMenu *popMenu)
{
    wxAuiToolBar* tb = static_cast<wxAuiToolBar*>(evt.GetEventObject());
    if (tb)
    {
        tb->SetToolSticky(evt.GetId(), true);

        //Line up our menu with the button:
        wxRect rect = tb->GetToolRect(evt.GetId());
        wxPoint pt = tb->ClientToScreen(rect.GetBottomLeft());
        pt = ScreenToClient(pt);
        PopupMenu(popMenu, pt);

        //Make sure the button is "un-stuck":
        tb->SetToolSticky(evt.GetId(), false);
    }
}


void MainFrame::onMenuFile(wxCommandEvent& evt)
{
    if (toolMainMenu)
    {
        evt.SetEventObject(toolMainMenu);
        evt.SetId(ID_MenuFile);
    }
    popUpMenu(evt, popFile);
}


void MainFrame::createFileBlock(bool mainCanvas, const wxString &newTabName)
{
    //##Debug:
    //wxMessageDialog dialog0(this, wxString("tempComponentNameCounter = ") << tempComponentNameCounter, _("createFileBlock"));
    //dialog0.ShowModal();

    //##Optimizar:
        //Llamar a bubble.getCurrentCanvas() una sóla vez, guardándolo en una variable...
    //bubble.addCanvas(ctrl); //##Funciona.
    //##Descablear los valores de scrollVInc y scrollHInc y sacarlos del archivo XML de configuración:
    bubble.addCanvas(wxNewId(), mainCanvas, 60, 60); //##Ver tema ID
    if (bubble.getCurrentCanvas())
    {
        int iconW = 16; //##
        int iconH = 16; //##
        //##Ver si va este bitmap:
        wxBitmap page_bmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(iconW, iconH));
        //##Obtener el título de la internacionalización (así lo comprenderán mejor los chicos) y el
        //número del último que se encuentre grabado en el directorio temporal, incrementado en 1:

//##Esto funciona bien y es independiente del idioma, pero es un poco complicado el nombre resultante, más
//como para la gente que usa Arduino, que para chicos:
//        wxDateTime now = wxDateTime::Now();
//        tempComponentName = wxString("C") +
//                            now.Format("%m%d") +
//                            wxString("_") +
//                            //now.Format("%H%M%S") +
//                            now.Format("%H%M") +
//                            wxString(".mbqc");
        if (newTabName == wxString(""))
        {
            tempComponentName = wxString("comp") << tempComponentNameCounter;
            tempComponentNameCounter++;
            tempComponentName += wxString(".mbqc");
        }
        else
        {
            tempComponentName = newTabName;
        }

        if (notebook)
        {
            notebook->AddPage(  bubble.getCurrentCanvas(),
                                //_("New-1.mbqc"),
                                tempComponentName,
                                false,
                                page_bmp);

            notebook->SetSelection(notebook->GetPageIndex(bubble.getCurrentCanvas()));
        }
        bubble.forceSaved(true);
        bubble.getCurrentCanvas()->SetFocusIgnoringChildren();

        //##Test:
        //bubble.getCurrentCanvas()->setScrollBarsVisible(true);
    }
}


void MainFrame::createComponent(bool canCancel)
{
    //##Debug:
    //wxMessageDialog dialog0(this, wxString("tempComponentNameCounter = ") << tempComponentNameCounter, _("createComponent"));
    //dialog0.ShowModal();

    //##Mejorar todo esto en el futuro:

    //##¿Qué pasa si no hay notebook en los 2 if que preguntan esto acá:?

    //##Implementar: Por ahora no se soportan múltiples componentes abiertos a la vez. Más adelante se hará:
    bool showCode = false;

    if (bubble.getCurrentCanvas())
    {
        if (!(bubble.isSaved()))
        {
            if (notebook)
            {
                wxString tempName = notebook->GetPageText(notebook->GetPageIndex(bubble.getCurrentCanvas()));
                long style = wxICON_QUESTION | wxYES_NO | wxYES_DEFAULT;
                if (canCancel)
                    style |= wxCANCEL;
                wxMessageDialog question(   this,
                                            _("The component ") + tempName + _(" was not saved. Do You want to save it?"),
                                            _("Question"),
                                            style
                                        );
                int answer = question.ShowModal();
                if (answer == wxID_CANCEL)
                    return; //Does nothing.
                else if (answer == wxID_YES)
                    saveComponent();

                if (notebook->GetPageIndex(editCode) != wxNOT_FOUND )
                {
                    showCode = true;
                    toggleGeneratedCode();
                }

                //##Pasar esto a una función, que se llame tipo closePage, o algo así:
                BubbleCanvas *tempCurrentCanvas = bubble.getCurrentCanvas();
                if (tempCurrentCanvas)
                {
                    //This does not destroy the object, so the auiNotebook must do the job:
                    bubble.deleteCanvas(tempCurrentCanvas);
                    notebook->DeletePage(notebook->GetPageIndex(tempCurrentCanvas));
                    tempCurrentCanvas = NULL;
                }
                createFileBlock(true, wxString(""));
                componentAlreadySaved = false;
            }
        }
        else
        {
            if (notebook)
            {
                if (notebook->GetPageIndex(editCode) != wxNOT_FOUND )
                {
                    showCode = true;
                    toggleGeneratedCode();
                }

                //##Pasar esto a una función, que se llame tipo closePage, o algo así:
                BubbleCanvas *tempCurrentCanvas = bubble.getCurrentCanvas();
                if (tempCurrentCanvas)
                {
                    //This does not destroy the object, so the auiNotebook must do the job:
                    bubble.deleteCanvas(tempCurrentCanvas);
                    notebook->DeletePage(notebook->GetPageIndex(tempCurrentCanvas));
                    tempCurrentCanvas = NULL;
                }
                createFileBlock(true, wxString(""));
                componentAlreadySaved = false;
            }
        }
        if (bubble.getCurrentCanvas())
            bubble.getCurrentCanvas()->setZoomIndex(3);
    }
    else
    {
        if (notebook->GetPageIndex(editCode) != wxNOT_FOUND )
        {
            showCode = true;
            toggleGeneratedCode();
        }

        createFileBlock(true, wxString(""));
        componentAlreadySaved = false;
    }

    if (showCode)
        toggleGeneratedCode();

    closeAllEditorFiles();

    //##Horrible, super temporal:
    wxFileName aux(tempComponentName);
    bubble.setComponentPath(bubble.getTempPath());
    bubble.setComponentFilesPath(bubble.getComponentPath() + wxString("/") + aux.GetName());
    bubble.setOutputPath(bubble.getComponentFilesPath() + wxString("/output"));
    //bubble.loadBoardRelations();
}


void MainFrame::onMenuFileCreateComponent(wxCommandEvent& evt)
{
    createComponent(true);
}


void MainFrame::onMenuFileCreateBlock(wxCommandEvent& evt)
{
    //##:
    createFileBlock(false, wxString(""));
}


void MainFrame::onMenuFileAdd(wxCommandEvent& evt)
{
    if (bubble.getHardwareManager() == NULL)
        return;
    if (bubble.getHardwareManager()->getCurrentBoardProperties() == NULL)
        return;

    //wxString wildcards = _("C++ files (*.cpp;*.h)|*.cpp;*.h|Block files (*.mbqb)|*.mbq");
    //wxString wildcards = _("C++ files (*.cpp;*.h)|*.cpp;*.h");

    //This is the desired format for wildcards: "(*.cpp;*.h)|*.cpp;*.h"
    wxString wildcards("");
    wxString extensionsList("");
    unsigned int i = 0;
    while (i < bubble.getHardwareManager()->getCurrentBoardProperties()->getFileExtensionsCount())
    {
        wxString extension = wxString("*.") + bubble.getHardwareManager()->getCurrentBoardProperties()->getFileExtension(i);
        if (i != 0)
            extension = wxString(";") + extension;
        extensionsList += extension;
        i++;
    }
    wildcards = wxString("(") + extensionsList + wxString(")|") + extensionsList;

    //##Debug:
    //wxMessageDialog dialog2(this, wildcards, _("File extenstions"));
    //dialog2.ShowModal();

    //##Futuro: obtener el último dir que el usuario quiere usar, y por defecto el work, etc..:
    wxFileDialog dialog(this, _("Add a block or code file to the current component"),
                        wxEmptyString, wxEmptyString, wildcards,
                        wxFD_OPEN|
                        wxFD_MULTIPLE|
                        wxFD_FILE_MUST_EXIST);
                        //##Ver si usaré o no el flag wxFD_FILE_MUST_EXIST

    //##Futuro: Implementar la lógica como para que si el usuario ingresa el nombre de un archivo que no
    //existía (aún cuando tenga seleccionados otros archivos además), éste se cree y se grabe (llamada
    //a AddBlock y Save, ya con el nombre del archivo devuelto por el diálogo.

    //If the component is not saved, save it and create it's files dir:
    if (!componentAlreadySaved)
    {
        wxMessageDialog question(   this,
                                    _("The component was not saved, and it has to be saved before adding files. Do you want to save it now?"),
                                    _("Question"),
                                    wxICON_QUESTION | wxYES_NO | wxYES_DEFAULT
                                );
        int answer = question.ShowModal();
        if (answer == wxID_YES)
        {
            if (saveComponentAs())
                bubble.createDirs(bubble.getOutputPath());
            else
                return;
        }
        else
        {
            //Do nothing:
            return;
        }
    }

    if (dialog.ShowModal() == wxID_OK)
    {
        wxArrayString paths, fileNames;

        dialog.GetPaths(paths);
        //dialog.GetFilenames(fileNames);

        //wxString msg; //##Debug.
        wxString filePath;
        wxString strComponentFilesPath = bubble.getComponentFilesPath();
        strComponentFilesPath.Replace("\\", "/");
        size_t count = paths.GetCount();
        for (size_t i = 0; i < count; i++)
        {
            filePath = paths[i];

            //Verify if the file is already in the component's dir, and copy it if not:
            wxString strFilePathWithoutName = filePath.BeforeLast(wxFileName::GetPathSeparator());
            wxString strFileName = filePath.AfterLast(wxFileName::GetPathSeparator());
            strFilePathWithoutName.Replace("\\", "/"); //Works both under Windows and Linux.
            filePath.Replace("\\", "/");
            wxString destFileName = strComponentFilesPath + "/" + strFileName;

            ////If the file already belongs to the component, process the next file:
            //if (bubble.isFileAdded(destFileName))
            //    continue;

            if (wxFile::Exists(filePath)) //Extra security check.
            {
                wxString strComponentName("");
                //Files automatically generated by miniBloq can not be added to a project:
                if (bubble.getCurrentCanvas())
                {
                    if (notebook)
                    {
                        strComponentName =  bubble.getComponentFilesPath().AfterLast('/') + wxString(".") +
                                            bubble.getHardwareManager()->getCurrentBoardProperties()->getCodeFileExtension();
                        //wxMessageDialog dialog2(this, strComponentName, _("Component name:")); //##Debug.
                        //dialog2.ShowModal(); //##Debug.
                    }
                }

                if ( (strFileName ==
                     (wxString("initBoard.") + bubble.getHardwareManager()->getCurrentBoardProperties()->getCodeFileExtension()))
                     ||
                     (strFileName ==
                     (wxString("mbq.") + bubble.getHardwareManager()->getCurrentBoardProperties()->getHeaderFileExtension()))
                     ||
                     (strFileName == strComponentName)
                   )
                {
                    wxMessageDialog info(   this,
                                            _("The file ") + strFileName + _(" is an automatically generated miniBloq's file and can't be added to the component."),
                                            _("Information"),
                                                wxICON_INFORMATION
                                            );
                    info.ShowModal();
                    continue;
                }

                if (strComponentFilesPath != strFilePathWithoutName)
                {
                    //If there is a file with the same name in the component's dir, asks the user to overwrite it or not:
                    if (wxFile::Exists(destFileName))
                    {
                        wxMessageDialog question(   this,
                                                    _("The file ") + strFileName + _(" already exist in the component's dir. Do You want to overwrite it?"),
                                                    _("Question"),
                                                    wxICON_QUESTION | wxYES_NO | wxNO_DEFAULT
                                                );
                        int answer = question.ShowModal();
                        if (answer == wxID_YES)
                        {
                            //Overwrite the file:
                            wxCopyFile(filePath, destFileName, true);

                            //If the file was overwritten and it was opened in an editor, the editor has to be reloaded:
                            if (bubble.isFileAdded(destFileName))
                            {
                                BubbleEditor *editor = bubble.getFileEditor(destFileName);
                                if  (editor)
                                {
                                    editor->LoadFile(destFileName);

                                    //Mark as saved, since it has been reloaded:
                                    wxBitmap page_bmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16, 16));
                                    if (notebook)
                                    {
                                        int index = notebook->GetPageIndex(editor);
                                        if (index != wxNOT_FOUND)
                                            notebook->SetPageBitmap(index, page_bmp);
                                    }
                                }
                                continue;
                            }
                        }
                        else
                        {
                            continue;
                        }
                    }
                    else
                    {
                        //Copy the file, whith overwriting disabled:
                        wxCopyFile(filePath, destFileName, false);
                    }
                }

                //Add the file to the component's file lists, whitout it's full path, since Bubble only will build files inside
                //it's ComponentFilesPath:
                BubbleEditor *newEditor = new BubbleEditor(this, &bubble, wxNewId());
                if (newEditor)
                {
                    if (bubble.addFile(destFileName, newEditor))
                    {
                        //If the file was added, open it, creating an new editor:
                        createCodeEditor(destFileName, newEditor); //Passes the full fileName.

                    }
                    else
                    {
                        //If not, destroy the new created editor:
                        delete newEditor;
                        newEditor = NULL; //Not necessary.
                    }
                }
            }
        }

        //If the canvas is visible, the split it to the left:
        if (bubble.getCurrentCanvas())
        {
            if (notebook)
            {
                int index = notebook->GetPageIndex(bubble.getCurrentCanvas());
                if (index != wxNOT_FOUND)
                {
                    //If there are other text files opened, then do not do the split:
                    //if (bubble.getFileEditorHash())
                    //{
                        //if (bubble.getFileEditorHash()->size() == 0)
                        //{
                            notebook->Split(notebook->GetPageIndex(bubble.getCurrentCanvas()), wxLEFT);
                            //toggleComponentBlocks();
                            //toggleComponentBlocks();

                            //wxMessageDialog dialog0(this, _("split!"), wxString("0") << getEditCodeZoom()); //##Debug.
                            //dialog0.ShowModal(); //##Debug.
                        //}
                    //}
                }
            }
        }

        //##Debug:
        //wxMessageDialog dialog2(this, msg, _("Selected files"));
        //dialog2.ShowModal();
    }
}


void MainFrame::onMenuFileRemove(wxCommandEvent& evt)
{
    //##Revisar esto: debería borrar el current canvas, y cerrar su notebook correspondiente:
    //bubble.deleteCanvas();
}


void MainFrame::onMenuFileOpen(wxCommandEvent& evt)
{
    //##Mejorar todo esto en el futuro:

    //##Este es exactamente el mismo código que OnMenuFileCreate, pero luego debe pasarse todo a una función,
    //en lo posible:

    if (bubble.getCurrentCanvas())
    {
        //There is a currentCanvas, is it saved yet?
        if (!(bubble.isSaved()))
        {
            if (notebook)
            {
                wxString tempName = notebook->GetPageText(notebook->GetPageIndex(bubble.getCurrentCanvas()));
                wxMessageDialog question(   this,
                                            _("The component ") + tempName + _(" was not saved. Do You want to save it?"),
                                            _("Question"),
                                            wxICON_QUESTION | wxYES_NO | wxCANCEL | wxYES_DEFAULT
                                        );
                int answer = question.ShowModal();
                if (answer == wxID_CANCEL)
                    return; //Does nothing.
                else if (answer == wxID_YES)
                    saveComponent();

                openFileComponent();
            }
        }
        else
        {
            if (notebook)
            {
                //##Futuro: Hacer que no se vea tan feo esto, porque se destruye el canvas luego de que se
                //crea el nuevo. Habría que invisibilizar el canvas actual, crear el nuevo y si falla,
                //hacer visible el actual nuevamente:
                openFileComponent();
            }
        }
    }
    else
    {
        //If there is no currentCanvas, just open the file (because there is nothing to save):
        openFileComponent();
    }
}


void MainFrame::onMenuFileExamples(wxCommandEvent& evt)
{
    if (bubble.getCurrentCanvas())
    {
        //There is a currentCanvas, is it saved yet?
        if (!(bubble.isSaved()))
        {
            if (notebook)
            {
                wxString tempName = notebook->GetPageText(notebook->GetPageIndex(bubble.getCurrentCanvas()));
                wxMessageDialog question(   this,
                                            _("The component ") + tempName + _(" was not saved. Do You want to save it?"),
                                            _("Question"),
                                            wxICON_QUESTION | wxYES_NO | wxCANCEL | wxYES_DEFAULT
                                        );
                int answer = question.ShowModal();
                if (answer == wxID_CANCEL)
                    return; //Does nothing.
                else if (answer == wxID_YES)
                    saveComponent();

                openExample();
            }
        }
        else
        {
            openExample();
        }
    }
}


void MainFrame::openExample()
{
    //##Si if this code is portable, and if this will show some kind or error message in case the command fails:
    wxString folderPath = bubble.getComponentsRepositoryPath() + wxString("/work/_Examples");
    //folderPath.Replace("/", "\\");  //##Horrible hardcoded!

    if ( !wxDir::Exists(folderPath) )
    {
        showMessage(_("Can't find folder ") + folderPath, //##Un-hardcode.
                    true,
                    true,
                    *wxRED);
    }

    openFileComponent(folderPath);
}


//##Puede que luego se unifique para grabar blocks y components, pero no por ahora:
bool MainFrame::openFileComponent(const wxString &defaultDir)
{
    //This is to avoid the bug (under Windows) where the second click from a double click event from the
    //open file dialog reloads hardware, or the like:
//    if (bubble.getHardwareManager())
//        bubble.getHardwareManager()->setAllEnabled(false);
//    bool showHardwareManager = false;
//    if (bubble.getHardwareManager())
//    {
//        if (bubble.getHardwareManager()->IsShown())
//        {
//            showHardwareManager = true;
//            toggleWindow("Hardware", menuViewHardware);
//        }
//    }

    //##Ojo que registré al menos 2 cuelgues con esto, no sé qué pasó, pero esto se colgó alguna vez, posiblemente
    //sea el delete, o algo del AUI. Intentar reproducir el bug, que no ha resultado sistemático:

    wxString wildcards = _("Component files (*.mbqc)|*.mbqc|All files (*.*)|*.*");

    //##Falta obtener el último dir que el usuario quiere usar, y por defecto el work, etc..:
    wxFileDialog dialog(this, _("Open"),
                        defaultDir, wxEmptyString, wildcards,
                        wxFD_OPEN);
                        //|wxFD_MULTIPLE); //##Future use.
                        //##Ver si usaré o no el flag wxFD_FILE_MUST_EXIST

    //##Ojo: Aparentemente, Windows recuerda esto del diálogo, pero no sé bien donde ni cómo lo guarda,
    //y obviamente no es portable, así que tengo que grabar todo en el archivo de configuración del
    //entorno (tamaño, posición, dir, etc..).
    //dialog.Centre();

    //##Implementar la lógica como para que si el usuario ingresa el nombre de un archivo que no
    //existía (aún cuando tenga seleccionados otros archivos además), éste se cree y se grabe (llamada
    //a AddBlock y Save, ya con el nombre del archivo devuelto por el diálogo.

    BubbleCanvas *tempCurrentCanvas = bubble.getCurrentCanvas();
    if (dialog.ShowModal() == wxID_OK)
    {
        closeAllEditorFiles();

        bool showCode = false;
        if (notebook)
        {
#if defined (WIN32)
            if (notebook->GetPage(notebook->GetPageIndex(editCode)) != NULL)
#else
            if (notebook->GetPageIndex(editCode) != wxNOT_FOUND )
#endif
            {
                showCode = true;
                toggleGeneratedCode();
            }
        }
        createFileBlock(true, dialog.GetFilename()); //##By now...

        bool visibleCanvas = true;
        wxFileName aux(dialog.GetPath());
        bubble.setComponentPath(aux.GetPath());
        tempComponentName = aux.GetFullName();
        bubble.setComponentFilesPath(bubble.getComponentPath() + wxString("/") + aux.GetName());
        bubble.setOutputPath(bubble.getComponentFilesPath() + wxString("/output"));
        bubble.setVisibleCanvas(true); //Default
        if (bubble.loadComponentFromFile(dialog.GetPath()))
        {
            visibleCanvas = bubble.getVisibleCanvas();
            componentAlreadySaved = true;
//            wxMessageDialog dialog0(this, _("0"), _("Debug:")); //##Debug.
//            dialog0.ShowModal(); //##Debug.
        }
        if (tempCurrentCanvas)
        {
            //This does not destroy the object, so the auiNotebook must do the job:
            bubble.deleteCanvas(tempCurrentCanvas);
            if (notebook)
            {
                notebook->DeletePage(notebook->GetPageIndex(tempCurrentCanvas));
                tempCurrentCanvas = NULL;
            }
            bubble.forceSaved(true);

            //##: Ojo, que estas líneas estaban produciendo errores:
            if (showCode)
                toggleGeneratedCode();

//            wxMilliSleep(1000);
//            if (bubble.getHardwareManager())
//                bubble.getHardwareManager()->setAllEnabled(true);
//            if (bubble.getHardwareManager())
//            {
//                if (showHardwareManager)
//                    toggleWindow("Hardware", menuViewHardware);
//            }


            if (bubble.getCurrentCanvas())
            {
                if (notebook)
                {
                    int index = notebook->GetPageIndex(bubble.getCurrentCanvas());
                    if (index != wxNOT_FOUND)
                    {
                        //notebook->Split(notebook->GetPageIndex(bubble.getCurrentCanvas()), wxLEFT);
                        toggleComponentBlocks();
                        toggleComponentBlocks();
                        if (!visibleCanvas)
                        {
                            toggleComponentBlocks();
                            //wxMessageDialog dialog0(this, wxString("visible=")<<bubble.getVisibleCanvas(), _("Debug:")); //##Debug.
                            //dialog0.ShowModal(); //##Debug.
                        }
                    }
                }
            }

            return true;
        }
    }

//    if (bubble.getHardwareManager())
//    {
//        if (showHardwareManager)
//            toggleWindow("Hardware", menuViewHardware);
//    }
    return false;
}


bool MainFrame::saveComponentAs()
{
    if (notebook)
    {
        wxString tempName = notebook->GetPageText(notebook->GetPageIndex(bubble.getCurrentCanvas()));
        wxFileDialog dialog(this,
                            _("Save file as"),
                            //##Por ahora, éste es el path sugerido, pero ¡ESTO SE VA!:
                            bubble.getComponentPath(), //wxEmptyString, //##
                            //wxString("New-1.mbqc"), //##
                            tempName,
                            _("Block files (*.mbqc)|*.mbqc"), //##Ver qué se hace con el archivo de proyecto
                            wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

        dialog.SetFilterIndex(1);
        if (dialog.ShowModal() == wxID_OK)
        {
            if (bubble.saveComponentToFile(dialog.GetPath()))
            {
                wxFileName aux(dialog.GetPath());
                bubble.setComponentPath(aux.GetPath());
                tempComponentName = aux.GetFullName();
                bubble.setComponentFilesPath(bubble.getComponentPath() + wxString("/") + aux.GetName());
                bubble.setOutputPath(bubble.getComponentFilesPath() + wxString("/output"));

                componentAlreadySaved = true;
                if (bubble.getCurrentCanvas())
                    notebook->SetPageText(  notebook->GetPageIndex(bubble.getCurrentCanvas()), tempComponentName);

                //Updates the mainOutputFile's name:
#if defined (WIN32)
                if (notebook->GetPage(notebook->GetPageIndex(editCode)))
#else
                if (notebook->GetPageIndex(editCode) != wxNOT_FOUND )
#endif
                {
                    toggleGeneratedCode();
                    toggleGeneratedCode();
                }

                if (notebook)
                {
                    if (bubble.getCurrentCanvas())
                    {
                        int index = notebook->GetPageIndex(bubble.getCurrentCanvas());
                        if (index != wxNOT_FOUND)
                        {
                            wxBitmap page_bmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16, 16));
                            notebook->SetPageBitmap(index, page_bmp);
                        }
                    }
                }

                saveAllEditorFiles();
                return true;
            }
        }
    }
    return false;
}


void MainFrame::saveComponent()
{
    if (componentAlreadySaved)
    {
        bubble.saveComponentToFile(bubble.getComponentPath() + wxString("/") + tempComponentName);

//        wxMessageDialog dialog0(this, bubble.getComponentPath() + wxString("/") + tempComponentName, _("0")); //Debug...
//        dialog0.ShowModal();

        if (notebook)
        {
            if (bubble.getCurrentCanvas())
            {
                int index = notebook->GetPageIndex(bubble.getCurrentCanvas());
                if (index != wxNOT_FOUND)
                {
                    wxBitmap page_bmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16, 16));
                    notebook->SetPageBitmap(index, page_bmp);
                }
            }
        }
//        wxMessageDialog dialog1(this, wxString("1"), _("1")); //Debug...
//        dialog1.ShowModal();
        saveAllEditorFiles();
    }
    else
    {
        saveComponentAs();
    }
}


void MainFrame::onMenuFileSave(wxCommandEvent& evt)
{
    saveComponent();
}


//void MainFrame::onMenuFileSaveAs(wxCommandEvent& evt)
//{
//    saveComponentAs();
//}


void MainFrame::onMenuFileSaveAll(wxCommandEvent& evt)
{
    //Saves the component itself:
    onMenuFileSave(evt);

    //Saves the added text files:
    if (notebook == NULL)
        return;

    //saveAllEditorFiles();
}


void MainFrame::closeAllEditorFiles()
{
    FileEditorHash *fileEditorHash = bubble.getFileEditorHash();
    if (fileEditorHash)
    {
        FileEditorHash::iterator it;
        for (it = fileEditorHash->begin(); it != fileEditorHash->end(); it++)
        {
            BubbleEditor *value = it->second;
            if (value)
            {
                int index = notebook->GetPageIndex(value);
                if (index != wxNOT_FOUND)
                {
                    askToSaveEditorContent(value, false);

                    //This does not destroy de objetcs. This is not a big problem, since is more secure
                    //and is similar to the policy with blocks (and there are in general less editor files
                    //than blocks!
                    notebook->RemovePage(index);
                    value->Hide();
                }
            }
        }
        fileEditorHash->clear();
    }
}


void MainFrame::saveAllEditorFiles()
{
    if (notebook == NULL)
        return;

    FileEditorHash *fileEditorHash = bubble.getFileEditorHash();
    if (fileEditorHash)
    {
        FileEditorHash::iterator it;
        for (it = fileEditorHash->begin(); it != fileEditorHash->end(); it++)
        {
//            wxMessageDialog dialog0(this, it->first, _("2")); //Debug...
//            dialog0.ShowModal();

            BubbleEditor *value = it->second;
            if (value)
            {
                value->SaveFile(it->first);

                //Marks the file as saved:
                wxBitmap page_bmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16, 16));
                int index = notebook->GetPageIndex(value);
                if (index != wxNOT_FOUND)
                    notebook->SetPageBitmap(index, page_bmp);
            }
        }
    }


//      //##2014.01.22: Strange bug here: Not always iterates over ALL the editors in the notebook, grrr!
//    for (size_t i=0; i<notebook->GetPageCount(); i++)
//    {
//        //Is the page an editor?
//        if (notebook->GetPage(i))
//        {
//            if (notebook->GetPage(i)->IsKindOf(CLASSINFO(BubbleEditor)))
//            {
//                wxString strComponentFilesPath = bubble.getComponentFilesPath();
//                strComponentFilesPath.Replace("\\", "/");
//                strComponentFilesPath += wxString("/") + notebook->GetPageText(i);
//
//                BubbleEditor *currentEditor = (BubbleEditor *)notebook->GetPage(i);
//                if (currentEditor)
//                {
//                    //Is the page the generatedCodeViewer?
//                    //if (currentEditor != editCode)
//                    {
//                        //wxMessageDialog dialog0(this, strComponentFilesPath, wxString("file")); //##Debug.
//                        //dialog0.ShowModal(); //##Debug.
//
//                        currentEditor->SaveFile(strComponentFilesPath);
//                        wxBitmap page_bmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16, 16));
//                        notebook->SetPageBitmap(i, page_bmp);
//                    }
//                }
//            }
//        }
//    }
}

void MainFrame::onMenuFileClose(wxCommandEvent& evt)
{
    //##Not by now...
}


void MainFrame::onMenuFileCloseAll(wxCommandEvent& evt)
{
    //##Not by now...
}


void MainFrame::onMenuFilePrint(wxCommandEvent& evt)
{
    //##Ver si se hará un subsistema de impresión más potente, donde se puedan seleccionar los canvas
    //a imprimir, etc.:
    if (bubble.getCurrentCanvas())
        bubble.getCurrentCanvas()->print();
}


void MainFrame::onMenuFileExportAsImageToClipboard(wxCommandEvent& evt)
{
    wxCursor cursor = GetCursor();
    try
    {
        if ( bubble.getCurrentCanvas() )
        {
            if ( wxTheClipboard->Open() )
            {
                SetCursor(wxCursor(wxCURSOR_WAIT));

                showMessage(_("Capturing..."),
                            true,
                            true,
                            *wxGREEN);

                wxBitmap bmp( bubble.getCurrentCanvas()->getCanvasCapture() );
                wxTheClipboard->SetData( new wxBitmapDataObject(bmp) );
                wxTheClipboard->Close();
                showMessage(_("\nReady."),
                            false,
                            true,
                            *wxGREEN);

                SetCursor(cursor);
            }
        }
    }
    catch(...)
    {
        SetCursor(cursor);
    }
}

void MainFrame::onMenuFileExit(wxCommandEvent& evt)
{
}


void MainFrame::onMenuEdit(wxCommandEvent& evt)
{
    if (toolMainMenu)
    {
        evt.SetEventObject(toolMainMenu);
        evt.SetId(ID_MenuEdit);
    }

    if (bubble.getCurrentCanvas())
    {
        if (menuEditPaste)
            menuEditPaste->Enable(bubble.getCurrentCanvas()->getEnableMenuPaste());
        if (menuEditCut)
            menuEditCut->Enable(bubble.getCurrentCanvas()->getEnableMenuCut());
        if (menuEditCopy)
            menuEditCopy->Enable(bubble.getCurrentCanvas()->getEnableMenuCopy());
        if (menuEditDelete)
            menuEditDelete->Enable(bubble.getCurrentCanvas()->getEnableMenuDelete());
        if (menuCommentUncomment)
            menuCommentUncomment->Enable(bubble.getCurrentCanvas()->getEnableMenuCommentUncomment());
    }

    popUpMenu(evt, popEdit);
}


void MainFrame::onMenuEditUndo(wxCommandEvent& evt)
{
    //##Future: Find the object with the focus, and call it's method for this action:
    if (bubble.getCurrentCanvas())
        bubble.getCurrentCanvas()->undo();
}


void MainFrame::onMenuEditRedo(wxCommandEvent& evt)
{
    //##Future: Find the object with the focus, and call it's method for this action:
    if (bubble.getCurrentCanvas())
        bubble.getCurrentCanvas()->redo();
}


void MainFrame::onMenuEditSelectAll(wxCommandEvent& evt)
{
    //##Future: Find the object with the focus, and call it's method for this action:
    if (bubble.getCurrentCanvas())
        bubble.getCurrentCanvas()->selectAll();
}


void MainFrame::onMenuEditCopy(wxCommandEvent& evt)
{
    BubbleCanvas *canvas = bubble.getCurrentCanvas();
//    //##The canvas does not manage well the focus in Windows, so I used an internal variable to keep it better:
//    if (canvas)
//    {
//        if (canvas->getFocused())
//        {
//            bubble.getCurrentCanvas()->onCopyBlock(evt);
//            return;
//        }
//    }

    //##Future: Make an array with all the GUI windows, and compare to that array. This will be compatible
    //with the future plug-in subsystem (add, for example, the "Help and Resource Center" too):
    wxWindow *window = wxWindow::FindFocus();
    if (notebook)
    {
        if (notebook->GetPageIndex(editCode) != wxNOT_FOUND)
        {
            if (editCode)
            {
                //Has the focus?
                if (editCode == window)
                {
                    //Is there a selection?
                    wxString sel = editCode->GetSelectedText();
                    if (sel.Len() > 0)
                    {
                        editCode->Copy();
                        return;
                    }
                }
            }
        }
    }

    if (messages)
    {
        wxTextCtrl* txtMsg = messages->getTxtMsg();
        if (txtMsg)
        {
            //Has the focus?
            if (txtMsg == window)
            {
                //Is there a selection?
                long i = 0;
                long j = 0;
                txtMsg->GetSelection(&i, &j);
                if (i != j)
                {
                    txtMsg->Copy();
                    return;
                }
            }
        }
    }

    if (singleTerminalGUI)
    {
        //Has the focus?
        if (singleTerminalGUI->getRxTxEditor())
        {
            //Is there a selection?
            if (singleTerminalGUI->getRxTxEditor() == window)
            {
                long i = 0;
                long j = 0;
                singleTerminalGUI->getRxTxEditor()->GetSelection(&i, &j);
                if (i != j)
                {
                    singleTerminalGUI->getRxTxEditor()->Copy();
                    return;
                }
            }
        }
    }

    if (splitTerminalGUI)
    {
        //Has the focus?
        if (splitTerminalGUI->getRxEditor() && splitTerminalGUI->getTxEditor())
        {
            //Is there a selection?
            if (splitTerminalGUI->getRxEditor() == window)
            {
                long i = 0;
                long j = 0;
                splitTerminalGUI->getRxEditor()->GetSelection(&i, &j);
                if (i != j)
                {
                    splitTerminalGUI->getRxEditor()->Copy();
                    return;
                }
            }
            else if (splitTerminalGUI->getTxEditor() == window)
            {
                long i = 0;
                long j = 0;
                splitTerminalGUI->getTxEditor()->GetSelection(&i, &j);
                if (i != j)
                {
                    splitTerminalGUI->getTxEditor()->Copy();
                    return;
                }
            }
        }
    }

    //If no other window with copyable data is selected, just copy the current block (or current selection)
    //in the block editor (this will be more handy, I think):
    if (canvas)
    {
        bubble.getCurrentCanvas()->onCopyBlock(evt);
        return;
    }
}


void MainFrame::onMenuEditCut(wxCommandEvent& evt)
{
    //##By now, only the following controls allow to cut something from them:
    //- Block editor (canvas).
    //- rxTxEditor from singleTerminal.
    //- txEditor from splitTerminal.
    //##In the future this function will be surely unified with the one that goes through the array of
    //GUI windows that will be implemented for the onMenyEditCopy event:

    BubbleCanvas *canvas = bubble.getCurrentCanvas();
//    //##The canvas does not manage well the focus in Windows, so I used an internal variable to keep it better:
//    if (canvas)
//    {
//        if (canvas->getFocused())
//        {
//            bubble.getCurrentCanvas()->onCutBlock(evt);
//            return;
//        }
//    }
//
    wxWindow *window = wxWindow::FindFocus();
    if (singleTerminalGUI)
    {
        //Has the focus?
        if (singleTerminalGUI->getRxTxEditor())
        {
            //Is there a selection?
            if (singleTerminalGUI->getRxTxEditor() == window)
            {
                long i = 0;
                long j = 0;
                singleTerminalGUI->getRxTxEditor()->GetSelection(&i, &j);
                if (i != j)
                {
                    singleTerminalGUI->getRxTxEditor()->Cut();
                    return;
                }
            }
        }
    }

    if (splitTerminalGUI)
    {
        //Has the focus?
        if (splitTerminalGUI->getRxEditor() && splitTerminalGUI->getTxEditor())
        {
            if (splitTerminalGUI->getTxEditor() == window)
            {
                long i = 0;
                long j = 0;
                splitTerminalGUI->getTxEditor()->GetSelection(&i, &j);
                if (i != j)
                {
                    splitTerminalGUI->getTxEditor()->Cut();
                    return;
                }
            }
        }
    }

    //If no other window with copyable data is selected, just copy the current block (or current selection)
    //in the block editor (this will be more handy, I think):
    if (canvas)
    {
        bubble.getCurrentCanvas()->onCutBlock(evt);
        return;
    }
}


void MainFrame::onMenuEditPaste(wxCommandEvent& evt)
{
    //##Future: Find the object with the focus, and call it's method for this action:
    if (bubble.getCurrentCanvas())
        bubble.getCurrentCanvas()->onPasteBlock(evt);
}


void MainFrame::onMenuEditDelete(wxCommandEvent& evt)
{
    //##Future: Find the object with the focus, and call it's method for this action:
    if (bubble.getCurrentCanvas())
        bubble.getCurrentCanvas()->cutBlock(bubble.getCurrentCanvas()->getCurrentBlock(), false);
}


void MainFrame::onMenuCommentUncomment(wxCommandEvent& evt)
{
    //##Future: Find the object with the focus, and call it's method for this action (for example, the code
    //viewer may be a valid object for this action):
    if (bubble.getCurrentCanvas())
        bubble.getCurrentCanvas()->commentUncommentBlock(bubble.getCurrentCanvas()->getCurrentBlock());
}


void MainFrame::onMenuEditFind(wxCommandEvent& evt)
{
    //##Future: Find the object with the focus, and call it's method for this action:
    //##Implementar el diálogo de búsqueda (por ahora se podría usar uno estándar de wxWidgets, para
    //la primera versión, ver esto bien):
        if (bubble.getCurrentCanvas())
        bubble.getCurrentCanvas()->findBlock(wxString(""));
}


void MainFrame::onMenuEditFindNext(wxCommandEvent& evt)
{
    //##Future: Find the object with the focus, and call it's method for this action:
    //##Implementar...
}


void MainFrame::onMenuComponent(wxCommandEvent& evt)
{
    if (toolMainMenu)
    {
        evt.SetEventObject(toolMainMenu);
        evt.SetId(ID_MenuComponent);
    }
    popUpMenu(evt, popComponent);
}


void MainFrame::onMenuComponentRun(wxCommandEvent& evt)
{
    //##Nota importante: En todos estos menúes que generan código, etc., hay que agregar, cuando esté
    //implementada, la opción de "grabar antes de compilar/distribuir", y hay que implementarla en los 3 casos.

    //This is to avoid GUI problems (like flickering, etc.) whith things such as the serial port combobox:
    if (bubble.getCurrentCanvas())
        bubble.getCurrentCanvas()->SetFocusIgnoringChildren();

    //##Patch (Juan Pizarro) to avoid the bug on Linux, where for some (still unknown reason), the call to
    //commManager->close() from deployStartedNotify() is not enough to close the terminal when the bootloader
    //is called. Perhaps is a matter of lack of delay between the port release by the commManager and the
    //start of the bootloading process.
    //##Future: The user must be able to configure if he wants to close the ports when the terminal ports
    //are different from Bubble's bootPort:
    if (commManager)
        commManager->close();

    saveAllEditorFiles();
    bubble.run();
}


void MainFrame::onMenuComponentBuild(wxCommandEvent& evt)
{
    //This is to avoid GUI problems (like flickering, etc.) whith things such as the serial port combobox:
    if (bubble.getCurrentCanvas())
        bubble.getCurrentCanvas()->SetFocusIgnoringChildren();

    saveAllEditorFiles();
    bubble.build();
}


void MainFrame::onMenuComponentDeploy(wxCommandEvent& evt)
{
    //This is to avoid GUI problems (like flickering, etc.) whith things such as the serial port combobox:
    if (bubble.getCurrentCanvas())
        bubble.getCurrentCanvas()->SetFocusIgnoringChildren();
    bubble.deploy();
}


void MainFrame::onMenuComponentMakePortable(wxCommandEvent& evt)
{
    bubble.makePortable();
}


void MainFrame::onMenuComponentOpenFolder(wxCommandEvent& evt)
{
    //##Si if this code is portable, and if this will show some kind or error message in case the command fails:
    wxString folderPath = bubble.getComponentPath();
//    folderPath.Replace("/", "\\");  //##Horrible hardcoded!

//##Works, but is less portable and shows a horrible command window:
//    wxString cmd =  _("explorer ") + folderPath;
//    if (wxShell(cmd))

    if (!wxLaunchDefaultApplication(folderPath))
    {
        showMessage(_("Can't find folder ") + folderPath, //##Un-hardcode.
                    true,
                    true,
                    *wxRED);
    }
}


void MainFrame::onMenuView(wxCommandEvent& evt)
{
    if (toolMainMenu)
    {
        evt.SetEventObject(toolMainMenu);
        evt.SetId(ID_MenuView);
    }
    popUpMenu(evt, popView);
}


void MainFrame::onCloseCurrentWindow(wxCommandEvent& evt)
{
    //##
}


void MainFrame::onCloseMessages(wxCommandEvent& evt)
{
    closeWindow("Messages", menuViewMessages);
    //evt.Skip();
}


void MainFrame::showWindow(const wxString& name, wxMenuItem *menuItem)
{
    if (!auiManager.GetPane(name).IsShown())
    {
        auiManager.GetPane(name).Show();
        if (menuItem)
            menuItem->Check(true);
    }
}


void MainFrame::closeWindow(const wxString& name, wxMenuItem *menuItem, bool update)
{
    if (auiManager.GetPane(name).IsShown())
    {
        auiManager.GetPane(name).Hide();
        if (menuItem)
            menuItem->Check(false);
    }
    if (update)
        auiManager.Update();
}


void MainFrame::toggleWindow(const wxString& name, wxMenuItem *menuItem)
{
    if (auiManager.GetPane(name).IsShown())
    {
        auiManager.GetPane(name).Hide();
        if (name == wxString("Terminal")) //##Un-hardcode.
        {
            if (commManager)
                commManager->close();
        }
        if (menuItem)
            menuItem->Check(false);
    }
    else
    {
        auiManager.GetPane(name).Show();
        if (menuItem)
            menuItem->Check(true);
        if (name == wxString("Terminal")) //##Un-hardcode.
        {
            //This ensures that the focus returns to the last focused terminal (single or split):
            if (terminal)
            {
                if (singleTerminalGUI && splitTerminalGUI)
                {
                    if (terminal->GetSelection() ==  terminal->GetPageIndex(singleTerminalGUI))
                        singleTerminalGUI->SetFocus();
                    else
                        splitTerminalGUI->SetFocus();
                }
            }
        }
    }
    auiManager.Update();
}


void MainFrame::onMenuViewHelpAndResourceCenter(wxCommandEvent& evt)
{
    if (useExternalHelpCenter)
    {
        wxLaunchDefaultBrowser(bubble.getDocPath() + wxString("/") + bubble.getLanguageCode() + wxString("/Help.Main.External.htm"));
    }
    else
    {
        toggleWindow("HelpAndResourceCenter", menuViewHelpAndResourceCenter);
    }
}


void MainFrame::onMenuViewLabels(wxCommandEvent& evt)
{
    if (menuViewLabels)
    {
        bubble.setVisibleLabels(!bubble.getVisibleLabels());
        menuViewLabels->Check(bubble.getVisibleLabels());
    }
}

void MainFrame::onMenuViewComponents(wxCommandEvent& evt)
{
    toggleWindow("Components", menuViewComponents);
}


void MainFrame::onMenuViewHardware(wxCommandEvent& evt)
{
    toggleWindow("Hardware", menuViewHardware);
}


void MainFrame::onMenuViewLocalVariables(wxCommandEvent& evt)
{
    toggleWindow("LocalVariables", menuViewLocalVariables);
}


void MainFrame::onMenuViewProperties(wxCommandEvent& evt)
{
    toggleWindow("Properties", menuViewProperties);
}


void MainFrame::onMenuViewMessages(wxCommandEvent& evt)
{
    toggleWindow("Messages", menuViewMessages);
}


void MainFrame::onMenuViewTerminal(wxCommandEvent& evt)
{
    toggleWindow("Terminal", menuViewTerminal);
    //##Implement a mechanism to set the focus to single or to split depending on which used the user last
    //time the terminal window was visible:
    //if (singleTerminalGUI)
    //    singleTerminalGUI->SetFocus();
}


void MainFrame::onMenuViewQuickToolbar(wxCommandEvent& evt)
{
    toggleWindow("QuickToolBar", menuViewQuickToolbar);
}


void MainFrame::refreshGeneratedCode()
{
    if (notebook == NULL)
        return;

    if (notebook->GetPageIndex(editCode) == wxNOT_FOUND)
        return;

    if (editCode == NULL)
        return;

    //##May be in the future: This could be optimized, just to update the modified lines of the code editor:
    editCode->SetReadOnly(false);
    editCode->clear();
    Bubble::linesFromArrayToBubbleEditor(bubble.getGeneratedCode(), editCode);

//    int i = bubble.getCodeFirstModifiedLineNumber();
//    int j = bubble.getCodeLastModifiedLineNumber();
//    //editCode->MarkerAdd(0, 0); //##Debug
//    //editCode->MarkerAdd(editCode->GetLineCount()-1, 0); //##Debug
//    if (i != wxNOT_FOUND)
//    {
//        //editCode->MarkerDeleteAll(0); //Not necessary, due the call to editClode->clear() above.
//        editCode->MarkerSetBackground(0, wxColour(0, 255, 0)); //##This may be done only once (and must be user-configurable)...
//        //Marks all lines between the first and the last mofidified lines:
//        if (i < editCode->GetLineCount())
//            editCode->MarkerAdd(i, 0);
////Don't delete: This marks all the lines between the first and the last modified lines:
//        while (i < j)
//        {
//            i++;
//            if (i < editCode->GetLineCount())
//                editCode->MarkerAdd(i, 0);
//            else
//                break;
//        }
////Don't delete: This marks the first and the last lines:
////        if (i != j)
////        {
////            if (j < editCode->GetLineCount())
////                editCode->MarkerAdd(j, 0);
////        }
//        editCode->GotoLine(i);
//    }

    editCode->SetReadOnly(true);

//    if (bubble.getCurrentCanvas())
//        bubble.getCurrentCanvas()->SetFocusIgnoringChildren();  //##For this line: Still not working 100% Ok (due to the
//                                                                //problem with the expressionPickers's focus).
}


void MainFrame::setEditorSyntax(BubbleEditor *editor)
{
    if (editor == NULL)
        return;

    if (bubble.getHardwareManager())
    {
        if (bubble.getHardwareManager()->getCurrentBoardProperties())
        {
            editor->SetLexer(bubble.getHardwareManager()->getCurrentBoardProperties()->getCodeLexer());

            editor->StyleSetForeground(wxSTC_C_OPERATOR, bubble.getHardwareManager()->getCurrentBoardProperties()->getCodeOperatorColor());
            editor->StyleSetForeground(wxSTC_C_STRING, bubble.getHardwareManager()->getCurrentBoardProperties()->getCodeStringColor());
            editor->StyleSetForeground(wxSTC_C_PREPROCESSOR, bubble.getHardwareManager()->getCurrentBoardProperties()->getCodePreprocessorColor());
            editor->StyleSetForeground(wxSTC_C_IDENTIFIER, bubble.getHardwareManager()->getCurrentBoardProperties()->getCodeIdentifierColor());
            editor->StyleSetForeground(wxSTC_C_NUMBER, bubble.getHardwareManager()->getCurrentBoardProperties()->getCodeNumberColor());
            editor->StyleSetForeground(wxSTC_C_CHARACTER, bubble.getHardwareManager()->getCurrentBoardProperties()->getCodeCharacterColor());
            editor->StyleSetForeground(wxSTC_C_WORD, bubble.getHardwareManager()->getCurrentBoardProperties()->getCodeWordColor());
            editor->StyleSetForeground(wxSTC_C_WORD2, bubble.getHardwareManager()->getCurrentBoardProperties()->getCodeWord2Color());
            editor->StyleSetForeground(wxSTC_C_COMMENT, bubble.getHardwareManager()->getCurrentBoardProperties()->getCodeCommentColor());
            editor->StyleSetForeground(wxSTC_C_COMMENTLINE, bubble.getHardwareManager()->getCurrentBoardProperties()->getCodeCommentLineColor());
            editor->StyleSetForeground(wxSTC_C_COMMENTDOC, bubble.getHardwareManager()->getCurrentBoardProperties()->getCodeCommentDocColor());
            editor->StyleSetForeground(wxSTC_C_COMMENTDOCKEYWORD, bubble.getHardwareManager()->getCurrentBoardProperties()->getCodeCommentDocKeywordColor());
            editor->StyleSetForeground(wxSTC_C_COMMENTDOCKEYWORDERROR, bubble.getHardwareManager()->getCurrentBoardProperties()->getCodeCommentDocKeywordErrorColor());

            //##This is not working (although the bold font seems to be setting correctly):
            editor->StyleSetBold(wxSTC_C_OPERATOR, bubble.getHardwareManager()->getCurrentBoardProperties()->getCodeOperatorBold());
            editor->StyleSetBold(wxSTC_C_STRING, bubble.getHardwareManager()->getCurrentBoardProperties()->getCodeStringBold());
            editor->StyleSetBold(wxSTC_C_PREPROCESSOR, bubble.getHardwareManager()->getCurrentBoardProperties()->getCodePreprocessorBold());
            editor->StyleSetBold(wxSTC_C_IDENTIFIER, bubble.getHardwareManager()->getCurrentBoardProperties()->getCodeIdentifierBold());
            editor->StyleSetBold(wxSTC_C_NUMBER, bubble.getHardwareManager()->getCurrentBoardProperties()->getCodeNumberBold());
            editor->StyleSetBold(wxSTC_C_CHARACTER, bubble.getHardwareManager()->getCurrentBoardProperties()->getCodeCharacterBold());
            editor->StyleSetBold(wxSTC_C_WORD, bubble.getHardwareManager()->getCurrentBoardProperties()->getCodeWordBold());
            editor->StyleSetBold(wxSTC_C_WORD2, bubble.getHardwareManager()->getCurrentBoardProperties()->getCodeWord2Bold());
            editor->StyleSetBold(wxSTC_C_COMMENT, bubble.getHardwareManager()->getCurrentBoardProperties()->getCodeCommentBold());
            editor->StyleSetBold(wxSTC_C_COMMENTLINE, bubble.getHardwareManager()->getCurrentBoardProperties()->getCodeCommentLineBold());
            editor->StyleSetBold(wxSTC_C_COMMENTDOC, bubble.getHardwareManager()->getCurrentBoardProperties()->getCodeCommentDocBold());
            editor->StyleSetBold(wxSTC_C_COMMENTDOCKEYWORD, bubble.getHardwareManager()->getCurrentBoardProperties()->getCodeCommentDocKeywordBold());
            editor->StyleSetBold(wxSTC_C_COMMENTDOCKEYWORDERROR, bubble.getHardwareManager()->getCurrentBoardProperties()->getCodeCommentDocKeywordErrorBold());

            //##editor->SetTabIndents(1);
            editor->SetTabWidth(bubble.getHardwareManager()->getCurrentBoardProperties()->getCodeTabWidth());

            // ##Load all of these from XML files:
            editor->SetKeyWords(0, bubble.getHardwareManager()->getCurrentBoardProperties()->getCodeKeywords0());
            editor->SetKeyWords(1, bubble.getHardwareManager()->getCurrentBoardProperties()->getCodeKeywords1());
            //editor->SetKeyWords(0, wxString("return for while break continue if else true false delay"));
            //editor->SetKeyWords(1, wxString("unsigned volatile const int float void char double motor0 motor1 serial0 serial1 setPower"));
        }
    }
}


void MainFrame::createCodeEditor(const wxString &fullFileName, BubbleEditor *editor)
{
    if (editor == NULL)
        return;
    if (notebook == NULL)
        return;
    if (bubble.getHardwareManager() == NULL)
        return;

    setEditorSyntax(editor);
    editor->SetZoom(getEditCodeZoom()); //##Zoom management is not ready yet!

    int iconW = 16; //##Unhardcode
    int iconH = 16;

    wxBitmap page_bmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(iconW, iconH)); //##
    if (bubble.getHardwareManager())
    {
        if (editCode)
        {
            //This is to always add the editor next to the generated code window, not to the canvas:
            notebook->SetSelection(notebook->GetPageIndex(editCode));
        }
        if (notebook->AddPage(editor, fullFileName.AfterLast(wxChar('/')), false, page_bmp))
        {
            if (wxFile::Exists(fullFileName))
                editor->LoadFile(fullFileName);
            //notebook->Split(notebook->GetPageIndex(editor), wxLEFT);
//                if (bubble.getCurrentCanvas())
//                    notebook->Split(notebook->GetPageIndex(bubble.getCurrentCanvas()), wxLEFT);
//                if (editCode)
//                    notebook->Split(notebook->GetPageIndex(editCode), wxRIGHT);
            editor->Show(true);

            if (editor != editCode)
            {
                wxBitmap page_bmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16, 16));
                notebook->SetPageBitmap(notebook->GetPageIndex(editor), page_bmp);
            }

            notebook->SetSelection(notebook->GetPageIndex(editor));
            editor->SetFocus();
        }
    }
}


void MainFrame::toggleGeneratedCode()
{
    if (notebook == NULL)
        return;

    if (bubble.getFileEditorHash())
    {
        if (bubble.getFileEditorHash()->size() > 0)
        {
            //If there are files, try to set focus on the first one, to show the editCode next to it:
            FileEditorHash::iterator it;
            it = bubble.getFileEditorHash()->begin();
            BubbleEditor *value = it->second;
            if (value)
            {
                value->SetFocus();
            }
        }
    }

    if (notebook->GetPageIndex(editCode) == wxNOT_FOUND )
    {
        //##Does de wxAUI automatically deletes this child when closed by user? It's possible, but have to test
        //well this stuff:
        editCode = new BubbleEditor(this, &bubble, wxNewId());
        if (editCode == NULL)
            return;

        if (bubble.getHardwareManager() == NULL)
            return;
        if (bubble.getHardwareManager()->getCurrentBoardProperties() == NULL)
            return;

        setEditorSyntax(editCode);

        editCode->SetZoom(getEditCodeZoom());
        //##Debug:
//        wxMessageDialog dialog0(this, _("zoom"), wxString("zoom = ") << getEditCodeZoom());
//        dialog0.ShowModal();

        int iconW = 16; //##Unhardcode
        int iconH = 16;

        wxBitmap page_bmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(iconW, iconH)); //##

        if (bubble.getHardwareManager())
        {
            if (bubble.getHardwareManager()->getCurrentBoardProperties())
            {
                if (notebook->AddPage(editCode,
                                      tempComponentName.BeforeLast('.') + wxString(".") +
                                      bubble.getHardwareManager()->getCurrentBoardProperties()->getOutputMainFileExtension(),
                                      false, page_bmp)
                   )
                {
                    //notebook->Split(notebook->GetPageIndex(editCode), wxRIGHT);
                    if (bubble.getCurrentCanvas())
                    {
                        //If there are other text files opened, then do not do the split:
                        if (bubble.getFileEditorHash())
                        {
                            if (bubble.getFileEditorHash()->size() == 0)
                            {
                                notebook->Split(notebook->GetPageIndex(bubble.getCurrentCanvas()), wxLEFT);
                                //wxMessageDialog dialog0(this, _("split!"), wxString("0") << getEditCodeZoom()); //##Debug.
                                //dialog0.ShowModal(); //##Debug.
                            }
                        }
                    }
                    toggleComponentBlocks();
                    toggleComponentBlocks();

                    editCode->Show(true);
                    bubble.updateCode();
                    refreshGeneratedCode();

                    //notebook->SetSelection(notebook->GetPageIndex(editCode));
                    editCode->SetFocus();

                    menuViewGeneratedCode->Check(true);

                }
            }
        }
    }
    else
    {
        if (editCode)
        {
            if (notebook)
            {
                if (notebook->GetPageIndex(editCode) != wxNOT_FOUND )
                {
                    setEditCodeZoom(editCode->GetZoom());
                    notebook->DeletePage(notebook->GetPageIndex(editCode));
                    editCode = NULL;
                    menuViewGeneratedCode->Check(false);
                }
            }
        }
    }
}


void MainFrame::toggleComponentBlocks()
{
    if (bubble.getCurrentCanvas())
    {
        if (notebook)
        {
            int index = notebook->GetPageIndex(bubble.getCurrentCanvas());
            if (index != wxNOT_FOUND)
            {
                showComponentBlocks(false);
            }
            else
            {
                showComponentBlocks(true);
            }
            bubble.forceSaved(false);
            //auiManager.Update();
        }
    }
}


void MainFrame::showComponentBlocks(bool value)
{
    if (bubble.getCurrentCanvas() == NULL)
        return;
    if (notebook == NULL)
        return;

    bubble.setVisibleCanvas(value);
    if (value)
    {
        wxBitmap page_bmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16, 16));

        if (!bubble.isSaved())
            page_bmp = wxArtProvider::GetBitmap(wxART_WARNING, wxART_OTHER, wxSize(16, 16));
        notebook->AddPage(  bubble.getCurrentCanvas(),
                            //_("New-1.mbqc"),
                            tempComponentName,
                            false,
                            page_bmp);
        notebook->SetSelection(notebook->GetPageIndex(bubble.getCurrentCanvas()));
        notebook->Split(notebook->GetPageIndex(bubble.getCurrentCanvas()), wxLEFT);
        if (menuViewComponentBlocks)
        {
            menuViewComponentBlocks->Check(true);
        }
    }
    else
    {
        int index = notebook->GetPageIndex(bubble.getCurrentCanvas());
        if (index != wxNOT_FOUND)
        {
            wxWindow *page = notebook->GetPage(index);
            if (page)
            {
                notebook->RemovePage(index);
                page->Hide();
                if (menuViewComponentBlocks)
                {
                    menuViewComponentBlocks->Check(false);
                }
            }
        }
    }
}


void MainFrame::onMenuViewComponentBlocks(wxCommandEvent& evt)
{
    toggleComponentBlocks();
}


void MainFrame::onMenuViewGeneratedCode(wxCommandEvent& evt)
{
    toggleGeneratedCode();
}


void MainFrame::onMenuViewFullScreen(wxCommandEvent& evt)
{
    //##Esto sólo funciona si no se hizo un resize de la ventana. De otro modo anda mal.
    //Hay que ver qué se hace. Por ahora no se implementa en la primer versión...
    if (evt.IsChecked())
        SetWindowStyle(wxMAXIMIZE | wxSUNKEN_BORDER);
    else
        SetWindowStyle(wxDEFAULT_FRAME_STYLE); //##Restaurar al estado anterior, al que hay que
                                                     //guardar antes de esta operación...

}


void MainFrame::onMenuViewPrevView(wxCommandEvent& evt)
{
    //##Implementar...
}


void MainFrame::onMenuViewNextView(wxCommandEvent& evt)
{
    //##Implementar...
}


void MainFrame::onQuickZoom(wxCommandEvent& evt)
{
    if (toolQuick)
    {
        evt.SetEventObject(toolQuick);
        evt.SetId(ID_QuickZoom);
    }
    popUpMenu(evt, popZoom);
}


void MainFrame::onExit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}


void MainFrame::onDecZoom(wxCommandEvent& evt)
{
    //##Future: Find the object with the focus, and call it's method for this action:
}


void MainFrame::onIncZoom(wxCommandEvent& evt)
{
    //##Future: Find the object with the focus, and call it's method for this action:
}


//////////////////////////////////////////
//##Revisar:
//////////////////////////////////////////
void MainFrame::onPaneClose(wxAuiManagerEvent& evt)
{
    if (evt.pane->name == wxString("toolMainMenu"))
    {
        //I would prefer to not draw the close button in the menu toolbar, but in the current version of wxAUI
        //it's impossible (the program crashes at startup!), so we avoid the close event here:
        evt.Veto();
    }
    //##Terminar de implementar esto para todos los menúes:
    else if (evt.pane->name == wxString("Components"))
    {
        menuViewComponents->Check(false);
    }
    else if (evt.pane->name == wxString("Hardware"))
    {
        menuViewHardware->Check(false);
    }
    else if (evt.pane->name == wxString("LocalVariables"))
    {
        menuViewLocalVariables->Check(false);
    }
    else if (evt.pane->name == wxString("Properties"))
    {
        menuViewProperties->Check(false);
    }
    else if (evt.pane->name == wxString("Messages"))
    {
        menuViewMessages->Check(false);
    }
    else if (evt.pane->name == wxString("Terminal"))
    {
        menuViewTerminal->Check(false);
        if (commManager)
            commManager->close();
    }
    else if (evt.pane->name == wxString("QuickToolBar"))
    {
        menuViewQuickToolbar->Check(false);
    }
    else if (evt.pane->name == wxString("GeneratedCode"))
    {
        //menuViewGeneratedCode->Check(false);

        //##Ver si tengo que hacer un delete del editCode acá, o si en realidad lo maneja wxWidgets
        //automáticamente como hace con los childs (porque editCode se crea con new() y no lo destruyo a mano
        //en ninguna parte...

        if (editCode) //Redundant security, just in case...
        {
            if (notebook)
            {
                if (notebook->GetPageIndex(editCode) != wxNOT_FOUND )
                {
                    setEditCodeZoom(editCode->GetZoom());
                    notebook->DeletePage(notebook->GetPageIndex(editCode));
                    editCode = NULL;
                    menuViewGeneratedCode->Check(false);
                }
            }
        }
    }
    else if (evt.pane->name == wxString("HelpAndResourceCenter"))
    {
        menuViewHelpAndResourceCenter->Check(false);
    }
}


bool MainFrame::addLocaleCatalog(const wxString& value)
{
    lastAddedLocaleCatalog = value;
    if (!locale.AddCatalog(lastAddedLocaleCatalog))
    {
        showMessage(_("Can't load ") + lastAddedLocaleCatalog, true, true, *wxRED);
        return false;
    }
    return true;
}


void MainFrame::zoomNotify()
{
    //##This will change in the future:
    unsigned int zoomIndex = bubble.getCurrentCanvas()->getZoomIndex();
    if (zoomIndex == 0) //##Implement this with an array, or at least with constants (unhardcode!).
    {
        if (menuZoom10)
        {
            toolZoom->SetLabel(_("10 %"));
            menuZoom10->Check(true);
        }
    }
    else if (zoomIndex == 1)
    {
        if (menuZoom25)
        {
            toolZoom->SetLabel(_("25 %"));
            menuZoom25->Check(true);
        }
    }
    else if (zoomIndex == 2)
    {
        if (menuZoom50)
        {
            toolZoom->SetLabel(_("50 %"));
            menuZoom50->Check(true);
        }
    }
    else if (zoomIndex == 3)
    {
        if (menuZoom75)
        {
            toolZoom->SetLabel(_("75 %"));
            menuZoom75->Check(true);
        }
    }
    else if (zoomIndex == 4)
    {
        if (menuZoom100)
        {
            toolZoom->SetLabel(_("100 %"));
            menuZoom100->Check(true);
        }
    }
    else if (zoomIndex == 5)
    {
        if (menuZoom125)
        {
            toolZoom->SetLabel(_("125 %"));
            menuZoom125->Check(true);
        }
    }
    else if (zoomIndex == 6)
    {
        if (menuZoom150)
        {
            toolZoom->SetLabel(_("150 %"));
            menuZoom150->Check(true);
        }
    }
    else if (zoomIndex == 7)
    {
        if (menuZoom175)
        {
            toolZoom->SetLabel(_("175 %"));
            menuZoom175->Check(true);
        }
    }
    else if (zoomIndex == 8)
    {
        if (menuZoom200)
        {
            toolZoom->SetLabel(_("200 %"));
            menuZoom200->Check(true);
        }
    }
/*##Futuro:
    else if (zoomIndex == 7)
    {
        if (menuZoomViewAll)
        {
            toolZoom->SetLabel(_("View all")); //##Traducir
            menuZoomViewAll->Check(true);
        }
    }
*/
    toolQuick->Update();
    toolQuick->Refresh();
}


//##void MainFrame::processTerminationNotify(BubbleProcess *process)
void MainFrame::processTerminationNotify()
{
    //##Debug:
//    wxMessageDialog dialog0(this, _("Deploy finished"), _("Info"));
//    dialog0.ShowModal();
}


void MainFrame::changeBoardNotify()
{
    //##Debug:
    //wxMessageDialog dialog0(this, tempComponentName, _("changeBoardNotify"));
    //dialog0.ShowModal();

    //##Destroy the current canvas (but giving to the user the chance to save his work) and creates a new
    //empty component.

    //This must be evaluated this way, because in the MainFrame constructor, the hardwareManager is created
    //before a canvas exists, and BEFORE A CANVAS CAN BE ADDED (because the blocks are not loaded yet), so
    //createComponent can not be called because if there is no currentCanvas createComponent() will try to
    //add one. So this if() prevents a crash on application startup (which is the only situation where there
    //is no current Canvas and the application tries to call createComponent()):
    if (bubble.getCurrentCanvas())
        createComponent(false);
}


void MainFrame::refreshGeneratedCodeNotify()
{
    refreshGeneratedCode();
}


void MainFrame::deployStartedNotify()
{
    //##Future: The user must be able to configure if he wants to close the ports when the terminal ports
    //are different from Bubble's bootPort:
    if (commManager)
        commManager->close();
}


void MainFrame::clearMessage()
{
    wxTextCtrl* txtMsg = messages->getTxtMsg();
    if (txtMsg)
        txtMsg->Clear();
}


void MainFrame::showMessage(const wxString &msg, bool clear, bool showWindow,
                            const wxColour& colour, bool clearProgressBar)
{
    if (messages)
    {
        showMessageArea(true);

        //##Package every possible thing into the class Message:
        wxTextCtrl* txtMsg = messages->getTxtMsg();
        if (txtMsg)
        {
            txtMsg->Show(true);

            if (clear)
                txtMsg->Clear();
            //txtMsg->AppendText(msg);
            messages->setTextColour(colour);
            txtMsg->WriteText(msg);
            txtMsg->Update();
            txtMsg->Refresh();
            //txtMsg->Update();
            //txtMsg->AppendText(""); //##Why clears everything?
            txtMsg->SetInsertionPoint(txtMsg->GetLastPosition()); //##Is it ok or there is a "- 1" missing?

            if (showWindow)
            {
                //showMessagesWindow();

                if (auiManager.GetPane("Messages").IsShown())
                    return;
                auiManager.GetPane("Messages").MinSize(wxSize(200, 150)).Show();
                auiManager.Update();
                if (menuViewMessages)
                    menuViewMessages->Check(true);
            }
        }
        if (clearProgressBar)
        {
            wxGauge* gauge = messages->getProgress();
            if (gauge)
                gauge->SetValue(0);
        }
    }
}


void MainFrame::showMessagesWindow()
{
    if (auiManager.GetPane("Messages").IsShown())
        return;
    auiManager.GetPane("Messages").Show();
    auiManager.Update();
    if (menuViewMessages)
        menuViewMessages->Check(true);
}


void MainFrame::hideMessagesWindow()
{
    closeWindow("Messages", menuViewMessages); //##Un-hardcode
}


void MainFrame::showMessageArea(bool value)
{
    if (showingMessageArea == value)
        return;
    closeWindow("Messages", menuViewMessages);
    showingMessageArea = value;

    wxAuiPaneInfo& paneInfo = auiManager.GetPane("Messages");
    if (paneInfo.IsOk())
    {
        if (messages)
        {
            if (messages->getTxtMsg())
                messages->getTxtMsg()->Show(value);
        }

        if (value)
        {
            paneInfo.BestSize(wxSize(200, 150)).MinSize(wxSize(200, 150));
        }
        else
        {
            wxGauge* gauge = messages->getProgress();
            if (gauge)
            {
               paneInfo .BestSize(wxSize(200, gauge->GetSize().GetHeight()))
                        .MinSize(wxSize(200, gauge->GetSize().GetHeight()));
               //paneInfo.BestSize( wxSize(200, 30) ); //Test.
            }
        }
        auiManager.Update();
//        Refresh();
//        Update();
    }
}


//##Ver si le cambio el nombre a algo más "wxWidgets", tipo setProgressRange, pero no sé:
void MainFrame::setProgressMax(unsigned int value)
{
    //##Encapsular esto en la clase Message:
    if (messages)
    {
        wxGauge* gauge = messages->getProgress();
        if (gauge)
            gauge->SetRange(value);
    }
}


unsigned int MainFrame::getProgressMax() const
{
    if (messages)
    {
        wxGauge* gauge = messages->getProgress();
        if (gauge)
            return gauge->GetRange();
    }
    return 0;
}


void MainFrame::setProgressPosition(unsigned int value, bool showWindow, bool showMessages)
{
    //##Encapsular esto en la clase Message:
    if (messages)
    {
        //##Agregar la recíproca, que sea "showProgress" en showMessages(), de modo que se puedan mostrar los
        //mensajes sin la barra de progreso, para ganar área:
        showMessageArea(showMessages);

        if (showWindow)
            showMessagesWindow();

        wxGauge* gauge = messages->getProgress();
        if (gauge)
            gauge->SetValue(value);
    }
}


unsigned int MainFrame::getProgressPosition() const
{
    //##Encapsular esto en la clase Message:
    if (messages)
    {
        wxGauge* gauge = messages->getProgress();
        if (gauge)
            return gauge->GetValue();
    }
    return 0;
}


void MainFrame::textChanged(BubbleEditor *source)
{
    if (source == NULL)
        return;

    if (notebook)
    {
        int index = notebook->GetPageIndex(source);
        if (index != wxNOT_FOUND)
        {
            if (source != editCode)
            {
                //if (currentEditor->GetModify())
                //{
                    wxBitmap page_bmp = wxArtProvider::GetBitmap(wxART_WARNING, wxART_OTHER, wxSize(16, 16));
                    notebook->SetPageBitmap(index, page_bmp);
                //}
            }
        }
    }
}


void MainFrame::canvasChanged(BubbleCanvas *source)
{
    if (source == NULL)
        return;

    if (notebook)
    {
        int index = notebook->GetPageIndex(source);
        if (index != wxNOT_FOUND)
        {
            wxBitmap page_bmp = wxArtProvider::GetBitmap(wxART_WARNING, wxART_OTHER, wxSize(16, 16));
            notebook->SetPageBitmap(index, page_bmp);
        }
    }
}


void MainFrame::addBubbleFile(const wxString &fileName)
{
    wxString strComponentFilesPath = bubble.getComponentFilesPath();
    strComponentFilesPath.Replace("\\", "/");
    wxString destFileName = strComponentFilesPath + "/" + fileName;

    BubbleEditor *newEditor = new BubbleEditor(this, &bubble, wxNewId());
    if (newEditor)
    {
        if (bubble.addFile(destFileName, newEditor))
        {
            //If the file was added, open it, creating an new editor:
            createCodeEditor(destFileName, newEditor); //Passes the full fileName.
        }
        else
        {
            //If not, destroy the new created editor:
            delete newEditor;
            newEditor = NULL; //Not necessary.
        }
    }
}


void MainFrame::onNotebookPageClose(wxAuiNotebookEvent& evt)
{
    //##Por ahora, no se permite cerrar desde la "X" al component:
    wxAuiNotebook* ctrl = (wxAuiNotebook *)evt.GetEventObject();
    if (ctrl->GetPage(evt.GetSelection())->IsKindOf(CLASSINFO(BubbleCanvas)))
    {
        if (notebook)
        {
            int index = notebook->GetPageIndex(bubble.getCurrentCanvas());
            if (index != wxNOT_FOUND)
            {
                wxWindow *page = notebook->GetPage(index);
                if (page)
                {
                    notebook->RemovePage(index);
                    page->Hide();
                    if (menuViewComponentBlocks)
                    {
                        menuViewComponentBlocks->Check(false);
                        bubble.setVisibleCanvas(false);
                        //bubble.forceSaved(false);
                        //##2##
                    }
                }
            }
        }
        evt.Veto();
    }
    else if (ctrl->GetPage(evt.GetSelection())->IsKindOf(CLASSINFO(BubbleEditor)))
    {
        BubbleEditor *currentEditor = (BubbleEditor *)ctrl->GetPage(evt.GetSelection());
        if (currentEditor)
        {
            //Is the page the generatedCodeViewer?
            if (currentEditor == editCode)
            {
                menuViewGeneratedCode->Check(false);
                if (editCode)
                {
                    if (notebook)
                    {
                        if (notebook->GetPageIndex(editCode) != wxNOT_FOUND)
                        {
                            setEditCodeZoom(editCode->GetZoom());
                        }
                    }
                    editCode = NULL;
                }
            }
            else
            {
                askToSaveEditorContent(currentEditor, true);
            }
        }
    }
//    else    //##En este else entran los terminales, PERO ESTO, SI SE MUESTRA EL BOTÓN DE CIERRE EN LOS TABS
//            //DEL TERMINAL SINGLE O DEL SPLIT, ¡DA ERROR!:
////##        if (   ctrl->GetPage(evt.GetSelection())->IsKindOf(CLASSINFO(SingleTerminal)) ||
////                ctrl->GetPage(evt.GetSelection())->IsKindOf(CLASSINFO(SplitTerminal))
////            )
//    {
////        wxMessageDialog dialog0(this, imagesPath, _("toolsPath")); //Debug...
////        dialog0.ShowModal();
//
//        evt.Veto();
//    }
}


void MainFrame::askToSaveEditorContent(BubbleEditor *editor, bool removeFile)
{
    if (editor == NULL)
        return;
    if (notebook == NULL)
        return;

    wxString strComponentFilesPath = bubble.getComponentFilesPath();
    strComponentFilesPath.Replace("\\", "/");
    int i = notebook->GetPageIndex(editor);
    if (i != wxNOT_FOUND)
    {
        strComponentFilesPath += wxString("/") + notebook->GetPageText(i);

        //wxMessageDialog dialog0(this, strComponentFilesPath, _("fileName")); //##Debug.
        //dialog0.ShowModal(); //##Debug.

        BubbleEditor *currentEditor = (BubbleEditor *)notebook->GetPage(i);
        if (currentEditor)
        {
            if (currentEditor != editCode)
            {
                if (currentEditor->GetModify())
                {
                    wxMessageDialog question(   this,
                                                _("The file ") + notebook->GetPageText(i) + _(" was not saved. Do you want to save it?"),
                                                _("Question"),
                                                wxICON_QUESTION | wxYES_NO | wxYES_DEFAULT
                                            );
                    int answer = question.ShowModal();
                    if (answer == wxID_YES)
                    {
                        currentEditor->SaveFile(strComponentFilesPath);
                        wxBitmap page_bmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16, 16));
                        notebook->SetPageBitmap(i, page_bmp);
                    }
                }
            }
        }
    }
    if (removeFile)
    {
        //##Debug:
//        wxMessageDialog dialog0(this, strComponentFilesPath, _("4"));
//        dialog0.ShowModal();
        bubble.removeFile(strComponentFilesPath);
    }
}


void MainFrame::onNotebookPageChanged(wxAuiNotebookEvent& evt)
{
    //##Debug:
//    wxMessageDialog dialog0(this, imagesPath, _("toolsPath"));
//    dialog0.ShowModal();

    wxAuiNotebook* ctrl = (wxAuiNotebook *)evt.GetEventObject();
    if (ctrl->GetPage(evt.GetSelection())->IsKindOf(CLASSINFO(BubbleCanvas)))
    {
        BubbleCanvas *canvas = (BubbleCanvas *)ctrl->GetPage(evt.GetSelection());
        bubble.setCurrentCanvas(canvas);
    }
    evt.Skip(); //##
}
