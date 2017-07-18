// CopyFile.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CopyFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#include <iostream>
#include <string>
#include <vector>

// The one and only application object

CWinApp theApp;

using namespace std;



bool BrowsFile(const CString pathStr, vector<CString>& file,vector<CString>& folder)
{
	CString myDataPath,fdPath;
	myDataPath=pathStr + "\\*.*";
	CString strTmp;

	CFileFind find;
	BOOL bf = find.FindFile(myDataPath);
	while(bf)
	{                        
		bf = find.FindNextFile();
		if(!find.IsDots())
		{
			fdPath=find.GetFilePath();
			if (find.IsDirectory())
			{
				//如果是文件夹，递归，继续往下找  
				BrowsFile(fdPath, file,folder);
				folder.push_back(fdPath);
				
			}
			else
			{
				//如果是文件
				file.push_back(fdPath);
				
			}
		}
	}
	
	find.Close();
	
	if(file.empty() && folder.empty())
	{
		return false;
	}
	else
	{
		return true;
	}
}    

void CreateFolder(vector<CString> folder,const CString inputfolder,const CString outputfoler)
{
	int nsize=folder.size();
	CString newfolername;
	for (int i=nsize-1;i>=0;i--)
	{
		CString tt1=folder[i];
	
		//CString tt = folder[i].TrimLeft(inputfolder);
		folder[i].Delete(-1,inputfolder.GetLength());

		newfolername=outputfoler+folder[i];
		
		if (CreateDirectory(newfolername,NULL))
		{
			cout<<"Create sucess:"<<newfolername<<endl;
		}
		else
		{
			cout<<"Create folder fail:"<<newfolername<<endl;
		}	
	}
}

void CreateFile(vector<CString> file,const CString inputfolder,const CString outputfoler)
{
	int nsize=file.size();
	int nfilesize;
	CString newfilername;
	FILE *pReadfile=NULL;
	FILE *pWritefile=NULL;
	char *buf=NULL;
	for (int i=nsize-1;i>=0;i--)
	{
		pReadfile = fopen(file[i],"r");
		if (NULL == pReadfile)
		{
			cout<<"readfile: "<<file[i]<<" open fail"<<endl;
			continue;
		}
		fseek (pReadfile, 0, SEEK_END);   ///将文件指针移动文件结尾
		nfilesize=ftell (pReadfile); ///求出当前文件指针距离文件开始的字节数
		buf=new char[nfilesize+1];
		memset(buf,0,nfilesize+1);
		fseek (pReadfile, 0, SEEK_SET);
		fread(buf,1,nfilesize,pReadfile);

		file[i].Delete(-1,inputfolder.GetLength());

		newfilername=outputfoler+file[i];

		pWritefile = fopen(newfilername,"w+");
		if (NULL == pWritefile)
		{
			cout<<"writefile: "<<newfilername<<"open file fail:"<<endl;
			continue;
		}
		fwrite(buf,nfilesize,1,pWritefile);
		cout<<"Create file sucess:"<<newfilername<<endl;

		fclose(pWritefile);
		fclose(pReadfile);
		delete []buf;
		buf=NULL;
	}
}
bool FolderExist(CString strPath)
{
	WIN32_FIND_DATA wfd;
	bool rValue = false;
	HANDLE hFind = FindFirstFile(strPath, &wfd);
	if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		FindClose(hFind);
		return true;
	}
	FindClose(hFind);
	return false;
}
int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: change error code to suit your needs
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: code your application's behavior here.
			if (argc!=5)
			{
				cout<<"argc is wrong"<<endl;
				system("pause");
				return nRetCode;
			}
			bool bflaginput=FolderExist(argv[2]);
			if (!bflaginput)
			{
				cout<<argv[2]<<" input folder is not exist"<<endl;
			}
			bool bflagonput=FolderExist(argv[4]);
			if (!bflagonput)
			{
				cout<<argv[4]<<" output folder  is not exist"<<endl;
			}
			if (!bflaginput || !bflagonput)
			{
				system("pause");
				return nRetCode;
			}

			vector<CString> vecfile;//文件
			vector<CString> vecFolder;//文件夹
			if (BrowsFile(argv[2],vecfile,vecFolder))
			{
				CreateFolder(vecFolder,argv[2],argv[4]);
				CreateFile(vecfile,argv[2],argv[4]);
			}
			else
			{
				cout<<argv[0]<<" file is not exist"<<endl;
				system("pause");
			}
			
			
		}

	}
	else
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}
	system("pause");
	return nRetCode;
}
