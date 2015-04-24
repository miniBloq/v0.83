#ifndef Bubble__h
#define Bubble__h

#include "IBubbleFileIO.h"
#include "BubbleCanvas.h"
#include "BubbleProcess.h"
#include "BubblePicker.h"
#include "BubbleEditor.h"

#include <wx/process.h>
#include <wx/textfile.h>
#include <wx/xml/xml.h>

#include "include/serport.h"

#include <stack>
#include <wx/hashmap.h>


//##This is temporal:
#define UNDER_DEVELOPMENT   0


//Used to optimize image loading:
WX_DECLARE_STRING_HASH_MAP(wxImage, ImagesHash);

//"BlockInfos" are also searched with a hash table:
WX_DECLARE_STRING_HASH_MAP(BubbleBlockInfo*, BlocksHash);

//Files and pointers to their editor:
WX_DECLARE_STRING_HASH_MAP(BubbleEditor*, FileEditorHash);

wxColour string2color(const wxString &value);

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

        virtual void textChanged(BubbleEditor *source) = 0;
        virtual void canvasChanged(BubbleCanvas *source) = 0;

        virtual void addBubbleFile(const wxString &fileName) = 0;
};


class Bubble;
class BubbleBoardProperties
{
    private:
        wxString name;
        wxString path;
        wxString imgMain;
        wxString cpu;
        wxString clockFreq;
        wxString url0;
        wxString url1;
        wxString imgThumb;
        wxString portType;
        wxString lang;
        wxString uploader;
        wxString uploaderCmd0;
        wxString corePath;
        wxString driverPath;
        wxString core;
        wxString outputMainFileExtension;
        bool resetBeforeBuild;
        unsigned int bootBaudRate;
        unsigned int bootFindPortTries;
        unsigned int bootTimeOut;
        bool useWrapper;
        wxString headerFileExtension;
        wxString codeFileExtension;
        wxString includeCodePrefix;
        wxString includeCodePostfix;
        wxString includeCodeInstancePrefix;
        wxString includeInitCode;
        wxString includeFinalCode;
        wxString includeBuildPrefix;
        wxString includeBuildPostfix;
        wxString initBoardHeader;
        wxString initBoardPrefix;
        wxString initBoardPostfix;
        wxString commentBegin;
        wxString commentEnd;
        wxString includesCodeList;
        wxString definesCodeList;
        wxString instancesCodeList;
        wxString instancesHeaderCodeList;
        wxString initCode;
        wxString finalCode;
        wxString initBoardCode;
        wxString includesBuildList;
        wxString arduinoVersion; //This is specific to Arduino-compatible hardware, but needed by now.
        wxString objectExtension;
        wxString boardDefine;
        wxString arduinoVariant;
        wxString usbVid;
        wxString usbPidBoot;
        wxString usbPidApp;
        wxString usbManufacturer;
        wxString usbProduct;
        wxArrayString relCommands;
        wxArrayString fileExtensions;

        int codeLexer;
        wxColour codeOperatorColor;
        wxColour codeStringColor;
        wxColour codePreprocessorColor;
        wxColour codeIdentifierColor;
        wxColour codeNumberColor;
        wxColour codeCharacterColor;
        wxColour codeWordColor;
        wxColour codeWord2Color;
        wxColour codeCommentColor;
        wxColour codeCommentLineColor;
        wxColour codeCommentDocColor;
        wxColour codeCommentDocKeywordColor;
        wxColour codeCommentDocKeywordErrorColor;
        bool codeOperatorBold;
        bool codeStringBold;
        bool codePreprocessorBold;
        bool codeIdentifierBold;
        bool codeNumberBold;
        bool codeCharacterBold;
        bool codeWordBold;
        bool codeWord2Bold;
        bool codeCommentBold;
        bool codeCommentLineBold;
        bool codeCommentDocBold;
        bool codeCommentDocKeywordBold;
        bool codeCommentDocKeywordErrorBold;
        unsigned int codeTabWidth;
        wxString codeKeywords0;
        wxString codeKeywords1;

    public:
        BubbleBoardProperties():    name(wxString("")),
                                    path(wxString("")),
                                    imgMain(wxString("")),
                                    cpu(wxString("")),
                                    clockFreq(wxString("")),
                                    url0(wxString("")),
                                    url1(wxString("")),
                                    imgThumb(wxString("")),
                                    portType(wxString("serial")),
                                    lang(wxString("")),
                                    uploader(wxString("")),
                                    uploaderCmd0(wxString("")),
                                    corePath(wxString("")),
                                    driverPath(wxString("")),
                                    core(wxString("")),
                                    outputMainFileExtension(wxString("ino")), //##Arduino compatible file by default?
                                    resetBeforeBuild(false),
                                    bootBaudRate(115200),
                                    bootFindPortTries(5),
                                    bootTimeOut(200),
                                    useWrapper(false),
                                    headerFileExtension(wxString("")),
                                    codeFileExtension(wxString("")),
                                    includeCodePrefix(wxString("")),
                                    includeCodePostfix(wxString("")),
                                    includeCodeInstancePrefix(wxString("")),
                                    includeInitCode(wxString("")),
                                    includeFinalCode(wxString("")),
                                    includeBuildPrefix(wxString("")),
                                    includeBuildPostfix(wxString("")),
                                    initBoardHeader(wxString("")),
                                    initBoardPrefix(wxString("")),
                                    initBoardPostfix(wxString("")),
                                    commentBegin(wxString("")),
                                    commentEnd(wxString("")),
                                    includesCodeList(wxString("")),
                                    definesCodeList(wxString("")),
                                    instancesCodeList(wxString("")),
                                    instancesHeaderCodeList(wxString("")),
                                    initCode(wxString("")),
                                    finalCode(wxString("")),
                                    initBoardCode(wxString("")),
                                    includesBuildList(wxString("")),
                                    arduinoVersion(wxString("")),
                                    objectExtension(wxString("")),
                                    boardDefine(wxString("")),
                                    arduinoVariant(wxString("")),
                                    usbVid(wxString("")),
                                    usbPidBoot(wxString("")),
                                    usbPidApp(wxString("")),
                                    usbManufacturer(wxString("")),
                                    usbProduct(wxString("")),

                                    codeLexer(3), //CPP syntax
                                    codeOperatorColor(wxColour(0, 0, 0)),
                                    codeStringColor(wxColour(0, 0, 0)),
                                    codePreprocessorColor(wxColour(0, 0, 0)),
                                    codeIdentifierColor(wxColour(0, 0, 0)),
                                    codeNumberColor(wxColour(0, 0, 0)),
                                    codeCharacterColor(wxColour(0, 0, 0)),
                                    codeWordColor(wxColour(0, 0, 0)),
                                    codeWord2Color(wxColour(0, 0, 0)),
                                    codeCommentColor(wxColour(0, 0, 0)),
                                    codeCommentLineColor(wxColour(0, 0, 0)),
                                    codeCommentDocColor(wxColour(0, 0, 0)),
                                    codeCommentDocKeywordColor(wxColour(0, 0, 0)),
                                    codeCommentDocKeywordErrorColor(wxColour(0, 0, 0)),
                                    codeOperatorBold(false),
                                    codeStringBold(false),
                                    codePreprocessorBold(false),
                                    codeIdentifierBold(false),
                                    codeNumberBold(false),
                                    codeCharacterBold(false),
                                    codeWordBold(false),
                                    codeWord2Bold(false),
                                    codeCommentBold(false),
                                    codeCommentLineBold(false),
                                    codeCommentDocBold(false),
                                    codeCommentDocKeywordBold(false),
                                    codeCommentDocKeywordErrorBold(false),
                                    codeTabWidth(4),
                                    codeKeywords0(wxString("")),
                                    codeKeywords1(wxString(""))
        {
            relCommands.Clear(); //Not necessary, but just in case.
            fileExtensions.Clear();
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
                setCpu(boardProperties->getCpu());
                setClockFreq(boardProperties->getClockFreq());
                setUrl0(boardProperties->getUrl0());
                setUrl1(boardProperties->getUrl1());
                setImgThumb(boardProperties->getImgThumb());
                setLang(boardProperties->getLang());
                setUploader(boardProperties->getUploader());
                setUploaderCmd0(boardProperties->getUploaderCmd0());
                setCorePath(boardProperties->getCorePath());
                setDriverPath(boardProperties->getDriverPath());
                setCore(boardProperties->getCore());
                setOutputMainFileExtension(boardProperties->getOutputMainFileExtension());
                setResetBeforeBuild(boardProperties->getResetBeforeBuild());
                setBootBaudRate(boardProperties->getBootBaudRate());
                setBootFindPortTries(boardProperties->getBootFindPortTries());
                setBootTimeOut(boardProperties->getBootTimeOut());
                setUseWrapper(boardProperties->getUseWrapper());
                setHeaderFileExtension(boardProperties->getHeaderFileExtension()),
                setCodeFileExtension(boardProperties->getCodeFileExtension()),
                setIncludeCodePrefix(boardProperties->getIncludeCodePrefix());
                setIncludeCodePostfix(boardProperties->getIncludeCodePostfix());
                setIncludeCodeInstancePrefix(boardProperties->getIncludeCodeInstancePrefix());
                setIncludeInitCode(boardProperties->getIncludeInitCode());
                setIncludeFinalCode(boardProperties->getIncludeFinalCode());
                setIncludeBuildPrefix(boardProperties->getIncludeBuildPrefix());
                setIncludeBuildPostfix(boardProperties->getIncludeBuildPostfix());
                setInitBoardHeader(boardProperties->getInitBoardHeader());
                setInitBoardPrefix(boardProperties->getInitBoardPrefix());
                setInitBoardPostfix(boardProperties->getInitBoardPostfix());
                setCommentBegin(boardProperties->getCommentBegin());
                setCommentEnd(boardProperties->getCommentEnd());
                setIncludesCodeList(boardProperties->getIncludesCodeList());
                setDefinesCodeList(boardProperties->getDefinesCodeList());
                setInstancesCodeList(boardProperties->getInstancesCodeList());
                setInstancesHeaderCodeList(boardProperties->getInstancesHeaderCodeList());
                setInitCode(boardProperties->getInitCode()),
                setFinalCode(boardProperties->getFinalCode()),
                setInitBoardCode(boardProperties->getInitBoardCode()),
                setIncludesBuildList(boardProperties->getIncludesBuildList());
                setArduinoVersion(boardProperties->getArduinoVersion());
                setObjectExtension(boardProperties->getObjectExtension());
                setBoardDefine(boardProperties->getBoardDefine());
                setArduinoVariant(boardProperties->getArduinoVariant());
                setUsbVid(boardProperties->getUsbVid());
                setUsbPidBoot(boardProperties->getUsbPidBoot());
                setUsbPidApp(boardProperties->getUsbPidApp());
                setUsbManufacturer(boardProperties->getUsbManufacturer());
                setUsbProduct(boardProperties->getUsbProduct());

                setCodeLexer(boardProperties->getCodeLexer());

                setCodeOperatorColor(boardProperties->getCodeOperatorColor());
                setCodeStringColor(boardProperties->getCodeStringColor());
                setCodePreprocessorColor(boardProperties->getCodePreprocessorColor());
                setCodeIdentifierColor(boardProperties->getCodeIdentifierColor());
                setCodeNumberColor(boardProperties->getCodeNumberColor());
                setCodeCharacterColor(boardProperties->getCodeCharacterColor());
                setCodeWordColor(boardProperties->getCodeWordColor());
                setCodeWord2Color(boardProperties->getCodeWord2Color());
                setCodeCommentColor(boardProperties->getCodeCommentColor());
                setCodeCommentLineColor(boardProperties->getCodeCommentLineColor());
                setCodeCommentDocColor(boardProperties->getCodeCommentDocColor());
                setCodeCommentDocKeywordColor(boardProperties->getCodeCommentDocKeywordColor());
                setCodeCommentDocKeywordErrorColor(boardProperties->getCodeCommentDocKeywordErrorColor());

                setCodeOperatorBold(boardProperties->getCodeOperatorBold());
                setCodeStringBold(boardProperties->getCodeStringBold());
                setCodePreprocessorBold(boardProperties->getCodePreprocessorBold());
                setCodeIdentifierBold(boardProperties->getCodeIdentifierBold());
                setCodeNumberBold(boardProperties->getCodeNumberBold());
                setCodeCharacterBold(boardProperties->getCodeCharacterBold());
                setCodeWordBold(boardProperties->getCodeWordBold());
                setCodeWord2Bold(boardProperties->getCodeWord2Bold());
                setCodeCommentBold(boardProperties->getCodeCommentBold());
                setCodeCommentLineBold(boardProperties->getCodeCommentLineBold());
                setCodeCommentDocBold(boardProperties->getCodeCommentDocBold());
                setCodeCommentDocKeywordBold(boardProperties->getCodeCommentDocKeywordBold());
                setCodeCommentDocKeywordErrorBold(boardProperties->getCodeCommentDocKeywordErrorBold());

                setCodeTabWidth(boardProperties->getCodeTabWidth());

                setCodeKeywords0(boardProperties->getCodeKeywords0());
                setCodeKeywords1(boardProperties->getCodeKeywords1());

                //relCommands = *(boardProperties->getRelCommands());
                unsigned int i = 0;
                while (i < boardProperties->getRelCommandsCount())
                {
                    relCommands.Add(boardProperties->getRelCommand(i));
                    i++;
                }

                i = 0;
                fileExtensions.Clear();
                while (i < boardProperties->getFileExtensionsCount())
                {
                    fileExtensions.Add(boardProperties->getFileExtension(i));
                    i++;
                }
            }
        }

        inline void setName(const wxString& value) { name = value; }
        inline const wxString &getName() const { return name; }

        inline void setPath(const wxString& value) { path = value; }
        inline const wxString &getPath() const { return path; }

        inline void setImgMain(const wxString& value) { imgMain = value; }
        inline const wxString &getImgMain() const { return imgMain; }

        inline void setCpu(const wxString& value) { cpu = value; }
        inline const wxString &getCpu() const { return cpu; }

        inline void setClockFreq(const wxString& value) { clockFreq = value; }
        inline const wxString &getClockFreq() const { return clockFreq; }

        inline void setUrl0(const wxString& value) { url0 = value; }
        inline const wxString &getUrl0() const { return url0; }

        inline void setUrl1(const wxString& value) { url1 = value; }
        inline const wxString &getUrl1() const { return url1; }

        inline void setImgThumb(const wxString& value) { imgThumb = value; }
        inline const wxString &getImgThumb() const { return imgThumb; }

        inline void setPortType(const wxString& value) { portType = value; }
        inline const wxString &getPortType() const { return portType; }

        inline void setLang(const wxString& value) { lang = value; }
        inline const wxString &getLang() const { return lang; }

        inline void setUploader(const wxString& value) { uploader = value; }
        inline const wxString &getUploader() const { return uploader; }

        inline void setUploaderCmd0(const wxString& value) { uploaderCmd0 = value; }
        inline const wxString &getUploaderCmd0() const { return uploaderCmd0; }

        inline void setCorePath(const wxString& value) { corePath = value; }
        inline const wxString &getCorePath() const { return corePath; }

        inline void setDriverPath(const wxString& value) { driverPath = value; }
        inline const wxString &getDriverPath() const { return driverPath; }

        inline void setCore(const wxString& value) { core = value; }
        inline const wxString &getCore() const { return core; }

        inline void setOutputMainFileExtension(const wxString& value) { outputMainFileExtension = value; }
        inline const wxString &getOutputMainFileExtension() const { return outputMainFileExtension; }

        inline void setResetBeforeBuild(const bool value) { resetBeforeBuild = value; }
        inline const bool getResetBeforeBuild() const { return resetBeforeBuild; }

        inline void setBootBaudRate(const unsigned int value) { bootBaudRate = value; }
        inline const unsigned int getBootBaudRate() const { return bootBaudRate; }

        inline void setBootFindPortTries(const unsigned int value) { bootFindPortTries = value; }
        inline const unsigned int getBootFindPortTries() const { return bootFindPortTries; }

        inline void setBootTimeOut(const unsigned int value) { bootTimeOut = value; }
        inline const unsigned int getBootTimeOut() const { return bootTimeOut; }

        inline void setUseWrapper(const bool value) { useWrapper = value; }
        inline const bool getUseWrapper() const { return useWrapper; }

        inline void setHeaderFileExtension(const wxString& value) { headerFileExtension = value; }
        inline const wxString & getHeaderFileExtension() const { return headerFileExtension; }

        inline void setCodeFileExtension(const wxString& value) { codeFileExtension = value; }
        inline const wxString & getCodeFileExtension() const { return codeFileExtension; }

        inline void setIncludeCodePrefix(const wxString& value) { includeCodePrefix = value; }
        inline const wxString &getIncludeCodePrefix() const { return includeCodePrefix; }

        inline void setIncludeCodePostfix(const wxString& value) { includeCodePostfix = value; }
        inline const wxString &getIncludeCodePostfix() const { return includeCodePostfix; }

        inline void setIncludeCodeInstancePrefix(const wxString& value) { includeCodeInstancePrefix = value; }
        inline const wxString &getIncludeCodeInstancePrefix() const { return includeCodeInstancePrefix; }

        inline void setIncludeInitCode(const wxString& value) { includeInitCode = value; }
        inline const wxString &getIncludeInitCode() const { return includeInitCode; }

        inline void setIncludeFinalCode(const wxString& value) { includeFinalCode = value; }
        inline const wxString &getIncludeFinalCode() const { return includeFinalCode; }

        inline void setIncludeBuildPrefix(const wxString& value) { includeBuildPrefix = value; }
        inline const wxString &getIncludeBuildPrefix() const { return includeBuildPrefix; }

        inline void setIncludeBuildPostfix(const wxString& value) { includeBuildPostfix = value; }
        inline const wxString &getIncludeBuildPostfix() const { return includeBuildPostfix; }

        inline void setInitBoardHeader(const wxString& value) { initBoardHeader = value; }
        inline const wxString &getInitBoardHeader() const { return initBoardHeader; }

        inline void setInitBoardPrefix(const wxString& value) { initBoardPrefix = value; }
        inline const wxString &getInitBoardPrefix() const { return initBoardPrefix; }

        inline void setInitBoardPostfix(const wxString& value) { initBoardPostfix = value; }
        inline const wxString &getInitBoardPostfix() const { return initBoardPostfix; }

        inline void setCommentBegin(const wxString& value) { commentBegin = value; }
        inline const wxString &getCommentBegin() const { return commentBegin; }

        inline void setCommentEnd(const wxString& value) { commentEnd = value; }
        inline const wxString &getCommentEnd() const { return commentEnd; }

        inline void setIncludesCodeList(const wxString &value) { includesCodeList = value; }
        inline wxString getIncludesCodeList() const { return includesCodeList; }

        inline void setDefinesCodeList(const wxString &value) { definesCodeList = value; }
        inline wxString getDefinesCodeList() const { return definesCodeList; }

        inline void setInstancesCodeList(const wxString &value) { instancesCodeList = value; }
        inline wxString getInstancesCodeList() const { return instancesCodeList; }

        inline void setInstancesHeaderCodeList(const wxString &value) { instancesHeaderCodeList = value; }
        inline wxString getInstancesHeaderCodeList() const { return instancesHeaderCodeList; }

        inline void setInitCode(const wxString &value) { initCode = value; }
        inline wxString getInitCode() const { return initCode; }

        inline void setFinalCode(const wxString &value) { finalCode = value; }
        inline wxString getFinalCode() const { return finalCode; }

        inline void setInitBoardCode(const wxString &value) { initBoardCode = value; }
        inline wxString getInitBoardCode() const { return initBoardCode; }

        inline void setIncludesBuildList(const wxString &value) { includesBuildList = value; }
        inline wxString getIncludesBuildList() const { return includesBuildList; }

        inline void setArduinoVersion(const wxString& value) { arduinoVersion = value; }
        inline const wxString &getArduinoVersion() const { return arduinoVersion; }

        inline void setObjectExtension(const wxString& value) { objectExtension = value; }
        inline const wxString &getObjectExtension() const { return objectExtension; }

        inline void setBoardDefine(const wxString& value) { boardDefine = value; }
        inline const wxString &getBoardDefine() const { return boardDefine; }

        inline void setArduinoVariant(const wxString& value) { arduinoVariant = value; }
        inline const wxString &getArduinoVariant() const { return arduinoVariant; }

        inline void setUsbVid(const wxString& value) { usbVid = value; }
        inline const wxString &getUsbVid() const { return usbVid; }

        inline void setUsbPidBoot(const wxString& value) { usbPidBoot = value; }
        inline const wxString &getUsbPidBoot() const { return usbPidBoot; }

        inline void setUsbPidApp(const wxString& value) { usbPidApp = value; }
        inline const wxString &getUsbPidApp() const { return usbPidApp; }

        inline void setUsbManufacturer(const wxString& value) { usbManufacturer = value; }
        inline const wxString &getUsbManufacturer() const { return usbManufacturer; }

        inline void setUsbProduct(const wxString& value) { usbProduct = value; }
        inline const wxString &getUsbProduct() const { return usbProduct; }

        inline void clearRelCommands() { return relCommands.Clear(); };
        inline unsigned int getRelCommandsCount() const { return relCommands.GetCount(); };
        inline void addRelCommand(const wxString& value)
        {
            relCommands.Add(value);
        }
        inline const wxString getRelCommand(const unsigned int index) const
        {
            if (index < relCommands.GetCount())
                return relCommands[index];
            return wxString("");
        }

        inline void clearFileExtensions() { return fileExtensions.Clear(); };
        inline unsigned int getFileExtensionsCount() const { return fileExtensions.GetCount(); };
        inline void addFileExtension(const wxString& value)
        {
            fileExtensions.Add(value);
        }
        inline const wxString getFileExtension(const unsigned int index) const
        {
            if (index < fileExtensions.GetCount())
                return fileExtensions[index];
            return wxString("");
        }

        //Syntax (code editor):
        inline void setCodeLexer(int value) { codeLexer = value; }
        inline int getCodeLexer() const { return codeLexer; }

        inline void setCodeOperatorColor(const wxString& value) { codeOperatorColor = string2color(value); }
        inline void setCodeOperatorColor(const wxColour& value) { codeOperatorColor = value; }
        inline wxColour getCodeOperatorColor() const { return codeOperatorColor; }
        inline void setCodeStringColor(const wxString& value) { codeStringColor = string2color(value); }
        inline void setCodeStringColor(const wxColour& value) { codeStringColor = value; }
        inline wxColour getCodeStringColor() const { return codeStringColor; }
        inline void setCodePreprocessorColor(const wxString& value) { codePreprocessorColor = string2color(value); }
        inline void setCodePreprocessorColor(const wxColour& value) { codePreprocessorColor = value; }
        inline wxColour getCodePreprocessorColor() const { return codePreprocessorColor; }
        inline void setCodeIdentifierColor(const wxString& value) { codeIdentifierColor = string2color(value); }
        inline void setCodeIdentifierColor(const wxColour& value) { codeIdentifierColor = value; }
        inline wxColour getCodeIdentifierColor() const { return codeIdentifierColor; }
        inline void setCodeNumberColor(const wxString& value) { codeNumberColor = string2color(value); }
        inline void setCodeNumberColor(const wxColour& value) { codeNumberColor = value; }
        inline wxColour getCodeNumberColor() const { return codeNumberColor; }
        inline void setCodeCharacterColor(const wxString& value) { codeCharacterColor = string2color(value); }
        inline void setCodeCharacterColor(const wxColour& value) { codeCharacterColor = value; }
        inline wxColour getCodeCharacterColor() const { return codeCharacterColor; }
        inline void setCodeWordColor(const wxString& value) { codeWordColor = string2color(value); }
        inline void setCodeWordColor(const wxColour& value) { codeWordColor = value; }
        inline wxColour getCodeWordColor() const { return codeWordColor; }
        inline void setCodeWord2Color(const wxString& value) { codeWord2Color = string2color(value); }
        inline void setCodeWord2Color(const wxColour& value) { codeWord2Color = value; }
        inline wxColour getCodeWord2Color() const { return codeWord2Color; }
        inline void setCodeCommentColor(const wxString& value) { codeCommentColor = string2color(value); }
        inline void setCodeCommentColor(const wxColour& value) { codeCommentColor = value; }
        inline wxColour getCodeCommentColor() const { return codeCommentColor; }
        inline void setCodeCommentLineColor(const wxString& value) { codeCommentLineColor = string2color(value); }
        inline void setCodeCommentLineColor(const wxColour& value) { codeCommentLineColor = value; }
        inline wxColour getCodeCommentLineColor() const { return codeCommentLineColor; }
        inline void setCodeCommentDocColor(const wxString& value) { codeCommentDocColor = string2color(value); }
        inline void setCodeCommentDocColor(const wxColour& value) { codeCommentDocColor = value; }
        inline wxColour getCodeCommentDocColor() const { return codeCommentDocColor; }
        inline void setCodeCommentDocKeywordColor(const wxString& value) { codeCommentDocKeywordColor = string2color(value); }
        inline void setCodeCommentDocKeywordColor(const wxColour& value) { codeCommentDocKeywordColor = value; }
        inline wxColour getCodeCommentDocKeywordColor() const { return codeCommentDocKeywordColor; }
        inline void setCodeCommentDocKeywordErrorColor(const wxString& value) { codeCommentDocKeywordErrorColor = string2color(value); }
        inline void setCodeCommentDocKeywordErrorColor(const wxColour& value) { codeCommentDocKeywordErrorColor = value; }
        inline wxColour getCodeCommentDocKeywordErrorColor() const { return codeCommentDocKeywordErrorColor; }

        inline void setCodeOperatorBold(bool value) { codeOperatorBold = value; }
        inline bool getCodeOperatorBold() const { return codeOperatorBold; }
        inline void setCodeStringBold(bool value) { codeStringBold = value; }
        inline bool getCodeStringBold() const { return codeStringBold; }
        inline void setCodePreprocessorBold(bool value) { codePreprocessorBold = value; }
        inline bool getCodePreprocessorBold() const { return codePreprocessorBold; }
        inline void setCodeIdentifierBold(bool value) { codeIdentifierBold = value; }
        inline bool getCodeIdentifierBold() const { return codeIdentifierBold; }
        inline void setCodeNumberBold(bool value) { codeNumberBold = value; }
        inline bool getCodeNumberBold() const { return codeNumberBold; }
        inline void setCodeCharacterBold(bool value) { codeCharacterBold = value; }
        inline bool getCodeCharacterBold() const { return codeCharacterBold; }
        inline void setCodeWordBold(bool value) { codeWordBold = value; }
        inline bool getCodeWordBold() const { return codeWordBold; }
        inline void setCodeWord2Bold(bool value) { codeWord2Bold = value; }
        inline bool getCodeWord2Bold() const { return codeWord2Bold; }
        inline void setCodeCommentBold(bool value) { codeCommentBold = value; }
        inline bool getCodeCommentBold() const { return codeCommentBold; }
        inline void setCodeCommentLineBold(bool value) { codeCommentLineBold = value; }
        inline bool getCodeCommentLineBold() const { return codeCommentLineBold; }
        inline void setCodeCommentDocBold(bool value) { codeCommentDocBold = value; }
        inline bool getCodeCommentDocBold() const { return codeCommentDocBold; }
        inline void setCodeCommentDocKeywordBold(bool value) { codeCommentDocKeywordBold = value; }
        inline bool getCodeCommentDocKeywordBold() const { return codeCommentDocKeywordBold; }
        inline void setCodeCommentDocKeywordErrorBold(bool value) { codeCommentDocKeywordErrorBold = value; }
        inline bool getCodeCommentDocKeywordErrorBold() const { return codeCommentDocKeywordErrorBold; }

        inline void setCodeTabWidth(unsigned int value) { codeTabWidth = value; }
        inline unsigned int getCodeTabWidth() const { return codeTabWidth; }

        void addCodeKeyWord(wxString& keywords, const wxString& value);

        inline void setCodeKeywords0(const wxString& value) { codeKeywords0 = value; }
        void addCodeKeywords0(const wxString& value) { addCodeKeyWord(codeKeywords0, value); }
        inline wxString getCodeKeywords0() const { return codeKeywords0; }

        inline void setCodeKeywords1(const wxString& value) { codeKeywords1 = value; }
        void addCodeKeywords1(const wxString& value) { addCodeKeyWord(codeKeywords1, value); }
        inline wxString getCodeKeywords1() const { return codeKeywords1; }
};


//The BubbleXML class manages the language structure (blocks info, canvases info, etc.). It DOES NOT deals
//with the load/save of programs, only with the structural things:
class BubbleXML
{
    protected:
        Bubble *bubble;
        BubbleInstanceManager XMLVariables;
        BlocksHash blocksHash;

        //i18n/l10n:
        wxLocale& locale;
        wxString languageCode;

        //Images:
        ImagesHash imagesHash;

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
            blocksHash.clear();
        }

        //Utils:
        static wxString getXMLEncodedString(const wxString &value);

        //i18n/l10n:
        inline void setLanguageCode(const wxString& value) { languageCode = value; }
        inline const wxString &getLanguageCode() const { return languageCode; }

        //XML generic functions:
        bool sectionExists(const wxString &fileName, const wxString &section);
        wxImage getImage(const wxString &fileName);

        //Variables:
        bool isXMLVariable(const wxString& variableName) const;
        wxString getInternalVariableValue(const wxString& variableName, const wxString& fileName) const;
        wxString getVariableValue(const wxString& variableName, const wxString& fileName) const;
        bool loadVariablesFromXML(const wxString& fileName, bool clearVariableList);

        //Language/Blocks structure:
        //int getValidBlocksCount(const wxString& path) const; //##Not necessary by now.
        int getBlockFilesCount(const wxString& blocksPath, int flags);
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
        BubbleBlockInfo *loadBlockInfo(const wxString &fileName, const wxString &fullBlockFileName, wxWindow *pickersParent);
        int loadBlocksInfo(wxWindow *pickersParent, bool showPickers);
        const BubbleBlockInfo& getBlockInfo(const wxString& name, const wxString& function); //##Off for debug: const;
        bool blockIsValid(const wxString& name, const wxString& type) const;

        //Hardware:
        BubbleBoardProperties *loadBoardProperties(const wxString &fullBoardFileName);
        //wxString parseCmd(const wxString &cmd);
        const wxArrayString loadExternalCommands(const wxString &section, const wxString &fullBoardFileName);
        const wxArrayString loadInternalCommands(const wxString &section, const wxString &fullBoardFileName);
        int loadHardwareTargets(BubbleHardwareManager *hardwareManager);
        bool loadBoardInstancesFromXML(wxXmlNode *node, BubbleCanvasInfo *canvasInfo, BubbleBoardProperties *boardProperties);
        bool loadRelData(const wxString &relFileName, BubbleBoardProperties *boardProperties);
        int loadBoardRelations();
        bool loadSyntaxFromXML(wxXmlNode *node, BubbleBoardProperties *boardProperties);
        bool loadExamplesFromXML(wxXmlNode *node, BubbleBoardProperties *boardProperties);
        bool loadFileExtensions(wxXmlNode *node, BubbleBoardProperties *boardProperties);
        bool loadIncludePathsFromXML(wxXmlNode *node, BubbleBoardProperties *boardProperties, bool onlyBoard);
        bool loadIncludeFilesFromXML(wxXmlNode *node, BubbleBoardProperties *boardProperties, bool onlyBoard);
        bool loadDefinesFromXML(wxXmlNode *node, BubbleBoardProperties *boardProperties);
        bool loadInstancesFromXML(wxXmlNode *node, BubbleBoardProperties *boardProperties);
        bool loadInitBoardCodeFromXML(wxXmlNode *node, BubbleBoardProperties *boardProperties, bool onlyBoard);
        bool loadInitCodeFromXML(wxXmlNode *node, BubbleBoardProperties *boardProperties);
        bool loadFinalCodeFromXML(wxXmlNode *node, BubbleBoardProperties *boardProperties);

        //Canvas:
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

        FileEditorHash fileEditorHash;

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
        wxString includesCodeList;
        wxString includesBuildList;
        wxString initBoardCode;

        bool blocksEnabled;
        bool visibleLabels;
        bool visibleCanvas;

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
        wxString profile;

        //Current (or active) component:
        wxString tempPath;
        wxString componentPath;
        wxString componentFilesPath;
        wxString outputPath;

        bool simplifyCode;
        wxArrayString prevGeneratedCode;
        wxArrayString generatedCode;

        bool isSubstringInArrayString(const wxArrayString &value, const wxString& substring);

        void addLibrariesToCode();
        void addInitCode();
        void addFinalCode();
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
        bool findErrorStringAndShow(const wxArrayString &value);
        void showStream(const wxArrayString &value, const wxColour& colour = *wxGREEN);

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

        inline void setProfile(const wxString& value) { profile = value; }
        inline const wxString &getProfile() const { return profile; }

        inline void setTempPath(const wxString& value) { tempPath = value; }
        inline const wxString &getTempPath() const { return tempPath; }

        inline void setComponentPath(const wxString& value) { componentPath = value; }
        inline const wxString &getComponentPath() const { return componentPath; }

        inline void setComponentFilesPath(const wxString& value) { componentFilesPath = value; }
        inline const wxString &getComponentFilesPath() const { return componentFilesPath; }

        inline void setOutputPath(const wxString& value) { outputPath = value; }
        inline const wxString &getOutputPath() const { return outputPath; }

        bool addFile(const wxString& fullFileName, BubbleEditor *const editor);
        bool removeFile(const wxString& fullFileName);
        BubbleEditor *getFileEditor(const wxString& fullFileName);
        bool isFileAdded(const wxString& fullFileName) const;
        inline FileEditorHash *getFileEditorHash() { return &fileEditorHash; }

        //Communications:
        //##In the future there will be two port names: bootPortName and commPortName:
        inline void setBootPortName(const wxString& value) { bootPortName = value; }
        inline const wxString &getBootPortName() const { return bootPortName; }

        //Hardware:
        inline void setHardwareManager(BubbleHardwareManager *value) //##Hacer const el puntero del parámetro.
        {
            hardwareManager = value;
        }
        inline BubbleHardwareManager *getHardwareManager() const { return hardwareManager; }

        inline void setIncludesCodeList(const wxString &value) { includesCodeList = value; }
        inline wxString getIncludesCodeList() const { return includesCodeList; }
        inline void setIncludesBuildList(const wxString &value) { includesBuildList = value; }
        inline wxString getIncludesBuildList() const { return includesBuildList; }

        inline void setInitBoardCode(const wxString &value) { initBoardCode = value; }
        inline wxString getInitBoardCode() const { return initBoardCode; }

        wxString getOutputObjectsList(const wxString &fileExtension);
        bool setBoardName(const wxString& value, wxWindow *pickersParent);
        inline const wxString &getBoardName() const { return boardName; }
        int loadBoardRelations();

        //Board drivers:
        bool winInstallINF(); //##Testing.

        //Components:
        //##Ver bien si el Component será de Bubble o de Minibloq (por ahora se lo considera ligado a Bubble,
        //pues brinda generación y compilación genéricas de código -genéricas hasta cierto punto, claro-)...
        bool loadComponentFromFile(const wxString& name); //##In the future, it's possible that Bubble manages multiple components at once, and
        bool saveComponentToFile(const wxString& name, bool format = true);   //these functions will change...
        bool formatComponentFile(const wxString& name);

        void createDirs(const wxString& path);
        //##bool executeCmd(const wxString& cmd);
        bool run();
        bool verifyPortExistance();
        bool deploy();
        bool build();
        bool runInternalCommand(const wxString& cmd);
        bool resetBoard();

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

        inline void setVisibleCanvas(bool value) { visibleCanvas = value; }
        inline bool getVisibleCanvas() const  { return visibleCanvas; }
};
#endif
