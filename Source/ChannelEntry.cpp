//--------------------------------------------------------------------------------------------------
//
//  File:       ChannelEntry.cpp
//
//  Project:    M+M
//
//  Contains:   The class definition for a visible entity that represents a channel or a port.
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

#include "ChannelEntry.h"
#include "ChannelContainer.h"
#include "EntitiesPanel.h"

#include "ODEnableLogging.h"
#include "ODLogging.h"

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 
 @brief The class definition for a visible entity that represents a channel or a port. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

using namespace ChannelManager;
using namespace std;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

/*! @brief The horizontal and vertical length of the arrow 'arm'. */
static const float kArrowSize = 7;

/*! @brief The scale factor to apply to get the length of the control vector. */
static const float kControlLengthScale = 0.25;

/*! @brief The width and height of the marker displayed during movement. */
static const float kMarkerSide = 10;

/*! @brief The line width for a normal connection. */
static const float kNormalConnectionWidth = 2;

/*! @brief The line width for a normal connection. */
static const float kServiceConnectionWidth = (2 * kNormalConnectionWidth);

/*! @brief The scale factor to apply to get the size of the target box. */
static const float kTargetBoxScale = 0.25;

/*! @brief The color to be used for non-TCP/non-UDP connection. */
static const Colour & kOtherConnectionColour(Colours::orange);

/*! @brief The color to be used for markers. */
static const Colour & kMarkerColour(Colours::yellow);

/*! @brief The color to be used for TCP connections. */
static const Colour & kTcpConnectionColour(Colours::teal);

/*! @brief The color to be used for UDP connections. */
static const Colour & kUdpConnectionColour(Colours::purple);

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

/*! @brief Determine if a new point is closer to a reference point than the previous point.
 @param distanceSoFar On input, the closest distance so far and on output, the new closest distance.
 @param refPoint The point to measure distance from.
 @param testPoint The point being checked.
 @param bestSoFar On input, the current closest point and output, the new closest point.
 @returns @c true if the new point is closer than the previous closest point. */
static bool calculateMinDistance(float &              distanceSoFar,
                                 const Point<float> & refPoint,
                                 const Point<float> & testPoint,
                                 Point<float> &       bestSoFar)
{
#if 0
    OD_LOG_ENTER(); //####
    OD_LOG_P4("distanceSoFar = ", &distanceSoFar, "refPoint = ", &refPoint, "testPoint = ", //####
              &testPoint, "bestSoFar = ", &bestSoFar); //####
#endif // 0
    bool  result;
    float newDistance = refPoint.getDistanceFrom(testPoint);
    
    if (newDistance < distanceSoFar)
    {
        distanceSoFar = newDistance;
        bestSoFar = testPoint;
        result = true;
    }
    else
    {
        result = false;
    }
#if 0
    OD_LOG_EXIT_B(result); //####
#endif // 0
    return result;
} // calculateMinDistance

/*! @brief Determine if a new point is closer to a reference point than the previous point.
 @param distanceSoFar On input, the closest distance so far and on output, the new closest distance.
 @param refPoint The point to measure distance from.
 @param testX The horizontal coordinate for the point being checked.
 @param testY The vertical coordinate for the point being checked.
 @param bestSoFar On input, the current closest point and output, the new closest point.
 @returns @c true if the new point is closer than the previous closest point. */
inline static bool calculateMinDistance(float &              distanceSoFar,
                                        const Point<float> & refPoint,
                                        const float          testX,
                                        const float          testY,
                                        Point<float> &       bestSoFar)
{
    return calculateMinDistance(distanceSoFar, refPoint, Point<float>(testX, testY), bestSoFar);
} // CalculateMinDistance

/*! @brief Determine the anchor point that is the minimum distance from a given point.
 @param newCentre The synthesized centre for the target point.
 @param disallowBottom @c true if the anchor cannot be bottom-centre.
 @param targetPoint The target point.
 @param refCentre The reference point.
 @returns The side to which the anchor is attached. */
static AnchorSide calculateAnchorForPoint(Point<float> &       newCentre,
                                          const bool           disallowBottom,
                                          const Point<float> & targetPoint,
                                          const Point<float> & refCentre)
{
#if 0
    OD_LOG_ENTER(); //####
    OD_LOG_P3("newCentre = ", &newCentre, "targetPoint = ", &targetPoint, //####
              "refCentre = ", &refCentre); //####
#endif // 0
    AnchorSide             anchor = kAnchorUnknown;
    float                  boxSize = (refCentre.getDistanceFrom(targetPoint) * kTargetBoxScale);
    float                  soFar = 1e23;           // Ridiculously big, just in case.
    Point<float>           tempPoint;
    juce::Rectangle<float> box(targetPoint.getX() - (boxSize / 2), targetPoint.y - (boxSize / 2),
                               boxSize, boxSize);
    
    if (calculateMinDistance(soFar, refCentre, box.getX(), box.getY() + (boxSize / 2), tempPoint))
    {
        anchor = kAnchorLeft;
        newCentre = targetPoint + Point<float>(boxSize, 0);
    }
    if (calculateMinDistance(soFar, refCentre, box.getX() + boxSize, box.getY() + (boxSize / 2),
                             tempPoint))
    {
        anchor = kAnchorRight;
        newCentre = targetPoint + Point<float>(-boxSize, 0);
    }
    if ((! disallowBottom) && calculateMinDistance(soFar, refCentre, box.getX() + (boxSize / 2),
                                                   box.getY() + boxSize, tempPoint))
    {
        anchor = kAnchorBottomCentre;
        newCentre = targetPoint + Point<float>(0, -boxSize);
    }
    if (calculateMinDistance(soFar, refCentre, box.getX() + (boxSize / 2), box.getY(), tempPoint))
    {
        anchor = kAnchorTopCentre;
        newCentre = targetPoint + Point<float>(0, boxSize);
    }
#if 0
    OD_LOG_EXIT_L(static_cast<int>(anchor)); //####
#endif // 0
    return anchor;
} // calculateAnchorForPoint

/*! @brief Displays an anchor leaving the given location.
 @param gg The graphics context in which to draw.
 @param anchor The side to which the anchor is attached.
 @param anchorPos The coordinates of the anchor point.
 @param thickness The line thickness to be used. */
static void drawSourceAnchor(Graphics &           gg,
                             const AnchorSide     anchor,
                             const Point<float> & anchorPos,
                             const float          thickness)
{
#if 0
    OD_LOG_ENTER(); //####
    OD_LOG_P2("gg = ", &gg, "anchorPos = ", &anchorPos); //####
    OD_LOG_L1("anchor = ", static_cast<int>(anchor)); //####
    OD_LOG_D1("thickness = ", thickness); //####
#endif // 0
    Point<float> first;
    Point<float> second;
    
    switch (anchor)
    {
	    case kAnchorLeft :
            first = anchorPos + Point<float>(kArrowSize, -kArrowSize);
            second = anchorPos + Point<float>(kArrowSize, kArrowSize);
            break;
            
	    case kAnchorRight :
            first = anchorPos + Point<float>(-kArrowSize, -kArrowSize);
            second = anchorPos + Point<float>(-kArrowSize, kArrowSize);
            break;
            
	    case kAnchorBottomCentre :
            first = anchorPos + Point<float>(-kArrowSize, -kArrowSize);
            second = anchorPos + Point<float>(kArrowSize, -kArrowSize);
            break;
            
	    case kAnchorTopCentre :
            first = anchorPos + Point<float>(-kArrowSize, kArrowSize);
            second = anchorPos + Point<float>(kArrowSize, kArrowSize);
            break;
            
	    default :
            break;
            
    }
    if (kAnchorUnknown != anchor)
    {
#if 0
        OD_LOG_D4("anchor.x = ", anchorPos.getX(), "anchor.y = ", anchorPos.getY(), //####
                  "first.x = ", first.getX(), "first.y = ", first.getY()); //####
        OD_LOG_D2("second.x = ", second.getX(), "second.y = ", second.getY()); //####
#endif // 0
        gg.drawLine(anchorPos.getX(), anchorPos.getY(), first.getX(), first.getY(), thickness);
        gg.drawLine(anchorPos.getX(), anchorPos.getY(), second.getX(), second.getY(), thickness);
    }
#if 0
    OD_LOG_EXIT(); //####
#endif // 0
} // drawSourceAnchor

/*! @brief Displays an anchor arriving at the given location.
 @param gg The graphics context in which to draw.
 @param anchor The side to which the anchor is attached.
 @param anchorPos The coordinates of the anchor point.
 @param thickness The line thickness to be used. */
static void drawTargetAnchor(Graphics &           gg,
                             const AnchorSide     anchor,
                             const Point<float> & anchorPos,
                             const float          thickness)
{
#if 0
    OD_LOG_ENTER(); //####
    OD_LOG_P2("gg = ", &gg, "anchorPos = ", &anchorPos); //####
    OD_LOG_L1("anchor = ", static_cast<int>(anchor)); //####
    OD_LOG_D1("thickness = ", thickness); //####
#endif // 0
    Point<float> first;
    Point<float> second;
    
    switch (anchor)
    {
	    case kAnchorLeft :
            first = anchorPos + Point<float>(-kArrowSize, -kArrowSize);
            second = anchorPos + Point<float>(-kArrowSize, kArrowSize);
            break;
            
	    case kAnchorRight :
            first = anchorPos + Point<float>(kArrowSize, -kArrowSize);
            second = anchorPos + Point<float>(kArrowSize, kArrowSize);
            break;
            
	    case kAnchorBottomCentre :
            first = anchorPos + Point<float>(-kArrowSize, kArrowSize);
            second = anchorPos + Point<float>(kArrowSize, kArrowSize);
            break;
            
	    case kAnchorTopCentre :
            first = anchorPos + Point<float>(-kArrowSize, -kArrowSize);
            second = anchorPos + Point<float>(kArrowSize, -kArrowSize);
            break;
            
	    default :
            break;
            
    }
    if (kAnchorUnknown != anchor)
    {
#if 0
        OD_LOG_D4("anchor.x = ", anchorPos.getX(), "anchor.y = ", anchorPos.getY(), //####
                  "first.x = ", first.getX(), "first.y = ", first.getY()); //####
        OD_LOG_D2("second.x = ", second.getX(), "second.y = ", second.getY()); //####
#endif // 0
        gg.drawLine(anchorPos.getX(), anchorPos.getY(), first.getX(), first.getY(), thickness);
        gg.drawLine(anchorPos.getX(), anchorPos.getY(), second.getX(), second.getY(), thickness);
    }
#if 0
    OD_LOG_EXIT(); //####
#endif // 0
} // drawTargetAnchor

/*! @brief Draw a bezier curve between two points.
 @param startPoint The beginning of the curve.
 @param endPoint The end of the curve.
 @param startCentre A reference point for the beginning of the curve, used to calculate the
 beginning tangent.
 @param endCentre A reference point for the end of the curve, used to calculate the ending
 tangent. */
static void drawBezier(Graphics &           gg,
                       const Point<float> & startPoint,
                       const Point<float> & endPoint,
                       const Point<float> & startCentre,
                       const Point<float> & endCentre,
                       const float          thickness)
{
#if 0
    OD_LOG_ENTER(); //####
    OD_LOG_P4("gg = ", &gg, "startPoint = ", &startPoint, "endPoint = ", &endPoint, //####
              "startCentre = ", &startCentre); //####
    OD_LOG_P1("endCentre = ", &endCentre); //####
    OD_LOG_D1("thickness = ", thickness); //####
#endif // 0
    Path         bPath;
    float        controlLength = (startPoint.getDistanceFrom(endPoint) * kControlLengthScale);
    float        startAngle = atan2(startPoint.getY() - startCentre.getY(),
                                    startPoint.getX() - startCentre.getX());
    float        endAngle = atan2(endPoint.getY() - endCentre.getY(),
                                  endPoint.getX() - endCentre.getX());
    Point<float> controlPoint1(controlLength * cos(startAngle), controlLength * sin(startAngle));
    Point<float> controlPoint2(controlLength * cos(endAngle), controlLength * sin(endAngle));
    
    bPath.startNewSubPath(startPoint);
    bPath.cubicTo(startPoint + controlPoint1, endPoint + controlPoint2, endPoint);
    gg.strokePath(bPath, PathStrokeType(thickness));
#if 0
    OD_LOG_EXIT();//####
#endif // 0
} // DrawBezier

/*! @brief Draw a connection between entries.
 @param gg The graphics context in which to draw.
 @param source The originating entry.
 @param destination The terminating entry.
 @param mode The kind of connection. */
static void drawConnection(Graphics &                  gg,
                           ChannelEntry *              source,
                           ChannelEntry *              destination,
                           MplusM::Common::ChannelMode mode)
{
#if 0
    OD_LOG_ENTER(); //####
    OD_LOG_P3("gg = ", &gg, "source = ", source, "destination = ", destination); //####
    OD_LOG_L1("mode = ", static_cast<int>(mode)); //####
#endif // 0
    if (source && destination)
    {
        AnchorSide   sourceAnchor;
        AnchorSide   destinationAnchor;
        Point<float> sourcePosition(source->getPositionInPanel());
        Point<float> destinationPosition(destination->getPositionInPanel());
        Point<float> sourceCentre(source->getCentre() + sourcePosition);
        Point<float> destinationCentre(destination->getCentre() + destinationPosition);
        Point<float> startPoint;
        Point<float> endPoint;
        float        thickness;
        
#if 0
        OD_LOG_D4("sourcePosition.x = ", sourcePosition.getX(), "sourcePosition.y = ", //####
                  sourcePosition.getY(), "destinationPosition.x = ", //####
                  destinationPosition.getX(), "destinationPosition.y = ", //####
                  destinationPosition.getY()); //####
        OD_LOG_D4("sourceCentre.x = ", sourceCentre.getX(), "sourceCentre.y = ", //####
                  sourceCentre.getY(), "destinationCentre.x = ", destinationCentre.getX(), //####
                  "destinationCentre.y = ", destinationCentre.getY()); //####
#endif // 0
        // Check if the destination is above the source, in which case we determine the anchors in
        // the reverse order.
        if (sourceCentre.getY() < destinationCentre.getY())
        {
            sourceAnchor = source->calculateClosestAnchor(startPoint, true, false,
                                                          destinationCentre);
            destinationAnchor = destination->calculateClosestAnchor(endPoint, false,
                                                                kAnchorBottomCentre == sourceAnchor,
                                                                    sourceCentre);
        }
        else
        {
            destinationAnchor = destination->calculateClosestAnchor(endPoint, false, false,
                                                                    sourceCentre);
            sourceAnchor = source->calculateClosestAnchor(startPoint, true,
                                                          kAnchorBottomCentre == destinationAnchor,
                                                          destinationCentre);
        }
#if 0
        OD_LOG_D4("startPoint.x <- ", startPoint.getX(), "startPoint.y <- ", //####
                  startPoint.getY(), "endPoint.x <- ", endPoint.getX(), "endPoint.y <- ", //####
                  endPoint.getY()); //####
#endif // 0
        if (destination->isService())
        {
            thickness = kServiceConnectionWidth;
        }
        else
        {
            thickness = kNormalConnectionWidth;
        }
#if 0
        OD_LOG_D1("thickness <- ", thickness); //####
#endif // 0
        switch (mode)
        {
            case MplusM::Common::kChannelModeTCP :
                gg.setColour(kTcpConnectionColour);
                break;
                
            case MplusM::Common::kChannelModeUDP :
                gg.setColour(kUdpConnectionColour);
                break;
                
            default :
                gg.setColour(kOtherConnectionColour);
                break;
                
        }
#if 0
        OD_LOG_D4("startPoint.x = ", startPoint.getX(), "startPoint.y <= ", //####
                  startPoint.getY(), "endPoint.x = ", endPoint.getX(), //####
                  "endPoint.y = ", endPoint.getY()); //####
#endif // 0
        drawBezier(gg, startPoint, endPoint, sourceCentre, destinationCentre, thickness);
        drawSourceAnchor(gg, sourceAnchor, startPoint, 1);
        drawTargetAnchor(gg, destinationAnchor, endPoint, 1);
    }
#if 0
    OD_LOG_EXIT(); //####
#endif // 0
} // drawConnection

/*! @brief Determine whether a connection can be made, based on the port protocols.
 @param sourceProtocol The protocol of the source port.
 @param destinationProtocol The protocol of the destination port.
 @returns @c true if the protocols permit a connection to be made and @c false
 otherwise. */
static bool protocolsMatch(const String & sourceProtocol,
                           const String & destinationProtocol)
{
    OD_LOG_ENTER(); //####
    OD_LOG_S2s("sourceProtocol = ", sourceProtocol.toStdString(), "destinationProtocol = ", //####
               destinationProtocol.toStdString()); //####
    bool result = false;
    
    if (0 == destinationProtocol.length())
    {
        result = true;
    }
    else
    {
        result = (sourceProtocol == destinationProtocol);
    }
    OD_LOG_EXIT_B(result); //####
    return result;
} // protocolsMatch

#if defined(__APPLE__)
# pragma mark Class methods
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Constructors and destructors
#endif // defined(__APPLE__)

ChannelEntry::ChannelEntry(ChannelContainer *  parent,
                           const String &      portName,
                           const String &      portProtocol,
                           const PortUsage     portKind,
                           const PortDirection direction) :
    inherited(), _portName(portName), _portProtocol(portProtocol), _parent(parent),
    _direction(direction), _usage(portKind), _drawConnectMarker(false),
    _drawDisconnectMarker(false), _isLastPort(true)
{
    OD_LOG_ENTER(); //####
    OD_LOG_P1("parent = ", parent); //####
    OD_LOG_S2s("portName = ", portName.toStdString(), "portProtocol = ", //####
               portProtocol.toStdString()); //####
    OD_LOG_L2("portKind = ", portKind, "direction = ", direction); //####
    Font & textFont = getOwningPanel().getNormalFont();
    String prefix;
    
    switch (_direction)
    {
	    case kPortDirectionInput :
            prefix = "In ";
            break;
            
	    case kPortDirectionInputOutput :
            prefix = "I/O ";
            break;
            
	    case kPortDirectionOutput :
            prefix = "Out ";
            break;
            
	    default :
            prefix = "Unk ";
            break;
            
    }
    _title = prefix + _portName;
    setSize(textFont.getStringWidthFloat(_title + " ") + _parent->getTextInset(),
            textFont.getHeight());
    setVisible(true);
    OD_LOG_EXIT_P(this); //####
} // ChannelEntry::ChannelEntry

ChannelEntry::~ChannelEntry(void)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT(); //####
} // ChannelEntry::~ChannelEntry

#if defined(__APPLE__)
# pragma mark Actions
#endif // defined(__APPLE__)

/*! @brief Add an input connection to the port.
 @param other The port that is to be connected. */
void ChannelEntry::addInputConnection(ChannelEntry *              other,
                                      MplusM::Common::ChannelMode mode)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("other = ", other); //####
    if (other)
    {
        bool canAdd = true;
        
        for (Connections::iterator walker(_inputConnections.begin());
             _inputConnections.end() != walker; ++walker)
        {
            if ((walker->_otherPort == other) ||
                (walker->_otherPort->getPortName() == other->getPortName()))
            {
                OD_LOG("already present"); //####
                walker->_valid = true;
                canAdd = false;
                break;
            }
            
        }
        if (canAdd)
        {
            PortConnection newConnection;
            
            newConnection._otherPort = other;
            newConnection._connectionMode = mode;
            newConnection._valid = true;
            _inputConnections.push_back(newConnection);
        }
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelEntry::addInputConnection

/*! @brief Add an output connection to the port.
 @param other The port that is to be connected. */
void ChannelEntry::addOutputConnection(ChannelEntry *              other,
                                       MplusM::Common::ChannelMode mode)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("other = ", other); //####
    if (other)
    {
        bool canAdd = true;
        
        for (Connections::iterator walker(_outputConnections.begin());
             _outputConnections.end() != walker; ++walker)
        {
            if ((walker->_otherPort == other) ||
                (walker->_otherPort->getPortName() == other->getPortName()))
            {
                OD_LOG("already present"); //####
                walker->_valid = true;
                canAdd = false;
                break;
            }
            
        }
        if (canAdd)
        {
            PortConnection newConnection;
            
            newConnection._otherPort = other;
            newConnection._connectionMode = mode;
            newConnection._valid = true;
            _outputConnections.push_back(newConnection);
        }
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelEntry::addOutputConnection

AnchorSide ChannelEntry::calculateClosestAnchor(Point<float> &       result,
                                                const bool           isSource,
                                                const bool           disallowBottom,
                                                const Point<float> & pp)
const
{
#if 0
    OD_LOG_OBJENTER(); //####
    OD_LOG_P2("result = ", &result, "pp = ", &pp); //####
    OD_LOG_B1("isSource = ", isSource); //####
#endif // 0
    // Check each anchor point - the two side centres and optionally the bottom - to find the
    // shortest distance.
    AnchorSide   anchor = kAnchorUnknown;
    float        soFar = 1e23; // Ridiculously big, just in case.
    Point<float> location(getPositionInPanel());
    
    if (calculateMinDistance(soFar, pp, location.getX(), location.getY() + (getHeight() / 2),
                             result))
    {
        anchor = kAnchorLeft;
        if (isSource)
        {
            // Adjust the anchor position if an output.
            result.x -= kArrowSize;
        }
    }
    if (calculateMinDistance(soFar, pp, location.getX() + getWidth(),
                             location.getY() + (getHeight() / 2), result))
    {
        anchor = kAnchorRight;
        if (isSource)
        {
            // Adjust the anchor position if an output.
            result.x += kArrowSize;
        }
    }
    if (_isLastPort && (! disallowBottom))
    {
        if (calculateMinDistance(soFar, pp, location.getX() + (getWidth() / 2),
                                 location.getY() + getHeight(), result))
        {
            anchor = kAnchorBottomCentre;
            if (isSource)
            {
                // Adjust the anchor position if an output.
                result.y += kArrowSize;
            }
        }
    }
#if 0
    OD_LOG_D2("result.x = ", result.getX(), "result.y = ", result.getY()); //####
    OD_LOG_OBJEXIT_L(static_cast<int>(anchor)); //####
#endif // 0
    return anchor;
} // ChannelEntry::calculateClosestAnchor

void ChannelEntry::drawOutgoingConnections(Graphics & gg)
{
#if 0
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("gg = ", &gg); //####
#endif // 0
    for (Connections::const_iterator walker(_outputConnections.begin());
         _outputConnections.end() != walker; ++walker)
    {
        drawConnection(gg, this, walker->_otherPort, walker->_connectionMode);
    }
#if 0
    OD_LOG_OBJEXIT(); //####
#endif // 0
} // ChannelEntry::drawOutgoingConnections

EntitiesPanel & ChannelEntry::getOwningPanel(void)
const
{
#if 0
    OD_LOG_OBJENTER(); //####
#endif // 0
    EntitiesPanel & result = _parent->getOwner();
    
#if 0
    OD_LOG_OBJEXIT_P(&result); //####
#endif // 0
    return result;
} // ChannelEntry::getOwningPanel

Point<float> ChannelEntry::getPositionInPanel(void)
const
{
#if 0
    OD_LOG_OBJENTER(); //####
#endif // 0
    Point<float> result(getPosition().toFloat() + _parent->getPositionInPanel());
    
#if 0
    OD_LOG_OBJEXIT(); //####
#endif // 0
    return result;
} // ChannelEntry::getPositionInPanel

void ChannelEntry::invalidateConnections(void)
{
    OD_LOG_OBJENTER(); //####
    for (Connections::iterator walker(_inputConnections.begin()); _inputConnections.end() != walker;
         ++walker)
    {
        walker->_valid = false;
    }
    for (Connections::iterator walker(_outputConnections.begin());
         _outputConnections.end() != walker; ++walker)
    {
        walker->_valid = false;
    }
    OD_LOG_EXIT(); //####
} // ChannelEntry::invalidateConnections

void ChannelEntry::mouseDown(const MouseEvent & ee)
{
    OD_LOG_OBJENTER(); //####
    bool           passOn = true;
    bool           wasUdp;
    ChannelEntry * firstAddPort = getOwningPanel().getFirstAddPoint(wasUdp);
    ChannelEntry * firstRemovePort = getOwningPanel().getFirstRemovePoint();
    
    if (firstRemovePort)
    {
        // We started a 'remove' operation.
        clearDisconnectMarker();
        repaint();
        getOwningPanel().rememberConnectionStartPoint();
        passOn = false;
        if (firstRemovePort != this)
        {
            // Check if we can end here.
            String firstName(firstRemovePort->getPortName());
            
            // Check if we can end here.
            firstRemovePort->clearDisconnectMarker();
            firstRemovePort->repaint();
            if (kPortDirectionOutput != _direction)
            {
                if (MplusM::Utilities::RemoveConnection(firstName.toStdString().c_str(),
                                                        getPortName().toStdString().c_str()))
                {
                    firstRemovePort->removeOutputConnection(this);
                    removeInputConnection(firstRemovePort);
                    getOwningPanel().repaint();
                }
            }
        }
    }
    else if (firstAddPort)
    {
        // We started an 'add' operation.
        clearConnectMarker();
        repaint();
        getOwningPanel().rememberConnectionStartPoint();
        passOn = false;
        if (firstAddPort != this)
        {
            // Check if we can end here.
            String firstName(firstAddPort->getPortName());
            String firstProtocol(firstAddPort->getProtocol());
            
            // Check if we can end here.
            firstAddPort->clearConnectMarker();
            firstAddPort->repaint();
            if ((kPortDirectionOutput != _direction) &&
                protocolsMatch(firstProtocol, _portProtocol))
            {
                if (MplusM::Utilities::AddConnection(firstName.toStdString().c_str(),
                                                     getPortName().toStdString().c_str(),
                                                     STANDARD_WAIT_TIME, wasUdp))
                {
                    MplusM::Common::ChannelMode mode = (wasUdp ? MplusM::Common::kChannelModeUDP :
                                                        MplusM::Common::kChannelModeTCP);
                    
                    firstAddPort->addOutputConnection(this, mode);
                    addInputConnection(firstAddPort, mode);
                    getOwningPanel().repaint();
                }
            }
        }
    }
    else
    {
        // No active operation.
        if (ee.mods.isAltDown())
        {
            // Check if Add is OK for this entry.
            if ((kPortDirectionInput != _direction) &&
                (kPortUsageClient != _usage))
            {
                getOwningPanel().rememberConnectionStartPoint(this, true, ee.mods.isShiftDown());
                setConnectMarker();
                repaint();
            }
            passOn = false;
            OD_LOG("ALT"); //####
        }
        else if (ee.mods.isCommandDown())
        {
            // Check if Remove is OK for this entry.
            if ((kPortDirectionInput != _direction) &&
                (kPortUsageClient != _usage))
            {
                getOwningPanel().rememberConnectionStartPoint(this, false);
                setDisconnectMarker();
                repaint();
            }
            passOn = false;
            OD_LOG("COMMAND"); //####
        }
        else if (ee.mods.isCtrlDown())
        {
            // Popup of description.
            passOn = false;
            OD_LOG("CTRL"); //####
        }
    }
    if (passOn)
    {
        _parent->mouseDown(ee);
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelEntry::mouseDown

void ChannelEntry::mouseDrag(const MouseEvent & ee)
{
    OD_LOG_OBJENTER(); //####
    bool passOn = true;

    if (ee.mods.isAltDown() || ee.mods.isCommandDown() || ee.mods.isCtrlDown())
    {
        passOn = false;
    }
    if (passOn)
    {
        _parent->mouseDrag(ee);
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelEntry::mouseDrag

void ChannelEntry::mouseUp(const MouseEvent & ee)
{
    OD_LOG_OBJENTER(); //####
    bool passOn = true;
    
    if (ee.mods.isAltDown())
    {
        // Check if we are processing an Add and this is OK.
        passOn = false;
        OD_LOG("ALT"); //####
    }
    else if (ee.mods.isCommandDown())
    {
        // Check if we are processing a Remove and this is OK.
        passOn = false;
        OD_LOG("COMMAND"); //####
    }
    else if (ee.mods.isCtrlDown())
    {
        passOn = false;
        OD_LOG("CTRL"); //####
    }
    if (passOn)
    {
        _parent->mouseUp(ee);
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelEntry::mouseUp

void ChannelEntry::paint(Graphics & gg)
{
#if 0
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("gg = ", &gg); //####
#endif // 0
    AttributedString as;
    
    as.setJustification(Justification::left);
    as.append(_title, getOwningPanel().getNormalFont(), Colours::white);
    juce::Rectangle<float> area(getLocalBounds().toFloat());
    
#if 0
    OD_LOG_D4("x <- ", area.getX(), "y <- ", area.getY(), "w <- ",area.getWidth(), "h <- ", //####
              area.getHeight()); //####
#endif // 0
    gg.setColour(Colours::black);
    gg.fillRect(area);
    area.setLeft(area.getX() + _parent->getTextInset());
    as.draw(gg, area);
    if (_drawConnectMarker)
    {
        Point<float> markerPos(getCentre() - Point<float>(kMarkerSide / 2, kMarkerSide / 2));
        
        gg.setColour(kMarkerColour);
        gg.fillEllipse(markerPos.getX(), markerPos.getY(), kMarkerSide, kMarkerSide);
    }
    else if (_drawDisconnectMarker)
    {
        Point<float> markerPos(getCentre() - Point<float>(kMarkerSide / 2, kMarkerSide / 2));
        
        gg.setColour(kMarkerColour);
        gg.drawEllipse(markerPos.getX(), markerPos.getY(), kMarkerSide, kMarkerSide, 2);
    }
#if 0
    OD_LOG_OBJEXIT(); //####
#endif // 0
} // ChannelEntry::paint

void ChannelEntry::removeInputConnection(ChannelEntry * other)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("other = ", other); //####
    if (other)
    {
        Connections::iterator walker(_inputConnections.begin());
        
        for ( ; _inputConnections.end() != walker; ++walker)
        {
            if (walker->_otherPort == other)
            {
                break;
            }
            
        }
        if (_inputConnections.end() != walker)
        {
            _inputConnections.erase(walker);
        }
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelEntry::removeInputConnection

void ChannelEntry::removeInvalidConnections(void)
{
    OD_LOG_OBJENTER(); //####
    for (Connections::iterator walker(_inputConnections.begin()); _inputConnections.end() != walker;
         ++walker)
    {
        if (! walker->_valid)
        {
            _inputConnections.erase(walker);
        }
    }
    for (Connections::iterator walker(_outputConnections.begin());
         _outputConnections.end() != walker; ++walker)
    {
        if (! walker->_valid)
        {
            _inputConnections.erase(walker);
        }
    }
    OD_LOG_EXIT(); //####
} // ChannelEntry::removeInvalidConnections

void ChannelEntry::removeOutputConnection(ChannelEntry * other)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("other = ", other); //####
    if (other)
    {
        Connections::iterator walker(_outputConnections.begin());
        
        for ( ; _outputConnections.end() != walker; ++walker)
        {
            if (walker->_otherPort == other)
            {
                break;
            }
        }
        if (_outputConnections.end() != walker)
        {
            _outputConnections.erase(walker);
        }
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelEntry::removeOutputConnection

#if defined(__APPLE__)
# pragma mark Accessors
#endif // defined(__APPLE__)

Point<float> ChannelEntry::getCentre(void)
const
{
#if 0
    OD_LOG_OBJENTER(); //####
#endif // 0
    juce::Rectangle<float> outer(getLocalBounds().toFloat());
    
#if 0
    OD_LOG_OBJEXIT(); //####
#endif // 0
    return outer.getCentre();
} // ChannelEntry::getCentre

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)
