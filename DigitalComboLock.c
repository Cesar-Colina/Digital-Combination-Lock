/* 
 * File:   DigitalComboLock.c
 * Author: cesar colina
 *
 * Created on September 21, 2021, 3:22 PM
 */
#define F_CPU 1000000
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>

#define n 6 // input length
uint8_t password[n];
uint8_t setpassword[n] = {2,3,1,1,3,3};

    
void buttonpress(){
    
        uint8_t index = 0;
       
        while(index < n){
            
            //while the button 1 is pressed
            if(!(PINB & (1<<PINB0))){ 
                for(int x = 0; x <= 50; x++) // beep
                    {
                        PORTB |= (1<<PORTB4);
                        _delay_ms(1);
                        PORTB &= ~(1<<PORTB4);
                        _delay_ms(1);
                    }  
                while(!(PINB & (1<<PINB0))); // wait for the user to let go of the button
                    password[index] = 1;
                    index++; 
                    _delay_ms(200); //delay to avoid double inputs
                }
            
            //while the button 2 is pressed
            if(!(PINB & (1<<PINB2))){ 
                for(int x = 0; x <= 50; x++) // beep
                    {
                        PORTB |= (1<<PORTB4);
                        _delay_ms(1);
                        PORTB &= ~(1<<PORTB4);
                        _delay_ms(1);
                    }  
                while(!(PINB & (1<<PINB2)));  // wait for the user 
                    password[index] = 2;
                    index++; 
                    _delay_ms(200); //delay to avoid double inputs
                }
          
            //while the button 3 is pressed
            if(!(PINB & (1<<PINB3))){
                 for(int x = 0; x <= 50; x++) // beep
                    {
                        PORTB |= (1<<PORTB4);
                        _delay_ms(1);
                        PORTB &= ~(1<<PORTB4);
                        _delay_ms(1);
                    }  
                while(!(PINB & (1<<PINB3))); // wait for the user 
                    password[index] = 3;
                    index++; 
                    _delay_ms(200); //delay to avoid double inputs
                } 
        }    
        
       
}    
    
uint8_t checkpassword(){
    // check the password attempt
    for(int i=0; i<n; i++)
    {
        if (password[i] == setpassword[i]) // continue if the current password is equal
        {
 
            continue;
        }
        else if(password[i] != setpassword[i]) // buzz and exit the loop if wrong
        {
            return 0; //wrong
        }     
    }
    return 1; //correct

}

void result(uint8_t output){
    // if the answer is correct then unlock/buzz
    if(output){
        //output a light and give a buzz
        PORTB |= (1<<PORTB1);
        //low buzz
        for(int x = 0; x <= 30; x++)
            {
                PORTB |= (1<<PORTB4);
                _delay_ms(10);
                PORTB &= ~(1<<PORTB4);
                _delay_ms(10);
            } 
        //high buzz
        for(int x = 0; x <= 50; x++)
            {
                PORTB |= (1<<PORTB4);
                _delay_ms(1);
                PORTB &= ~(1<<PORTB4);
                _delay_ms(1);
            }  
      
    }
    
    // if the answer is wrong then give a different buzz
    else if(!output){
        //high buzz
        for(int x = 0; x <= 100; x++)
            {
                PORTB |= (1<<PORTB4);
                _delay_ms(1);
                PORTB &= ~(1<<PORTB4);
                _delay_ms(1);
            }  
        //low buzz
        for(int x = 0; x <= 30; x++)
            {
                PORTB |= (1<<PORTB4);
                _delay_ms(10);
                PORTB &= ~(1<<PORTB4);
                _delay_ms(10);
            }  
    }
}

void reset(uint8_t output){
    //if the output is correct
    if (output){
        // wait for an input
        while(1){
            // if any of the buttons are pressed
            if(!(PINB & (1<<PINB0)) || !(PINB & (1<<PINB2)) || !(PINB & (1<<PINB3)))
            {
                 for(int x = 0; x <= 50; x++) // beep
                    {
                        PORTB |= (1<<PORTB4);
                        _delay_ms(1);
                        PORTB &= ~(1<<PORTB4);
                        _delay_ms(1);
                    }  
                
                while(!(PINB & (1<<PINB0)) || !(PINB & (1<<PINB2)) || !(PINB & (1<<PINB3))); //wait for the user to let go
                PORTB &= ~(1<<PORTB1);//turn the light off to signal it is now locked
                break;
            }
        }
        
    }
    // if the output is incorrect then do nothing
}

int main(int argc, char** argv) {
    
   
    // initialization and setting pull ups
    DDRB |= (1<<DDB1); // LED 
    DDRB &= ~(1<<DDB0); // switch input button 1
    DDRB &= ~(1<<DDB2); // switch input button 2
    DDRB &= ~(1<<DDB3); // switch input button 3
    DDRB |= (1<<DDB4); // buzzer as output
    PORTB |= (1<<PORTB0); // turns on the pull up
    PORTB |= (1<<PORTB2); // turns on the pull up
    PORTB |= (1<<PORTB3); // turns on the pull up
   
    
    while(1){
        buttonpress(); // input the password attempt
        uint8_t answer = checkpassword(); // checks to see if the input is correct
        result(answer);// execute the results of the password attempt
        reset(answer); //re-lock the device if opened 
    }
    
    
    
    
    
    
   
    return (EXIT_SUCCESS);
}
