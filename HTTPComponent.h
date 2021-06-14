#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HttpModule.h"
#include "Document.h"
#include "ItemStruct.h"
#include "HTTPComponent.generated.h"

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAuthRequestCompleteDelegate, bool, result);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetDocumentRequestCompleteDelegate, struct FDocument&, result);
// DECLARE_DELEGATE_OneParam(FListDocumentsRequestCompleteDelegate, TArray<FDocument>);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
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

	FString apiKey = "AIzaSyBVT22t-x2H76119AHG8SgPU0_A0U-N1uA";
	FString idToken;
	FString userId;

	FString emailPasswordSignUp = "https://identitytoolkit.googleapis.com/v1/accounts:signUp?key=";
    FString emailPasswordAuthUrl = "https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword?key=";
	FString sendPwdResetUrl = "https://identitytoolkit.googleapis.com/v1/accounts:sendOobCode?key=";
	FString verifyPwdResetUrl = "https://identitytoolkit.googleapis.com/v1/accounts:resetPassword?key=";
	FString confirmPwdResetUrl = "https://identitytoolkit.googleapis.com/v1/accounts:resetPassword?key=";
	FString changeEmailUrl = "https://identitytoolkit.googleapis.com/v1/accounts:update?key=";
	FString changePasswordUrl = "https://identitytoolkit.googleapis.com/v1/accounts:update?key=";
	FString updateUserProfileUrl = "https://identitytoolkit.googleapis.com/v1/accounts:update?key=";
	FString getUserProfileUrl = "https://identitytoolkit.googleapis.com/v1/accounts:lookup?key=";
	FString sendEmailVerificationUrl = "https://identitytoolkit.googleapis.com/v1/accounts:sendOobCode?key=";
	FString confirmEmailVerificationUrl = "https://identitytoolkit.googleapis.com/v1/accounts:update?key=";
	FString deleteAccountUrl = "https://identitytoolkit.googleapis.com/v1/accounts:delete?key=";
	
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
	
	UPROPERTY(BlueprintAssignable)
    // FAuthRequestCompleteDelegate OnAuthRequestComplete;

	// UPROPERTY(BlueprintAssignable)
	// FGetDocumentRequestCompleteDelegate OnGetDocumentRequestComplete;

	// UPROPERTY(BlueprintAssignable)
	// FListDocumentsRequestCompleteDelegate OnListDocumentsRequestComplete;
};
