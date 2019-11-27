// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface.h"
#include "InterfaceClass.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInterfaceClass : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FPSGAME_API IInterfaceClass
{
	GENERATED_IINTERFACE_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	IInterfaceClass() {};
	IInterfaceClass(const FObjectInitializer* ObjectInitializer) {};
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void TempFunc();
};
