// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/WarriorHeroController.h"

#include "EnhancedInputSubsystems.h"

UEnhancedInputLocalPlayerSubsystem* AWarriorHeroController::GetHeroEnhancedInputLocalPlayerSubsystem() const
{
	if (const ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		return LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	}
	return nullptr;
}
