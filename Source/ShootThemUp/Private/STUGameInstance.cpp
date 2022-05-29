// Shoot Them Up Game. All Rights Reserved

#include "STUGameInstance.h"
#include "Sound/STUBlueprintFuncLib.h"

void USTUGameInstance::ToggleVolume() 
{
    USTUBlueprintFuncLib::ToggleSoundClassValume(MasterSoundClass);
}
