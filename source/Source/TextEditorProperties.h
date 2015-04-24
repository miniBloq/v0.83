#ifndef TextEditorProperties__h
#define TextEditorProperties__h

#include "BubblePanel.h"

#include <wx/sizer.h>
#include <wx/bmpbuttn.h>

class Bubble;
class TextEditorProperties : public BubblePanel
{
    private:
        wxWindow* parent;
        Bubble *bubble;

        //AERobot's calibration screen:
        wxFlexGridSizer *sizerButtons;
        //BubblePanel *panelButtons;
        wxButton *buttonFwd;
        wxButton *buttonFwdClear;
        wxButton *buttonBack;
        wxButton *buttonBackClear;
        wxButton *buttonCW;
        wxButton *buttonCWClear;
        wxButton *buttonCCW;
        wxButton *buttonCCWClear;
        wxButton *buttonLine;
        wxButton *buttonLineClear;
        wxButton *buttonTest;
        wxButton *buttonClearAll;

    protected:
        void TextEditorProperties::flashFile(wxString fileName);

        void onButtonFwdClick(wxMouseEvent& event);
        void onButtonFwdClearClick(wxMouseEvent& event);
        void onButtonBackClick(wxMouseEvent& event);
        void onButtonBackClearClick(wxMouseEvent& event);
        void onButtonCWClick(wxMouseEvent& event);
        void onButtonCWClearClick(wxMouseEvent& event);
        void onButtonCCWClick(wxMouseEvent& event);
        void onButtonCCWClearClick(wxMouseEvent& event);
        void onButtonLineClick(wxMouseEvent& event);
        void onButtonLineClearClick(wxMouseEvent& event);
        void onButtonTestClick(wxMouseEvent& event);
        void onButtonClearAllClick(wxMouseEvent& event);

    public:
        TextEditorProperties(   wxWindow* parent,
                                wxWindowID id,
                                Bubble *const bubble,
                                const wxColour& colour,
                                const wxPoint& pos = wxDefaultPosition,
                                const wxSize& size = wxDefaultSize,
                                long style = wxTAB_TRAVERSAL,
                                const wxString& name = "AERobotCalibration"
                            );
        virtual ~TextEditorProperties();

        void updateGUI();
        //##void onSize(wxSizeEvent& event);

        //##DECLARE_EVENT_TABLE()
};
#endif
