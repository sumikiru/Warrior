// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/Weapons/WarriorWeaponBase.h"
#include "ItemExtensionComponentBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WARRIOR_API UItemExtensionComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:
#pragma region GetOwner
	template <class T>
	T* GetOwningItem() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, AActor>::Value, "'T' template parameter to GetItem must be derived from AActor");
		return CastChecked<T>(GetOwner());
	}

	AWarriorWeaponBase* GetOwningWeapon() const
	{
		return GetOwningItem<AWarriorWeaponBase>();
	}
#pragma endregion
	
};
