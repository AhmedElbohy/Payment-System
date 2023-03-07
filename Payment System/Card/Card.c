#include <stdio.h>
#include <string.h>
#include "card.h"

//Implementation of getCardHolderName function

EN_cardError_t getCardHolderName(ST_cardData_t *cardData)
{
    uint8_t Name[50]={0};
    
    //get the name of card holder
    printf("please enter card holder name (20-24 characters): ");
    gets(Name);
    
    uint8_t NameLength = strlen(Name);

    if((Name != NULL) && (NameLength>=20) && (NameLength<= 24))
    {
        //correct input
        strcpy(cardData->cardHolderName,Name);
        return CARD_OK;
    }
    else
    {
        //wrong input
        return WRONG_NAME;
    }
    printf("\n");
}

//Implementation of getCardExpiryDate function

EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData)
{
    uint8_t Date[50];
    
    //get expiry date of card
    printf("please enter card expiry date(MM/YY): ");
    gets(Date);
    
    uint8_t DateLength = strlen(Date);

    uint8_t MM,YY;

    // month from 01 to 12
    MM = ((Date[0] - '0') * 10) + (Date[1] - '0');

    // year from 00 to 99
    YY = ((Date[3] - '0') * 10) + (Date[4] - '0');

    if(Date!= NULL && DateLength==5 && Date[2] == '/' && ( MM >0 && MM <=12 ) &&  ( YY >= 0 && YY <= 99 ))
    {
        //correct input
        strcpy(cardData->cardExpirationDate,Date);
        return CARD_OK;
    }
    else
    {
        //wrong input
        return WRONG_EXP_DATE;
    }
    printf("\n");
}

//Implementation of getCardPAN function

EN_cardError_t getCardPAN(ST_cardData_t *cardData)
{
    uint8_t PAN[50];
    
    //get the name of card holder
    printf("please enter Primary Account Number (16-19 Numbers): ");
    gets(PAN);
    
    uint8_t PANLength = strlen(PAN);

    if(PAN!= NULL && PANLength>=16 && PANLength<= 19)
    {
        //correct input
        strcpy(cardData->primaryAccountNumber,PAN);
        return CARD_OK;
    }
    else
    {
        //wrong input
        return WRONG_PAN;
    }
    printf("\n");

}

/********************************************
      /* Test Functions*/


void getCardHolderNameTest(void) {


    ST_cardData_t cardData;
    uint8_t Error;

    printf("Tester Name : Ahmed elbohy\n");
    printf("Function name : getCardHolderName\n\n");

    printf("Test Case 1 : ahmed\nInput Data : ");
    Error = getCardHolderName(&cardData);
    printf("Expected Result : wrong name\nActual result : ");
    if (Error == WRONG_NAME)
    {
        printf("wrong name");
    }
    else if (Error == CARD_OK)
    {
        printf("right name");
    }
    /******************************************************************/
    printf("\n\nTest Case 2 : ahmed mohammed mohammed elbohy\nInput Data : ");
    Error = getCardHolderName(&cardData);
    printf("Expected Result : wrong name\nActual result : ");
    if (Error == WRONG_NAME)
    {
        printf("wrong name");
    }
    else if (Error == CARD_OK)
    {
        printf("right name");
    }
    /******************************************************************/
    printf("\n\nTest Case 3 : ahmed mohammed mohammed \nInput Data : ");
    Error = getCardHolderName(&cardData);
    printf("Expected Result : right name\nActual result : ");
    if (Error == WRONG_NAME)
    {
        printf("wrong name");
    }
    else if (Error == CARD_OK)
    {
        printf("right name");
    }
}

void getCardExpiryDateTest(void)
{
    ST_cardData_t cardData;
    uint8_t Error;

    printf("Tester Name : Ahmed elbohy\n");
    printf("Function name : getCardExpiryDateTest\n\n");

    printf("Test Case 1 : 11/1/2010\nInput Data : ");
    Error = getCardExpiryDate(&cardData);
    printf("Expected Result :wrong expiration date\nActual result : ");
    if (Error == WRONG_EXP_DATE)
    {
        printf("wrong expiration date");
    }
    else if (Error == CARD_OK)
    {
        printf("card ok");
    }
    /******************************************************************/
    printf("\n\nTest Case 2 : 11-01\nInput Data : ");
    Error = getCardExpiryDate(&cardData);
    printf("Expected Result : wrong expiration date\nActual result : ");
    if (Error == WRONG_EXP_DATE)
    {
        printf("wrong expiration date");
    }
    else if (Error == CARD_OK)
    {
        printf("card ok");
    }
    /******************************************************************/
    printf("\n\nTest Case 3 : 11/01\nInput Data : ");
    Error = getCardExpiryDate(&cardData);
    printf("Expected Result : card ok\nActual result : ");
    if (Error == WRONG_EXP_DATE)
    {
        printf("wrong expiration date");
    }
    else if (Error == CARD_OK)
    {
        printf("card ok");
    }
}


void getCardPANTest(void)
{
    ST_cardData_t cardData;
    uint8_t Error;

    printf("Tester Name : Ahmed elbohy\n");
    printf("Function name : getCardPANTest\n\n");
    
    printf("Test Case 1 : 123456789\nInput Data : ");
    Error = getCardPAN(&cardData);
    printf("Expected Result :wrong PAN\nActual result : ");
    if (Error == WRONG_PAN)
    {
        printf("wrong PAN");
    }
    else if (Error == CARD_OK)
    {
        printf("card ok");
    }
    /******************************************************************/
    printf("\n\nTest Case 2 : 123456789123456789123456789\nInput Data : ");
    Error = getCardPAN(&cardData);
    printf("Expected Result :wrong PAN\nActual result : ");
    if (Error == WRONG_PAN)
    {
        printf("wrong PAN");
    }
    else if (Error == CARD_OK)
    {
        printf("card ok");
    }
    /*******************************************************************/
    printf("\n\nTest Case 3 : 123456789123456789\nInput Data : ");
    Error = getCardPAN(&cardData);
    printf("Expected Result :card ok\nActual result : ");
    if (Error == WRONG_PAN)
    {
        printf("wrong PAN");
    }
    else if (Error == CARD_OK)
    {
        printf("card ok");
    }
}



/*void main(void)
{
    //getCardHolderNameTest();
    //getCardExpiryDateTest();
   //getCardPANTest();
}*/