#include "stdafx.h"

using namespace std;


// Reference: https://en.wikibooks.org/wiki/Algorithm_Implementation/Miscellaneous/Base64

template<class CHARTYPE>
class CBase64
{
public:
	static std::basic_string<CHARTYPE>&& base64Encode(const void* buf, size_t len)
	{
		const static char encodeLookup[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		const static char padCharacter = '=';
		std::basic_string<CHARTYPE> encodedString;
		encodedString.reserve(((len / 3) + (len % 3 > 0)) * 4);
		DWORD temp;
		const BYTE* cursor = (const BYTE*)buf;
		for (size_t idx = 0; idx < len / 3; idx++)
		{
			temp = (*cursor++) << 16; //Convert to big endian
			temp += (*cursor++) << 8;
			temp += (*cursor++);
			encodedString.append(1, encodeLookup[(temp & 0x00FC0000) >> 18]);
			encodedString.append(1, encodeLookup[(temp & 0x0003F000) >> 12]);
			encodedString.append(1, encodeLookup[(temp & 0x00000FC0) >> 6]);
			encodedString.append(1, encodeLookup[(temp & 0x0000003F)]);
		}
		switch (len % 3)
		{
		case 1:
			temp = (*cursor++) << 16; //Convert to big endian
			encodedString.append(1, encodeLookup[(temp & 0x00FC0000) >> 18]);
			encodedString.append(1, encodeLookup[(temp & 0x0003F000) >> 12]);
			encodedString.append(2, padCharacter);
			break;
		case 2:
			temp = (*cursor++) << 16; //Convert to big endian
			temp += (*cursor++) << 8;
			encodedString.append(1, encodeLookup[(temp & 0x00FC0000) >> 18]);
			encodedString.append(1, encodeLookup[(temp & 0x0003F000) >> 12]);
			encodedString.append(1, encodeLookup[(temp & 0x00000FC0) >> 6]);
			encodedString.append(1, padCharacter);
			break;
		}
		return move(encodedString);
	}

	static std::vector<BYTE>&& base64Decode(const std::basic_string<CHARTYPE> input)
	{
		const static TCHAR padCharacter = TEXT('=');
		if (input.length() % 4) //Sanity check
			throw std::runtime_error("Non-Valid base64!");
		size_t padding = 0;
		if (input.length())
		{
			if (input[input.length() - 1] == padCharacter)
				padding++;
			if (input[input.length() - 2] == padCharacter)
				padding++;
		}
		//Setup a vector to hold the result
		std::vector<BYTE> decodedBytes;
		decodedBytes.reserve(((input.length() / 4) * 3) - padding);
		DWORD temp = 0; //Holds decoded quanta
		std::basic_string<CHARTYPE>::const_iterator cursor = input.begin();
		while (cursor < input.end())
		{
			for (size_t quantumPosition = 0; quantumPosition < 4; quantumPosition++)
			{
				temp <<= 6;
				if (*cursor >= 0x41 && *cursor <= 0x5A) // This area will need tweaking if
					temp |= *cursor - 0x41;		              // you are using an alternate alphabet
				else if (*cursor >= 0x61 && *cursor <= 0x7A)
					temp |= *cursor - 0x47;
				else if (*cursor >= 0x30 && *cursor <= 0x39)
					temp |= *cursor + 0x04;
				else if (*cursor == 0x2B)
					temp |= 0x3E; //change to 0x2D for URL alphabet
				else if (*cursor == 0x2F)
					temp |= 0x3F; //change to 0x5F for URL alphabet
				else if (*cursor == padCharacter) //pad
				{
					switch (input.end() - cursor)
					{
					case 1: //One pad character
						decodedBytes.push_back((temp >> 16) & 0x000000FF);
						decodedBytes.push_back((temp >> 8) & 0x000000FF);
						return move(decodedBytes);
					case 2: //Two pad characters
						decodedBytes.push_back((temp >> 10) & 0x000000FF);
						return move(decodedBytes);
					default:
						throw std::runtime_error("Invalid Padding in Base 64!");
					}
				}
				else
					throw std::runtime_error("Non-Valid Character in Base 64!");
				cursor++;
			}
			decodedBytes.push_back((temp >> 16) & 0x000000FF);
			decodedBytes.push_back((temp >> 8) & 0x000000FF);
			decodedBytes.push_back((temp)& 0x000000FF);
		}
		return move(decodedBytes);
	}
};



std::string&& ESBCommon::DataToBase64(const void* data, size_t len)
{
	return CBase64<char>::base64Encode(data, len);
}

std::string&& ESBCommon::DataToBase64(const std::vector<BYTE>& data)
{
	return DataToBase64(&data[0], data.size());
}

std::vector<BYTE>&& ESBCommon::Base64ToData(const char* szBase64)
{
	return Base64ToData(string(szBase64));
}

std::vector<BYTE>&& ESBCommon::Base64ToData(const std::string& szBase64)
{
	return CBase64<char>::base64Decode(szBase64);
}

std::wstring&& ESBCommon::DataToBase64W(const void* data, size_t len)
{
	return CBase64<wchar_t>::base64Encode(data, len);
}

std::wstring&& ESBCommon::DataToBase64W(const std::vector<BYTE>& data)
{
	return DataToBase64W(&data[0], data.size());
}

std::vector<BYTE>&& ESBCommon::Base64ToDataW(const wchar_t* szBase64)
{
	return Base64ToDataW(wstring(szBase64));
}

std::vector<BYTE>&& ESBCommon::Base64ToDataW(const std::wstring& szBase64)
{
	return CBase64<wchar_t>::base64Decode(szBase64);
}
