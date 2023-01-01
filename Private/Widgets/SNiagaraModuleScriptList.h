#pragma once

#include "CoreMinimal.h"
#include "SNiagaraModuleScriptNode.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Views/SListView.h"

class SSearchBox;

class SNiagaraModuleScriptList : public SCompoundWidget
{
public:
	SNiagaraModuleScriptList(){}
	~SNiagaraModuleScriptList(){}

	DECLARE_DELEGATE_OneParam(FOnCollectAllModules, TArray<FAssetData>&);
	DECLARE_DELEGATE_OneParam(FOnModuleSelected, FAssetData*);

	SLATE_BEGIN_ARGS(SNiagaraModuleScriptList)
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
	// 委托实例（作为控件参数）
	FOnCollectAllModules OnCollectAllModules;
	FOnModuleSelected OnModuleSeleted;
	// 列表控件
	TSharedPtr< SListView< TSharedPtr<FNiagaraModuleScriptNode> > > ListView;
	
	// 搜索框控件和功能接口
	TSharedPtr<SSearchBox> FilterTextBox;
	void OnFilterTextChanged(const FText& InFilterText);
	
	// All Modules
	TArray<TSharedPtr<FNiagaraModuleScriptNode>> ModuleScriptsToShow; // 结构体共享指针数组，用于 ListView 构建
	TArray<FAssetData> AllModuleAssetDatas; // FAssetData 数组，用于构建结构体，是真正与 Dialog 之间通信的桥梁
	
	TSharedRef<ITableRow> OnGenerateRow(TSharedPtr<FNiagaraModuleScriptNode> InItem, const TSharedRef<STableViewBase>& OwnerTable);
	void OnSelectionChanged(TSharedPtr< FNiagaraModuleScriptNode > InSelectedItem, ESelectInfo::Type SelectInfo);
	void GenerateFilteredItems();
};