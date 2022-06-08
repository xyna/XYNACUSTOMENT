// (C) Copyright 2002-2012 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "MyCustomEntity.h"

//-----------------------------------------------------------------------------
#define szRDS _RXST("")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CXYNACUSTOMENTApp : public AcRxDbxApp {

public:
	CXYNACUSTOMENTApp () : AcRxDbxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxDbxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxDbxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}
	


	static void XYNACUSTOMENT_MyCustEnt(void)
	{
		// Input information
		ads_point pt1, pt2;
		if (acedGetPoint(NULL, _T("Set the first point:\n"), pt1) != RTNORM) return;
		if (acedGetCorner(pt1, _T("Set the second point:\n"), pt2) != RTNORM)	return;
		TCHAR buffer[133] = { 0 };
		if (acedGetString(0, _T("Enter the text:\n"), buffer) != RTNORM) return;

		// Setup entity
		MyCustomEntity *pEnt = new MyCustomEntity();
		pEnt->put_m_PtA(asPnt3d(pt1));
		pEnt->put_m_PtAB(AcGePoint3d(pt2[X], pt1[Y], pt1[Z]));
		pEnt->put_m_PtB(asPnt3d(pt2));
		pEnt->put_m_PtBA(AcGePoint3d(pt1[X], pt2[Y], pt2[Z]));
		pEnt->put_m_Text(buffer);



		// Post to Database
		AcDbBlockTable *pBlockTable;
		acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable, AcDb::kForRead);

		AcDbBlockTableRecord *pBlockTableRecord;
		pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);
		pBlockTable->close();

		AcDbObjectId retId = AcDbObjectId::kNull;
		pBlockTableRecord->appendAcDbEntity(retId, pEnt);

		pBlockTableRecord->close();
		pEnt->close();
	}
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CXYNACUSTOMENTApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CXYNACUSTOMENTApp, XYNACUSTOMENT, _MyCustEnt, MyCustEnt, ACRX_CMD_MODAL, NULL)
