// SGLLoft.h: Schnittstelle für die Klasse SGLLoft.
//
//////////////////////////////////////////////////////////////////////

/**
  *@author Enrico Reimer
  */

#if !defined(AFX_SGLLOFT_H__667F42E2_A2EF_4C4D_A4C3_F4DDBAAC7DB0__INCLUDED_)
#define AFX_SGLLOFT_H__667F42E2_A2EF_4C4D_A4C3_F4DDBAAC7DB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "sglpolygonobj.h"

/**
  *@author Enrico Reimer
  */
class SGLLoft : public SGLPolygonObj
{
public:
	SGLLoft(SGLPolygon &Flaeche,SGLVektor loft=SGLVektor(0,0,1),SGLMaterial *Material=0,unsigned short mode=3);
	virtual ~SGLLoft();

};

#endif // !defined(AFX_SGLLOFT_H__667F42E2_A2EF_4C4D_A4C3_F4DDBAAC7DB0__INCLUDED_)
