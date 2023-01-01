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

	// STreeView�ĳ�ʼ��
	TreeView = SNew(STreeView<TSharedPtr<FNigaraModuleScriptNode>>)
		.ItemHeight(24)
		.TreeItemsSource(&(RootNode->Childrens)) // ������Ҫ��ʾ�Ķ���
		.OnGenerateRow(this, &SNiagaraModuleScriptTree::MakeWidget) // ע������ÿ��TreeItem�Ļص�
		.OnSelectionChanged(this, &SNiagaraModuleScriptTree::OnItemSelected) // ѡ�к�Ļص�
		.SelectionMode(ESelectionMode::Single) // ����ѡ��ģʽ����ѡ
		.OnGetChildren(this, &SNiagaraModuleScriptTree::OnGetChildren); // ע��Tree Children�Ļ�ȡ�ص�
	
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot().AutoHeight().Padding(0, 10, 0, 10)
		[
			SAssignNew(FilterTextBox, SSearchBox) // ������
			.OnTextChanged(this, &SNiagaraModuleScriptTree::OnFilterTextChanged) // ɸѡ���е��ı��б仯ʱ�Ļص��ӿ�
			.DelayChangeNotificationsWhileTyping(false)
		]
				
		+ SVerticalBox::Slot().Padding(FMargin(0.0f, 2.0f, 0.0f, 0.0f)).FillHeight(1.0f)
		[
			// �ô�ֱ�����ؼ�����ʾTreeItem
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
	// ��ȡ���е�Modules
	OnCollectAllModules.ExecuteIfBound(AllModuleAssetDatas);
	
	GenerateFilteredItems();
}

void SNiagaraModuleScriptTree::GenerateFilteredItems()
{
	// ����ɸѡ��ؼ��������ɸѡ�ı�������Modules�����жϣ����������ķ��뵽Tree�С�
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