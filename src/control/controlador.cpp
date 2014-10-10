#include "controlador.h"
#include "view/view.h"

Controlador::Controlador()
{
}

void Controlador::mensagem(Mongoose::StreamResponse& response, const string& htmlTextErro)
{
    calassomys::View view;
    view.setContent(ifstream("WebContent/template.html"));
    view.insertContentId("conteudo", htmlTextErro);

    response << view;
}
