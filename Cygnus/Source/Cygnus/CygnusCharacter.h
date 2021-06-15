#pragma once

#include "CoreMinimal.h"

#include "CygnusGameInstance.h"
#include "Inventory.h"
#include "Firebase.h"
#include "GameFramework/Character.h"
#include "CygnusCharacter.generated.h"

UCLASS(config=Game)
class ACygnusCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	ACygnusCharacter();

	UPROPERTY(BlueprintReadWrite)
	UCygnusGameInstance* CygnusGameInstance;

	UPROPERTY(BlueprintReadWrite)
	UFirebase* FirebaseObject;
	
	UPROPERTY(BlueprintReadWrite)
	UInventory* Inventory;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;
	
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	void ToggleInventoryCall();
};

