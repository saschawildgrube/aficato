<?php

	function IsValidCurrencyID($strCurrency)
	{
		return ArrayKeyExists(GetAllCurrencySymbols(),$strCurrency);
	}

	function GetAllCurrencyIDs()
	{
		return array_keys(GetAllCurrencySymbols());
	}
	
	function GetAllCurrencySymbols()
	{
		return array(
			'AED' => 'AED',
			'CAD' => 'C$',
			'CHF' => 'SFr', 
			'CNY' => '¥',
			'COP' => '$',
			'CZK' => 'Kc',
			'EUR' => '€',
			'GBP' => '£',
			'ISK' => 'ikr',
			'MAD' => 'Dh',
			'NOK' => 'kr',
			'PHP' => '₱',
			'PLN' => 'zł',
			'SGD' => 'S$',
			'USD' => '$',
			'YEN' => '¥');
	}
	
	function GetCurrencySymbol($strCurrencyID)
	{
		if (!IsValidCurrencyID($strCurrencyID))
		{
			return '¤';	
		}
		return ArrayGetValue(GetAllCurrencySymbols(),$strCurrencyID);
	}
	
	