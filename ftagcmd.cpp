#include "stdafx.h"
#include "ftaglib.h"

typedef int(__cdecl *LIBPROC_CREATETAGDB)(LPWSTR, LPWSTR);
typedef int(__cdecl *LIBPROC_CREATETAGDBEMPTY)();
typedef int(__cdecl *LIBPROC_DESTROYTAGDB)(TagDB*);
LIBPROC_CREATETAGDBEMPTY ProcCreateTagDBEmptyInstance;

#ifdef _DEBUG
#define DEBUG_DIR_PATH (LPWSTR)L"D:\\Software\\Visual Studio Projects\\test\\"

#endif

int main(int argc, const char* argv[])
{
	HINSTANCE hinstLib = LoadLibrary(TEXT("ftaglib.dll"));
	if (hinstLib == NULL)
		return 0;

	LIBPROC_CREATETAGDB ProcCreateTagDBInstance = (LIBPROC_CREATETAGDB)GetProcAddress(hinstLib, "CreateTagDbInstance");
	ProcCreateTagDBEmptyInstance = (LIBPROC_CREATETAGDBEMPTY)GetProcAddress(hinstLib, "CreateTagDbEmptyInstance");
	LIBPROC_DESTROYTAGDB ProcDestroyTagDBInstance = (LIBPROC_DESTROYTAGDB)GetProcAddress(hinstLib, "DestroyTagDbInstance");
	if (ProcCreateTagDBInstance == NULL || ProcCreateTagDBEmptyInstance == NULL)
	{
		FreeLibrary(hinstLib);
		return 0;
	}

	TagDB* tagdb = (TagDB*)ProcCreateTagDBEmptyInstance();
	bool dbExists = tagdb->load_db();
	if (!dbExists)
	{
		ProcDestroyTagDBInstance(tagdb);
		tagdb = nullptr;
	}

	if (argc == 1)
	{
		
		if (tagdb == nullptr)
		{
			wprintf_s(L"No tag database found.");
		}
		else
		{
			wprintf_s(L"Tag Database: %s", tagdb->name);
		}		
	}
	else
	{
		vector<LPWSTR> args;
		LPWSTR arg;
		for (int i = 2; i < argc; i++)
		{
			arg = new wchar_t[256];
			mbstowcs_s(nullptr,arg,256,argv[i],257);
			args.push_back(arg);
		}

		const char* cmd = argv[1];
		if (!lstrcmpiA(cmd, "init"))
		{
			if (argc == 3)
			{
				if (tagdb == nullptr)
				{
					LPWSTR dir_path = new wchar_t[MAX_PATH];
					GetCurrentDirectoryW(MAX_PATH, dir_path);
					tagdb = (TagDB*)ProcCreateTagDBInstance(args[0], dir_path);
				}
			}
		}
		else if (!lstrcmpiA(cmd, "addt"))
		{
			if (argc == 4)
			{
				tagdb->add_tag_to_group(args[0], args[1]);
			}
			else if (argc == 5)
			{
				tagdb->add_tag_to_file(args[0], args[1], args[2]);
			}
		}
		else if (!lstrcmpiA(cmd, "addg"))
		{
			if (argc == 3)
			{
				tagdb->add_group(args[0]);
			}
		}
		else if (!lstrcmpiA(cmd, "addf"))
		{
			if (argc == 3)
			{
				tagdb->add_file(args[0]);
			}
		}
		else if (!lstrcmpiA(cmd, "remt"))
		{
			if (argc == 4)
			{
				tagdb->remove_tag_from_group(args[0], args[1]);
			}
			else if (argc == 5)
			{
				tagdb->remove_tag_from_file(args[0], args[1], args[2]);
			}
		}
		else if (!lstrcmpiA(cmd, "remg"))
		{
			if (argc == 3)
			{
				tagdb->remove_group(args[0]);
			}
		}
		else if (!lstrcmpiA(cmd, "filter"))
		{
			if (argc == 4)
			{
				vector<LPWSTR> fnames = tagdb->filter(args[0], args[1]);
				for (int i = 0; i < fnames.size(); i++)
				{
					wprintf_s(L"%s ", fnames[i]);
				}
			}
		}
		else if (!lstrcmpiA(cmd, "infog"))
		{
			if (argc == 3)
			{
				TagGroup* grp = tagdb->get_tag_group(args[0]);
				wprintf_s(L"Tag count : %d\n", grp->get_tag_count());
				vector<Tag> tags = grp->get_tags();
				int pcntr = 0;
				for (vector<Tag>::iterator iter = tags.begin(); iter != tags.end(); iter++)
				{
					if (pcntr == 2)
					{
						wprintf_s(L"%s\n", iter->get_name());
					}
					else
					{
						wprintf_s(L"%s\t", iter->get_name());
					}

					pcntr = (pcntr + 1) % 3;
				}
			}
		}
		else if (!lstrcmpiA(cmd, "infof"))
		{
			if (argc == 3)
			{
				TaggedFile* file = tagdb->get_file(args[0]);
				wprintf_s(L"Tag count : %d\n\n", file->get_tag_count());
				vector<TagGroupSkel> grps = file->get_tag_groups();
				for (vector<TagGroupSkel>::iterator iter = grps.begin(); iter != grps.end(); iter++)
				{
					TagGroup* grp = tagdb->get_tag_group(iter->gid);
					wprintf_s(L"Group: %s\n", grp->get_name());
					int cntr = 0;
					for (vector<long>::iterator it = iter->tids.begin(); it != iter->tids.end(); it++)
					{
						Tag* t = grp->get_tag(*it);
						wprintf_s(L"%s", t->get_name());
						if (cntr < 3)
							wprintf_s(L"\t");
						else
						{
							wprintf_s(L"\n");
							cntr = (cntr + 1) % 3;
						}							
					}
					wprintf_s(L"\n");
				}
			}
		}
		else if (!lstrcmpiA(cmd, "debug"))
		{
			tagdb->DEBUG_info();
		}
		else
		{
			wprintf_s(L"Invalid command.");
		}
	}

	if (tagdb != nullptr)
		tagdb->save_db();
	//ProcDestroyTagDBInstance(tagdb);
	FreeLibrary(hinstLib);
    return 0;
}