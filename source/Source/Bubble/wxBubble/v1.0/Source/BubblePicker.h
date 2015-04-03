#ifndef BubblePicker__h
#define BubblePicker__h

#include <wx/dialog.h>
#include <wx/sizer.h>
#include "BubbleButton.h"
#include "Bubble.h"


//class BubblePickerPanel : public BubblePanel
//{
//    protected:
//
//
//    public:
//        BubblePickerPanel
//};


class BubbleCanvas;
class BubblePicker : public wxDialog
{
    protected:
        static const wxWindowID ID_Buttons;

        wxWindow* parent;
        Bubble *bubble;

        wxString dataType;

        int buttonsCount;
        int buttonsWidth, buttonsHeight, buttonsHGap, buttonsVGap;
        //bool activated;

        wxFlexGridSizer *sizerMain;
        //BubbleButton *buttonMethod;//##

        bool buttonPressed;
        bool closeAfterPick;

        unsigned int cols;

        //##Ver si se agrega constructor de copia privado para evitar la copia accidental.

        virtual void onLeftUp(wxMouseEvent& event);
        virtual void onLeftDoubleClick(wxMouseEvent& event);
        virtual void onLeftDown(wxMouseEvent& event);
        virtual void onLeaveWindow(wxMouseEvent& event);
        virtual void onKillFocus(wxFocusEvent& event); //##
        virtual void onActivate(wxActivateEvent& event); //##
        virtual void onClose(wxCloseEvent &event);
        //void onMouseLeave(wxMouseEvent& event);

        virtual BubblePanel *getFunctionPanel(const wxString& function);

    public:
        BubblePicker(wxWindow* parent,
                     Bubble *const bubble,
                     wxWindowID id,
                     const wxString& dataType,
                     const wxString& caption,
                     const unsigned int buttonsSide,
                     const wxColour& colour,
                     const unsigned int cols);
        virtual ~BubblePicker();

        virtual inline void setDataType(const wxString& value) { dataType = value; }
        virtual inline const wxString &getDataType() const { return dataType; }

        //##Por si hubiera que realizar alguna otra acción al mostrar el picker:
        virtual inline void setCloseAfterPick(const bool value) { closeAfterPick = value; }
        virtual inline bool getCloseAfterPick() const { return closeAfterPick; }

        virtual void addButton( const wxString& name,
                                const wxString& function,
                                const wxString& returnType,
                                const wxString& tooltip,
                                const wxColour& backgroundColor,
                                const wxImage& imgDefault,
                                const wxImage& imgPressed,
                                const wxImage& imgHover,
                                const wxImage& imgDisabled);
        virtual void enableAllExceptTheseButtons(const wxSortedArrayString& buttonNames, bool value);

        virtual void SetBackgroundColourAndRefresh(const wxColour& colour);

        DECLARE_EVENT_TABLE()
};


class BubbleActionPicker : public BubblePicker
{
    private:
        void onLeftUp(wxMouseEvent& event);
        void onLeftDown(wxMouseEvent& event);
        void onLeftDoubleClick(wxMouseEvent& event);

    public:
        BubbleActionPicker( wxWindow* parent,
                            Bubble *const bubble,
                            wxWindowID id,
                            const wxString& dataType,
                            const wxString& caption,
                            const unsigned int buttonsSide,
                            const wxColour& colour,
                            const unsigned int cols): BubblePicker( parent,
                                                                    bubble,
                                                                    id,
                                                                    dataType,
                                                                    caption,
                                                                    buttonsSide,
                                                                    colour,
                                                                    cols)

        {
        };
        virtual ~BubbleActionPicker() { };
};


class BubbleExpressionPicker : public BubblePicker
{
    private:
        BubbleParam *paramSlot;

        //void onLeftUp(wxMouseEvent& event);
        void onLeftDown(wxMouseEvent& event);

    public:
        BubbleExpressionPicker( wxWindow* parent,
                                Bubble *const bubble,
                                wxWindowID id,
                                const wxString& dataType,
                                const wxString& caption,
                                const unsigned int buttonsSide,
                                const wxColour& colour,
                                const unsigned int cols):   BubblePicker(   parent,
                                                                            bubble,
                                                                            id,
                                                                            dataType,
                                                                            caption,
                                                                            buttonsSide,
                                                                            colour,
                                                                            cols),
                                                            paramSlot(NULL)

        {
        };
        virtual ~BubbleExpressionPicker() { };

        inline virtual void setParamSlot(BubbleParam *value) { paramSlot = value; }
        inline virtual BubbleParam *getParamSlot() { return paramSlot; }
};

#endif
