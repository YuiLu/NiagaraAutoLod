#pragma once

#include "NiagaraSystem.h"
#include "Templates/SharedPointer.h"

#include "Widgets/SWindow.h"


static const FName TabId("Select Module");
static const FName TestTab("Test Tab");

class FSelectModuleToolkit
{
public:
	// Constructor &. Destructor
	FSelectModuleToolkit() : NiagaraAsset(nullptr) {}
	~FSelectModuleToolkit() {}
	
	void Initialize(UNiagaraSystem* NiagaraAsset);
	
private:
	UNiagaraSystem* NiagaraAsset;
};