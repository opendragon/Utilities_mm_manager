//--------------------------------------------------------------------------------------------------
//
//  File:       ChannelEntry.h
//
//  Project:    M+M
//
//  Contains:   The class declaration for a visible entity that represents a channel or a port.
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
//  Created:    2014-07-16
//
//--------------------------------------------------------------------------------------------------

#if (! defined(ChannelEntry_H_))
# define ChannelEntry_H_ /* Header guard */

# include "ChannelsDataTypes.h"

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file
 
 @brief The class declaration for a visible entity that represents a channel or a port. */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

namespace ChannelManager
{
    class ChannelContainer;
    class EntitiesPanel;
    
    /*! @brief A port or channel. */
    class ChannelEntry : public Component
    {
    public:
        
        /*! @brief The constructor.
         @param parent The GUI element containing this element.
         @param portName The port name for the entry.
         @param portProtocol The protocol of the port.
         @param portKind What the port will be used for.
         @param direction The primary direction of the port. */
        ChannelEntry(ChannelContainer *  parent,
                     const String &      portName,
                     const String &      portProtocol,
                     const PortUsage     portKind,
                     const PortDirection direction = kPortDirectionInput);
        
        /*! @brief The destructor. */
        virtual ~ChannelEntry(void);
        
        /*! @brief Add an input connection to the port.
         @param other The port that is to be connected.
         @param mode The mode of the connection. */
        void addInputConnection(ChannelEntry *              other,
                                MplusM::Common::ChannelMode mode);
        
        /*! @brief Add an output connection to the port.
         @param other The port that is to be connected.
         @param mode The mode of the connection. */
        void addOutputConnection(ChannelEntry *              other,
                                 MplusM::Common::ChannelMode mode);
        
        /*! @brief Determine the anchor point that is the minimum distance from a given point.
         @param result The coordinates of the anchor point.
         @param isSource @c true if the anchor is for an outgoing line and @c false otherwise.
         @param disallowBottom @c true if the anchor cannot be bottom-centre.
         @param xx The horizontal coordinate for the point of interest.
         @param yy The vertical coordinate for the point of interest.
         @returns The side to which the anchor is attached. */
        inline AnchorSide calculateClosestAnchor(Point<float> & result,
                                                 const bool     isSource,
                                                 const bool     disallowBottom,
                                                 const float    xx,
                                                 const float    yy)
        const
        {
            return calculateClosestAnchor(result, isSource, disallowBottom, Point<float>(xx, yy));
        } // calculateClosestAnchor
        
        /*! @brief Determine the anchor point that is the minimum distance from a given point.
         @param result The coordinates of the anchor point.
         @param isSource @c true if the anchor is for an outgoing line and @c false otherwise.
         @param disallowBottom @c true if the anchor cannot be bottom-centre.
         @param pp The point of interest.
         @returns The side to which the anchor is attached. */
        AnchorSide calculateClosestAnchor(Point<float> &       result,
                                          const bool           isSource,
                                          const bool           disallowBottom,
                                          const Point<float> & pp)
        const;
        
        /*! @brief Stop displaying the connect marker. */
        inline void clearConnectMarker(void)
        {
            _drawConnectMarker = false;
        } // clearConnectMarker
        
        /*! @brief Stop displaying the disconnect marker. */
        inline void clearDisconnectMarker(void)
        {
            _drawDisconnectMarker = false;
        } // clearDisconnectMarker
        
        /*! @brief Display the connections between containers.
         @param gg The graphics context in which to draw. */
        void drawOutgoingConnections(Graphics & gg);
        
        /*! @brief Return the location of the centre of the port entry.
         @returns The location of the centre of the port entry. */
        Point<float> getCentre(void)
        const;
        
        /*! @brief Return the direction of the port entry.
         @returns The direction of the port entry. */
        PortDirection getDirection(void)
        const
        {
            return _direction;
        } // getDirection
        
        /*! @brief Return the set of input connections to the port.
         @returns The set of input connections to the port. */
        inline const Connections & getInputConnections(void)
        const
        {
            return _inputConnections;
        } // getInputConnections
        
        /*! @brief Return the set of output connections to the port.
         @returns The set of output connections to the port. */
        inline const Connections & getOutputConnections(void)
        const
        {
            return _outputConnections;
        } // getOutputConnections
        
        /*! @brief Return the panel which contains the entry.
         @returns The panel which contains the entry. */
        EntitiesPanel & getOwningPanel(void)
        const;
        
        inline ChannelContainer * getParent(void)
        const
        {
            return _parent;
        } // getParent
        
        /*! @brief Return the name of the associated port.
         @returns The name of the associated port. */
        inline String getPortName(void)
        const
        {
            return _portName;
        } // getPortName
        
        /*! @brief Return the position of the entity within it's containing panel.
         @returns The position of the entity within it's containing panel. */
        Point<float> getPositionInPanel(void)
        const;
        
        /*! @brief Return the protocol of the associated port.
         @returns The protocol of the associated port. */
        inline String getProtocol(void)
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
        
        /*! @brief Mark all the connections as invalid. */
        void invalidateConnections(void);
        
        /*! @brief Returns @c true if the port entry is the bottom-most (last) port entry in a
         panel and @c false otherwise.
         @returns @c true if the port is the last port entry in a panel and @c false
         otherwise. */
        inline bool isLastPort(void)
        const
        {
            return _isLastPort;
        } // isLastPort
        
        /*! @brief Returns @c true if the port entry is part of a service and @c false
         otherwise.
         @returns @c true if the port is part of a service and @c false otherwise. */
        inline bool isService(void)
        const
        {
            return (kPortUsageService == _usage);
        } // isService
        
        /*! @brief Called when a mouse button is pressed.
         @param ee Details about the position and status of the mouse event. */
        virtual void mouseDown(const MouseEvent & ee) override;
        
        /*! @brief Called when the mouse is moved while a button is held down.
         @param ee Details about the position and status of the mouse event. */
        virtual void mouseDrag(const MouseEvent & ee) override;
        
        /*! @brief Called when a mouse button is released.
         @param ee Details about the position and status of the mouse event. */
        virtual void mouseUp (const MouseEvent& event) override;
        
        /*! @brief Draw the content of the component.
         @param gg The graphics context in which to draw. */
        void paint(Graphics & gg) override;
        
        /*! @brief Remove an input connection from a port.
         @param other The port that is to be disconnected. */
        void removeInputConnection(ChannelEntry * other);
        
        /*! @brief Remove connections that are invalid. */
        void removeInvalidConnections(void);
        
        /*! @brief Remove an output connection from a port.
         @param other The port that is to be disconnected. */
        void removeOutputConnection(ChannelEntry * other);
        
        /*! @brief Mark the port entry as the bottom-most (last) port entry in a panel. */
        inline void setAsLastPort(void)
        {
            _isLastPort = true;
        } // setAsLastPort
        
        /*! @brief Start displaying the connect marker. */
        inline void setConnectMarker(void)
        {
            _drawConnectMarker = true;
        } // setConnectMarker
        
        /*! @brief Start displaying the disconnect marker. */
        inline void setDisconnectMarker(void)
        {
            _drawDisconnectMarker = true;
        } // setDisconnectMarker
        
        /*! @brief Mark the port entry as not being the bottom-most port entry in a panel. */
        inline void unsetAsLastPort(void)
        {
            _isLastPort = false;
        } // unsetAsLastPort
        
    protected:
        
    private:
        
        /*! @brief The class that this class is derived from. */
        typedef Component inherited;

        /*! @brief The connections to the port. */
        Connections _inputConnections;
        
        /*! @brief The connections to the port. */
        Connections _outputConnections;
        
        /*! @brief The name of the associated port. */
        String _portName;
        
        /*! @brief The protocol of the associated port. */
        String _portProtocol;
        
        /*! @brief The text to be displayed for the channel entry. */
        String _title;
        
        /*! @brief The container in which this is embedded. */
        ChannelContainer * _parent;
        
        /*! @brief The primary direction for connections to the port. */
        PortDirection _direction;
        
        /*! @brief The primary usage for the port. */
        PortUsage _usage;
        
        /*! @brief @c true if the connect marker is to be displayed and @c false otherwise. */
        bool _drawConnectMarker;
        
        /*! @brief @c true if the disconnect marker is to be displayed and @c false otherwise. */
        bool _drawDisconnectMarker;
        
        /*! @brief @c true if the channel entry is the bottom-most (last) channel entry in a panel
         and @c false otherwise. */
        bool _isLastPort;
        
# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wunused-private-field"
# endif // defined(__APPLE__)
        /*! @brief Filler to pad to alignment boundary */
        char _filler[5];
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChannelEntry)
        
    }; // ChannelEntry
    
} // ChannelManager

#endif // ! defined(ChannelContainer_H_)
