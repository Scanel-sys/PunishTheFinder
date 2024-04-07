#pragma once

#include "Hookpatch.h"

#define PATCH_SIZE_64			(13)


class Hookpatch64 : Hookpatch
{

public:
	BYTE* trampHook(BYTE* src, BYTE* dst, const uintptr_t len) override;
	void trampUnhook() override;
	bool detour(BYTE* src, BYTE* dst, const uintptr_t len) override;
};