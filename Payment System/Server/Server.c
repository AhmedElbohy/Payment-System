#include <stdio.h>
#include <string.h>
#include "../card/Card.h"
#include "../Terminal/Terminal.h"
#include "Server.h"
 
//Implement some accounts with balance, state, PAN
ST_accountsDB_t accountsDB[255] ={{100000, RUNNING, "123456789987654321"},
                                  {5000, BLOCKED, "123456789987654333"}, 
                                  {3000, BLOCKED, "123123456456789789"},
                                  {2000, RUNNING, "123456789987654322"},
                                  {5000, RUNNING, "123456789123456789"},
                                  {6000, BLOCKED, "987654321987654321"}};


//Implement transactions array with initial values 0
ST_transaction_t transactionsDB[255]={0};

uint32_t TransSeqNum = 0;



/*Implementation of isValidAccount function
 
 the function checking if card PAN available in database*/

EN_serverError_t isValidAccount(ST_cardData_t *cardData, ST_accountsDB_t *accountRefrence)
{ 
    uint8_t flag = 0;

    for(uint8_t i=0;i<6;i++)
    {
        if(strcmp(cardData->primaryAccountNumber, accountsDB[i].primaryAccountNumber)==0)
        {
            accountRefrence->state = accountsDB[i].state;
            accountRefrence->balance = accountsDB[i].balance;
            strcpy(accountRefrence->primaryAccountNumber, accountsDB[i].primaryAccountNumber);
            flag = 1;
            return SERVER_OK;
        }
    }
      if(flag == 0)
      {
          accountRefrence = NULL;
          return ACCOUNT_NOT_FOUND;
      }
    
    printf("\n");
}


//Implementation of isBlockedAccount function
//the function checks if account blocked or not

EN_serverError_t isBlockedAccount(ST_accountsDB_t *accountRefrence)
{
    if(accountRefrence->state == BLOCKED)
    {
        return BLOCKED_ACCOUNT;
    }
    else
    {
        return SERVER_OK;
    }
}

//Implementation of isAmountAvailable function
//the function checks if balance can cover transaction amount or not

EN_serverError_t isAmountAvailable(ST_terminalData_t *termData, ST_accountsDB_t *accountRefrence)
{
    if (termData->transAmount > accountRefrence->balance) {
        return LOW_BALANCE;
    }
    else
    {
        return SERVER_OK;
    }
}

//Implementation of saveTransaction function

EN_serverError_t saveTransaction(ST_transaction_t *transData)
{

    if(TransSeqNum<255)
    {
        //copy card data
        strcpy(transactionsDB[TransSeqNum].cardHolderData.cardHolderName , transData->cardHolderData.cardHolderName);
        strcpy(transactionsDB[TransSeqNum].cardHolderData.cardExpirationDate , transData->cardHolderData.cardExpirationDate);
        strcpy(transactionsDB[TransSeqNum].cardHolderData.primaryAccountNumber , transData->cardHolderData.primaryAccountNumber);

        //copy terminal data
        strcpy(transactionsDB[TransSeqNum].terminalData.transactionDate , transData->terminalData.transactionDate);
        transactionsDB[TransSeqNum].terminalData.transAmount = transData->terminalData.transAmount;
        transactionsDB[TransSeqNum].terminalData.maxTransAmount = transData->terminalData.maxTransAmount;

        //copy account state
        transactionsDB[TransSeqNum].transState = transData->transState;

        //copy sequence number
        transactionsDB[TransSeqNum].transactionSequenceNumber = TransSeqNum;

        TransSeqNum++;
        return SERVER_OK;
    }
    else
    {
        return SAVING_FAILED;
    }
    
}


//Implementation of RecieveTransactionData function
EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
    ST_accountsDB_t accountRefrence;

    if (isValidAccount(&transData->cardHolderData, &accountRefrence) == ACCOUNT_NOT_FOUND)
    {
        transData->transState = FRAUD_CARD;
        return FRAUD_CARD;
    }
    else if (isBlockedAccount(&accountRefrence) == BLOCKED_ACCOUNT)
    {
        transData->transState = DECLINED_STOLEN_CARD;
        return DECLINED_STOLEN_CARD;
    }
    else if (isAmountAvailable(&transData->terminalData, &accountRefrence) == LOW_BALANCE)
    {
        transData->transState = DECLINED_INSUFFECIENT_FUND;
        return DECLINED_INSUFFECIENT_FUND;
    }
    else if (saveTransaction(transData) == SAVING_FAILED)
    {
        return INTERNAL_SERVER_ERROR;
    }
    else
    {
        accountsDB[TransSeqNum].balance = accountsDB[TransSeqNum].balance - transData->terminalData.transAmount;
        printf("\nyour new balance = %.2f\n\n", accountsDB[TransSeqNum].balance);
        return APPROVED;
    }
    printf("\n");
}

void listSavedTransactions(void)
{

    printf("\n*****transactions history*****\n");

    printf("\nTransaction Sequence Number: %d\n", TransSeqNum);

    printf("Transaction Date: %s\n", transactionsDB[TransSeqNum].terminalData.transactionDate);

    printf("Transaction Amount: %f\n", transactionsDB[TransSeqNum].terminalData.transAmount);

    printf("Transaction State: ");
    if (transactionsDB[TransSeqNum].transState == APPROVED) {
		printf("APPROVED\n");
	}
	else if (transactionsDB[TransSeqNum].transState == DECLINED_INSUFFECIENT_FUND)
    {
		printf("DECLINED_INSUFFECIENT_FUND\n");
	}
	else if (transactionsDB[TransSeqNum].transState == DECLINED_STOLEN_CARD)
    {
		printf("DECLINED_STOLEN_CARD\n");
	}
    else if (transactionsDB[TransSeqNum].transState == FRAUD_CARD)
    {
		printf("FRAUD_CARD\n");
	}
     else if (transactionsDB[TransSeqNum].transState == INTERNAL_SERVER_ERROR)
    {
		printf("INTERNAL_SERVER_ERROR\n");
	}

    printf("Terminal Max Amount: %f\n", transactionsDB[TransSeqNum].terminalData.maxTransAmount);

    printf("Cardholder Name: %s\n", transactionsDB[TransSeqNum].cardHolderData.cardHolderName);

    printf("PAN: %s\n", transactionsDB[TransSeqNum].cardHolderData.primaryAccountNumber);

    printf("Card Expiration Date: %s\n\n", transactionsDB[TransSeqNum].cardHolderData.cardExpirationDate);

}


/********************************************
      /* Test Functions*/

void isValidAccountTest(void)
{
    ST_terminalData_t termData;
    ST_accountsDB_t accountRefrence;
    uint8_t Error;

    printf("Tester Name : Ahmed elbohy\n");
    printf("Function name : isValidAccountTest\n\n");

    printf("Test Case 1 : 123456789987654444\nInput Data : ");
    getCardPAN(&termData);
    Error = isValidAccount(&termData, &accountRefrence);
    printf("Expected Result : ACCOUNT_NOT_FOUND\nActual result : ");
    if(Error == ACCOUNT_NOT_FOUND)
    {
        printf("ACCOUNT_NOT_FOUND");
    }
    else if(Error == SERVER_OK) 
    {
        printf("SERVER OK");
    }

    printf("\n\nTest Case 2 : 123456789987654321\nInput Data : ");
    getCardPAN(&termData);
    Error = isValidAccount(&termData, &accountRefrence);
    printf("Expected Result : SERVER OK\nActual result : ");
    if (Error == ACCOUNT_NOT_FOUND)
    {
        printf("ACCOUNT_NOT_FOUND");
    }
    else if (Error == SERVER_OK)
    {
        printf("SERVER OK");
    }
}


void isBlockedAccountTest(void)
{
    ST_accountsDB_t accountRefrence;
    uint8_t Error;

    printf("Tester Name : Ahmed elbohy\n");
    printf("Function name : isBlockedAccountTest\n\n");

    printf("Test Case 1 : BLOCKED\n");
    Error = isBlockedAccount(&accountsDB[1]);
    printf("Expected Result : BLOCKED_ACCOUNT\nActual result : ");
    if (Error == BLOCKED_ACCOUNT)
    {
        printf("BLOCKED_ACCOUNT");
    }
    else if (Error == SERVER_OK)
    {
        printf("SERVER_OK");
    }

    printf("\n\nTest Case 2 : RUNNING\n");
    Error = isBlockedAccount(&accountsDB[0]);
    printf("Expected Result : SERVER OK\nActual result : ");
    if (Error == BLOCKED_ACCOUNT)
    {
        printf("BLOCKED_ACCOUNT");
    }
    else if (Error == SERVER_OK)
    {
        printf("SERVER_OK");
    }
}

void isAmountAvailableTest(void)
{
    ST_terminalData_t termData;
    ST_accountsDB_t accountRefrence;
    uint8_t Error;

    printf("Tester Name : Ahmed elbohy\n");
    printf("Function name : isAmountAvailableTest\n\n");

    printf("Test Case 1 : LOW BALANCE\n");
    getTransactionAmount(&termData);
    Error = isAmountAvailable(&termData, &accountsDB[0]);
    printf("Expected Result : LOW BALANCE\nInput Data : ");
    if (Error == LOW_BALANCE)
    {
        printf("LOW BALANCE");
    }
    else if (Error == SERVER_OK)
    {
        printf("SERVER OK");
    }

    printf("\n\nTest Case 2 :SERVER OK\n");
    getTransactionAmount(&termData);
    Error = isAmountAvailable(&termData, &accountsDB[0]);
    printf("Expected Result : SERVER OK\nInput Data : ");
    if (Error == LOW_BALANCE)
    {
        printf("LOW BALANCE");
    }
    else if (Error == SERVER_OK)
    {
        printf("SERVER OK");
    }
}


void recieveTransactionDataTest(void)
{
    ST_transaction_t transData;
    uint8_t Error;

    printf("Tester Name : Ahmed elbohy\n");
    printf("Function name : recieveTransactionDataTest\n\n");

   //printf("Test Case 1: 123456789987654311\nexpected result: FRAUD CARD\nInput Data : ");
    //printf("Test Case 2: 123456789987654333\nexpected result:DECLINED STOLEN CARD\nInput Data : ");
    //printf("Test Case 3: 123456789123456789\nexpected result:DECLINED INSUFFECIENT FUND\nInput Data : ");
    printf("Test Case 4: 123456789123456789\nexpected result:APPROVED\nInput Data : ");
    getCardPAN(&transData.cardHolderData);
   
    Error = recieveTransactionData(&transData); 
    if (Error == FRAUD_CARD)
    {
        printf("expected result: FRAUD CARD\n");
    }
    else if (Error == DECLINED_STOLEN_CARD)
    {
        printf("expected result:DECLINED STOLEN CARD\n");
    }
    else if (Error == INTERNAL_SERVER_ERROR)
    {
        printf("expected result:INTERNAL SERVER ERROR\n");
    }
    else
    {
        getTransactionAmount(&transData.terminalData);
        if (Error == DECLINED_INSUFFECIENT_FUND)
        {
            printf("expected result:DECLINED INSUFFECIENT FUND\n"); 
        }
        else if(Error == APPROVED)
        {
            printf("expected result:APPROVED\n");
        }
    }
}

void saveTransactionTest(void)
{
    ST_transaction_t transData;
    uint8_t Error;

    printf("Tester Name : Ahmed elbohy\n");
    printf("Function name : saveTransactionTest\n\n");
    
   // printf("Test Case 1 : SERVER_OK \nExpected Result : SERVER_OK\nInput Data : ");
    /**/printf("Test Case 2 : SAVING_FAILED \nExpected Result : SAVING_FAILED\nInput Data : ");
    TransSeqNum = 255;
    Error = saveTransaction(&transData);
    if(Error == SAVING_FAILED)
    {
        printf("result: SAVING FAILED\n");
    }
    else if(Error == SERVER_OK) 
    {
        printf("result: SERVER OK\n");
    }
    printf("\n");
}




/*void main(void)
{
    //recieveTransactionDataTest();
    //isValidAccountTest();
    //isBlockedAccountTest();
    //isAmountAvailableTest();
    //saveTransactionTest();
    //listSavedTransactionsTest();
}*/