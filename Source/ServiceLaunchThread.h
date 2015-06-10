//--------------------------------------------------------------------------------------------------
//
//  File:       ServiceLaunchThread.h
//
//  Project:    M+M
//
//  Contains:   The class declaration for the background service launcher.
//
//  Written by: Norman Jaffe
//
//  Copyright:  (c) 2015 by H Plus Technologies Ltd. and Simon Fraser University.
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
//  Created:    2015-05-12
//
//--------------------------------------------------------------------------------------------------

#if (! defined(ServiceLaunchThread_H_))
# define ServiceLaunchThread_H_ /* Header guard */

# include "ChannelManagerDataTypes.h"

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wunknown-pragmas"
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file
 
 @brief The class declaration for the background service launcher. */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

namespace ChannelManager
{
    /*! @brief A background service launcher. */
    class ServiceLaunchThread : public Thread
    {
    public :
        
        /*! @brief The constructor.
         @param pathToExecutable The file system path for the executable.
         @param endpointName The endpoint to use, if not the default.
         @param tag The tag to use, if any.
         @param portNumber The network port number to use.
         @param arguments The arguments to the service.
         @param needsGo @c true if a '--go' option should be used with the executable. */
        ServiceLaunchThread(const String &      pathToExecutable,
                            const String &      endpointName,
                            const String &      tag,
                            const String &      portNumber,
                            const StringArray & arguments,
                            const bool          needsGo);

        /*! @brief The destructor. */
        virtual ~ServiceLaunchThread(void);
        
		/*! @brief Force the child process to terminate. */
		void killChildProcess(void);

    protected :
        
    private :
        
        /*! @brief Perform the background scan. */
        virtual void run(void);
        
    public :
    
    protected :
    
    private :

        /*! @brief The class that this class is derived from. */
        typedef Thread inherited;
        
        /*! @brief The running background service process. */
        ScopedPointer<ChildProcess> _serviceProcess;

        /*! @brief The endpoint for the service. */
        String _serviceEndpoint;

        /*! @brief The file system path to the executable. */
        String _servicePath;

        /*! @brief The network port number to use. */
        String _servicePort;

        /*! @brief The tag for the service. */
        String _serviceTag;

        /*! @brief The arguments to the executable. */
        StringArray _arguments;

        /*! @brief @c true if the '--go' option should be used. */
        bool _needsGo;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ServiceLaunchThread)
        
    }; // ServiceLaunchThread
    
} // ChannelManager

#endif // ! defined(ServiceLaunchThread_H_)
