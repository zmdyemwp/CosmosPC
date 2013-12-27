#ifndef		__COSMOS_ENVIRONMENT_SETTINGS_H__
#define		__COSMOS_ENVIRONMENT_SETTINGS_H__


//		Do not use SPP command but use simple aabbcc command
//#define		__COSMOS_MIN_DOWNLOAD_PROC__

//		Use old data format to parse log files
#define		__W10_OLD_DATA_FORMAT__
#ifdef		__W10_OLD_DATA_FORMAT__
	#define		FILE_FORMAT_OLD
#endif		//__W10_OLD_DATA_FORMAT__


//		Directly send sql command but not create *.cosmos files
//#define		__DIRECTLY_SQL__


#endif		//__COSMOS_ENVIRONMENT_SETTINGS_H__

