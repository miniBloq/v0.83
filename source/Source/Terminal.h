#ifndef Terminal__h
#define Terminal__h


#include "BubblePanel.h"
#include "Bubble.h"

#include <wx/stc/stc.h> //##I don't know why this is necessary, but without it there is an unresolved
                        //external error! And it must be exactly here (weird!).
#include "include/serport.h"
#include <wx/sizer.h>
#include <wx/thread.h>
#include <wx/panel.h>
#include <wx/splitter.h>
#include <wx/bmpbuttn.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>


enum CommMode
{
    commModeSerial = 0,
    commModeUsbHid,
    commModeUsbHid2
    //commModeTcp
    //commModeUdp
};


class TerminalTxEditor : public wxTextCtrl
{
    protected:
        DECLARE_EVENT_TABLE()

        CommMode mode;
        wxSerialPort *serialPort;
        wxColour rxColour, txColour;

        void OnChar(wxKeyEvent& event);

    public:
        TerminalTxEditor(   wxSerialPort *const serialPort,
                            wxWindow *parent,
                            wxWindowID id = wxID_ANY,
                            const wxPoint &pos = wxDefaultPosition,
                            const wxSize &size = wxDefaultSize,
                            long style = wxVSCROLL ):   wxTextCtrl( parent,
                                                                    id,
                                                                    wxString(""),
                                                                    pos,
                                                                    size,
                                                                    wxHSCROLL |
                                                                    wxTE_MULTILINE |
                                                                    wxTE_RICH2
                                                                  ),
                                                        mode(commModeSerial),
                                                        serialPort(serialPort),
                                                        rxColour(*wxGREEN),
                                                        txColour(*wxBLUE)
        {
        }
        virtual ~TerminalTxEditor() { }

        inline CommMode getMode() const { return mode; }
        inline void setMode(CommMode value) { mode =  value; }

        inline const wxColour& getRxColour() const { return rxColour; }
        inline void setRxColour(const wxColour& value) { rxColour = value; }
        inline const wxColour& getTxColour() const { return txColour; }
        inline void setTxColour(const wxColour& value) { txColour = value; }
};


class EmoticonScreen : public BubbleButton
{
    protected:
        wxImage imageEmoticonNothing;
        wxImage imageEmoticonSmile;
        wxImage imageEmoticonAngry;
        wxImage imageEmoticonWhatever;
        wxImage imageEmoticonArrowLeft;
        wxImage imageEmoticonArrowRight;
        wxImage imageEmoticonStop;

    public:
        EmoticonScreen( wxWindow* parent,
                        wxWindowID id) :    BubbleButton(   parent,
                                                            id
                                                        )
        {
        }
        virtual ~EmoticonScreen() { }

        void setEmoticonStr(const wxString& value);

        //Image nothing is to reset the emoticon screen:
        inline void setImageEmoticonNothing(const wxImage& image) { imageEmoticonNothing = image; }
        inline const wxImage &getImageEmoticonNothing() const{ return imageEmoticonNothing; }

        inline void setImageEmoticonSmile(const wxImage& image) { imageEmoticonSmile = image; }
        inline const wxImage &getImageEmoticonSmile() const{ return imageEmoticonSmile; }
        inline void setImageEmoticonAngry(const wxImage& image){ imageEmoticonAngry = image; }
        inline const wxImage &getImageEmoticonAngry() const { return imageEmoticonAngry; }
        inline void setImageEmoticonWhatever(const wxImage& image) { imageEmoticonWhatever = image; }
        inline const wxImage &getImageEmoticonWhatever() const { return imageEmoticonWhatever; }
        inline void setImageEmoticonArrowLeft(const wxImage& image) { imageEmoticonArrowLeft = image; }
        inline const wxImage &getImageEmoticonArrowLeft() const { return imageEmoticonArrowLeft; }
        inline void setImageEmoticonArrowRight(const wxImage& image) { imageEmoticonArrowRight = image; }
        inline const wxImage &getImageEmoticonArrowRight() const { return imageEmoticonArrowRight; }
        inline void setImageEmoticonStop(const wxImage& image) { imageEmoticonStop = image; }
        inline const wxImage &getImageEmoticonStop() const { return imageEmoticonStop; }
};


class TerminalRXThread : public wxThread
{
    private:
        CommMode mode;
        wxSerialPort *serialPort;
        wxTextCtrl *rxEditor;
        TerminalTxEditor *rxTxEditor;
        EmoticonScreen *emoticonScreen;

    public:
        //This will be a wxTHREAD_DETACHED thread:
        TerminalRXThread(CommMode mode,
                         wxSerialPort *const serialPort,
                         wxTextCtrl *const rxEditor,
                         TerminalTxEditor *const rxTxEditor,
                         EmoticonScreen*const emoticonScreen) : wxThread(wxTHREAD_JOINABLE),
                                                                mode(mode),
                                                                serialPort(serialPort),
                                                                rxEditor(rxEditor),
                                                                rxTxEditor(rxTxEditor),
                                                                emoticonScreen(emoticonScreen)
        {
        }
        virtual ~TerminalRXThread() { }

        inline CommMode getMode() const { return mode; }

        virtual ExitCode Entry();
        int readSerial(char *buffer, size_t sizeOfBuffer);
        int readUsbHid(char *buffer, size_t sizeOfBuffer);
};


//This class manages the underlying communications themselves.
//##In the future the Channel class may appear to manage TCP/IP, other USB, serial, etc. all with one
//unified interface (probably will be a SNAPI Channel).
//##NOTE: the functions belonging to the BubbleHardwareManager class, such as serialPortExists will not be
//here. This class manages the communications, while BubbleHardwareManager does tasks related to resource
//managment. Determining if a serial port does exist is it's task. More: TerminalCommManager class is not
//part of Bubble, but part of Minibloq.
class SplitTerminalGUI;
class SingleTerminalGUI;
class TerminalCommManager
{
    private:
        //##Add private copy constructor to avoid accidental copy?

    protected:
        wxString portName;
        CommMode mode;
        wxSerialPort_DCS serialPortDCS; //##For future use, when the port will be configurable by the user.

        wxBaud baudRate;

        int usbVid;
        int usbPidApp;
        int usbPidBoot;
        int usbDeviceNumber;

        wxSerialPort *serialPort;

        TerminalRXThread *rxThread;

        SplitTerminalGUI *splitTerminal;
        SingleTerminalGUI *singleTerminal;

        void setMode(CommMode value); //##By now, it's better to do not user this ouside the setPortName method.

    public:
        TerminalCommManager();
        virtual ~TerminalCommManager();

        //Generic communication functions:
        inline const wxString &getPortName() const { return portName; }
        void setPortName(const wxString& nameValue, int usbVidValue, int usbPidValue, int usbPidAppValue);
        inline CommMode getMode() const { return mode; }
        bool open();
        void close();

        //Specific functions for communication systems that support baudrate configuration:
        inline int getBaudRate() const { return baudRate; }
        void setBaudRate(wxBaud value);

        //Specific functions for serial communications:
        inline wxSerialPort *getSerialPort() const { return serialPort; }

        //Specific functions for USB non-CDC emulated communications:
        //##Right now only support devices with HID serial emulation, but may support other systems in the
        //future (such as DFU, etc.):
        inline int getUsbVid() const { return usbVid; }
        void setUsbVid(int value);
        inline int getUsbPidBoot() const { return usbPidBoot; }
        void setUsbPidBoot(int value);
        inline int getUsbPidApp() const { return usbPidApp; }
        void setUsbPidApp(int value);
        inline int getUsbDeviceNumber() const { return usbDeviceNumber; }
        void setUsbDeviceNumber(int value);
        bool openUsbHid();
        bool openUsbHid2();

        //Specific functions for Serial communications:
        bool openSerial();

        //GUI functions:
        inline SplitTerminalGUI *getSplitTerminal() const { return splitTerminal; }
        inline void setSplitTerminal(SplitTerminalGUI *const terminal) { splitTerminal = terminal; }
        inline SingleTerminalGUI *getSingleTerminal() const { return singleTerminal; }
        inline void setSingleTerminal(SingleTerminalGUI *const terminal) { singleTerminal = terminal; }
        void enableTerminalsButtons(bool openEnable, bool closeEnable);
};


class BaseTerminalGUI : public BubblePanel
{
    protected:
        enum
        {
            ID_CheckShowEmoticons = wxID_HIGHEST+1
        };

        DECLARE_EVENT_TABLE()

        //##Add private copy constructor to avoid accidental copy?
        Bubble *bubble;
        TerminalCommManager *commManager;
        bool showConfiguration;

        wxFlexGridSizer *sizerMain;
        wxFlexGridSizer *sizerEditors;
        wxFlexGridSizer *sizerRx;
        wxFlexGridSizer *sizerButtons;
        BubblePanel *panelEditors;
        BubblePanel *panelButtons;
        BubblePanel *panelPortStatus;
        //wxBitmapButton *buttonOpenPort; //##Near future: use wxBitmapButtons!
        //wxBitmapButton *buttonClosePort;
        //wxBitmapButton *buttonClearAll;
        wxButton *buttonOpenPort; //##Near future: use wxBitmapButtons!
        wxButton *buttonClosePort;
        wxButton *buttonClearAll;
        wxCheckBox *checkShowEmoticons;

        wxColour rxColour, txColour;

        void onButtonOpenPort(wxCommandEvent& event);
        void onButtonClosePort(wxCommandEvent& event);
        void onButtonClearAll(wxCommandEvent& event);

    public:
        BaseTerminalGUI(wxWindow* parent,
                        wxWindowID id,
                        Bubble *bubble,
                        TerminalCommManager *commManager,
                        bool showConfiguration, //##Horrible: This will change in the near future
                                                //once the configuration window exists.
                        const wxColour& colour,
                        const wxPoint& pos = wxDefaultPosition,
                        const wxSize& size = wxDefaultSize,
                        long style = wxTAB_TRAVERSAL,
                        const wxString& name = "SingleTerminalGUI"
                       );
        virtual ~BaseTerminalGUI();


        inline TerminalCommManager *getCommManager() const { return commManager; }

        virtual void updateGUI();
        virtual void clear() = 0;

        inline const wxColour& getRxColour() const { return rxColour; }
        virtual void setRxColour(const wxColour& value) = 0;
        inline const wxColour& getTxColour() const { return txColour; }
        virtual void setTxColour(const wxColour& value) = 0;

        virtual void enableTerminalsButtons(bool openEnable, bool closeEnable);
        virtual void OnCheckShowEmoticonsClick(wxCommandEvent &event) { };
};


class SingleTerminalGUI : public BaseTerminalGUI
{
    private:
        //##Add private copy constructor to avoid accidental copy?
        SplitTerminalGUI *brotherTerminal;
        TerminalTxEditor *rxTxEditor;

    public:
        SingleTerminalGUI(  wxWindow* parent,
                            wxWindowID id,
                            Bubble *bubble,
                            TerminalCommManager *commManager,
                            const wxColour& colour,
                            const wxPoint& pos = wxDefaultPosition,
                            const wxSize& size = wxDefaultSize,
                            long style = wxTAB_TRAVERSAL,
                            const wxString& name = "SingleTerminalGUI"
                         );
        virtual ~SingleTerminalGUI();

        virtual void clear();

        inline TerminalTxEditor *getRxTxEditor() const { return rxTxEditor; }
        virtual void setRxColour(const wxColour& value);
        virtual void setTxColour(const wxColour& value);

        inline SplitTerminalGUI *getBrotherTerminal() const { return brotherTerminal; }
        inline void setBrotherEditor(SplitTerminalGUI *const terminal) { brotherTerminal = terminal; }
};


class SplitTerminalGUI : public BaseTerminalGUI
{
    private:
        //##Add private copy constructor to avoid accidental copy?
        SingleTerminalGUI *brotherTerminal;
        BubblePanel *rxPanel;
        EmoticonScreen *rxEmoticon;
        wxTextCtrl *rxEditor;
        TerminalTxEditor *txEditor;
        wxSplitterWindow *splitRxTx;

        virtual void OnCheckShowEmoticonsClick(wxCommandEvent &event);

    protected:
        virtual void enableEmoticons(bool value);


    public:
        SplitTerminalGUI(   wxWindow* parent,
                            wxWindowID id,
                            Bubble *bubble,
                            TerminalCommManager *commManager,
                            const wxColour& colour,
                            const wxPoint& pos = wxDefaultPosition,
                            const wxSize& size = wxDefaultSize,
                            long style = wxTAB_TRAVERSAL,
                            const wxString& name = "SplitTerminalGUI"
                         );
        virtual ~SplitTerminalGUI();

        virtual void clear();

        inline wxTextCtrl *getRxEditor() const { return rxEditor; }
        inline wxTextCtrl *getTxEditor() const { return txEditor; }
        inline EmoticonScreen *getRxEmoticon() const { return rxEmoticon; }
        virtual void setRxColour(const wxColour& value);
        virtual void setTxColour(const wxColour& value);

        inline SingleTerminalGUI *getBrotherTerminal() const { return brotherTerminal; }
        inline void setBrotherEditor(SingleTerminalGUI *const terminal) { brotherTerminal = terminal; }

        virtual bool getEmoticonsEnabled() const;
        virtual void setEmoticonsEnabled(bool value);
};
#endif
