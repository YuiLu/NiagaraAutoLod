#include "SNiagaraModuleScriptTree.h"

#include "Widgets/Layout/SScrollBorder.h"
#include "Widgets/Input/SSearchBox.h"
#include "Widgets/Views/STableRow.h"
#include "Widgets/Layout/SScrollBox.h"

SNiagaraModuleScriptTree::SNiagaraModuleScriptTree()
{
}

SNiagaraModuleScriptTree::~SNiagaraModuleScriptTree()
{
}

void SNiagaraModuleScriptTree::Construct(const FArguments& InArgs)
{
	this->OnCollectAllModules = InArgs._OnCollectAllModules;
	this->OnModuleSeleted = InArgs._OnModuleSeleted;
	this->RootNode = FNigaraModuleScriptNode::NewRootNode();

	// STreeView的初始化
	TreeView = SNew(STreeView<TSharedPtr<FNigaraModuleScriptNode>>)
		.ItemHeight(24)
		.TreeItemsSource(&(RootNode->Childrens)) // 设置需要显示的对象
		.OnGenerateRow(this, &SNiagaraModuleScriptTree::MakeWidget) // 注册生成每个TreeItem的回调
		.OnSelectionChanged(this, &SNiagaraModuleScriptTree::OnItemSelected) // 选中后的回调
		.SelectionMode(ESelectionMode::Single) // 设置选择模式，单选
		.OnGetChildren(this, &SNiagaraModuleScriptTree::OnGetChildren); // 注册Tree Children的获取回调
	
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot().AutoHeight().Padding(0, 10, 0, 10)
		[
			SAssignNew(FilterTextBox, SSearchBox) // 搜索框
			.OnTextChanged(this, &SNiagaraModuleScriptTree::OnFilterTextChanged) // 筛选框中的文本有变化时的回调接口
			.DelayChangeNotificationsWhileTyping(false)
		]
				
		+ SVerticalBox::Slot().Padding(FMargin(0.0f, 2.0f, 0.0f, 0.0f)).FillHeight(1.0f)
		[
			// 用垂直滚动控件来显示TreeItem
			SNew(SScrollBorder, TreeView.ToSharedRef())
			[
				TreeView.ToSharedRef()
			]
		]
	];
	RefreshAllModules();
}

void SNiagaraModuleScriptTree::OnFilterTextChanged(const FText& InFilterText)
{
	GenerateFilteredItems();
}

TSharedRef<ITableRow> SNiagaraModuleScriptTree::MakeWidget(TSharedPtr<FNigaraModuleScriptNode> InItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	TSharedPtr<STableRow<TSharedPtr<FNigaraModuleScriptNode>>> TableRow = SNew(STableRow< TSharedPtr<FNigaraModuleScriptNode> >, OwnerTable);
	TableRow->SetRowContent(
		SNew(SNiagaraModuleScriptTreeNodeWidget, InItem)
	);
	return TableRow.ToSharedRef();
}

void SNiagaraModuleScriptTree::OnItemSelected(TSharedPtr< FNigaraModuleScriptNode > InSelectedItem, ESelectInfo::Type SelectInfo)
{
	OnModuleSeleted.ExecuteIfBound(InSelectedItem->AssetDataPtr);
}

void SNiagaraModuleScriptTree::OnGetChildren(TSharedPtr<FNigaraModuleScriptNode> InItem, TArray< TSharedPtr<FNigaraModuleScriptNode> >& OutChildren)
{
	if (InItem->Childrens.Num())
	{
		OutChildren = InItem->Childrens;
	}
}

void SNiagaraModuleScriptTree::RefreshAllModules()
{
	AllModuleAssetDatas.Empty();
	// 获取所有的Modules
	OnCollectAllModules.ExecuteIfBound(AllModuleAssetDatas);
	
	GenerateFilteredItems();
}

void SNiagaraModuleScriptTree::GenerateFilteredItems()
{
	// 根据筛选框控件中输入的筛选文本，来对Modules进行判断，符合条件的放入到Tree中。
	FString FilterString;
	if (FilterTextBox.IsValid())
	{
		FilterString = FilterTextBox->GetText().ToString();
	}

	RootNode->ClearChildren();
	for (FAssetData& AssetData : AllModuleAssetDatas)
	{
		if (!FilterString.IsEmpty() )
		{
			if (AssetData.AssetName.ToString().Contains(FilterString))
			{
				RootNode->AddChild(&AssetData);
			}
		}
		else
		{
			RootNode->AddChild(&AssetData);
		}
	}
	TreeView->RebuildList();
}