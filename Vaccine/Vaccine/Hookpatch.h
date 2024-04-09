#ifndef HOOKPATCH_H
#define HOOKPATCH_H



namespace hookpatch
{
	std::wstring s2ws(const std::string& str);
}

class Hookpatch
{
protected:
	BYTE* _gateway;
	BYTE* _stolen_bytes;
	BYTE* _src;
	BYTE* _dst;
	uintptr_t _len;

public:
	virtual bool detour(BYTE* src, BYTE* dst, const uintptr_t len) = 0;
	virtual BYTE * trampHook(BYTE* src, BYTE* dst, const uintptr_t len) = 0;
	virtual void trampUnhook() = 0;
};

#endif