//--------------------------------------------------------------------------------------------------
//
//  File:       ChannelManagerDataTypes.h
//
//  Project:    M+M
//
//  Contains:   The common data types for the Channel Manager application.
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
//  Created:    2014-07-18
//
//--------------------------------------------------------------------------------------------------

#if (! defined(ChannelManagerDataTypes_H_))
# define ChannelManagerDataTypes_H_ /* Header guard */

# if (! defined(DOXYGEN))
#  include "../JuceLibraryCode/JuceHeader.h"
# endif // ! defined(DOXYGEN)

# include <mpm/M+MGeneralChannel.h>
# include <mpm/M+MUtilities.h>

# include <list>
# include <map>
# include <set>
# include <vector>

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wunknown-pragmas"
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file
 
 @brief The common data types for the Channel Manager application. */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

# define CHECK_FOR_STALE_PORTS_ /* Check for 'stale' ports in the scanner. */

# define DO_SINGLE_CHECK_FOR_STALE_PORTS_ /* Perform an initial check for stale ports. */

/*! @brief A longer sleep, in milliseconds. */
# define LONG_SLEEP_ (VERY_SHORT_SLEEP_ * 100)

/*! @brief A slightly longer sleep, in milliseconds. */
# define MIDDLE_SLEEP_ (VERY_SHORT_SLEEP_ * 7)

/*! @brief The minimum time for a thread to sleep, in milliseconds. */
# define SHORT_SLEEP_ (VERY_SHORT_SLEEP_ * 4)

# define USE_OGDF_FOR_FIRST_POSITIONING_ONLY_ /* Use OGDF for the initial entity placement. */

# define USE_OGDF_POSITIONING_ /* Use OGDF for new entity placement.*/

/*! @brief A very short sleep, in milliseconds. */
# define VERY_SHORT_SLEEP_ 5

namespace ChannelManager
{
    class ChannelContainer;
    class ChannelEntry;
    class EntityData;
    class PortData;
    
    /*! @brief The anchor position for a connection between ports. */
    enum AnchorSide
    {
        /*! @brief The connection is to the left edge of the port entry. */
        kAnchorLeft,
        
        /*! @brief The connection is to the right edge of the port entry. */
        kAnchorRight,
        
        /*! @brief The connection is to the bottom edge of the port entry. */
        kAnchorBottomCentre,
        
        /*! @brief The connection is to the top edge of the port entry. */
        kAnchorTopCentre,
        
        /*! @brief The anchor position is unknown. */
        kAnchorUnknown = 0x7FFFFFF
        
    }; // AnchorSide
    
    /*! @brief The kind of application. */
    enum ApplicationKind
    {
        /*! @brief The application is an Adapter. */
        kApplicationAdapter,
        
        /*! @brief The application is a Service. */
        kApplicationService,
        
        /*! @brief The application is not recognized. */
        kApplicationUnknown
        
    }; // ApplicationKind
    
    /*! @brief The menu selection from the popup menu. */
    enum ChannelPopupMenuSelection
    {
        /*! @brief Add a scrolling monitor to the port. */
        kPopupAddScrollingMonitor = 0x2200,
        
        /*! @brief Add a simple monitor to the port. */
        kPopupAddSimpleMonitor,
        
        /*! @brief Display detailed information request. */
        kPopupDetailedDisplayPortInfo,
        
        /*! @brief Display information request. */
        kPopupDisplayPortInfo,
        
        /*! @brief Display the metrics for the channel. */
        kPopupDisplayChannelMetrics
        
    }; // ChannelPopupMenuSelection
    
    /*! @brief What kind of container. */
    enum ContainerKind
    {
        /*! @brief The container is an adapter - a special form of service. */
        kContainerKindAdapter,
        
        /*! @brief The container is a service. */
        kContainerKindService,
        
        /*! @brief The container is not a service. */
        kContainerKindOther,
        
        /*! @brief Force the size to be 4 bytes. */
        kContainerKindUnknown = 0x7FFFFFF
        
    }; // ContainerKind
    
    /*! @brief The menu selection from the popup menu. */
    enum EntityPopupMenuSelection
    {
        /*! @brief Change the state of service metrics collection. */
        kPopupDisplayChangeServiceMetrics = 0x2100,
        
        /*! @brief Display detailed information request. */
        kPopupDetailedDisplayEntityInfo,
        
        /*! @brief Display information request. */
        kPopupDisplayEntityInfo,
        
        /*! @brief Display the channel metrics for a service. */
        kPopupDisplayServiceMetrics,
        
        /*! @brief Hide the entity. */
        kPopupHideEntity,
        
        /*! @brief Stop the service. */
        kPopupStopService
        
    }; // EntityPopupMenuSelection
    
    /*! @brief The primary direction of the port. */
    enum PortDirection
    {
        /*! @brief The port direction is input. */
        kPortDirectionInput,
        
        /*! @brief The port direction is both input and output. */
        kPortDirectionInputOutput,
        
        /*! @brief The port direction is output. */
        kPortDirectionOutput,
        
        /*! @brief The port direction is unknown. */
        kPortDirectionUnknown = 0x7FFFFFF
        
    }; // PortDirection
    
    /*! @brief What the port will be used for. */
    enum PortUsage
    {
        /*! @brief The port is for a client or adapter. */
        kPortUsageClient,
        
        /*! @brief The port is for input / output. */
        kPortUsageInputOutput,
        
        /*! @brief The port has no specific characterization. */
        kPortUsageOther,
        
        /*! @brief The port is for a service. */
        kPortUsageService,
        
        /*! @brief Force the size to be 4 bytes. */
        kPortUsageUnknown = 0x7FFFFFF
        
    }; // PortUsage
    
    /*! @brief The information used to launch an application. */
    struct ApplicationInfo
    {
        /*! @brief The argument descriptions for the application. */
        MplusM::Utilities::DescriptorVector _argDescriptions;

        /*! @brief The file system path to the application. */
        String _applicationPath;
 
        /*! @brief The matching criteria (if an Adapter). */
        String _criteria;
        
        /*! @brief The supported options (if a Service). */
        String _options;
        
        /*! @brief The description provided by the application. */
        String _description;
        
        /*! @brief The 'short name' of the application. */
        String _shortName;
        
        /*! @brief What kind of application this is. */
        ApplicationKind _kind;
        
    }; // ApplicationInfo
    
    /*! @brief The form of a channel connection. */
    struct ChannelInfo
    {
        /*! @brief The 'other-end' of a channel connection, as a ChannelEntry. */
        ChannelEntry * _otherChannel;
            
        /*! @brief The kind of channel connection. */
        MplusM::Common::ChannelMode _connectionMode;
        
        /*! @brief @c true if the channel connection protocols were overridden and @c false
         otherwise. */
        bool _forced;
        
        /*! @brief @c true if the channel connection is valid and @c false otherwise. */
        bool _valid;
        
    }; // ChannelInfo
    
    /*! @brief The information for a connection. */
    struct ConnectionDetails
    {
        /*! @brief The name of the destination port. */
        YarpString _inPortName;
        
        /*! @brief The name of the source port. */
        YarpString _outPortName;
        
        /*! @brief The mode of the connection. */
        MplusM::Common::ChannelMode _mode;
        
    }; // ConnectionDetails
    
    /*! @brief The name and direction for a port. */
    struct NameAndDirection
    {
        /*! @brief The name of the port. */
        YarpString _name;
        
        /*! @brief The direction of the port. */
        PortDirection _direction;
        
    }; // NameAndDirection
    
    /*! @brief The form of a port connection. */
    struct PortInfo
    {
        /*! @brief The 'other-end' of a connection, as PortData. */
        PortData * _otherPort;
        
        /*! @brief The kind of connection. */
        MplusM::Common::ChannelMode _connectionMode;
        
        /*! @brief @c true if the port connection is valid and @c false otherwise. */
        bool _valid;
        
    }; // PortInfo
    
    /*! @brief Coordinates on the display. */
    typedef Point<float> Position;
    
    /*! @brief The set of connections to the channel. */
    typedef std::vector<ChannelInfo> ChannelConnections;
    
    /*! @brief A collection of application details. */
    typedef std::vector<ApplicationInfo> ApplicationList;
    
    /*! @brief A collection of connections. */
    typedef std::vector<ConnectionDetails> ConnectionList;
    
    /*! @brief A collection of services and ports. */
    typedef std::vector<ChannelContainer *> ContainerList;
    
    /*! @brief A collection of entities (services, standalone ports, et cetera). */
    typedef std::vector<EntityData *> EntitiesList;
    
    /*! @brief A mapping from strings to channels. */
    typedef std::map<YarpString, ChannelEntry *> ChannelEntryMap;
    
    /*! @brief A mapping from strings to ports. */
    typedef std::map<YarpString, PortData *> PortDataMap;
    
    /*! @brief A collection of ports. */
    typedef std::vector<PortData *> Ports;
    
    /*! @brief The set of connections to the port. */
    typedef std::vector<PortInfo> PortConnections;
    
    /*! @brief A collection of port names. */
    typedef std::set<YarpString> PortSet;
    
    /*! @brief A mapping from entity names to positions. */
    typedef std::map<YarpString, Position> PositionMap;
    
    /*! @brief A mapping from strings to service descriptions. */
    typedef std::map<YarpString, MplusM::Utilities::ServiceDescriptor> ServiceMap;
    
    /*! @brief A collection of singular port names. */
    typedef std::map<YarpString, NameAndDirection> SingularPortMap;
    
} // ChannelManager

/*! @brief Return @c true if exit is requested.
 @param stuff Dummy argument to satisfy caller.
 @returns @c true if exit has been requested. */
bool CheckForExit(void * stuff);

/*! @brief Indicate that an exit has been requested. */
void SetExitRequest(void);

#endif // ! defined(ChannelManagerDataTypes_H_)
