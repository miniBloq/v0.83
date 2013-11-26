#ifndef BubbleCombo__h
#define BubbleCombo__h

#include "BubbleButton.h"

#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/menu.h>


class BubbleCombo : public wxPanel
{
    protected:
        wxWindow* parent;

        //wxFlexGridSizer *sizerMain;
        BubbleButton *buttonSelect;
        wxStaticText *text;
        wxMenu *popList;

        unsigned int hGap;
        bool visible;
        bool sorted;
        bool autoSize;
        wxColour    defaultMouseOverColour,
                    defaultErrorColour;

        BubbleCombo::BubbleCombo(const BubbleCombo &button)
        {
            //##Not public yet, so the compiler fails if someone makes an accidental copy.
        };

        void onMouseEnter(wxMouseEvent& event);
        void onMouseLeave(wxMouseEvent& event);
        void onLeftDown(wxMouseEvent& event);
        //void onLeftUp(wxMouseEvent& event);
        void onSize(wxSizeEvent& event);

        void onPopUpList(wxMouseEvent& event);
        void onSelectOption(wxCommandEvent& event);

    public:
        //BubbleButton(); //##Ver si lo dejo.
        //##Ver si se provee un constructor por defecto.
        BubbleCombo(wxWindow* parent,
                    wxWindowID id,
                    const wxImage &imageDefault,
                    const wxImage &imagePressed,
                    const wxImage &imageHover,
                    const wxImage &imageDisabled,
                    bool autoSize,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize,
                    long style = wxTAB_TRAVERSAL | wxNO_BORDER | wxTRANSPARENT_WINDOW,
                    //const wxValidator& validator,
                    const wxString& name = wxPanelNameStr
                   );
        virtual ~BubbleCombo();

        void setVisible(const bool value);
        inline bool getVisible() const { return visible; }

        inline void setSorted(const bool value) { sorted = value; }
        inline bool getSorted() const { return sorted; }

        bool Enable(bool value = true);
        bool Disable();

        wxString getText() const;
        void setText(const wxString& value);

        void append(const wxString& itemCaption, wxImage *icon = NULL, int iconW = 0, int iconH = 0);
        void clear();
        unsigned int getCount() const;
        int getSelection() const;
        bool setSelection(unsigned int index);
        bool setSelection(const wxString &value);
        bool popUpList();
        bool textExists(const wxString &value);

        wxFont getFont();
        void setFont(const wxFont& font);

        inline void setAutoSize(bool value) { autoSize = value; }
        inline bool getAutoSize() const { return autoSize; }
        //void setSize(wxSize size); //##

        void setBackgroundColour(const wxColour& colour);

        inline void setDefaultMouseOverColour(const wxColour& colour) { defaultMouseOverColour = colour; }
        inline wxColour getDefaultMouseOverColour() const { return defaultMouseOverColour; }

        inline void setDefaultErrorColour(const wxColour& colour) { defaultErrorColour = colour; }
        inline wxColour getDefaultErrorColour() const { return defaultErrorColour; }

        DECLARE_EVENT_TABLE()
};
#endif
