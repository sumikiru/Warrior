// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/WarriorHeroCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "DataAssets/HeroCharacterBasicConfig.h"
#include "EnhancedInputSubsystems.h"
#include "WarriorGameplayTags.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "Components/Input/WarriorInputComponent.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

AWarriorHeroCharacter::AWarriorHeroCharacter()
{
	// 注意：组件创建应保留在构造函数，此时BasicConfig还未加载完成，应该在PostInitializeComponents()中完成
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// 这个不需要SetupAttachment，详细结构见蓝图
	HeroCombatComponent = CreateDefaultSubobject<UHeroCombatComponent>(TEXT("HeroCombatComponent"));
}

void AWarriorHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	/**
	 * 不应使用IsValid()检查，软指针中的IsValid()表示是否已经加载完成。
	 * 同步加载(Synchronous Loading)：
	 * Cons: Blocks game thread.
	 * Pros: Returns loaded object from the loading function
	 * 异步加载(Asynchronous Loading)：
	 * Pros: Loading happens in background
	 * Cons: Loaded assets appears suddenly
	 */
	if (!CharacterStartUpData.IsNull())
	{
		if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.LoadSynchronous())
		{
			LoadedData->GiveToAbilitySystemComponent(WarriorAbilitySystemComponent);
		}
	}
}

void AWarriorHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	checkf(InputConfigDataAsset, TEXT("Input config data asset is not set."));

	// 获取Enhanced Input Local Player Subsystem
	const ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	checkf(InputSubsystem, TEXT("Input subsystem not found!"));

	// 为Enhanced Input Local Player Subsystem添加Input Mapping Context（每个按键对应什么InputAction）
	InputSubsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);

	// 获取Input Component并为该组件绑定输入(为每个InputAction配置实现逻辑)
	UWarriorInputComponent* WarriorInputComponent = CastChecked<UWarriorInputComponent>(PlayerInputComponent);
	WarriorInputComponent->BindNativeInputAction(
		InputConfigDataAsset,
		WarriorGameplayTags::InputTag_Move,
		ETriggerEvent::Triggered,
		this,
		&ThisClass::Input_Move /* 或者 &AWarriorHeroCharacter::Input_Move */);
	WarriorInputComponent->BindNativeInputAction(
		InputConfigDataAsset,
		WarriorGameplayTags::InputTag_Look,
		ETriggerEvent::Triggered,
		this,
		&ThisClass::Input_Look);

	WarriorInputComponent->BindAbilityInputAction(
		InputConfigDataAsset,
		this,
		&ThisClass::Input_AbilityInputPressed,
		&ThisClass::Input_AbilityInputReleased);
}

void AWarriorHeroCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	checkf(BasicConfig, TEXT("Basic Config is not loaded yet!"));
	
	GetCapsuleComponent()->InitCapsuleSize(BasicConfig->CapsuleRadius, BasicConfig->CapsuleHalfHeight);

	bUseControllerRotationPitch = BasicConfig->bUseControllerRotationPitch;
	bUseControllerRotationYaw = BasicConfig->bUseControllerRotationYaw;
	bUseControllerRotationRoll = BasicConfig->bUseControllerRotationRoll;

	CameraBoom->TargetArmLength = BasicConfig->CameraBoomTargetArmLength;
	CameraBoom->SocketOffset = BasicConfig->CameraBoomSocketOffset;
	CameraBoom->bUsePawnControlRotation = BasicConfig->bCameraBoomUsePawnControlRotation;

	FollowCamera->bUsePawnControlRotation = BasicConfig->bFollowCameraUsePawnControlRotation;
	
	GetCharacterMovement()->bOrientRotationToMovement = BasicConfig->bOrientRotationToMovement;
	GetCharacterMovement()->RotationRate = BasicConfig->RotationRate;
	GetCharacterMovement()->MaxWalkSpeed = BasicConfig->MaxWalkSpeed;
	GetCharacterMovement()->BrakingDecelerationWalking = BasicConfig->BrakingDecelerationWalking;
}

void AWarriorHeroCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void AWarriorHeroCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

	if (MovementVector.Y != 0.f)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(ForwardDirection, MovementVector.Y);
	}
	if (MovementVector.X != 0.f)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AWarriorHeroCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (LookAxisVector.X != 0.f)
	{
		AddControllerYawInput(LookAxisVector.X * InputConfigDataAsset->HorizontalSensitivity);
	}
	if (LookAxisVector.Y != 0.f)
	{
		AddControllerPitchInput(LookAxisVector.Y * InputConfigDataAsset->VerticalSensitivity);
	}
}

void AWarriorHeroCharacter::Input_AbilityInputPressed(FGameplayTag InInputTag)
{
	WarriorAbilitySystemComponent->OnAbilityInputPressed(InInputTag);
}

void AWarriorHeroCharacter::Input_AbilityInputReleased(FGameplayTag InInputTag)
{
	WarriorAbilitySystemComponent->OnAbilityInputReleased(InInputTag);
}
