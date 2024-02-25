// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <future>

#include "Protocol.pb.h"
#include "ClientPacketHandler.h"

#include "Base.h"
#include "ThreadManager.h"

#include "TypeLessValue.h"
#include "ItemCode.h"

#include "UI_Manager.h"
#include "Sound_Manager.h"

#define _CLIENT

#ifdef UNICODE                                                                                      
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else                                                                                                    
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
 
// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
