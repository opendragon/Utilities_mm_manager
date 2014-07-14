//--------------------------------------------------------------------------------------------------
//
//  File:       ChannelManagerApp.h
//
//  Project:    M+M
//
//  Contains:   The function and variable declarations for common entities for M+M clients and
//              services.
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

# include "../JuceLibraryCode/JuceHeader.h"
# include "MainWindow.h"

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file
 
 @brief The class declaration for objects thrown by exceptions within M+M. */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

/*! @brief BLORT */
class ChannelManagerApplication  : public JUCEApplication
{
public:
    
    /*! @brief BLORT */
    ChannelManagerApplication(void);
    
    /*! @brief BLORT */
    ~ChannelManagerApplication(void);
    
    /*! @brief BLORT */
    void anotherInstanceStarted(const String & commandLine) override;
    
    /*! @brief BLORT */
    const String getApplicationName(void) override;
    
    /*! @brief BLORT */
    const String getApplicationVersion(void) override;
    
    /*! @brief BLORT */
    void initialise(const String & commandLine) override;
    
    /*! @brief BLORT */
    bool moreThanOneInstanceAllowed(void) override;
    
    /*! @brief BLORT */
    void shutdown(void) override;
    
    /*! @brief BLORT */
    void systemRequestedQuit(void) override;
    
private:
    
    /*! @brief The class that this class is derived from. */
    typedef JUCEApplication inherited;
    
    /*! @brief BLORT */
    ScopedPointer<MainWindow> mainWindow;

}; // ChannelManagerApplication

#endif // ! defined(ChannelManagerApp_H_)
