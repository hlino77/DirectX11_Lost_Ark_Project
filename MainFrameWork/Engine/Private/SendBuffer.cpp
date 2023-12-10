#include "SendBuffer.h"
#include "ObjectPool.h"
#include "ServerMemory.h"
#include "ThreadManager.h"
/*----------------
	SendBuffer
-----------------*/

SendBuffer::SendBuffer(SendBufferChunkRef owner, BYTE* buffer, uint32 allocSize)
	: _owner(owner), _buffer(buffer), _allocSize(allocSize)
{
}

SendBuffer::~SendBuffer()
{
}

void SendBuffer::Close(uint32 writeSize)
{
	ASSERT_CRASH(_allocSize >= writeSize);
	_writeSize = writeSize;
	_owner->Close(writeSize);
}

/*--------------------
	SendBufferChunk
--------------------*/

SendBufferChunk::SendBufferChunk()
{
}

SendBufferChunk::~SendBufferChunk()
{
}

void SendBufferChunk::Reset()
{
	_open = false;
	_usedSize = 0;
}

SendBufferRef SendBufferChunk::Open(uint32 allocSize)
{
	ASSERT_CRASH(allocSize <= SEND_BUFFER_CHUNK_SIZE);
	ASSERT_CRASH(_open == false);

	if (allocSize > FreeSize())
		return nullptr;

	_open = true;
	return ObjectPool<SendBuffer>::MakeShared(shared_from_this(), Buffer(), allocSize);
}

void SendBufferChunk::Close(uint32 writeSize)
{
	ASSERT_CRASH(_open == true);
	_open = false;
	_usedSize += writeSize;
}

/*---------------------
	SendBufferManager
----------------------*/
IMPLEMENT_SINGLETON(SendBufferManager)


SendBufferManager::SendBufferManager()
{
}

SendBufferRef SendBufferManager::Open(uint32 size)
{
	ThreadManager::MTLS& tTLS = ThreadManager::GetInstance()->Get_TLS(this_thread::get_id());


	if (tTLS.LSendBufferChunk == nullptr)
	{
		tTLS.LSendBufferChunk = Pop(); // WRITE_LOCK
		tTLS.LSendBufferChunk->Reset();
	}

	ASSERT_CRASH(tTLS.LSendBufferChunk->IsOpen() == false);

	// 다 썼으면 버리고 새거로 교체
	if (tTLS.LSendBufferChunk->FreeSize() < size)
	{
		tTLS.LSendBufferChunk = Pop(); // WRITE_LOCK
		tTLS.LSendBufferChunk->Reset();
	}

	return tTLS.LSendBufferChunk->Open(size);
}

SendBufferChunkRef SendBufferManager::Pop()
{

	{
		WRITE_LOCK;
		if (_sendBufferChunks.empty() == false)
		{
			SendBufferChunkRef sendBufferChunk = _sendBufferChunks.back();
			_sendBufferChunks.pop_back();
			return sendBufferChunk;
		}
	}

	return SendBufferChunkRef(xnew<SendBufferChunk>(), PushGlobal);
}

void SendBufferManager::Push(SendBufferChunkRef buffer)
{
	WRITE_LOCK;
	_sendBufferChunks.push_back(buffer);
}

void SendBufferManager::PushGlobal(SendBufferChunk* buffer)
{
	SendBufferManager::GetInstance()->Push(SendBufferChunkRef(buffer, PushGlobal));
}