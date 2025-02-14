// game.h
#pragma once
#include "SDK.hpp"  // Include the main SDK header instead of individual files

class Game {
public:
    static bool GetPlayerHealth(SDK::APawn* pawn, float& health, float& maxHealth) {
        if (!pawn) return false;

        // Cast to the interface
        auto healthInterface = Cast<SDK::IBodycam_Interface>(pawn);
        if (healthInterface) {
            health = healthInterface->GetHealth();
            maxHealth = healthInterface->GetMaxHealth();
            return true;
        }
        return false;
    }

    static bool IsEnemy(SDK::APlayerState* localPlayerState, SDK::APlayerState* otherPlayerState) {
        if (!localPlayerState || !otherPlayerState) return true;

        // Check team information
        return localPlayerState->GetTeam() != otherPlayerState->GetTeam();
    }
};