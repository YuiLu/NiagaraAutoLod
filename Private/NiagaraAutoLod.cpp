// Copyright Epic Games, Inc. All Rights Reserved.

#include "NiagaraAutoLod.h"
#include "ContentBrowserModule.h"
#include "NiagaraSystem.h"
#include "Toolkits/SelectModuleToolkit.h"

#define LOCTEXT_NAMESPACE "FNiagaraAutoLodModule"

void FNiagaraAutoLodModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// ע��ContentBrowser���ʲ��Ҽ��˵�����ʱ�Ļص�
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	TArray<FContentBrowserMenuExtender_SelectedAssets>& CBAssetMenuExtenderDelegates = ContentBrowserModule.GetAllAssetViewContextMenuExtenders();
	CBAssetMenuExtenderDelegates.Add(FContentBrowserMenuExtender_SelectedAssets::CreateRaw(this, &FNiagaraAutoLodModule::OnExtendCBAssetSelectionMenu));
	ContentBrowserAssetExtenderDelegateHandle = CBAssetMenuExtenderDelegates.Last().GetHandle();
}

void FNiagaraAutoLodModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	// �Ƴ��ص��ӿ�
	FContentBrowserModule* ContentBrowserModule = FModuleManager::GetModulePtr<FContentBrowserModule>(TEXT("ContentBrowser"));
	if (ContentBrowserModule)
	{
		TArray<FContentBrowserMenuExtender_SelectedAssets>& CBMenuExtenderDelegates = ContentBrowserModule->GetAllAssetViewContextMenuExtenders();
		CBMenuExtenderDelegates.RemoveAll(
			[this](const FContentBrowserMenuExtender_SelectedAssets& Delegate)
			{
				return Delegate.GetHandle() == ContentBrowserAssetExtenderDelegateHandle;
			}
		);
	}
}

TSharedRef<FExtender> FNiagaraAutoLodModule::OnExtendCBAssetSelectionMenu(const TArray<FAssetData>& SelectedAssets)
{
	TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
	
	// �ж�ѡ�е��ʲ��Ƿ�ȫ����NiagaraSystem�ʲ�
	bool bNiagaraAssetsExtension = true;
	for (const FAssetData& Asset : SelectedAssets)
	{
		if (Asset.AssetClass != UNiagaraSystem::StaticClass()->GetFName())
		{
			bNiagaraAssetsExtension = false;
			break;
		}
	}

	// ���ǣ����ڡ�GetAssetActions����չ��������һ���˵�ѡ��
	if (bNiagaraAssetsExtension)
	{
		MenuExtender->AddMenuExtension(
			"GetAssetActions",
			EExtensionHook::After,
			nullptr,
			FMenuExtensionDelegate::CreateRaw(this, &FNiagaraAutoLodModule::NiagaraAssetMenuExtension, SelectedAssets)
		);
	}
	return MenuExtender.ToSharedRef();
}

void FNiagaraAutoLodModule::NiagaraAssetMenuExtension(FMenuBuilder& MenuBuilder, const TArray<FAssetData> SelectedAssets)
{
	MenuBuilder.AddMenuEntry(
		NSLOCTEXT("AssetTypeActions_Niagara", "ObjectContext_NiagaraAutoLod", "Auto Module"),
		NSLOCTEXT("AssetTypeActions_Niagara", "ObjectContext_NiagaraAutoLodTooltip", "Automatically add module for all emitters in this niagara system."),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.LOD"),
		FUIAction(FExecuteAction::CreateRaw(this, &FNiagaraAutoLodModule::ExtensionButtonClicked, SelectedAssets), FCanExecuteAction())
	);
}

// �˵���ť�ص�������ɸѡNiagaraSystem�ʲ�������Ϊ�������������Ľ���
void FNiagaraAutoLodModule::ExtensionButtonClicked(TArray<FAssetData> SelectedAssets)
{
	TArray<FAssetData> NiagaraAssets;
	for (const FAssetData& Asset : SelectedAssets)
	{
		if (Asset.AssetClass == UNiagaraSystem::StaticClass()->GetFName())
		{
			NiagaraAssets.Add(Asset);
		}
	}

	if (NiagaraAssets.IsEmpty())
	{
		return;
	}
	// ����
	TSharedRef<FSelectModuleToolkit> SelectModuleToolkit = MakeShareable(new FSelectModuleToolkit);
	SelectModuleToolkit->Initialize(NiagaraAssets);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FNiagaraAutoLodModule, NiagaraAutoLod)