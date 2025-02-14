// external.cpp
#include "overlay.h"
#include "memory.h"
#include <iostream>
#include <thread>
#include <vector>

// Include necessary SDK headers
#include "SDK/CoreUObject_classes.hpp"
#include "SDK/Engine_classes.hpp"
#include "SDK/GameMode_structs.hpp"
#include "SDK/Bodycam_Interface_classes.hpp"
#include "SDK/GM_Bodycam_classes.hpp"

class ESP {
public:
    ESP() : mem("Bodycam-Win64-Shipping.exe"), overlay(1920, 1080, L"ESP Overlay") {
        if (!mem.IsProcessRunning()) {
            throw std::runtime_error("Failed to find game process");
        }
        std::cout << "ESP initialized successfully\n";
    }

    void Run() {
        while (!GetAsyncKeyState(VK_END)) {
            overlay.Begin();

            // Get UWorld instance
            auto world = SDK::UWorld::GetWorld();
            if (!world) continue;

            // Get GameInstance
            auto gameInstance = world->OwningGameInstance;
            if (!gameInstance) continue;

            // Get LocalPlayer
            auto localPlayer = gameInstance->LocalPlayers[0];
            if (!localPlayer) continue;

            // Get PlayerController
            auto playerController = localPlayer->PlayerController;
            if (!playerController) continue;

            // Get LocalPawn
            auto localPawn = playerController->AcknowledgedPawn;
            if (!localPawn) continue;

            // Get all players from GameState
            auto gameState = world->GameState;
            if (!gameState) continue;

            // Get player array
            auto& playerArray = gameState->PlayerArray;

            for (int i = 0; i < playerArray.Count(); i++) {
                auto playerState = playerArray[i];
                if (!playerState) continue;

                auto pawn = playerState->PawnPrivate;
                if (!pawn) continue;

                // Skip if it's the local player
                if (pawn == localPawn) continue;

                // Get player location
                auto rootComponent = pawn->RootComponent;
                if (!rootComponent) continue;

                SDK::FVector playerLocation = rootComponent->RelativeLocation;

                // Get camera manager for screen projection
                auto cameraMgr = playerController->PlayerCameraManager;
                if (!cameraMgr) continue;

                SDK::FVector cameraLocation = cameraMgr->GetCameraLocation();
                SDK::FRotator cameraRotation = cameraMgr->GetCameraRotation();
                float fov = cameraMgr->GetFOVAngle();

                // Convert world position to screen position
                SDK::FVector2D screenPos;
                if (playerController->ProjectWorldLocationToScreen(playerLocation, screenPos)) {
                    // Calculate ESP box dimensions based on distance
                    float distance = FVector::Dist(cameraLocation, playerLocation);
                    float boxHeight = 15000.0f / distance;
                    float boxWidth = boxHeight * 0.6f;

                    // Get player health
                    auto healthComponent = pawn->FindComponentByClass<SDK::UHealthComponent>();
                    float health = healthComponent ? healthComponent->GetHealth() : 100.0f;
                    float maxHealth = healthComponent ? healthComponent->GetMaxHealth() : 100.0f;

                    // Draw ESP box
                    overlay.DrawBox(
                        screenPos.X - boxWidth / 2,
                        screenPos.Y - boxHeight,
                        boxWidth,
                        boxHeight,
                        D3DCOLOR_RGBA(255, 0, 0, 255)
                    );

                    // Draw health bar
                    overlay.DrawHealthBar(
                        screenPos.X - boxWidth / 2 - 5.0f,
                        screenPos.Y - boxHeight,
                        5.0f,
                        boxHeight,
                        health,
                        maxHealth
                    );
                }
            }

            overlay.End();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

private:
    Memory mem;
    Overlay overlay;
};

int main() {
    try {
        ESP esp;
        esp.Run();
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}