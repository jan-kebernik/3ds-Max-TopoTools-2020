//**************************************************************************/
// Copyright (c) 1998-2018 Autodesk, Inc.
// All rights reserved.
// 
// Use of this software is subject to the terms of the Autodesk license 
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.
//**************************************************************************/
// DESCRIPTION: Appwizard generated plugin
// AUTHOR: 
//***************************************************************************/

#include "MXTopoTools.h"
#include "MXTopoToolsDesc.h"

#define MXTopoTools_CLASS_ID	Class_ID(0x37e845c3, 0x52d16fa8)


class MXTopoTools : public GUP
{
public:
	//Constructor/Destructor
	MXTopoTools();
	virtual ~MXTopoTools();

	// GUP Methods
	virtual DWORD     Start();
	virtual void      Stop();
	virtual DWORD_PTR Control(DWORD parameter);
	virtual void      DeleteThis();

	// Loading/Saving
	virtual IOResult Save(ISave* isave);
	virtual IOResult Load(ILoad* iload);
};



class MXTopoTools2020ClassDesc : public ClassDesc2 
{
public:
	virtual int           IsPublic() override                       { return TRUE; }
	virtual void*         Create(BOOL /*loading = FALSE*/) override { return new MXTopoTools(); }
	virtual const TCHAR * ClassName() override                      { return GetString(IDS_CLASS_NAME); }
	virtual SClass_ID     SuperClassID() override                   { return GUP_CLASS_ID; }
	virtual Class_ID      ClassID() override                        { return MXTopoTools_CLASS_ID; }
	virtual const TCHAR*  Category() override                       { return GetString(IDS_CATEGORY); }

	virtual const TCHAR*  InternalName() override                   { return _T("MXTopoTools"); } // Returns fixed parsable name (scripter-visible name)
	virtual HINSTANCE     HInstance() override                      { return hInstance; } // Returns owning module handle


};

ClassDesc2* GetMXTopoToolsDesc()
{
	static MXTopoTools2020ClassDesc MXTopoTools2020Desc;
	return &MXTopoTools2020Desc; 
}

MXTopoTools::MXTopoTools()
{
	// nothing
}

MXTopoTools::~MXTopoTools()
{
	// nothing
}

void MXTopoTools::DeleteThis()
{
	delete this;
}

// Activate and Stay Resident
DWORD MXTopoTools::Start()
{
	//#pragma message(TODO("Do plugin initialization here"))
	//#pragma message(TODO("Return if you want remain loaded or not"))
	return GUPRESULT_KEEP;
}

void MXTopoTools::Stop()
{
	//#pragma message(TODO("Do plugin un-initialization here"))
}

DWORD_PTR MXTopoTools::Control( DWORD /*parameter*/ )
{
	return 0;
}

IOResult MXTopoTools::Save(ISave* /*isave*/)
{
	return IO_OK;
}

IOResult MXTopoTools::Load(ILoad* /*iload*/)
{
	return IO_OK;
}

