#ifndef ENTRADAMODEL_H
#define ENTRADAMODEL_H

#include "entity/entradadematerial.h"
#include "materialmodel.h"
#include "entradadematerialmodel.h"
#include "dao/dao.h"

class EntradaModel
{
    DaoPrt dao;
    MaterialModel materialModel;
    EntradaDeMaterialModel entradaDeMaterialModel;
public:
    EntradaModel();
    MateiralList getListMaterial();
    EntradaList getListEntrada();
    EntradaDeMaterialList getListEntradaDeMaterial(const string& idEntrada);
    MateiralPtr getMaterialPorId(const int& id);
    void salvaEntrada(Entrada& entrada);
	void alterarEntrada(Entrada& entrada);
    void salvaListEntradaDeMaterial(EntradaDeMaterialList& list);
	void alterarListEntradaDeMaterial(EntradaDeMaterialList& list, string id);
};

#endif // ENTRADAMODEL_H
