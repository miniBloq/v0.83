#ifndef BubbleProcess__h
#define BubbleProcess__h


//#include "Bubble.h" //##
#include <wx/process.h>


//##Esto no está terminado:
//Hay que armar cuidadosamente le sistema de redirección del stdout, stderror, etc. para no llenar los buffers,
//cosa que ¿podría crasehar la aplicación? De todos modos, necesito esos buffers para mostrar la barra de progreso
//en el futuro.
//##Si la cosa realmente no va, voy a usar la API del sistema operativo directamente, como hice en el MinibloqRun.
class IBubbleNotifier;
class BubbleProcess : public wxProcess
{
    protected:
        IBubbleNotifier *notifier;

    public:
        BubbleProcess(IBubbleNotifier *notifier) :  //wxProcess(wxPROCESS_DEFAULT), //##<- Only for debug.
                                                    wxProcess(wxPROCESS_REDIRECT), //##Va éste: Ojo que no llene el buffer de stdout
                                                    notifier(notifier)
        {
        }

        void OnTerminate(int pid, int status);
};


#endif
