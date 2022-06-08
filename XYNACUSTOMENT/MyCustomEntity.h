// (C) Copyright 2002-2007 by Autodesk, Inc. 
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
//----- MyCustomEntity.h : Declaration of the MyCustomEntity
//-----------------------------------------------------------------------------
#pragma once

#ifdef XYNACUSTOMENT_MODULE
#define DLLIMPEXP __declspec(dllexport)
#else
//----- Note: we don't use __declspec(dllimport) here, because of the
//----- "local vtable" problem with msvc. If you use __declspec(dllimport),
//----- then, when a client dll does a new on the class, the object's
//----- vtable pointer points to a vtable allocated in that client
//----- dll. If the client dll then passes the object to another dll,
//----- and the client dll is then unloaded, the vtable becomes invalid
//----- and any virtual calls on the object will access invalid memory.
//-----
//----- By not using __declspec(dllimport), we guarantee that the
//----- vtable is allocated in the server dll during the ctor and the
//----- client dll does not overwrite the vtable pointer after calling
//----- the ctor. And, since we expect the server dll to remain in
//----- memory indefinitely, there is no problem with vtables unexpectedly
//----- going away.
#define DLLIMPEXP
#endif

//-----------------------------------------------------------------------------
#include "dbmain.h"

//-----------------------------------------------------------------------------
class DLLIMPEXP MyCustomEntity : public AcDbEntity {

public:
	ACRX_DECLARE_MEMBERS(MyCustomEntity) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	MyCustomEntity () ;
	virtual ~MyCustomEntity () ;

	//----- AcDbObject protocols
	//- Dwg Filing protocol
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const ;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler) ;

	//----- AcDbEntity protocols
	//- Graphics protocol
protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode) ;
	//virtual Adesk::UInt32 subSetAttributes (AcGiDrawableTraits *traits) ;

	//- Osnap points protocol
public:
	virtual Acad::ErrorStatus subGetOsnapPoints (
		AcDb::OsnapMode osnapMode,
		Adesk::GsMarker gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds) const ;
	//virtual Acad::ErrorStatus subGetOsnapPoints (
	//	AcDb::OsnapMode osnapMode,
	//	Adesk::GsMarker gsSelectionMark,
	//	const AcGePoint3d &pickPoint,
	//	const AcGePoint3d &lastPoint,
	//	const AcGeMatrix3d &viewXform,
	//	AcGePoint3dArray &snapPoints,
	//	AcDbIntArray &geomIds,
	//	const AcGeMatrix3d &insertionMat) const ;

	//- Grip points protocol
	virtual Acad::ErrorStatus subGetGripPoints (AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds) const ;
	virtual Acad::ErrorStatus subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) ;
	//virtual Acad::ErrorStatus subGetGripPoints (
	//	AcDbGripDataPtrArray &grips, const double curViewUnitSize, const int gripSize, 
	//	const AcGeVector3d &curViewDir, const int bitflags) const ;
	//virtual Acad::ErrorStatus subMoveGripPointsAt (const AcDbVoidPtrArray &gripAppData, const AcGeVector3d &offset, const int bitflags) ;

protected:
	// Point A
	AcGePoint3d m_PtA;
public:
	AcGePoint3d get_m_PtA(void) const;
	Acad::ErrorStatus put_m_PtA(AcGePoint3d newVal);
protected:
	// Point B
	AcGePoint3d m_PtB;
public:
	AcGePoint3d get_m_PtB(void) const;
	Acad::ErrorStatus put_m_PtB(AcGePoint3d newVal);
protected:
	// Text
	CString m_Text;
public:
	void get_m_Text(CString& text) const;
	Acad::ErrorStatus put_m_Text(LPCTSTR text);
protected:
	// Point AB
	AcGePoint3d m_PtAB;
public:
	AcGePoint3d get_m_PtAB(void) const;
	Acad::ErrorStatus put_m_PtAB(AcGePoint3d newVal);
protected:
	// Point BA
	AcGePoint3d m_PtBA;
public:
	AcGePoint3d get_m_PtBA(void) const;
	Acad::ErrorStatus put_m_PtBA(AcGePoint3d newVal);

} ;

#ifdef XYNACUSTOMENT_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(MyCustomEntity)
#endif
