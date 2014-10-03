//--------------------------------------------------------------------------------------------------
//
//  File:       ScannerThread.h
//
//  Project:    M+M
//
//  Contains:   The class declaration for the background port and service scanner.
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
//  Created:    2014-07-17
//
//--------------------------------------------------------------------------------------------------

#if (! defined(ScannerThread_H_))
# define ScannerThread_H_ /* Header guard */

# include "EntitiesData.h"

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file
 
 @brief The class declaration for the background port and service scanner. */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

namespace ChannelManager
{
    class ChannelManagerWindow;
    class EntitiesData;
    
    /*! @brief A background scanner thread. */
    class ScannerThread : public Thread
    {
    public :
        
        /*! @brief The constructor.
         @param name The name to give to the thread.
         @param window The window to be updated. */
        ScannerThread(const yarp::os::ConstString & name,
                      ChannelManagerWindow &        window);
        
        /*! @brief The destructor. */
        virtual ~ScannerThread(void);
        
        /*! @brief Returns @c true if the scan data is available and @c false otherwise.
         
         Note that what is returned is the value prior to the call; the flag is cleared so that the
         next call will return @c false.
         @returns @c true if the scan data is available and @c false otherwise. */
        bool checkAndClearIfScanIsComplete(void);
        
        /*! @brief Request access for reading from shared resources.
         @returns @c true if the read lock has been acquired and @c false otherwise. */
        bool conditionallyAcquireForRead(void);
        
        /*! @brief Request access for writing to shared resources.
         @returns @c true if the write lock has been acquired and @c false otherwise. */
        bool conditionallyAcquireForWrite(void);
        
        /*! @brief Indicate that a scan should be performed as soon as possible. */
        void doScanSoon(void);
        
        /*! @brief Return the collected entities data.
         @returns The collected entities data. */
        EntitiesData & getEntitiesData(void)
        {
            return _workingData;
        } // getEntitiesData
        
        /*! @brief Release access from reading from the shared resources. */
        void relinquishFromRead(void);
        
        /*! @brief Release access from writing to the shared resources. */
        void relinquishFromWrite(void);
        
        /*! @brief Perform the background scan. */
        virtual void run(void);
        
        /*! @brief Indicate that the scan data has been processed and the scan can proceed. */
        void scanCanProceed(void);
        
        /*! @brief Indicate that the thread must leave as soon as possible. */
        void stopNow(void);
        
        /*! @brief Request access for reading from shared resources. */
        void unconditionallyAcquireForRead(void);
        
        /*! @brief Request access for writing to shared resources. */
        void unconditionallyAcquireForWrite(void);
        
    protected :
        
    private :
        
        /*! @brief Add the detected entities and connections. */
        void addEntities(void);
        
        /*! @brief Add connections between detected ports in the to-be-displayed list.
         @param detectedPorts The set of detected YARP ports.
         @param checker A function that provides for early exit from loops.
         @param checkStuff The private data for the early exit function. */
        void addPortConnections(const MplusM::Utilities::PortVector & detectedPorts,
                                MplusM::Common::CheckFunction         checker,
                                void *                                checkStuff);
        
        /*! @brief Add ports that have associates as 'adapter' entities to the to-be-displayed list.
         @param detectedPorts The set of detected YARP ports.
         @param checker A function that provides for early exit from loops.
         @param checkStuff The private data for the early exit function. */
        void addPortsWithAssociates(const MplusM::Utilities::PortVector & detectedPorts,
                                    MplusM::Common::CheckFunction         checker,
                                    void *                                checkStuff);
        
        /*! @brief Add regular YARP ports as distinct entities to the to-be-displayed list.
         @param detectedPorts The set of detected YARP ports.
         @param checker A function that provides for early exit from loops.
         @param checkStuff The private data for the early exit function. */
        void addRegularPortEntities(const MplusM::Utilities::PortVector & detectedPorts,
                                    MplusM::Common::CheckFunction         checker,
                                    void *                                checkStuff);
        
        /*! @brief Add services as distinct entities to the list of entities.
         @param services The set of detected services.
         @param checker A function that provides for early exit from loops.
         @param checkStuff The private data for the early exit function. */
        void addServices(const MplusM::Common::StringVector & services,
                         MplusM::Common::CheckFunction        checker,
                         void *                               checkStuff);
        
        /*! @brief Determine whether a port can be used for input and/or output.
         @param oldEntry The previous record for the port, if it exists.
         @param portName The name of the port to check.
         @param checker A function that provides for early exit from loops.
         @param checkStuff The private data for the early exit function.
         @returns The allowed directions for the port. */
        PortDirection determineDirection(ChannelEntry *                oldEntry,
                                         const yarp::os::ConstString & portName,
                                         MplusM::Common::CheckFunction checker,
                                         void *                        checkStuff);
        
        /*! @brief Identify the YARP network entities.
         @param checker A function that provides for early exit from loops.
         @param checkStuff The private data for the early exit function.
         @returns @c true if the network entity information was gathered and @c false otherwise. */
        bool gatherEntities(MplusM::Common::CheckFunction checker,
                            void *                        checkStuff);
        
        /*! @brief Tell the displayed panel to do a repaint. */
        void triggerRepaint(void);
        
    public :
    
    protected :
    
    private :

        /*! @brief The class that this class is derived from. */
        typedef Thread inherited;
        
        /*! @brief The window to be updated. */
        ChannelManagerWindow & _window;
        
        /*! @brief A set of known ports. */
        PortSet _rememberedPorts;
        
        /*! @brief A set of known services. */
        ServiceMap _detectedServices;
        
        /*! @brief A set of associated ports. */
        AssociatesMap _associatedPorts;
        
        /*! @brief A set of standalone ports. */
        SingularPortMap _standalonePorts;
        
        /*! @brief The working set of entities. */
        EntitiesData _workingData;
        
        /*! @brief A lock to manage access to shared resources. */
        ReadWriteLock _lock;
        
        /*! @brief The name of the port used to determine if a port being checked can be used as an
         output. */
        yarp::os::ConstString _inputOnlyPortName;
        
        /*! @brief The name of the port used to determine if a port being checked can be used as an
         input. */
        yarp::os::ConstString _outputOnlyPortName;
        
# if (defined(CHECK_FOR_STALE_PORTS) && (! defined(DO_SINGLE_CHECK_FOR_STALE_PORTS)))
        /*! @brief The time when the last stale removal occurred. */
        int64 _lastStaleTime;
# endif // defined(CHECK_FOR_STALE_PORTS) && (! defined(DO_SINGLE_CHECK_FOR_STALE_PORTS))

        /*! @brief The port used to determine if a port being checked can be used as an output. */
        MplusM::Common::AdapterChannel * _inputOnlyPort;
        
        /*! @brief The port used to determine if a port being checked can be used as an input. */
        MplusM::Common::AdapterChannel * _outputOnlyPort;
        
# if (defined(CHECK_FOR_STALE_PORTS) && defined(DO_SINGLE_CHECK_FOR_STALE_PORTS))
        /*! @brief @c true if the initial stale removal occurred and @c false otherwise. */
        bool _initialStaleCheckDone;
# endif // defined(CHECK_FOR_STALE_PORTS) && defined(DO_SINGLE_CHECK_FOR_STALE_PORTS)
        
        /*! @brief @c true if the port direction resources are available. */
        bool _portsValid;
        
        /*! @brief @c true if the scan can proceed and @c false otherwise. */
        bool _scanCanProceed;
        
        /*! @brief @c true if the scan has been finished and the data is available, @c false
         otherwise. */
        bool _scanIsComplete;
        
        /*! @brief @c true if the scan should be done as soon as possible and @c false otherwise. */
        bool _scanSoon;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScannerThread)
        
    }; // ScannerThread
    
} // ChannelManager

#endif // ! defined(ScannerThread_H_)
