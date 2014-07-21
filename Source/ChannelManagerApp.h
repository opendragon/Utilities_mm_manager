//--------------------------------------------------------------------------------------------------
//
//  File:       ChannelManagerApp.h
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

#if (! defined(ChannelManagerApp_H_))
# define ChannelManagerApp_H_ /* Header guard */

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
        void anotherInstanceStarted(const String & commandLine) override;
        
        /*! @brief Return the application name.
         @returns The application's name. */
        const String getApplicationName(void) override;
        
        /*! @brief Return the application version number.
         @returns The application's version number. */
        const String getApplicationVersion(void) override;
        
        /*! @brief Called when the application starts.
         @param commandLine The parameters passed to the application. */
        void initialise(const String & commandLine) override;
        
        /*! @brief Return @c true if multiple instances of the application are allowed and @c false
         otherwise.
         @returns @c true if multiple instanaces of the application are allowed and @c false
         otherwise. */
        bool moreThanOneInstanceAllowed(void) override;
        
        /*! @brief Called to allow the application to clear up before exiting. */
        void shutdown(void) override;
        
        /*! @brief Called when the operating system is trying to close the application. */
        void systemRequestedQuit(void) override;
        
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
        
    }; // ChannelManagerApplication
    
} // ChannelManager

#endif // ! defined(ChannelManagerApp_H_)
