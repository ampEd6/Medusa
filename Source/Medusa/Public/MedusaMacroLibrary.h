 #pragma once

#include "CoreMinimal.h"
#include "DrawDebugHelpers.h"

#pragma region Debug Macros

	
/// @brief Draws a debug sphere given a location (FVector)
/// @param Location The location to draw the sphere at
#define DEUBG_SPHERE(Location) if (bEnableDebug) { (GetWorld()) { DrawDebugSphere(GetWorld(), Location, 4, 12, FColor::Red, true, 5, 0); } }

/// @brief Draws a debug arrow given a start and end location (FVector)
/// @param Start The start location of the line
/// @param End The end location of the line
/// @param Color (Optional overload) The color of the line
#define DEBUG_LINE(Start, End) if (bEnableDebug) { if (GetWorld()) { DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 5, 0, 3); } }
#define DEBUG_LINE_COLOR(Start, End, Color) if (bEnableDebug) { if (GetWorld()) { DrawDebugLine(GetWorld(), Start, End, Color, false, 5, 0, 3); } }

#pragma endregion