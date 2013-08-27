//##Agregar licencia y avisos de copyright de terceros en TODOS LOS headers y cpp.

#include "BubbleCombo.h"

#include <wx/msgdlg.h> //##Debug, se va en release. Agregar compilación condicional.


//##Future: Add the keyboard navigation keys to this combo class!

BEGIN_EVENT_TABLE(BubbleCombo, wxPanel)
    //EVT_ENTER_WINDOW(BubbleCombo::onMouseEnter)
    //EVT_LEAVE_WINDOW(BubbleCombo::onMouseLeave)
    EVT_LEFT_DOWN(BubbleCombo::onLeftDown)
    EVT_LEFT_UP(BubbleCombo::onPopUpList)
    EVT_SIZE(BubbleCombo::onSize)
END_EVENT_TABLE()


BubbleCombo::BubbleCombo(   wxWindow* parent,
                            wxWindowID id,
                            const wxImage &imageDefault,
                            const wxImage &imagePressed,
                            const wxImage &imageHover,
                            const wxImage &imageDisabled,
                            bool autoSize,
                            const wxPoint& pos,
                            const wxSize& size,
                            long style,
                            //const wxValidator& validator = wxDefaultValidator,
                            const wxString& name) : wxPanel(parent,
                                                            id,
                                                            pos,
                                                            size,
                                                            style,
                                                            name),
                                                    parent(parent),
                                                    //sizerMain(NULL),
                                                    buttonSelect(NULL),
                                                    text(NULL),
                                                    popList(NULL),
                                                    hGap(2), //##Make this configurable.
                                                    visible(true),
                                                    sorted(true),
                                                    autoSize(autoSize),
                                                    defaultMouseOverColour(wxColour(255, 255, 0)),
                                                    defaultErrorColour(wxColour(255, 0, 0))
{
    popList = new wxMenu;
    text = new wxStaticText(this, wxNewId(), wxString(""), wxPoint(hGap, 0)); //##Test!
    //text = new wxStaticText(this, wxNewId(),
    //                        wxString(""),
    //                        wxPoint(hGap, 0), wxDefaultSize,
    //                        wxTRANSPARENT_WINDOW);

    //sizerMain = new wxFlexGridSizer(2, 1);
    //if (sizerMain)
    //{
        //sizerMain->AddGrowableRow(0);
        //sizerMain->AddGrowableCol(0);
        //sizerMain->AddGrowableCol(1);
        //SetSizer(sizerMain);
        if (text)
        {
            text->Connect(  wxEVT_LEFT_UP,
                            wxMouseEventHandler(BubbleCombo::onPopUpList),
                            NULL,
                            this
                         );
            text->Connect(  wxEVT_LEFT_DOWN,
                            wxMouseEventHandler(BubbleCombo::onLeftDown),
                            NULL,
                            this
                         );
            //text->Connect(  wxEVT_ENTER_WINDOW,
            //                wxMouseEventHandler(BubbleCombo::onMouseEnter),
            //                NULL,
            //                this
            //               );
            //text->Connect(  wxEVT_LEAVE_WINDOW,
            //                wxMouseEventHandler(BubbleCombo::onMouseLeave),
            //                NULL,
            //                this
            //             );
            //SetClientSize(  text->GetSize().GetWidth(), text->GetSize().GetHeight());

            //sizerMain->Add(text, wxSizerFlags().Expand().Bottom());

            buttonSelect = new BubbleButton(this, wxNewId(),
                                            wxPoint(text->GetSize().GetWidth() + 2*hGap, 0));
            if (buttonSelect)
            {
                buttonSelect->setImageDefault(imageDefault);
                buttonSelect->setImagePressed(imagePressed);
                buttonSelect->setImageHover(imageHover);
                buttonSelect->setImageDisabled(imageDisabled);
                buttonSelect->SetSize(size.GetHeight(), size.GetHeight());
                //sizerMain->Add(buttonSelect, wxSizerFlags().Right());
                if (autoSize)
                {
                    SetClientSize(  text->GetSize().GetWidth() + buttonSelect->GetSize().GetWidth() + 2*hGap,
                                    buttonSelect->GetSize().GetHeight());//##
                }
                else
                {
                    SetClientSize(size.GetWidth(), size.GetHeight());//##
                    buttonSelect->Move(size.GetWidth() - buttonSelect->GetSize().GetWidth(), 0);
                }
                buttonSelect->Connect(  wxEVT_LEFT_DOWN,
                                        wxMouseEventHandler(BubbleCombo::onLeftDown),
                                        NULL,
                                        this
                                     );
                buttonSelect->Connect(  wxEVT_LEFT_UP,
                                        wxMouseEventHandler(BubbleCombo::onPopUpList),
                                        NULL,
                                        this
                                     );
            }
        }
        //sizerMain->Fit(this);
    //}
}


BubbleCombo::~BubbleCombo()
{
    //##Forgetting something?
}


void BubbleCombo::onMouseEnter(wxMouseEvent& event)
{
    if (text)
    {
        //Compares to avoid flickering:
        if (text->GetBackgroundColour() != defaultMouseOverColour)
        {
            //text->SetBackgroundColour(defaultMouseOverColour);
            //text->Update();
            //text->Refresh();
            text->SetBackgroundColour(defaultMouseOverColour);
            SetBackgroundColour(defaultMouseOverColour);
            Update();
            Refresh();
        }
    }
    event.Skip();
}


void BubbleCombo::onMouseLeave(wxMouseEvent& event)
{
    if (text)
    {
        //Compares to avoid flickering:
        if (text->GetBackgroundColour() == defaultMouseOverColour)
        {
            //text->SetBackgroundColour(GetBackgroundColour());
            //text->Update();
            //text->Refresh();
            text->SetBackgroundColour(GetBackgroundColour());
            Update();
            Refresh();
        }
    }
}


void BubbleCombo::onLeftDown(wxMouseEvent& event)
{
    //Sends an event to indicate that the text has changed:
    wxCommandEvent evt(wxEVT_COMMAND_TOOL_CLICKED, GetId());
    evt.SetEventObject(this);
    //evt.SetString(value);
    ProcessWindowEvent(evt);

    event.Skip();
}


//void BubbleCombo::onLeftUp(wxMouseEvent& event)
//{
//    //##
//    event.Skip();
//}


void BubbleCombo::onSize(wxSizeEvent& event)
{
    wxSize size = GetClientSize();
    if (buttonSelect)
    {
        buttonSelect->SetSize(size.GetHeight(), size.GetHeight());
        if (autoSize)
            buttonSelect->Move(wxPoint(text->GetSize().GetWidth() + 2*hGap, 0));
        else
            buttonSelect->Move(GetClientSize().GetWidth() - buttonSelect->GetSize().GetWidth(), 0);
    }
    event.Skip();
}


bool BubbleCombo::popUpList()
{
    if (popList)
    {
        if (text)
        {
            if (getCount() > 0)
            {
                PopupMenu(popList, wxPoint(0, text->GetSize().GetHeight()));
                return true;
            }
        }
    }
    return false;
}


wxFont BubbleCombo::getFont()
{
    wxFont dummyFont;
    if (text)
        return text->GetFont();
    return dummyFont;
}


void BubbleCombo::setFont(const wxFont& font)
{
    if (text)
        text->SetFont(font);
}


//##:
//void BubbleCombo::setSize(wxSize size)
//{
//    //If the don't want this behaviour, just call SetClientSize instead:
//    if (autoSize)
//    {
//        if (text && buttonSelect)
//        {
//            int width = text->GetSize().GetWidth() + buttonSelect->GetSize().GetWidth() + 2*hGap;
//            if (size.GetWidth() > width)
//                size.SetWidth(width);
//        }
//    }
//    SetClientSize(size);
//}


void BubbleCombo::setBackgroundColour(const wxColour& colour)
{
    //This if is to avoid flickering:
    if (GetBackgroundColour() != colour)
    {
        if (text)
            text->SetBackgroundColour(colour);
        SetBackgroundColour(colour);
        Update();
        Refresh();
    }
}


void BubbleCombo::onPopUpList(wxMouseEvent& event)
{
    if (!popUpList())
        event.Skip();   //This is necessary to force buttonSelect to show again it's default image
                        //when the left button is released (LEFT_UP event).
}


void BubbleCombo::onSelectOption(wxCommandEvent& event)
{
//##Testing:
//    if (buttonSelect)
//        buttonSelect->showImageDefault();

    event.GetId();
    if (popList)
    {
        wxMenuItem *item  = popList->FindItem(event.GetId());
        if (item)
        {
            setText(item->GetItemLabel());
        }
    }
    event.Skip();
}


void BubbleCombo::setText(const wxString& value)
{
    if (text == NULL)
        return;
    if (buttonSelect == NULL)
        return;

    text->SetLabel(value);
    //SetClientSize(  text->GetSize().GetWidth(), text->GetSize().GetHeight());
    if (autoSize)
    {
        SetClientSize(  text->GetSize().GetWidth() + buttonSelect->GetSize().GetWidth() + 2*hGap,
                        buttonSelect->GetSize().GetHeight());
    }
    text->Lower();
    buttonSelect->Raise();
    buttonSelect->Update();
    buttonSelect->Refresh();

    //Sends an event to indicate that the text has changed:
    wxCommandEvent event(wxEVT_COMMAND_TEXT_UPDATED, GetId());
    event.SetEventObject(this);
    event.SetString(value);
    ProcessWindowEvent(event);
}


void BubbleCombo::setVisible(const bool value)
{
    //##:
    visible = value;
}


bool BubbleCombo::Enable(bool value)
{
    //##
    return wxPanel::Enable(value);
}


bool BubbleCombo::Disable()
{
    return Enable(false);
}


//void BubbleCombo::setText(const wxString& value)
//{
//    //##
//}


wxString BubbleCombo::getText() const
{
    //##
    if (text)
        return text->GetLabel();
    return (wxString(""));
}


//##Make the image pointer constant:
void BubbleCombo::append(const wxString& itemCaption, wxImage *icon, int iconW, int iconH)
{
    if (popList == NULL)
        return;

    long id = wxNewId();
    wxMenuItem *newItem = NULL;
    int count = popList->GetMenuItemCount();

    newItem = new wxMenuItem(popList, id, itemCaption);
    if (newItem == NULL)
        return;

    if (icon)
    {
        //Both iconW and iconH has "0" as default value, which has no sense. So if the user leaves one of these fields in "0", the size is the original
        //size.
        if ((iconW > 0) && (iconH > 0))
            newItem->SetBitmap(wxBitmap(icon->Scale(iconW, iconH)));
        else
            newItem->SetBitmap(wxBitmap(*icon));
    }

    //##Bug: Probarlo con el ejemplo de PID, porque el ordenamiento falla de un modo extraño...
    if ( getSorted() && (count > 0) )
    {
        wxMenuItem *item = popList->FindItemByPosition(0);
        if (item)
        {
            //##Future: Write a better compare function, where the user can select this direct compare or
            //a more advanced one, which compares things like "D9" and "D10" and identifies the number (just
            //as modern operating systems do with file names):
            int i = 0;
            while ( (itemCaption > item->GetItemLabel()) &&  (i < count))
            {
                i++;
                item = popList->FindItemByPosition(i);
                if (item == NULL) //More secure.
                    break;
            }
            //wxMessageDialog dialog1(parent, itemCaption + (wxString(" ") << i), _("insert")); //##Debug.
            //dialog1.ShowModal();//##Debug
            popList->Insert(i, newItem);
        }
        else
            return; //Error! GetMenuItemCount() > 0, but there is no item at GetMenuItemCount()-1 position.
    }
    else
    {
        //if ( getSorted()) //##Debug
        //{
        //    wxMessageDialog dialog1(parent, itemCaption + (wxString(" ") << 0), _("insert")); //##Debug.
        //    dialog1.ShowModal();//##Debug
        //}
        //If the menu is not sorted, or if this is the first item (GetMenuItemCount() <= 0), appends the
        //new item:
        //newItem = popList->Append(id, itemCaption);
        popList->Append(newItem);
    }

    if (newItem)
    {
        Connect(id,
                wxEVT_COMMAND_MENU_SELECTED,
                wxCommandEventHandler(BubbleCombo::onSelectOption)
               );
        //This is not possible:
        //newItem->Connect(   wxEVT_MENU,
        //                    wxCommandEventHandler(BubbleCombo::onSelectOption),
        //                    NULL,
        //                    this
        //                );
    }
}


void BubbleCombo::clear()
{
    if (popList)
    {
        unsigned int count = popList->GetMenuItemCount();
        for (unsigned int i = 0; i < count; i++)
        {
            wxMenuItem *item = popList->FindItemByPosition(0);
            if (item)
                popList->Delete(item);
        }
    }
}


unsigned int BubbleCombo::getCount() const
{
    if (popList)
        return popList->GetMenuItemCount();
    return 0;
}


int BubbleCombo::getSelection() const
{
    if (popList && text)
    {
        return popList->FindItem(text->GetLabel());
    }
    return wxNOT_FOUND;
}


bool BubbleCombo::setSelection(unsigned int index)
{
    if (popList && text)
    {
        if (index < getCount())
        {
            wxMenuItem *item = popList->FindItemByPosition(index);  //It seems that FindItemByPosition returns
                                                                    //NULL if not found, but I did not see it
                                                                    //documented in the wx documentation.
            if (item)
            {
                setText(item->GetItemLabel());
                return true;
            }
        }
    }
    //If popList or text doesn't exist, or if index is not lower than getCount(): do nothing.
    return false;
}


bool BubbleCombo::setSelection(const wxString &value)
{
    if (popList)
    {
        //Finds the wxMenuItem's ID with this string (the string must be unique):
        int id = popList->FindItem(value);
        if (id != wxNOT_FOUND)
        {
            //Now obtains the pointer to the wxMenuItem itself:
            wxMenuItem *item = popList->FindItem(id);
            if (item)
            {
                //Finally, obtains the index of the wxMenuItem:
                wxMenuItemList items = popList->GetMenuItems();
                int index = items.IndexOf(item);
                if (index != wxNOT_FOUND)
                {
                    //wxMessageDialog dialog1(parent, value + (wxString(" ") << index), _("setSelection")); //##Debug.
                    //dialog1.ShowModal();//##Debug
                    return setSelection(index);
                }
            }
        }
    }
    return false;
}
