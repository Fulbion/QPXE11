#pragma once

#include "Engine/Graphics/ResourceManager/ResourceManager.hpp"

class MeshManager :
	public ResourceManager
{
public:
	MeshManager();
	~MeshManager();

	MeshPtr createMeshFromFile(const wchar_t* filePath);

private:
	virtual Resource* createResourceFromFileConcrete(const wchar_t* filePath) override;
};
