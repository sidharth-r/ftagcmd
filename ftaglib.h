#pragma once
#include "stdafx.h"

using namespace std;

class tab_group;

class tag
{
public:
	wchar_t name[256];
	tab_group* group;
};

class tag;

class tab_group
{
public:
	wchar_t name[256];
	int tag_count;
	virtual LPWSTR get_name() = 0;
	virtual void add_tag(LPWSTR) = 0;
	virtual int get_tag_count() = 0;
	virtual vector<tag> get_tags() = 0;
};