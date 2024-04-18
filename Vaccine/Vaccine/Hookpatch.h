#ifndef HOOKPATCH_H
#define HOOKPATCH_H


namespace hookpatch
{
	std::wstring s2ws(const std::string& str);
}

class Hookpatch
{
protected:
	BYTE* _stolen_bytes;
	BYTE* _patch_bytes;
	BYTE* _src;
	BYTE* _dst;
	uintptr_t _len;

public:
	virtual void init_base_hook(BYTE* src, BYTE* dst, const uintptr_t len) = 0;
	virtual BYTE * initTrampHook(BYTE* src, BYTE* dst, const uintptr_t len) = 0;
	virtual void hook() = 0;
	virtual void unhook() = 0;
};
#endif