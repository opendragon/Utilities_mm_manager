//--------------------------------------------------------------------------------------------------
//
//  File:       m+mSettingsWindow.cpp
//
//  Project:    m+m
//
//  Contains:   The class definition for the application settings window of the m+m manager
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

#include "m+mSettingsWindow.h"

#include "m+mManagerApplication.h"
#include "m+mTextValidator.h"
#include "m+mValidatingTextEditor.h"

#include <m+m/m+mChannelArgumentDescriptor.h>
#include <m+m/m+mEndpoint.h>
#include <m+m/m+mPortArgumentDescriptor.h>

//#include <odl/ODEnableLogging.h>
#include <odl/ODLogging.h>

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunknown-pragmas"
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 
 @brief The class definition for the application settings window of the m+m manager application. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Namespace references
#endif // defined(__APPLE__)

using namespace MplusM;
using namespace MPlusM_Manager;
using namespace std;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

/*! @brief The colour to be used for the window background. */
static const Colour & kWindowBackgroundColour(Colours::whitesmoke);

/*! @brief The extra space around the content in the window. */
static const int kExtraSpaceInWindow = 20;

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

SettingsWindow::SettingsWindow(const String &          title,
                               const String &          execType,
                               const ApplicationInfo & appInfo,
                               String &                endpointToUse,
                               String &                tagToUse,
                               String &                portToUse,
                               StringArray &           argsToUse)  :
    inherited1(), inherited2(), inherited3(title, kWindowBackgroundColour, 0), inherited4(),
    _topText("topText"), _cancelButton("Cancel"), _okButton("OK"),
    _descriptors(appInfo._argDescriptions),
    _errorFont(Font::getDefaultMonospacedFontName(), FormField::kFontSize,
               Font::italic + Font::bold), _regularFont(Font::getDefaultMonospacedFontName(),
                                                        FormField::kFontSize, Font::plain),
    _execType(execType), _extraArgumentsGroup(NULL), _addArgumentsButton(NULL),
    _removeArgumentsButton(NULL), _endpointField(NULL), _portField(NULL), _tagField(NULL),
    _endpointDescriptor(new Utilities::ChannelArgumentDescriptor(kEndpointFieldName.toStdString(),
                                                                 "", Utilities::kArgModeOptional,
                                                                 "")),
    _portDescriptor(new Utilities::PortArgumentDescriptor(kPortFieldName.toStdString(), "",
                                                          Utilities::kArgModeOptional, 0, false)),
    _appInfo(appInfo), _endpointToUse(endpointToUse), _portToUse(portToUse), _tagToUse(tagToUse),
    _argsToUse(argsToUse), _canSetEndpoint(false), _canSetPort(false), _canSetTag(false),
    _hasExtraArguments(false), _hasFileField(false)
{
    OD_LOG_ENTER(); //####
    OD_LOG_S2s("title = ", title, "execType = ", execType); //####
    OD_LOG_P4("appInfo = ", &appInfo, "endpointToUse = ", &endpointToUse, "tagToUse = ", //####
              &tagToUse, "portToUse = ", &portToUse); //####
    OD_LOG_P1("argsToUse = ", &argsToUse); //####
    _contentArea.setSize(100, 100);
    setContentNonOwned(&_contentArea, true);
    BorderSize<int> bt = getBorderThickness();
    BorderSize<int> cb = getContentComponentBorder();
    int             heightSoFar = 0;
    int             widthSoFar = 0;
    
    _argsToUse.clear();
    _canSetEndpoint = appInfo._options.contains("e");
    _canSetPort = appInfo._options.contains("p");
    _canSetTag = appInfo._options.contains("t");
    setUpStandardFields(widthSoFar, heightSoFar);
    int minW = jmax(widthSoFar, _cancelButton.getWidth() + _okButton.getWidth() +
                    (3 * FormField::kButtonGap));
    int calcW = minW + bt.getLeftAndRight() + cb.getLeftAndRight();
    int calcH = heightSoFar + bt.getTopAndBottom() + cb.getTopAndBottom();
    
    centreWithSize(calcW + kExtraSpaceInWindow, calcH + kExtraSpaceInWindow);
    adjustFields();
    setOpaque(true);
    setResizable(false, false);
    setVisible(true);
    addKeyListener(ManagerWindow::getApplicationCommandManager().getKeyMappings());
    triggerAsyncUpdate();
    OD_LOG_EXIT_P(this); //####
} // SettingsWindow::SettingsWindow

SettingsWindow::~SettingsWindow(void)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT(); //####
} // SettingsWindow::~SettingsWindow

#if defined(__APPLE__)
# pragma mark Actions and Accessors
#endif // defined(__APPLE__)

void SettingsWindow::addAnExtraField(void)
{
    OD_LOG_ENTER(); //####
    String               compCountAsString(static_cast<int>(_extraFields.size() + 1));
    CaptionedTextField * newField = new CaptionedTextField(*this, _regularFont, _errorFont,
                                                           _extraFields.size(),
                                                           _extraArgRootName + " " +
                                                           compCountAsString, 0, true, false, NULL,
                                                           NULL, _extraArgRootName + "_" +
                                                           compCountAsString);
    
    _extraFields.add(newField);
    newField->addToComponent(_extraArgumentsGroup);
    recalculateArea();
    adjustFields();
    if (_removeArgumentsButton)
    {
        _removeArgumentsButton->setVisible(true);
    }
    OD_LOG_EXIT(); //####
} // SettingsWindow::addAnExtraField

void SettingsWindow::adjustFields(void)
{
    OD_LOG_OBJENTER(); //####
    Component * content = getContentComponent();
    int         newButtonTop = content->getHeight() - (_cancelButton.getHeight() +
                                                       FormField::kButtonGap);
    int         newFieldWidth = content->getWidth() - (2 * FormField::kFieldInset);
    
    if (_hasFileField)
    {
        newFieldWidth -= (FormField::kButtonGap + CaptionedTextField::getFileButtonWidth());
    }
    _cancelButton.setTopLeftPosition(getWidth() - (_cancelButton.getWidth() +
                                                   FormField::kButtonGap), newButtonTop);
    _okButton.setTopLeftPosition(_cancelButton.getX() - (_okButton.getWidth() +
                                                         FormField::kButtonGap), newButtonTop);
    if (_endpointField)
    {
        _endpointField->setWidth(newFieldWidth);
    }
    if (_portField)
    {
        _portField->setWidth(newFieldWidth);
    }
    if (_tagField)
    {
        _tagField->setWidth(newFieldWidth);
    }
    if (_addArgumentsButton)
    {
        _addArgumentsButton->setTopLeftPosition(_okButton.getX() -
                                                (_addArgumentsButton->getWidth() +
                                                 FormField::kButtonGap), newButtonTop);
    }
    if (_removeArgumentsButton)
    {
        _removeArgumentsButton->setTopLeftPosition(_addArgumentsButton->getX() -
                                                   (_removeArgumentsButton->getWidth() +
                                                    FormField::kButtonGap), newButtonTop);
    }
    for (size_t ii = 0, maxf = _standardFields.size(); maxf > ii; ++ii)
    {
        FormField * aField = _standardFields[static_cast<int>(ii)];
        
        if (aField)
        {
            aField->setWidth(newFieldWidth);
        }
    }
    if (_extraArgumentsGroup)
    {
        int groupWidth = (_cancelButton.getX() + _cancelButton.getWidth() -
                          (_extraArgumentsGroup->getX() + FormField::kButtonGap));
        int innerWidth = groupWidth - (FormField::kFieldInset + (2 * FormField::kButtonGap));
        
        for (size_t ii = 0, maxf = _extraFields.size(); maxf > ii; ++ii)
        {
            FormField * aField = _extraFields[static_cast<int>(ii)];
            
            aField->setWidth(innerWidth);
        }
        _extraArgumentsGroup->setSize(groupWidth, _extraArgumentsGroup->getHeight());
    }
    OD_LOG_OBJEXIT(); //####
} // SettingsWindow::adjustFields

void SettingsWindow::buttonClicked(Button * aButton)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("aButton = ", aButton); //####
    int commandId = aButton->getCommandID();
    
    tellAllFieldsToIgnoreNextFocusLoss();
    switch (commandId)
    {
        case kConfigurationAddField :
            addAnExtraField();
            break;
            
        case kConfigurationRemoveField :
            removeMostRecentlyAddedExtraField();
            break;
            
        case kConfigurationOK :
            if (fieldsAreValid())
            {
                exitModalState(commandId);
            }
            break;
            
        case kConfigurationFileRequest :
            for (size_t ii = 0, maxf = _standardFields.size(); maxf > ii; ++ii)
            {
                FormField * aField = _standardFields[static_cast<int>(ii)];
                
                if (aField && (aField->getButton() == aButton))
                {
                    aField->performButtonAction();
                    break;
                }
                
            }
            break;
            
        default :
            exitModalState(commandId);
            break;
            
    }
    OD_LOG_OBJEXIT(); //####
} // SettingsWindow::buttonClicked

bool SettingsWindow::fieldsAreValid(void)
{
    OD_LOG_ENTER(); //####
    int    badCount = 0;
    String badArgs;
    String primaryChannel;
    
    // Counterintuitively, we check the values from the descriptors first, before checking the
    // endpoint, port or tag values.
    _argsToUse.clear();
    for (size_t ii = 0, maxf = _standardFields.size(); maxf > ii; ++ii)
    {
        FormField * aField = _standardFields[static_cast<int>(ii)];
        
        if (aField && (! aField->validateField(_argsToUse)))
        {
            if (0 < badArgs.length())
            {
                badArgs += "\n";
            }
            badArgs += aField->getName();
            ++badCount;
        }
    }
    if (0 == badCount)
    {
        // Add the extra arguments here.
        for (size_t ii = 0, maxf = _extraFields.size(); maxf > ii; ++ii)
        {
            FormField * aField = _extraFields[static_cast<int>(ii)];
            
            _argsToUse.add(aField->getText());
        }
    }
    if (_canSetEndpoint)
    {
        if (_endpointField->validateField())
        {
            _endpointToUse = _endpointField->getText();
        }
        else
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
        if (_portField->validateField())
        {
            _portToUse = _portField->getText();
        }
        else
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
        _tagToUse = _tagField->getText();
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
        ManagerApplication * ourApp = ManagerApplication::getApp();
        
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
} // SettingsWindow::fieldsAreValid

#if (! MAC_OR_LINUX_)
# pragma warning(push)
# pragma warning(disable: 4100)
#endif // ! MAC_OR_LINUX_
void SettingsWindow::focusGained(FocusChangeType cause)
{
#if MAC_OR_LINUX_
# pragma unused(cause)
#endif // MAC_OR_LINUX_
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT(); //####
} // SettingsWindow::focusGained
#if (! MAC_OR_LINUX_)
# pragma warning(pop)
#endif // ! MAC_OR_LINUX_

#if (! MAC_OR_LINUX_)
# pragma warning(push)
# pragma warning(disable: 4100)
#endif // ! MAC_OR_LINUX_
void SettingsWindow::focusLost(FocusChangeType cause)
{
#if MAC_OR_LINUX_
# pragma unused(cause)
#endif // MAC_OR_LINUX_
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT(); //####
} // SettingsWindow::focusLost
#if (! MAC_OR_LINUX_)
# pragma warning(pop)
#endif // ! MAC_OR_LINUX_

void SettingsWindow::handleAsyncUpdate(void)
{
    OD_LOG_OBJENTER(); //####
    ApplicationCommandManager & commandManager = ManagerWindow::getApplicationCommandManager();
    
    commandManager.registerAllCommandsForTarget(JUCEApplication::getInstance());
    OD_LOG_OBJEXIT(); //####
} // SettingsWindow::handleAsyncUpdate

bool SettingsWindow::keyPressed(const KeyPress & key)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("key = ", &key); //####
    bool result;
    
    if (key == KeyPress::escapeKey)
    {
        tellAllFieldsToIgnoreNextFocusLoss();
        exitModalState(kConfigurationCancel);
        result = true;
    }
    else if (key == KeyPress::returnKey)
    {
        tellAllFieldsToIgnoreNextFocusLoss();
        if (fieldsAreValid())
        {
            exitModalState(kConfigurationOK);
        }
        result = true;
    }
    else
    {
        result = inherited3::keyPressed(key);
    }
    OD_LOG_OBJEXIT_B(result); //####
    return result;
} // SettingsWindow::keyPressed

void SettingsWindow::recalculateArea(void)
{
    OD_LOG_ENTER(); //####
    int    heightSoFar = _topText.getY() + _topText.getHeight() + FormField::kButtonGap;
    int    widthSoFar = _topText.getX() + _topText.getWidth();
    size_t numExtra = _extraFields.size();
    
    if (_canSetEndpoint)
    {
        widthSoFar = jmax(widthSoFar, _endpointField->getMinimumWidth());
        heightSoFar = (_endpointField->getY() + _endpointField->getHeight() +
                       (FormField::kButtonGap / 2));
    }
    if (_canSetPort)
    {
        widthSoFar = jmax(widthSoFar, _portField->getMinimumWidth());
        heightSoFar = _portField->getY() + _portField->getHeight() + (FormField::kButtonGap / 2);
    }
    if (_canSetTag)
    {
        widthSoFar = jmax(widthSoFar, _tagField->getMinimumWidth());
        heightSoFar = _tagField->getY() + _tagField->getHeight() + (FormField::kButtonGap / 2);
    }
    for (size_t ii = 0, numDescriptors = _descriptors.size(), jj = 0; numDescriptors > ii; ++ii)
    {
        Utilities::BaseArgumentDescriptor * aDescriptor = _descriptors[ii];
        
        if (aDescriptor)
        {
            String argName(aDescriptor->argumentName().c_str());
            String argDescription(aDescriptor->argumentDescription().c_str());
            
            if (! aDescriptor->isExtra())
            {
                FormField * aField = _standardFields[static_cast<int>(jj)];
                
                if (aField)
                {
                    widthSoFar = jmax(widthSoFar, aField->getMinimumWidth());
                    heightSoFar = (aField->getY() + aField->getHeight() +
                                   (FormField::kButtonGap / 2));
                    ++jj;
                }
            }
        }
    }
    if (_extraArgumentsGroup)
    {
        int innerHeight = static_cast<int>(_regularFont.getHeight()) + (FormField::kButtonGap / 2);
        int innerWidth = (2 * jmax(FormField::kFieldInset, FormField::kLabelInset));
        
        _extraArgumentsGroup->setTopLeftPosition(FormField::kFieldInset, heightSoFar);
        for (size_t ii = 0; numExtra > ii; ++ii)
        {
            FormField * aField = _extraFields[static_cast<int>(ii)];
            
            aField->setY(innerHeight);
            innerHeight = aField->getY() + aField->getHeight() + (FormField::kButtonGap / 2);
        }
        if (0 < numExtra)
        {
            innerHeight += (3 * FormField::kButtonGap / 4);
        }
        else
        {
            innerHeight += (FormField::kButtonGap / 2);
        }
        _extraArgumentsGroup->setSize(innerWidth, innerHeight);
        heightSoFar = _extraArgumentsGroup->getY() + _extraArgumentsGroup->getHeight();
        widthSoFar = jmax(widthSoFar, _extraArgumentsGroup->getX() +
                          _extraArgumentsGroup->getWidth());
    }
    BorderSize<int> cb = getContentComponentBorder();
    int             minW = jmax(widthSoFar, _cancelButton.getWidth() + _okButton.getWidth() +
                                (3 * FormField::kButtonGap));
    
    setContentComponentSize(minW + kExtraSpaceInWindow + cb.getLeftAndRight(),
                            heightSoFar + kExtraSpaceInWindow + cb.getTopAndBottom());
    OD_LOG_EXIT(); //####
} // SettingsWindow::recalculateArea

void SettingsWindow::removeMostRecentlyAddedExtraField(void)
{
    OD_LOG_ENTER(); //####
    FormField * lastField = _extraFields.getLast();
    
    _extraFields.removeLast();
    lastField->removeFromComponent(_extraArgumentsGroup);
    recalculateArea();
    adjustFields();
    if (_removeArgumentsButton)
    {
        _removeArgumentsButton->setVisible(0 < _extraFields.size());
    }
    OD_LOG_EXIT(); //####
} // SettingsWindow::removeMostRecentlyAddedExtraField

void SettingsWindow::reportErrorInField(FormField & fieldOfInterest)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("fieldOfInterest = ", &fieldOfInterest); //####
    String nameToDisplay;
    
    if (&fieldOfInterest == _endpointField)
    {
        nameToDisplay = "Endpoint";
    }
    else if (&fieldOfInterest == _portField)
    {
        nameToDisplay = "Port";
    }
    else
    {
        nameToDisplay = fieldOfInterest.getName();
    }
    AlertWindow::showMessageBox(AlertWindow::WarningIcon, getName(),
                                String("The ") + nameToDisplay + " argument is invalid.\n"
                                "Please correct the argument and try again.", String::empty, this);
    OD_LOG_OBJEXIT(); //####
} // SettingsWindow::reportErrorInField

void SettingsWindow::reportErrorInField(ValidatingTextEditor & fieldOfInterest)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("fieldOfInterest = ", &fieldOfInterest); //####
    AlertWindow::showMessageBox(AlertWindow::WarningIcon, getName(),
                                String("The ") + fieldOfInterest.getName() +
                                " argument is invalid.\n"
                                "Please correct the argument and try again.", String::empty, this);
    OD_LOG_OBJEXIT(); //####
} // SettingsWindow::reportErrorInField

void SettingsWindow::resized(void)
{
    OD_LOG_OBJENTER(); //####
    Button * close = getCloseButton();
    
    inherited3::resized();
    if (close)
    {
        const KeyPress esc(KeyPress::escapeKey, 0, 0);
        
        if (! close->isRegisteredForShortcut(esc))
        {
            close->addShortcut(esc);
        }
    }
    OD_LOG_OBJEXIT(); //####
} // SettingsWindow::resized

void SettingsWindow::setUpStandardFields(int & widthSoFar,
                                         int & heightSoFar)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P2("widthSoFar = ", &widthSoFar, "heightSoFar = ", &heightSoFar); //####
    Component * content = getContentComponent();
    int         buttonHeight = ManagerApplication::getButtonHeight();
    Point<int>  dimensions;
    size_t      numDescriptors = _descriptors.size();

    widthSoFar = heightSoFar = 0;
    _topText.setFont(_regularFont);
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
    MPlusM_Manager::CalculateTextArea(dimensions, _regularFont, _topText.getText());
    _topText.setBounds(FormField::kButtonGap, FormField::kButtonGap + getTitleBarHeight(),
                       dimensions.getX() + FormField::kButtonGap, dimensions.getY());
    content->addAndMakeVisible(&_topText, 0);
    heightSoFar = _topText.getY() + _topText.getHeight() + FormField::kButtonGap;
    widthSoFar = jmax(widthSoFar, _topText.getX() + _topText.getWidth());
    if (_canSetEndpoint)
    {
        _endpointField = new CaptionedTextField(*this, _regularFont, _errorFont, 0,
                                                "(Optional) Endpoint to use", heightSoFar, false,
                                                false, NULL,
                                                new TextValidator(*_endpointDescriptor),
                                                kEndpointFieldName);
        
        _endpointField->addToComponent(content);
        widthSoFar = jmax(widthSoFar, _endpointField->getMinimumWidth());
        heightSoFar = (_endpointField->getY() + _endpointField->getHeight() +
                       (FormField::kButtonGap / 2));
    }
    if (_canSetPort)
    {
        _portField = new CaptionedTextField(*this, _regularFont, _errorFont, 0,
                                            "(Optional) Network port to use", heightSoFar, false,
                                            false, NULL, new TextValidator(*_portDescriptor),
                                            kPortFieldName);
        
        _portField->addToComponent(content);
        widthSoFar = jmax(widthSoFar, _portField->getMinimumWidth());
        heightSoFar = _portField->getY() + _portField->getHeight() + (FormField::kButtonGap / 2);
    }
    if (_canSetTag)
    {
        _tagField = new CaptionedTextField(*this, _regularFont, _errorFont, 0,
                                           String("(Optional) Tag for the ") + _execType,
                                           heightSoFar, false, false, NULL, NULL, kTagFieldName);
        
        _tagField->addToComponent(content);
        widthSoFar = jmax(widthSoFar, _tagField->getMinimumWidth());
        heightSoFar = _tagField->getY() + _tagField->getHeight() + (FormField::kButtonGap / 2);
    }
    // Check for one or more file descriptors.
    for (size_t ii = 0; numDescriptors > ii; ++ii)
    {
        bool                                forOutput;
        Utilities::BaseArgumentDescriptor * aDescriptor = _descriptors[ii];
        
        if (aDescriptor && aDescriptor->isForFiles(forOutput))
        {
            _hasFileField = true;
            break;
        }
        
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
                if (! _hasExtraArguments)
                {
                    _hasExtraArguments = true;
                    _extraArgRootName = argName;
                    _extraArgumentsGroup = new GroupComponent("", argDescription);
                    _extraArgumentsGroup->setBounds(FormField::kFieldInset, heightSoFar,
                                                    widthSoFar - (FormField::kButtonGap +
                                                                  FormField::kFieldInset),
                                                    static_cast<int>(_regularFont.getHeight()) +
                                                    FormField::kButtonGap);
                    content->addAndMakeVisible(_extraArgumentsGroup);
                    heightSoFar = (_extraArgumentsGroup->getY() + _extraArgumentsGroup->getHeight() +
                                   (FormField::kButtonGap / 2));
                    widthSoFar = jmax(widthSoFar, _extraArgumentsGroup->getX() +
                                      _extraArgumentsGroup->getWidth());
                    _addArgumentsButton = new TextButton(String("+ ") + argName);
                    _addArgumentsButton->setWantsKeyboardFocus(true);
                    _addArgumentsButton->setMouseClickGrabsKeyboardFocus(false);
                    _addArgumentsButton->setCommandToTrigger(NULL, kConfigurationAddField, false);
                    _addArgumentsButton->addListener(this);
                    _addArgumentsButton->changeWidthToFitText(buttonHeight);
                    content->addAndMakeVisible(_addArgumentsButton, 0);
                    _removeArgumentsButton = new TextButton(String("- ") + argName);
                    _removeArgumentsButton->setWantsKeyboardFocus(true);
                    _removeArgumentsButton->setMouseClickGrabsKeyboardFocus(false);
                    _removeArgumentsButton->setCommandToTrigger(NULL, kConfigurationRemoveField,
                                                                false);
                    _removeArgumentsButton->addListener(this);
                    _removeArgumentsButton->changeWidthToFitText(buttonHeight);
                    content->addChildComponent(_removeArgumentsButton);
                }
            }
            else
            {
                bool forFilePath;
                bool forOutput;
                
                if (aDescriptor->isForFiles(forOutput))
                {
                    forFilePath = true;
                }
                else
                {
                    forFilePath = false;
                }
                String               descriptionPrefix(aDescriptor->isOptional() ? "(Optional) " :
                                                       "");
                CaptionedTextField * newField = new CaptionedTextField(*this, _regularFont,
                                                                       _errorFont, ii,
                                                                       descriptionPrefix +
                                                                       argDescription, heightSoFar,
                                                                       false, forFilePath, this,
                                                                   new TextValidator(*aDescriptor),
                                                                       argName,
                                                                       aDescriptor->isPassword() ?
                                                                       CHAR_TO_USE_FOR_PASSWORD_ :
                                                                       0);
                
                newField->setText(aDescriptor->getDefaultValue().c_str(), false);
                _standardFields.add(newField);
                newField->addToComponent(content);
                widthSoFar = jmax(widthSoFar, newField->getMinimumWidth());
                heightSoFar = (newField->getY() + newField->getHeight() +
                               (FormField::kButtonGap / 2));
            }
        }
    }
    _cancelButton.setWantsKeyboardFocus(true);
    _cancelButton.setMouseClickGrabsKeyboardFocus(false);
    _cancelButton.setCommandToTrigger(NULL, kConfigurationCancel, false);
    _cancelButton.addListener(this);
    _cancelButton.changeWidthToFitText(buttonHeight);
    _cancelButton.setTopLeftPosition(0, heightSoFar + FormField::kButtonGap);
    content->addAndMakeVisible(&_cancelButton, 0);
    _okButton.setWantsKeyboardFocus(true);
    _okButton.setMouseClickGrabsKeyboardFocus(false);
    _okButton.setCommandToTrigger(NULL, kConfigurationOK, false);
    _okButton.addListener(this);
    _okButton.changeWidthToFitText(buttonHeight);
    _okButton.setTopLeftPosition(0, heightSoFar + FormField::kButtonGap);
    content->addAndMakeVisible(&_okButton, 0);
    heightSoFar += buttonHeight;
    OD_LOG_OBJEXIT(); //####
} // SettingsWindow::setUpStandardFields

void SettingsWindow::tellAllFieldsToIgnoreNextFocusLoss(void)
{
    OD_LOG_ENTER(); //####
    if (_endpointField)
    {
        _endpointField->ignoreNextFocusLoss();
    }
    if (_portField)
    {
        _portField->ignoreNextFocusLoss();
    }
    if (_tagField)
    {
        _tagField->ignoreNextFocusLoss();
    }
    for (size_t ii = 0, maxf = _standardFields.size(); maxf > ii; ++ii)
    {
        FormField * aField = _standardFields[static_cast<int>(ii)];
        
        if (aField)
        {
            aField->ignoreNextFocusLoss();
        }
    }
    for (size_t ii = 0, maxf = _extraFields.size(); maxf > ii; ++ii)
    {
        FormField * aField = _extraFields[static_cast<int>(ii)];
        
        if (aField)
        {
            aField->ignoreNextFocusLoss();
        }
    }
    OD_LOG_EXIT(); //####
} // SettingsWindow::tellAllFieldsToIgnoreNextFocusLoss

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)
