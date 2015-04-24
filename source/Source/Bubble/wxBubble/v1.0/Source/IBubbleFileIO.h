#ifndef IBubbleFileIO__h
#define IBubbleFileIO__h


#include <wx/string.h>


class IBubbleFileIO
{
    protected:
        bool saved;

    public:
        IBubbleFileIO()  { saved = true; } //##Ver qué se hace con las clases abstractas y este asunto...

        virtual ~IBubbleFileIO(){ }

        virtual bool isSaved() const = 0;
        inline virtual void forceSaved(const bool value) { saved = true; }

        virtual bool loadComponentFromFile(const wxString& name) = 0;
        virtual bool saveComponentToFile(const wxString& name, bool format = true) = 0;
};
#endif
