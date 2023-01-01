// Copyright Epic Games, Inc. All Rights Reserved.

#include "NiagaraAutoLod.h"
#include "ContentBrowserModule.h"
#include "NiagaraSystem.h"
#include "Toolkits/SelectModuleToolkit.h"

#define LOCTEXT_NAMESPACE "FNiagaraAutoLodModule"

void FNiagaraAutoLodModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// 注册ContentBrowser中资产右键菜单创建时的回调
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	TArray<FContentBrowserMenuExtender_SelectedAssets>& CBAssetMenuExtenderDelegates = ContentBrowserModule.GetAllAssetViewContextMenuExtenders();
	CBAssetMenuExtenderDelegates.Add(FContentBrowserMenuExtender_SelectedAssets::CreateRaw(this, &FNiagaraAutoLodModule::OnExtendCBAssetSelectionMenu));
	ContentBrowserAssetExtenderDelegateHandle = CBAssetMenuExtenderDelegates.Last().GetHandle();
}

void FNiagaraAutoLodModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	// 移除回调接口
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
	
	// 判断选中的资产是否全都是NiagaraSystem资产
	bool bNiagaraAssetsExtension = true;
	for (const FAssetData& Asset : SelectedAssets)
	{
		if (Asset.AssetClass != UNiagaraSystem::StaticClass()->GetFName())
		{
			bNiagaraAssetsExtension = false;
			break;
		}
	}

	// 若是，则在“GetAssetActions”扩展点后面加入一个菜单选项
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

// 菜单按钮回调函数，筛选NiagaraSystem资产，并作为参数传给弹出的界面
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
	// 弹窗
	TSharedRef<FSelectModuleToolkit> SelectModuleToolkit = MakeShareable(new FSelectModuleToolkit);
	SelectModuleToolkit->Initialize(NiagaraAssets);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FNiagaraAutoLodModule, NiagaraAutoLod)