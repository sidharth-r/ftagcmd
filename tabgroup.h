#pragma once

class tab_group
{
public:
	wchar_t name[256];
	int tag_count;
	virtual LPWSTR get_name() = 0;
};
