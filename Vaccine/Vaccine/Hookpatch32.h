#ifndef HOOKPATCH32_H
#define HOOKPATCH32_H

#include "Hookpatch.h"

#define PATCH_SIZE_32			(5)
#define RELATIVE_JMP			(0xE9)


class Hookpatch32 : Hookpatch
{
protected:
	BYTE* _stolen_bytes;
	BYTE* _patch_bytes;
	BYTE* _src;
	BYTE* _dst;
	uintptr_t _len;

public:

	Hookpatch32();
	~Hookpatch32();

	void init_base_hook(BYTE* src, BYTE* dst, const uintptr_t len) override;
	BYTE* initTrampHook(BYTE* src, BYTE* dst, const uintptr_t len) override;

	void hook() override;
	void unhook() override;
};

#endif