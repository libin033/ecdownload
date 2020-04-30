#include "CIMISSMain.h"

/*
此函数下载文件（数值模式、雷达资料）
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "dataQueryClientC.h"

using namespace std;

/*
 * 客户端调取，下载文件，并返回RetFilesInfo对象(面向过程)
 */
 
char *strLastTime = NULL;
char *strstartime = NULL;
char *strendtime = NULL;
char *strSavePath = NULL;
 
void FileReName(string str1,char* strstartime, char* strSavePath)
{
    // 替换文件名，将文件名
    // Z_NAFP_C_BABJ_20190801012056_P_CLDAS_RT_ASI_0P0625_HOR-SM000005-2019080100.nc转换成 
    // NAFP_CLDAS_BTC_YYYYMMDDhhmmss_SM100200.NC
    int pos1 = str1.find_last_of('_', str1.length());
    int pos2 = str1.find_last_of('-', str1.length());
    string str2 = str1.substr(pos1+5, pos2-pos1-5);
    cout << "pos1:"  << pos1 << "pos2:" <<  pos2 << endl;
    //cout << "strSavePath:" << strSavePath << endl;
    char strFileName[256];memset(strFileName, 0, sizeof(char)*256);
    strcpy(strFileName, strSavePath);
    strcat(strFileName, "/NAFP_CLDAS_BTC_");
    strcat(strFileName, strstartime);
    strcat(strFileName, "_");
    strcat(strFileName, str2.c_str());
    strcat(strFileName, ".NC");
    cout << str1.c_str()  << endl;
    cout <<  strFileName << endl;
    rename(str1.c_str(), strFileName);

}
 
 
int FileInfoSearchAPI_CLIB_callAPI_to_downFile(char* interfaceId, char **params,int parmcnt,char * savepath)
{
  /* 调用方法的参数定义，并赋值 */
  printf("%s\n",interfaceId);

//  string passwork = "zhangxz123";
//  string username = "NSMC_KZS_ZHANGXZ";
//  char* USRNAME = const_cast<char*>(username.c_str());
//  char* PASSWORK = const_cast<char*>(passwork.c_str());
  
  RetFilesInfoC retFilesInfoc;
  RetFilesInfoC* retFilesInfo=&retFilesInfoc;
    
  //初始化接口服务连接资源，不带参，服务端IP和端口在config文件中配置
  initResources(); 
  //调用接口
  int ret = callAPI_to_downFile(USRNAME, 
            PASSWORK,
            interfaceId,
            params, 
            parmcnt, 
            savepath, 
            retFilesInfo);  
   printf("ret:%d\n", ret);
   sleep(1);
  //输出结果信息：文件列表
  //if(retFilesInfo->fileInfos.listNum>0)
  //{
  //  int count=retFilesInfo->fileInfos.listNum;
  //  char value[256];
  //  int i=0;
  //  for(i=0;i<count;i++)
  //  {
  //    //value=retFilesInfo->fileInfos.filelist[i].fileName;
  //    sprintf(value, "%s/%s", savepath, retFilesInfo->fileInfos.filelist[i].fileName);
  //    printf("%s\n",value);
  //    FileReName((string)value, strtimes, savepath);
  //    
  //  }
  //}
  //释放内存空间
  //freeMem(retFilesInfo,FILELIST); 

  //释放接口服务连接资源
  destroyResources();

  return ret;
}

string GetEXEDir(char* exePath)
{
    
    string strPath = "";

    strPath = (string)exePath;
    int pos = strPath.find_last_of('/', strPath.length());
    
    return strPath.substr(0, pos);
}

int DownFile(int itime, char* strElements, char* strtimes, char* strSavePath)
{
    char strvalidTime[256];
    sprintf(strvalidTime, "validTime=%d", itime);

    char strfcstEle[256];
    sprintf(strfcstEle, "fcstEle=%s", strElements);

    char* interfaceId = "getNafpFileByElementAndTimeAndValidtime";
    const int parmcnt = 4;
    char* params[parmcnt] = {
      "dataCode=NAFP_FOR_FTM_HIGH_EC_GLB",
      strtimes,
      strfcstEle,
      strvalidTime,
    };

    printf("***************************************\n");
    printf("\t%s\n", interfaceId);
    printf("\t%s\n", strSavePath);
    printf("\t%s\n", strtimes);
    printf("\t%s\n", strfcstEle);
    printf("\t%s\n", strvalidTime);
    printf("***************************************\n");

    int status = FileInfoSearchAPI_CLIB_callAPI_to_downFile(interfaceId, params, parmcnt, strSavePath);
    printf("status=%d\n", status);

    return status;
}

int main(int argc, char* argv[])
{
  printf("************CIMISS DownLoad Start************************\n");
 
  chdir(GetEXEDir(argv[0]).c_str());

  	/*
	调度参数：
	-s startime:YYYYMMDDHHMMSS
	-e endtime:YYYYMMDDHHMMSS
	-o outname/outpath:
	-h help
	-elm elements
	-r region : region extend -> [top_lat, left_lon, buttom_lat, right_lon]
	-ac adminCodes : 行政编码
	*/
    char *strLastTime = NULL;
    char *strstartime = NULL;
    char *strendtime = NULL;
    char *strSavePath = NULL;
        

  char strElements[7][256] = {
      "WIU10",      // 10米风的u分量
      "WIV10",      // 10米风的V分量
      "DPT",        // 露点温度
      "GRLCC",      // 低云量
      "GRTCC",      // 总云量
      "TEF2",       // 2米温度
      "TPE",        // 总降水量
  };

  char strtimes[256];
  sprintf(strtimes, "time=%s", argv[1]);
  
  strSavePath = new char[256];
  strcpy(strSavePath, argv[2]);

  int count = 0;
  int num = 0;

  for (int i = 0; i < 7; i++)
  {
      for (int j = 0; j <= 72; j += 3)
      {
          int status = DownFile(j, strElements[i], strtimes, strSavePath);
          if (status == 0)
              count++;
          else
          {
              num++;
          }
      }
  }
  printf("Success Download File Count: %d\n", count);
  printf("Failed  Download File Count: %d\n", num);
  
    if(strLastTime != NULL)
    {
        delete [] strLastTime;
        strLastTime = NULL;
    }
    
    if(strstartime != NULL)
    {
        delete [] strstartime;
        strstartime = NULL;
    }
    
    if(strendtime != NULL)
    {
        delete [] strendtime;
        strendtime = NULL;
    }
    
    if(strSavePath != NULL)
    {
        delete [] strSavePath;
        strSavePath = NULL;
    }
    
    
  printf("************CIMISS DownLoad Success************************\n");
  
  return 0;
}


