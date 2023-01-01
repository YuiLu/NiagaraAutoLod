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
	// ί��ʵ������Ϊ�ؼ�������
	FOnCollectAllModules OnCollectAllModules;
	FOnModuleSelected OnModuleSeleted;
	// �б�ؼ�
	TSharedPtr< SListView< TSharedPtr<FNiagaraModuleScriptNode> > > ListView;
	
	// ������ؼ��͹��ܽӿ�
	TSharedPtr<SSearchBox> FilterTextBox;
	void OnFilterTextChanged(const FText& InFilterText);
	
	// All Modules
	TArray<TSharedPtr<FNiagaraModuleScriptNode>> ModuleScriptsToShow; // �ṹ�干��ָ�����飬���� ListView ����
	TArray<FAssetData> AllModuleAssetDatas; // FAssetData ���飬���ڹ����ṹ�壬�������� Dialog ֮��ͨ�ŵ�����
	
	TSharedRef<ITableRow> OnGenerateRow(TSharedPtr<FNiagaraModuleScriptNode> InItem, const TSharedRef<STableViewBase>& OwnerTable);
	void OnSelectionChanged(TSharedPtr< FNiagaraModuleScriptNode > InSelectedItem, ESelectInfo::Type SelectInfo);
	void GenerateFilteredItems();
};