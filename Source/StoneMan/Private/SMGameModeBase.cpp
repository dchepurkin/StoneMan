// Created by DChepurkin

#include "SMGameModeBase.h"

#include "SMPlayerCharacter.h"
#include "SMPlayerController.h"

ASMGameModeBase::ASMGameModeBase()
{
	DefaultPawnClass = ASMPlayerCharacter::StaticClass();
	PlayerControllerClass = ASMPlayerController::StaticClass();
}
