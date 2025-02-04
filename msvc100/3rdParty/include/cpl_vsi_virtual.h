/******************************************************************************
 * $Id: cpl_vsi_virtual.h 27044 2014-03-16 23:41:27Z rouault $
 *
 * Project:  VSI Virtual File System
 * Purpose:  Declarations for classes related to the virtual filesystem.
 *           These would only be normally required by applications implmenting
 *           their own virtual file system classes which should be rare.  
 *           The class interface may be fragile through versions.
 * Author:   Frank Warmerdam, warmerdam@pobox.com
 *
 ******************************************************************************
 * Copyright (c) 2005, Frank Warmerdam <warmerdam@pobox.com>
 * Copyright (c) 2010-2014, Even Rouault <even dot rouault at mines-paris dot org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ****************************************************************************/

#ifndef CPL_VSI_VIRTUAL_H_INCLUDED
#define CPL_VSI_VIRTUAL_H_INCLUDED

#include "cpl_vsi.h"
#include "cpl_string.h"

#if defined(WIN32CE)
#  include "cpl_wince.h"
#  include <wce_errno.h>
#  pragma warning(disable:4786) /* Remove annoying warnings in eVC++ and VC++ 6.0 */
#endif

#include <map>
#include <vector>
#include <string>

/************************************************************************/
/*                           VSIVirtualHandle                           */
/************************************************************************/

class CPL_DLL VSIVirtualHandle { 
  public:
    virtual int       Seek( vsi_l_offset nOffset, int nWhence ) = 0;
    virtual vsi_l_offset Tell() = 0;
    virtual size_t    Read( void *pBuffer, size_t nSize, size_t nMemb ) = 0;
    virtual int       ReadMultiRange( int nRanges, void ** ppData, const vsi_l_offset* panOffsets, const size_t* panSizes );
    virtual size_t    Write( const void *pBuffer, size_t nSize,size_t nMemb)=0;
    virtual int       Eof() = 0;
    virtual int       Flush() {return 0;}
    virtual int       Close() = 0;
    virtual int       Truncate( vsi_l_offset nNewSize ) { return -1; }
    virtual void     *GetNativeFileDescriptor() { return NULL; }
    virtual           ~VSIVirtualHandle() { }
};

/************************************************************************/
/*                         VSIFilesystemHandler                         */
/************************************************************************/

class CPL_DLL VSIFilesystemHandler {

public:

    virtual ~VSIFilesystemHandler() {}

    virtual VSIVirtualHandle *Open( const char *pszFilename, 
                                    const char *pszAccess) = 0;
    virtual int Stat( const char *pszFilename, VSIStatBufL *pStatBuf, int nFlags) = 0;
    virtual int Unlink( const char *pszFilename )
                      { (void) pszFilename; errno=ENOENT; return -1; }
    virtual int Mkdir( const char *pszDirname, long nMode ) 
                      {(void)pszDirname; (void)nMode; errno=ENOENT; return -1;}
    virtual int Rmdir( const char *pszDirname ) 
                      { (void) pszDirname; errno=ENOENT; return -1; }
    virtual char **ReadDir( const char *pszDirname ) 
                      { (void) pszDirname; return NULL; }
    virtual int Rename( const char *oldpath, const char *newpath )
                      { (void) oldpath; (void)newpath; errno=ENOENT; return -1; }
    virtual int IsCaseSensitive( const char* pszFilename )
                      { (void) pszFilename; return TRUE; }
};

/************************************************************************/
/*                            VSIFileManager                            */
/************************************************************************/

class CPL_DLL VSIFileManager 
{
private:
    VSIFilesystemHandler *poDefaultHandler;
    std::map<std::string, VSIFilesystemHandler *> oHandlers;

    VSIFileManager();

    static VSIFileManager *Get();

public:
    ~VSIFileManager();

    static VSIFilesystemHandler *GetHandler( const char * );
    static void InstallHandler( const std::string& osPrefix, 
                                VSIFilesystemHandler * );
    static void RemoveHandler( const std::string& osPrefix );
};


/************************************************************************/
/* ==================================================================== */
/*                       VSIArchiveFilesystemHandler                   */
/* ==================================================================== */
/************************************************************************/

class VSIArchiveEntryFileOffset
{
    public:
        virtual ~VSIArchiveEntryFileOffset();
};

typedef struct
{
    char         *fileName;
    vsi_l_offset  uncompressed_size;
    VSIArchiveEntryFileOffset*      file_pos;
    int           bIsDir;
    GIntBig       nModifiedTime;
} VSIArchiveEntry;

typedef struct
{
    int nEntries;
    VSIArchiveEntry* entries;
} VSIArchiveContent;

class VSIArchiveReader
{
    public:
        virtual ~VSIArchiveReader();

        virtual int GotoFirstFile() = 0;
        virtual int GotoNextFile() = 0;
        virtual VSIArchiveEntryFileOffset* GetFileOffset() = 0;
        virtual GUIntBig GetFileSize() = 0;
        virtual CPLString GetFileName() = 0;
        virtual GIntBig GetModifiedTime() = 0;
        virtual int GotoFileOffset(VSIArchiveEntryFileOffset* pOffset) = 0;
};

class VSIArchiveFilesystemHandler : public VSIFilesystemHandler 
{
protected:
    void* hMutex;
    /* We use a cache that contains the list of files containes in a VSIArchive file as */
    /* unarchive.c is quite inefficient in listing them. This speeds up access to VSIArchive files */
    /* containing ~1000 files like a CADRG product */
    std::map<CPLString,VSIArchiveContent*>   oFileList;

    virtual const char* GetPrefix() = 0;
    virtual std::vector<CPLString> GetExtensions() = 0;
    virtual VSIArchiveReader* CreateReader(const char* pszArchiveFileName) = 0;

public:
    VSIArchiveFilesystemHandler();
    virtual ~VSIArchiveFilesystemHandler();

    virtual int      Stat( const char *pszFilename, VSIStatBufL *pStatBuf, int nFlags );
    virtual int      Unlink( const char *pszFilename );
    virtual int      Rename( const char *oldpath, const char *newpath );
    virtual int      Mkdir( const char *pszDirname, long nMode );
    virtual int      Rmdir( const char *pszDirname );
    virtual char   **ReadDir( const char *pszDirname );

    virtual const VSIArchiveContent* GetContentOfArchive(const char* archiveFilename, VSIArchiveReader* poReader = NULL);
    virtual char* SplitFilename(const char *pszFilename, CPLString &osFileInArchive, int bCheckMainFileExists);
    virtual VSIArchiveReader* OpenArchiveFile(const char* archiveFilename, const char* fileInArchiveName);
    virtual int FindFileInArchive(const char* archiveFilename, const char* fileInArchiveName, const VSIArchiveEntry** archiveEntry);
};

VSIVirtualHandle* VSICreateBufferedReaderHandle(VSIVirtualHandle* poBaseHandle);
VSIVirtualHandle* VSICreateCachedFile( VSIVirtualHandle* poBaseHandle, size_t nChunkSize = 32768, size_t nCacheSize = 0 );
VSIVirtualHandle* VSICreateGZipWritable( VSIVirtualHandle* poBaseHandle, int bRegularZLibIn, int bAutoCloseBaseHandle );

#endif /* ndef CPL_VSI_VIRTUAL_H_INCLUDED */
