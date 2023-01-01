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
	
	// SListView��ʼ��
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
		
		// ������
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 10, 0, 10)
		[
			SAssignNew(FilterTextBox, SSearchBox)
			.OnTextChanged(this, &SNiagaraModuleScriptList::OnFilterTextChanged) // ɸѡ���е��ı��б仯ʱ�Ļص��ӿ�
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
	// ��ȡ���е�Modules
	OnCollectAllModules.ExecuteIfBound(AllModuleAssetDatas);
	
	GenerateFilteredItems();
}

void SNiagaraModuleScriptList::GenerateFilteredItems()
{
	// ����ɸѡ��ؼ��������ɸѡ�ı�������Modules�����жϣ����������ķ��뵽Tree�С�
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