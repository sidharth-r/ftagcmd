#include "stdafx.h"
#include "ftaglib.h"

typedef int(__cdecl *LIBPROC_CREATETAGDB)(LPWSTR, LPWSTR);
typedef int(__cdecl *LIBPROC_CREATETAGDBEMPTY)();
typedef int(__cdecl *LIBPROC_DESTROYTAGDB)(TagDB*);
LIBPROC_CREATETAGDBEMPTY ProcCreateTagDBEmptyInstance;


#define DB_NAME "ftag.db"

#ifdef _DEBUG
#define DEBUG_DIR_PATH (LPWSTR)L"D:\\Software\\Visual Studio Projects\\test\\"

#endif

bool saveDB(TagDB*);
TagDB* loadDB();

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

	TagDB* tagdb = loadDB();

	if (argc == 1)
	{
		
		if (tagdb == nullptr)
		{
			wprintf_s(L"No tag database found.");
		}
		else
		{
			wprintf_s(L"Tag Database: %s\n", tagdb->name);
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
		else if (!lstrcmpiA(cmd, "filter"))
		{
			if (argc == 4)
			{
				tagdb->filter(args[0], args[1]);
			}
		}
		else if (!lstrcmpiA(cmd, "debug"))
		{
			tagdb->DEBUG_info();
		}
		else
		{
			wprintf_s(L"invalid command");
		}
	}

	if (tagdb != nullptr)
		saveDB(tagdb);
	//ProcDestroyTagDBInstance(tagdb);
	FreeLibrary(hinstLib);
    return 0;
}

bool saveDB(TagDB* tagDb)		//convert tagdb to writable format
{
	FILE* db = nullptr;
	fopen_s(&db, DB_NAME, "wb");
	
	if (db == nullptr)
		return false;

	fwrite(tagDb, sizeof(TagDB), 1, db);
	fclose(db);

	return true;
}

TagDB* loadDB()
{
	FILE* db = nullptr;
	TagDB* tagDb = (TagDB*)ProcCreateTagDBEmptyInstance();

	fopen_s(&db, DB_NAME, "rb");

	if (db == nullptr)
		return nullptr;

	fread((void*)tagDb, sizeof(TagDB), 1, db);
	return tagDb;
}