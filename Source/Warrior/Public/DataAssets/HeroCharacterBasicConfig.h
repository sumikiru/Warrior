// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HeroCharacterBasicConfig.generated.h"

/**
 * 
 */
UCLASS()
class WARRIOR_API UHeroCharacterBasicConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Basic")
	bool bUseControllerRotationPitch = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Basic")
	bool bUseControllerRotationYaw = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Basic")
	bool bUseControllerRotationRoll = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Basic|CapsuleComponent")
	float CapsuleRadius = 42.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Basic|CapsuleComponent")
	float CapsuleHalfHeight = 96.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Basic|Camera|CameraBoom")
	float CameraBoomTargetArmLength = 200.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Basic|Camera|CameraBoom")
	FVector CameraBoomSocketOffset = FVector(0.f, 55.f, 65.f);
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Basic|Camera|CameraBoom")
	bool bCameraBoomUsePawnControlRotation = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Basic|Camera|FollowCamera")
	bool bFollowCameraUsePawnControlRotation = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Basic|Movement")
	bool bOrientRotationToMovement = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Basic|Movement")
	FRotator RotationRate = FRotator(0.f, 500.f, 0.f);
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Basic|Movement")
	float MaxWalkSpeed = 400.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Basic|Movement")
	float BrakingDecelerationWalking = 2000.f;
};
