#include "stdafx.h"
#include "BinModel.h"
#include "MeshPart.h"

Model::BinModel::BinModel()
{
}

Model::BinModel::~BinModel()
{
	for (MeshPart* part : meshParts)
		SAFE_DELETE(part);
}

void Model::BinModel::Clone(void ** clone)
{
	BinModel* binModel = new BinModel();
	binModel->name = this->name;

	binModel->meshParts.reserve(this->meshParts.size());
	for (MeshPart* part : meshParts)
	{
		MeshPart* temp = NULL;
		part->Clone((void **)&temp);

		binModel->meshParts.push_back(temp);
	}

	*clone = binModel;
}

void Model::BinModel::Update()
{
	for (MeshPart* part : meshParts)
		part->Update();
}

void Model::BinModel::Render()
{
	for (MeshPart* part : meshParts)
		part->Render();
}
