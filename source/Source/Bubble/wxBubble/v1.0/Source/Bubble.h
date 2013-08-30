#ifndef Bubble__h
#define Bubble__h

#include "IBubbleFileIO.h"
#include "BubbleCanvas.h"
#include "BubbleProcess.h"
#include "BubblePicker.h" //##
#include "BubbleEditor.h"

#include <wx/process.h>
#include <wx/textfile.h>
#include <wx/xml/xml.h>

#include "include/serport.h"

#include <stack>

//##This is temporal:
#define UNDER_DEVELOPMENT   0


//##Puede que a esta clase le cambie el nombre, porque es más un intercambiador de información entre Bubble
//y el frame (o quien herede de esta clase) que un verdadero Notifier. Cumple como notifier en una dirección
//(desde Buble hacia el frame), pero del otro lado, funciona pasando los pickers, por la limitación ¿de
//wxWidgets? de establecer como parent al frame si las variables internas (punteros) pertenecen al bubble
//(esto es raro pero en las pruebas realizadas parece estar ocurriendo algo de este estilo):
//
//Abstract class that must be implemented by a GUI that wants to know when something changes in a
//BubbleCanvas:
class BubbleActionPicker; //##
class BubbleExpressionPicker; //##
class BubbleProcess; //##
class IBubbleNotifier
{
    public:
        IBubbleNotifier()
        {
        }

        virtual ~IBubbleNotifier() { }

        //To avoid use of void pointers, base class arguments/returns, etc., the user can define a friend
        //function member called from the inherited xxxNotify() method:
        virtual void zoomNotify() = 0;

        void processTermination(BubbleProcess *process)
        {
            //The process can't be deleted before the termination callback:
            if (process)
            {
                delete process;
                process = NULL;
            }
            processTerminationNotify();
        }

        virtual void changeBoardNotify() = 0;

        virtual void processTerminationNotify() = 0;

        virtual void refreshGeneratedCodeNotify() = 0;

        virtual void buildFinishedNotify() = 0;
        virtual void deployStartedNotify() = 0;
        virtual void deployFinishedNotify() = 0;
        virtual void runFinishedNotify() = 0;
        virtual void makePortableFinishedNotify() = 0;

        //##See if this has to be changed:
        virtual void clearMessage() = 0;
        virtual void showMessage(const wxString &msg, bool clear, bool showWindow,
                                 const wxColour& colour, bool clearProgressBar = true) = 0;
        virtual void hideMessagesWindow() = 0;
        virtual void showMessageArea(bool value) = 0;

        virtual void setProgressMax(unsigned int value) = 0;
        virtual unsigned int getProgressMax() const = 0;
        virtual void setProgressPosition(unsigned int value, bool showWindow, bool showMessages) = 0;
        virtual unsigned int getProgressPosition() const = 0;
};


class BubbleBoardProperties
{
    private:
        wxString name;
        wxString path;
        wxString imgMain;
        wxString imgThumb;
        wxString portType;
        wxString lang;
        wxString corePath;
        wxString core;

    public:
        BubbleBoardProperties():    name(wxString("")),
                                    path(wxString("")),
                                    imgMain(wxString("")),
                                    imgThumb(wxString("")),
                                    portType(wxString("serial")),
                                    lang(wxString("")),
                                    corePath(wxString("")),
                                    core(wxString(""))

        {
        }
        //##Ver si se necesita constructor de copia por las dudas, al menos que no haga gran cosa...

        virtual ~BubbleBoardProperties()
        {
        }

        void set(BubbleBoardProperties *boardProperties)
        {
            if (boardProperties)
            {
                setName(boardProperties->getName());
                setPath(boardProperties->getPath());
                setPortType(boardProperties->getPortType());
                setImgMain(boardProperties->getImgMain());
                setImgThumb(boardProperties->getImgThumb());
                setLang(boardProperties->getLang());
                setCorePath(boardProperties->getCorePath());
                setCore(boardProperties->getCore());
            }
        }

        inline void setName(const wxString& value) { name = value; }
        inline const wxString &getName() const { return name; }

        inline void setPath(const wxString& value) { path = value; }
        inline const wxString &getPath() const { return path; }

        inline void setImgMain(const wxString& value) { imgMain = value; }
        inline const wxString &getImgMain() const { return imgMain; }

        inline void setImgThumb(const wxString& value) { imgThumb = value; }
        inline const wxString &getImgThumb() const { return imgThumb; }

        inline void setPortType(const wxString& value) { portType = value; }
        inline const wxString &getPortType() const { return portType; }

        inline void setLang(const wxString& value) { lang = value; }
        inline const wxString &getLang() const { return lang; }

        inline void setCorePath(const wxString& value) { corePath = value; }
        inline const wxString &getCorePath() const { return corePath; }

        inline void setCore(const wxString& value) { core = value; }
        inline const wxString &getCore() const { return core; }
};

//The BubbleXML class manages the language structure (blocks info, canvases info, etc.). It DOES NOT deals
//with the load/save of programs, only with the structural things:
class Bubble; //##
class BubbleXML
{
    protected:
        Bubble *bubble;
        BubbleInstanceManager XMLVariables;
        arrayOfBlockInfo blocksInfo;

        //i18n/l10n:
        wxLocale& locale;
        wxString languageCode;

        //##Horrible, but works nice!
        BubbleBlockInfo emptyBubbleInfo;


    public:
        BubbleXML(  Bubble *const bubble,
                    wxLocale& locale) : bubble(bubble),
                                        locale(locale),
                                        languageCode(wxString("en")) //Raw English (no specific country) is the default
        {
        }

        virtual ~BubbleXML()
        {
            blocksInfo.Clear();
        }

        //Utils:
        static wxString getXMLEncodedString(const wxString &value);

        //i18n/l10n:
        inline void setLanguageCode(const wxString& value) { languageCode = value; }
        inline const wxString &getLanguageCode() const { return languageCode; }

        //Language/Blocks structure:
        //int getValidBlocksCount(const wxString& path) const; //##Not necessary by now.
        int getBlockFilesCount(const wxString& blocksPath, int flags);
        bool isXMLVariable(const wxString& variableName) const;
        wxString getInternalVariableValue(const wxString& variableName, const wxString& fileName) const;
        wxString getVariableValue(const wxString& variableName, const wxString& fileName) const;
        bool loadVariablesFromXML(const wxString& fileName, bool clearVariableList);
        bool loadBlocksPropertiesFromXML(const wxString& fileName, bool clearVariableList);

        wxSize loadSizeFromXML(wxXmlNode *node, const wxString& fileName) const;
        wxColour loadColourFromXML(wxXmlNode *node, const wxString& fileName) const;
        //wxImage *loadImageFromXML(wxXmlNode *node, const wxString& fileName) const; //##
        //wxString loadStringFromXML(wxXmlNode *node) const; //##Not necessary.

        bool addCatalog(wxString catalog);
        bool loadBlockInfoPropertiesFromXML(wxXmlNode *node, BubbleBlockInfo *blockInfo);
        bool loadBlockInfoCodeFromXML(wxXmlNode *node, BubbleBlockInfo *blockInfo);
        bool loadBlockInfoInterfaceFromXML(wxXmlNode *node, const wxString& fileName, BubbleBlockInfo *blockInfo);
        bool loadBlockInfoParamsFromXML(wxXmlNode *node, const wxString& fileName, BubbleBlockInfo *blockInfo);
        bool loadBlockInfoBrothersFromXML(wxXmlNode *node, const wxString& blockName, BubbleBlockInfo *blockInfo, wxWindow *pickersParent);
        bool loadBlockInfoFriendsFromXML(wxXmlNode *node, const wxString& blockName, BubbleBlockInfo *blockInfo, wxWindow *pickersParent);
        //##Don't delete:bool loadBlockInfoDocFromXML(const wxString& fullDocFileName, BubbleBlockInfo *blockInfo);
        bool loadBlockIncludes(const wxString& blockName, const wxString& targetName, const wxString& fileName);
        bool loadBlockLibs(const wxString& blockName, const wxString& targetName, const wxString& fileName);
        BubbleBlockInfo *loadBlockInfo(const wxString &fileName, const wxString &fullBlockFileName, wxWindow *pickersParent);
        int loadBlocksInfo(wxWindow *pickersParent, bool showPickers);
        const BubbleBlockInfo& getBlockInfo(const wxString& name, const wxString& function); //##Off for debug: const;
        bool blockIsValid(const wxString& name, const wxString& type) const;


        //Hardware:
        BubbleBoardProperties *loadBoardProperties(const wxString &fullBoardFileName);
        wxString parseCmd(const wxString &cmd);
        const wxString loadBoardBuildCommands(const wxString &fullBoardFileName);
        int loadHardwareTargets(BubbleHardwareManager *hardwareManager);
        bool loadBoardInstancesFromXML(wxXmlNode *node, BubbleCanvasInfo *canvasInfo);
        BubbleCanvasInfo getCanvasInfo(bool mainCanvas);
};


class Bubble : public IBubbleFileIO
{
    protected:
        wxWindow *parent;

        BubbleCanvas *currentCanvas;
        listOfCanvas canvases;
        BubbleActionPicker *actionPicker;
        arrayOfExpressionPickers expressionPickers;

        BubbleXML bubbleXML;

        wxString actionDataType;
        wxString variableInitName;

        IBubbleNotifier *notifier;

        wxString bootPortName;
        //##Future: There may be other serial ports, not only the main one (used for both bootloading and
        //communications in most targets):
        wxSerialPort bootSerialPort;
        BubbleHardwareManager *hardwareManager;
        wxString boardName;

        bool blocksEnabled;
        bool visibleLabels;

        std::stack<BubbleBlock *> brothers;

        //Components:
        wxString componentsRepositoryPath;
        wxString toolsPath;

        //Minibloq components:
        wxString targetsPath;
        wxString blocksPath;

        //Application (Minibloq itself):
        wxString host;
        wxString appPath;
        wxString themePath;
        //##Will be necessary?:
        wxString docPath;

        //Current (or active) component:
        wxString tempPath;
        wxString componentPath;
        wxString outputPath;

        bool simplifyCode;
        wxArrayString prevGeneratedCode;
        wxArrayString generatedCode;

        bool isSubstringInArrayString(const wxArrayString &value, const wxString& substring);
        bool findErrorStringAndShow(const wxArrayString &value);
        void showStream(const wxArrayString &value, const wxColour& colour = *wxGREEN);

        void addHeaderCode();
        void addLibrariesCode();
        void addInitializationCode();
        void addFinalizationCode();
        wxString generateParamsCode(BubbleBlock *block);

        void loadParamsFromXML(wxXmlNode *child);
        BubbleBlockInfo loadBlockFromXML(wxXmlNode *child);

        wxString generateXMLFromParams(BubbleBlock *block);

    public:
        //Construction:
        //Bubble(IBubbleNotifier *notifier);
        Bubble(wxLocale& locale);
        virtual ~Bubble();

        //Utils:
        wxWindow *getParent() { return parent; };
#if defined (WIN32)
        static LPWSTR cstrToWChar(LPCSTR value);
#endif
        static wxString bool2string(const bool value);
        static bool string2bool(const wxString &value);
        static bool nonListedCharsInString(const wxString &charList, const wxString &value);
        static wxImage dc2Image(wxClientDC *dc);
        static void linesFromArrayToBubbleEditor(const wxArrayString &strings, BubbleEditor *editor);
        static void linesFromArrayToFile(const wxArrayString &strings, wxTextFile *file);
        static wxArrayString string2Array(const wxString &value);
        static unsigned int strOcurrencesInArray(const wxString &str, const wxSortedArrayString &array);

        virtual bool isSaved() const;

        void setCanvasesParent(wxWindow *value); //##Agregar los const al parámetro
        //##Esto es casi obligatorio, si no no hay intercambio de eventos con el frame o con quien contenga la instancia de Bubble:
        void setNotifier(IBubbleNotifier *value);
        IBubbleNotifier *getNotifier();

        //Language/Blocks structure:
        bool addBlockToPicker(BubbleBlockInfo *block, wxWindow *pickersParent);
        const BubbleBlockInfo& getBlockInfo(const wxString& name, const wxString& function); //##Off for debug: const
        bool blockIsValid(const wxString& name, const wxString& type) const;
        int loadBlocksInfo(wxWindow *pickersParent, bool showPickers);

        //Hardware:
        int loadHardwareTargets(BubbleHardwareManager *hardwareManager);

        //Canvases:
        //##Es necesario asignar NULL a currentCanvas cuando no haya ninguno:
        //Verificar métodos tipo deleteCanvas, etc..
        void setCurrentCanvas(BubbleCanvas *value);
        inline BubbleCanvas *getCurrentCanvas() const { return currentCanvas; }

        void addCanvas( wxWindowID id, //##= wxID_ANY, //##Ver si devuelvo puntero al canvas nuevo...
                        bool mainCanvas,
                        int defaultScrollHInc,
                        int defaultScrollVInc
                      );
        void deleteCanvas(BubbleCanvas *const value); //##Ver qué parámetros recibe //##Ver si se podrá borrar el último canvas...

        //i18n/l10n:
        inline void setLanguageCode(const wxString& value) { bubbleXML.setLanguageCode(value); }
        inline const wxString &getLanguageCode() const { return bubbleXML.getLanguageCode(); }

        //Paths:
        inline void setComponentsRepositoryPath(const wxString& value) { componentsRepositoryPath = value; }
        inline const wxString &getComponentsRepositoryPath() const { return componentsRepositoryPath; }
        inline void setToolsPath(const wxString& value) { toolsPath = value; }
        inline const wxString &getToolsPath() const { return toolsPath; }

        inline void setTargetsPath(const wxString& value) { targetsPath = value; }
        inline const wxString &getTargetsPath() const { return targetsPath; }

        inline void setBlocksPath(const wxString& value) { blocksPath = value; }
        inline const wxString &getBlocksPath() const { return blocksPath; }

        inline void setHost(const wxString& value) { host = value; }
        inline const wxString &getHost() const { return host; }

        inline void setAppPath(const wxString& value) { appPath = value; }
        inline const wxString &getAppPath() const { return appPath; }
        inline void setThemePath(const wxString& value) { themePath = value; }
        inline const wxString &getThemePath() const { return themePath; }
        //##Will be necessary, or is an application path?:
        inline void setDocPath(const wxString& value) { docPath = value; }
        inline const wxString &getDocPath() const { return docPath; }

        inline void setTempPath(const wxString& value) { tempPath = value; }
        inline const wxString &getTempPath() const { return tempPath; }
        inline void setComponentPath(const wxString& value) { componentPath = value; }
        inline const wxString &getComponentPath() const { return componentPath; }
        inline void setOutputPath(const wxString& value) { outputPath = value; }
        inline const wxString &getOutputPath() const { return outputPath; }

        //Communications:
        inline void setBootPortName(const wxString& value) { bootPortName = value; }
        inline const wxString &getBootPortName() const { return bootPortName; }

        //Hardware:
        inline void setHardwareManager(BubbleHardwareManager *value) //##Hacer const el puntero del parámetro.
        {
            hardwareManager = value;
        }
        inline BubbleHardwareManager *getHardwareManager() const { return hardwareManager; }
        bool setBoardName(const wxString& value, wxWindow *pickersParent);
        inline const wxString &getBoardName() const { return boardName; }
        bool loadTargetFromFile(const wxString& name); //##See the question about name target/board.
        bool saveTargetToFile(const wxString& name); //##See the question about name target/board.

        //Board drivers:
        bool winInstallINF(); //##Testing.

        //Components:
        //##Ver bien si el Component será de Bubble o de Minibloq (por ahora se lo considera ligado a Bubble,
        //pues brinda generación y compilación genéricas de código -genéricas hasta cierto punto, claro-)...
        bool loadComponentFromFile(const wxString& name); //##In the future, it's possible that Bubble manages multiple components at once, and
        bool saveComponentToFile(const wxString& name, bool format = true);   //these functions will change...
        bool formatComponentFile(const wxString& name);

        void createDirs(const wxString& path);
        bool executeCmd(const wxString& cmd);
        bool run(); //##Ver si faltan parámetros
        bool deploy(); //##Ver si faltan parámetros
        bool build(); //##Ver si faltan parámetros
        bool resetBoard();
        bool verifyBoard();

        inline bool getSimplifyCode() const { return simplifyCode; }
        inline void setSimplifyCode(bool value) { simplifyCode =  value; }
        bool simplifyCodeLine(wxString &code);
        int getCodeFirstModifiedLineNumber() const;
        int getCodeLastModifiedLineNumber() const;
        bool updateCode();
        inline const wxArrayString &getGeneratedCode() const { return generatedCode; }
        bool generateCodeAndSaveToFile();

        //##: virtual bool verify();
        bool verifyMissingParams();
        bool clean();
        bool makePortable(); //##Ver si faltan parámetros
        //##Falta método para obtener qué errores se generaron.
        //##Puede que falte notifier para el progreso de los procesos de run, build, deploy, makeportable,...

        //Canvas:
        void loadAcceleratorTableFromFile();

        //Pickers:
        BubbleActionPicker *createActionPicker(wxWindow *pickerParent, bool show); //##Agregar los const al parámetro
        inline BubbleActionPicker * getActionPicker() const { return actionPicker; }
        void clearExpressionPickers();
        BubbleExpressionPicker* getExpressionPicker(const wxString &dataType);
        void closeTemporalPickers();
        void showPicker(BubblePicker* picker, const wxPoint& position, bool value);
        void togglePicker(BubblePicker* picker, const wxPoint& position);

        //Blocks info:
        inline void setActionDataType(const wxString& value) { actionDataType = value; }
        inline const wxString &getActionDataType() const { return actionDataType; }
        inline void setVariableInitName(const wxString& value) { variableInitName = value; }
        inline const wxString &getVariableInitName() const { return variableInitName; }

        //Blocks:
        inline bool areBlocksEnabled() const { return blocksEnabled; }
        void enableAllBlocks(bool value);

        //Labels:
        inline bool getVisibleLabels() const  { return visibleLabels; }
        void setVisibleLabels(bool value);
};
#endif
