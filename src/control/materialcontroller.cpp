#include "materialcontroller.h"
#include "view/view.h"
#include "util.hpp"


MaterialController::MaterialController()
{
}

void MaterialController::listaMateriais(Request &request, StreamResponse &response)
{
	try{

		calassomys::View view;
		view.setContent(ifstream(server->getOption("document_root")+"/template.html"));
		view.insertContentId("conteudo", ifstream(server->getOption("document_root") + "/materiais.html"));
        GrupoList grupoList = model.getListGrupo();
        for(GrupoPtr& grupo: *grupoList)
        {
            view.insertContentId("grupos", "<option value='"+grupo->getNome()+"'>");
        }
        EntradaDeMaterialList list = model.getListMaterialComLote();
        string grupoNome;
        string htmlMatVencido;
        string htmlMatValido;
        const string divMat = "<div class='material";
        time_t hoje = time(NULL);
        for(EntradaDeMaterialPtr& em: *list)
        {
            const MateiralPtr& material = em->getMaterial();
            const LotePtr& lote = em->getLote();
            const string& validade = to_string(lote->getValidade());
            grupoNome = material->getGrupo()?material->getGrupo()->getNome():"";
            const string& img = material->getImagem().size()?material->getImagem():"/images/ampolas.jpg";
            const string& html = "' name='"+material->getNome()+"' onclick=mostrarMaterial('" + to_string(material->getId()) + "') id='mat" + to_string(material->getId()) + "'>\
                    <img name='imagem' src='"+img+"'/>\
                    <h4 name='nome'>"+material->getNome()+"</h4>\
                    <label name='descricao'>"+material->getDescricao()+"</label>\
                    <span>Grupo: <label name='grupo'>"+grupoNome+"</label></span>\
                    <span>Quantidade: <label name='quantidade'>"+to_string(material->getQuantidade())+"</label></span>"+
                    (validade.size()?"<span>Validade: <label name='validade'>"+validade+"</label></span>":"")+
                    "</div>";

            tm tm = lote->getValidade();
            time_t val = mktime(&tm);
            if(val<hoje && lote->getQuantidade()>0)
                htmlMatVencido+=divMat+" vencido"+html;
            else
                htmlMatValido+=divMat+html;
        }
        view.insertContentId("listMateiral", htmlMatVencido);
        view.insertContentId("listMateiral", htmlMatValido);
        response << view;
    }
    catch(exception &ex){
        string erro = "Não foi possível listar os produtos. Por favor tente mais tarde!<br>";
        mensagem(response, erro+ex.what());
    }
}

void MaterialController::salvarMaterial(Request &request, StreamResponse &response)
{
    const string& nome = request.get("nome");
    const string& descricao = request.get("descricao");
    const string& grupo = request.get("grupo");
    const string& imagem = request.get("imagem");
    const string& quantidade = request.get("quantidade");

    Mateiral material;
    material.setNome(nome);
    material.setDescricao(descricao);
    if(!grupo.empty())
        material.setGrupo(GrupoPtr(new Grupo(grupo)));
    material.setImagem(imagem);
    material.setQuantidade(stoi(quantidade.size()?quantidade:"0"));

    try{
        model.salvarMaterial(material);
        response.setCode(301);
        response.setHeader("Location", "/");
    }catch(const exception& ex){
        string msg = "<h1>Erro ao salvar o material</h1><br>";
        msg += ex.what();        
        this->mensagem(response, msg);
    }
}

void MaterialController::excluirMaterial(Request &request, StreamResponse &response)
{
	try{
		MateiralPtr material = model.getMaterialPorId(stoi(request.get("id")));
		model.excluirMaterial(*material);
		redirecionar(response, "/");
	}
	catch (const exception& ex){
		string msg = "<h1>Erro ao excluir o material</h1><br>";
		msg += ex.what();
		this->mensagem(response, msg);
	}
}

void MaterialController::alterarMaterial(Request &request, StreamResponse &response)
{
	try{
		MateiralPtr material = model.getMaterialPorId(stoi(request.get("id")));
		auto params = request.getAllVariable();
		material->setNome(params.find("nome")->second);
		material->setDescricao(params.find("descricao")->second);
		material->setImagem(params.find("imagem")->second);
		material->setQuantidade(stoi(params.find("quantidade")->second));
		if (!params.find("grupo")->second.empty())
			material->setGrupo(GrupoPtr(new Grupo(params.find("grupo")->second)));
		
		model.alterarMaterial(*material);
		redirecionar(response, "/");
	}
	catch (const exception& ex){
		string msg = "<h1>Erro ao alterar o material</h1><br>";
		msg += ex.what();
		this->mensagem(response, msg);
	}
}

void MaterialController::setup()
{
    addRoute("GET", "/", MaterialController, listaMateriais);
	addRoute("POST", "/material/salvar", MaterialController, salvarMaterial);
	addRoute("GET", "/material/excluir", MaterialController, excluirMaterial);
	addRoute("POST", "/material/alterar", MaterialController, alterarMaterial);
}
