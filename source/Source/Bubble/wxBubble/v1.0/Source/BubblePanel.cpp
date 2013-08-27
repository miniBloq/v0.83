#include "BubblePanel.h"

BubblePanel::BubblePanel(   wxWindow* parent,
                            wxWindowID id,
                            const wxColour& colour,
                            bool visible,
                            const wxPoint& pos,
                            const wxSize& size,
                            long style,
                            const wxString& name):  wxPanel(parent,
                                                            id,
                                                            pos,
                                                            size,
                                                            style,
                                                            name
                                                            )
{
    if (visible)
    {
        //SetBackgroundColour(colour);
        SetBackgroundColourAndRefresh(colour);
    }
    else
    {
        Hide();
    }
}


BubblePanel::~BubblePanel()
{
}


void BubblePanel::SetBackgroundColourAndRefresh(const wxColour& colour)
{
    SetBackgroundColour(colour);
    //##backgroundColour = colour;

    //This is to do the repainting inmediatly:
    Refresh();
    Update();
}
