#include <stdio.h>
#include <string.h>
#include "../card/Card.h"
#include "../Terminal/Terminal.h"
#include "../Server/Server.h"
#include "App.h"

//variables to save data over the process
ST_cardData_t cardData;
ST_terminalData_t termData;
ST_transaction_t transData;

#define transaction_max_amount 50000

void appStart(void)
{
	while(1)
	{ 
    /*Get inputs from user about card data
    variable to return card error*/
    EN_cardError_t carderror;

    //check card holder name
	carderror=getCardHolderName(&cardData);
	while (carderror == WRONG_NAME) {
		printf("Wrong Name\n");
		printf("ReEnter Name:\n");
		carderror = getCardHolderName(&cardData);
        printf("\n");
	}

    //check card expiration date
	carderror = getCardExpiryDate(&cardData);
	while (carderror == WRONG_EXP_DATE) {
		printf("Wrong Expiration Date\n");
		printf("ReEnter Expiration Date:\n");
		carderror = getCardExpiryDate(&cardData);
        printf("\n");
	}

    //check card PAN
	carderror = getCardPAN(&cardData);
	while (carderror == WRONG_PAN) {
		printf("Wrong PAN\n");
		printf("ReEnter PAN:\n");
		carderror = getCardPAN(&cardData);
        printf("\n");
	}
	printf("\n");

    /*Get inputs from user about card data
    variable to return terminal error*/
    EN_terminalError_t terminalerror;

    //check transaction date
	terminalerror = getTransactionDate(&termData);
	while (terminalerror == WRONG_DATE) {
		printf("Wrong Date\n");
		printf("ReEnter Date:\n");
		terminalerror = getTransactionDate(&termData);
        printf("\n");
	}

    //check if card expired or not
	terminalerror = isCardExpired(&cardData, &termData);
	if (terminalerror == EXPIRED_CARD) {
		printf("\nExpired Card\n");
		break;
	}
  
    //check transaction amount
	terminalerror = getTransactionAmount(&termData);
	while (terminalerror == INVALID_AMOUNT) {
		printf("Invalid Anount\n");
		printf("ReEnter:\n");
		terminalerror = getTransactionAmount(&termData);
        printf("\n");
	}

    //check maximum amount
	terminalerror = setMaxAmount(&termData,transaction_max_amount);
	if (terminalerror == INVALID_MAX_AMOUNT) {
		printf("Invalid Max Amount\n");
        printf("\n");
	}

    //check if you exceed max amount or not
	terminalerror = isBelowMaxAmount(&termData);
	if (terminalerror == EXCEED_MAX_AMOUNT){
		printf("\nDeclined Exceed Max Amount\n");
        printf("\n");
		break;
	}
	printf("\n");
  
    //card and terminal data are transfered into transaction data at server module
	transData.cardHolderData= cardData;
	transData.terminalData = termData;

    /*Get inputs from user about transaction data
    variable to return transaction state*/
	EN_transState_t transaction_state;
    
    //check transaction state
    transaction_state = recieveTransactionData(&transData);
    if (transaction_state == FRAUD_CARD)
    {
        printf("\nFraud Card\n");
    }
	else if (transaction_state == DECLINED_STOLEN_CARD) 
    {
		printf("\nDeclined Stolen Card\n");
	}
	else if (transaction_state== DECLINED_INSUFFECIENT_FUND) 
    {
		printf("\nDeclined Insufficient Fund\n");
	}
    else if (transaction_state== INTERNAL_SERVER_ERROR) 
    {
		printf("\nInternal Server Error\n");
	}
	else if (transaction_state == APPROVED) 
    {
		printf("Approved, Saving Transaction...\n");
		printf("Transaction saved successfully\n");
	}
	break;
    printf("\n");
}
}