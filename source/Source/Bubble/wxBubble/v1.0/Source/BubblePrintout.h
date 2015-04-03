#ifndef BubblePrintout__h
#define BubblePrintout__h


#include <wx/print.h>


class BubblePrintout : public wxPrintout
{
    protected:
        bool saved;

    public:
        BubblePrintout(const wxString& title) : wxPrintout(title)
        {
        }
        virtual ~BubblePrintout() { }

        virtual bool OnPrintPage(int);
};
#endif
