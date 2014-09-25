//--------------------------------------------------------------------------------------------------
//
//  File:       PortData.h
//
//  Project:    M+M
//
//  Contains:   The class declaration for a port detected by the background scanner.
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
//  Created:    2014-08-06
//
//--------------------------------------------------------------------------------------------------

#if (! defined(PortData_H_))
# define PortData_H_ /* Header guard */

# include "ChannelManagerDataTypes.h"

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file
 
 @brief The class declaration for a port detected by the background scanner. */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

namespace ChannelManager
{
    
    /*! @brief A port detected by the background scanner. */
    class PortData
    {
    public :
        
        /*! @brief The constructor.
         @param portName The port name for the entry.
         @param portProtocol The protocol of the port.
         @param portKind What the port will be used for.
         @param direction The primary direction of the port. */
        PortData(const yarp::os::ConstString & portName,
                 const yarp::os::ConstString & portProtocol,
                 const PortUsage               portKind,
                 const PortDirection           direction = PortDirection::kPortDirectionInput);
        
        /*! @brief The destructor. */
        virtual ~PortData(void);
        
        /*! @brief Add an input connection to the port.
         @param other The port that is to be connected.
         @param mode The mode of the connection. */
        void addInputConnection(PortData *                  other,
                                MplusM::Common::ChannelMode mode);
        
        /*! @brief Add an output connection to the port.
         @param other The port that is to be connected.
         @param mode The mode of the connection. */
        void addOutputConnection(PortData *                  other,
                                 MplusM::Common::ChannelMode mode);
        
        /*! @brief Return the direction of the port entry.
         @returns The direction of the port entry. */
        PortDirection getDirection(void)
        const
        {
            return _direction;
        } // getDirection
        
        /*! @brief Return the set of input connections to the port.
         @returns The set of input connections to the port. */
        inline const PortConnections & getInputConnections(void)
        const
        {
            return _inputConnections;
        } // getInputConnections
        
        /*! @brief Return the set of output connections to the port.
         @returns The set of output connections to the port. */
        inline const PortConnections & getOutputConnections(void)
        const
        {
            return _outputConnections;
        } // getOutputConnections
        
        /*! @brief Return the name of the associated port.
         @returns The name of the associated port. */
        inline yarp::os::ConstString getPortName(void)
        const
        {
            return _portName;
        } // getPortName
        
        /*! @brief Return the protocol of the associated port.
         @returns The protocol of the associated port. */
        inline yarp::os::ConstString getProtocol(void)
        const
        {
            return _portProtocol;
        } // getProtocol
        
        /*! @brief Return the usage of the port entry.
         @returns The usage of the port entry. */
        PortUsage getUsage(void)
        const
        {
            return _usage;
        } // getUsage
        
        /*! @brief Returns @c true if there is an outgoing connection to the named port.
         @param otherPort The name of the destination port.
         @returns @c true if there is an outgoing connection to the named port. */
        bool hasOutgoingConnectionTo(const yarp::os::ConstString & otherPort)
        const;
        
        /*! @brief Mark all the connections as invalid. */
        void invalidateConnections(void);
        
        /*! @brief Returns @c true if the port entry is a secondary port of a service and @c false
         otherwise.
         @returns @c true if the port is a secondary port of a service and @c false otherwise. */
        inline bool isInputOutput(void)
        const
        {
            return (PortUsage::kPortUsageInputOutput == _usage);
        } // isInputOutput
        
        /*! @brief Returns @c true if the port entry is part of a service and @c false
         otherwise.
         @returns @c true if the port is part of a service and @c false otherwise. */
        inline bool isService(void)
        const
        {
            return (PortUsage::kPortUsageService == _usage);
        } // isService

        /*! @brief Remove an input connection from a port.
         @param other The port that is to be disconnected. */
        void removeInputConnection(PortData * other);
        
        /*! @brief Remove connections that are invalid. */
        void removeInvalidConnections(void);
        
        /*! @brief Remove an output connection from a port.
         @param other The port that is to be disconnected. */
        void removeOutputConnection(PortData * other);
        
    protected :
        
    private :
        
        COPY_AND_ASSIGNMENT_(PortData);
        
    public :
    
    protected :
    
    private :
        
        /*! @brief The connections to the port. */
        PortConnections _inputConnections;
        
        /*! @brief The connections to the port. */
        PortConnections _outputConnections;
        
        /*! @brief The name of the associated port. */
        yarp::os::ConstString _portName;
        
        /*! @brief The protocol of the associated port. */
        yarp::os::ConstString _portProtocol;
        
        /*! @brief The primary direction for connections to the port. */
        PortDirection _direction;
        
        /*! @brief The primary usage for the port. */
        PortUsage _usage;
        
    }; // PortData
    
} // ChannelManager

#endif // ! defined(PortData_H_)
