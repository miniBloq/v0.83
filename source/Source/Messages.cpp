#include "Messages.h"

/*##Ver si se usará:
BEGIN_EVENT_TABLE(Messages, BubblePanel)
    //EVT_SIZE(Messages::onSize)
END_EVENT_TABLE()
*/

Messages::Messages( wxWindow* parent,
                    wxWindowID id,
                    const wxColour& colour,
                    const wxPoint& pos,
                    const wxSize& size,
                    long style,
                    const wxString& name) : BubblePanel(parent,
                                                        id,
                                                        colour,
                                                        false,
                                                        pos,
                                                        size,
                                                        style,
                                                        name),
                                            sizer(NULL),
                                            txtMsg(NULL),
                                            progress(NULL)
{
    //##Hide();

    //##sizer = new wxGridSizer(1, 0, 0);
    //sizer = new wxBoxSizer(wxVERTICAL);
    sizer = new wxFlexGridSizer(1, 0, 0);
    SetSizer(sizer);

    if (sizer)
    {
        //sizer->SetFlexibleDirection(wxVERTICAL);

        txtMsg = new wxTextCtrl(this,
                                wxNewId(),
                                wxEmptyString,
                                wxDefaultPosition,
                                wxDefaultSize,
                                wxTE_AUTO_SCROLL | wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH2 |
                                wxTE_DONTWRAP, //##Ver si algo de acá tiene que ser configurable...
                                wxDefaultValidator,
                                wxString("txtMsg")
                               );
        if (txtMsg)
        {
            //txtMsg->SetMinSize(wxSize(200, 30)); //##¿Funciona esto?
            txtMsg->SetDefaultStyle(wxTextAttr(*wxGREEN, txtMsg->GetBackgroundColour()));
            txtMsg->SetBackgroundColour(wxColour(0, 0, 0));

            sizer->Add(txtMsg, 1, wxFIXED_MINSIZE | wxEXPAND);
            //sizer->Add(txtMsg, 1, wxEXPAND);
        }

        //##Ver por qué no muestra el wxGauge con el aspecto de Windows Vista, y la muestra plana, sin
        //iluminación, ni efecto 3D (tatno en smooth como en estándar):
        progress = new wxGauge( this,
                                wxNewId(),
                                100, //##Range..
                                wxDefaultPosition,
                                wxDefaultSize,
                                wxGA_HORIZONTAL,// | wxGA_SMOOTH,
                                wxDefaultValidator,
                                wxString("progress")
                              );
        if (progress)
        {
            //progress->SetBezelFace(2);
            sizer->Add(progress, 1, wxEXPAND);
        }

        sizer->AddGrowableRow(0);
        sizer->AddGrowableCol(0);
#if defined (WIN32)
        sizer->AddGrowableCol(1);
#endif
    }

    //SetBackgroundColour(colour);
    SetBackgroundColourAndRefresh(colour);
}


Messages::~Messages()
{
    //##No va: Como los elementos de la lista son descendientes de BubblePanel, y tienen sus parents,
    //el frame o dialog al que pertenecen los destruirá. Y aquí, como el objeto canvases es estático,
    //se asegura la llamada a su destructor sin hacer nada:
    //canvases.Clear();
}


void Messages::updateGUI()
{
    //##
}


/*##Ver si es necesario este evento...
void Messages::onSize(wxSizeEvent& event)
{
    event.Skip();
}
*/
void Messages::setTextColour(const wxColour& colour)
{
    if (txtMsg)
    {
        txtMsg->SetDefaultStyle(wxTextAttr(colour, txtMsg->GetBackgroundColour())); //##Future: let configure the Background colour too.
    }
}
