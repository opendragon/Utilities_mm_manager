//--------------------------------------------------------------------------------------------------
//
//  File:       ChannelsDataTypes.h
//
//  Project:    M+M
//
//  Contains:   The common data types for the Channel Manager application.
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
//  Created:    2014-07-18
//
//--------------------------------------------------------------------------------------------------

#if (! defined(ChannelsDataTypes_H_))
# define ChannelsDataTypes_H_ /* Header guard */

# if (! defined(DOXYGEN))
#  include "../JuceLibraryCode/JuceHeader.h"
# endif // ! defined(DOXYGEN)

# include <mpm/M+MAdapterChannel.h>
# include <mpm/M+MUtilities.h>

# include <list>
# include <map>
# include <set>
# include <vector>

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file
 
 @brief The common data types for the Channel Manager application. */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

/*! @brief A slightly longer sleep, in milliseconds. */
# define MIDDLE_SLEEP (VERY_SHORT_SLEEP * 7)

/*! @brief The minimum time for a thread to sleep, in milliseconds. */
# define SHORT_SLEEP (VERY_SHORT_SLEEP * 4)

/*! @brief A very short sleep, in milliseconds. */
# define VERY_SHORT_SLEEP 5

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
        
        /*! @brief Force the enumeration to be 4 bytes. */
        kAnchorUnknown = 0x80000000
        
    }; // AnchorSide
    
    /*! @brief What kind of container. */
    enum ContainerKind
    {
        /*! @brief The container is a client or adapter. */
        kContainerKindClientOrAdapter,
        
        /*! @brief The container is a service. */
        kContainerKindService,
        
        /*! @brief The container is neither a serice nor a client nor an adapter. */
        kContainerKindOther,
        
        /*! @brief Force the enumeration to be 4 bytes. */
        kContainerKindUnknown = 0x80000000
        
    }; // ContainerKind
    
    /*! @brief The primary direction of the port. */
    enum PortDirection
    {
        /*! @brief The port direction is input. */
        kPortDirectionInput,
        
        /*! @brief The port direction is both input and output. */
        kPortDirectionInputOutput,
        
        /*! @brief The port direction is output. */
        kPortDirectionOutput,
        
        /*! @brief Force the enumeration to be 4 bytes. */
        kPortDirectionUnknown = 0x80000000
        
    }; // PortDirection
    
    /*! @brief What the port will be used for. */
    enum PortUsage
    {
        /*! @brief The port is for a client or adapter. */
        kPortUsageClient,
        
        /*! @brief The port is for a service. */
        kPortUsageService,
        
        /*! @brief The port is for input / output. */
        kPortUsageInputOutput,
        
        /*! @brief The port has no specific characterization. */
        kPortUsageOther,
        
        /*! @brief Force the enumeration to be 4 bytes. */
        kPortUsageUnknown = 0x80000000
        
    }; // PortUsage
    
    /*! @brief The form of a connection. */
    struct Channel
    {
        union
        {
            /*! @brief The 'other-end' of a connection, as a ChannelEntry. */
            ChannelEntry * _otherChannel;
            
            /*! @brief The 'other-end' of a connection, as PortData. */
            PortData * _otherPort;

        };
        
        /*! @brief The kind of connection. */
        MplusM::Common::ChannelMode _connectionMode;
        
        /*! @brief @c true if the connection is valid and @c false otherwise. */
        bool _valid;
        
    }; // Channel
    
    /*! @brief The information for a connection. */
    struct ConnectionDetails
    {
        /*! @brief The name of the destination port. */
        String _inPortName;
        
        /*! @brief The name of the source port. */
        String _outPortName;
        
        /*! @brief The mode of the connection. */
        MplusM::Common::ChannelMode _mode;
        
    }; // ConnectionDetails
    
    /*! @brief The name and direction for a port. */
    struct NameAndDirection
    {
        /*! @brief The name of the port. */
        String _name;
        
        /*! @brief The direction of the port. */
        PortDirection _direction;
        
    }; // NameAndDirection
    
    /*! @brief The name and associates for a port. */
    struct PortAndAssociates
    {
        /*! @brief The name of the port. */
        String _name;
        
        /*! @brief The associates of the port. */
        MplusM::Utilities::PortAssociation _associates;
        
    }; // PortAndAssociates
    
    /*! @brief A mapping from port names to associates. */
    typedef std::map<String, PortAndAssociates> AssociatesMap;
    
    /*! @brief The set of connections to the port. */
    typedef std::vector<Channel> Channels;
    
    /*! @brief A collection of connections. */
    typedef std::vector<ConnectionDetails> ConnectionList;
    
    /*! @brief A collection of services and ports. */
    typedef std::vector<ChannelContainer *> ContainerList;
    
    /*! @brief A collection of entities (services, standalone ports, et cetera). */
    typedef std::vector<EntityData *> EntitiesList;
    
    /*! @brief A mapping from strings to channels. */
    typedef std::map<String, ChannelEntry *> ChannelEntryMap;
    
    /*! @brief A mapping from strings to ports. */
    typedef std::map<String, PortData *> PortDataMap;
    
    /*! @brief A collection of ports. */
    typedef std::vector<PortData *> Ports;
    
    /*! @brief A collection of port names. */
    typedef std::set<String> PortSet;
    
    /*! @brief A mapping from strings to service descriptions. */
    typedef std::map<String, MplusM::Utilities::ServiceDescriptor> ServiceMap;
    
    /*! @brief A collection of singular port names. */
    typedef std::map<String, NameAndDirection> SingularPortMap;
    
} // ChannelManager

#endif // ! defined(ChannelsDataTypes_H_)
