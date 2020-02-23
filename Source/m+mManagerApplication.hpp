//--------------------------------------------------------------------------------------------------
//
//  File:       m+mManagerApplication.hpp
//
//  Project:    m+m
//
//  Contains:   The class declaration for the application object of the m+m manager application.
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

#if (! defined(mpmManagerApplication_HPP_))
# define mpmManagerApplication_HPP_ /* Header guard */

# include "m+mManagerWindow.hpp"

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
# if (! MAC_OR_LINUX_)
#  pragma warning(push)
#  pragma warning(disable: 4996)
#  pragma warning(disable: 4458)
# endif // ! MAC_OR_LINUX_
# include <yarp/os/Network.h>
# if (! MAC_OR_LINUX_)
#  pragma warning(pop)
# endif // ! MAC_OR_LINUX_
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wunknown-pragmas"
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file

 @brief The class declaration for the application object of the m+m manager application. */

/*! @dir Source
 @brief The set of files that support managing channels. */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

namespace MPlusM_Manager
{
    class PeekInputHandler;
    class RegistryLaunchThread;
    class ScannerThread;
    class ServiceLaunchThread;
    class YarpLaunchThread;

    /*! @brief The application object of the application. */
    class ManagerApplication : public JUCEApplication
    {
    public :

    protected :

    private :

        /*! @brief The class that this class is derived from. */
        typedef JUCEApplication inherited;

    public :

        /*! @brief The constructor. */
        ManagerApplication(void);

        /*! @brief The destructor. */
        virtual
        ~ManagerApplication(void);

        /*! @brief Returns @c true if there is a Registry Service executable available but no
         running Registry Service was detected.
         @return @c true if the Registry Service can be launched and @c false otherwise. */
        inline bool
        canTheRegistryServiceBeLaunched(void)
        const
        {
            return _registryServiceCanBeLaunched;
        } // canTheRegistryServiceBeLaunched

        /*! @brief Connect (or reconnect) the peek channel to the Registry Service. */
        void
        connectPeekChannel(void);

        /*! @brief Indicate that a port cleanup should be performed as soon as possible. */
        void
        doCleanupSoon(void);

        /*! @brief Ask the user to select an application, provide information required to launch the
         application and launch it if the user requests. */
        void
        doLaunchOtherApplication(void);

        /*! @brief Ask the user for information required to launch the Registry Service and launch
         it if the user requests.
         @return @c true if the Registry Service was launched and @c false otherwise. */
        bool
        doLaunchRegistry(void);

        /*! @brief Indicate that a scan should be performed as soon as possible. */
        void
        doScanSoon(void);

        /*! @brief Determine the path to an executable, using the system PATH environment variable.
         @param[in] execName The short name of the executable.
         @return The full path to the first executable found in the system PATH environment
         variable. */
        static String
        findPathToExecutable(const String & execName);

        /*! @brief Return the application object.
         @return The application object. */
        static
        ManagerApplication * getApp(void);

        /*! @brief Return the button height to use.
         @return The button height to use. */
        static int
        getButtonHeight(void);

        /*! @brief Return the number of launchable applications.
         @return The number of launchable applications. */
        inline size_t
        getCountOfApplications(void)
        const
        {
            return _applicationList.size();
        } // getCountOfApplications

        /*! @brief Return the value of a system environment variable.
         @param[in] varName The name of the system environment variable.
         @return The value of the system environment variable, or an empty value. */
        static String
        getEnvironmentVar(const char * varName);

        /*! @brief Collect the set of system environment variables.

         Note that the generated code for a std::map of String, String includes illegal instructions
         when end() is referenced, so we are using a pair of simple vectors - performance is not
         critical, since the environment variables are only used once.
         @param[out] keys The list of environment variable names.
         @param[out] values The list of environment variable values. */
        static void
        getEnvironmentVars(YarpStringVector & keys,
                           YarpStringVector & values);

        /*! @brief Return the home directory of the current user.
         @return The home directory of the current user. */
        String
        getHomeDir(void);

        /*! @brief Return the main window of the application.
         @return The main window of the application. */
        static ManagerWindow *
        getMainWindow(void);

        /*! @brief Get the primary channel for a service.
         @param[in] appInfo The description of the executable.
         @param[in] endpointName The endpoint to use, if not the default.
         @param[in] tag The tag to use, if any.
         @param[in] portNumber The network port number to use.
         @param[in] arguments The current set of arguments to be applied.
         @param[in] tagModifierCount The number of bytes of the IP address to use as a tag modifier.
         @param[out] channelName The channel that the service will be using.
         @return @c true if the channel was retrieved and @c false otherwise. */
        bool
        getPrimaryChannelForService(const ApplicationInfo & appInfo,
                                    const String &          endpointName,
                                    const String &          tag,
                                    const String &          portNumber,
                                    const StringArray &     arguments,
                                    const int               tagModifierCount,
                                    String &                channelName);

        /*! @brief Return the real name of the current user.
         @return The real name of the current user. */
        String
        getRealName(void);

        /*! @brief Return the name of the current user.
         @return The set of name of the current user. */
        String
        getUserName(void);

    protected :

    private :

        /*! @brief The copy constructor.
         @param[in] other The object to be copied. */
        ManagerApplication(const ManagerApplication & other);

        /*! @brief Called when an attempt was made to launch another instance of the application.
         @param[in] commandLine The arguments passed to the new instance. */
        virtual void
        anotherInstanceStarted(const String & commandLine);

        /*! @brief If the Registry Service is not currently running, give the user the option to
         launch it.
         @return @c true if the Registry Service was launched and @c false otherwise. */
        bool
        checkForRegistryServiceAndLaunchIfDesired(void);

        /*! @brief If YARP is not currently running, give the user the option to launch a private
         copy.
         @return A pointer to the %Network object used for YARP access. */
        yarp::os::Network *
        checkForYarpAndLaunchIfDesired(void);

        /*! @brief Ask the user for information required to launch a service and launch it if the
         user requests.
         @param[in] appInfo The description of the executable. */
        void
        doLaunchAService(const ApplicationInfo & appInfo);

        /*! @brief Return the application name.
         @return The application's name. */
        virtual const String
        getApplicationName(void);

        /*! @brief Return the application version number.
         @return The application's version number. */
        virtual const String
        getApplicationVersion(void);

        /*! @brief Get the operational arguments for an application.
         @param[out] theInfo The retrieved parameters.
         @return @c true if the operational arguments were retrieved and @c false otherwise. */
        bool
        getArgumentsForApplication(ApplicationInfo & theInfo);

        /*! @brief Get the operational parameters for an application.
         @param[in] execName The name of the executable to be analyzed.
         @param[out] theInfo The retrieved parameters.
         @return @c true if the operational parameters were retrieved and @c false otherwise. */
        bool
        getParametersForApplication(const String &    execName,
                                    ApplicationInfo & theInfo);

        /*! @brief Called when the application starts.
         @param[in] commandLine The parameters passed to the application. */
        virtual void
        initialise(const String & commandLine);

        /*! @brief Load the text files containing the standard and user-defined applications, and
         set up for later use. */
        void
        loadApplicationLists(void);

        /*! @brief Return @c true if multiple instances of the application are allowed and @c false
         otherwise.
         @return @c true if multiple instanaces of the application are allowed and @c false
         otherwise. */
        virtual bool
        moreThanOneInstanceAllowed(void);

        /*! @brief The assignment operator.
         @param[in] other The object to be copied.
         @return The updated object. */
        ManagerApplication &
        operator =(const ManagerApplication & other);

        /*! @brief Put back the YARP configuration settings that were in effect prior to launching a
         private YARP network. */
        void
        restoreYarpConfiguration(void);

        /*! @brief Called to allow the application to clear up before exiting. */
        virtual void
        shutdown(void);

        /*! @brief Called when the operating system is trying to close the application. */
        virtual void
        systemRequestedQuit(void);

        /*! @brief Check if the Registry Service can be launched and if the user wishes it to be.
         @return @c true if the user requests that the Registry Service be started and @ c false
         if the YARP executable is invalid or the user does not want to launch the Registry
         Service. */
        bool
        validateRegistryService(void);

        /*! @brief Check if YARP can be launched and if the user wishes it to be.
         @return @c true if the user requests that a private YARP network be set up and @ c false
         if the YARP executable is invalid or the user does not want a private YARP network. */
        bool
        validateYarp(void);

    public :

    protected :

    private :

        /*! @brief The primary window of the application. */
        ScopedPointer<ManagerWindow> _mainWindow;

        /*! @brief Used to establish connections to the YARP infrastructure. */
        ScopedPointer<yarp::os::Network> _yarp;

        /*! @brief The background scanner thread. */
        ScopedPointer<ScannerThread> _scanner;

        /*! @brief The background Registry Service launch thread. */
        ScopedPointer<RegistryLaunchThread> _registryLauncher;

        /*! @brief The background private YARP launch thread. */
        ScopedPointer<YarpLaunchThread> _yarpLauncher;

        /*! @brief The set of background general service launch threads. */
        OwnedArray<ServiceLaunchThread> _serviceLaunchers;

        /*! @brief The list of launchable applications. */
        ApplicationList _applicationList;

        /*! @brief The popup menu used for the list of applications. */
        PopupMenu _applicationMenu;

        /*! @brief The configured YARP address prior to launching a private YARP network. */
        String _configuredYarpAddress;

        /*! @brief The file system path to the Registry Service executable. */
        String _registryServicePath;

        /*! @brief The file system path to the YARP executable. */
        String _yarpPath;

        /*! @brief A channel to watch the Registry Service status. */
        MplusM::Common::GeneralChannel * _peeker;

        /*! @brief The input handler for the 'peek' channel. */
        PeekInputHandler * _peekHandler;

        /*! @brief The height of a standard button. */
        int _buttonHeight;

        /*! @brief The configured YARP port prior to launching a private YARP network. */
        int _configuredYarpPort;

        /*! @brief @c true if it is possible to launch the Registry Service and @c false if there is
         a running Registry Service or a launchable one cannot be found. */
        bool _registryServiceCanBeLaunched;

    }; // ManagerApplication

    /*! @brief Determine the maximum dimensions of a text string.
     @param[out] dimensions The calculated maximum width and height.
     @param[in] aFont The font to use for the calculations.
     @param[in] aString The string to be analyzed. */
    void
    CalculateTextArea(Point<int> &   dimensions,
                      const Font &   aFont,
                      const String & aString);

} // MPlusM_Manager

#endif // ! defined(mpmManagerApplication_HPP_)
