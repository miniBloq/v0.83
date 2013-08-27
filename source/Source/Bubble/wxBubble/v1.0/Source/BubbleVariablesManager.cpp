#include "BubbleVariablesManager.h"

/*##Ver si se usará:
BEGIN_EVENT_TABLE(BubbleVariablesManager, BubblePanel)
    //EVT_SIZE(BubbleVariablesManager::onSize)
END_EVENT_TABLE()
*/

BubbleVariablesManager::BubbleVariablesManager( wxWindow* parent,
                                                wxWindowID id,
                                                const wxColour& colour,
                                                const wxPoint& pos,
                                                const wxSize& size,
                                                long style,
                                                const wxString& name) : BubblePanel(parent,
                                                                                    id,
                                                                                    colour,
                                                                                    true,
                                                                                    pos,
                                                                                    size,
                                                                                    style,
                                                                                    name)
{
    //Hide();

    //##SetBackgroundColourAndRefresh(colour);
}


BubbleVariablesManager::~BubbleVariablesManager()
{
}


/*##Ver si es necesario este evento...
void BubbleVariablesManager::onSize(wxSizeEvent& event)
{
    event.Skip();
}
*/
