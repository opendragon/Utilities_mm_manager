//--------------------------------------------------------------------------------------------------
//
//  File:       RegistryServiceLaunchThread.cpp
//
//  Project:    M+M
//
//  Contains:   The class declaration for the background Registry Service launcher.
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
//  Created:    2015-05-07
//
//--------------------------------------------------------------------------------------------------

#include "RegistryServiceLaunchThread.h"

//#include <odl/ODEnableLogging.h>
#include <odl/ODLogging.h>

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunknown-pragmas"
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)

#if (! MAC_OR_LINUX_)
# include <Windows.h>
#endif //! MAC_OR_LINUX_

/*! @file
 
 @brief The class declaration for the background Registry Service launcher. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Namespace references
#endif // defined(__APPLE__)

using namespace ChannelManager;
using namespace MplusM;
using namespace std;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Global constants and variables
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Class methods
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Constructors and Destructors
#endif // defined(__APPLE__)

RegistryServiceLaunchThread::RegistryServiceLaunchThread(const String & pathToExecutable,
                                                         const int      portNumber) :
	inherited("YARP launcher"), _registryServiceProcess(nullptr),
    _registryServicePath(pathToExecutable), _registryServicePort(portNumber)
{
    OD_LOG_ENTER(); //####
	OD_LOG_S1s("pathToExecutable = ", pathToExecutable.toStdString()); //####
    OD_LOG_LL1("portNumber = ", portNumber); //####
    OD_LOG_EXIT_P(this); //####
} // RegistryServiceLaunchThread::RegistryServiceLaunchThread

RegistryServiceLaunchThread::~RegistryServiceLaunchThread(void)
{
    OD_LOG_OBJENTER(); //####
	killChildProcess();
	_registryServiceProcess = nullptr;
    OD_LOG_OBJEXIT(); //####
} // RegistryServiceLaunchThread::~RegistryServiceLaunchThread

#if defined(__APPLE__)
# pragma mark Actions and Accessors
#endif // defined(__APPLE__)

void RegistryServiceLaunchThread::killChildProcess(void)
{
	OD_LOG_OBJENTER(); //####
	if (_registryServiceProcess)
	{
		_registryServiceProcess->kill();
	}
	OD_LOG_OBJEXIT(); //####
} // RegistryServiceLaunchThread::killChildProcess

void RegistryServiceLaunchThread::run(void)
{
    OD_LOG_OBJENTER(); //####
    _registryServiceProcess = new ChildProcess;
    if (_registryServiceProcess)
    {
        OD_LOG("(_registryServiceProcess)"); //####
        StringArray nameAndArgs(_registryServicePath);

        if (0 < _registryServicePort)
        {
            OD_LOG("(0 < _registryServicePort)"); //####
            nameAndArgs.add("--port");
            nameAndArgs.add(String(_registryServicePort));
        }
        if (_registryServiceProcess->start(nameAndArgs, 0))
        {
            OD_LOG("(_registryServiceProcess->start(nameAndArgs, 0))"); //####
            const String childOutput(_registryServiceProcess->readAllProcessOutput());
            
            _registryServiceProcess->waitForProcessToFinish(10000);
        }
    }
    OD_LOG_OBJEXIT(); //####
} // RegistryServiceLaunchThread::run

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)
