#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Http.h"
#include "HttpModule.h"
#include "HTTPComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHTTPComponentDelegate, bool, result);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CYGNUS_API UHTTPComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHTTPComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FHttpModule* Http;

	FString apiKey = FString(TEXT("AIzaSyBVT22t-x2H76119AHG8SgPU0_A0U-N1uA"));
	FString idToken;
	FString userId;

	FString AuthStatus = FString("Unauthenticated");

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> FirebaseHTTPRequest(FString httpUrl, FString verb, FString data = "");

	bool VerifyUsername(FString username);
	bool VerifyEmail(FString email);
	bool VerifyPassword(FString password);
	
	UFUNCTION(BlueprintCallable)
	void FirebaseEmailPasswordSignUp(FString username, FString email, FString password, APlayerController* PlayerController);
	
	UFUNCTION(BlueprintCallable)
	void FirebaseEmailPasswordAuth(FString identity, FString password, APlayerController* PlayerController);

	void FirebaseSendPwdReset(FString email);
	void FirebaseVerifyPwdReset(FString code);
	void FirebaseConfirmPwdReset(FString code, FString password);
	void FirebaseChangeEmail(FString email);
	void FirebaseChangePassword(FString password);
	void FirebaseUpdateUserProfile(FString displayName = "", FString photoUrl = "");
	void FirebaseGetUserProfile();
	void FirebaseSendEmailVerification();
	void FirebaseConfirmEmailVerification(FString code);
	void FirebaseDeleteAccount();

	void FirebaseGet(FString path);
	void FirebaseCreate(FString name, FString path, FString data);
	void FirebaseDelete(FString path);
	void FirebaseList(FString path);

	UPROPERTY()
	bool result;
	
	UPROPERTY(BlueprintAssignable)
    FHTTPComponentDelegate OnRequestComplete;
};
