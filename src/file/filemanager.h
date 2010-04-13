#ifndef __FILEMANAGER_H__
#define __FILEMANAGER_H__

#define MAX_FILE_LINE_LENGTH 256
#define MAX_FILE_WORD_LENGTH 128

class FileManager
{
	protected:
		// --- Generic File Loading
		bool ReadWord(byte** buffer, byte* wordBuffer);
		bool ReadLine(byte** buffer, byte* lineBuffer);
		bool LoadFile(byte** buffer, string filename);

	public:
		FileManager();
		virtual ~FileManager();
};

#endif //__FILEMANAGER_H__
