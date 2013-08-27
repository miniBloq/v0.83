//##Agregar licencia y avisos de copyright de terceros en TODOS LOS headers y cpp.

#include "BubbleButton.h"

#include <wx/dcclient.h>
#include <wx/validate.h>


BEGIN_EVENT_TABLE(BubbleButton, wxControl)
    EVT_PAINT(BubbleButton::onPaint)
    EVT_ERASE_BACKGROUND(BubbleButton::onEraseBackground)
    EVT_ENTER_WINDOW(BubbleButton::onMouseEnter)
    EVT_LEAVE_WINDOW(BubbleButton::onMouseLeave)
    EVT_LEFT_DOWN(BubbleButton::onLeftDown)
    EVT_LEFT_UP(BubbleButton::onLeftUp)
END_EVENT_TABLE()


/*
BubbleButton::BubbleButton()
{
    //##¿Dejo esto?
}
*/

BubbleButton::BubbleButton( wxWindow* parent,
                            wxWindowID id,
                            const wxPoint& pos,
                            const wxSize& size,
                            //const wxValidator& validator = wxDefaultValidator,
                            const wxString& name,
                            const bool refreshParent) : wxControl(  parent,
                                                                    id,
                                                                    pos,
                                                                    size,
                                                                    wxNO_BORDER|wxTRANSPARENT_WINDOW,
                                                                    wxDefaultValidator, //##
                                                                    name),
                                                        parent(parent),
                                                        imageCurrent(NULL),
                                                        imageDefaultAssigned(false),
                                                        imageHoverAssigned(false),
                                                        imagePressedAssigned(false),
                                                        imageDisabledAssigned(false),
                                                        maskColour(255, 255, 255),
                                                        mask(false),
                                                        refreshParent(refreshParent),
                                                        name(wxString("")),
                                                        function(wxString("")),
                                                        _returnDataType(wxString("")),
                                                        backgroundColor(wxColour(255, 255, 255))
{
}


BubbleButton::~BubbleButton()
{
    //##Forgetting something?
}


void BubbleButton::setImageDefault(const wxImage& image)
{
    //##Verificar que esto esté correcto, por el reference counting y la asignación desde un
    //objeto constante:
    imageDefault = image;
    imageCurrent = &imageDefault;
    if (mask)
        imageDefault.SetMaskColour(maskColour.Red(), maskColour.Green(), maskColour.Blue());
    imageDefaultAssigned = true;
}


const wxImage &BubbleButton::getImageDefault() const
{
    return imageDefault;
}


void BubbleButton::setImagePressed(const wxImage& image)
{
    imagePressed = image;
    if (mask)
        imagePressed.SetMaskColour(maskColour.Red(), maskColour.Green(), maskColour.Blue());
    imagePressedAssigned = true;
}


const wxImage &BubbleButton::getImagePressed() const
{
    return imagePressed;
}


void BubbleButton::setImageHover(const wxImage& image)
{
    imageHover = image;
    if (mask)
        imageHover.SetMaskColour(maskColour.Red(), maskColour.Green(), maskColour.Blue());
    imageHoverAssigned = true;
}


const wxImage &BubbleButton::getImageHover() const
{
    return imageHover;
}


void BubbleButton::setImageDisabled(const wxImage& image)
{
    imageDisabled = image;
    if (mask)
        imageDisabled.SetMaskColour(maskColour.Red(), maskColour.Green(), maskColour.Blue());
    imageDisabledAssigned = true;
}


const wxImage &BubbleButton::getImageDisabled() const
{
    return imageDisabled;
}


void BubbleButton::showImageDefault()
{
    if (imageDefaultAssigned)
    {
        imageCurrent = &imageDefault;
        Refresh();
    }
}


bool BubbleButton::isShowingImageDefault() const
{
    if (imageDefaultAssigned)
        return (imageCurrent == &imageDefault);
    return false;
}


void BubbleButton::showImagePressed()
{
    if (imagePressedAssigned)
    {
        imageCurrent = &imagePressed;
        Refresh();
    }
}


bool BubbleButton::isShowingImagePressed() const
{
    if (imagePressedAssigned)
        return (imageCurrent == &imagePressed);
    return false;
}


void BubbleButton::showImageHover()
{
    if (imageHoverAssigned)
    {
        imageCurrent = &imageHover;
        Refresh();
    }
}


bool BubbleButton::isShowingImageHover() const
{
    if (imageHoverAssigned)
        return (imageCurrent == &imageHover);
    return false;
}


void BubbleButton::showImageDisabled()
{
    if (imageDisabledAssigned)
    {
        imageCurrent = &imageDisabled;
        Refresh();
    }
}


bool BubbleButton::isShowingImageDisabled() const
{
    if (imageDisabledAssigned)
        return (imageCurrent == &imageDisabled);
    return false;
}


void BubbleButton::onMouseEnter(wxMouseEvent& event)
{
    showImageHover();
    event.Skip();
}


void BubbleButton::onMouseLeave(wxMouseEvent& event)
{
    if (imageHoverAssigned) //Only restores to the defaultImage if there is a hoverImage.
    {
        if (imageDefaultAssigned)
        {
            if (IsEnabled())
            {
                if (imageDefaultAssigned)
                    imageCurrent = &imageDefault;
            }
            else
            {
                if (imageDisabledAssigned)
                    imageCurrent = &imageDisabled;
            }

            //##Esto podría ser agregado en todos los cambios de imagen, pero genera flickering, y la verdad
            //es que en Minibloq, el único lugar necesario, debido al tipo de imágenes que se usan en los
            //botones de los parámetros (que tienen un gran área transparente, por lo que si se pasa con el
            //mouse por arriba, al salir no se redibuja bien porque la nueva -el "triangulito" se dibuja
            //con fondo transparente sobre la imagen anterior, no viéndose el efecto del mouseLeave-):
            if (refreshParent)
            {
                if (parent)
                {
                    //##Ver si se puede hacer funcionar el SetClippingRegion: Quizá pasándo el clippingRegion
                    //al parent, y agregando a los BubbleBlocks este código en su evento paint, tomando
                    //el clippingRegion pasado como parámetro con algún método nuevo:
//                    wxClientDC parentDC(parent);
//                    parentDC.DestroyClippingRegion();
//                    parentDC.SetClippingRegion(GetPosition(), GetSize());
//                    parent->Refresh();
//                    parentDC.DestroyClippingRegion();

                    parent->Refresh();

//                    //##20110321:
//                    Refresh();
                }
            }
            else
            {
                Refresh();
            }
        }
    }
    event.Skip();
}


void BubbleButton::onLeftDown(wxMouseEvent& event)
{
    if (IsEnabled())
    {
        showImagePressed();
    }
    else
    {
        if (imageDisabledAssigned)
        {
            imageCurrent = &imageDisabled;
            //Update();
            Refresh();
        }
    }
    event.Skip();
}


void BubbleButton::onLeftUp(wxMouseEvent& event)
{
    //##Creo que esto se puede mejorar para reducir el flickering, pero no estoy seguro por ahora:
    if (IsEnabled())
    {
        if (imagePressedAssigned)
        {
            if (imageHoverAssigned)
            {
                imageCurrent = &imageHover;
            }
            else
            {
                if (imageDefaultAssigned)
                    imageCurrent = &imageDefault;
            }
        }
    }
    else
    {
        if (imageDisabledAssigned)
            imageCurrent = &imageDisabled;
    }
    //Update();
    Refresh();
    event.Skip();
}


void BubbleButton::setMaskColour(const wxColour& colour)
{
    maskColour = colour;

    imageDefault.SetMaskColour(maskColour.Red(), maskColour.Green(), maskColour.Blue());
    imageHover.SetMaskColour(maskColour.Red(), maskColour.Green(), maskColour.Blue());
    imagePressed.SetMaskColour(maskColour.Red(), maskColour.Green(), maskColour.Blue());
}


const wxColour &BubbleButton::getMaskColour() const
{
    return maskColour;
}


void BubbleButton::setMask(const bool value)
{
    mask = value;
    if (mask)
    {
        imageDefault.SetMaskColour(maskColour.Red(), maskColour.Green(), maskColour.Blue());
        imageHover.SetMaskColour(maskColour.Red(), maskColour.Green(), maskColour.Blue());
        imagePressed.SetMaskColour(maskColour.Red(), maskColour.Green(), maskColour.Blue());
    }
    else
    {
        imageDefault.SetMask(false);
        imageHover.SetMask(false);
        imagePressed.SetMask(false);
    }
}


bool BubbleButton::Enable(bool value)
{
    if (value)
    {
        if (imageDefaultAssigned)
        {
            imageCurrent = &imageDefault;
            //Update();
            Refresh();
        }
    }
    else
    {
        if (imageDisabledAssigned)
        {
            imageCurrent = &imageDisabled;
            //Update();
            Refresh();
        }
    }
    return wxControl::Enable(value);
}


bool BubbleButton::Disable()
{
    return Enable(false);
}


void BubbleButton::onPaint(wxPaintEvent& event)
{
//    //##20110321:
//    if (parent == NULL)
//        return;

    //##Ver si esto no tiene que integrarse con el sistema estándar de visibilidad del wxWindow del que hereda:
    if (IsShown())
    {
        if (imageCurrent)
        {
//            wxClientDC parentDC(parent); //##20110321
            wxPaintDC dc(this); //This must be created here, inside the onPaint event, and on the stack.
            //dc.SetBackgroundMode(wxTRANSPARENT);//##No parece hacer gran cosa...
            //dc.Clear();
            wxSize size = GetClientSize();

#if defined (linux)
            SetBackgroundColour( getBackgroundColor() );
#endif

//            //##20110321:
//            wxPoint pos = GetPosition();
//            dc.Blit(0,
//                    0,
//                    size.GetWidth(),
//                    size.GetHeight(),
//                    &parentDC,
//                    pos.x,
//                    pos.y
////                    int logicalFunc = wxCOPY,
////                    bool useMask = false,
////                    wxCoord xsrcMask = -1,
////                    wxCoord ysrcMask = -1
//                   );

            wxBitmap bmp(imageCurrent->Scale(size.GetWidth(), size.GetHeight()));
            dc.DrawBitmap(bmp, 0, 0, true);
        }
    }
    //This is necessary in order to avoid event problems:
    //event.Skip();
}


void BubbleButton::onEraseBackground(wxEraseEvent & event)
{
    //Do nothing, to prevent background erasing...
    //event.Skip();
}
