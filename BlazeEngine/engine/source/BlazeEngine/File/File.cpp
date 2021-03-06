#include "BlazeEngine/File/File.h"
#include "BlazeEngine/Logger/Logger.h"
#include <io.h>
#include <fcntl.h>

namespace Blaze
{	
		File::File()
			: fd(-1)
		{
		}
		File::File(const Path& path, FileOpenMode mode, FileOpenFlags flags, FilePermission perms)
		{
			Open(path, mode, flags);
		}
		File::~File()
		{
			if (fd != -1 && _close(fd) == -1)			
					BLAZE_ERROR_LOG("stdlib", "Failed to close file");
		}

		Result File::Open(const Path& path, FileOpenMode mode, FileOpenFlags flags, FilePermission perms)
		{
			errno_t returned = _sopen_s(&fd, path.GetString().Ptr(), (int)mode | (int)flags | _O_BINARY, _SH_DENYNO, (int)perms);

			if (returned != 0)			
				return Result(Log(LogType::Warning, BLAZE_FILE_NAME, BLAZE_FUNCTION_NAME, BLAZE_FILE_LINE, 
					"stdlib", "_sopen_s failed with error: " + String::Convert(returned)), true);							

			return Result();
		}
		Result File::Close()
		{
			if (_close(fd) == -1)
			{
				fd = -1;

				return Result(Log(LogType::Warning, BLAZE_FILE_NAME, BLAZE_FUNCTION_NAME, BLAZE_FILE_LINE,
					"stdlib", "_close failed."), true);				
			}
			fd = -1;

			return Result();
		}
		size_t File::Size()
		{
			return _filelength(fd);
		}
		size_t File::Read(Buffer& buffer)
		{
			return _read(fd, buffer.Ptr(), buffer.Size());
		}		
		size_t File::Write(BufferView buffer)
		{
			return _write(fd, buffer.Ptr(), buffer.Size());
		}
		inline bool File::IsOpen() const
		{
			return fd >= 0;
		}	
}           