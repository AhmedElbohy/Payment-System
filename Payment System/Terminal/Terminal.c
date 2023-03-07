#include <stdio.h>
#include <string.h>
#include "../Card/Card.h"
#include "Terminal.h"

#define MaxAmount 50000

//Implementation of getTransactionDate function
EN_terminalError_t getTransactionDate(ST_terminalData_t *termData)
{
    uint8_t TransDate[20];
    
    //get transaction date from user
    printf("please enter transaction date (DD/MM/YYYY): ");
    gets(TransDate);
    
    uint8_t TransDateLength = strlen(TransDate);

    uint8_t DD,MM;

    // DAY from 01 to 30
    DD = ((TransDate[0] - '0') * 10) + (TransDate[1] - '0');

    // month from 01 to 12
    MM = ((TransDate[3] - '0') * 10) + (TransDate[4] - '0');


    if((TransDate!= NULL) && (TransDateLength==10) && (TransDate[2] == '/') && (TransDate[5] == '/') &&
       ( DD > 0 && DD <= 30 ) && ( MM >0 && MM <=12 ))
    {
        //correct input
        strcpy(termData->transactionDate,TransDate);
        return TERMINAL_OK;
    }
    else
    {
        //wrong input
        return WRONG_DATE;
    }
    printf("\n");
}


//Implementation of isCardExpired function

EN_terminalError_t isCardExpired(ST_cardData_t *cardData, ST_terminalData_t *termData)
{
    //get expiration date (MM/YY)
    uint8_t Exp_Month = ((cardData->cardExpirationDate[0] - '0') * 10) + (cardData->cardExpirationDate[1] - '0');
    uint8_t Exp_year  = ((cardData->cardExpirationDate[3]  - '0') * 10) + (cardData->cardExpirationDate[4] - '0');

    //get transaction date (DD/MM/YYYY)
    uint8_t Trans_Month = ((termData->transactionDate[3] - '0') * 10) + (termData->transactionDate[4] - '0');
    uint8_t Trans_Year = ((termData->transactionDate[8] - '0') * 10) + (termData->transactionDate[9] - '0');

    if(Trans_Year > Exp_year)
    {
        return EXPIRED_CARD;
    }
    else if((Trans_Year = Exp_year) && (Trans_Month > Exp_Month))
    {
        return EXPIRED_CARD;
    }
    else
    {
        return TERMINAL_OK;
    }
    printf("\n");
}


//Implementation of getTransactionAmount function

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
    float Amount;
    printf("please enter transaction amount: ");
    scanf("%f",&Amount);

    //checking if amount is valid or not
    if(Amount > 0)
    {
        termData->transAmount = Amount;
        return TERMINAL_OK;
    }
    else
    {
        return INVALID_AMOUNT;
    }
    
    printf("\n");
}

//Implementation of setMaxAmount function

EN_terminalError_t setMaxAmount(ST_terminalData_t *termData, float maxAmount)
{
    if(maxAmount>0)
    {
        termData->maxTransAmount = maxAmount;
        return TERMINAL_OK;
    }
     else
    {
        return INVALID_MAX_AMOUNT;
    }
    printf("\n");
}


//Implementation of isBelowMaxAmount function

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData)
{
    if (termData->maxTransAmount > termData->transAmount) {
		return TERMINAL_OK;
    }
     else
    {
        return EXCEED_MAX_AMOUNT;
	}
    printf("\n");
}



/************************************************
      /* Test Functions*/

void getTransactionDateTest(void)
{
    ST_terminalData_t termData;
    uint8_t Error;

    printf("Tester Name : Ahmed elbohy\n");
    printf("Function name : getTransactionDateTest\n\n");

    printf("Test Case 1 : 25/10\nInput Data : ");
    Error = getTransactionDate(&termData);
    printf("Expected Result : wrong date\nActual result : ");
    if (Error == WRONG_DATE)
    {
        printf("wrong date");
    }
    else
    {
        printf("Terminal ok");
    }

    printf("\n\nTest Case 2 : 25/10/20\nInput Data : ");
    Error = getTransactionDate(&termData);
    printf("Expected Result : wrong date\nActual result : ");
    if (Error == WRONG_DATE)
    {
        printf("wrong date");
    }
    else
    {
        printf("Terminal ok");
    }

    printf("\n\nTest Case 3 : 25/10/2020\nInput Data : ");
    Error = getTransactionDate(&termData);
    printf("Expected Result : Terminal ok\nActual result : ");
    if (Error == WRONG_DATE)
    {
        printf("wrong date");
    }
    else
    {
        printf("Terminal ok");
    }
}


void isCardExpriedTest(void)
{
    ST_cardData_t cardData;
    ST_terminalData_t termData;


    uint8_t Error;
    printf("Tester Name : Ahmed elbohy\n");
    printf("Function name : isCardExpriedTest\n\n");

    getCardExpiryDate(&cardData);

    printf("\nTest Case 1 : 11/03/2022\nInput Data : ");
    getTransactionDate(&termData);
    Error = isCardExpired(&cardData, &termData);
    printf("Expected Result : EXPIRED CARD\nActual result : ");
    if (Error == EXPIRED_CARD)
    {
        printf("EXPIRED CARD");
    }
    else if (Error == TERMINAL_OK)
    {
        printf("Terminal Ok");
    }
    printf("\n\nTest Case 2 : 25/10/2020\nInput Data : ");
    getTransactionDate(&termData);
    Error = isCardExpired(&cardData, &termData);
    printf("Expected Result : Terminal Ok\nActual result : ");
    if (Error == EXPIRED_CARD)
    {
        printf("EXPIRED CARD");
    }
    else if (Error == TERMINAL_OK)
    {
        printf("Terminal Ok");
    }
    printf("\n\nTest Case 3 : 25/09/2019\nInput Data : ");
    getTransactionDate(&termData);
    Error = isCardExpired(&cardData, &termData);
    printf("Expected Result : Terminal Ok\nActual result : ");
    if (Error == EXPIRED_CARD)
    {
        printf("EXPIRED CARD");
    }
    else if (Error == TERMINAL_OK)
    {
        printf("Terminal Ok");
    }
}
                        
void getTransactionAmountTest(void)
{
    ST_terminalData_t termData;
    uint8_t Error;

    printf("Tester Name : Ahmed elbohy\n");
    printf("Function name : getTransactionAmountTest\n\n");
   
    printf("\nTest Case 1 : 0\nInput Data : ");
    Error = getTransactionAmount(&termData);
    printf("Expected Result : invalid amount\nActual result : ");
    if(Error == INVALID_AMOUNT)
    {
        printf("invalid amount");
    }
    else
    {
        printf("TERMINAL OK");
    }

    printf("\n\nTest Case 1 : 1000\nInput Data : ");
    Error = getTransactionAmount(&termData);
    printf("Expected Result : TERMINAL OK\nActual result : ");
    if (Error == INVALID_AMOUNT)
    {
        printf("invalid amount");
    }
    else
    {
        printf("TERMINAL OK");
    }
}


void setMaxAmountTest(void)
{
    ST_terminalData_t termData;
    uint8_t Error;

    printf("Tester Name : Ahmed elbohy\n");
    printf("Function name : getTransactionAmountTest\n\n");


    Error = setMaxAmount(&termData,MaxAmount);

    if(Error == INVALID_MAX_AMOUNT)
    {
        printf("invalid Max amount\n");
    }
    else
    {
        printf("Terminal Ok\n");
    }
}


void isBelowMaxAmountTest(void)
{
    ST_terminalData_t termData;
    uint8_t Error;

    printf("Tester Name : Ahmed elbohy\n");
    printf("Function name : isBelowMaxAmountTest\n\n");

    termData.maxTransAmount=50000;

    printf("\nTest Case 1 : 150000\nInput Data : ");
    getTransactionAmount(&termData);
    Error = isBelowMaxAmount(&termData);
    printf("Expected Result : EXCEED_MAX_AMOUNT\nActual result : ");
    if(Error == EXCEED_MAX_AMOUNT)
    {
        printf("EXCEED_MAX_AMOUNT");
    }
    else 
    {
        printf("Terminal ok");
    }

    printf("\n\nTest Case 2 : 2000\nInput Data : ");
    getTransactionAmount(&termData);
    Error = isBelowMaxAmount(&termData);
    printf("Expected Result : Terminal ok\nActual result : ");
    if (Error == EXCEED_MAX_AMOUNT)
    {
        printf("EXCEED_MAX_AMOUNT");
    }
    else
    {
        printf("Terminal ok");
    }
}



/*void main(void)
{
    //getTransactionDateTest();
    //isCardExpriedTest();
    //getTransactionAmountTest();
    //setMaxAmountTest();
    //isBelowMaxAmountTest();
}*/