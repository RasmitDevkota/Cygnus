#include "Firebase.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/OutputDeviceDebug.h"
#include "Http.h"

UFirebase::UFirebase()
{
	Http = &FHttpModule::Get();

	UE_LOG(LogTemp, Warning, TEXT("Firebase instantiated"));
}

// Utility Functions

bool UFirebase::VerifyUsername(FString username) {
	FRegexPattern usernamePattern("^[a-zA-Z0-9_-]{6,20}$");
	FRegexMatcher usernameMatcher(usernamePattern, username);

	return usernameMatcher.FindNext();
}

bool UFirebase::VerifyEmail(FString email) {
	FRegexPattern emailPattern("^([a-z0-9_\\.-]+)@([\\da-z\\.-]+)\\.([a-z\\.]{2,5})$");
	FRegexMatcher emailMatcher(emailPattern, email);

	return emailMatcher.FindNext();
}

bool UFirebase::VerifyPassword(FString password) {
	FRegexPattern passwordPattern("^(?=^\\S*$)(?=[A-Z]*)(?=[a-z]*)(?=[0-9]*)(?=[#?!@$%^&*-]*).{8,20}$");
	FRegexMatcher passwordMatcher(passwordPattern, password);

	return passwordMatcher.FindNext();
}

// Authentication Functions

void UFirebase::FirebaseEmailPasswordSignUp(FString username, FString email, FString password, APlayerController* PlayerController) {
	bool verifyU = VerifyUsername(username);
	bool verifyE = VerifyEmail(email);
	bool verifyP = VerifyPassword(password);

	if (verifyU && verifyE && verifyP) {
		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FirebaseHTTPRequest(emailPasswordSignUp + apiKey, "POST", "{\"email\":\"" + email + "\",\"password\":\"" + password + "\",\"returnSecureToken\":true}");

		Request->OnProcessRequestComplete().BindLambda([&](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
	    {
	        TSharedPtr<FJsonObject> JsonObject;
	        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	        UE_LOG(LogTemp, Warning, TEXT("Response received"));

	        const FString status = bWasSuccessful ? "Success" : "Failure";
	        UE_LOG(LogTemp, Warning, TEXT("Response status : %s"), *FString(status));

	        const FString responseString = Response->GetContentAsString();
	        UE_LOG(LogTemp, Warning, TEXT("Response String: %s"), *FString(responseString));

	        if (FJsonSerializer::Deserialize(Reader, JsonObject)) {
	        	UE_LOG(LogTemp, Warning, TEXT("Response deserialized"));

		        idToken = JsonObject->GetStringField("idToken");
		        userId = JsonObject->GetStringField("localId");
	        } else {
	            UE_LOG(LogTemp, Error, TEXT("Failed to deserialize HTTP response JSON"));
	        }
	    });

		Request->ProcessRequest();

		FirebaseUpdateUserProfile(username);

		FString userDocPath = "users/${userId}";
		FString userDocData = "{\"fields\": {\"email\":{\"stringValue\":\"" + email + "\"},\"displayName\":{\"stringValue\": \"" + username + "\"}, \"id\":{\"stringValue\": \"" + userId + "\"}}}";
		
		FirebaseCreate(userId, userDocPath, userDocData);
	} else {
		FString verifyUStr = verifyU ? "Success" : "Failure";
		FString verifyEStr = verifyE ? "Success" : "Failure";
		FString verifyPStr = verifyP ? "Success" : "Failure";

		UE_LOG(LogTemp, Warning, TEXT("VerifyUsername returned: %s"), *FString(verifyUStr));
		UE_LOG(LogTemp, Warning, TEXT("VerifyEmail returned: %s"), *FString(verifyEStr));
		UE_LOG(LogTemp, Warning, TEXT("VerifyPassword returned: %s"), *FString(verifyPStr));
	}
}

void UFirebase::FirebaseEmailPasswordAuth(FString identity, FString password, APlayerController* PlayerController) {
	// Is this optimal really? Maybe consider changing order?

	bool verifyU = VerifyUsername(identity);
	bool verifyE = VerifyEmail(identity);
	bool verifyP = VerifyPassword(password);

	if (verifyE && verifyP) {
    	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FirebaseHTTPRequest(emailPasswordAuthUrl + apiKey, "POST", "{\"email\":\"" + identity + "\",\"password\":\"" + password + "\",\"returnSecureToken\":true}");

    	Request->OnProcessRequestComplete().BindLambda([&](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
        {
            TSharedPtr<FJsonObject> JsonObject;
            TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
    
            UE_LOG(LogTemp, Warning, TEXT("Response received"));
    
            const FString status = bWasSuccessful ? "Success" : "Failure";
            UE_LOG(LogTemp, Warning, TEXT("Response status : %s"), *FString(status));
    
            const FString responseString = Response->GetContentAsString();
            UE_LOG(LogTemp, Warning, TEXT("Response String: %s"), *FString(responseString));
    
            if (FJsonSerializer::Deserialize(Reader, JsonObject)) {
                UE_LOG(LogTemp, Warning, TEXT("Response deserialized"));

            	if (JsonObject->HasField("idToken"))
            	{
	                idToken = JsonObject->GetStringField("idToken");
            		userId = JsonObject->GetStringField("localId");

            		this->AuthStatus = "Authenticated";

            		OnAuthRequestComplete.Broadcast(true);
                } else {
                	UE_LOG(LogTemp, Warning, TEXT("Could not find field 'idToken'"));

            		OnAuthRequestComplete.Broadcast(false);
                }
            } else {
                UE_LOG(LogTemp, Error, TEXT("Failed to deserialize HTTP response JSON"));

            	OnAuthRequestComplete.Broadcast(false);
            }
        });
        
    	Request->ProcessRequest();
    } else if (verifyU && verifyP) {
    	FString getUrl = "https://firestore.googleapis.com/v1beta1/projects/my-scrap-project/databases/(default)/documents/emails/" + identity + "?key=";
    	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FirebaseHTTPRequest(getUrl + apiKey, "GET");

    	Request->OnProcessRequestComplete().BindLambda([&](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
        {
            TSharedPtr<FJsonObject> JsonObject;
            TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

            UE_LOG(LogTemp, Warning, TEXT("Response received"));

            const FString status = bWasSuccessful ? "Success" : "Failure";
            UE_LOG(LogTemp, Warning, TEXT("Response status : %s"), *FString(status));

            const FString responseString = Response->GetContentAsString();
            UE_LOG(LogTemp, Warning, TEXT("Response String: %s"), *FString(responseString));

            if (FJsonSerializer::Deserialize(Reader, JsonObject)) {
                UE_LOG(LogTemp, Warning, TEXT("Response deserialized"));

            	if (JsonObject->HasField("email"))
            	{
            		FString email = JsonObject->GetStringField("email");
            		
	                FirebaseEmailPasswordAuth(email, password, PlayerController);
            	} else
            	{
            		UE_LOG(LogTemp, Warning, TEXT("Could not find field 'email'."));

            		OnAuthRequestComplete.Broadcast(false);
            	}
            } else {
                UE_LOG(LogTemp, Error, TEXT("Failed to deserialize HTTP response JSON"));

            	OnAuthRequestComplete.Broadcast(false);
            }
        });
	
    	Request->ProcessRequest();
    } else {
		FString verifyEStr = verifyE ? "Success" : "Failure";
		FString verifyUStr = verifyU ? "Success" : "Failure";
		FString verifyPStr = verifyP ? "Success" : "Failure";

		UE_LOG(LogTemp, Warning, TEXT("VerifyEmail returned: %s"), *FString(verifyEStr));
		UE_LOG(LogTemp, Warning, TEXT("VerifyUsername returned: %s"), *FString(verifyUStr));
		UE_LOG(LogTemp, Warning, TEXT("VerifyPassword returned: %s"), *FString(verifyPStr));

        OnAuthRequestComplete.Broadcast(false);
    }
}

// User Management

void UFirebase::FirebaseSendPwdReset(FString email) {
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FirebaseHTTPRequest(sendPwdResetUrl + apiKey, "POST", "{\"requestType\":\"PASSWORD_RESET\",\"email\":\"" + email + "\"}");
}

void UFirebase::FirebaseVerifyPwdReset(FString code) {
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FirebaseHTTPRequest(verifyPwdResetUrl + apiKey, "POST", "{\"oobCode\":\"" + code + "\"}");
}

void UFirebase::FirebaseConfirmPwdReset(FString code, FString password) {
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FirebaseHTTPRequest(confirmPwdResetUrl + apiKey, "POST", "{\"oobCode\":\"" + code + "\",\"newPassword\":\"" + password + "\"}");
}

void UFirebase::FirebaseChangeEmail(FString email) {
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FirebaseHTTPRequest(changeEmailUrl + apiKey, "POST", "{\"idToken\":\"" + idToken + "\",\"email\":\"" + email + "\",\"returnSecureToken\":true}");
}

void UFirebase::FirebaseChangePassword(FString password) {
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FirebaseHTTPRequest(changePasswordUrl + apiKey, "POST", "{\"idToken\":\"" + idToken + "\",\"password\":\"" + password + "\",\"returnSecureToken\":true}");
}

void UFirebase::FirebaseUpdateUserProfile(FString displayName, FString photoUrl) {
	FString data = "{\"idToken\":\"" + idToken + "\"";

	if (displayName != "") {
		data += ",\"displayName\":\"" + displayName + "\"";
	}

	if (photoUrl != "") {
		data += ",\"photoUrl\":\"" + photoUrl + "\"";
	}

	data += ",\"returnSecureToken\":true}";

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FirebaseHTTPRequest(updateUserProfileUrl + apiKey, "POST", data);

	Request->OnProcessRequestComplete().BindLambda([&](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
    {
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

        UE_LOG(LogTemp, Warning, TEXT("Response received"));

        const FString status = bWasSuccessful ? "Success" : "Failure";
        UE_LOG(LogTemp, Warning, TEXT("Response status : %s"), *FString(status));

        const FString responseString = Response->GetContentAsString();
        UE_LOG(LogTemp, Warning, TEXT("Response String: %s"), *FString(responseString));

        if (FJsonSerializer::Deserialize(Reader, JsonObject)) {
            UE_LOG(LogTemp, Warning, TEXT("Response deserialized"));

        	FString userDisplayName = JsonObject->GetStringField("displayName");
        	FString userPhotoUrl = JsonObject->GetStringField("photoUrl");

        	UE_LOG(LogTemp, Warning, TEXT("displayName: %s, photoUrl: %s"), *FString(userDisplayName), *FString(userPhotoUrl));
        } else {
            UE_LOG(LogTemp, Error, TEXT("Failed to deserialize HTTP response JSON"));
        }
    });

	Request->ProcessRequest();
}

void UFirebase::FirebaseGetUserProfile() {
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FirebaseHTTPRequest(getUserProfileUrl + apiKey, "POST", "{\"idToken\":\"" + idToken + "\"}");
}

void UFirebase::FirebaseSendEmailVerification() {
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FirebaseHTTPRequest(sendEmailVerificationUrl + apiKey, "POST", "data");
}

void UFirebase::FirebaseConfirmEmailVerification(FString code) {
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FirebaseHTTPRequest(confirmEmailVerificationUrl + apiKey, "POST", "{\"oobCode\":\"" + code + "\"}");
}

void UFirebase::FirebaseDeleteAccount() {
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FirebaseHTTPRequest(deleteAccountUrl + apiKey, "POST", "data");
}

// Basic Firebase Functions

void UFirebase::FirebaseGet(FString path) {
	FString getUrl = "https://firestore.googleapis.com/v1beta1/projects/my-scrap-project/databases/(default)/documents/" + path + "?key=";
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FirebaseHTTPRequest(getUrl + apiKey, "GET");

	Request->OnProcessRequestComplete().BindLambda([&](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
	    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	    UE_LOG(LogTemp, Warning, TEXT("Response received"));

	    const FString status = bWasSuccessful ? "Success" : "Failure";
	    UE_LOG(LogTemp, Warning, TEXT("Response status : %s"), *FString(status));

	    const FString responseString = Response->GetContentAsString();
	    UE_LOG(LogTemp, Warning, TEXT("Response String: %s"), *FString(responseString));

	    if (FJsonSerializer::Deserialize(Reader, JsonObject)) {
	    	UE_LOG(LogTemp, Warning, TEXT("Response deserialized"));

	    	FDocument result = FDocument(*JsonObject.Get());

	    	// OnGetDocumentRequestComplete.Broadcast(result);
	    } else {
	        UE_LOG(LogTemp, Error, TEXT("Failed to deserialize HTTP response JSON"));
	    }
	});
	
	Request->ProcessRequest();
}

void UFirebase::FirebaseCreate(FString name, FString path, FString data) {
	FString createUrl = "https://firestore.googleapis.com/v1beta1/projects/my-scrap-project/databases/(default)/documents/" + path + "/placeholder/..?documentId=" + name + "&key=";
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FirebaseHTTPRequest(createUrl + apiKey, "POST", data);

	Request->OnProcessRequestComplete().BindLambda([&](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
    {
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

        UE_LOG(LogTemp, Warning, TEXT("Response received"));

        const FString status = bWasSuccessful ? "Success" : "Failure";
        UE_LOG(LogTemp, Warning, TEXT("Response status : %s"), *FString(status));

        const FString responseString = Response->GetContentAsString();
        UE_LOG(LogTemp, Warning, TEXT("Response String: %s"), *FString(responseString));

        if (FJsonSerializer::Deserialize(Reader, JsonObject)) {
            UE_LOG(LogTemp, Warning, TEXT("Response deserialized"));
        } else {
            UE_LOG(LogTemp, Error, TEXT("Failed to deserialize HTTP response JSON"));
        }
    });
	
	Request->ProcessRequest();
}

void UFirebase::FirebaseUpdate(FString path, FString data)
{
	FString updateUrl = "https://firestore.googleapis.com/v1beta1/projects/my-scrap-project/databases/(default)/documents/" + path + "?key=";
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FirebaseHTTPRequest(updateUrl + apiKey, "PATCH");

	Request->OnProcessRequestComplete().BindLambda([&](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		UE_LOG(LogTemp, Warning, TEXT("Response received"));

		const FString status = bWasSuccessful ? "Success" : "Failure";
		UE_LOG(LogTemp, Warning, TEXT("Response status : %s"), *FString(status));

		const FString responseString = Response->GetContentAsString();
		UE_LOG(LogTemp, Warning, TEXT("Response String: %s"), *FString(responseString));

		if (FJsonSerializer::Deserialize(Reader, JsonObject)) {
			UE_LOG(LogTemp, Warning, TEXT("Response deserialized"));

			OnUpdateDocumentComplete.Broadcast(true);
		} else {
			UE_LOG(LogTemp, Error, TEXT("Failed to deserialize HTTP response JSON"));
		}
	});
	
	Request->ProcessRequest();
}

void UFirebase::FirebaseDelete(FString path) {
	FString deleteUrl = "https://firestore.googleapis.com/v1beta1/projects/my-scrap-project/databases/(default)/documents/" + path + "?key=";
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FirebaseHTTPRequest(deleteUrl + apiKey, "DELETE", "");

	Request->OnProcessRequestComplete().BindLambda([&](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
    {
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

        UE_LOG(LogTemp, Warning, TEXT("Response received"));

        const FString status = bWasSuccessful ? "Success" : "Failure";
        UE_LOG(LogTemp, Warning, TEXT("Response status : %s"), *FString(status));

        const FString responseString = Response->GetContentAsString();
        UE_LOG(LogTemp, Warning, TEXT("Response String: %s"), *FString(responseString));

        if (FJsonSerializer::Deserialize(Reader, JsonObject)) {
            UE_LOG(LogTemp, Warning, TEXT("Response deserialized"));
        } else {
            UE_LOG(LogTemp, Error, TEXT("Failed to deserialize HTTP response JSON"));
        }
    });
	
	Request->ProcessRequest();
}

void UFirebase::FirebaseList(FString path) {
	FString listUrl = "https://firestore.googleapis.com/v1beta1/projects/my-scrap-project/databases/(default)/documents/" + path + "?key=";
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FirebaseHTTPRequest(listUrl + apiKey, "GET", "");

	UE_LOG(LogTemp, Warning, TEXT("ListURL: %s"), *FString(listUrl + apiKey));

	Request->OnProcessRequestComplete().BindLambda([&](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
    {
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

        UE_LOG(LogTemp, Warning, TEXT("Response received"));

        const FString status = bWasSuccessful ? "Success" : "Failure";
        UE_LOG(LogTemp, Warning, TEXT("Response status : %s"), *FString(status));

        const FString responseString = Response->GetContentAsString();
        UE_LOG(LogTemp, Warning, TEXT("Response String: %s"), *FString(responseString));

        if (FJsonSerializer::Deserialize(Reader, JsonObject)) {
            UE_LOG(LogTemp, Warning, TEXT("Response deserialized"));
        	
        	TArray<FDocument> result;

        	if (JsonObject->HasField("documents"))
        	{
        		TArray<TSharedPtr<FJsonValue>> Documents = JsonObject->GetArrayField("documents");

				for (int i = 0; i < Documents.Num(); i++)
				{
					FJsonObject* DocumentObject = Documents[i]->AsObject().Get();
					result.Add(FDocument(*DocumentObject));
				}
        	}
        	
			OnListDocumentsRequestComplete.Broadcast(result);
        } else {
            UE_LOG(LogTemp, Error, TEXT("Failed to deserialize HTTP response JSON"));
        }
    });
	
	Request->ProcessRequest();
}

// HTTP Functions

TSharedRef<IHttpRequest, ESPMode::ThreadSafe> UFirebase::FirebaseHTTPRequest(FString httpUrl, FString verb, FString data) {
	UE_LOG(LogTemp, Warning, TEXT("Processing %s request"), *FString(verb));

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	
	Request->SetURL(httpUrl);
	Request->SetVerb(verb);
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));

	if (data != "") {
		UE_LOG(LogTemp, Warning, TEXT("Setting data..."));
	
		Request->SetContentAsString(data);
	} else {
		UE_LOG(LogTemp, Warning, TEXT("No data to send, skipped setting"));
	}

	UE_LOG(LogTemp, Warning, TEXT("Finished setting up request"))

	return Request;
}