/* ListFile.cpp : 定義主控台應用程式的進入點。
	
Reference:
	https://github.com/tronkko/dirent
	https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
	https://stackoverflow.com/questions/14503605/using-fstream-object-as-a-function-parameter
	https://stackoverflow.com/questions/8411931/what-does-dot-and-dotdot-means
*/

#include "stdafx.h"
#include <cstdio>
#include <iostream>
#include <fstream>
#include "dirent.h"	

using namespace std;

/* Config */
#define lenPath 256
#define Indentation "  "
//#define DefaultPath "D:/"	//Start from D drive

void ListFile(fstream &file, char *path, int nIndent)
{
	DIR *dir;
	struct dirent *ent;
	char find[lenPath] = { 0x00 };

	/* Open directory stream */
	dir = opendir(path);
	if (dir != NULL) 
	{
		/* Print all files and directories within the directory */
		while ((ent = readdir(dir)) != NULL)
		{
			if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
			{
				switch (ent->d_type)
				{
				case DT_REG:
					for (int i = 0; i < nIndent; i++)
						file << Indentation;
					file << ent->d_name << endl;
					file << "";
					break;

				case DT_DIR:
					for (int i = 0; i < nIndent; i++)
						file << Indentation;
					file << ent->d_name << "/" << endl;
					memset(find, 0x00, lenPath);
					memcpy(find, path, strlen(path));
					find[strlen(path)] = '/';
					memcpy(find + strlen(path) + 1, ent->d_name, strlen(ent->d_name));
					ListFile(file, find, nIndent + 1);
					break;

				case DT_LNK:
					for (int i = 0; i < nIndent; i++)
						file << Indentation;
					file << ent->d_name << "@" << endl;
					break;

				default:
					;
				}
			}
		}
		closedir(dir);
	}
	else
	{
		/* Could not open directory */
		file << "Cannot open directory: \"" << path << "\"" << endl;
	}
}

int main()
{
	fstream file;
	char dirname[lenPath] = {0x00};
	int Indent = 1;

	file.open("FileList.txt", ios::out);
	if (!file)
	{
		cout << "open .txt failed!" << endl;		
		system("pause");    
		return 0;
	}
	
	cout << "Input path (Ex: C:/, D:/Program/AppData...): ";
	cin >> dirname;

	ListFile(file, dirname, Indent);
	file.close();
	return 0;
}

