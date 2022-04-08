// Fill out your copyright notice in the Description page of Project Settings.
#include "WebSocketGameInstance.h"
#include "WebSocketsModule.h"
#include <string>
#include <iostream>

void UWebSocketGameInstance::Init() {

	Super::Init();

	if (!FModuleManager::Get().IsModuleLoaded("WebSockets")) {
		FModuleManager::Get().LoadModule("WebSockets");
	}

	WebSocket = FWebSocketsModule::Get().CreateWebSocket("ws://localhost:5500");

	// Event handlers
	WebSocket->OnConnected().AddLambda([]() {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Successfully connected");
		});

	WebSocket->OnConnectionError().AddLambda([](const FString& Error) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Connection lost " + Error);
		});

	WebSocket->OnClosed().AddLambda([](int32 StatusCode, const FString& Reason, bool bWasClean) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, bWasClean ? FColor::Green : FColor::Red, "Connection closed " + Reason);
		});

	// Receiving message
	WebSocket->OnMessage().AddLambda([this](const FString& MessageString) {
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, MessageString);

		//Check if we are in puzzle1
		FString Trigger1 = "Trigger1";
		const FString& Trigger1Ref = Trigger1;
		bool isTrigger1 = MessageString.Equals(Trigger1Ref);

		//Check if we are in puzzle2
		FString Trigger2 = "Trigger2";
		const FString& Trigger2Ref = Trigger2;
		bool isTrigger2 = MessageString.Equals(Trigger2Ref);

		//Check if we are in puzzle3
		FString Trigger3 = "Trigger3";
		const FString& Trigger3Ref = Trigger3;
		bool isTrigger3 = MessageString.Equals(Trigger3Ref);

		//Check if we are in control room
		FString controlRoom = "ControlRoom";
		const FString& controlRoomRef = controlRoom;
		bool iscontrolRoom = MessageString.Equals(controlRoomRef);

		if (isTrigger1) {
			InControlRoom = false;
			InPuzzle1 = true;
			GoToPuzzle1();
			OutputText = "Im in puzzle1 woho :)";
		}

		if (isTrigger2) {
			InControlRoom = false;
			InPuzzle2 = true;
			GoToPuzzle2();
			OutputText = "Im in puzzle2 woho :)";
		}

		if (isTrigger3) {
			InControlRoom = false;
			InPuzzle3 = true;
			GoToPuzzle3();
			OutputText = "Im in puzzle3 woho :)";
		}

		if (iscontrolRoom) {
			InControlRoom = true;
			InPuzzle1 = false;
			InPuzzle2 = false;
			InPuzzle3 = false;

			GoToControlRoom();
			OutputText = "Im in control room woho :)";
		}

		//Check if player triggers puzzles below
		//Trigger puzzle1
		FString TriggerPuzzle1 = "TriggerPuzzle1";
		const FString& TriggerPuzzle1Ref = TriggerPuzzle1;
		bool isTriggerPuzzle1 = MessageString.Equals(TriggerPuzzle1Ref);

		//Trigger puzzle2
		FString TriggerPuzzle2 = "TriggerPuzzle2";
		const FString& TriggerPuzzle2Ref = TriggerPuzzle2;
		bool isTriggerPuzzle2 = MessageString.Equals(TriggerPuzzle2Ref);
		
		//Trigger puzzle3
		FString TriggerPuzzle3 = "TriggerPuzzle3";
		const FString& TriggerPuzzle3Ref = TriggerPuzzle3;
		bool isTriggerPuzzle3 = MessageString.Equals(TriggerPuzzle3Ref);
		
		if (isTriggerPuzzle1)
		{
			TriggerPuzzle1event();
		}

		if (isTriggerPuzzle2)
		{
			TriggerPuzzle2event();
		}

		if (isTriggerPuzzle3)
		{
			TriggerPuzzle3event();
		}

		//Check if the player sent a sequence of integes by
		//checking if server message is convertable to integers

		if (MessageString.IsNumeric())
		{
			int code = FCString::Atoi(*MessageString);
			
			if (InPuzzle1)
				SendPlayerInputPuzzle1(MessageString);
			if (InPuzzle2)
				SendPlayerInputPuzzle2(MessageString);
			if (InPuzzle3)
				SendPlayerInputPuzzle3(MessageString);
		}


		//If(convertable)
		//send the entered code to level blueprint and compare

	});

	// Sending message
	WebSocket->OnMessageSent().AddLambda([](const FString& MessageString) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Magenta, "Sent message: " + MessageString);
	});

	WebSocket->Connect();
}

void UWebSocketGameInstance::Shutdown() {

	if (WebSocket->IsConnected()) {
		WebSocket->Close();
	}

	Super::Shutdown();
}

void UWebSocketGameInstance::GoToPuzzle1()
{
	DelegatePuzzle1.Broadcast(69, "");
}

void UWebSocketGameInstance::GoToPuzzle2()
{
	DelegatePuzzle2.Broadcast(69, "");
}

void UWebSocketGameInstance::GoToPuzzle3()
{
	DelegatePuzzle3.Broadcast(69, "");
}

void UWebSocketGameInstance::GoToControlRoom()
{
	ControlRoom.Broadcast(69, "");
}

void UWebSocketGameInstance::TriggerPuzzle1event()
{
	DelegateTriggerPuzzle1.Broadcast(69, "");
}

void UWebSocketGameInstance::TriggerPuzzle2event()
{
	DelegateTriggerPuzzle2.Broadcast(69, "");
}

void UWebSocketGameInstance::TriggerPuzzle3event()
{
	DelegateTriggerPuzzle3.Broadcast(69, "");
}

void UWebSocketGameInstance::SendPlayerInputPuzzle1(FString text)
{
	DelegateSendPlayerInputPuzzle1.Broadcast(420, text);
}

void UWebSocketGameInstance::SendPlayerInputPuzzle2(FString text)
{
	DelegateSendPlayerInputPuzzle2.Broadcast(420, text);
}

void UWebSocketGameInstance::SendPlayerInputPuzzle3(FString text)
{
	DelegateSendPlayerInputPuzzle3.Broadcast(420, text);
}