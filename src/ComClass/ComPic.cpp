/*
 * File name: ComPic.h.
 * Function:
 * Author: Ylonge from MCL-ZJU.
 * Date: 2015/10/6.
 */

#include <assert.h>
#include <string.h>

#include "ComPic.h"

ComPic::ComPic()
{
}

ComPic::~ComPic()
{
}

void ComPic::ParamInit( unsigned int uiWidth, unsigned int uiHeight )
{
  assert( uiWidth > 0 && uiHeight > 0 );
  m_uiWidth = uiWidth;
  m_uiHeight = uiHeight;
}

void ComPic::Create()
{
  m_pucCompY = new unsigned char [ m_uiWidth * m_uiHeight ];
  m_pucCompU = new unsigned char [ m_uiWidth * m_uiHeight >> 2 ];
  m_pucCompV = new unsigned char [ m_uiWidth * m_uiHeight >> 2 ];
}

void ComPic::Destroy()
{
  if ( m_pucCompY )
  {
    delete [] m_pucCompY;
  }
  if ( m_pucCompY )
  {
    delete [] m_pucCompU;
  }
  if ( m_pucCompY )
  {
    delete [] m_pucCompV;
  }
}

void ComPic::Clear()
{
  ::memset( m_pucCompY, 0, ( m_uiWidth  * m_uiHeight ) * sizeof( unsigned char ));
  ::memset( m_pucCompU, 0, ( m_uiWidth  * m_uiHeight >> 2 ) * sizeof( unsigned char ));
  ::memset( m_pucCompV, 0, ( m_uiWidth  * m_uiHeight >> 2 ) * sizeof( unsigned char ));
}