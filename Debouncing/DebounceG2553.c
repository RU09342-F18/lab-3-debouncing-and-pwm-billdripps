#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;//stop watchdog timer
    BCSCTL3 = LFXT1S_2;//use internal crystal
    P1SEL = 0; //port 1 is i/o
    P1DIR |= BIT0; //set p1.0 to output (to LED)
    P1DIR &= ~BIT3; //set p1.3 to input (from switch)
    P1IE |= BIT3; //enable interrupts on p1.3
    P1IES |= BIT3; //set interrupt to flag on a falling edge
    //P1REN |= BIT3; //enable pull resistor (apparently unnecessary)

    //initialize count up to value
    TACCR0 = 0x0041; //2ms delay time

    __bis_SR_register(GIE); //enable interrupts

    while(1);


	return 0;
}

//interrupt service routine
#pragma vector = PORT1_VECTOR
__interrupt void PORT1_vector(void)//
{
     //check button press and release triggers
     if(P1IES & BIT3) //falling edge trigger (button press)
     {
         //disable interrupts and activate timer
         P1IE &= ~BIT3;//disables interrupts on port 1.3
         TA0CTL = TASSEL_1|MC_1|TACLR; //sets up the timer to the 32 kHz clock in up mode

         while(TA0R != TACCR0);//counts 2ms

         TA0CTL = MC_0|TACLR; //turns the timer off
         P1OUT |= BIT0;//turns on LED
         P1IE |= BIT3;//enables interrupts on port 1.3
         P1IES &= ~BIT3; //set interrupt to flag on a rising edge
     }

     else // rising edge trigger (button release)
     {
         //disable interrupts and activate timer
         P1IE &= ~BIT3;//disables interrupts on port 1.3
         TA0CTL = TASSEL_1|MC_1|TACLR; //sets up the timer to the 32 kHz clock in up mode

         while(TA0R != TACCR0);//counts 2ms

         TA0CTL = MC_0|TACLR; //turns the timer off
         P1OUT &= ~BIT0;//turns of LED
         P1IE |= BIT3;//enables interrupts on port 1.3
         P1IES |= BIT3; //set interrupt to flag on a rising edge
     }
     P1IFG &= ~BIT3; //turn off the flag
}
