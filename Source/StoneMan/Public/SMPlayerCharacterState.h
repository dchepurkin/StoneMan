// Created by DChepurkin

#pragma once

class ASMPlayerCharacter;

class SMPlayerCharacterState
{
public:
	explicit SMPlayerCharacterState(ASMPlayerCharacter* PlayerCharacter = nullptr)
		: PlayerCharacter(PlayerCharacter) {}

private:
	ASMPlayerCharacter* PlayerCharacter;
};
