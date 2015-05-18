//--------------------------------------------------------------------------------------------------
//
//  File:       ChannelManagerApplication.cpp
//
//  Project:    M+M
//
//  Contains:   The class definition for the application object of the channel manager application.
//
//  Written by: Norman Jaffe
//
//  Copyright:  (c) 2014 by H Plus Technologies Ltd. and Simon Fraser University.
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

#include "ChannelManagerApplication.h"
#include "EntitiesPanel.h"
#include "GeneralServiceLaunchThread.h"
#include "PeekInputHandler.h"
#include "RegistryServiceLaunchThread.h"
#include "ScannerThread.h"
#include "YarpLaunchThread.h"

#include <mpm/M+MRequests.h>

//#include <odl/ODEnableLogging.h>
#include <odl/ODLogging.h>

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunknown-pragmas"
# pragma clang diagnostic ignored "-Wc++11-extensions"
# pragma clang diagnostic ignored "-Wconversion"
# pragma clang diagnostic ignored "-Wdeprecated-declarations"
# pragma clang diagnostic ignored "-Wdocumentation"
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# pragma clang diagnostic ignored "-Wextern-c-compat"
# pragma clang diagnostic ignored "-Wextra-semi"
# pragma clang diagnostic ignored "-Wpadded"
# pragma clang diagnostic ignored "-Wshadow"
# pragma clang diagnostic ignored "-Wsign-conversion"
# pragma clang diagnostic ignored "-Wunused-parameter"
# pragma clang diagnostic ignored "-Wweak-vtables"
#endif // defined(__APPLE__)
#include <yarp/os/impl/SystemInfo.h>
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)
#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunknown-pragmas"
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 
 @brief The class definition for the application object of the channel manager application. */
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
/*! @brief The system environment variable table. */
extern char * * environ;
#endif // __APPLE__

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

/*! @brief @c true if an exit has been requested and @c false otherwise. */
static bool lExitRequested = false;

/*! @brief The number of milliseconds before a thread is force-killed. */
static const int kThreadKillTime = 5000;

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

ChannelManagerApplication::ChannelManagerApplication(void) :
    inherited(), _mainWindow(nullptr), _yarp(nullptr), _scanner(nullptr),
    _registryServiceLauncher(nullptr), _yarpLauncher(nullptr), _peeker(nullptr),
    _peekHandler(nullptr), _registryServiceCanBeLaunched(false)
{
#if defined(MpM_ServicesLogToStandardError)
    OD_LOG_INIT(ProjectInfo::projectName, kODLoggingOptionIncludeProcessID | //####
                kODLoggingOptionIncludeThreadID | kODLoggingOptionWriteToStderr | //####
                kODLoggingOptionEnableThreadSupport); //####
#else // ! defined(MpM_ServicesLogToStandardError)
    OD_LOG_INIT(ProjectInfo::projectName, kODLoggingOptionIncludeProcessID | //####
                kODLoggingOptionIncludeThreadID | kODLoggingOptionEnableThreadSupport); //####
#endif // ! defined(MpM_ServicesLogToStandardError)
    OD_LOG_ENTER(); //####
    OD_LOG_EXIT_P(this); //####
} // ChannelManagerApplication::ChannelManagerApplication

ChannelManagerApplication::~ChannelManagerApplication(void)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT(); //####
} // ChannelManagerApplication::~ChannelManagerApplication

#if defined(__APPLE__)
# pragma mark Actions and Accessors
#endif // defined(__APPLE__)

#if (! MAC_OR_LINUX_)
# pragma warning(push)
# pragma warning(disable: 4100)
#endif // ! MAC_OR_LINUX_
void ChannelManagerApplication::anotherInstanceStarted(const String & commandLine)
{
#if (! defined(OD_ENABLE_LOGGING))
# if MAC_OR_LINUX_
#  pragma unused(commandLine)
# endif // MAC_OR_LINUX_
#endif // ! defined(OD_ENABLE_LOGGING)
    OD_LOG_OBJENTER(); //####
    OD_LOG_S1s("commandLine = ", commandLine.toStdString()); //####
    // When another instance of the app is launched while this one is running,
    // this method is invoked, and the commandLine parameter tells you what
    // the other instance's command-line arguments were.
    OD_LOG_OBJEXIT(); //####
} // ChannelManagerApplication::anotherInstanceStarted
#if (! MAC_OR_LINUX_)
# pragma warning(pop)
#endif // ! MAC_OR_LINUX_

bool ChannelManagerApplication::checkForRegistryServiceAndLaunchIfDesired(void)
{
    OD_LOG_OBJENTER(); //####
    bool                     didLaunch = false;
#if MAC_OR_LINUX_
    yarp::os::impl::Logger & theLogger = Common::GetLogger();
#endif // MAC_OR_LINUX_

    if (0 < _registryServicePath.length())
    {
        // If the Registry Service is installed, give the option of running it.
        if (validateRegistryService())
        {
            didLaunch = doLaunchRegistry();
        }
    }
    else
    {
        // If YARP isn't installed, say so and leave.
        AlertWindow::showMessageBox(AlertWindow::WarningIcon, getApplicationName(),
                                    "A Registry Service executable could not be found in the PATH "
                                    "system environment variable. "
                                    "Launching the Registry Service is not possible.",
                                    String::empty, _mainWindow);
    }
    OD_LOG_OBJEXIT_B(didLaunch); //####
    return didLaunch;
} // ChannelManagerApplication::checkForRegistryServiceAndLaunchIfDesired

yarp::os::Network * ChannelManagerApplication::checkForYarpAndLaunchIfDesired(void)
{
    OD_LOG_OBJENTER(); //####
    yarp::os::Network *      result = nullptr;
#if MAC_OR_LINUX_
    yarp::os::impl::Logger & theLogger = Common::GetLogger();
#endif // MAC_OR_LINUX_
    
    // No running YARP server was detected - first check if YARP is actually available:
    if (0 < _yarpPath.length())
    {
        // If YARP is installed, give the option of running a private copy.
        if (validateYarp())
        {
            char                     ipBuffer[INET_ADDRSTRLEN + 1];
            const char *             ipAddressAsString = nullptr;
            struct in_addr           serverAddress;
            int                      serverPort = 10000;
            String                   selectedIpAddress;
            String                   serverPortAsString;
            Common::YarpStringVector ipAddressVector;
            
#if MAC_OR_LINUX_
            theLogger.warning("Private YARP network being launched.");
#endif // MAC_OR_LINUX_
            if (Utilities::GetCurrentYarpConfiguration(serverAddress, serverPort))
            {
                if (INADDR_NONE != serverAddress.s_addr)
                {
                    serverPortAsString = String(serverPort);
                    ipAddressAsString = inet_ntop(AF_INET, &serverAddress.s_addr, ipBuffer,
                                                  sizeof(ipBuffer));
                    _configuredYarpAddress = ipAddressAsString;
                    _configuredYarpPort = serverPort;
                }
            }
			Utilities::GetMachineIPs(ipAddressVector);
            int         initialSelection = 0;
            int         portChoice = serverPort;
            int         res = 0;
            StringArray ipAddressArray;
            String      appName(JUCEApplication::getInstance()->getApplicationName());
            AlertWindow ww(appName, "Please select the IP address and port to be used to start the "
                           "private YARP network:", AlertWindow::QuestionIcon, _mainWindow);
            
            for (unsigned ii = 0; ipAddressVector.size() > ii; ++ii)
            {
                Common::YarpString anAddress(ipAddressVector[ii]);
                
                ipAddressArray.add(anAddress.c_str());
                if (anAddress == ipAddressAsString)
                {
                    initialSelection = ii;
                }
            }
            ww.addComboBox("IPAddress", ipAddressArray, "IP address:");
            ComboBox * cBox = ww.getComboBoxComponent("IPAddress");
            
            cBox->setSelectedItemIndex(initialSelection);
            ww.addTextEditor("NetworkPort", serverPortAsString, "Network port:");
            ww.addButton("OK", 1, KeyPress(KeyPress::returnKey, 0, 0));
            ww.addButton("Cancel", 0, KeyPress(KeyPress::escapeKey, 0, 0));
            for (bool keepGoing = true; keepGoing; )
            {
                res = ww.runModalLoop();
                if (1 == res)
                {
                    int    addressIndexChosen = cBox->getSelectedItemIndex();
                    String portText = ww.getTextEditorContents("NetworkPort");

                    selectedIpAddress = ipAddressArray[addressIndexChosen];
                    portChoice = portText.getIntValue();
                    if (Utilities::ValidPortNumber(portChoice))
                    {
                        keepGoing = false;
                    }
                    else
                    {
                        AlertWindow::showMessageBox(AlertWindow::WarningIcon, getApplicationName(),
                                                    "The port number is invalid. "
                                                    "Please enter a value between 1024 and 65535",
                                                    String::empty, _mainWindow);
                    }
                }
                else
                {
                    keepGoing = false;
                }
            }
            if (1 == res)
            {
                _yarpLauncher = new YarpLaunchThread(_yarpPath, selectedIpAddress, portChoice);
                if (_yarpLauncher)
                {
                    _yarpLauncher->startThread();
                    // Sleep for a little while and recheck if YARP is active.
                    for ( ; ! result; )
                    {
                        Thread::sleep(100);
                        if (_yarpLauncher->isThreadRunning())
                        {
                            result = new yarp::os::Network;
                        }
                        else
                        {
                            _yarpLauncher = nullptr;
                            break;
                        }
                        
                    }
                }
            }
        }
    }
    else
    {
        // If YARP isn't installed, say so and leave.
        AlertWindow::showMessageBox(AlertWindow::WarningIcon, getApplicationName(),
                                    "No YARP network was detected and a YARP executable could not "
                                    "be found in the PATH system environment variable. "
                                    "Execution is not possible.", String::empty, _mainWindow);
    }
    OD_LOG_OBJEXIT_P(result); //####
    return result;
} // ChannelManagerApplication::checkForYarpAndLaunchIfDesired

void ChannelManagerApplication::connectPeekChannel(void)
{
    OD_LOG_OBJENTER(); //####
    if (_peeker)
    {
        if (! Utilities::NetworkConnectWithRetries(MpM_REGISTRY_STATUS_NAME, _peeker->name(),
                                                   STANDARD_WAIT_TIME))
        {
            OD_LOG("(! Utilities::NetworkConnectWithRetries(MpM_REGISTRY_STATUS_NAME, " //####
                   "_peeker->name(), STANDARD_WAIT_TIME))"); //####
        }
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelManagerApplication::connectPeekChannel

void ChannelManagerApplication::doCleanupSoon(void)
{
    OD_LOG_OBJENTER(); //####
    if (_scanner)
    {
        _scanner->doCleanupSoon();
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelManagerApplication::doCleanupSoon

bool ChannelManagerApplication::doLaunchAnAdapter(const ApplicationInfo & appInfo)
{
    OD_LOG_OBJENTER(); //####
    bool                     result = false;
#if MAC_OR_LINUX_
    yarp::os::impl::Logger & theLogger = Common::GetLogger();
#endif // MAC_OR_LINUX_
    int                      res = 0;
    String                   appName(JUCEApplication::getInstance()->getApplicationName());
    AlertWindow              ww(appName, "Please select the network port to be used to start the "
                                "Registry Service (enter 0 to use the default port):",
                                AlertWindow::NoIcon, _mainWindow);
    
#if MAC_OR_LINUX_
    theLogger.warning("Adapter being launched.");
#endif // MAC_OR_LINUX_

    // Apply the '--channels' option to the executable to get the number of channels and their
    // default values.
    // Set up the text fields for the dialog box.
    
    // getChannelsForAdapter()
    
    //ww.addTextEditor("NetworkPort", "0", "Network port [0 for the default port]:");
    
    
    
    ww.addButton("OK", 1, KeyPress(KeyPress::returnKey, 0, 0));
    ww.addButton("Cancel", 0, KeyPress(KeyPress::escapeKey, 0, 0));
    
    
    
    for (bool keepGoing = true; keepGoing; )
    {
        res = ww.runModalLoop();
        if (1 == res)
        {
            // Get the command-line arguments, apply to the '--channels' option and check if any of
            // the channels are already known - flag them if so and go again.
            
            // getChannelsForAdapter()
            
//            String portText = ww.getTextEditorContents("NetworkPort");
//            
//            portChoice = portText.getIntValue();
//            if ((! portChoice) || Utilities::ValidPortNumber(portChoice))
//            {
//                keepGoing = false;
//            }
//            else
//            {
//                AlertWindow::showMessageBox(AlertWindow::WarningIcon, getApplicationName(),
//                                            "The port number is invalid. "
//                                            "Please enter a value between 1024 and 65535",
//                                            String::empty, _mainWindow);
//            }
        }
        else
        {
            keepGoing = false;
        }
    }
    if (1 == res)
    {
        // If we get here, the command-line arguments are useable and the user has said 'go ahead'.
        
//        _registryServiceLauncher = new RegistryServiceLaunchThread(_registryServicePath,
//                                                                   portChoice);
//        if (_registryServiceLauncher)
//        {
//            _registryServiceLauncher->startThread();
//            _registryServiceCanBeLaunched = false;
//            result = true;
//        }
    }
    
    OD_LOG_OBJEXIT_B(result); //####
    return result;
} // ChannelManagerApplication::doLaunchAnAdapter

bool ChannelManagerApplication::doLaunchAService(const ApplicationInfo & appInfo)
{
    OD_LOG_OBJENTER(); //####
    bool result = false;
    
    // We need to check the endpoint used by the service, using the '--endpoint'
    
    OD_LOG_OBJEXIT_B(result); //####
    return result;
} // ChannelManagerApplication::doLaunchAService

bool ChannelManagerApplication::doLaunchOtherApplication(void)
{
    OD_LOG_OBJENTER(); //####
    bool      result = false;
    int       res;

    res = _applicationMenu.show();
    if (0 < res)
    {
        const ApplicationInfo & appInfo = _applicationList.at(res - 1);
        
        if (kApplicationAdapter == appInfo._kind)
        {
            result = doLaunchAnAdapter(appInfo);
        }
        else if (kApplicationService == appInfo._kind)
        {
            result = doLaunchAService(appInfo);
        }
    }
    OD_LOG_OBJEXIT_B(result); //####
    return result;
} // ChannelManagerApplication::doLaunchOtherApplication

bool ChannelManagerApplication::doLaunchRegistry(void)
{
    OD_LOG_OBJENTER(); //####
    bool                     result = false;
#if MAC_OR_LINUX_
    yarp::os::impl::Logger & theLogger = Common::GetLogger();
#endif // MAC_OR_LINUX_
    int                      portChoice = 0;
    int                      res = 0;
    String                   appName(JUCEApplication::getInstance()->getApplicationName());
    AlertWindow              ww(appName, "Please select the network port to be used to start the "
                                "Registry Service (enter 0 to use the default port):",
                                AlertWindow::QuestionIcon, _mainWindow);
    
#if MAC_OR_LINUX_
    theLogger.warning("Registry Service being launched.");
#endif // MAC_OR_LINUX_
    ww.addTextEditor("NetworkPort", "0", "Network port [0 for the default port]:");
    ww.addButton("OK", 1, KeyPress(KeyPress::returnKey, 0, 0));
    ww.addButton("Cancel", 0, KeyPress(KeyPress::escapeKey, 0, 0));
    for (bool keepGoing = true; keepGoing; )
    {
        res = ww.runModalLoop();
        if (1 == res)
        {
            String portText = ww.getTextEditorContents("NetworkPort");
            
            portChoice = portText.getIntValue();
            if ((! portChoice) || Utilities::ValidPortNumber(portChoice))
            {
                keepGoing = false;
            }
            else
            {
                AlertWindow::showMessageBox(AlertWindow::WarningIcon, getApplicationName(),
                                            "The port number is invalid. "
                                            "Please enter a value between 1024 and 65535",
                                            String::empty, _mainWindow);
            }
        }
        else
        {
            keepGoing = false;
        }
    }
    if (1 == res)
    {
        _registryServiceLauncher = new RegistryServiceLaunchThread(_registryServicePath,
                                                                   portChoice);
        if (_registryServiceLauncher)
        {
            _registryServiceLauncher->startThread();
            _registryServiceCanBeLaunched = false;
            result = true;
        }
    }
    OD_LOG_OBJEXIT_B(result); //####
    return result;
} // ChannelManagerApplication::doLaunchRegistry

void ChannelManagerApplication::doScanSoon(void)
{
    OD_LOG_OBJENTER(); //####
    if (_scanner)
    {
        _scanner->doScanSoon();
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelManagerApplication::doScanSoon

String ChannelManagerApplication::findPathToExecutable(const String & execName)
{
    OD_LOG_ENTER(); //####
    OD_LOG_S1s("execName = ", execName.toStdString()); //####
    String result;
    
    if (juce::File::isAbsolutePath(execName))
    {
        bool       doCheck = false;
        juce::File aFile(juce::File::createFileWithoutCheckingPath(execName));
        
        if (aFile.existsAsFile())
        {
            doCheck = true;
        }
#if (! MAC_OR_LINUX_)
        else
        {
            String temp(aFile.getFileNameWithoutExtension() + ".exe");
            
            aFile = juce::File::createFileWithoutCheckingPath(temp);
            if (aFile.existsAsFile())
            {
                doCheck = true;
            }
        }
#endif // ! MAC_OR_LINUX_
        if (doCheck)
        {
            String temp(aFile.getFullPathName());
            
#if MAC_OR_LINUX_
            if (! access(temp.toStdString().c_str(), X_OK))
#else // ! MAC_OR_LINUX_
            if (! _access(temp.toStdString().c_str(), 0)) // Note that there's no explicit check for
                                                          // execute permission in Windows
#endif // ! MAC_OR_LINUX_
            {
                // We've found an executable that we can use!
                result = temp;
            }
        }
    }
    else
    {
        String pathVar(getEnvironmentVar("PATH"));
        
        if (pathVar.isNotEmpty())
        {
            for ( ; pathVar.isNotEmpty(); )
            {
                bool       doCheck = false;
                juce::File aFile;
                String     pathToCheck;
#if MAC_OR_LINUX_
                int        indx = pathVar.indexOfChar(':');
#else // ! MAC_OR_LINUX_
                int        indx = pathVar.indexOfChar(';');
#endif // ! MAC_OR_LINUX_
                
                if (-1 == indx)
                {
                    pathToCheck = pathVar;
                    pathVar.clear();
                }
                else
                {
                    pathToCheck = pathVar.substring(0, indx);
                    pathVar = pathVar.substring(indx + 1);
                }
                pathToCheck = juce::File::addTrailingSeparator(pathToCheck);
                aFile = juce::File::createFileWithoutCheckingPath(pathToCheck + execName);
                if (aFile.existsAsFile())
                {
                    doCheck = true;
                }
#if (! MAC_OR_LINUX_)
                else
                {
                    String temp(aFile.getFileNameWithoutExtension() + ".exe");

                    aFile = juce::File::createFileWithoutCheckingPath(temp));
                    if (aFile.existsAsFile())
                    {
                        doCheck = true;
                    }
                }
#endif // ! MAC_OR_LINUX_
                if (doCheck)
                {
                    String temp(aFile.getFullPathName());
                    
#if MAC_OR_LINUX_
                    if (! access(temp.toStdString().c_str(), X_OK))
#else // ! MAC_OR_LINUX_
                    if (! _access(temp.toStdString().c_str(), 0)) // Note that there's no explicit
                                                                  // check for execute permission in
                                                                  // Windows
#endif // ! MAC_OR_LINUX_
                    {
                        // We've found an executable that we can use!
                        result = temp;
                    }
                }
            }
        }
    }
    OD_LOG_EXIT_s(result.toStdString()); //####
    return result;
} // ChannelManagerApplication::findPathToExecutable

ChannelManagerApplication * ChannelManagerApplication::getApp(void)
{
    OD_LOG_ENTER(); //####
    ChannelManagerApplication * result =
                        static_cast<ChannelManagerApplication *>(JUCEApplication::getInstance());

    OD_LOG_EXIT_P(result); //####
    return result;
} // ChannelManagerApplication::getApp

const String ChannelManagerApplication::getApplicationName(void)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT_S(ProjectInfo::projectName); //####
    return ProjectInfo::projectName;
} // ChannelManagerApplication::getApplicationName

const String ChannelManagerApplication::getApplicationVersion(void)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT_S(ProjectInfo::versionString); //####
    return ProjectInfo::versionString;
} // ChannelManagerApplication::getApplicationVersion

bool ChannelManagerApplication::getArgumentsForApplication(const String &    execName,
                                                           ApplicationInfo & theInfo)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_S1s("execName = ", execName.toStdString()); //####
    OD_LOG_P1("theInfo = ", &theInfo); //####
    bool   okSoFar = false;
    String execPath(findPathToExecutable(execName));

    if (0 < execPath.length())
    {
        ChildProcess runApplication;
        StringArray  nameAndArgs(findPathToExecutable(execName));

        nameAndArgs.add("--args");
        if (runApplication.start(nameAndArgs))
        {
            const String childOutput(runApplication.readAllProcessOutput());

            runApplication.waitForProcessToFinish(kThreadKillTime);
            if (0 < childOutput.length())
            {
                StringArray aRecord(StringArray::fromTokens(childOutput, "\t", ""));

                // The input lines should be composed of tab-separated argument descriptions.
                for (int ii = 0, mm = aRecord.size(); mm > ii; ++ii)
                {
                    Common::YarpString                  argString(aRecord[ii].toStdString());
                    Utilities::BaseArgumentDescriptor * argDesc =
                                                    Utilities::ConvertStringToArgument(argString);

                    if (argDesc)
                    {
                        theInfo._argDescriptions.push_back(argDesc);
                    }
                }
                okSoFar = true;
            }
        }
    }
    OD_LOG_OBJEXIT_B(okSoFar); //####
    return okSoFar;
} // ChannelManagerApplication::getArgumentsForApplication

String ChannelManagerApplication::getEnvironmentVar(const char * varName)
{
    OD_LOG_ENTER(); //####
    OD_LOG_S1("varName = ", varName); //####
    String                        result;
    JuceStringMap                 envVars(getEnvironmentVars());
    JuceStringMap::const_iterator it(envVars.find(varName));
    
    if (envVars.end() != it)
    {
        result = it->second;
    }
    OD_LOG_EXIT_s(result.toStdString()); //####
    return result;
} // ChannelManagerApplication::getEnvironmentVar

ChannelManagerApplication::JuceStringMap ChannelManagerApplication::getEnvironmentVars(void)
{
    OD_LOG_ENTER(); //####
    JuceStringMap      result;
#if defined(__APPLE__)
    char *             varChar = *environ;
#else // ! defined(__APPLE__)
    yarp::os::Property vars(yarp::os::impl::SystemInfo::getPlatformInfo().environmentVars);
    Common::YarpString varsAsString(vars.toString());
    yarp::os::Bottle   varsAsBottle(varsAsString);
#endif // ! defined(__APPLE__)

#if defined(__APPLE__)
    for (int ii = 0; varChar; ++ii)
    {
        std::string tmpVariable(varChar);
        size_t      equalsSign = tmpVariable.find("=");
        
        if (equalsSign != std::string::npos)
        {
            result.insert(JuceStringMap::value_type(tmpVariable.substr(0, equalsSign),
                                                    tmpVariable.substr(equalsSign + 1)));
        }
        varChar = *(environ + ii);
    }
#else // ! defined(__APPLE__)
    for (int ii = 0, numVars = varsAsBottle.size(); numVars > ii; ++ii)
    {
        yarp::os::Value & aValue = varsAsBottle.get(ii);
        
        if (aValue.isList())
        {
            yarp::os::Bottle * asList = aValue.asList();
            
            if (asList && (2 == asList->size()))
            {
                yarp::os::Value & keyValue = asList->get(0);
                yarp::os::Value & valueValue = asList->get(1);
                
                if (keyValue.isString() && valueValue.isString())
                {
                    YarpString keyString = keyValue.asString();
                    YarpString valueString = valueValue.asString();
                    
                    if ((0 < keyString.length()) && (0 < valueString.length()))
                    {
                        result.insert(JuceStringMap::value_type(keyString.c_str(),
                                                                valueString.c_str()));
                    }
                }
            }
        }
    }
#endif // ! defined(__APPLE__)
    OD_LOG_EXIT(); //####
    return result;
} // ChannelManagerApplication::getEnvironmentVars

String ChannelManagerApplication::getHomeDir(void)
{
    OD_LOG_ENTER(); //####
    juce::File homeDir(juce::File::getSpecialLocation(juce::File::userHomeDirectory));
    String     result(homeDir.getFullPathName());
    
    OD_LOG_EXIT_s(result.toStdString()); //####
    return result;
} // ChannelManagerApplication::getHomeDir

void ChannelManagerApplication::getChannelsForAdapter(const String & execName,
                                                      const String & arguments)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_S2s("execName = ", execName.toStdString(), "arguments = ", arguments.toStdString()); //####
    
    OD_LOG_OBJEXIT(); //####
} // ChannelManagerApplication::getChannelsForAdapter

bool ChannelManagerApplication::getParametersForApplication(const String &    execName,
                                                            ApplicationInfo & theInfo)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_S1s("execName = ", execName.toStdString()); //####
    OD_LOG_P1("theInfo = ", &theInfo); //####
    bool   okSoFar = false;
    String execPath(findPathToExecutable(execName));
    
    if (0 < execPath.length())
    {
        ChildProcess runApplication;
        StringArray  nameAndArgs(findPathToExecutable(execName));
        
        nameAndArgs.add("--info");
        if (runApplication.start(nameAndArgs))
        {
            const String childOutput(runApplication.readAllProcessOutput());
            
            runApplication.waitForProcessToFinish(kThreadKillTime);
            if (0 < childOutput.length())
            {
                StringArray aRecord(StringArray::fromTokens(childOutput, "\t", ""));
                
                // The input lines should be composed of three tab-separated items:
                // 0) Type ('Service' or 'Adapter')
                // 1) Allowed options (if 'Service') or matching criteria (if 'Adapter')
                // 2) Description
                if (3 <= aRecord.size())
                {
                    String execKind(aRecord[0]);
                    
                    if (execKind == "Adapter")
                    {
                        theInfo._kind = kApplicationAdapter;
                        okSoFar = true;
                    }
                    else if (execKind == "Service")
                    {
                        theInfo._kind = kApplicationService;
                        okSoFar = true;
                    }
                    if (okSoFar)
                    {
                        theInfo._applicationPath = nameAndArgs[0];
                        theInfo._criteriaOrOptions = aRecord[1];
                        theInfo._description = aRecord[2];
                        theInfo._shortName = execName;
                    }
                }
            }
        }
    }
    OD_LOG_OBJEXIT_B(okSoFar); //####
    return okSoFar;
} // ChannelManagerApplication::getParametersForApplication

String ChannelManagerApplication::getRealName(void)
{
    OD_LOG_ENTER(); //####
    String          result;
#if defined(__APPLE__)
    struct passwd   pwd;
    struct passwd * pwdPtr = NULL;
    char *          buf;
    long            bufSize = sysconf(_SC_GETPW_R_SIZE_MAX);
#endif // defined(__APPLE__)
    
    // Note that yarp::os::impl::SystemInfo::getUserInfo() does nothing in Mac OS X!
#if defined(__APPLE__)
    if (-1 == bufSize)
    {
        // Value was indeterminate.
        bufSize = 16384; // Should be more than enough.
    }
    buf = static_cast<char *>(malloc(bufSize));
    if (buf)
    {
        getpwuid_r(getuid(), &pwd, buf, bufSize, &pwdPtr);
        if (pwdPtr)
        {
            result = pwd.pw_gecos;
        }
        free(buf);
    }
#else // ! defined(__APPLE__)
    result = yarp::os::impl::SystemInfo::getUserInfo().realName;
#endif // ! defined(__APPLE__)
    OD_LOG_EXIT_s(result.toStdString()); //####
    return result;
} // ChannelManagerApplication::getRealName

String ChannelManagerApplication::getUserName(void)
{
    OD_LOG_ENTER(); //####
    String          result;
#if defined(__APPLE__)
    struct passwd   pwd;
    struct passwd * pwdPtr = NULL;
    char *          buf;
    long            bufSize = sysconf(_SC_GETPW_R_SIZE_MAX);
#endif // defined(__APPLE__)
    
    // Note that yarp::os::impl::SystemInfo::getUserInfo() does nothing in Mac OS X!
#if defined(__APPLE__)
    if (-1 == bufSize)
    {
        // Value was indeterminate.
        bufSize = 16384; // Should be more than enough.
    }
    buf = static_cast<char *>(malloc(bufSize));
    if (buf)
    {
        getpwuid_r(getuid(), &pwd, buf, bufSize, &pwdPtr);
        if (pwdPtr)
        {
            result = pwd.pw_name;
        }
        free(buf);
    }
#else // ! defined(__APPLE__)
    result = yarp::os::impl::SystemInfo::getUserInfo().userName;
#endif // ! defined(__APPLE__)
    OD_LOG_EXIT_s(result.toStdString()); //####
    return result;
} // ChannelManagerApplication::getUserName

#if (! MAC_OR_LINUX_)
# pragma warning(push)
# pragma warning(disable: 4100)
#endif // ! MAC_OR_LINUX_
void ChannelManagerApplication::initialise(const String & commandLine)
{
#if (! defined(OD_ENABLE_LOGGING))
# if MAC_OR_LINUX_
#  pragma unused(commandLine)
# endif // MAC_OR_LINUX_
#endif // ! defined(OD_ENABLE_LOGGING)
    OD_LOG_OBJENTER(); //####
    OD_LOG_S1s("commandLine = ", commandLine.toStdString()); //####
    bool launchedRegistry = false;
    
#if MAC_OR_LINUX_
    Common::SetUpLogger(ProjectInfo::projectName);
#endif // MAC_OR_LINUX_
    Common::Initialize(ProjectInfo::projectName);
    Utilities::SetUpGlobalStatusReporter();
#if defined(MpM_ReportOnConnections)
    ChannelStatusReporter * reporter = Utilities::GetGlobalStatusReporter();
#endif // defined(MpM_ReportOnConnections)

    Utilities::CheckForNameServerReporter();
    loadApplicationLists();
    _mainWindow = new ChannelManagerWindow(ProjectInfo::projectName);
    if (Utilities::CheckForValidNetwork(true))
    {
        _yarp = new yarp::os::Network; // This is necessary to establish any connections to the YARP
                                       // infrastructure
    }
    else
    {
        OD_LOG("! (yarp::os::Network::checkNetwork())"); //####
#if MAC_OR_LINUX_
        yarp::os::impl::Logger & theLogger = Common::GetLogger();
#endif // MAC_OR_LINUX_

#if MAC_OR_LINUX_
        theLogger.warning("YARP network not running.");
#endif // MAC_OR_LINUX_
        _yarpPath = findPathToExecutable("yarp");
        _yarp = checkForYarpAndLaunchIfDesired();
    }
    if (_yarp)
    {
        if (! Utilities::CheckForRegistryService())
        {
            _registryServicePath = findPathToExecutable(MpM_REGISTRY_EXECUTABLE_NAME);
            launchedRegistry = checkForRegistryServiceAndLaunchIfDesired();
        }
        EntitiesPanel & entities = _mainWindow->getEntitiesPanel();
        
        entities.recallPositions();
        _peeker = new Common::AdapterChannel(false);
        _peekHandler = new PeekInputHandler;
        if (_peeker && _peekHandler)
        {
#if defined(MpM_ReportOnConnections)
            _peeker->setReporter(reporter);
            _peeker->getReport(reporter);
#endif // defined(MpM_ReportOnConnections)
            Common::YarpString peekName = Common::GetRandomChannelName(HIDDEN_CHANNEL_PREFIX
                                                                       "peek_/"
                                                                       DEFAULT_CHANNEL_ROOT);
            
            if (_peeker->openWithRetries(peekName, STANDARD_WAIT_TIME))
            {
                _peeker->setReader(*_peekHandler);
                _scanner = new ScannerThread(*_mainWindow, launchedRegistry);
                _scanner->startThread();
            }
        }
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelManagerApplication::initialise
#if (! MAC_OR_LINUX_)
# pragma warning(pop)
#endif // ! MAC_OR_LINUX_

void ChannelManagerApplication::loadApplicationLists(void)
{
    OD_LOG_OBJENTER(); //####
    juce::File  commonDir(juce::File::getSpecialLocation(juce::File::commonDocumentsDirectory));
    String      commonPath(juce::File::addTrailingSeparator(commonDir.getFullPathName()) + "M+M");
    String      pathToStdList(juce::File::addTrailingSeparator(commonPath) +
                              "standardApplications.txt");
    String      pathToCustomList(juce::File::addTrailingSeparator(commonPath) +
                                 "customApplications.txt");
    juce::File  stdListFile(pathToStdList);
    juce::File  customListFile(pathToCustomList);
    StringArray lines;
    
    if (stdListFile.existsAsFile())
    {
        stdListFile.readLines(lines);
    }
    if (customListFile.existsAsFile())
    {
        customListFile.readLines(lines);
    }
    _applicationMenu.addSectionHeader("Applications available:");
    _applicationMenu.addSeparator();
    for (int ii = 0, idx = 0, mm = lines.size(); mm > ii; ++ii)
    {
        String aLine(lines[ii]);
        
        if ('#' != aLine[0])
        {
            ApplicationInfo theInfo;

            if (getParametersForApplication(aLine, theInfo))
            {
                if (getArgumentsForApplication(aLine, theInfo))
                {
                    _applicationList.push_back(theInfo);
                    _applicationMenu.addItem(++idx, theInfo._description);
                }
            }
        }
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelManagerApplication::loadApplicationLists

bool ChannelManagerApplication::moreThanOneInstanceAllowed(void)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT_B(true); //####
    return true;
} // ChannelManagerApplication::moreThanOneInstanceAllowed

void ChannelManagerApplication::shutdown(void)
{
    OD_LOG_OBJENTER(); //####
    SetExitRequest();
	if (_scanner)
	{
		_scanner->signalThreadShouldExit();
	}
    if (_registryServiceLauncher)
    {
        _registryServiceLauncher->killChildProcess();
    }
	if (_yarpLauncher)
	{
		_yarpLauncher->killChildProcess();
	}
	if (_scanner)
	{
        _scanner->stopThread(kThreadKillTime);
		_scanner = nullptr; // shuts down thread
	}
    if (_registryServiceLauncher)
    {
        _registryServiceLauncher->stopThread(kThreadKillTime);
        _registryServiceLauncher = nullptr; // shuts down thread
    }
	if (_yarpLauncher)
    {
        _yarpLauncher->stopThread(kThreadKillTime);
		_yarpLauncher = nullptr; // shuts down thread
        restoreYarpConfiguration();
	}
	EntitiesPanel & entities = _mainWindow->getEntitiesPanel();

    entities.rememberPositions();
#if defined(MpM_DoExplicitClose)
    _peeker->close();
#endif // defined(MpM_DoExplicitClose)
    Common::AdapterChannel::RelinquishChannel(_peeker);
    _mainWindow = nullptr; // (deletes our window)
    yarp::os::Network::fini();
    _yarp = nullptr;
    Utilities::ShutDownGlobalStatusReporter();
    OD_LOG_OBJEXIT(); //####
} // ChannelManagerApplication::shutdown

void ChannelManagerApplication::systemRequestedQuit(void)
{
    OD_LOG_OBJENTER(); //####
    // This is called when the app is being asked to quit: you can ignore this
    // request and let the app carry on running, or call quit() to allow the app to close.
    quit();
    OD_LOG_OBJEXIT(); //####
} // ChannelManagerApplication::systemRequestedQuit

void ChannelManagerApplication::restoreYarpConfiguration(void)
{
    OD_LOG_OBJENTER(); //####
    ChildProcess runYarp;
    String       appName(JUCEApplication::getInstance()->getApplicationName());
    StringArray  nameAndArgs(_yarpPath);
    
    nameAndArgs.add("conf");
    nameAndArgs.add(_configuredYarpAddress);
    nameAndArgs.add(String(_configuredYarpPort));
    if (runYarp.start(nameAndArgs))
    {
        const String childOutput(runYarp.readAllProcessOutput());
        
        runYarp.waitForProcessToFinish(kThreadKillTime);
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelManagerApplication::restoreYarpConfiguration

bool ChannelManagerApplication::validateRegistryService(void)
{
    OD_LOG_OBJENTER(); //####
    bool         doLaunch = false;
    ChildProcess runRegistryService;
    String       appName(JUCEApplication::getInstance()->getApplicationName());
    StringArray  nameAndArgs(_registryServicePath);
    
    nameAndArgs.add("--vers");
    if (runRegistryService.start(nameAndArgs))
    {
        const String childOutput(runRegistryService.readAllProcessOutput());
        
        runRegistryService.waitForProcessToFinish(kThreadKillTime);
        if (0 < childOutput.length())
        {
            // We have a useable Registry Service executable - ask what the user wants to do.
            _registryServiceCanBeLaunched = true;
            doLaunch = (1 == AlertWindow::showOkCancelBox(AlertWindow::QuestionIcon,
                                                          "No running Registry Service was "
                                                          "detected - do you wish to launch the "
                                                          "Registry Service?",
                                                          "If you do, it may take a few moments to "
                                                          "start, depending on network traffic and "
                                                          "system activity. "
                                                          "Also, the Registry Service will be "
                                                          "shut down when this application exits, "
                                                          "resulting in a potential loss of "
                                                          "connectivity to any M+M services that "
                                                          "were started after the Registry Service "
                                                          "was launched.", "Yes", "No",
                                                          _mainWindow, nullptr));
        }
        else
        {
            // The Registry Service executable can't actually be launched!
            AlertWindow::showMessageBox(AlertWindow::WarningIcon, appName,
                                        "The Registry Service executable found in the PATH system "
                                        "environment variable did not return valid data. "
                                        "Launching the Registry Service is not possible.",
                                        String::empty, _mainWindow);
        }
    }
    else
    {
        AlertWindow::showMessageBox(AlertWindow::WarningIcon, appName,
                                    "The Registry Service executable found in the PATH system "
                                    "environment variable could not be started. "
                                    "Launching the Registry Service is not possible.",
                                    String::empty, _mainWindow);
    }
    OD_LOG_OBJEXIT_B(doLaunch); //####
    return doLaunch;
} // ChannelManagerApplication::validateRegistryService

bool ChannelManagerApplication::validateYarp(void)
{
    OD_LOG_OBJENTER(); //####
    bool         doLaunch = false;
    ChildProcess runYarp;
    String       appName(JUCEApplication::getInstance()->getApplicationName());
    StringArray  nameAndArgs(_yarpPath);
    
    nameAndArgs.add("version");
    if (runYarp.start(nameAndArgs))
    {
        const String childOutput(runYarp.readAllProcessOutput());
        
        runYarp.waitForProcessToFinish(kThreadKillTime);
        if (0 < childOutput.length())
        {
            // We have a useable YARP executable - ask what the user wants to do.
            doLaunch = (1 == AlertWindow::showOkCancelBox(AlertWindow::QuestionIcon,
                                                          "No YARP network was detected - do you "
                                                          "wish to launch a private YARP network?",
                                                          "If you do, it may take a few moments to "
                                                          "start, depending on network traffic and "
                                                          "system activity. "
                                                          "Also, the private YARP network will be "
                                                          "shut down when this application exits, "
                                                          "resulting in a potential loss of "
                                                          "connectivity to any M+M services that "
                                                          "were started after the private YARP "
                                                          "network was launched.", "Yes", "No",
                                                          _mainWindow, nullptr));
        }
        else
        {
            // The YARP executable can't actually be launched!
            AlertWindow::showMessageBox(AlertWindow::WarningIcon, appName,
                                        "No YARP network was detected and the YARP executable "
                                        "found in the PATH system environment variable did not "
                                        "return valid data. "
                                        "Execution is not possible.", String::empty, _mainWindow);
        }
    }
    else
    {
        AlertWindow::showMessageBox(AlertWindow::WarningIcon, appName,
                                    "No YARP network was detected and the YARP executable found in "
                                    "the PATH system environment variable could not be started. "
                                    "Execution is not possible.", String::empty, _mainWindow);
    }
    OD_LOG_OBJEXIT_B(doLaunch); //####
    return doLaunch;
} // validateYarp

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)

#if (! MAC_OR_LINUX_)
# pragma warning(push)
# pragma warning(disable: 4100)
#endif // ! MAC_OR_LINUX_
bool CheckForExit(void * stuff)
{
#if (! defined(OD_ENABLE_LOGGING))
# if MAC_OR_LINUX_
#  pragma unused(stuff)
# endif // MAC_OR_LINUX_
#endif // ! defined(OD_ENABLE_LOGGING)
    OD_LOG_ENTER(); //####
    OD_LOG_P1("stuff = ", stuff); //####
    OD_LOG_EXIT_B(lExitRequested); //####
    return lExitRequested;
} // CheckForExit
#if (! MAC_OR_LINUX_)
# pragma warning(pop)
#endif // ! MAC_OR_LINUX_

void SetExitRequest(void)
{
    OD_LOG_ENTER(); //####
    lExitRequested = true;
    OD_LOG_EXIT(); //####
} // SetExitRequest
