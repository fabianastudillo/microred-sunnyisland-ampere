/*
 *      YASDI - (Y)et (A)nother (S)MA(D)ata (I)mplementation
 *      Copyright(C) 2001-2008 SMA Solar Technology AG
 *
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2.1 of the License, or (at your option) any later version.
 *
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 *
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 */

/**************************************************************************
*
*  HISTORY :
*
*  XXX MM.DD.JJ Beschreibung
*  ------------------------------------------------------------------------
* $Revision: 1.34 $, $Name:  $
*
* $Log: src/shell/CommonShellUIMain.c  $
* Revision 1.34 2009/12/14 14:15:52CET Heiko Pruessing (pruessing)
* remove some compiler warnings
* Revision 1.33 2009/12/14 09:05:08CET Heiko Pruessing (pruessing)
* nach Abgleich mit SVN: Kanallisten download Events, divs
* Revision 1.32 2009/12/14 08:24:32CET Heiko Pruessing (pruessing)
* V1.8.1Build8
* Revision 1.31 2008/09/11 10:58:39CEST Heiko Prssing (pruessing)
* .
* Revision 1.30 2008/08/08 10:59:31CEST Heiko Prssing (pruessing)
* .
* Revision 1.29 2008/08/01 14:35:05CEST Heiko Prssing (pruessing)
* reworked the user event system, some problems with threads
* Revision 1.28 2008/07/30 09:03:26CEST Heiko Prssing (pruessing)
* .
* Revision 1.27 2008/04/07 10:38:37CEST Heiko Prüssing (pruessing)
* .
* Revision 1.26 2007/10/23 11:35:35CEST Heiko Prüssing (pruessing)
* .
* Revision 1.25 2007/03/01 13:12:06GMT+01:00 Heiko Prï¿½ssing (pruessing)
* Concurrent SMA Data Master
* Revision 1.22 2007/01/03 12:55:48GMT+01:00 Heiko Pruessing (pruessing)
* divs
* Revision 1.21 2007/01/03 11:04:16GMT+01:00 Heiko Pruessing (pruessing)
* Mit SVN Revision 139 zusammengefuehrt
* Revision 1.20 2006/11/23 11:50:17GMT+01:00 Heiko Pruessing (pruessing)
* Momentanwertkanï¿½le als Array + divs, Kanalmaske fï¿½r "alles Kanï¿½le"
* Revision 1.19 2006/11/16 08:16:18GMT+01:00 Heiko Pruessing (pruessing)
* divs
* Revision 1.18 2006/11/16 08:07:14GMT+01:00 Heiko Pruessing (pruessing)
* divs
* Revision 1.16 2006/10/30 08:25:26GMT+01:00 Heiko Pruessing (pruessing)
* automatische Multiprotokollunterstï¿½tzung (SMANet + SunnyNet gleichzeitig)
* Revision 1.15  2006/10/19 20:07:52  pruessing
* - ï¿½nderungen wegen Multiprotokollunterstï¿½tzung
* - AccessLevel ï¿½ber Shell mï¿½glich...
* Revision 1.14  2006/10/18 11:07:30  pruessing
* some changes for multiprotocol (SMANet and SunnyNet), not fully
* functional...
* Revision 1.13  2006/10/06 20:24:34  pruessing
* Revision 1.12  2006/09/22 15:05:56  pruessing
* Revision 1.11  2006/07/19 11:46:29  duell
* Revision 1.10  2006/07/17 17:16:07Z  pruessing
* eingecheckt vom offiziellen Subversion Repository
* Revision 1.9  2005/12/06 12:15:48Z  pruessing
* Revision 1.8  2005/07/13 11:24:58Z  pruessing
* Revision 1.7  2005/06/29 15:17:22Z  pruessing
* Revision 1.6  2005/06/29 14:42:37Z  pruessing
* Revision 1.5  2005/06/20 09:21:54Z  pruessing
* Revision 1.4  2005/03/08 14:46:58Z  pruessing
* Revision 1.3  2005/03/07 09:45:14  pruessing
* Revision 1.2  2005/03/07 09:45:14  pruessing
* Revision 1.1  2003/11/11 14:57:04  pruessing
* Initial revision
* Revision 1.2  2003/11/11 14:57:04  pruessing
* Pruessing 28.04.2002 Creation of File
*
*
***************************************************************************/


/*************************************************************************
*   I N C L U D E
*************************************************************************/
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "os.h"
#include "smadef.h"
#include "chandef.h"
#include "libyasdi.h"
#include "libyasdimaster.h"
#include "smadata_layer.h"
#include "tools.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

#ifdef __cplusplus
}
#endif
const char *filename = "/home/rpi/Desktop/SPOTCHANNELS.txt";
const char *filename3 = "/home/rpi/Desktop/PARAMCHANNELS.txt";
const char *filenameSetInfo = "/home/rpi/Desktop/SetInformation.txt";
const char *filenameSunnyLog = "/home/rpi/Desktop/LoggYasdiProgram.txt";

/*************************************************************************
*   F U N C T I O N   D E C L A R A T I O N S
**************************************************************************/


/**************************************************************************
*   M A C R O   D E F I N I T I O N S
**************************************************************************/


/**************************************************************************
*   G L O B A L
**************************************************************************/

#define DEVMAX 30       //For simplicity we use here maximal 30 devices ...
#define MAXDRIVERS 10   //... and 10 YASDI Bus drivers
#define EXPECT_CHAN_CNT 300  //lets say that we expect 300 channels in max
                             //for one device

/**************************************************************************
*   S T A T I C
**************************************************************************/


/**************************************************************************
*   L O C A L   F U N C T I O N S
**************************************************************************/

void PrintDevList( void );
void PrintDevList( void );
void PrintChannelValues(TChanType chanType, char *routeDataBase, int flag, FILE *fp);
void SetParamValue(FILE *fichero);
void DoStartDetection( int DevCnt );
void DoStartDetectionAsync( int DevCnt );
void DoCommands( void );
int GetDateFromFile();

/**************************************************************************
*   G L O B A L  F U N C T I O N S
**************************************************************************/





/**************************************************************************
   Description   : print out device list
   Parameter     : (none)
   Return-Value  : (none)
   Changes       : Author, Date, Version, Reason
                   ********************************************************
                   PRUESSING, 02.07.2001, 1.0, Created
**************************************************************************/
void PrintDevList( void )
{
   DWORD i;
   DWORD DevHandles[DEVMAX];
   DWORD DevCount=0;
   char NameBuffer[50];


   /* get all device handles...*/
   DevCount   = GetDeviceHandles(DevHandles, DEVMAX );
   if (DevCount)
   {
      printf("-------------------------------------------\n");
      printf("Device handle | Device Name  \n");
      printf("-------------------------------------------\n");

      for(i=0;i<DevCount;i++)
      {
         /* get the name of this device */
         GetDeviceName(DevHandles[i], NameBuffer, sizeof(NameBuffer)-1);
         printf("   %3lu        | '%s'\n",
                (unsigned long)DevHandles[i],
                NameBuffer);
      }
      printf("-------------------------------------------\n\n");
   }
   else
   {
      printf("Sorry, no devices currently available...\n\n");
   }
}

/**************************************************************************
   Description   : print out all channels of a device
   Parameter     : ChanMask: the filter mask (see SMA-Data-Description)
   Return-Value  : (none)
   Changes       : Author, Date, Version, Reason
                   ********************************************************
                   PRUESSING, 02.07.2001, 1.0, Created
**************************************************************************/
void PrintChannelValues(TChanType chanType, char *routeDataBase, int flag, FILE *fp)
{
   int res;
   //FILE *fp = fopen(routeDataBase, "w");

   DWORD ChanHandle[EXPECT_CHAN_CNT];
   //DWORD ChanHandle[13];
   char ChanName[50];
   char DevName[50];
   char cUnit[17];
   int i;
   int ChanCount;
   int DevHandle = 1;
   double Value;
   char TextValue[30];
   DWORD MaxValueAge = 5; /* maximum age of the channel value in seconds...*/

   int spotValues[18] = {192, 193, 194, 202, 206, 210, 214, 215, 219, 236, 237, 238, 275, 190, 232, 196, 197, 223};// Borre 276, 274
   int paramValues[29] = {22, 23, 24, 25,26, 9, 10, 17, 18, 19, 20, 31, 32, 33, 34, 35, 36, 48, 49, 50, 51, 52, 53, 64, 65, 66, 67, 75, 76}; // hasta 25 23 y 24 25 repetidos



   if(chanType == PARAMCHANNELS && flag == 0){
      ChanCount = 29;
                for(int i = 0 ; i < ChanCount; i++){
                        ChanHandle[i] = paramValues[i];
                }

   }else if(chanType == SPOTCHANNELS && flag == 0){
      ChanCount = 18;
      for(int i = 0 ; i < ChanCount; i++){
                        ChanHandle[i] = spotValues[i];
                }
   }else if(chanType == PARAMCHANNELS && flag == 1){
         ChanCount = 1;
         ChanHandle[0] = 4;
   }
   //printf("INGRESO AL BUCLE...\n");
   rewind(fp);
   for(i=0;i<ChanCount;i++)
   {
      GetChannelName(ChanHandle[i],ChanName, sizeof(ChanName)-1);

      //get the channel unit?
      cUnit[0]=0;
      GetChannelUnit(ChanHandle[i], cUnit, sizeof(cUnit)-1);

      /* Get channel value... */
      res = GetChannelValue(ChanHandle[i], DevHandle, &Value, TextValue,
                            sizeof(TextValue)-1, MaxValueAge);
      if(res==0)
      {
         /* Status texts?*/
         if (strlen( TextValue )==0)
            sprintf( TextValue,"%.3f", Value);
      }
      else
      {
         printf("Error reading channel value....error code=%d\n",res);
         break;
      }

     /*printf("     %3lu       | '%16s' | '%s' %c%s%c \n", (unsigned long)ChanHandle[i], ChanName,
            TextValue,
            strlen(cUnit) > 0 ? '(' : ' ',
            cUnit,
            strlen(cUnit) > 0 ? ')' : ' ');*/
   fprintf(fp, "%s\n", TextValue);
   fflush(fp);
   }
}

/*
** Gebe alle Statustexte eines Kanals aus (wenn es wleche gibt, sonst nicht!)
*/
/**************************************************************************
   Description   : Print out all status texts of a channel
   Parameter     : ChanHandle: channel Handle
   Return-Value  : (none)
   Changes       : Author, Date, Version, Reason
                   ********************************************************
                   PRUESSING, 01.12.2002, 1.0, Created
**************************************************************************/
void printStatTexts(DWORD ChanHandle)
{
   int i;
   int TxtCnt = GetChannelStatTextCnt( ChanHandle );
   if (TxtCnt)
   {
      printf("All possible status texts of this channel:\n");

      for(i=0;i<TxtCnt;i++)
      {
         char StatText[30];
         GetChannelStatText(ChanHandle, i, StatText, sizeof(StatText)-1);

         printf("(%2d): '%s'\n",i,StatText);
      }
   }
}

/**************************************************************************
   Description   : set a channel value
   Parameter     : (none)
   Return-Value  : (none)
   Changes       : Author, Date, Version, Reason
                   ********************************************************
                   PRUESSING, 02.07.2001, 1.0, Created
**************************************************************************/
void SetParamValue(FILE *fichero)
{
   #define limite 10
   char value[50]={0};
   int DevHandle = 1;
   int ChanHandle;
   double ChanValue;
   //int ChanValue;
   int iResult;
   char valor1[limite];
   char valor2[limite];
   //FILE *fichero;
   //FILE *copiaFichero;
   //fichero = fopen("/home/rpi/Desktop/SetInformation.txt", "rt");
   //copiaFichero = fopen("/home/rpi/Desktop/SetInformation.txt", "rt");
   fseek(fichero, 0, SEEK_END);

   if(ftell(fichero) != 0){
      fseek(fichero, 0, SEEK_SET);
      fgets(valor1, limite, fichero);
      fgets(valor2, limite, fichero);
      //puts(valor1);
      //puts(valor2);
      ChanValue = atof(valor2);
      ChanHandle = atoi(valor1);
      printf("%d -%d\n", ChanHandle, ChanValue);
      system("truncate -s 0 /home/rpi/Desktop/SetInformation.txt");
      iResult = SetChannelValue(ChanHandle, DevHandle, ChanValue );
      if (iResult==0)
         printf("Ok, channel was written!\n");
      else
         printf("ERROR: Channel was not written! Error code=%d\n", iResult);
   }else{
     fseek(fichero, 0, SEEK_SET);
      //puts("Archivo Vacio...\n");
   }
}

/**************************************************************************
   Description   : Removes a device from the current plant...
   Parameter     : (none)
   Return-Value  : (none)
   Changes       : Author, Date, Version, Reason
                   ********************************************************
                   PRUESSING, 11.11.2003, 1.0, created
**************************************************************************/
void DoRemoveDevice( void )
{
   int DevHandle;

   //printf("DoRemoveDevice:\n\n");
   //printf("Device handle: ");   scanf("%d", &DevHandle);

   //call Yasdi-Master-API...
   RemoveDevice( (DWORD)DevHandle );
}


/**************************************************************************
   Description   : Start device detection (find all devices)
   Parameter     : (none)
   Return-Value  : (none)
   Changes       : Author, Date, Version, Reason
                   ********************************************************
                   Pruessing, 02.07.2001, 1.0, Created
                   Pruessing, 01.12.2002, 1.1, Using new API Funktion
                                               "DoMasterCmdEx()"
**************************************************************************/
void DoStartDetectionAsync( int DevCnt )
{
   int iErrorCode;

   printf("Starting device detection (async).\n");
   printf("How many devices should be searched? "); scanf("%d", &DevCnt);

   /* Do start searching devices... */
   iErrorCode = DoStartDeviceDetection( DevCnt, FALSE /*do not block*/ );
   switch(iErrorCode)
   {
      case YE_DEV_DETECT_IN_PROGRESS:
         printf("Sorry, but there is already an running device detection...\n");
         return; //currently not possible...
         //break;

      case YE_OK:
         printf("Ok, Device detection is now in progress...\n");
         break;

      case YE_INVAL_ARGUMENT:
         printf("Sorry, but the device count was invalid.\n");
         break;

      default:
         printf("mhhh....\n");
   }
}

//Start device detection synchrone (blocks until device detection is done)
void DoStartDetection( int DevCnt )
{
   int iErrorCode;

   printf("Starting device detection (sync).\n");

   if (DevCnt==0)
   {
           printf("How many devices should be searched? "); scanf("%d", &DevCnt);
   }

   /* Do start searching devices... */
   iErrorCode = DoStartDeviceDetection( DevCnt, TRUE /*blocking*/ );
   switch(iErrorCode)
   {
      case YE_OK:
         break;

      case YE_DEV_DETECT_IN_PROGRESS:
         printf("Sorry, but there is already an running device detection...\n");
         return; //currently not possible...

      case YE_NOT_ALL_DEVS_FOUND:
         printf("Sorry, but not all devices were found.\n");
         break;

      default:
         printf("mhhh....\n");
   }

   PrintDevList();
}



/**************************************************************************
*
* NAME        : DoChangeAccessLevel
*
* DESCRIPTION : Change the access level to the device channels...
*
*
***************************************************************************
*
* IN     : ---
*
* OUT    : ---
*
* RETURN : ---
*
* THROWS : ---
*
**************************************************************************/
void DoChangeAccessLevel( void )
{
   char username[30] = "inst";
   char passwd[11]   = "ID00";

   /*printf("Username:"); scanf("%s", username);
   printf("Password:"); scanf("%s", passwd);*/
   time_t t;
   t = time(NULL);
   struct tm tm = *localtime(&t);
   char codeStr[20];

   int day = tm.tm_mday;
   int month = tm.tm_mon+1;
   int year = tm.tm_year+1900;

   int mill = year/1000;
   int cent = (year - (mill*1000))/100;
   int dec = (year - (mill*1000 + cent*100))/10;
   int unit = year - (mill*1000 + cent*100 + dec*10);

   int code = dec*10 + unit + day + month;
   sprintf(codeStr, "%d", code);
   strcat(passwd, codeStr);

   if (yasdiMasterSetAccessLevel(username, passwd))
      printf("Success! Changed access level to '%s'\n", username );
   else
      printf("No access! Please try again...\n");
}



/**************************************************************************
   Description   : Interpret commands...
   Parameter     : (none)
   Return-Value  : (none)
   Changes       : Author, Date, Version, Reason
                   ********************************************************
                   PRUESSING, 02.07.2001, 1.0, Created
**************************************************************************/
int GetDateFromFile(){

   #define limite 10
   char value[50]={0};
   int DevHandle = 1;
   int ChanHandle;
   //double ChanValue;
   int ChanValue;
   int iResult;
   char valor1[limite];
   FILE *fichero;
   FILE *copiaFichero;
   fichero = fopen("/home/rpi/Desktop/DATEVALUE.txt", "rt");
   copiaFichero = fopen("/home/rpi/Desktop/DATEVALUE.txt", "rt");
   fseek(copiaFichero, 0, SEEK_END);

   if(ftell(copiaFichero) != 0){
      fgets(valor1, limite, fichero);
      fclose(fichero);
      fichero = fopen("/home/rpi/Desktop/DATEVALUE.txt", "w");
      fwrite("", 0, 0, fichero);
      fclose(fichero);
      //puts("Extraido:\n");
      ChanHandle = (int) strtol(valor1, NULL, 10);
      //printf("%d\n", ChanHandle);
   }else{
      //puts("Archivo Vacio...\n");
   }
   return ChanHandle;
}

void DoCommands( void )
{
   //char Cmd;
   BOOL bEnd = false;
   int date;
   int newDate;
   DoStartDetection(1);
   DoChangeAccessLevel();
   const FILE *fpSPOT = fopen(filename, "r+");
   const FILE *fpPARAM = fopen(filename3, "r+");
   const FILE *fpSet = fopen(filenameSetInfo, "r+");
   const FILE *fpLogSunny = fopen(filenameSunnyLog, "r+");

   while(!bEnd)
   {
      // 4
      time_t t;
      t = time(NULL);
      struct tm tm = *localtime(&t);
      char codeStr[20];

      int day = tm.tm_mday;
      int month = tm.tm_mon+1;
      int year = tm.tm_year+1900;
      int hour = tm.tm_hour;
      int min = tm.tm_min;
      int sec = tm.tm_sec;
      //printf("%d - %d - %d - %d - %d - %d\n", day, month, year, hour, min, sec);
      rewind(fpLogSunny);
      fprintf(fpLogSunny, "InicioSetParam1: %d, %d, %d, %d, %d, %d\n", day, month, year, hour, min, sec);
      SetParamValue(fpSet);
      //printf("PRINT SPOTCHANNELS");
      PrintChannelValues( SPOTCHANNELS, filename, 0, fpSPOT); /* only all spot channels */
      fprintf(fpLogSunny, "Acaba spotchannels: %d - %d - %d - %d - %d - %d\n", day, month, year, hour, min, sec);
      SetParamValue(fpSet);
      fprintf(fpLogSunny, "Acaba 2 set param: %d - %d - %d - %d - %d - %d\n", day, month, year, hour, min, sec);
      //printf("PRINT PARAMACHANNELS");
      PrintChannelValues( PARAMCHANNELS, filename3, 0, fpPARAM); /* only all parameter channels */
      fprintf(fpLogSunny, "Termina todo el recorrido: %d - %d - %d - %d - %d - %d\n", day, month, year, hour, min, sec);
   }
}


//! Receive Device Detection Events from YASDI...
void cbDeviceDetectionEvent(TYASDIDetectionSub event, DWORD DeviceHandle, DWORD param1 )
{
   char NameBuffer[30]={0};

   //resolve the device handle and get the device name if possible...
   GetDeviceName(DeviceHandle, NameBuffer, sizeof(NameBuffer)-1);

   switch(event)
   {
      case YASDI_EVENT_DEVICE_ADDED:
         printf("New device found: '%s'\n", NameBuffer);
         break;

      case YASDI_EVENT_DEVICE_REMOVED:
         printf("Device was removed: '%s'\n", NameBuffer);
         break;

      case YASDI_EVENT_DEVICE_SEARCH_END:
         printf("\nAsync Device Detection finished. %d devices available.\n", (int)DeviceHandle);
         PrintDevList();
         break;

      case YASDI_EVENT_DOWNLOAD_CHANLIST:
      {
         BYTE percentDone = param1;
         printf(".");
         if (percentDone == 100)
            printf("\n");
         break;
      }

      default:
         printf("unknown yasdi (0x%2x) event...\n", event);
         break;
   }

   return;
}

void cbLongDataTransportEvent(BYTE percentDone)
{
   printf(".");
}

/**************************************************************************
   Description   : This is the Start ("main")
                   I have some trouble with the include files. So the
                   real "main" call this function from a different place...
   Parameter     : (none)
   Return-Value  : (none)
   Changes       : Author, Date, Version, Reason
                   ********************************************************
                   PRUESSING, 02.07.2001, 1.0, Created
**************************************************************************/
int main(int argv, char **argc)
{
   DWORD i;
   DWORD dDriverNum;
   DWORD Driver[MAXDRIVERS];
   char DriverName[30];
   BOOL bOnDriverOnline = FALSE; //Is at least one driver online?
   char IniFile[]="yasdi.ini";

   if (argv>=2)
   {
      strcpy(IniFile,argc[1]);
   }

  // printf("************************************************************\n");
   //printf("\n");
   //printf("        YASDI Mini Shell (build for %s) \n", os_GetOSIdentifier());
  // printf("\n");
   //printf("      (This is an test program using YASDI)\n\n");
   //printf("************************************************************\n");


   /* init Yasdi- and Yasdi-Master-Library */
   if (0 > yasdiMasterInitialize(IniFile, &dDriverNum))
       printf("ERROR: YASDI ini file was not found or is unreadable!\n");

   /* I want to be informed if new devices are inserted or removed...
   ** Insert callbac function...*/
   yasdiMasterAddEventListener( cbDeviceDetectionEvent, YASDI_EVENT_DEVICE_DETECTION );


   /* get List of all supported drivers...*/
   dDriverNum = yasdiMasterGetDriver( Driver, MAXDRIVERS );

   /* Switch all drivers online (you should only do one of them online!)...*/
   for(i=0;i<dDriverNum;i++)
   {
      /* The name of the driver */
      yasdiGetDriverName(Driver[i], DriverName, sizeof(DriverName));

      printf("Switching driver '%s' on...", DriverName);

      if (yasdiSetDriverOnline( Driver[i] ))
      {
         printf("success\n");
         bOnDriverOnline = TRUE;
      }
      else
         printf("false\n");
   }
   printf("\n");

   //Check that at least one driver is online...
   if (FALSE == bOnDriverOnline)
      printf("WARNING: No drivers are online! YASDI can't communicate with devices!\n");

   //Autodetect when started?
   if (argv>=3)
           if (strnicmp("autodetect", argc[2], 10) == 0)
                   DoStartDetection( 1 );


   /* Start "User interface"... */
   DoCommands();

   /* Shutdown all yasdi drivers... */
   for(i=0;i<dDriverNum;i++)
   {
      yasdiGetDriverName(Driver[i], DriverName, sizeof(DriverName));
      printf("Switching driver '%s' off...\n", DriverName);
      yasdiSetDriverOffline( Driver[i] );
   }

   /* Shutdown YASDI..., bye, bye */
   yasdiMasterShutdown();
   return 0;
}
