#pragma once

#include "CoreMinimal.h"
#include "NigaraModuleScriptNode.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Views/STreeView.h"

class SSearchBox;
struct FNigaraModuleScriptNode;

////////////////////////////////////////////////////////////////////////////////////
class SNiagaraModuleScriptTreeNodeWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SNiagaraModuleScriptTreeNodeWidget)
	{}
	SLATE_END_ARGS()

	SNiagaraModuleScriptTreeNodeWidget(){}
	~SNiagaraModuleScriptTreeNodeWidget() { Node.Reset(); }
public:
	void Construct(const FArguments& InArgs, TSharedPtr<FNigaraModuleScriptNode> InNode)
	{
		Node = InNode;
		this->ChildSlot
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.Padding(0, 5, 0, 5)
			[
				SNew(STextBlock)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 9))
				.Text(Node->GetDisplayName())
			]
		];
	}

private:
	TSharedPtr<FNigaraModuleScriptNode> Node;
};

/////////////////////////////////////////////////////////////////////////////////////
class SNiagaraModuleScriptTree : public SCompoundWidget
{
public:
	DECLARE_DELEGATE_OneParam(FOnCollectAllModules, TArray<FAssetData>&);
	DECLARE_DELEGATE_OneParam(FOnModuleSelected, FAssetData*);

public:
	SNiagaraModuleScriptTree();
	~SNiagaraModuleScriptTree();

	SLATE_BEGIN_ARGS(SNiagaraModuleScriptTree)
		: _AutoExpandActionMenu(false)
		, _AlphaSortItems(true)
	{}

	SLATE_EVENT(FOnCollectAllModules, OnCollectAllModules)
	SLATE_EVENT(FOnModuleSelected, OnModuleSeleted)
	SLATE_ARGUMENT(bool, AutoExpandActionMenu)
	SLATE_ARGUMENT(bool, AlphaSortItems)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void RefreshAllModules();

private:
	// 搜索框控件和功能接口
	TSharedPtr<SSearchBox> FilterTextBox;
	void OnFilterTextChanged(const FText& InFilterText);

	TSharedPtr< STreeView< TSharedPtr<FNigaraModuleScriptNode> > > TreeView;

	// 获取所有的Module
	TArray<FAssetData> AllModuleAssetDatas;
	FOnCollectAllModules OnCollectAllModules;
	FOnModuleSelected OnModuleSeleted;

	//
	TSharedPtr<FNigaraModuleScriptNode> RootNode;
	TSharedRef<ITableRow> MakeWidget(TSharedPtr<FNigaraModuleScriptNode> InItem, const TSharedRef<STableViewBase>& OwnerTable);
	void OnItemSelected(TSharedPtr< FNigaraModuleScriptNode > InSelectedItem, ESelectInfo::Type SelectInfo);
	void OnGetChildren(TSharedPtr<FNigaraModuleScriptNode> InItem, TArray< TSharedPtr<FNigaraModuleScriptNode> >& OutChildren);
	void GenerateFilteredItems();
};