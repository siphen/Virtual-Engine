#include "DirectionalLight.h"
#include "World.h"
#include "../RenderComponent/RenderTexture.h"
#include "../RenderComponent/DescriptorHeap.h"
ObjectPtr<DirectionalLight> DirectionalLight::current;
DirectionalLight::DirectionalLight(const ObjectPtr<Transform>& trans, ObjectPtr<Component>& ptr) :
	Component(trans, ptr)
{
	
}

DirectionalLight* DirectionalLight::GetInstance(
	const ObjectPtr<Transform>& trans,
	uint* shadowmapResolution,
	ID3D12Device* device)
{
	if (!current)
	{
		ObjectPtr<Component> comp;
		new DirectionalLight(trans, comp);
		current = comp.CastTo<DirectionalLight>();
	}
	memcpy(current->shadowmapResolution, shadowmapResolution, sizeof(uint) * CascadeLevel);
	RenderTextureFormat smFormat;
	smFormat.depthFormat = RenderTextureDepthSettings_Depth32;
	smFormat.usage = RenderTextureUsage::DepthBuffer;
	for (uint i = 0; i < CascadeLevel; ++i)
	{
		current->shadowmaps[i] = ObjectPtr<RenderTexture>::NewObject(
			device, shadowmapResolution[i], shadowmapResolution[i], smFormat, TextureDimension::Tex2D, 1, 0, RenderTextureState::Generic_Read
		);
	}
	return current;
}
/*
uint* shadowmapResolution,
ID3D12Device* device*/

DirectionalLight::~DirectionalLight()
{
	current = nullptr;
	for (uint i = 0; i < CascadeLevel; ++i)
	{
		shadowmaps[i].Destroy();
	}
}