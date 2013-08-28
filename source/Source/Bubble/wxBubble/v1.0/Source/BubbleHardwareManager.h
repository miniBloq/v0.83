#ifndef BubbleHardwareManager__h
#define BubbleHardwareManager__h

#include "BubblePanel.h"
#include "BubbleButton.h"
#include "BubbleCombo.h"

#include <wx/combobox.h>
#include <wx/stattext.h>
#include <wx/button.h>


#include <wx/arrimpl.cpp> //This is a magic incantation which must be done!
class BubbleBoardProperties;
WX_DECLARE_OBJARRAY(BubbleBoardProperties, arrayOfBoardProperties);


class Bubble; //##horrible...
class BubbleBoardProperties;
class BubbleHardwareManager : public BubblePanel
{
    private:
        wxWindow* parent;
        Bubble *bubble;
        BubbleBoardProperties *currentBoardProperties;
        wxStaticText *lblBootPortName;
        BubbleCombo *comboBootPortName;
        wxStaticText *lblBoardName;
        BubbleCombo *comboBoardName;
        wxButton *buttonReloadBlocks;
        BubbleButton *buttonMainImage;//##

        //##Horrible, but works nice!
        wxString emptyDummyString;

        arrayOfBoardProperties boardsProperties;

        //##Add private copy constructor to avoid accidental copy?

        void onButtonReloadBlocksLeftUp(wxMouseEvent& event);
        void onKillFocus(wxFocusEvent& event);

    protected:
        void onSize(wxSizeEvent& event);
        void onUpdatePorts(wxCommandEvent &event);
        void onComboBootPortNameChanged(wxCommandEvent &event);
        void onComboBoardNameChanged(wxCommandEvent &event);

        void fit(const wxSize& size);

    public:
        BubbleHardwareManager(  wxWindow* parent,
                                wxWindowID id,
                                Bubble *const bubble,
                                const wxColour& colour,
                                const wxPoint& pos = wxDefaultPosition,
                                const wxSize& size = wxDefaultSize,
                                long style = wxTAB_TRAVERSAL,
                                const wxString& name = "BubbleHardwareManager"
                              );
        virtual ~BubbleHardwareManager();

        void clearBoardProperties()
        {
            boardsProperties.Clear();
        }
        void addBoard(BubbleBoardProperties *boardProperties);

        void updateGUI();
        void changeImage();

        static bool serialPortExists(const wxString& strPort);
        void updatePorts();

        void setPortSelectorEnabled(bool value);
        bool getPortSelectorEnabled();
        wxString getPortNameString();
        void setPortNameString(const wxString& value);

        void popUpPortList();
        void popUpBoardList();

        DECLARE_EVENT_TABLE()
};
#endif
