#include "pch.h"
#include "BlazeEngineCore/DataStructures/StringViewUTF8.h"
#include "BlazeEngineCore/DataStructures/StringUTF8.h"

namespace Blaze
{
	StringViewUTF8::Iterator::Iterator()
		: ptr(nullptr)
	{
	}
	StringViewUTF8::Iterator::Iterator(const Iterator& i)
		: ptr(i.ptr), value(i.value)
	{
	}
	StringViewUTF8::Iterator::Iterator(const void* ptr, uintMem size)
		: ptr(ptr), value(ptr, size)
	{
	}
	UnicodeChar StringViewUTF8::Iterator::ToUnicode() const
	{
		return value;
	}
	StringViewUTF8::Iterator StringViewUTF8::Iterator::operator++()
	{
		Iterator old = *this;
		if ((*(uint8*)ptr) >> 3 == 0b11110)
			ptr = (byte*)ptr + 4;
		else if ((*(uint8*)ptr) >> 4 == 0b1110)
			ptr = (byte*)ptr + 3;
		else if ((*(uint8*)ptr) >> 5 == 0b110)
			ptr = (byte*)ptr + 2;
		else 
			ptr = (byte*)ptr + 1;
		value = UnicodeChar(ptr, 4);
		return old;
	}
	StringViewUTF8::Iterator StringViewUTF8::Iterator::operator--()
	{
		Iterator old = *this;
		if ((*((byte*)ptr - 1) & 0b10000000) == 0)
			ptr = (byte*)ptr - 1;
		else if ((*((byte*)ptr - 2) & 0b10000000) == 0)
			ptr = (byte*)ptr - 2;
		else if ((*((byte*)ptr - 3) & 0b10000000) == 0)
			ptr = (byte*)ptr - 3;
		else if ((*((byte*)ptr - 4) & 0b10000000) == 0)
			ptr = (byte*)ptr - 4;
		value = UnicodeChar(ptr, 4);
		return old;
	}
	StringViewUTF8::Iterator& StringViewUTF8::Iterator::operator++(int)
	{		
		if (((*(uint8*)ptr) >> 6) == 0)
			ptr = (byte*)ptr + 1;
		else if (((*(uint8*)((byte*)ptr + 1)) >> 4) == 0)
			ptr = (byte*)ptr + 2;
		else if (((*(uint8*)((byte*)ptr + 2)) >> 2) == 0)
			ptr = (byte*)ptr + 3;
		else if (((*(uint8*)((byte*)ptr + 3)) >> 5) == 0)
			ptr = (byte*)ptr + 4;
		value = UnicodeChar(ptr, 4);
		return *this;
	}
	StringViewUTF8::Iterator& StringViewUTF8::Iterator::operator--(int)
	{		
		if ((*((byte*)ptr - 1) & 0b10000000) == 0)
			ptr = (byte*)ptr - 1;
		else if ((*((byte*)ptr - 2) & 0b10000000) == 0)
			ptr = (byte*)ptr - 2;
		else if ((*((byte*)ptr - 3) & 0b10000000) == 0)
			ptr = (byte*)ptr - 3;
		else if ((*((byte*)ptr - 4) & 0b10000000) == 0)
			ptr = (byte*)ptr - 4;
		value = UnicodeChar(ptr, 4);
		return *this;
	}
	bool StringViewUTF8::Iterator::operator<(const Iterator& i) const
	{
		return ptr < i.ptr;
	}
	bool StringViewUTF8::Iterator::operator>(const Iterator& i) const
	{
		return ptr > i.ptr;
	}
	bool StringViewUTF8::Iterator::operator<=(const Iterator& i) const
	{
		return ptr <= i.ptr;
	}
	bool StringViewUTF8::Iterator::operator>=(const Iterator& i) const
	{
		return ptr >= i.ptr;
	}
	bool StringViewUTF8::Iterator::operator==(const Iterator& i) const
	{
		return ptr == i.ptr;
	}
	bool StringViewUTF8::Iterator::operator!=(const Iterator& i) const
	{
		return ptr != i.ptr;
	}
	UnicodeChar StringViewUTF8::Iterator::operator*() const
	{
		return ToUnicode();
	}
	StringViewUTF8::Iterator& StringViewUTF8::Iterator::operator=(const Iterator& i)
	{
		ptr = i.ptr;
		value = i.value;
		return *this;
	}

	StringViewUTF8::StringViewUTF8()
		: buffer(nullptr), bufferSize(0), characterCount(0)
	{
	}
	StringViewUTF8::StringViewUTF8(const StringViewUTF8& s)
		: buffer(s.buffer), bufferSize(s.bufferSize), characterCount(s.characterCount)
	{
	}	
	StringViewUTF8::StringViewUTF8(const char* ptr)
		: buffer(ptr), bufferSize(0), characterCount(0)
	{
		characterCount = ptr == nullptr ? 0 : strlen(ptr);
		bufferSize = characterCount + 1;
	}
	StringViewUTF8::StringViewUTF8(const char* ptr, size_t size)
		: buffer(ptr), bufferSize(size + 1), characterCount(size)
	{
	}
	StringViewUTF8::StringViewUTF8(const StringUTF8& s)
		: buffer(s.Buffer()), bufferSize(s.BufferSize()), characterCount(s.CharacterCount())
	{
	}
	StringViewUTF8::StringViewUTF8(const StringView& s)
		: buffer(s.Ptr()), bufferSize(s.Count() + 1), characterCount(s.Count())
	{
	}
	StringViewUTF8::StringViewUTF8(const String& s)
		: buffer(s.Ptr()), bufferSize(s.Count() + 1), characterCount(s.Count())
	{
	}

	StringViewUTF8::Iterator StringViewUTF8::begin() const
	{
		return Iterator(buffer, bufferSize - 1);
	}
	StringViewUTF8::Iterator StringViewUTF8::end() const
	{
		return Iterator((byte*)buffer + bufferSize - 1, 0);
	}
	StringViewUTF8& StringViewUTF8::operator=(const StringViewUTF8& s)
	{
		buffer = s.buffer;
		bufferSize = s.bufferSize;
		characterCount = s.characterCount;
		return *this;
	}
	bool StringViewUTF8::operator==(const StringViewUTF8& s) const
	{
		if (bufferSize != s.bufferSize)
			return false;
		
		return memcmp(buffer, s.buffer, bufferSize) == 0;
	}
	bool StringViewUTF8::operator!=(const StringViewUTF8& s) const
	{
		if (bufferSize != s.bufferSize)
			return true;

		return memcmp(buffer, s.buffer, bufferSize) != 0;
	}	
}