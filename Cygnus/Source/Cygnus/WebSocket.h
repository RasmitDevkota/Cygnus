#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "IWebSocket.h"
#include "WebSocket.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWebSocketConnectedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWebSocketMessageSentDelegate, FString, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWebSocketMessageReceivedDelegate, FString, Message);

UCLASS()
class CYGNUS_API UWebSocket : public UObject
{
	GENERATED_BODY()

public:
	UWebSocket();

	TSharedPtr<IWebSocket> Socket;

	UFUNCTION(BlueprintCallable)
	bool Send(FString Message);

	UFUNCTION(BlueprintCallable)
	void Close();

	UPROPERTY(BlueprintAssignable)
	FWebSocketConnectedDelegate OnWebSocketConnected;

	UPROPERTY(BlueprintAssignable)
	FWebSocketMessageSentDelegate OnWebSocketMessageSent;

	UPROPERTY(BlueprintAssignable)
	FWebSocketMessageReceivedDelegate OnWebSocketMessageReceived;
};
