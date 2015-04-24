

#include "BubbleProcess.h"
#include "Bubble.h"

void BubbleProcess::OnTerminate(int pid, int status)
{
    //##Ver si hay que implementar alto más...
    if (notifier)
        notifier->processTermination(this);   //##Ver si más adelante voy a pasar alguna info como parámetro
                                                //(no quiero enviar el this como en el ejemplo de wxWidgets)...
}
