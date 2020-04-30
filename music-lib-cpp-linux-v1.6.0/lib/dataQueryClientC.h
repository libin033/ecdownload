//cppmodule.h
//#include "dataQueryClientI.h"

#ifndef _CPPMODULE_H_
#define _CPPMODULE_H_

#ifdef __cplusplus
extern "C" {
#endif

	enum {
		DATA2D,
		GRID2D,
		FILELIST
	};

	typedef struct StringVector
	{
		char **attributes;
		int size;
	}StringVector;

	typedef	struct FileInfoC
	{
		char fileName[256];
		char savePath[256];
		char suffix[28];
		char size[28];
		char fileUrl[512];
		char* imgBase64;
		StringVector Attributes;
	}FileInfo;

	

	typedef	struct FileInfoListC
	{
		struct FileInfoC *filelist;
		int listNum;
	}FileInfoList;

	typedef struct RequestInfoC
	{
		int errorCode;
		char errorMessage[1024];
		char requestElems[1024];
		char requestParams[1024];
		char requestTime[64];
		char responseTime[64];
		int rowCount;
		int takeTime;
	}RequestInfoC;

	typedef struct RetFilesInfoC
	{
		struct FileInfoListC fileInfos;
		struct RequestInfoC request;
	}RetFilesInfoC;

	typedef struct StringArrayC
	{
		//char data[MAX_RECORD][MAX_FIELD][MAX_LEN];
		char ***data;
		int rowCount;
		int colCount;
	}StringArray;  //150107

	typedef  struct RetArray2D_C
	{
		struct StringArrayC data;
		struct RequestInfoC request;
	}RetArray2DC;

	typedef struct GridArrayC
	{
		float **gridData;
		int latPoints;
		int lonPoints;	
	}GridArray;


	typedef	struct RetGridArray2D_C
	{
		struct GridArrayC data;
		struct RequestInfoC request;
		float startLat;
		float startLon;
		float endLat;
		float endLon;
		int latCount;
		int lonCount;
		float lonStep;
		float latStep;
	}RetGridArray2DC;

	//string 长度
	#define CS_LEN 256
	#define COL_LEN 128
	/**
	*initResources nouse
	*/
	int initResources1(char *,int);
	/**********************************************************************************
	**初始化dll资源
	**return:-1表示未找到client.config文件，0表示成功
	***********************************************************************************/
	int initResources();
	
	int callAPI_to_array2D(char* userId, char*  pwd,	char* interfaceId, char** params,int paramCnt, RetArray2DC *retArray2D);

	int callAPI_to_gridArray2D(char* userId, char*  pwd,	char*  interfaceId, char** params,int paramCnt,	RetGridArray2DC *retGridArray2D);

	int callAPI_to_fileList(char* userId, char*  pwd,	char*  interfaceId,char** params,int paramCnt,RetFilesInfoC *retFilesInfo);

	char* callAPI_to_serializedStr(char* userId, char*  pwd,	char*  interfaceId, char** params,int paramCnt,	char* dataformat, char* retStr);

	int callAPI_to_saveAsFile(char* userId, char*  pwd,	char*  interfaceId, char** params,int paramCnt,	char* dataformat, char*  saveFullName, RetFilesInfoC * retFilesInfo);
	

	int callAPI_to_storeArray2D( char* userId, char* pwd, char* interfaceId, char** params,int paramCnt, char inArray2D[][COL_LEN][CS_LEN],int inputInfo[], RequestInfoC* retRequestInfo );
	
    int callAPI_to_storeFile(char* userId, char* pwd, char* interfaceId, char** params,int paramCnt, char inArray2D[][COL_LEN][CS_LEN], char inFilePaths[][CS_LEN],int inputInfo[], RequestInfoC* retRequestInfo);
	
	int callAPI_to_storeSerializedStr( char* userId, char* pwd,char* interfaceId, char* params, char* inString );



	/***********************************************************************************
	**通过输入条件下载文件
	**savePath：存储路径
	**retFilesInfo：文件信息列表，如果下载失败，文件信息中的size=0
	** return ：0表示成功，-1表示失败，-1102表示未检索到文件
	***********************************************************************************/
	int callAPI_to_downFile(char* userId, char*  pwd,	char*  interfaceId,char** params,int paramCnt, char*  savePath, RetFilesInfoC * retFilesInfo);
	

	/***********************************************************************************
	**通过url下载文件
	**fileURL：下载文件url
	**save_as：文件保存名称，需包含路径
	** return ：0表示成功，-1表示失败
	***********************************************************************************/
	int callAPI_to_downFile_ByUrl(char * fileURL,char * save_as);
	
	
	/********************************************************************************
	**下载指定url文件，并存储在ret_bytes内存中,使用完后务必释放空间（free(ret_bytes)),否则长时间运行内存可能溢出
	**注意：二进制数据不能用strlen（ret_bytes)判断其大小，其大小需要配合返回文件大小来获取
	**fileURL：下载文件url
	**ret_bytes：返回char数据，
	** return ：ret_bytes数据
	** 
	*********************************************************************************/
	char* callAPI_to_downFileByte(char* fileURL,char *ret_bytes);

	/********************************************************************************
	**释放资源
	********************************************************************************/
	int destroyResources();

	/**
	* free memery space,remind!
	**/
	int freeDataMem(RetArray2DC *retArray2D);
	int freeGridMem(RetGridArray2DC *retArrayGrid2d);
	int freeFileListMem(RetFilesInfoC *retFilesInfo);
	/********************************************************************************
	**释放内存空间
	** v:初始化的结构体指针（RetArray2DC，RetGridArray2DC，RetFilesInfoC）
	** type:结构体类型，对应指针（DATA2D,	GRID2D,FILELIST）
	** return:0 成功，-1失败
	*********************************************************************************/
	int freeMem(void * v,int type);


#ifdef __cplusplus
}
#endif

#endif