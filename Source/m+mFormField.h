//--------------------------------------------------------------------------------------------------
//
//  File:       m+mFormField.h
//
//  Project:    m+m
//
//  Contains:   The class declaration for a generalized input field.
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
//  Created:    2015-08-31
//
//--------------------------------------------------------------------------------------------------

#if (! defined(mpmFormField_H_))
# define mpmFormField_H_ /* Header guard */

# include "m+mManagerDataTypes.h"

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wunknown-pragmas"
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file
 
 @brief The class declaration for a generalized input field. */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

namespace MPlusM_Manager
{
    class FormFieldErrorResponder;
    
    /*! @brief A generalized input field. */
    class FormField
    {
    public :
        
        /*! @brief The constructor.
         @param responder The entity that will report errors in this field.
         @param regularLabelFont The font to use with the label when the text editor data is valid.
         @param errorLabelFont The font to use with the label when the text editor data is invalid.
         @param index The order of the text editor. */
        FormField(FormFieldErrorResponder & responder,
                  Font &                    regularLabelFont,
                  Font &                    errorLabelFont,
                  const size_t              index);
        
        /*! @brief The destructor. */
        virtual ~FormField(void);
        
        /*! @brief Return the associated button.
         @returns The associated button. */
        inline TextButton * getButton(void)
        const
        {
            return _button;
        } // getButton
        
        /*! @brief Return the associated caption.
         @returns The associated caption. */
        inline Label * getCaption(void)
        const
        {
            return _caption;
        } // getCaption
        
        /*! @brief Return the component associated with the field.
         @return The component associated with the field. */
        virtual Component * getComponent(void)
        const = 0;

        /*! @brief Return the height of the field in pixels.
         @return The height of the field in pixels. */
        virtual int getHeight(void)
        const = 0;
        
        /*! @brief Return the order of the text editor.
         @returns The order of the text editor. */
        inline size_t getIndex(void)
        const
        {
            return _index;
        } // getIndex
        
        /*! @brief Returns the name of the field.
         @returns The name of the field. */
        virtual const String & getName(void)
        const = 0;
        
        /*! @brief Returns the text value associated with the field.
         @returns The text value associated with the field. */
        virtual String getText(void)
        const = 0;

        /*! @brief Return the left coordinate of the field.
         @return The left coordinate of the field. */
        virtual int getX(void)
        const = 0;
        
        /*! @brief Return the top coordinate of the field.
         @return The top coordinate of the field. */
        virtual int getY(void)
        const = 0;
        
        /*! @brief Do not perform validation on next loss of focus. */
        virtual void ignoreNextFocusLoss(void) = 0;
        
        /*! @brief Perform the action triggered by the button. */
        virtual void performButtonAction(void);

        /*! @brief Set the position and size of the field.
         @param xx The leftmost coordinate of the field.
         @param yy The topmost coordinate of the field.
         @param width The width of the field.
         @param height The height of the field. */
        virtual void setBounds(const int xx,
                               const int yy,
                               const int width,
                               const int height) = 0;
        
        /*! @brief Sets the associated button.
         @param newButton The associated button. */
        void setButton(TextButton * newButton = NULL);
        
        /*! @brief Set the action on receiving focus.
         @param shouldSelectAll If @c true, obtaining focus will select all the content. */
        virtual void setSelectAllWhenFocused(const bool shouldSelectAll) = 0;

        /*! @brief Change the size of the field.
         @param newWidth The new width of the field.
         @param newHeight The new height of the field. */
        virtual void setSize(const int newWidth,
                             const int newHeight) = 0;
        
        /*! @brief Set the text value associated with the field.
         @param newText The text to be used.
         @param sendTextChangeMessage @c true if a change message is sent to all listeners. */
        virtual void setText(const String & newText,
                             const bool     sendTextChangeMessage) = 0;

        /*! @brief Set the position of the field.
         @param xx The new leftmost coordinate of the field.
         @param yy The new topmost coordinate of the field. */
        virtual void setTopLeftPosition(const int xx,
                                        const int yy) = 0;

        /*! @brief Check the field for validity.
         @returns @c true if the validator accepts the field or there's no validation required or
         @c false if the validator rejects the field. */
        virtual bool validateField(void) = 0;
        
        /*! @brief Check the field for validity.
         @param argsToUse A set of valid arguments.
         @returns @c true if the validator accepts the field or there's no validation required or
         @c false if the validator rejects the field. */
        virtual bool validateField(StringArray & argsToUse) = 0;
        
    protected :
        
    private :
        
    public :
    
    protected :
    
        /*! @brief The font to use with the label when the text editor data is invalid. */
        Font & _errorFont;
        
        /*! @brief The font to use with the label when the text editor data is valid. */
        Font & _regularFont;
        
        /*! @brief The entity that can report an error in this field. */
        FormFieldErrorResponder & _responder;
        
        /*! @brief An associated button for the text editor. */
        ScopedPointer<TextButton> _button;
        
        /*! @brief The caption for the field. */
        ScopedPointer<Label> _caption;
        
        /*! @brief The order of the field. */
        size_t _index;
        
    private :

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FormField)
        
    }; // FormField
    
} // MPlusM_Manager

#endif // ! defined(mpmFormField_H_)
