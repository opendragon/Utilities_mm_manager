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

#if (! defined(ChannelManagerApplication_H_))
# define ChannelManagerApplication_H_ /* Header guard */

# include "ChannelManagerWindow.h"

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wunknown-pragmas"
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
#  pragma clang diagnostic ignored "-Wunknown-pragmas"
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
    class GeneralServiceLaunchThread;
    class PeekInputHandler;
    class RegistryServiceLaunchThread;
    class ScannerThread;
    class YarpLaunchThread;
    
    /*! @brief The application object of the application. */
    class ChannelManagerApplication : public JUCEApplication
    {
    public :
        
        /*! @brief A mapping from Strings to Strings. */
        typedef std::map<String, String> JuceStringMap;
        
        /*! @brief The constructor. */
        ChannelManagerApplication(void);
        
        /*! @brief The destructor. */
        virtual ~ChannelManagerApplication(void);
        
        /*! @brief Called when an attempt was made to launch another instance of the application.
         @param commandLine The arguments passed to the new instance. */
        void anotherInstanceStarted(const String & commandLine);
        
        /*! @brief Returns @c true if there is a Registry Service executable available but no
         running Registry Service was detected.
         @returns @c true if the Registry Service can be launched and @c false otherwise. */
        inline bool canTheRegistryServiceBeLaunched(void)
        const
        {
            return _registryServiceCanBeLaunched;
        } // canTheRegistryServiceBeLaunched
        
        /*! @brief Connect (or reconnect) the peek channel to the Registry Service. */
        void connectPeekChannel(void);
        
        /*! @brief Indicate that a port cleanup should be performed as soon as possible. */
        void doCleanupSoon(void);
        
        /*! @brief Ask the user to select an application, provide information required to launch the
         application and launch it if the user requests.
         @returns @c true if the application was launched and @c false otherwise. */
        bool doLaunchOtherApplication(void);
        
        /*! @brief Ask the user for information required to launch the Registry Service and launch
         it if the user requests.
         @returns @c true if the Registry Service was launched and @c false otherwise. */
        bool doLaunchRegistry(void);

        /*! @brief Indicate that a scan should be performed as soon as possible. */
        void doScanSoon(void);
        
        /*! @brief Determine the path to an executable, using the system PATH environment variable.
         @param execName The short name of the executable.
         @returns The full path to the first executable found in the system PATH environment
         variable. */
        static String findPathToExecutable(const String & execName);
        
        /*! @brief Return the application object.
         @returns The application object. */
        static ChannelManagerApplication * getApp(void);
        
        /*! @brief Return the application name.
         @returns The application's name. */
        virtual const String getApplicationName(void);
        
        /*! @brief Return the application version number.
         @returns The application's version number. */
        virtual const String getApplicationVersion(void);
        
        /*! @brief Return the number of launchable applications.
         @returns The number of launchable applications. */
        inline size_t getCountOfApplications(void)
        const
        {
            return _applicationList.size();
        } // getCountOfApplications
        
        /*! @brief Return the value of a system environment variable.
         @param varName The name of the system environment variable.
         @returns The value of the system environment variable, or an empty value. */
        static String getEnvironmentVar(const char * varName);
        
        /*! @brief Return the set of system environment variables.
         @returns The set of system environment variables. */
        static JuceStringMap getEnvironmentVars(void);
        
        /*! @brief Return the home directory of the current user.
         @returns The home directory of the current user. */
        String getHomeDir(void);
        
        /*! @brief Return the real name of the current user.
         @returns The real name of the current user. */
        String getRealName(void);
        
        /*! @brief Return the name of the current user.
         @returns The set of name of the current user. */
        String getUserName(void);
        
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
        
    protected :
        
    private :
        
        /*! @brief If the Registry Service is not currently running, give the user the option to
         launch it.
         @returns @c true if the Registry Service was launched and @c false otherwise. */
        bool checkForRegistryServiceAndLaunchIfDesired(void);
        
        /*! @brief If YARP is not currently running, give the user the option to launch a private
         copy.
         @returns A pointer to the %Network object used for YARP access. */
        yarp::os::Network * checkForYarpAndLaunchIfDesired(void);
        
        /*! @brief Ask the user for information required to launch an adapter and launch it if the
         user requests.
         @param appInfo The description of the executable.
         @returns @c true if the adapter was launched and @c false otherwise. */
        bool doLaunchAnAdapter(const ApplicationInfo & appInfo);
        
        /*! @brief Ask the user for information required to launch a service and launch it if the
         user requests.
         @param appInfo The description of the executable.
         @returns @c true if the service was launched and @c false otherwise. */
        bool doLaunchAService(const ApplicationInfo & appInfo);
        
        /*! @brief Get the channel arguments for an adapter.
         @param execName The name of the executable to be analyzed.
         @param arguments The current set of arguments to be applied. */
        void getChannelsForAdapter(const String & execName,
                                   const String & arguments);
        
        /*! @brief Get the operational parameters for an application and add them to the list of
         applications.
         @param execName The name of the executable to be analyzed.
         @param menuIndex The position in the application menu where the item is to go. */
        void getParametersForApplication(const String & execName,
                                         int &          menuIndex);

        /*! @brief Load the text files containing the standard and user-defined applications, and
         set up for later use. */
        void loadApplicationLists(void);
        
        /*! @brief Put back the YARP configuration settings that were in effect prior to launching a
         private YARP network. */
        void restoreYarpConfiguration(void);
        
        /*! @brief Check if the Registry Service can be launched and if the user wishes it to be.
         @param execPath The file system path to the Registry Service executable.
         @returns @c true if the user requests that the Registry Service be started and @ c false
         if the YARP executable is invalid or the user does not want to launch the Registry
         Service. */
        bool validateRegistryService(void);
        
        /*! @brief Check if YARP can be launched and if the user wishes it to be.
         @param execPath The file system path to the YARP executable.
         @returns @c true if the user requests that a private YARP network be set up and @ c false
         if the YARP executable is invalid or the user does not want a private YARP network. */
        bool validateYarp(void);
        
        COPY_AND_ASSIGNMENT_(ChannelManagerApplication);

    public :
    
    protected :
    
    private :
        
        /*! @brief The class that this class is derived from. */
        typedef JUCEApplication inherited;
        
        /*! @brief The primary window of the application. */
        ScopedPointer<ChannelManagerWindow> _mainWindow;
        
        /*! @brief Used to establish connections to the YARP infrastructure. */
        ScopedPointer<yarp::os::Network> _yarp;
        
        /*! @brief The background scanner thread. */
        ScopedPointer<ScannerThread> _scanner;
        
        /*! @brief The background Registry Service launch thread. */
        ScopedPointer<RegistryServiceLaunchThread> _registryServiceLauncher;

        /*! @brief The background private YARP launch thread. */
        ScopedPointer<YarpLaunchThread> _yarpLauncher;
        
        /*! @brief The set of background general service launch threads. */
        OwnedArray<GeneralServiceLaunchThread> _serviceLaunchers;
        
        /*! @brief The list of launchable applications. */
        ApplicationList _applicationList;
        
        PopupMenu _applicationMenu;
        
        /*! @brief The configured YARP address prior to launching a private YARP network. */
        String _configuredYarpAddress;
        
        /*! @brief The file system path to the Registry Service executable. */
        String _registryServicePath;
        
		/*! @brief The file system path to the YARP executable. */
		String _yarpPath;

        /*! @brief A channel to watch the Registry Service status. */
        MplusM::Common::AdapterChannel * _peeker;
        
        /*! @brief The input handler for the 'peek' channel. */
        PeekInputHandler * _peekHandler;
        
        /*! @brief The configured YARP port prior to launching a private YARP network. */
        int _configuredYarpPort;
        
        /*! @brief @c true if it is possible to launch the Registry Service and @c false if there is
         a running Registry Service or a launchable one cannot be found. */
        bool _registryServiceCanBeLaunched;
        
    }; // ChannelManagerApplication
    
} // ChannelManager

#endif // ! defined(ChannelManagerApplication_H_)
