#include "WebSocket.h"
#include "WebSocketsModule.h"

UWebSocket::UWebSocket()
{
	FWebSocketsModule& Module = FModuleManager::LoadModuleChecked<FWebSocketsModule>(TEXT("WebSockets"));

	Socket = FWebSocketsModule::Get().CreateWebSocket("wss://CSSA-Learn.dralientech.repl.co", "wss");
	
	Socket->OnConnected().AddLambda([&]() -> void {
		UE_LOG(LogTemp, Warning, TEXT("Connected to WebSocket!"));

		OnWebSocketConnected.Broadcast();
	});
    
	Socket->OnConnectionError().AddLambda([&](const FString& Error) -> void {
		UE_LOG(LogTemp, Warning, TEXT("Error connecting to WebSocket: %s"), *Error);
	});
    
	Socket->OnMessageSent().AddLambda([&](const FString& Message) -> void {
		UE_LOG(LogTemp, Warning, TEXT("<- %s"), *Message);

		OnWebSocketMessageSent.Broadcast(Message);
	});
    
	Socket->OnMessage().AddLambda([&](const FString& Message) -> void {
		UE_LOG(LogTemp, Warning, TEXT("-> %s"), *Message);

		OnWebSocketMessageReceived.Broadcast(Message);
	});
    
	Socket->OnClosed().AddLambda([&](int32 StatusCode, const FString& Reason, bool bWasClean) -> void {
		UE_LOG(LogTemp, Warning, TEXT("Closed WebSocket connection %s"), bWasClean ? *FString("successfully") : *FString("badly"));
	});
     
	Socket->Connect();
}

bool UWebSocket::Send(FString Message)
{
	if (!Socket->IsConnected()) return false;
	
	Socket->Send(Message);
	
	return true;
}

void UWebSocket::Close()
{
	// Send closing message
	// Socket->Send("");
	
	Socket->Close();
}
