#include "SNiagaraModuleScriptList.h"

#include "Widgets/Layout/SScrollBorder.h"
#include "Widgets/Input/SSearchBox.h"
#include "Widgets/Views/STableRow.h"
#include "Widgets/Layout/SScrollBox.h"


void SNiagaraModuleScriptList::Construct(const FArguments& InArgs)
{
	this->OnCollectAllModules = InArgs._OnCollectAllModules;
	this->OnModuleSeleted = InArgs._OnModuleSeleted;

	ModuleScriptsToShow.Empty();
	
	// SListView初始化
	ListView = SNew(SListView<TSharedPtr<FNiagaraModuleScriptNode>>)
		.OnGenerateRow(this, &SNiagaraModuleScriptList::OnGenerateRow)
		.ListItemsSource(&ModuleScriptsToShow)
		.ItemHeight(24)
		.OnSelectionChanged(this, &SNiagaraModuleScriptList::OnSelectionChanged)
		.SelectionMode(ESelectionMode::Single)
		.ClearSelectionOnClick(true);
	
	ChildSlot
	[
		SNew(SVerticalBox)
		
		// 搜索框
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 10, 0, 10)
		[
			SAssignNew(FilterTextBox, SSearchBox)
			.OnTextChanged(this, &SNiagaraModuleScriptList::OnFilterTextChanged) // 筛选框中的文本有变化时的回调接口
			.DelayChangeNotificationsWhileTyping(false)
		]
		
		+ SVerticalBox::Slot()
		.FillHeight(1.0)
		[
			ListView.ToSharedRef()
		]
	];
	RefreshAllModules();
}

void SNiagaraModuleScriptList::OnFilterTextChanged(const FText& InFilterText)
{
	GenerateFilteredItems();
}

TSharedRef<ITableRow> SNiagaraModuleScriptList::OnGenerateRow(TSharedPtr<FNiagaraModuleScriptNode> InNode, const TSharedRef<STableViewBase>& OwnerTable)
{
	TSharedPtr<STableRow<TSharedPtr<FNiagaraModuleScriptNode>>> TableRow = SNew(STableRow< TSharedPtr<FNiagaraModuleScriptNode> >, OwnerTable);
	TableRow->SetRowContent(SNew(SNiagaraModuleScriptNode, InNode));
	return TableRow.ToSharedRef();
}

void SNiagaraModuleScriptList::OnSelectionChanged(TSharedPtr< FNiagaraModuleScriptNode > InSelectedItem, ESelectInfo::Type SelectInfo)
{
	OnModuleSeleted.ExecuteIfBound(InSelectedItem ? InSelectedItem->AssetDataPtr : nullptr);
}

void SNiagaraModuleScriptList::RefreshAllModules()
{
	AllModuleAssetDatas.Empty();
	// 获取所有的Modules
	OnCollectAllModules.ExecuteIfBound(AllModuleAssetDatas);
	
	GenerateFilteredItems();
}

void SNiagaraModuleScriptList::GenerateFilteredItems()
{
	// 根据筛选框控件中输入的筛选文本，来对Modules进行判断，符合条件的放入到Tree中。
	FString FilterString;
	if (FilterTextBox.IsValid())
	{
		FilterString = FilterTextBox->GetText().ToString();
	}

	ModuleScriptsToShow.Empty();
	for (FAssetData& AssetData : AllModuleAssetDatas)
	{
		if (!FilterString.IsEmpty() )
		{
			if (AssetData.AssetName.ToString().Contains(FilterString))
			{
				ModuleScriptsToShow.Add(FNiagaraModuleScriptNode::NewNode(&AssetData));
			}
		}
		else
		{
			ModuleScriptsToShow.Add(FNiagaraModuleScriptNode::NewNode(&AssetData));
		}
	}
	ListView->RebuildList();
}