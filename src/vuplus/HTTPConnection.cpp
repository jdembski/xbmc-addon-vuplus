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

#include "HTTPConnection.h"
#include "client.h"
#include "../platform/threads/mutex.h"
#include "../platform/util/timeutils.h"


#include "../platform/sockets/tcp.h"

using namespace std;
using namespace PLATFORM;
using namespace ADDON;
HTTPConnection::HTTPConnection() 
{
  m_iConnectTimeout = 5;
  m_bIsConnected = false;
}

HTTPConnection::HTTPConnection(CStdString strUsername, CStdString strPassword, CStdString strHostname, int iPort) 
{
  HTTPConnection();
  m_strUsername = strUsername;
  m_strPassword = strPassword;
  m_strHostname = strHostname;
  m_iPort       = iPort;
}

HTTPConnection::~HTTPConnection() 
{
  if (m_bIsConnected)
    Abort();
}

bool HTTPConnection::Connect()
{
  {
    CLockObject lock(m_mutex);

    if (m_bIsConnected)
      return true;

    if (!m_socket)
    {
      XBMC->Log(LOG_ERROR, "%s - failed to connect to the backend (couldn't create a socket)", __FUNCTION__);
      return false;
    }

    XBMC->Log(LOG_DEBUG, "%s - connecting to '%s', port '%d'", __FUNCTION__, m_strHostname.c_str(), m_iPort);

    CTimeout timeout(m_iConnectTimeout);
    while (!m_socket->IsOpen() && timeout.TimeLeft() > 0)
    {
      if (!m_socket->Open(timeout.TimeLeft()))
        CEvent::Sleep(100);
    }

    if (!m_socket->IsOpen())
    {
      XBMC->Log(LOG_ERROR, "%s - failed to connect to the backend (%s)", __FUNCTION__, m_socket->GetError().c_str());
      return false;
    }

    m_bIsConnected = true;
    XBMC->Log(LOG_DEBUG, "%s - connected to '%s', port '%d'", __FUNCTION__, m_strHostname.c_str(), m_iPort);
  }

  if (!SendGreeting())
  {
    XBMC->Log(LOG_ERROR, "%s - failed to sent greeting to the backend", __FUNCTION__);
    Close();
    return false;
  }

  if (!Auth())
  {
    XBMC->Log(LOG_ERROR, "%s - failed to authenticate", __FUNCTION__);
    Close();
    return false;
  }
  return true;
}

void HTTPConnection::Close()
{
  CLockObject lock(m_mutex);
  m_bIsConnected = false;

  if(m_socket && m_socket->IsOpen())
    m_socket->Close();
}

void HTTPConnection::Abort(void)
{
  CLockObject lock(m_mutex);
  m_bIsConnected = false;

  if(m_socket && m_socket->IsOpen())
    m_socket->Shutdown();
}


bool HTTPConnection::Auth(void)
{
  if (m_strUsername.empty())
  {
    XBMC->Log(LOG_DEBUG, "CHTSPConnection - %s - no username set. not authenticating", __FUNCTION__);
    return true;
  }
}

bool HTTPConnection::IsConnected(void)
{
  CLockObject lock(m_mutex);
  return m_bIsConnected && m_socket && m_socket->IsOpen();
}

