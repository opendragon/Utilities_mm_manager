//--------------------------------------------------------------------------------------------------
//
//  File:       ApplicationSettingsWindow.cpp
//
//  Project:    M+M
//
//  Contains:   The class definition for the application settings window of the channel manager
//              application.
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
//  Created:    2015-06-10
//
//--------------------------------------------------------------------------------------------------

#include "ApplicationSettingsWindow.h"
#include "ChannelManagerApplication.h"

#include <mpm/M+MEndpoint.h>

//#include <odl/ODEnableLogging.h>
#include <odl/ODLogging.h>

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunknown-pragmas"
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 
 @brief The class definition for the application settings window of the channel manager
 application. */
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

/*! @brief The horizontal gap between buttons. */
static const int kButtonGap = 10;

/*! @brief The amount to add to the height of text editor fields. */
static const int kEditorHeightAdjustment = 4;

/*! @brief The overhead for the buttons on the title bar. */
static const int kTitleBarMinWidth = 80;

/*! @brief The colour to be used for the window background. */
static const Colour & kWindowBackgroundColour(Colours::whitesmoke);

/*! @brief The internal name for the endpoint text entry field. */
static const String kEndpointFieldName("$$$endpoint$$$");

/*! @brief The internal name for the port text entry field. */
static const String kPortFieldName("$$$port$$$");

/*! @brief The internal name for the tag text entry field. */
static const String kTagFieldName("$$$tag$$$");

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

ApplicationSettingsWindow::ApplicationSettingsWindow(const String &          title,
                                                     const String &          execType,
                                                     const ApplicationInfo & appInfo,
                                                     String &                endpointToUse,
                                                     String &                tagToUse,
                                                     String &                portToUse,
                                                     StringArray &           argsToUse)  :
    inherited1(), inherited2(title, kWindowBackgroundColour, inherited2::closeButton),
    _topText("topText"), _cancelButton("Cancel"), _okButton("OK"),
    _descriptors(appInfo._argDescriptions),
    _monoFont(Font::getDefaultMonospacedFontName(), 16, Font::plain), _execType(execType),
    _endpointCaption(nullptr), _extraArgumentsCaption(nullptr), _portCaption(nullptr),
    _tagCaption(nullptr), _addArgumentsButton(nullptr), _removeArgumentsButton(nullptr),
    _endpointEditor(nullptr), _portEditor(nullptr), _tagEditor(nullptr), _appInfo(appInfo),
    _endpointToUse(endpointToUse), _portToUse(portToUse), _tagToUse(tagToUse),
    _argsToUse(argsToUse), _canHaveExtraArguments(false), _canSetEndpoint(false),
    _canSetPort(false), _canSetTag(false)
{
    OD_LOG_ENTER(); //####
    OD_LOG_S2s("title = ", title, "execType = ", execType); //####
    OD_LOG_P4("appInfo = ", &appInfo, "endpointToUse = ", &endpointToUse, "tagToUse = ", //####
              &tagToUse, "portToUse = ", &portToUse); //####
    OD_LOG_P1("argsToUse = ", &argsToUse); //####
    BorderSize<int> bt = getBorderThickness();
    BorderSize<int> cb = getContentComponentBorder();
    int             heightSoFar = 0;
    int             widthSoFar = 0;

    _adjustedEditorHeight = _monoFont.getHeight() + kEditorHeightAdjustment;
    _argsToUse.clear();
    _canSetEndpoint = appInfo._options.contains("e");
    _canSetPort = appInfo._options.contains("p");
    _canSetTag = appInfo._options.contains("t");
    setUpStandardFields(widthSoFar, heightSoFar);
    int buttonRowHeight = jmax(_cancelButton.getHeight(), _okButton.getHeight());
    int titleW = _monoFont.getStringWidth(getName()) + kTitleBarMinWidth;
    int topW = widthSoFar + kButtonGap;
    int minW = jmax(topW, jmax(titleW,
                               _cancelButton.getWidth() + _okButton.getWidth() + (3 * kButtonGap)));
    int calcW = minW + bt.getLeftAndRight() + cb.getLeftAndRight();
    int calcH = heightSoFar + bt.getTopAndBottom() + cb.getTopAndBottom();
    
    centreWithSize(calcW, calcH);
    adjustFields();
    setOpaque(true);
    setResizable(false, false);
    setVisible(true);
    addKeyListener(ChannelManagerWindow::getApplicationCommandManager().getKeyMappings());
    triggerAsyncUpdate();
    OD_LOG_EXIT_P(this); //####
} // ApplicationSettingsWindow::ApplicationSettingsWindow

ApplicationSettingsWindow::~ApplicationSettingsWindow(void)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT(); //####
} // ApplicationSettingsWindow::~ApplicationSettingsWindow

#if defined(__APPLE__)
# pragma mark Actions and Accessors
#endif // defined(__APPLE__)

void ApplicationSettingsWindow::addAnExtraField(void)
{
    OD_LOG_ENTER(); //####
    String     compCountAsString(static_cast<int>(_extraFieldEditors.size() + 1));
    String     compName(_extraArgRootName + "_" + compCountAsString);
    String     compTitle(_extraArgRootName + " " + compCountAsString);
    Label *    newLabel = new Label("", compTitle);
    Point<int> dimensions;
    
    _extraFieldLabels.add(newLabel);
    newLabel->setFont(_monoFont);
    ChannelManager::CalculateTextArea(dimensions, _monoFont, newLabel->getText());
    newLabel->setBounds(2 * kButtonGap, 0, dimensions.getX(), dimensions.getY());
    Component::addAndMakeVisible(newLabel);
    TextEditor * newEditor = new TextEditor(compName);

    _extraFieldEditors.add(newEditor);
    newEditor->setFont(_monoFont);
    newEditor->setBounds(newLabel->getX(), 0, 0, static_cast<int>(_adjustedEditorHeight));
    newEditor->setSelectAllWhenFocused(true);
    Component::addAndMakeVisible(newEditor);
    recalculateArea();
    adjustFields();
    if (_removeArgumentsButton)
    {
        _removeArgumentsButton->setVisible(true);
    }
    OD_LOG_EXIT(); //####
} // ApplicationSettingsWindow::addAnExtraField

void ApplicationSettingsWindow::adjustFields(void)
{
    OD_LOG_OBJENTER(); //####
    int newButtonTop = getHeight() - (_cancelButton.getHeight() + kButtonGap);
    int newFieldWidth = getWidth() - (4 * kButtonGap);
    
    _cancelButton.setTopLeftPosition(getWidth() - (_cancelButton.getWidth() + kButtonGap),
                                     newButtonTop);
    _okButton.setTopLeftPosition(_cancelButton.getX() - (_okButton.getWidth() + kButtonGap),
                                 newButtonTop);
    if (_endpointEditor)
    {
        _endpointEditor->setBounds(_endpointEditor->getX(), _endpointEditor->getY(), newFieldWidth,
                                   _endpointEditor->getHeight());
    }
    if (_portEditor)
    {
        _portEditor->setBounds(_portEditor->getX(), _portEditor->getY(), newFieldWidth,
                               _portEditor->getHeight());
    }
    if (_tagEditor)
    {
        _tagEditor->setBounds(_tagEditor->getX(), _tagEditor->getY(), newFieldWidth,
                              _tagEditor->getHeight());
    }
    if (_addArgumentsButton)
    {
        _addArgumentsButton->setTopLeftPosition(_okButton.getX() -
                                                (_addArgumentsButton->getWidth() + kButtonGap),
                                                newButtonTop);
    }
    if (_removeArgumentsButton)
    {
        _removeArgumentsButton->setTopLeftPosition(_addArgumentsButton->getX() -
                                               (_removeArgumentsButton->getWidth() + kButtonGap),
                                                   newButtonTop);
    }
    for (int ii = 0, maxf = _standardFieldEditors.size(); maxf > ii; ++ii)
    {
        TextEditor * anEditor = _standardFieldEditors[ii];
        
        anEditor->setBounds(anEditor->getX(), anEditor->getY(), newFieldWidth,
                            anEditor->getHeight());
    }
    for (int ii = 0, maxf = _extraFieldEditors.size(); maxf > ii; ++ii)
    {
        TextEditor * anEditor = _extraFieldEditors[ii];
        
        anEditor->setBounds(anEditor->getX(), anEditor->getY(), newFieldWidth,
                            anEditor->getHeight());
    }
    OD_LOG_OBJEXIT(); //####
} // ApplicationSettingsWindow::adjustFields

void ApplicationSettingsWindow::buttonClicked(Button * aButton)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("aButton = ", aButton); //####
    Component * parent = aButton->getParentComponent();
    int         commandId = aButton->getCommandID();
    
    if (parent)
    {
        switch (commandId)
        {
            case kSettingsAddField :
                addAnExtraField();
                break;
                
            case kSettingsRemoveField :
                removeMostRecentlyAddedExtraField();
                break;
                
            case kSettingsAccept :
                if (fieldsAreValid())
                {
                    parent->exitModalState(commandId);
                }
                break;
                
            default :
                parent->exitModalState(commandId);
                break;
                
        }
    }
    OD_LOG_OBJEXIT(); //####
} // ApplicationSettingsWindow::buttonClicked

void ApplicationSettingsWindow::closeButtonPressed(void)
{
    OD_LOG_OBJENTER(); //####
    setVisible(false);
    OD_LOG_OBJEXIT(); //####
} // ApplicationSettingsWindow::closeButtonPressed

bool ApplicationSettingsWindow::fieldsAreValid(void)
{
    OD_LOG_ENTER(); //####
    int    badCount = 0;
    String badArgs;
    String primaryChannel;
    
    // Counterintuitively, we check the values from the descriptors first, before checking the
    // endpoint, port or tag values.
    for (int ii = 0, maxf = _standardFieldEditors.size(); maxf > ii; ++ii)
    {
        TextEditor * anEditor = _standardFieldEditors[ii];
        int          descriptorIndex = anEditor->getComponentID().getIntValue();

        if ((0 <= descriptorIndex) && (descriptorIndex < static_cast<int>(_descriptors.size())))
        {
            Utilities::BaseArgumentDescriptor * aDescriptor = _descriptors[descriptorIndex];
            
            if (aDescriptor && (! aDescriptor->isExtra()))
            {
                String argName = aDescriptor->argumentName().c_str();
                String anArg = anEditor->getText();
                
                cerr << anArg << endl;
                if (0 == anArg.length())
                {
                    if (aDescriptor->isOptional())
                    {
                        _argsToUse.add(aDescriptor->getDefaultValue().c_str());
                    }
                    else
                    {
                        if (0 < badArgs.length())
                        {
                            badArgs += "\n";
                        }
                        badArgs += aDescriptor->argumentName().c_str();
                        ++badCount;
                    }
                }
                else if (aDescriptor->validate(anArg.toStdString()))
                {
                    _argsToUse.add(anArg);
                }
                else
                {
                    if (0 < badArgs.length())
                    {
                        badArgs += "\n";
                    }
                    badArgs += aDescriptor->argumentName().c_str();
                    ++badCount;
                }
            }
        }
    }
    // Add the extra arguments here.
    for (int ii = 0, maxf = _extraFieldEditors.size(); maxf > ii; ++ii)
    {
        TextEditor * anEditor = _extraFieldEditors[ii];

        _argsToUse.add(anEditor->getText());
    }
    if (_canSetEndpoint)
    {
        _endpointToUse = _endpointEditor->getText();
        if ((0 < _endpointToUse.length()) &&
            (! Common::Endpoint::CheckEndpointName(_endpointToUse.toStdString())))
        {
            if (0 < badArgs.length())
            {
                badArgs += "\n";
            }
            badArgs += "Endpoint";
            ++badCount;
        }
    }
    if (_canSetPort)
    {
        _portToUse = _portEditor->getText();
        int portChoice = _portToUse.getIntValue();
        
        if ((0 != portChoice) && (! Utilities::ValidPortNumber(portChoice)))
        {
            if (0 < badArgs.length())
            {
                badArgs += "\n";
            }
            badArgs += "Port";
            ++badCount;
        }
    }
    if (_canSetTag)
    {
        _tagToUse = _tagEditor->getText();
    }
    if (0 < badCount)
    {
        String message1((1 < badCount) ? "arguments are" : "argument is");
        String message2((1 < badCount) ? "arguments" : "argument");
        
        AlertWindow::showMessageBox(AlertWindow::WarningIcon, getName(),
                                    String("The following ") + message1 + " invalid:\n" + badArgs +
                                    "\n" + String("Please correct the ") + message2 + " to the " +
                                    _execType + " and try again.", String::empty, this);
    }
    else
    {
        ChannelManagerApplication * ourApp = ChannelManagerApplication::getApp();
        
        if (ourApp->getPrimaryChannelForService(_appInfo, _endpointToUse, _tagToUse, _portToUse,
                                                _argsToUse, primaryChannel))
        {
            if (Utilities::CheckForChannel(primaryChannel.toStdString()))
            {
                AlertWindow::showMessageBox(AlertWindow::WarningIcon, getName(),
                                            String("The primary channel\n") + primaryChannel +
                                            "\nof the " + _execType + " is in use.\n"
                                            "Please correct the arguments to the " + _execType +
                                            " and try again.", String::empty, this);
                badCount = -1; // Flag this as bad.
            }
        }
        else
        {
            AlertWindow::showMessageBox(AlertWindow::WarningIcon, getName(),
                                        "There was a problem retrieving the primary channel for "
                                        "the " + _execType + ".\n"
                                        "Please try again.", String::empty, this);
            badCount = -1; // Flag this as bad.
        }
    }
    OD_LOG_EXIT_B(0 == badCount); //####
    return (0 == badCount);
} // ApplicationSettingsWindow::fieldsAreValid

void ApplicationSettingsWindow::resized(void)
{
    OD_LOG_OBJENTER(); //####
    Button * close = getCloseButton();
    
    inherited2::resized();
    if (close)
    {
        const KeyPress esc(KeyPress::escapeKey, 0, 0);
        
        if (! close->isRegisteredForShortcut(esc))
        {
            close->addShortcut(esc);
        }
    }
    OD_LOG_OBJEXIT(); //####
} // ApplicationSettingsWindow::resized

void ApplicationSettingsWindow::handleAsyncUpdate(void)
{
    OD_LOG_OBJENTER(); //####
    ApplicationCommandManager & commandManager =
                                            ChannelManagerWindow::getApplicationCommandManager();

    commandManager.registerAllCommandsForTarget(JUCEApplication::getInstance());
    OD_LOG_OBJEXIT(); //####
} // ApplicationSettingsWindow::handleAsyncUpdate

bool ApplicationSettingsWindow::keyPressed(const KeyPress & key)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("key = ", &key); //####
    bool result;
    
    if (key == KeyPress::escapeKey)
    {
        closeButtonPressed();
        result = true;
    }
    else
    {
        result = inherited2::keyPressed(key);
    }
    OD_LOG_OBJEXIT_B(result); //####
    return result;
} // ApplicationSettingsWindow::keyPressed

void ApplicationSettingsWindow::recalculateArea(void)
{
    OD_LOG_ENTER(); //####
    int buttonHeight = getLookAndFeel().getAlertWindowButtonHeight();
    int heightSoFar = 0;
    int widthSoFar = 0;
    
    heightSoFar = _topText.getY() + _topText.getHeight() + (kButtonGap / 2);
    widthSoFar = jmax(widthSoFar, _topText.getWidth());
    if (_canSetEndpoint)
    {
        heightSoFar = _endpointCaption->getY() + _endpointCaption->getHeight();
        widthSoFar = jmax(widthSoFar, _endpointCaption->getWidth());
        heightSoFar = _endpointEditor->getY() + _endpointEditor->getHeight() + (kButtonGap / 2);
    }
    if (_canSetPort)
    {
        heightSoFar = _portCaption->getY() + _portCaption->getHeight();
        widthSoFar = jmax(widthSoFar, _portCaption->getWidth());
        heightSoFar = _portEditor->getY() + _portEditor->getHeight() + (kButtonGap / 2);
    }
    if (_canSetTag)
    {
        heightSoFar = _tagCaption->getY() + _tagCaption->getHeight();
        widthSoFar = jmax(widthSoFar, _tagCaption->getWidth());
        heightSoFar = _tagEditor->getY() + _tagEditor->getHeight() + (kButtonGap / 2);
    }
    for (size_t ii = 0, numDescriptors = _descriptors.size(), jj = 0; numDescriptors > ii; ++ii)
    {
        Utilities::BaseArgumentDescriptor * aDescriptor = _descriptors[ii];
        
        if (aDescriptor)
        {
            String argName(aDescriptor->argumentName().c_str());
            String argDescription(aDescriptor->argumentDescription().c_str());
            
            if (aDescriptor->isExtra())
            {
                heightSoFar = _extraArgumentsCaption->getY() + _extraArgumentsCaption->getHeight();
                widthSoFar = jmax(widthSoFar, _extraArgumentsCaption->getWidth());
            }
            else
            {
                Label *      aLabel = _standardFieldLabels[static_cast<int>(jj)];
                TextEditor * anEditor = _standardFieldEditors[static_cast<int>(jj)];
                
                heightSoFar = aLabel->getY() + aLabel->getHeight();
                widthSoFar = jmax(widthSoFar, aLabel->getWidth());
                heightSoFar = anEditor->getY() + anEditor->getHeight() + (kButtonGap / 2);
                ++jj;
            }
        }
    }
    for (size_t ii = 0, numExtra = _extraFieldEditors.size(); numExtra > ii; ++ii)
    {
        Label *      aLabel = _extraFieldLabels[static_cast<int>(ii)];
        TextEditor * anEditor = _extraFieldEditors[static_cast<int>(ii)];
        
        aLabel->setTopLeftPosition(2 * kButtonGap, heightSoFar);
        heightSoFar = aLabel->getY() + aLabel->getHeight();
        widthSoFar = jmax(widthSoFar, aLabel->getWidth());
        anEditor->setTopLeftPosition(aLabel->getX(), heightSoFar);
        heightSoFar = anEditor->getY() + anEditor->getHeight() + (kButtonGap / 2);
    }
    heightSoFar += buttonHeight;
    BorderSize<int> bt = getBorderThickness();
    BorderSize<int> cb = getContentComponentBorder();
    int             buttonRowHeight = jmax(_cancelButton.getHeight(), _okButton.getHeight());
    int             titleW = _monoFont.getStringWidth(getName()) + kTitleBarMinWidth;
    int             topW = widthSoFar + kButtonGap;
    int             minW = jmax(topW, jmax(titleW,
                                           _cancelButton.getWidth() + _okButton.getWidth() +
                                           (3 * kButtonGap)));
    int             calcW = minW + bt.getLeftAndRight() + cb.getLeftAndRight();
    int             calcH = heightSoFar + bt.getTopAndBottom() + cb.getTopAndBottom();
    
    setSize(calcW, calcH);
    OD_LOG_EXIT(); //####
} // ApplicationSettingsWindow::recalculateArea

void ApplicationSettingsWindow::removeMostRecentlyAddedExtraField(void)
{
    OD_LOG_ENTER(); //####
    TextEditor * lastEditor = _extraFieldEditors.getLast();
    
    _extraFieldEditors.removeLast();
    Component::removeChildComponent(lastEditor);
    Label * lastLabel = _extraFieldLabels.getLast();
    
    _extraFieldLabels.removeLast();
    Component::removeChildComponent(lastLabel);
    recalculateArea();
    adjustFields();
    if (_removeArgumentsButton)
    {
        _removeArgumentsButton->setVisible(0 < _extraFieldLabels.size());
    }
    OD_LOG_EXIT(); //####
} // ApplicationSettingsWindow::removeMostRecentlyAddedExtraField

void ApplicationSettingsWindow::setUpStandardFields(int & widthSoFar,
                                                    int & heightSoFar)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P2("widthSoFar = ", &widthSoFar, "heightSoFar = ", &heightSoFar); //####
    int        buttonHeight = getLookAndFeel().getAlertWindowButtonHeight();
    Point<int> dimensions;
    size_t     numDescriptors = _descriptors.size();

    widthSoFar = heightSoFar = 0;
    _topText.setFont(_monoFont);
    if ((0 < numDescriptors) || _canSetEndpoint || _canSetPort || _canSetTag)
    {
        _topText.setText(String("The ") + _execType + " has one or more arguments, that need to be "
                         "provided before it can be launched.", dontSendNotification);
    }
    else
    {
        _topText.setText(String("The ") + _execType + " has no arguments or options, so it can be "
                         "launched right now.", dontSendNotification);
    }
    ChannelManager::CalculateTextArea(dimensions, _monoFont, _topText.getText());
    _topText.setBounds(kButtonGap, kButtonGap + getTitleBarHeight(), dimensions.getX() + kButtonGap,
                       dimensions.getY());
    Component::addAndMakeVisible(&_topText, 0);
    heightSoFar = _topText.getY() + _topText.getHeight() + (kButtonGap / 2);
    widthSoFar = jmax(widthSoFar, _topText.getWidth());
    if (_canSetEndpoint)
    {
        _endpointCaption = new Label("", "(Optional) Endpoint to use");
        _endpointCaption->setFont(_monoFont);
        ChannelManager::CalculateTextArea(dimensions, _monoFont, _endpointCaption->getText());
        _endpointCaption->setBounds(2 * kButtonGap, heightSoFar, dimensions.getX(),
                                    dimensions.getY());
        Component::addAndMakeVisible(_endpointCaption);
        heightSoFar = _endpointCaption->getY() + _endpointCaption->getHeight();
        widthSoFar = jmax(widthSoFar, _endpointCaption->getWidth());
        _endpointEditor = new TextEditor(kEndpointFieldName);
        _endpointEditor->setFont(_monoFont);
        _endpointEditor->setBounds(_endpointCaption->getX(), heightSoFar, widthSoFar - kButtonGap,
                                   static_cast<int>(_adjustedEditorHeight));
        _endpointEditor->setSelectAllWhenFocused(true);
        Component::addAndMakeVisible(_endpointEditor);
        heightSoFar = _endpointEditor->getY() + _endpointEditor->getHeight() + (kButtonGap / 2);
    }
    if (_canSetPort)
    {
        _portCaption = new Label("", "(Optional) Network port to use");
        _portCaption->setFont(_monoFont);
        ChannelManager::CalculateTextArea(dimensions, _monoFont, _portCaption->getText());
        _portCaption->setBounds(2 * kButtonGap, heightSoFar, dimensions.getX(), dimensions.getY());
        Component::addAndMakeVisible(_portCaption);
        heightSoFar = _portCaption->getY() + _portCaption->getHeight();
        widthSoFar = jmax(widthSoFar, _portCaption->getWidth());
        _portEditor = new TextEditor(kEndpointFieldName);
        _portEditor->setFont(_monoFont);
        _portEditor->setBounds(_portCaption->getX(), heightSoFar, widthSoFar - kButtonGap,
                               static_cast<int>(_adjustedEditorHeight));
        _portEditor->setSelectAllWhenFocused(true);
        Component::addAndMakeVisible(_portEditor);
        heightSoFar = _portEditor->getY() + _portEditor->getHeight() + (kButtonGap / 2);
    }
    if (_canSetTag)
    {
        _tagCaption = new Label("", String("(Optional) Tag for the ") + _execType);
        _tagCaption->setFont(_monoFont);
        ChannelManager::CalculateTextArea(dimensions, _monoFont, _tagCaption->getText());
        _tagCaption->setBounds(2 * kButtonGap, heightSoFar, dimensions.getX(), dimensions.getY());
        Component::addAndMakeVisible(_tagCaption);
        heightSoFar = _tagCaption->getY() + _tagCaption->getHeight();
        widthSoFar = jmax(widthSoFar, _tagCaption->getWidth());
        _tagEditor = new TextEditor(kEndpointFieldName);
        _tagEditor->setFont(_monoFont);
        _tagEditor->setBounds(_tagCaption->getX(), heightSoFar, widthSoFar - kButtonGap,
                               static_cast<int>(_adjustedEditorHeight));
        _tagEditor->setSelectAllWhenFocused(true);
        Component::addAndMakeVisible(_tagEditor);
        heightSoFar = _tagEditor->getY() + _tagEditor->getHeight() + (kButtonGap / 2);
    }
    for (size_t ii = 0; numDescriptors > ii; ++ii)
    {
        Utilities::BaseArgumentDescriptor * aDescriptor = _descriptors[ii];
        
        if (aDescriptor)
        {
            String argName(aDescriptor->argumentName().c_str());
            String argDescription(aDescriptor->argumentDescription().c_str());
            
            if (aDescriptor->isExtra())
            {
                if (! _canHaveExtraArguments)
                {
                    _canHaveExtraArguments = true;
                    _extraArgRootName = argName;
                    _extraArgumentsCaption = new Label("", argDescription);
                    _extraArgumentsCaption->setFont(_monoFont);
                    ChannelManager::CalculateTextArea(dimensions, _monoFont,
                                                      _extraArgumentsCaption->getText());
                    _extraArgumentsCaption->setBounds(2 * kButtonGap, heightSoFar,
                                                      dimensions.getX(), dimensions.getY());
                    Component::addAndMakeVisible(_extraArgumentsCaption);
                    heightSoFar = _extraArgumentsCaption->getY() +
                                    _extraArgumentsCaption->getHeight() + (kButtonGap / 2);
                    widthSoFar = jmax(widthSoFar, _extraArgumentsCaption->getWidth());
                    _addArgumentsButton = new TextButton(String("+ ") + argName);
                    _addArgumentsButton->setWantsKeyboardFocus(true);
                    _addArgumentsButton->setMouseClickGrabsKeyboardFocus(false);
                    _addArgumentsButton->setCommandToTrigger(nullptr, kSettingsAddField, false);
                    _addArgumentsButton->addListener(this);
                    _addArgumentsButton->changeWidthToFitText(buttonHeight);
                    Component::addAndMakeVisible(_addArgumentsButton, 0);
                    _removeArgumentsButton = new TextButton(String("- ") + argName);
                    _removeArgumentsButton->setWantsKeyboardFocus(true);
                    _removeArgumentsButton->setMouseClickGrabsKeyboardFocus(false);
                    _removeArgumentsButton->setCommandToTrigger(nullptr, kSettingsRemoveField,
                                                                false);
                    _removeArgumentsButton->addListener(this);
                    _removeArgumentsButton->changeWidthToFitText(buttonHeight);
                    Component::addChildComponent(_removeArgumentsButton);
                }
            }
            else
            {
                String descriptionPrefix;
                
                if (aDescriptor->isOptional())
                {
                    descriptionPrefix = "(Optional) ";
                }
                Label * newLabel = new Label("", descriptionPrefix + argDescription);

                _standardFieldLabels.add(newLabel);
                newLabel->setFont(_monoFont);
                ChannelManager::CalculateTextArea(dimensions, _monoFont, newLabel->getText());
                newLabel->setBounds(2 * kButtonGap, heightSoFar, dimensions.getX(),
                                    dimensions.getY());
                Component::addAndMakeVisible(newLabel);
                heightSoFar = newLabel->getY() + newLabel->getHeight();
                widthSoFar = jmax(widthSoFar, newLabel->getWidth());
                TextEditor * newEditor = new TextEditor(argName);
                
                _standardFieldEditors.add(newEditor);
                newEditor->setFont(_monoFont);
                newEditor->setBounds(newLabel->getX(), heightSoFar, widthSoFar - kButtonGap,
                                      static_cast<int>(_adjustedEditorHeight));
                newEditor->setText(aDescriptor->getDefaultValue().c_str(), false);
                newEditor->setComponentID(String(static_cast<int>(ii)));
                newEditor->setSelectAllWhenFocused(true);
                Component::addAndMakeVisible(newEditor);
                heightSoFar = newEditor->getY() + newEditor->getHeight() + (kButtonGap / 2);
            }
        }
    }
    _cancelButton.setWantsKeyboardFocus(true);
    _cancelButton.setMouseClickGrabsKeyboardFocus(false);
    _cancelButton.setCommandToTrigger(nullptr, kSettingsCancel, false);
    _cancelButton.addListener(this);
    _cancelButton.changeWidthToFitText(buttonHeight);
    _cancelButton.setTopLeftPosition(0, heightSoFar + kButtonGap);
    Component::addAndMakeVisible(&_cancelButton, 0);
    _okButton.setWantsKeyboardFocus(true);
    _okButton.setMouseClickGrabsKeyboardFocus(false);
    _okButton.setCommandToTrigger(nullptr, kSettingsAccept, false);
    _okButton.addListener(this);
    _okButton.changeWidthToFitText(buttonHeight);
    _okButton.setTopLeftPosition(0, heightSoFar + kButtonGap);
    Component::addAndMakeVisible(&_okButton, 0);
    heightSoFar += buttonHeight;
    OD_LOG_OBJEXIT(); //####
} // ApplicationSettingsWindow::setUpStandardFields

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)
