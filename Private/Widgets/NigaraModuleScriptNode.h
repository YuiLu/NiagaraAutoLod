#pragma once
#include "CoreMinimal.h"
#include "AssetRegistry/AssetData.h"

// һ�����ڴ洢��״����Ľṹ��
// ��������
//		  --
//			|--
//			|--
//		  --
//			|--
//			   |--
//             |--
struct FNigaraModuleScriptNode : TSharedFromThis<FNigaraModuleScriptNode>
{
public:
	FText ShowName;
	FAssetData* AssetDataPtr;
	TArray<TSharedPtr<FNigaraModuleScriptNode>> Childrens;

public:
	static TSharedPtr<FNigaraModuleScriptNode> NewRootNode();

	TSharedPtr<FNigaraModuleScriptNode> AddChild(FAssetData* InAssetDataPtr);

	FText const& GetDisplayName() const;

	void ClearChildren() { Childrens.Empty(); }
private:
	FNigaraModuleScriptNode(const FText& InShowName, FAssetData* InAssetDataPtr = nullptr);
	static TSharedPtr<FNigaraModuleScriptNode> NewNode(FAssetData* InAssetDataPtr);
};