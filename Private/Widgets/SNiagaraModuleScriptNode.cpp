#include "SNiagaraModuleScriptNode.h"

TSharedPtr<FNiagaraModuleScriptNode> FNiagaraModuleScriptNode::NewNode(FAssetData* InAssetDataPtr)
{
	if (!InAssetDataPtr)
		return nullptr;
	return MakeShareable(new FNiagaraModuleScriptNode(FText::FromName(InAssetDataPtr->AssetName), InAssetDataPtr));
}


//----------------------------------------------------------------------------------
void SNiagaraModuleScriptNode::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	AssetThumbnailPool->Tick(InDeltaTime);
}

void SNiagaraModuleScriptNode::Construct(const FArguments& InArgs, TSharedPtr<FNiagaraModuleScriptNode> InNode)
{
	Node = InNode;
	const FAssetData AssetData = *InNode->AssetDataPtr;
	// 缩略图
	const int32 ThumbnailSize = 64;
	AssetThumbnailPool = MakeShareable(new FAssetThumbnailPool(48));
	TSharedRef<FAssetThumbnail> AssetThumbnail = MakeShared<FAssetThumbnail>(AssetData, ThumbnailSize, ThumbnailSize, AssetThumbnailPool);
	FAssetThumbnailConfig ThumbnailConfig;
	ThumbnailConfig.bAllowFadeIn = false;
	TSharedRef<SWidget> ThumbnailWidget = AssetThumbnail->MakeThumbnailWidget(ThumbnailConfig);
	
	this->ChildSlot
	[
		SNew(SHorizontalBox)
		
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(16, 3, 3, 3)
		[
			SNew(SBox)
			.WidthOverride(64)
			.HeightOverride(64)
			[
				ThumbnailWidget
			]
		]
		
		+ SHorizontalBox::Slot()
		.VAlign(VAlign_Center)
		.Padding(5)
		[
			SNew(STextBlock)
			.Font(FCoreStyle::GetDefaultFontStyle("Bold", 9))
			.Text(Node->DisplayName)
		]
	];
}
