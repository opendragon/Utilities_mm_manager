//--------------------------------------------------------------------------------------------------
//
//  File:       ChannelManagerApplication.h
//
//  Project:    M+M
//
//  Contains:   The class declaration for the application object of the channel manager application.
//
//  Written by: Norman Jaffe
//
//  Copyright:  (c) 2014 by HPlus Technologies Ltd. and Simon Fraser University.
//
//              All rights reserved. Redistribution and use in source and binary forms, with or
//              without modification, are permitted provided that the following conditions are met:
//                * Redistributions of source code must retain the above copyright notice, this list
//                  of conditions and the following disclaimer.
//                * Redistributions in binary form must reproduce the above copyright notice, this
//                  list of conditions and the following disclaimer in the documentation and/or
//                  other materials provided with the distribution.
//                * Neither the name of the copyright holders nor the names of its contributors may
//                  be used to endorse or promote products derived from this software without
//                  specific prior written permission.
//
//              THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
//              EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
//              OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
//              SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//              INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
//              TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
//              BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//              CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
//              ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
//              DAMAGE.
//
//  Created:    2014-07-14
//
//--------------------------------------------------------------------------------------------------

#if (! defined(ChannelManagerApplication_H_))
# define ChannelManagerApplication_H_ /* Header guard */

# include "ChannelManagerWindow.h"

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wc++11-extensions"
#  pragma clang diagnostic ignored "-Wdocumentation"
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#  pragma clang diagnostic ignored "-Wpadded"
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wweak-vtables"
# endif // defined(__APPLE__)
# include <yarp/os/Network.h>
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file
 
 @brief The class declaration for the application object of the channel manager application. */

/*! @dir Source
 @brief The set of files that support managing channels. */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

namespace ChannelManager
{
    class PeekInputHandler;
    class ScannerThread;
    
    /*! @brief The application object of the application. */
    class ChannelManagerApplication  : public JUCEApplication
    {
    public:
        
        /*! @brief The constructor. */
        ChannelManagerApplication(void);
        
        /*! @brief The destructor. */
        virtual ~ChannelManagerApplication(void);
        
        /*! @brief Called when an attempt was made to launch another instance of the application.
         @param commandLine The arguments passed to the new instance. */
        void anotherInstanceStarted(const String & commandLine);
        
        /*! @brief Connect (or reconnect) the peek channel to the Service Registry. */
        void connectPeekChannel(void);
        
        /*! @brief Indicate that a scan should be performed as soon as possible. */
        void doScanSoon(void);
        
        /*! @brief Return the application name.
         @returns The application's name. */
        virtual const String getApplicationName(void);
        
        /*! @brief Return the application version number.
         @returns The application's version number. */
        virtual const String getApplicationVersion(void);
        
        /*! @brief Called when the application starts.
         @param commandLine The parameters passed to the application. */
        virtual void initialise(const String & commandLine);
        
        /*! @brief Return @c true if multiple instances of the application are allowed and @c false
         otherwise.
         @returns @c true if multiple instanaces of the application are allowed and @c false
         otherwise. */
        virtual bool moreThanOneInstanceAllowed(void);
        
        /*! @brief Called to allow the application to clear up before exiting. */
        virtual void shutdown(void);
        
        /*! @brief Called when the operating system is trying to close the application. */
        virtual void systemRequestedQuit(void);
        
    protected:
        
    private:
        
        /*! @brief The class that this class is derived from. */
        typedef JUCEApplication inherited;
        
        /*! @brief The primary window of the application. */
        ScopedPointer<ChannelManagerWindow> _mainWindow;
        
        /*! @brief Used to establish connections to the YARP infrastructure. */
        ScopedPointer<yarp::os::Network> _yarp;
        
        /*! @brief The background scanner thread. */
        ScopedPointer<ScannerThread> _scanner;
        
        /*! @brief A channel to watch the Service Registry status. */
        MplusM::Common::AdapterChannel * _peeker;
        
        /*! @brief The input handler for the 'peek' channel. */
        PeekInputHandler * _peekHandler;
        
    }; // ChannelManagerApplication
    
} // ChannelManager

/*! @brief Return @c true if exit is requested.
 @param stuff Dummy argument to satisfy caller.
 @returns @c true if exit has been requested. */
bool CheckForExit(void * stuff);

/*! @brief Indicate that an exit has been requested. */
void SetExitRequest(void);

#endif // ! defined(ChannelManagerApplication_H_)