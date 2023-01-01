#pragma once
#include "CoreMinimal.h"
#include "AssetRegistry/AssetData.h"

// 一个用于存储树状对象的结构体
// 类似这种
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