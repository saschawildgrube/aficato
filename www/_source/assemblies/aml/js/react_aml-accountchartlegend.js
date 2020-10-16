'use strict';

class AMLAccountChartLegendReactComponent extends React.Component
{
  constructor(props)
  {
    super(props);
    this.state =
    {
    };
  }

  render()
  {
		var aTypes = this.props.transactiontypes;

	  var items = [];
	  items.push(
	  	{
		  	color: '{CSSCOLOR_TRANSACTION_DEPOSIT}',
		  	key: 'deposit',
		  	text: '?TID_AML_ITEM_DEPOSIT?'
	  	});
	 
	 /*
	 	for (var nIndex=0; nIndex < aTypes.length; nIndex++)
	 	{
	 		var strType = aTypes[nIndex];	
		  items.push(
		  	{
			  	color: '{CSSCOLOR_TRANSACTION_DEPOSIT_'+strType.toUpperCase()+'}',
			  	key: 'deposit-'+strType.toLowerCase(),
			  	text: '?TID_AML_TRANSACTIONTYPE_'+strType.toUpperCase()+'?'
		  	});
	 		
	 	}
	 	*/ 	

	  items.push(
	  	{
		  	color: '{CSSCOLOR_TRANSACTION_DEPOSIT_CASH}',
		  	key: 'deposit-cash',
		  	text: '?TID_AML_TRANSACTIONTYPE_CASH?'
	  	});
	  items.push(
	  	{
		  	color: '{CSSCOLOR_TRANSACTION_DEPOSIT_DEBITCARD}',
		  	key: 'deposit-debitcard',
		  	text: '?TID_AML_TRANSACTIONTYPE_DEBITCARD?'
	  	});
	  items.push(
	  	{
		  	color: '{CSSCOLOR_TRANSACTION_DEPOSIT_WIRE}',
		  	key: 'deposit-wire',
		  	text: '?TID_AML_TRANSACTIONTYPE_WIRE?'
	  	});
	  items.push(
	  	{
		  	color: '{CSSCOLOR_TRANSACTION_DEPOSIT_SALARY}',
		  	key: 'deposit-salary',
		  	text: '?TID_AML_TRANSACTIONTYPE_SALARY?'
	  	});	  	
	  items.push(
	  	{
		  	color: '{CSSCOLOR_TRANSACTION_DEPOSIT_OTHER}',
		  	key: 'deposit-other',
		  	text: '?TID_AML_TRANSACTIONTYPE_OTHER?'
	  	});


	  items.push(
	  	{
		  	color: '{CSSCOLOR_TRANSACTION_WITHDRAWAL}',
		  	key: 'withdrawal',
		  	text: '?TID_AML_ITEM_WITHDRAWAL?'
	  	});
	  items.push(
	  	{
		  	color: '{CSSCOLOR_TRANSACTION_WITHDRAWAL_CASH}',
		  	key: 'withdrawal-cash',
		  	text: '?TID_AML_TRANSACTIONTYPE_CASH?'
	  	});
	  items.push(
	  	{
		  	color: '{CSSCOLOR_TRANSACTION_WITHDRAWAL_DEBITCARD}',
		  	key: 'withdrawal-debitcard',
		  	text: '?TID_AML_TRANSACTIONTYPE_DEBITCARD?'
	  	});	  	
	  items.push(
	  	{
		  	color: '{CSSCOLOR_TRANSACTION_WITHDRAWAL_WIRE}',
		  	key: 'withdrawal-wire',
		  	text: '?TID_AML_TRANSACTIONTYPE_WIRE?'
	  	});
	  items.push(
	  	{
		  	color: '{CSSCOLOR_TRANSACTION_WITHDRAWAL_SALARY}',
		  	key: 'withdrawal-salary',
		  	text: '?TID_AML_TRANSACTIONTYPE_SALARY?'
	  	});
	  items.push(
	  	{
		  	color: '{CSSCOLOR_TRANSACTION_WITHDRAWAL_OTHER}',
		  	key: 'withdrawal-other',
		  	text: '?TID_AML_TRANSACTIONTYPE_OTHER?'
	  	});

	  
	  var props = [];
	  props.items = items;
	  
  	return e(ColorLegendReactComponent,props,null);

  }
}

