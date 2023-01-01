// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IAssetTools.h"
#include "Modules/ModuleManager.h"
#include "Toolkits/SelectModuleToolkit.h"

class FNiagaraAutoLodModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

protected:
	void RegisterAssetMenuAction();
	void UnRegisterAssetMenuAction();
	
private:
	TArray<TSharedRef<IAssetTypeActions>> RegisteredAssetTypeActions;
};
