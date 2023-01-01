#include "NigaraModuleScriptNode.h"

TSharedPtr<FNigaraModuleScriptNode> FNigaraModuleScriptNode::NewRootNode()
{
	return MakeShareable(new FNigaraModuleScriptNode(FText::GetEmpty()));
}

TSharedPtr<FNigaraModuleScriptNode> FNigaraModuleScriptNode::AddChild(FAssetData* InAssetDataPtr)
{
	Childrens.Add(FNigaraModuleScriptNode::NewNode(InAssetDataPtr));
	return Childrens.Last();
}

FText const& FNigaraModuleScriptNode::GetDisplayName() const
{
	return ShowName;
}

FNigaraModuleScriptNode::FNigaraModuleScriptNode(const FText& InShowName, FAssetData* InAssetDataPtr)
	: ShowName(InShowName), AssetDataPtr(InAssetDataPtr)
{
	Childrens.Empty();
}

TSharedPtr<FNigaraModuleScriptNode> FNigaraModuleScriptNode::NewNode(FAssetData* InAssetDataPtr)
{
	if (!InAssetDataPtr)
	{
		return nullptr;
	}

	return MakeShareable(new FNigaraModuleScriptNode(FText::FromName(InAssetDataPtr->AssetName), InAssetDataPtr));
}