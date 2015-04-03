#ifndef BubbleHardwareManager__h
#define BubbleHardwareManager__h

#include "BubblePanel.h"
#include "BubbleButton.h"
#include "BubbleCombo.h"

#include <string>
#include <vector>
#include "include/portscan.h"

#include <wx/combobox.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/hyperlink.h>

#include <wx/arrimpl.cpp> //This is a magic incantation which must be done!
class BubbleBoardProperties;
WX_DECLARE_OBJARRAY(BubbleBoardProperties, arrayOfBoardProperties);


class Bubble;
class BubbleBoardProperties;
class BubbleHardwareManager : public BubblePanel
{
    private:
        wxWindow* parent;
        Bubble *bubble;
        wxString boardName;
        BubbleBoardProperties *currentBoardProperties;
        wxStaticText *lblBootPortName;
        BubbleCombo *comboBootPortName;
        wxStaticText *lblBoardName;
        BubbleCombo *comboBoardName;
        wxButton *buttonReloadBlocks;
        wxButton *buttonReloadHardware;
        wxHyperlinkCtrl *lblURL0;
        wxHyperlinkCtrl *lblURL1;
        wxButton *buttonGoToDriversDir;
        BubbleButton *buttonMainImage;//##

        //##Horrible, but works nice!
        wxString emptyDummyString;

        arrayOfBoardProperties boardsProperties;

        std::vector<std::string> ports;
        wxString newPort;

        //##Add private copy constructor to avoid accidental copy?

        void onButtonReloadBlocksLeftUp(wxMouseEvent& event);
        void onButtonButtonReloadHardwareLeftUp(wxMouseEvent& event);
        void onButtonGoToDriversDirLeftUp(wxMouseEvent& event);
        void onKillFocus(wxFocusEvent& event);

    protected:
        void setPortType();

        void onSize(wxSizeEvent& event);
        void onUpdatePorts(wxCommandEvent &event);
        void onComboBootPortNameChanged(wxCommandEvent &event);
        void onComboBoardNameChanged(wxCommandEvent &event);

        void fit(const wxSize& size);

    public:
        BubbleHardwareManager(  wxWindow* parent,
                                wxWindowID id,
                                Bubble *const bubble,
                                const wxString& boardName,
                                const wxColour& colour,
                                const wxPoint& pos = wxDefaultPosition,
                                const wxSize& size = wxDefaultSize,
                                long style = wxTAB_TRAVERSAL,
                                const wxString& name = "BubbleHardwareManager"
                              );
        virtual ~BubbleHardwareManager();

        void selectFirstBoard();
        void clearBoardProperties()
        {
            boardsProperties.Clear();
            if (comboBoardName)
                comboBoardName->clear();
        }
        void addBoard(BubbleBoardProperties *boardProperties);
        inline BubbleBoardProperties *getCurrentBoardProperties() { return currentBoardProperties; };

        void updateGUI();
        void changeImage();

        static bool serialPortExists(const wxString& strPort);
        void updatePorts();
        bool getAvailablePorts(std::vector<std::string>& result);
        bool findNewPort();
        wxString getNewPort();
        //##void getPorts(std::vector<std::string>& result);

        void setAllEnabled(bool value);
        void showDeveloperButtons(bool value);

        void setPortSelectorEnabled(bool value);
        bool getPortSelectorEnabled();
        wxString getPortNameString();
        void setPortNameString(const wxString& value);

        void setBoardSelection(const wxString& value);
        wxString getBoardSelection();
        void setPortSelection(const wxString& value);
        wxString getPortSelection();

        void popUpPortList();
        void popUpBoardList();

        DECLARE_EVENT_TABLE()
};
#endif
