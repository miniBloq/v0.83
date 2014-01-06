#include "BubbleCanvas.h"
#include "Bubble.h"

#include <wx/gdicmn.h>
#include <wx/msgdlg.h> //##Debug, se va en release. Agregar compilación condicional.

WX_DEFINE_OBJARRAY(arrayOfParams);


BubbleParam::BubbleParam(   wxWindow* parent,
                            wxWindowID id,
                            const wxString& dataType,
                            const wxPoint& pos,
                            const wxSize& size,
                            const wxString& name):  BubbleButton(parent,
                                                                 id,
                                                                 pos,
                                                                 size,
                                                                 name,
                                                                 true),
                                                    dataType(dataType),
                                                    firstBlock(NULL)
{
    Connect(    wxEVT_LEFT_DOWN,
                wxMouseEventHandler(BubbleParam::onParamLeftDown),
                NULL,
                this
           );
}


BubbleParam::~BubbleParam()
{
}


void BubbleParam::onParamLeftDown(wxMouseEvent& event)
{
//    wxMessageDialog dialog0(parent, _("Debug:"), _("Debug:")); //##Debug.
//    dialog0.ShowModal();

    if (parent)
    {
        BubbleExpressionPicker *picker = NULL;
        if ( ((BubbleBlock*)parent)->getCanvas() )
        {
            if ( (((BubbleBlock*)parent)->getCanvas())->getBubble() )
            {
                Bubble *bubble = (((BubbleBlock*)parent)->getCanvas())->getBubble();
                if (bubble)
                {
                    picker = bubble->getExpressionPicker(dataType);
                    bubble->closeTemporalPickers();
                }
            }
            //(((BubbleBlock*)parent)->getCanvas())->setCurrentBlock((BubbleBlock*)parent);
        }

        if (picker)
        {
            //picker->ShowModal();
            picker->setParamSlot(this);

            //Prevents the picker to appear outside the visible screen area:
            int x = 0, y = 0;
            int screenOrigX = 0, screenOrigY = 0;
            int screenWidth = 0, screenHeight = 0;
            int pickerWidth = picker->GetSize().GetWidth();
            int pickerHeight = picker->GetSize().GetHeight();
            int xOffset = 0, yOffset = 0;
            int xPos = 0, yPos = 0;

            wxClientDisplayRect(&screenOrigX, &screenOrigY, &screenWidth, &screenHeight);

            //Obtains the paramSlot's screen position:
            GetScreenPosition(&x, &y);

            //Shows the picker in it's proper place:
            x = x + GetSize().GetWidth();
            if ( (x + pickerWidth) > screenWidth )
            {
                //The picker goes outside the right margin:
                xOffset = screenWidth - (x + pickerWidth);
                yOffset = GetSize().GetHeight();
            }
            //screenHeight*0.x is the security margin, because in some systems wxClientDisplayRect() does
            //not properly detect the OS taskbar:
            if ( (y + pickerHeight) > screenHeight )
            {
                //The picker goes outside the bottom margin too, so it's showed OVER the paramslot, no
                //matter the previous calcs for the yOffset:
                yOffset = -pickerHeight;
            }
            if ( (y + yOffset) > 0 )
                yPos = y + yOffset;
            xPos = x + xOffset;
//##See if the correction in x becomes necessary:
//            if (xPos < 0)
//                xPos = 0;
            picker->Move(xPos, yPos);
            picker->Show();
        }
    }
    event.Skip();
}


void BubbleParam::setImageAssigned(const wxImage& image)
{
    //##Verificar que esto esté correcto, por el reference counting y la asignación desde un
    //objeto constante:
    imageAssigned = image;
}


const wxImage &BubbleParam::getImageAssigned() const
{
    return imageAssigned;
}


void BubbleParam::setImageNotAssigned(const wxImage& image)
{
    //##Verificar que esto esté correcto, por el reference counting y la asignación desde un
    //objeto constante:
    imageNotAssigned = image;
}


const wxImage &BubbleParam::getImageNotAssigned() const
{
    return imageNotAssigned;
}
