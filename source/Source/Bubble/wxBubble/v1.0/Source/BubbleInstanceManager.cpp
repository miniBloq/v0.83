#include "BubbleCanvas.h"
//## #include "Bubble.h"

//#include <wx/msgdlg.h> //##Debug. Add conditional compilation!


int BubbleInstanceManager::getInstanceIndex(const wxString &name) const
{
    //##Lineal search by now, make a binary search in the future to optimize:
    for (unsigned int index = 0; index<instances.GetCount(); index++)
    {
        BubbleInstance *tempInstance = &(instances[index]);
        if (tempInstance)
        {
            if (name == tempInstance->getName())
                return index;
        }
    }
    return wxNOT_FOUND;
}


//##If the instance name doesn't exists, this adds it. If it exists, this function allows to modify its type:
void BubbleInstanceManager::setInstance(BubbleInstance *const value)
{
    if (value)
    {
        removeInstance(value->getName());
        instances.Add(value);
    }
}


//##Receives the name as a param, and if the name already exists, returns the pointer to a BubbleInstance
//object. If it doesn't exist, returns NULL:
BubbleInstance *BubbleInstanceManager::getInstance(const wxString& name) const
{
    int index = getInstanceIndex(name);
    if (index != wxNOT_FOUND)
        return &(instances[index]);
    return NULL;
}


BubbleInstance *BubbleInstanceManager::getInstance(int index) const
{
    if ((unsigned int)index < instances.GetCount())
        return &(instances[index]);
    return NULL;
}


//##If the instance name exists, this deletes the instance. If it doew not exist, this function does nothing:
void BubbleInstanceManager::removeInstance(const wxString& name)
{
//    wxMessageDialog dialog1(NULL, name, _("3")); //##Debug.
//    dialog1.ShowModal();//##Debug

    int index = getInstanceIndex(name);
//    wxString t = ""; //##Debug.
//    wxMessageDialog dialog2(NULL, t<<index, _("4")); //##Debug.
//    dialog2.ShowModal();//##Debug

    if (index != wxNOT_FOUND)
    {
        //Since it's a wxObjArray, the RemoveAt() really deletes the stored object:
        instances.RemoveAt(index);
    }
}


void BubbleInstanceManager::clearType(const wxString& typeName)
{
    for (unsigned int index = 0; index<instances.GetCount(); index++)
    {
        BubbleInstance *tempInstance = &(instances[index]);
        if (tempInstance)
        {
            if (typeName == tempInstance->getType())
            {
                instances.RemoveAt(index);
            }
        }
    }
}
