#include <windows.h>

typedef HMODULE (WINAPI *FpGetModuleHandleW)(LPCWSTR);
typedef FARPROC (WINAPI *FpGetProcAddress)(HMODULE, LPCSTR);
typedef BOOL (WINAPI *FpUpdatePerUserSystemParameters)(DWORD, BOOL);
typedef HANDLE (WINAPI *FpOpenProcess)(DWORD, BOOL, DWORD);
typedef BOOL (WINAPI *FpOpenProcessToken)(HANDLE, DWORD, PHANDLE);
typedef BOOL (WINAPI *FpImpersonateLoggedOnUser)(HANDLE);
typedef BOOL (WINAPI *FpRevertToSelf)(VOID);
typedef BOOL (WINAPI *FpCloseHandle)(HANDLE);

typedef struct {
	DWORD pid_;
	TCHAR advapi32_[64];
	CHAR impersonateLoggedOnUser_[32];
	CHAR revertToSelf_[32];
	CHAR openProcessToken_[32];
	FpGetModuleHandleW pGetModuleHandle;
	FpGetProcAddress pGetProcAddress;
	FpUpdatePerUserSystemParameters pUpdate;
	FpOpenProcess pOpenProcess;
	FpCloseHandle pCloseHandle;
} InjectInfo;

class FixScancodeMap {
private:
	typedef struct {
		DWORD header1;
		DWORD header2;
		DWORD count;
		DWORD entry[1];
	} ScancodeMap;

private:
	static const DWORD s_fixEntryNum;
	static const DWORD s_fixEntry[];

private:
	InjectInfo m_info;

private:
	int acquirePrivileges();
	DWORD getWinLogonPid();
	int injectThread(DWORD dwPID);
	int update();

public:
	FixScancodeMap();
	~FixScancodeMap();

	int fix();
	int restore();
};