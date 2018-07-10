#include "stdafx.h"
#include "Material.h"
#include "../Utilities/String.h"

Material::Material(wstring file)
	: shader(NULL)
	, diffuseMap(NULL), specularMap(NULL), emissiveMap(NULL), normalMap(NULL), detailMap(NULL)
{
	if (file.length() > 0)
		shader = new Shader(file);

	buffer = new MaterialBuffer();
}

Material::~Material()
{
	SAFE_DELETE(diffuseMap);
	SAFE_DELETE(specularMap);
	SAFE_DELETE(emissiveMap);
	SAFE_DELETE(normalMap);
	SAFE_DELETE(detailMap);

	SAFE_DELETE(buffer);
	SAFE_DELETE(shader);
}

void Material::Clone(void ** clone)
{
	wstring shaderFile = this->shader->GetFile();
	Material* material = new Material(shaderFile);

	material->SetNumber(this->number);
	material->SetName(this->name);

	material->SetAmbient(*this->GetAmbient());
	material->SetDiffuse(*this->GetDiffuse());
	material->SetSpecular(*this->GetSpecular());
	material->SetEmissive(*this->GetEmissive());

	material->SetShininess(*this->GetShininess());
	material->SetDetailFactor(*this->GetDetailFactor());


	if (this->diffuseMap != NULL)
		material->SetDiffuseMap(this->diffuseMap->GetFile());

	if (this->specularMap != NULL)
		material->SetSpecularMap(this->specularMap->GetFile());

	if (this->emissiveMap != NULL)
		material->SetEmissiveMap(this->emissiveMap->GetFile());

	if (this->normalMap != NULL)
		material->SetNormalMap(this->normalMap->GetFile());

	if (this->detailMap != NULL)
		material->SetDetailMap(this->detailMap->GetFile());

	*clone = material;
}

void Material::SetShader(wstring file)
{
	SAFE_DELETE(shader);

	shader = new Shader(file);
}

void Material::SetDiffuseMap(string file, D3DX11_IMAGE_LOAD_INFO * info)
{
	SetDiffuseMap(String::StringToWString(file));
}

void Material::SetDiffuseMap(wstring file, D3DX11_IMAGE_LOAD_INFO * info)
{
	SAFE_DELETE(diffuseMap);

	diffuseMap = new Texture(file, info);
}

void Material::SetSpecularMap(string file, D3DX11_IMAGE_LOAD_INFO * info)
{
	SetSpecularMap(String::StringToWString(file));
}

void Material::SetSpecularMap(wstring file, D3DX11_IMAGE_LOAD_INFO * info)
{
	SAFE_DELETE(specularMap);

	specularMap = new Texture(file, info);
}

void Material::SetEmissiveMap(string file, D3DX11_IMAGE_LOAD_INFO * info)
{
	SetEmissiveMap(String::StringToWString(file));
}

void Material::SetEmissiveMap(wstring file, D3DX11_IMAGE_LOAD_INFO * info)
{
	SAFE_DELETE(emissiveMap);

	emissiveMap = new Texture(file, info);
}

void Material::SetNormalMap(string file, D3DX11_IMAGE_LOAD_INFO * info)
{
	SetNormalMap(String::StringToWString(file));
}

void Material::SetNormalMap(wstring file, D3DX11_IMAGE_LOAD_INFO * info)
{
	SAFE_DELETE(normalMap);

	normalMap = new Texture(file, info);
}

void Material::SetDetailMap(string file, D3DX11_IMAGE_LOAD_INFO * info)
{
	SetDetailMap(String::StringToWString(file));
}

void Material::SetDetailMap(wstring file, D3DX11_IMAGE_LOAD_INFO * info)
{
	SAFE_DELETE(detailMap);

	detailMap = new Texture(file, info);
}

void Material::SetBuffer()
{
	ID3D11ShaderResourceView* null[1]{ NULL };

	UINT slot = 0;
	if (diffuseMap != NULL)
	{
		diffuseMap->SetShaderResource(slot);
		diffuseMap->SetShaderSampler(slot);
	}
	else
	{
		Texture::SetBlankShaderResource(slot);
		Texture::SetBlankSamplerState(slot);
	}


	slot = 1;
	if (specularMap != NULL)
	{
		specularMap->SetShaderResource(slot);
		specularMap->SetShaderSampler(slot);
	}
	else
	{
		Texture::SetBlankShaderResource(slot);
		Texture::SetBlankSamplerState(slot);
	}


	slot = 2;
	if (emissiveMap != NULL)
	{
		emissiveMap->SetShaderResource(slot);
		emissiveMap->SetShaderSampler(slot);
	}
	else
	{
		Texture::SetBlankShaderResource(slot);
		Texture::SetBlankSamplerState(slot);
	}


	slot = 3;
	if (normalMap != NULL)
	{
		normalMap->SetShaderResource(slot);
		normalMap->SetShaderSampler(slot);
	}
	else
	{
		Texture::SetBlankShaderResource(slot);
		Texture::SetBlankSamplerState(slot);
	}


	slot = 4;
	if (detailMap != NULL)
	{
		detailMap->SetShaderResource(slot);
		detailMap->SetShaderSampler(slot);
	}
	else
	{
		Texture::SetBlankShaderResource(slot);
		Texture::SetBlankSamplerState(slot);
	}


	buffer->SetPSBuffer(1);

	if (shader != NULL)
		shader->Render();
}