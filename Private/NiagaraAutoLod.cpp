// Copyright Epic Games, Inc. All Rights Reserved.

#include "NiagaraAutoLod.h"

#include "AutoModules/NiagaraAutoModules.h"
#include "Templates/SharedPointer.h"

#define LOCTEXT_NAMESPACE "FNiagaraAutoLodModule"

void FNiagaraAutoLodModule::StartupModule()
{
	RegisterAssetMenuAction();
	TSharedRef<FSelectModuleToolkit> SelectModuleToolkit = MakeShareable(new FSelectModuleToolkit);
}

void FNiagaraAutoLodModule::ShutdownModule()
{
	UnRegisterAssetMenuAction();
	TSharedRef<FSelectModuleToolkit> SelectModuleToolkit = MakeShareable(new FSelectModuleToolkit);
}


// Niagara 资产右键菜单按钮 Action 注册
void FNiagaraAutoLodModule::RegisterAssetMenuAction()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	auto Action = MakeShareable(new FNiagaraAutoModules);
	AssetTools.RegisterAssetTypeActions(Action);
	RegisteredAssetTypeActions.Add(Action);
}
void FNiagaraAutoLodModule::UnRegisterAssetMenuAction()
{
	FAssetToolsModule* AssetToolsModule = FModuleManager::GetModulePtr<FAssetToolsModule>("AssetTools");
	if(AssetToolsModule != nullptr)
	{
		IAssetTools& AssetTools = AssetToolsModule->Get();
		for(auto Action:RegisteredAssetTypeActions)
		{
			AssetTools.UnregisterAssetTypeActions(Action);
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FNiagaraAutoLodModule, NiagaraAutoLod)