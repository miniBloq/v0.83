#ifndef BubbleButton__h
#define BubbleButton__h

#include <wx/control.h>
#include <wx/bitmap.h>
#include <wx/image.h>


/** \Documentation:

MASKS/TRANSPARENCY: There are several ways to achive transparency in these buttons:

1. Just use a PNG image with transparent colour defined. The button will draw it right. This is easy, and let
you manage the transparency stuff from graphics editing sosftware:

    button = new BubbleButton(this,
                              ID_BUBBLEBUTTON,
                              wxPoint(0, 0),
                              wxSize(iconW, iconH));
    button->SetImageDefault(wxImage(_("C:/button.png")));
    button->SetImageHover(wxImage (_("C:/buttonHover.png")));
    button->SetImagePressed(wxImage(_("C:/buttonPressed.png")));


2. Use a non-transparency format, and define a mask colour for the image BEFORE asigning the image to the
button:

    button = new BubbleButton(this,
                              ID_BUBBLEBUTTON,
                              wxPoint(60, 10),
                              wxSize(iconW, iconH));
    wxImage img(_("C:/button.bmp"));
    img.SetMaskColour(255, 255, 255);
    button->SetImageDefault(img); //Just an example: you can always resize the image this way.

This let you use any image, and assign a different transparency colour for every image in the button.

3. Use a non-transparency format, and define the button general mask colour. This method is very useful if
you use similar images with the same transparency colour:

    wxImage img(_("C:/Projects/Multiplo/Soft/Minibloq-RG/v1.0/Soft/Bin/Minibloq-RG.v1.0/Components/Minibloq/v1.0/Minibloq.bmp"));
    button = new BubbleButton(this,
                              ID_BUBBLEBUTTON,
                              wxPoint(60, 10),
                              wxSize(iconW, iconH));
    button->SetImageDefault(img);
    button->SetMaskColour(wxColour(255, 0, 0));

If you will use white (255, 255, 255) as transparency colour, which is the default for the button, you can
just write:

    button->SetMask(true);

instead of

    button->SetMaskColour(wxColour(255, 255, 255));

Once called SetMaskColour() it's not necessary to call SetMask(true), because that call already activates
the mask.

**/

class BubbleButton : public wxControl
{
    protected:
        wxWindow* parent;

        wxImage *imageCurrent;
        wxImage imageDefault;
        wxImage imageHover;
        wxImage imagePressed;
        wxImage imageDisabled;
        bool imageDefaultAssigned;
        bool imageHoverAssigned;
        bool imagePressedAssigned;
        bool imageDisabledAssigned;
        wxColour maskColour;
        bool mask;
        bool refreshParent;

        wxString name;
        wxString function;
        wxString _returnDataType;
        wxColour backgroundColor;


        BubbleButton::BubbleButton(const BubbleButton &button)
        {
            //##Not public yet, so the compiler fails if someone makes an accidental copy.
        };
        void onMouseEnter(wxMouseEvent& event);
        void onMouseLeave(wxMouseEvent& event);
        void onLeftDown(wxMouseEvent& event);
        void onLeftUp(wxMouseEvent& event);
        void onPaint(wxPaintEvent& event);
        void onEraseBackground(wxEraseEvent & event);

    public:
        //BubbleButton(); //##Ver si lo dejo.
        //##Ver si se provee un constructor por defecto.
        BubbleButton(   wxWindow* parent,
                        wxWindowID id,
                        const wxPoint& pos = wxDefaultPosition,
                        const wxSize& size = wxDefaultSize,
                        //const wxValidator& validator,
                        const wxString& name = wxPanelNameStr,
                        const bool refreshParent = false);
        virtual ~BubbleButton();

        void setMaskColour(const wxColour& colour);
        const wxColour &getMaskColour() const;

        void setMask(const bool value);
        inline bool GetMask() const { return mask; }

        bool Enable(bool value = true);
        bool Disable();

        inline void setName(const wxString& value) { name = value; }
        inline const wxString &getName() const { return name; }

        inline void setFunction(const wxString& value) { function = value; }
        inline const wxString &getFunction() const { return function; }

        inline void setReturnDataType(const wxString& value) { _returnDataType = value; }
        inline const wxString &getReturnDataType() const { return _returnDataType; }

        inline void setBackgroundColor(const wxColour& value) { backgroundColor = value; }
        inline const wxColour &getBackgroundColor() const { return backgroundColor; }

        void setImageDefault(const wxImage& image);
        const wxImage &getImageDefault() const;

        void setImagePressed(const wxImage& image);
        const wxImage &getImagePressed() const;

        void setImageHover(const wxImage& image);
        const wxImage &getImageHover() const;

        void showImageDefault();
        bool isShowingImageDefault() const;

        void showImagePressed();
        bool isShowingImagePressed() const;

        void showImageHover();
        bool isShowingImageHover() const;

        void showImageDisabled();
        bool isShowingImageDisabled() const;

        //##
        virtual bool HasTransparentBackground() const
        {
            return true;
        }

        //##Not used by now:
        void setImageDisabled(const wxImage& image);
        const wxImage &getImageDisabled() const;

        DECLARE_EVENT_TABLE()
};
#endif
