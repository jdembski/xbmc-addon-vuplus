#pragma once

/*
 *      Copyright (C) 2005-2011 Team XBMC
 *      http://www.xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#include "../platform/threads/mutex.h"
#include "../platform/util/StdString.h"

namespace PLATFORM
{
  class CTcpConnection;
}

class HTTPConnection
{
public:
  HTTPConnection();
  HTTPConnection(CStdString strUsername, CStdString strPassword, CStdString strHostname, int iPort, CStdString strURL);
  ~HTTPConnection();

  bool        Connect(void);
  void        Close();
  void        Abort();
  bool        IsConnected(void);

  void        SetUsername(std::string strUsername);
  void        SetPassword(std::string strPassword);
  void        SetURL(std::string strURL);
  CStdString  getResponse(); 

private:
  bool SendGreeting(void);
  bool Auth(void);

  PLATFORM::CMutex          m_mutex;
  PLATFORM::CTcpConnection* m_socket;

  bool                      m_bIsConnected;
  std::string               m_strUsername;
  std::string               m_strPassword;
  std::string               m_strHostname;
  std::string               m_strURL;
  int                       m_iPort;
  int                       m_iConnectTimeout;
};
