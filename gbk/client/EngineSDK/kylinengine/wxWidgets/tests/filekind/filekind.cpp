///////////////////////////////////////////////////////////////////////////////
// Name:        tests/filetype/filetype.cpp
// Purpose:     Test wxGetFileKind and wxStreamBase::IsSeekable
// Author:      Mike Wetherell
// RCS-ID:      $Id: filekind.cpp,v 1.2 2005/04/25 10:32:55 MW Exp $
// Copyright:   (c) 2005 Mike Wetherell
// Licence:     wxWidgets licence
///////////////////////////////////////////////////////////////////////////////

#include "testprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#ifdef __UNIX__
    #include <sys/socket.h>
#endif

#include "wx/file.h"
#include "wx/ffile.h"
#include "wx/wfstream.h"
#include "wx/filename.h"
#include "wx/socket.h"
#include "wx/sckstrm.h"
#include "wx/mstream.h"

#if wxUSE_STREAMS

///////////////////////////////////////////////////////////////////////////////
// The test case

class FileKindTestCase : public CppUnit::TestCase
{
    CPPUNIT_TEST_SUITE(FileKindTestCase);
        CPPUNIT_TEST(File);
#if defined __UNIX__ || defined _MSC_VER || defined __MINGW32__
        CPPUNIT_TEST(Pipe);
#endif
#if defined __UNIX__
        CPPUNIT_TEST(Socket);
#endif
        CPPUNIT_TEST(Stdin);
        CPPUNIT_TEST(MemoryStream);
#if wxUSE_SOCKETS
        CPPUNIT_TEST(SocketStream);
#endif
    CPPUNIT_TEST_SUITE_END();

    void File();
    void Pipe();
    void Socket();
    void Stdin();
    void MemoryStream();
#if wxUSE_SOCKETS
    void SocketStream();
#endif

    void TestFILE(wxFFile& file, bool expected);
    void TestFd(wxFile& file, bool expected);
};

// test a wxFFile and wxFFileInput/OutputStreams of a known type
// 
void FileKindTestCase::TestFILE(wxFFile& file, bool expected)
{
    CPPUNIT_ASSERT(file.IsOpened());
    CPPUNIT_ASSERT((wxGetFileKind(file.fp()) == wxFILE_KIND_DISK) == expected);
    CPPUNIT_ASSERT((file.GetKind() == wxFILE_KIND_DISK) == expected);

    wxFFileInputStream inStream(file);
    CPPUNIT_ASSERT(inStream.IsSeekable() == expected);

    wxFFileOutputStream outStream(file);
    CPPUNIT_ASSERT(outStream.IsSeekable() == expected);
}

// test a wxFile and wxFileInput/OutputStreams of a known type
//
void FileKindTestCase::TestFd(wxFile& file, bool expected)
{
    CPPUNIT_ASSERT(file.IsOpened());
    CPPUNIT_ASSERT((wxGetFileKind(file.fd()) == wxFILE_KIND_DISK) == expected);
    CPPUNIT_ASSERT((file.GetKind() == wxFILE_KIND_DISK) == expected);

    wxFileInputStream inStream(file);
    CPPUNIT_ASSERT(inStream.IsSeekable() == expected);

    wxFileOutputStream outStream(file);
    CPPUNIT_ASSERT(outStream.IsSeekable() == expected);
}

struct TempFile
{
    ~TempFile() { if (!m_name.IsEmpty()) wxRemoveFile(m_name); }
    wxString m_name;
};

// test with an ordinary file
//
void FileKindTestCase::File()
{
    TempFile tmp; // put first
    wxFile file;
    tmp.m_name = wxFileName::CreateTempFileName(_T("wxft"), &file);
    TestFd(file, true);
    file.Close();

    wxFFile ffile(tmp.m_name);
    TestFILE(ffile, true);
}

// test with a pipe
//
#if defined __UNIX__ || defined _MSC_VER || defined __MINGW32__
void FileKindTestCase::Pipe()
{
    int afd[2];
#ifdef __UNIX__
    pipe(afd);
#else
    _pipe(afd, 256, O_BINARY);
#endif

    wxFile file0(afd[0]);
    wxFile file1(afd[1]);
    TestFd(file0, false);
    file0.Detach();

    wxFFile ffile(fdopen(afd[0], "r"));
    TestFILE(ffile, false);
}
#endif

// test with a socket
//
#if defined __UNIX__
void FileKindTestCase::Socket()
{
    int s = socket(PF_INET, SOCK_STREAM, 0);

    wxFile file(s);
    TestFd(file, false);
    file.Detach();

    wxFFile ffile(fdopen(s, "r"));
    TestFILE(ffile, false);
}
#endif

// Socket streams should be non-seekable
//
#if wxUSE_SOCKETS
void FileKindTestCase::SocketStream()
{
    wxSocketClient client;
    wxSocketInputStream inStream(client);
    CPPUNIT_ASSERT(!inStream.IsSeekable());
    wxSocketOutputStream outStream(client);
    CPPUNIT_ASSERT(!outStream.IsSeekable());

    wxBufferedInputStream nonSeekableBufferedInput(inStream);
    CPPUNIT_ASSERT(!nonSeekableBufferedInput.IsSeekable());
    wxBufferedOutputStream nonSeekableBufferedOutput(outStream);
    CPPUNIT_ASSERT(!nonSeekableBufferedOutput.IsSeekable());
}
#endif

// Memory streams should be seekable
//
void FileKindTestCase::MemoryStream()
{
    char buf[20];
    wxMemoryInputStream inStream(buf, sizeof(buf));
    CPPUNIT_ASSERT(inStream.IsSeekable());
    wxMemoryOutputStream outStream(buf, sizeof(buf));
    CPPUNIT_ASSERT(outStream.IsSeekable());

    wxBufferedInputStream seekableBufferedInput(inStream);
    CPPUNIT_ASSERT(seekableBufferedInput.IsSeekable());
    wxBufferedOutputStream seekableBufferedOutput(outStream);
    CPPUNIT_ASSERT(seekableBufferedOutput.IsSeekable());
}

// Stdin will usually be a terminal, if so then test it
// 
void FileKindTestCase::Stdin()
{
    if (isatty(0))
        CPPUNIT_ASSERT(wxGetFileKind(0) == wxFILE_KIND_TERMINAL);
    if (isatty(fileno(stdin)))
        CPPUNIT_ASSERT(wxGetFileKind(stdin) == wxFILE_KIND_TERMINAL);
}

// register in the unnamed registry so that these tests are run by default
CPPUNIT_TEST_SUITE_REGISTRATION(FileKindTestCase);

// also include in it's own registry so that these tests can be run alone
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(FileKindTestCase, "FileKindTestCase");

#endif // wxUSE_STREAMS
