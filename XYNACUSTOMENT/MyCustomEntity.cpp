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
//----- MyCustomEntity.cpp : Implementation of MyCustomEntity
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "MyCustomEntity.h"

//-----------------------------------------------------------------------------
Adesk::UInt32 MyCustomEntity::kCurrentVersionNumber =1 ;

//-----------------------------------------------------------------------------
ACRX_DXF_DEFINE_MEMBERS (
	MyCustomEntity, AcDbEntity,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent, 
	AcDbProxyEntity::kNoOperation, MYCUSTOMENTITY,
XYNACUSTOMENTAPP
|Product Desc:     A description for your object
|Company:          Your company name
|WEB Address:      Your company WEB site address
)

//-----------------------------------------------------------------------------
MyCustomEntity::MyCustomEntity () : AcDbEntity () {
}

MyCustomEntity::~MyCustomEntity () {
}

//-----------------------------------------------------------------------------
//----- AcDbObject protocols
//- Dwg Filing protocol
Acad::ErrorStatus MyCustomEntity::dwgOutFields (AcDbDwgFiler *pFiler) const {
	assertReadEnabled () ;
	//----- Save parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dwgOutFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be saved first
	if ( (es =pFiler->writeUInt32 (MyCustomEntity::kCurrentVersionNumber)) != Acad::eOk )
		return (es) ;
	//----- Output params
	//.....

	pFiler->writeItem(m_PtA);
	pFiler->writeItem(m_PtB);
	pFiler->writeItem(m_PtAB);
	pFiler->writeItem(m_PtBA);

	return (pFiler->filerStatus ()) ;
}

Acad::ErrorStatus MyCustomEntity::dwgInFields (AcDbDwgFiler *pFiler) {
	assertWriteEnabled () ;
	//----- Read parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dwgInFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be read first
	Adesk::UInt32 version =0 ;
	if ( (es =pFiler->readUInt32 (&version)) != Acad::eOk )
		return (es) ;
	if ( version > MyCustomEntity::kCurrentVersionNumber )
		return (Acad::eMakeMeProxy) ;
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < MyCustomEntity::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params
	//.....

	if (version >= 1 /*&& version <= endVersion*/) pFiler->readItem(&m_PtA);
	if (version >= 1 /*&& version <= endVersion*/) pFiler->readItem(&m_PtB);
	if (version >= 1 /*&& version <= endVersion*/) pFiler->readItem(&m_PtAB);
	if (version >= 1 /*&& version <= endVersion*/) pFiler->readItem(&m_PtBA);


	return (pFiler->filerStatus ()) ;
}

//-----------------------------------------------------------------------------
//----- AcDbEntity protocols
Adesk::Boolean MyCustomEntity::subWorldDraw (AcGiWorldDraw *mode) {
	assertReadEnabled () ;
	// Bounding Polyline
	AcGePoint3d pts[4];
	pts[0] = m_PtA;
	pts[1] = m_PtAB;
	pts[2] = m_PtB;
	pts[3] = m_PtBA;
	mode->subEntityTraits().setSelectionMarker(1); // Mark 1
	mode->subEntityTraits().setColor(1); // Red
	mode->geometry().polygon(4, pts);

	// Entity's Text
	mode->subEntityTraits().setSelectionMarker(2); // Mark 2
	mode->subEntityTraits().setColor(256);	// ByLayer

	AcGiTextStyle style;
	style.setFileName(_T("txt.shx"));
	style.setBigFontFileName(_T(""));
	style.setTextSize(25);
	style.loadStyleRec();

	AcGePoint3d txtPt((m_PtB.x + m_PtA.x) / 2.0, (m_PtB.y + m_PtA.y) / 2.0, m_PtA.z);

	mode->geometry().text(txtPt, AcGeVector3d::kZAxis, (m_PtAB - m_PtA),
		m_Text, m_Text.GetLength(), Adesk::kFalse, style);

	return Adesk::kTrue;
}


//Adesk::UInt32 MyCustomEntity::subSetAttributes (AcGiDrawableTraits *traits) {
//	assertReadEnabled () ;
//	return (AcDbEntity::subSetAttributes (traits)) ;
//}

	//- Osnap points protocol
Acad::ErrorStatus MyCustomEntity::subGetOsnapPoints (
	AcDb::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const AcGePoint3d &pickPoint,
	const AcGePoint3d &lastPoint,
	const AcGeMatrix3d &viewXform,
	AcGePoint3dArray &snapPoints,
	AcDbIntArray &geomIds) const
{
	assertReadEnabled () ;
	switch (osnapMode) {
	case AcDb::kOsModeEnd:
		snapPoints.append(m_PtA);
		snapPoints.append(m_PtAB);
		snapPoints.append(m_PtB);
		snapPoints.append(m_PtBA);
		break;

	case AcDb::kOsModeMid:
		snapPoints.append(m_PtA + ((m_PtAB - m_PtA).length() / 2.0)*((m_PtAB - m_PtA).normalize()));
		snapPoints.append(m_PtAB + ((m_PtB - m_PtAB).length() / 2.0)*((m_PtB - m_PtAB).normalize()));
		snapPoints.append(m_PtB + ((m_PtBA - m_PtB).length() / 2.0)*((m_PtBA - m_PtB).normalize()));
		snapPoints.append(m_PtBA + ((m_PtA - m_PtBA).length() / 2.0)*((m_PtA - m_PtBA).normalize()));
		break;

	case AcDb::kOsModeCen:
		snapPoints.append(AcGePoint3d((m_PtB.x + m_PtAB.x + m_PtBA.x + m_PtA.x) / 4.0,
			(m_PtB.y + m_PtAB.y + m_PtBA.y + m_PtA.y) / 4.0, m_PtA.z));
		break;
	}
	return (Acad::eOk);
}

//Acad::ErrorStatus MyCustomEntity::subGetOsnapPoints (
//	AcDb::OsnapMode osnapMode,
//	Adesk::GsMarker gsSelectionMark,
//	const AcGePoint3d &pickPoint,
//	const AcGePoint3d &lastPoint,
//	const AcGeMatrix3d &viewXform,
//	AcGePoint3dArray &snapPoints,
//	AcDbIntArray &geomIds,
//	const AcGeMatrix3d &insertionMat) const
//{
//	assertReadEnabled () ;
//	return (AcDbEntity::subGetOsnapPoints (osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds, insertionMat)) ;
//}

//- Grip points protocol
Acad::ErrorStatus MyCustomEntity::subGetGripPoints (
	AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds
) const {
	assertReadEnabled () ;
	//----- This method is never called unless you return eNotImplemented 
	//----- from the new getGripPoints() method below (which is the default implementation)
	gripPoints.append(m_PtA); // Grip 0
	gripPoints.append(m_PtAB); // Grip 1
	gripPoints.append(m_PtB); // Grip 2
	gripPoints.append(m_PtBA); // Grip 3
	gripPoints.append(AcGePoint3d((m_PtB.x + m_PtA.x) / 2.0, (m_PtB.y + m_PtA.y) / 2.0, m_PtA.z)); // Grip 4

	return (Acad::eOk);
}

Acad::ErrorStatus MyCustomEntity::subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) {
	assertWriteEnabled () ;
	//----- This method is never called unless you return eNotImplemented 
	//----- from the new moveGripPointsAt() method below (which is the default implementation)
	for (int i = 0; i < indices.length(); i++) {
		int idx = indices.at(i);
		// For A and center point
		if (idx == 0 || idx == 4) m_PtA += offset;
		// For AB and center point
		if (idx == 1 || idx == 4) m_PtAB += offset;
		// For B and center point
		if (idx == 2 || idx == 4) m_PtB += offset;
		// For BA and center point
		if (idx == 3 || idx == 4) m_PtBA += offset;
	}
	return (Acad::eOk);
}

//Acad::ErrorStatus MyCustomEntity::subGetGripPoints (
//	AcDbGripDataPtrArray &grips, const double curViewUnitSize, const int gripSize, 
//	const AcGeVector3d &curViewDir, const int bitflags
//) const {
//	assertReadEnabled () ;
//
//	//----- If you return eNotImplemented here, that will force AutoCAD to call
//	//----- the older getGripPoints() implementation. The call below may return
//	//----- eNotImplemented depending of your base class.
//	return (AcDbEntity::subGetGripPoints (grips, curViewUnitSize, gripSize, curViewDir, bitflags)) ;
//}

//Acad::ErrorStatus MyCustomEntity::subMoveGripPointsAt (
//	const AcDbVoidPtrArray &gripAppData, const AcGeVector3d &offset,
//	const int bitflags
//) {
//	assertWriteEnabled () ;
//
//	//----- If you return eNotImplemented here, that will force AutoCAD to call
//	//----- the older getGripPoints() implementation. The call below may return
//	//----- eNotImplemented depending of your base class.
//	return (AcDbEntity::subMoveGripPointsAt (gripAppData, offset, bitflags)) ;
//}

AcGePoint3d MyCustomEntity::get_m_PtA(void) const
{
	assertReadEnabled();
	return (m_PtA);
}

Acad::ErrorStatus MyCustomEntity::put_m_PtA(AcGePoint3d newVal)
{
	assertWriteEnabled();
	m_PtA = newVal;
	return (Acad::eOk);
}

AcGePoint3d MyCustomEntity::get_m_PtB(void) const
{
	assertReadEnabled();
	return (m_PtB);
}

Acad::ErrorStatus MyCustomEntity::put_m_PtB(AcGePoint3d newVal)
{
	assertWriteEnabled();
	m_PtB = newVal;
	return (Acad::eOk);
}

void MyCustomEntity::get_m_Text(CString & text) const
{
	assertReadEnabled();
	text.Format(_T("%s"), m_Text);
}

Acad::ErrorStatus MyCustomEntity::put_m_Text(LPCTSTR text)
{
	assertWriteEnabled();
	m_Text.Format(_T("%s"), text);
	return (Acad::eOk);
}

AcGePoint3d MyCustomEntity::get_m_PtAB(void) const
{
	assertReadEnabled();
	return (m_PtAB);
}

Acad::ErrorStatus MyCustomEntity::put_m_PtAB(AcGePoint3d newVal)
{
	assertWriteEnabled();
	m_PtAB = newVal;
	return (Acad::eOk);
}

AcGePoint3d MyCustomEntity::get_m_PtBA(void) const
{
	assertReadEnabled();
	return (m_PtBA);
}

Acad::ErrorStatus MyCustomEntity::put_m_PtBA(AcGePoint3d newVal)
{
	assertWriteEnabled();
	m_PtBA = newVal;
	return (Acad::eOk);
}
