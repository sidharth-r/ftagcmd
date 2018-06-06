// ftagcmd.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ftaglib.h"

typedef int(__cdecl *LIBPROC_CREATETAGGROUP)(LPWSTR);
typedef int(__cdecl *LIBPROC_CREATETAG)(LPWSTR,tab_group*);

int main()
{
	HINSTANCE hinstLib = LoadLibrary(TEXT("ftaglib.dll"));
	if (hinstLib == NULL)
		return 0;
	LIBPROC_CREATETAGGROUP ProcCreateTabGroupInstance = (LIBPROC_CREATETAGGROUP)GetProcAddress(hinstLib, "CreateTagGroupInstance");
	LIBPROC_CREATETAG ProcCreateTabInstance = (LIBPROC_CREATETAG)GetProcAddress(hinstLib, "CreateTagInstance");
	if (ProcCreateTabGroupInstance == NULL || ProcCreateTabInstance == NULL)
	{
		FreeLibrary(hinstLib);
		return 0;
	}

	
	wchar_t name[] = L"new";
	tab_group* tgroup = (tab_group*)ProcCreateTabGroupInstance(name);
	wprintf(tgroup->get_name());

	//tag* newtag = (tag*)ProcCreateTabInstance((LPWSTR)L"newtag", tgroup);

	tgroup->add_tag((LPWSTR)L"newtag");

	printf("%d",tgroup->get_tag_count());

	vector<tag> tags = tgroup->get_tags();
	for (unsigned int i = 0; i < tags.size(); i++)
	{
		wprintf(L"\n%s %s", tags[i].name, tags[i].group->get_name());
	}

	FreeLibrary(hinstLib);
    return 0;
}

