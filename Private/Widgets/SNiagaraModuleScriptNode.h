#pragma once
#include "CoreMinimal.h"
#include "AssetThumbnail.h"

struct FNiagaraModuleScriptNode : TSharedFromThis<FNiagaraModuleScriptNode>
{
	FNiagaraModuleScriptNode(const FText& InDisplayName, FAssetData* InAssetDataPtr = nullptr)
		:DisplayName(InDisplayName), AssetDataPtr(InAssetDataPtr) {}
	
	static TSharedPtr<FNiagaraModuleScriptNode> NewNode(FAssetData* InAssetDataPtr);

	FText DisplayName;
	FAssetData* AssetDataPtr;
};


//----------------------------------------------------------------------------------
class SNiagaraModuleScriptNode : public SCompoundWidget
{
public:
	SNiagaraModuleScriptNode(){}
	~SNiagaraModuleScriptNode() { Node.Reset(); }
	
	SLATE_BEGIN_ARGS(SNiagaraModuleScriptNode)
	{}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, TSharedPtr<FNiagaraModuleScriptNode> InNode);
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	
private:
	TSharedPtr<FNiagaraModuleScriptNode> Node;
	TSharedPtr<FAssetThumbnailPool> AssetThumbnailPool;
};