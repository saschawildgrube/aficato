<?php

	require_once(GetWDKDir().'wdk_currency.inc');

	function AML_TransformTransactionsToAccounts($arrayTransactions, $arrayMapping)
	{
		$arrayAccounts = array();
		
		if (is_array($arrayTransactions) == false)
		{
			return false;	
		}
		
		if (!is_array($arrayMapping))
		{
			$arrayMapping = array();	
		}
		if (ArrayGetValue($arrayMapping,'DATE') == '')
		{
			$arrayMapping['DATE'] = 'DATE';
		}
		if (ArrayGetValue($arrayMapping,'AMOUNT') == '')
		{
			$arrayMapping['AMOUNT'] = 'AMOUNT';
		}
		if (ArrayGetValue($arrayMapping,'TO') == '')
		{
			$arrayMapping['TO'] = 'TO';
		}
		if (ArrayGetValue($arrayMapping,'FROM') == '')
		{
			$arrayMapping['FROM'] = 'FROM';
		}
		
		foreach ($arrayTransactions as $arrayTransaction)
		{
			if (!is_array($arrayTransaction))
			{
				continue;
			}
			$strDate = ArrayGetValue($arrayTransaction,$arrayMapping['DATE']);
			$strAmount = ArrayGetValue($arrayTransaction,$arrayMapping['AMOUNT']);
			$strFrom = ArrayGetValue($arrayTransaction,$arrayMapping['FROM']);
			$strTo = ArrayGetValue($arrayTransaction,$arrayMapping['TO']);
			
			if ($strDate == '' || $strAmount == '' || $strFrom == '' || $strTo == '')
			{
				continue;	
			}
			
			if (IsValidDate($strDate) == false)
			{
				$strDate = ConvertToDate($strDate);
			}
			$fAmount = GetFloatValue($strAmount);
		
			$arrayAccounts[$strFrom]['ID'] = $strFrom;
			$arrayAccounts[$strFrom]['TRANSACTIONS'][] = array(
				'DATE' => $strDate,
				'AMOUNT' => RenderNumber(-$fAmount,2),
				'ID' => $strTo,
				'TYPE' => '',
				'DESCRIPTION' => '');

			$arrayAccounts[$strTo]['ID'] = $strTo;
			$arrayAccounts[$strTo]['TRANSACTIONS'][] = array(
				'DATE' => $strDate,
				'AMOUNT' => RenderNumber($fAmount,2),
				'ID' => $strFrom,
				'TYPE' => '',
				'DESCRIPTION' => '');
				
		}
	
		// Remove the associative key in the accounts array
		$arrayAccounts = array_values($arrayAccounts);
		
		// Sort the transactions by date and set data quality indicator
		foreach ($arrayAccounts as &$arrayAccount)
		{
			$arrayAccountTransactions = ArrayGetValue($arrayAccount,'TRANSACTIONS');
			$arrayAccount['DATAQUALITY'] = 1; // means we assume best quality
			$arrayAccount['TRANSACTIONS'] =	ArraySort($arrayAccountTransactions,false,
				function ($a,$b)
				{
					return CompareString(
						ArrayGetValue($a,'DATE'),
						ArrayGetValue($b,'DATE'));
				});	
		}
		
		return $arrayAccounts;	
	} 


	function AML_MakeNetworkNodeName($arrayAccount)
	{
		$strID = ArrayGetValue($arrayAccount,'ID');
		return StringCutOff($strID,12);	
	}


	function AML_TransformAccountsToNetworkGraph($arrayAccounts, $strDateBegin = '', $strDateEnd = '')
	{
		$arrayNetwork	= array();
		$arrayNetwork['nodes'] = array();
		$arrayNetwork['links'] = array();
		
		if (is_array($arrayAccounts) == false)
		{
			return false;	
		}
		
		$arrayAccounts = AML_CreateMissingCounterpartyAccounts($arrayAccounts, $strDateBegin, $strDateEnd);
		$arrayAccounts = AML_CalculateAccounts($arrayAccounts, $strDateBegin, $strDateEnd);

		$arrayNetwork['nodes'] = array();
		$arrayNetwork['links'] = array();
		
		$arrayNodes = array();
		$arrayLinks = array();
		
		$fTransactionAmountMin = null;
		$fTransactionAmountMax = 0;
		
		foreach ($arrayAccounts as $arrayAccount)
		{
			$arrayNode = array();
			$strID = ArrayGetValue($arrayAccount,'ID');
			if ($strID == '')
			{
				continue;	
			}
			$arrayNode['id'] = $strID;
			$arrayNode['name'] = AML_MakeNetworkNodeName($arrayAccount);
			$arrayNode['currency'] = GetCurrencySymbol(ArrayGetValue($arrayAccount,'CURRENCY'));
			$arrayNode['text_transaction_amount_in'] = RenderNumber(ArrayGetValue($arrayAccount,'TRANSACTION_AMOUNT_IN'),-3);
			$arrayNode['text_transaction_amount_out'] = RenderNumber(ArrayGetValue($arrayAccount,'TRANSACTION_AMOUNT_OUT'),-3);
			$arrayNode['text_transaction_netflow'] = RenderNumber(ArrayGetValue($arrayAccount,'TRANSACTION_NETFLOW'),-3);
						
			$fTransactionAmountIn = ArrayGetValue($arrayAccount,'TRANSACTION_AMOUNT_IN');
			$fTransactionAmountOut = ArrayGetValue($arrayAccount,'TRANSACTION_AMOUNT_OUT');
			$fTransactionAmount = $fTransactionAmountIn+$fTransactionAmountOut;
			
			if ($fTransactionAmountMin === null)
			{
				$fTransactionAmountMin = $fTransactionAmount;
			}
			else
			{
				$fTransactionAmountMin = min($fTransactionAmountMin,$fTransactionAmount);
			}
			$fTransactionAmountMax = max($fTransactionAmountMax,$fTransactionAmount);
			$arrayNode['transaction_amount'] = $fTransactionAmount;
			$arrayNode['transaction_amount_in'] = $fTransactionAmountIn;
			$arrayNode['transaction_amount_out'] = $fTransactionAmountOut;
			$arrayNode['transaction_netflow'] = ''.ArrayGetValue($arrayAccount,'TRANSACTION_NETFLOW');
			$arrayNode['transaction_count'] = ''.ArrayGetValue($arrayAccount,'TRANSACTION_COUNT');
			$fDataQuality = floatval(ArrayGetValue($arrayAccount,'DATAQUALITY'));
			$arrayNode['dataquality'] = $fDataQuality;
			$arrayNodes[$strID] = $arrayNode;
			
			$arrayCounterparties = ArrayGetValue($arrayAccount,'COUNTERPARTIES');
			if (is_array($arrayCounterparties))
			{
				foreach ($arrayCounterparties as $arrayCounterparty)
				{
					$strCounterpartyID = ArrayGetValue($arrayCounterparty,'ID');
					if ($strCounterpartyID != '')
					{
						$arrayIDs = array($strID,$strCounterpartyID);
						$arrayIDs = ArraySort($arrayIDs);
						$strLinkID = $arrayIDs[0].$arrayIDs[1];
						if (ArrayKeyExists($arrayLinks,$strLinkID) == true)
						{
							if ($fDataQuality <= ArrayGetValue($arrayLinks,$strLinkID,'dataquality'))
							{
								continue;	
							}
						}
						$arrayLinks[$strLinkID]['dataquality'] = $fDataQuality;
						$arrayLinks[$strLinkID]['source'] = $arrayIDs[0];
						$arrayLinks[$strLinkID]['target'] = $arrayIDs[1];
					
						$fAmountIn = ArrayGetValue($arrayCounterparty,'TRANSACTION_AMOUNT_IN');
						$fAmountOut = ArrayGetValue($arrayCounterparty,'TRANSACTION_AMOUNT_OUT');
						$fCountIn = ArrayGetValue($arrayCounterparty,'TRANSACTION_COUNT_IN');
						$fCountOut = ArrayGetValue($arrayCounterparty,'TRANSACTION_COUNT_OUT');

						// Calculate the transaction direction quotient:
						// 0 means all transaction go out.
						// 1 means all transactions go in.
						$fAmountTotal = $fAmountIn + $fAmountOut;
						$fTransactionDirectionQuotient = $fAmountIn / $fAmountTotal;
						
						// if it is to-from (instead of from-to)
						if ($arrayIDs[0] == $strID)
						{
							$arrayLinks[$strLinkID]['transaction_amount_in'] = $fAmountIn;
							$arrayLinks[$strLinkID]['transaction_amount_out'] = $fAmountOut;
							$arrayLinks[$strLinkID]['transaction_count_in'] = $fCountIn;
							$arrayLinks[$strLinkID]['transaction_count_out'] = $fCountOut;
						}
						else
						{
							$fTransactionDirectionQuotient = 1 - $fTransactionDirectionQuotient;
							$arrayLinks[$strLinkID]['transaction_amount_in'] = $fAmountOut;
							$arrayLinks[$strLinkID]['transaction_amount_out'] = $fAmountIn;
							$arrayLinks[$strLinkID]['transaction_count_in'] = $fCountOut;
							$arrayLinks[$strLinkID]['transaction_count_out'] = $fCountIn;
						}
						$arrayLinks[$strLinkID]['transaction_direction_quotient'] = $fTransactionDirectionQuotient;
					}	
				}
			}
		}
		
		
		$fTransactionAmountMinMaxDelta = $fTransactionAmountMax - $fTransactionAmountMin;
		foreach ($arrayNodes as &$arrayNode)
		{
			if ($fTransactionAmountMinMaxDelta == 0)
			{
				$arrayNode['transaction_amount_factor'] = 0.5;
			}
			else
			{
				$arrayNode['transaction_amount_factor'] = ($arrayNode['transaction_amount'] - $fTransactionAmountMin) / $fTransactionAmountMinMaxDelta;
			}
		}
		
		$arrayNetwork['nodes'] = array_values($arrayNodes);
		$arrayNetwork['links'] = array_values($arrayLinks);
		
		return $arrayNetwork;
	}


	function AML_CalculateAccounts($arrayAccounts,$strDateBegin = '', $strDateEnd = '')
	{
		if (!is_array($arrayAccounts))
		{
			return array();
		}
		
		foreach ($arrayAccounts as &$arrayAccount)
		{
			$arrayAccount = AML_CalculateAccount($arrayAccount,$strDateBegin,$strDateEnd);
		}
		return $arrayAccounts;
	}

	function AML_CreateMissingCounterpartyAccounts($arrayAccounts,$strDateBegin = '', $strDateEnd = '',$strDefaultCurrency = 'EUR')
	{
		if (!is_array($arrayAccounts))
		{
			return array();
		}
		
		$arrayAccountIDs = array();
		foreach ($arrayAccounts as $arrayAccount)
		{
			$arrayAccountIDs[] = $arrayAccount['ID'];
		}
		$arrayAccountIDs = ArrayUnique($arrayAccountIDs);
		
		$arrayMissingCounterpartyIDs = array();
		foreach ($arrayAccounts as $arrayAccount)
		{
			$arrayTransactions = ArrayGetValue($arrayAccount,'TRANSACTIONS');
			if (is_array($arrayTransactions))
			{
				foreach ($arrayTransactions as $arrayTransaction)
				{
					$strCounterpartyID = ArrayGetValue($arrayTransaction,'ID');
					if ($strCounterpartyID != '')
					{
						if (ArrayValueExists($arrayAccountIDs,$strCounterpartyID) == false)
						{
							$arrayMissingCounterpartyIDs[] = $strCounterpartyID;
						}	
					}
				}	
			}	
		}
		$arrayMissingCounterpartyIDs = ArrayUnique($arrayMissingCounterpartyIDs);
		
		foreach ($arrayMissingCounterpartyIDs as $strID)
		{
			$arrayAccount = array();
			$arrayAccount['ID'] = $strID;
			$arrayAccount['CURRENCY'] = $strDefaultCurrency;
			$arrayAccount['DATAQUALITY'] = 0;
			
			$arrayAccounts[] = $arrayAccount;
		}
		
		
		return $arrayAccounts;
	}


	function CompareTransactionByDate($arrayTransaction1, $arrayTransaction2)
	{
		$strDate1 = ArrayGetValue($arrayTransaction1,'DATE');
		$strDate2 = ArrayGetValue($arrayTransaction2,'DATE');
		return CompareString($strDate1,$strDate2);
	}

	function CompareAccountsByID($arrayAccount1, $arrayAccount2)
	{
		$strID1 = ArrayGetValue($arrayAccount1,'ID');
		$strID2 = ArrayGetValue($arrayAccount2,'ID');
		return CompareString($strID1,$strID2);
	}
	
	
	function CompareRedFlagsByStartDate($arrayRedFlag1, $arrayRedFlag2)
	{
		$strStartDate1 = ArrayGetValue($arrayRedFlag1,'START_DATE');
		$strStartDate2 = ArrayGetValue($arrayRedFlag2,'START_DATE');
		return CompareString($strStartDate1,$strStartDate2);
	}

	function CompareRedFlagsByTypology($arrayRedFlag1, $arrayRedFlag2)
	{
		$strTypology1 = ArrayGetValue($arrayRedFlag1,'TYPOLOGY');
		$strTypology2 = ArrayGetValue($arrayRedFlag2,'TYPOLOGY');
		return CompareString($strTypology1,$strTypology2);
	}

	function CompareRedFlags($arrayRedFlag1, $arrayRedFlag2)
	{
		$nResult = CompareRedFlagsByTypology($arrayRedFlag1, $arrayRedFlag2);
		if ($nResult != 0)
		{
			return $nResult;
		}
		return CompareRedFlagsByStartDate($arrayRedFlag1, $arrayRedFlag2);
	}

	
	function AML_CalculateAccount($arrayAccount,$strDateBegin = '', $strDateEnd = '')
	{
		if (!is_array($arrayAccount))
		{
			return false;
		}
		if (ArrayCount($arrayAccount) == 0)
		{
			return false;	
		}
		
		$arrayAccount['CALCULATED'] = true;
		
		$nTransactionCountIn = 0;
		$nTransactionCountOut = 0;
		$fTransactionAmountIn = 0.0;
		$fTransactionAmountOut = 0.0;
		$arrayTransactions = ArrayGetValue($arrayAccount,'TRANSACTIONS');
		$arrayCounterparties = array();
		$strTransactionFirstDate = null;
		$strTransactionLastDate = null;
		if (is_array($arrayTransactions))
		{
			foreach($arrayTransactions as $arrayTransaction)
			{
				$strDate = ArrayGetValue($arrayTransaction,'DATE');
				if ($strDateBegin != '')
				{
					if (CompareString($strDate,$strDateBegin) < 0)
					{
						continue;	
					} 	
				}
				if ($strDateEnd != '')
				{
					if (CompareString($strDateEnd,$strDate) < 0)
					{
						continue;	
					} 	
				}
				
				if ($strTransactionFirstDate == null)
				{
					$strTransactionFirstDate = $strDate;
				}
				else
				{
					if (CompareString($strDate,$strTransactionFirstDate) < 0)
					{
						$strTransactionFirstDate = $strDate;
					}	
				}
				if ($strTransactionLastDate == null)
				{
					$strTransactionLastDate = $strDate;
				}
				else
				{
					if (CompareString($strTransactionLastDate,$strDate) < 0)
					{
						$strTransactionLastDate = $strDate;
					}	
				}
				
				
				$fAmount = floatval(ArrayGetValue($arrayTransaction,'AMOUNT'));

				$strCounterpartyID = ArrayGetValue($arrayTransaction,'ID');
				if (ArrayGetValue($arrayCounterparties,$strCounterpartyID,'ID') == '')
				{
					$arrayCounterparties[$strCounterpartyID]['ID'] = $strCounterpartyID;
					$arrayCounterparties[$strCounterpartyID]['TRANSACTION_COUNT_IN'] = 0;
					$arrayCounterparties[$strCounterpartyID]['TRANSACTION_COUNT_OUT'] = 0;
					$arrayCounterparties[$strCounterpartyID]['TRANSACTION_COUNT'] = 0;
					$arrayCounterparties[$strCounterpartyID]['TRANSACTION_AMOUNT_IN'] = 0;
					$arrayCounterparties[$strCounterpartyID]['TRANSACTION_AMOUNT_OUT'] = 0;
					$arrayCounterparties[$strCounterpartyID]['TRANSACTION_NETFLOW'] = 0;
				} 						
				$arrayCounterparties[$strCounterpartyID]['TRANSACTION_COUNT'] = 1 + ArrayGetValue($arrayCounterparties,$strCounterpartyID,'TRANSACTION_COUNT');
				$arrayCounterparties[$strCounterpartyID]['TRANSACTION_NETFLOW'] = $fAmount + ArrayGetValue($arrayCounterparties,$strCounterpartyID,'TRANSACTION_NETFLOW');
				
				if ($fAmount > 0.0)
				{
					$nTransactionCountIn++;
					$fTransactionAmountIn += $fAmount;
					$arrayCounterparties[$strCounterpartyID]['TRANSACTION_AMOUNT_IN'] = $fAmount + ArrayGetValue($arrayCounterparties,$strCounterpartyID,'TRANSACTION_AMOUNT_IN');
					$arrayCounterparties[$strCounterpartyID]['TRANSACTION_COUNT_IN'] = 1 + ArrayGetValue($arrayCounterparties,$strCounterpartyID,'TRANSACTION_COUNT_IN');

				}
				if ($fAmount < 0.0)
				{
					$nTransactionCountOut++;
					$fTransactionAmountOut -= $fAmount;
					$arrayCounterparties[$strCounterpartyID]['TRANSACTION_AMOUNT_OUT'] = -$fAmount + ArrayGetValue($arrayCounterparties,$strCounterpartyID,'TRANSACTION_AMOUNT_OUT');
					$arrayCounterparties[$strCounterpartyID]['TRANSACTION_COUNT_OUT'] = 1 + ArrayGetValue($arrayCounterparties,$strCounterpartyID,'TRANSACTION_COUNT_OUT');

				}
			}	
		}
		
		$arrayTransactions = ArraySort($arrayTransactions,false,'CompareTransactionByDate');
		$arrayAccount['TRANSACTIONS'] = $arrayTransactions;
		
		$arrayAccount['TRANSACTION_COUNT_IN'] = $nTransactionCountIn;
		$arrayAccount['TRANSACTION_COUNT_OUT'] = $nTransactionCountOut;
		$arrayAccount['TRANSACTION_COUNT'] = $nTransactionCountIn + $nTransactionCountOut;
		$arrayAccount['TRANSACTION_AMOUNT_IN'] = $fTransactionAmountIn;
		$arrayAccount['TRANSACTION_AMOUNT_OUT'] = $fTransactionAmountOut;
		$fTransactionBalance = $fTransactionAmountIn - $fTransactionAmountOut;
		$arrayAccount['TRANSACTION_NETFLOW'] = $fTransactionAmountIn - $fTransactionAmountOut; 
		$arrayAccount['COUNTERPARTIES'] = array_values($arrayCounterparties);

		$arrayAccount['TRANSACTION_FIRST_DATE'] = $strTransactionFirstDate;
		$arrayAccount['TRANSACTION_LAST_DATE'] = $strTransactionLastDate;
		
		$fBalanceBegin = GetFloatValue(ArrayGetValue($arrayAccount,'BALANCE_BEGIN'));
		$arrayAccount['BALANCE_BEGIN'] = $fBalanceBegin;
		$arrayAccount['BALANCE_END'] = $fBalanceBegin + $fTransactionBalance;
		
		
		
		return $arrayAccount;
	}


	function AML_GetAccountChartTimePeriod($strFirstDate,$strLastDate,$nMaxColumns = 15)
	{
		$nDays = (GetTime($strLastDate) - GetTime($strFirstDate)) / 60 / 60 / 24;  
		if ($nDays <= $nMaxColumns)
		{
			$strTimePeriod = 'DAILY';
		}
		else if ($nDays <= ($nMaxColumns-2)*7)
		{
			$strTimePeriod = 'WEEKLY';
		}
		else
		{
			$strTimePeriod = 'MONTHLY';	
		}
		return $strTimePeriod;
	}
	
	


	function AML_TransformAccountToAccountChart($arrayAccount, $strDateBegin, $strDateEnd, $strTimePeriod, $arrayTypes)
	{ 
		if (is_array($arrayAccount) == false)
		{
			return false;	
		}
		if (!IsValidDate($strDateBegin))
		{
			return false;	
		}
		if (!IsValidDate($strDateBegin))
		{
			return false;	
		}
		if (CompareString($strDateBegin,$strDateEnd) > 0)
		{
			return false;	
		}
	
		$arraySupportedTimePeriods = array('DAILY','WEEKLY','MONTHLY');
		if (ArrayValueExists($arraySupportedTimePeriods,$strTimePeriod) == false)
		{
			return false;	
		}
		
		$arrayAccount = AML_CalculateAccount($arrayAccount, $strDateBegin, $strDateEnd);
		
		$arrayTransactions = ArrayGetValue($arrayAccount,'TRANSACTIONS');
		if (!is_array($arrayTransactions))
		{
			$arrayTransactions = array();	
		}


		$arrayAccountChart	= array();
		$arrayAccountChartBars = array();
		
		if ($strTimePeriod == 'MONTHLY')
		{
			$strDateBeginEx = RenderDate(GetTimeMonthBegin(GetTime($strDateBegin)));	
			$strDateEndEx = RenderDate(GetTimeMonthEnd(GetTime($strDateEnd)));
			
			$strCurrentMonth = $strDateBeginEx;
			
			while (CompareString($strCurrentMonth,$strDateEndEx) < 0)
			{
				$strRowID = StringSection($strCurrentMonth,0,7);
				$strAxisText = $strRowID;
				$arrayAccountChartBars[$strRowID] = array(
					'axis_text' => $strAxisText,
					'line_value_left' => null,
					'line_value_right' => null,
					'bar_positive_amount' => 0,
					'bar_negative_amount' => 0,
					'bar_positive_count' => 0,
					'bar_negative_count' => 0
					);
				if (is_array($arrayTypes))
				{
					foreach ($arrayTypes as $strFraction)
					{
						$arrayAccountChartBars[$strRowID]['bar_positive_fractions'][$strFraction]['name'] = $strFraction;
						$arrayAccountChartBars[$strRowID]['bar_positive_fractions'][$strFraction]['amount'] = 0;
						$arrayAccountChartBars[$strRowID]['bar_positive_fractions'][$strFraction]['count'] = 0;
						$arrayAccountChartBars[$strRowID]['bar_negative_fractions'][$strFraction]['name'] = $strFraction;						
						$arrayAccountChartBars[$strRowID]['bar_negative_fractions'][$strFraction]['amount'] = 0;
						$arrayAccountChartBars[$strRowID]['bar_negative_fractions'][$strFraction]['count'] = 0;
					}	
				}	
				
				$strCurrentMonth = RenderDate(GetTimeAddMonths(GetTime($strCurrentMonth),1));
			}
			
			foreach ($arrayTransactions as $arrayTransaction)
			{
				$strDate = ArrayGetValue($arrayTransaction,'DATE');
				if (!IsValidDate($strDate))
				{
					continue;
				}
				if (CompareString($strDateBegin,$strDate) > 0)
				{
					continue;		
				}
				if (CompareString($strDate,$strDateEnd) > 0)
				{
					continue;		
				}
				$fAmount = floatval(ArrayGetValue($arrayTransaction,'AMOUNT'));
				if ($fAmount == 0)
				{
					continue;	
				}
				
				$strRowID = StringSection($strDate,0,7);
				if (ArrayKeyExists($arrayAccountChartBars,$strRowID) == false)
				{
					continue;
				}
				
				if ($fAmount > 0)
				{
					$arrayAccountChartBars[$strRowID]['bar_positive_amount'] = $fAmount + ArrayGetValue($arrayAccountChartBars,$strRowID,'bar_positive_amount');
					$arrayAccountChartBars[$strRowID]['bar_positive_count'] = 1 + ArrayGetValue($arrayAccountChartBars,$strRowID,'bar_positive_count');
				}				
				if ($fAmount < 0)
				{
					$arrayAccountChartBars[$strRowID]['bar_negative_amount'] = -$fAmount + ArrayGetValue($arrayAccountChartBars,$strRowID,'bar_negative_amount');
					$arrayAccountChartBars[$strRowID]['bar_negative_count'] = 1 + ArrayGetValue($arrayAccountChartBars,$strRowID,'bar_negative_count');
				}
				
				if (is_array($arrayTypes))
				{
					$strType = ArrayGetValue($arrayTransaction,'TYPE');
					$strFraction = '';
					if (ArrayValueExists($arrayTypes,$strType) == true)
					{
						$strFraction = $strType;
					}
					else if (ArrayValueExists($arrayTypes,'*'))
					{
						$strFraction = '*';
					}
					if ($strFraction != '')
					{
						if ($fAmount > 0)
						{
							$arrayAccountChartBars[$strRowID]['bar_positive_fractions'][$strFraction]['amount'] = $fAmount + floatval(ArrayGetValue($arrayAccountChartBars,$strRowID,'bar_positive_fractions',$strFraction,'amount'));
							$arrayAccountChartBars[$strRowID]['bar_positive_fractions'][$strFraction]['count'] = 1 + floatval(ArrayGetValue($arrayAccountChartBars,$strRowID,'bar_positive_fractions',$strFraction,'count'));
						}				
						if ($fAmount < 0)
						{
							$arrayAccountChartBars[$strRowID]['bar_negative_fractions'][$strFraction]['amount'] = -$fAmount + floatval(ArrayGetValue($arrayAccountChartBars,$strRowID,'bar_negative_fractions',$strFraction,'amount'));
							$arrayAccountChartBars[$strRowID]['bar_negative_fractions'][$strFraction]['count'] = 1 + floatval(ArrayGetValue($arrayAccountChartBars,$strRowID,'bar_negative_fractions',$strFraction,'count'));
						}
					}
				}
			}
		}
		else if ($strTimePeriod == 'WEEKLY')
		{
			$strDateBeginEx = RenderDate(GetTimeWeekBegin(GetTime($strDateBegin)));	
			$strDateEndEx = RenderDate(GetTimeWeekEnd(GetTime($strDateEnd)));


			$strCurrentWeek = $strDateBeginEx;
			
			while (CompareString($strCurrentWeek,$strDateEndEx) <= 0)
			{
				$strRowID = StringSection($strCurrentWeek,0,10);
				$strAxisText = $strRowID;
				$arrayAccountChartBars[$strRowID] = array(
					'axis_text' => $strAxisText,
					'line_value_left' => null,
					'line_value_right' => null,
					'bar_positive_amount' => 0,
					'bar_negative_amount' => 0,
					'bar_positive_count' => 0,
					'bar_negative_count' => 0
					);
				if (is_array($arrayTypes))
				{
					foreach ($arrayTypes as $strFraction)
					{
						$arrayAccountChartBars[$strRowID]['bar_positive_fractions'][$strFraction]['name'] = $strFraction;
						$arrayAccountChartBars[$strRowID]['bar_positive_fractions'][$strFraction]['amount'] = 0;
						$arrayAccountChartBars[$strRowID]['bar_positive_fractions'][$strFraction]['count'] = 0;
						$arrayAccountChartBars[$strRowID]['bar_negative_fractions'][$strFraction]['name'] = $strFraction;						
						$arrayAccountChartBars[$strRowID]['bar_negative_fractions'][$strFraction]['amount'] = 0;
						$arrayAccountChartBars[$strRowID]['bar_negative_fractions'][$strFraction]['count'] = 0;
					}	
				}	
				
				$strCurrentWeek = RenderDate(GetTimeAddDays(GetTime($strCurrentWeek),7));
			}
			
			foreach ($arrayTransactions as $arrayTransaction)
			{
				$strDate = ArrayGetValue($arrayTransaction,'DATE');
				if (!IsValidDate($strDate))
				{
					continue;
				}
				if (CompareString($strDateBegin,$strDate) > 0)
				{
					continue;		
				}
				if (CompareString($strDate,$strDateEnd) > 0)
				{
					continue;		
				}
				$fAmount = floatval(ArrayGetValue($arrayTransaction,'AMOUNT'));
				if ($fAmount == 0)
				{
					continue;	
				}
				
				$strRowID = StringSection(RenderDate(GetTimeWeekBegin(GetTime($strDate))),0,10);
				if (ArrayKeyExists($arrayAccountChartBars,$strRowID) == false)
				{
					continue;
				}
				
				if ($fAmount > 0)
				{
					$arrayAccountChartBars[$strRowID]['bar_positive_amount'] = $fAmount + ArrayGetValue($arrayAccountChartBars,$strRowID,'bar_positive_amount');
					$arrayAccountChartBars[$strRowID]['bar_positive_count'] = 1 + ArrayGetValue($arrayAccountChartBars,$strRowID,'bar_positive_count');
				}				
				if ($fAmount < 0)
				{
					$arrayAccountChartBars[$strRowID]['bar_negative_amount'] = -$fAmount + ArrayGetValue($arrayAccountChartBars,$strRowID,'bar_negative_amount');
					$arrayAccountChartBars[$strRowID]['bar_negative_count'] = 1 + ArrayGetValue($arrayAccountChartBars,$strRowID,'bar_negative_count');
				}
				
				if (is_array($arrayTypes))
				{
					$strType = ArrayGetValue($arrayTransaction,'TYPE');
					$strFraction = '';
					if (ArrayValueExists($arrayTypes,$strType) == true)
					{
						$strFraction = $strType;
					}
					else if (ArrayValueExists($arrayTypes,'*'))
					{
						$strFraction = '*';
					}
					if ($strFraction != '')
					{
						if ($fAmount > 0)
						{
							$arrayAccountChartBars[$strRowID]['bar_positive_fractions'][$strFraction]['amount'] = $fAmount + floatval(ArrayGetValue($arrayAccountChartBars,$strRowID,'bar_positive_fractions',$strFraction,'amount'));
							$arrayAccountChartBars[$strRowID]['bar_positive_fractions'][$strFraction]['count'] = 1 + floatval(ArrayGetValue($arrayAccountChartBars,$strRowID,'bar_positive_fractions',$strFraction,'count'));
						}				
						if ($fAmount < 0)
						{
							$arrayAccountChartBars[$strRowID]['bar_negative_fractions'][$strFraction]['amount'] = -$fAmount + floatval(ArrayGetValue($arrayAccountChartBars,$strRowID,'bar_negative_fractions',$strFraction,'amount'));
							$arrayAccountChartBars[$strRowID]['bar_negative_fractions'][$strFraction]['count'] = 1 + floatval(ArrayGetValue($arrayAccountChartBars,$strRowID,'bar_negative_fractions',$strFraction,'count'));
						}
					}
				}
			}
		}		
		else if ($strTimePeriod == 'DAILY')
		{
			$strCurrentDay = $strDateBegin;
			
			while (CompareString($strCurrentDay,$strDateEnd) <= 0)
			{
				$strRowID = StringSection($strCurrentDay,0,10);
				$strAxisText = $strRowID;
				$arrayAccountChartBars[$strRowID] = array(
					'axis_text' => $strAxisText,
					'line_value_left' => null,
					'line_value_right' => null,
					'bar_positive_amount' => 0,
					'bar_negative_amount' => 0,
					'bar_positive_count' => 0,
					'bar_negative_count' => 0
					);
				if (is_array($arrayTypes))
				{
					foreach ($arrayTypes as $strFraction)
					{
						$arrayAccountChartBars[$strRowID]['bar_positive_fractions'][$strFraction]['name'] = $strFraction;
						$arrayAccountChartBars[$strRowID]['bar_positive_fractions'][$strFraction]['amount'] = 0;
						$arrayAccountChartBars[$strRowID]['bar_positive_fractions'][$strFraction]['count'] = 0;
						$arrayAccountChartBars[$strRowID]['bar_negative_fractions'][$strFraction]['name'] = $strFraction;						
						$arrayAccountChartBars[$strRowID]['bar_negative_fractions'][$strFraction]['amount'] = 0;
						$arrayAccountChartBars[$strRowID]['bar_negative_fractions'][$strFraction]['count'] = 0;
					}	
				}	
				
				$strCurrentDay = RenderDate(GetTimeAddDays(GetTime($strCurrentDay),1));
			}
			
			foreach ($arrayTransactions as $arrayTransaction)
			{
				$strDate = ArrayGetValue($arrayTransaction,'DATE');
				if (!IsValidDate($strDate))
				{
					continue;
				}
				if (CompareString($strDateBegin,$strDate) > 0)
				{
					continue;		
				}
				if (CompareString($strDate,$strDateEnd) > 0)
				{
					continue;		
				}
				$fAmount = floatval(ArrayGetValue($arrayTransaction,'AMOUNT'));
				if ($fAmount == 0)
				{
					continue;	
				}
				
				$strRowID = StringSection($strDate,0,10);
				if (ArrayKeyExists($arrayAccountChartBars,$strRowID) == false)
				{
					continue;
				}
				
				if ($fAmount > 0)
				{
					$arrayAccountChartBars[$strRowID]['bar_positive_amount'] = $fAmount + ArrayGetValue($arrayAccountChartBars,$strRowID,'bar_positive_amount');
					$arrayAccountChartBars[$strRowID]['bar_positive_count'] = 1 + ArrayGetValue($arrayAccountChartBars,$strRowID,'bar_positive_count');
				}				
				if ($fAmount < 0)
				{
					$arrayAccountChartBars[$strRowID]['bar_negative_amount'] = -$fAmount + ArrayGetValue($arrayAccountChartBars,$strRowID,'bar_negative_amount');
					$arrayAccountChartBars[$strRowID]['bar_negative_count'] = 1 + ArrayGetValue($arrayAccountChartBars,$strRowID,'bar_negative_count');
				}
				
				if (is_array($arrayTypes))
				{
					$strType = ArrayGetValue($arrayTransaction,'TYPE');
					$strFraction = '';
					if (ArrayValueExists($arrayTypes,$strType) == true)
					{
						$strFraction = $strType;
					}
					else if (ArrayValueExists($arrayTypes,'*'))
					{
						$strFraction = '*';
					}
					if ($strFraction != '')
					{
						if ($fAmount > 0)
						{
							$arrayAccountChartBars[$strRowID]['bar_positive_fractions'][$strFraction]['amount'] = $fAmount + floatval(ArrayGetValue($arrayAccountChartBars,$strRowID,'bar_positive_fractions',$strFraction,'amount'));
							$arrayAccountChartBars[$strRowID]['bar_positive_fractions'][$strFraction]['count'] = 1 + floatval(ArrayGetValue($arrayAccountChartBars,$strRowID,'bar_positive_fractions',$strFraction,'count'));
						}				
						if ($fAmount < 0)
						{
							$arrayAccountChartBars[$strRowID]['bar_negative_fractions'][$strFraction]['amount'] = -$fAmount + floatval(ArrayGetValue($arrayAccountChartBars,$strRowID,'bar_negative_fractions',$strFraction,'amount'));
							$arrayAccountChartBars[$strRowID]['bar_negative_fractions'][$strFraction]['count'] = 1 + floatval(ArrayGetValue($arrayAccountChartBars,$strRowID,'bar_negative_fractions',$strFraction,'count'));
						}
					}
				}
			}
		}		
	
		
		$fBalanceBegin = floatval(ArrayGetValue($arrayAccount,'BALANCE_BEGIN'));
		
		$fCurrentBalance = $fBalanceBegin;
		foreach ($arrayAccountChartBars as &$arrayRow)
		{
			$arrayRow['line_value_left'] = $fCurrentBalance;
			$fCurrentBalance = $fCurrentBalance + $arrayRow['bar_positive_amount'] - $arrayRow['bar_negative_amount']; 
			$arrayRow['line_value_right'] = $fCurrentBalance;
			if (is_array($arrayTypes))
			{
				$arrayRow['bar_positive_fractions'] = array_values(ArrayGetValue($arrayRow,'bar_positive_fractions'));
				$arrayRow['bar_negative_fractions'] = array_values(ArrayGetValue($arrayRow,'bar_negative_fractions'));					
			}
		}
		
		$arrayAccountChartBars = array_values($arrayAccountChartBars);
		
		$arrayAccountChart['metric'] = GetCurrencySymbol(ArrayGetValue($arrayAccount,'CURRENCY'));

		$nBarCount = ArrayCount($arrayAccountChartBars);
		$nMod = 0;
		if ($nBarCount > 60)
		{
			$nMod = 12;
		}		
		else if ($nBarCount > 20)
		{
			$nMod = 6;
		}
		else if ($nBarCount > 13)
		{
			$nMod = 2;
		}

		
		if ($nMod > 0)
		{
			$nIndex = 0;
			foreach ($arrayAccountChartBars as &$arrayAccountChartBar)
			{
				if (($nIndex % $nMod) != 0)
				{
					$arrayAccountChartBar['axis_text'] = StringRepeat(' ',$nIndex); 
				}
				$nIndex++;
			}	
		}	


		$arrayAccountChart['bars'] = $arrayAccountChartBars;
		

		
		
		
		return $arrayAccountChart;
	}



	function AML_TransformAccountToAccountFlows($arrayAccount, $strDateBegin, $strDateEnd)
	{ 
		if (is_array($arrayAccount) == false)
		{
			return false;	
		}
		if (!IsValidDate($strDateBegin))
		{
			return false;	
		}
		if (!IsValidDate($strDateBegin))
		{
			return false;	
		}
		if (CompareString($strDateBegin,$strDateEnd) > 0)
		{
			return false;	
		}
	
		$arrayAccount = AML_CalculateAccount($arrayAccount, $strDateBegin, $strDateEnd);
		
		$arrayTransactions = ArrayGetValue($arrayAccount,'TRANSACTIONS');
		if (!is_array($arrayTransactions))
		{
			$arrayTransactions = array();	
		}

		//$arrayDimensions = array('Direction','Transaction Type','Geography');
		$arrayDimensions = array(
			'?TID_AML_ITEM_DIRECTION?',
			'?TID_AML_ITEM_TRANSACTIONTYPE?',
			'?TID_AML_ITEM_GEOGRAPHY?');

		$arrayData = array();
		
		$strAccountCountry = ArrayGetValue($arrayAccount,'COUNTRY');
		
		foreach ($arrayTransactions as $arrayTransaction)
		{
			$arrayDataItem = array();

			$fTransactionAmount = GetFloatValue(ArrayGetValue($arrayTransaction,'AMOUNT'));
			if ($fTransactionAmount > 0)
			{
				$arrayDataItem['?TID_AML_ITEM_DIRECTION?'] = '?TID_AML_ITEM_DEPOSITS?';
			}
			else if ($fTransactionAmount < 0)
			{
				$arrayDataItem['?TID_AML_ITEM_DIRECTION?'] = '?TID_AML_ITEM_WITHDRAWALS?';	
			}
			else
			{
				continue;	
			}
			
			$strTransactionCountry = ArrayGetValue($arrayTransaction,'COUNTRY');
			if ($strAccountCountry == $strTransactionCountry || $strTransactionCountry == '')
			{
				$arrayDataItem['?TID_AML_ITEM_GEOGRAPHY?'] = '?TID_AML_ITEM_DOMESTIC?';
			}
			else
			{
				$arrayDataItem['?TID_AML_ITEM_GEOGRAPHY?'] = '?TID_AML_ITEM_INTERNATIONAL?';
			}
			
			$strTransactionType = ArrayGetValue($arrayTransaction,'TYPE');
			if ($strTransactionType == '')
			{
				$strTransactionType = 'OTHER';
			}
			
			if (ArrayValueExists(AML_GetAllTransactionTypes(),$strTransactionType) == true)
			{
				$strTransactionType = '?TID_AML_TRANSACTIONTYPE_'.$strTransactionType.'?';	
			}
			
			$arrayDataItem['?TID_AML_ITEM_TRANSACTIONTYPE?'] = $strTransactionType;
			
			$arrayDataItem['Amount'] = abs($fTransactionAmount).'';
			
			$arrayData[] = $arrayDataItem;
		}


		$arrayAccountFlows = array();
		$arrayAccountFlows['DIMENSIONS'] = $arrayDimensions;
		$arrayAccountFlows['DATA'] = $arrayData;
	
		
		return $arrayAccountFlows;
	}


	
	function AML_GetDefaultTransactionTypes()
	{
		return array(
			'CASH',
			'DEBITCARD',
			'WIRE',
			'SALARY',
			'*');	
	}

	function AML_GetAllTransactionTypes()
	{
		return array(
			'CASH',
			'DEBITCARD',
			'DIRECTDEBIT',
			'WIRE',
			'SALARY',
			'CHARGES',
			'OTHER');	
	}

	
	function AML_GetSupportedTypologyIDs()
	{
		return array(
			'HIGHRISKGEOGRAPHIES',
			'INACTIVITY',
			'INTENSIVECASH',
			'INTERNATIONALWIRES',
			'RAPIDMOVEMENT',
			'ROUNDAMOUNTS',
			'STRUCTURING'
			);	
	}
	
	function AML_GetDefaultTypologyDefinitions($arrayTypologyIDs)
	{
		if (ArrayCount($arrayTypologyIDs) == 0)
		{
			$arrayTypologyIDs = AML_GetSupportedTypologyIDs();
		}
	
		$arrayTypologyDefinitions = array();
		if (ArrayValueExists($arrayTypologyIDs,'INACTIVITY'))
		{
			$arrayTypologyDefinitions['INACTIVITY']['THRESHOLD_DAYS'] = 21;
		}
		if (ArrayValueExists($arrayTypologyIDs,'STRUCTURING'))
		{
			$arrayIgnoreTypes = array('DIRECTDEBIT','DEBITCARD','SALARY','OTHER');
			$arrayVariant = array();
			$arrayVariant['THRESHOLD_DAYS'] = 1;
			$arrayVariant['THRESHOLD_TRANSACTION_COUNT'] = 2;
			$arrayVariant['THRESHOLD_TRANSACTION_AMOUNT_MAX']['EUR'] = 9500;
			$arrayVariant['THRESHOLD_TRANSACTION_AMOUNT_MIN']['EUR'] = 150;
			$arrayVariant['THRESHOLD_AMOUNT_TOTAL']['EUR'] = 3000;
			$arrayVariant['THRESHOLD_TRANSACTION_AMOUNT_MAX']['USD'] = 9500;
			$arrayVariant['THRESHOLD_TRANSACTION_AMOUNT_MIN']['USD'] = 150;
			$arrayVariant['THRESHOLD_AMOUNT_TOTAL']['USD'] = 3000;
			$arrayVariant['IGNORE_TYPES'] = $arrayIgnoreTypes;
			$arrayTypologyDefinitions['STRUCTURING']['VARIANTS'][] = $arrayVariant;

			$arrayVariant = array();
			$arrayVariant['THRESHOLD_DAYS'] = 7;
			$arrayVariant['THRESHOLD_TRANSACTION_COUNT'] = 2;
			$arrayVariant['THRESHOLD_TRANSACTION_AMOUNT_MAX']['EUR'] = 9500;
			$arrayVariant['THRESHOLD_TRANSACTION_AMOUNT_MIN']['EUR'] = 150;
			$arrayVariant['THRESHOLD_AMOUNT_TOTAL']['EUR'] = 5000;
			$arrayVariant['THRESHOLD_TRANSACTION_AMOUNT_MAX']['USD'] = 9500;
			$arrayVariant['THRESHOLD_TRANSACTION_AMOUNT_MIN']['USD'] = 150;
			$arrayVariant['THRESHOLD_AMOUNT_TOTAL']['USD'] = 5000;
			$arrayVariant['IGNORE_TYPES'] = $arrayIgnoreTypes;
			$arrayTypologyDefinitions['STRUCTURING']['VARIANTS'][] = $arrayVariant;
		}
		if (ArrayValueExists($arrayTypologyIDs,'INTERNATIONALWIRES'))
		{
			$arrayTypologyDefinitions['INTERNATIONALWIRES']['THRESHOLD_AMOUNT']['EUR'] = 8000;
			$arrayTypologyDefinitions['INTERNATIONALWIRES']['THRESHOLD_AMOUNT']['USD'] = 8000;
		}
		if (ArrayValueExists($arrayTypologyIDs,'RAPIDMOVEMENT'))
		{
			$arrayIgnoreTypes = array('DIRECTDEBIT','DEBITCARD','SALARY','OTHER');
			$arrayVariant = array();
			$arrayVariant['THRESHOLD_DAYS'] = 7;
			$arrayVariant['THRESHOLD_RATIO'] = 0.45;
			//$arrayVariant['THRESHOLD_TRANSACTION_AMOUNT_MAX']['EUR'] = 9500;
			$arrayVariant['THRESHOLD_TRANSACTION_AMOUNT_MIN']['EUR'] = 150;
			//$arrayVariant['THRESHOLD_TRANSACTION_AMOUNT_MAX']['USD'] = 9500;
			$arrayVariant['THRESHOLD_TRANSACTION_AMOUNT_MIN']['USD'] = 150;
			$arrayVariant['IGNORE_TYPES'] = $arrayIgnoreTypes;
			$arrayTypologyDefinitions['RAPIDMOVEMENT']['VARIANTS'][] = $arrayVariant;
		}
		if (ArrayValueExists($arrayTypologyIDs,'ROUNDAMOUNTS'))
		{
			$arrayVariant = array();
			$arrayVariant['1000000'] = 4;
			$arrayVariant['100000'] = 3;
			$arrayVariant['10000'] = 2;
			$arrayVariant['2000'] = 2;

			$arrayTypologyDefinition = array();			
			$arrayTypologyDefinition['EUR'] = $arrayVariant;
			$arrayTypologyDefinition['USD'] = $arrayVariant;
			$arrayTypologyDefinition['CHF'] = $arrayVariant;
			
			$arrayTypologyDefinitions['ROUNDAMOUNTS'] = $arrayTypologyDefinition;
		}
		if (ArrayValueExists($arrayTypologyIDs,'INTENSIVECASH'))
		{
			$arrayVariant = array();
			$arrayVariant['THRESHOLD_DAYS'] = 1;
			$arrayVariant['THRESHOLD_TRANSACTION_COUNT'] = 1;
			$arrayVariant['THRESHOLD_TRANSACTION_AMOUNT_MIN']['EUR'] = 150;
			$arrayVariant['THRESHOLD_AMOUNT_TOTAL']['EUR'] = 3000;
			$arrayVariant['THRESHOLD_TRANSACTION_AMOUNT_MIN']['USD'] = 150;
			$arrayVariant['THRESHOLD_AMOUNT_TOTAL']['USD'] = 3000;
			$arrayTypologyDefinitions['INTENSIVECASH']['VARIANTS'][] = $arrayVariant;

			$arrayVariant = array();
			$arrayVariant['THRESHOLD_DAYS'] = 7;
			$arrayVariant['THRESHOLD_TRANSACTION_COUNT'] = 3;
			$arrayVariant['THRESHOLD_TRANSACTION_AMOUNT_MAX']['EUR'] = 1500;
			$arrayVariant['THRESHOLD_TRANSACTION_AMOUNT_MIN']['EUR'] = 150;
			$arrayVariant['THRESHOLD_AMOUNT_TOTAL']['EUR'] = 1500;
			$arrayVariant['THRESHOLD_TRANSACTION_AMOUNT_MAX']['USD'] = 1500;
			$arrayVariant['THRESHOLD_TRANSACTION_AMOUNT_MIN']['USD'] = 150;
			$arrayVariant['THRESHOLD_AMOUNT_TOTAL']['USD'] = 1500;
			$arrayTypologyDefinitions['INTENSIVECASH']['VARIANTS'][] = $arrayVariant;			
		}
		if (ArrayValueExists($arrayTypologyIDs,'HIGHRISKGEOGRAPHIES'))
		{
			$arrayTypologyDefinition = array();
			$arrayTypologyDefinition['THRESHOLD']['SCORE'] = 1.0;
			$arrayTypologyDefinition['COUNTRIES']['AFG']['SCORE'] = 1.0;
			$arrayTypologyDefinition['COUNTRIES']['ETH']['SCORE'] = 1.0;
			$arrayTypologyDefinition['COUNTRIES']['DEU']['SCORE'] = 0.0;
			$arrayTypologyDefinition['COUNTRIES']['USA']['SCORE'] = 0.0;
			$arrayTypologyDefinitions['HIGHRISKGEOGRAPHIES'] = $arrayTypologyDefinition;
		}

		return $arrayTypologyDefinitions;
	}
	
	function AML_RaiseRedFlags($arrayAccount,$arrayTypologyDefinitions,$strDateStart = '', $strDateEnd = '')
	{
		$arrayRedFlags = array();
		if (ArrayKeyExists($arrayTypologyDefinitions,'INACTIVITY'))
		{
			$arrayNewRedFlags = AML_RaiseRedFlags_Inactivity($arrayAccount,$arrayTypologyDefinitions['INACTIVITY'],$strDateStart,$strDateEnd);
			if ($arrayNewRedFlags != false)
			{
				$arrayRedFlags = ArrayMerge($arrayRedFlags,$arrayNewRedFlags);
			}
		}	
		if (ArrayKeyExists($arrayTypologyDefinitions,'STRUCTURING'))
		{
			$arrayNewRedFlags = AML_RaiseRedFlags_Structuring($arrayAccount,$arrayTypologyDefinitions['STRUCTURING'],$strDateStart,$strDateEnd);
			if ($arrayNewRedFlags != false)
			{
				$arrayRedFlags = ArrayMerge($arrayRedFlags,$arrayNewRedFlags);
			}
		}
		if (ArrayKeyExists($arrayTypologyDefinitions,'INTERNATIONALWIRES'))
		{
			$arrayNewRedFlags = AML_RaiseRedFlags_InternationalWires($arrayAccount,$arrayTypologyDefinitions['INTERNATIONALWIRES'],$strDateStart,$strDateEnd);
			if ($arrayNewRedFlags != false)
			{
				$arrayRedFlags = ArrayMerge($arrayRedFlags,$arrayNewRedFlags);
			}
		}
		if (ArrayKeyExists($arrayTypologyDefinitions,'RAPIDMOVEMENT'))
		{
			$arrayNewRedFlags = AML_RaiseRedFlags_RapidMovement($arrayAccount,$arrayTypologyDefinitions['RAPIDMOVEMENT'],$strDateStart,$strDateEnd);
			if ($arrayNewRedFlags != false)
			{
				$arrayRedFlags = ArrayMerge($arrayRedFlags,$arrayNewRedFlags);
			}
		}		
		if (ArrayKeyExists($arrayTypologyDefinitions,'ROUNDAMOUNTS'))
		{
			$arrayNewRedFlags = AML_RaiseRedFlags_RoundAmounts($arrayAccount,$arrayTypologyDefinitions['ROUNDAMOUNTS'],$strDateStart,$strDateEnd);
			if ($arrayNewRedFlags != false)
			{
				$arrayRedFlags = ArrayMerge($arrayRedFlags,$arrayNewRedFlags);
			}
		}		
		if (ArrayKeyExists($arrayTypologyDefinitions,'INTENSIVECASH'))
		{
			$arrayNewRedFlags = AML_RaiseRedFlags_IntensiveCash($arrayAccount,$arrayTypologyDefinitions['INTENSIVECASH'],$strDateStart,$strDateEnd);
			if ($arrayNewRedFlags != false)
			{
				$arrayRedFlags = ArrayMerge($arrayRedFlags,$arrayNewRedFlags);
			}
		}			
		if (ArrayKeyExists($arrayTypologyDefinitions,'HIGHRISKGEOGRAPHIES'))
		{
			$arrayNewRedFlags = AML_RaiseRedFlags_HighRiskGeographies($arrayAccount,$arrayTypologyDefinitions['HIGHRISKGEOGRAPHIES'],$strDateStart,$strDateEnd);
			if ($arrayNewRedFlags != false)
			{
				$arrayRedFlags = ArrayMerge($arrayRedFlags,$arrayNewRedFlags);
			}
		}	
		
		// https://trello.com/c/9Yw99OcI/225-red-flags-should-be-sorted-by-typology
		$arrayRedFlags = ArraySort($arrayRedFlags,false,'CompareRedFlags');
		
		return $arrayRedFlags;
	}
	
	function AML_RaiseRedFlags_Inactivity($arrayAccount,$arrayDefinition,$strDateStart = '', $strDateEnd = '')
	{
		$bCalculated = GetBoolValue(ArrayGetValue($arrayAccount,'CALCULATED'));
		if (!$bCalculated)
		{
			$arrayAccount = AML_CalculateAccount($arrayAccount);
		}
		$arrayTransactions = ArrayGetValue($arrayAccount,'TRANSACTIONS');
		
		
		$strTransactionFirstDate = ArrayGetValue($arrayAccount,'TRANSACTION_FIRST_DATE');
		$strTransactionLastDate = ArrayGetValue($arrayAccount,'TRANSACTION_LAST_DATE');
		
		$nThresholdDays = intval(ArrayGetValue($arrayDefinition,'THRESHOLD_DAYS'));
		if ($nThresholdDays == 0)
		{
			$nThresholdDays = 21;
		}
		
		$arrayRedFlags = array();
		
		$nTransactions = ArrayCount($arrayTransactions);
		
		for ($nIndex = 0; $nIndex < $nTransactions; $nIndex++)
		{
			if ($nIndex < $nTransactions - 1)
			{
				$strTransactionDate1 = $arrayTransactions[$nIndex]['DATE'];
				$strTransactionDate2 = $arrayTransactions[$nIndex+1]['DATE'];
				
				$timeInactivityStart = GetTimeAddDays($strTransactionDate1,1);
				$timeInactivityEnd = GetTimeAddDays($strTransactionDate2,-1);
								
				$nInactivityDays = GetTimeAddDays($timeInactivityEnd - $timeInactivityStart,1) / 60 / 60 / 24;  
				if ($nInactivityDays >= $nThresholdDays)
				{
					$arrayRedFlag = array();
					$arrayRedFlag['TYPOLOGY'] = 'INACTIVITY';
					$arrayRedFlag['THRESHOLD_DAYS'] = $nThresholdDays;
					$arrayRedFlag['START_DATE'] = RenderDate($timeInactivityStart);
					$arrayRedFlag['END_DATE'] = RenderDate($timeInactivityEnd);	
					$arrayRedFlag['INACTIVITY_DAYS'] = GetIntegerValue($nInactivityDays);
					$arrayRedFlags[] = $arrayRedFlag;
				}
			}
		}
		return $arrayRedFlags;
	}
	

	function AML_RaiseRedFlags_Structuring($arrayAccount,$arrayDefinition,$strDateStart = '', $strDateEnd = '')
	{
		$bCalculated = GetBoolValue(ArrayGetValue($arrayAccount,'CALCULATED'));
		if (!$bCalculated)
		{
			$arrayAccount = AML_CalculateAccount($arrayAccount);
		}
		$arrayTransactions = ArrayGetValue($arrayAccount,'TRANSACTIONS');
		$nTransactions = ArrayCount($arrayTransactions);
		
		$arrayDefinitonVariants = ArrayGetValue($arrayDefinition,'VARIANTS');
		if (ArrayCount($arrayDefinitonVariants) == 0)
		{
			return false;	
		}
		
		$strAccountCurrency = ArrayGetValue($arrayAccount,'CURRENCY');
		if (IsValidCurrencyID($strAccountCurrency) == false)
		{
			return false;	
		}

		$arrayRedFlags = array();
		
		foreach ($arrayDefinitonVariants as $arrayVariant)
		{
			$fThresholdTransactionAmountMax	= GetFloatValue(ArrayGetValue($arrayVariant,'THRESHOLD_TRANSACTION_AMOUNT_MAX',$strAccountCurrency));
			if ($fThresholdTransactionAmountMax == 0)
			{
				return false;	
			}
			$fThresholdTransactionAmountMin	= GetFloatValue(ArrayGetValue($arrayVariant,'THRESHOLD_TRANSACTION_AMOUNT_MIN',$strAccountCurrency));
			if ($fThresholdTransactionAmountMax == 0)
			{
				return false;	
			}
			$fThresholdAmountTotal	= GetFloatValue(ArrayGetValue($arrayVariant,'THRESHOLD_AMOUNT_TOTAL',$strAccountCurrency));
			if ($fThresholdAmountTotal == 0)
			{
				return false;	
			}
			$nThresholdDays = GetIntegerValue(ArrayGetValue($arrayVariant,'THRESHOLD_DAYS'));
			if ($nThresholdDays == 0)
			{
				return false;	
			}
			$nThresholdTransactionCount = GetIntegerValue(ArrayGetValue($arrayVariant,'THRESHOLD_TRANSACTION_COUNT'));
			if ($nThresholdTransactionCount == 0)
			{
				return false;	
			}
			
			$arrayIgnoreTypes = ArrayGetValue($arrayVariant,'IGNORE_TYPES');
			if (!is_array($arrayIgnoreTypes))
			{
				return false;	
			}

			$arrayRedFlag = array();
			$arrayRedFlag['TYPOLOGY'] = 'STRUCTURING';
			$arrayRedFlag['THRESHOLD_DAYS'] = $nThresholdDays;
			$arrayRedFlag['THRESHOLD_TRANSACTION_COUNT'] = $nThresholdTransactionCount;

			$strPivotDate = '';

			for ($nIndexPivot = 0; $nIndexPivot < $nTransactions; $nIndexPivot++)
			{
				$arrayRedFlagTransactionsWithdrawals = array();
				$arrayRedFlagTransactionsDeposits = array();
				$strPivotTransactionDate = ArrayGetValue($arrayTransactions,$nIndexPivot,'DATE');
				if ($strPivotDate == '')
				{
					$strPivotDate = $strPivotTransactionDate;
				}
				if (CompareString($strPivotTransactionDate,$strPivotDate) >= 0)
				{
					$strThresholdDate = RenderDate(GetTimeAddDays($strPivotTransactionDate,$nThresholdDays));
					
					$fTotalAmountWithdrawals = 0.0;
					$fTotalAmountDeposits = 0.0;
					$nTransactionCountWithdrawals = 0;
					$nTransactionCountDeposits = 0;
					
					$nIndex = $nIndexPivot;
					$strTransactionDate = $strPivotTransactionDate;
					
					$bContinue = true;
					while (	($bContinue == true) &&	($nIndex < $nTransactions) )
					{
						$strTransactionDate = ArrayGetValue($arrayTransactions,$nIndex,'DATE');
						if ( CompareString($strTransactionDate,$strThresholdDate) >= 0)
						{
							$bContinue = false;
						}
						else
						{
							$strTransactionType = ArrayGetValue($arrayTransactions,$nIndex,'TYPE');
							if (ArrayValueExists($arrayIgnoreTypes,$strTransactionType) == false)
							{
							
								$fTransactionAmount = GetFloatValue(ArrayGetValue($arrayTransactions,$nIndex,'AMOUNT'));
								if ($fTransactionAmount > 0)
								{
									if (	($fTransactionAmount >= $fThresholdTransactionAmountMin)
										&&	($fTransactionAmount <= $fThresholdTransactionAmountMax))
									{
										$arrayRedFlagTransactionsDeposits[] = ArrayGetValue($arrayTransactions,$nIndex);
										$fTotalAmountDeposits += $fTransactionAmount;
										$nTransactionCountDeposits++;
									}
								}
								else if ($fTransactionAmount < 0)
								{
									if (	(abs($fTransactionAmount) >= $fThresholdTransactionAmountMin)
										&&	(abs($fTransactionAmount) <= $fThresholdTransactionAmountMax))
									{
										$arrayRedFlagTransactionsWithdrawals[] = ArrayGetValue($arrayTransactions,$nIndex);
										$fTotalAmountWithdrawals += abs($fTransactionAmount);
										$nTransactionCountWithdrawals++;
									}
								}
								if ( ($fTotalAmountDeposits >= $fThresholdAmountTotal) && ($nTransactionCountDeposits >= $nThresholdTransactionCount) )
								{
									$arrayRedFlag['START_DATE'] = $strPivotTransactionDate;
									$arrayRedFlag['END_DATE'] = $strTransactionDate;	
									$arrayRedFlag['DIRECTION'] = 'DEPOSITS';
									$arrayRedFlag['TRANSACTION_COUNT'] = $nTransactionCountDeposits;
									$arrayRedFlag['TOTAL_AMOUNT'] = $fTotalAmountDeposits;
									$arrayRedFlag['TRANSACTIONS'] = $arrayRedFlagTransactionsDeposits;
									$arrayRedFlags[] = $arrayRedFlag;
									$strPivotDate = RenderDate(GetTimeAddDays($strTransactionDate,1));
									$bContinue = false;
								}
								if ( ($fTotalAmountWithdrawals >= $fThresholdAmountTotal) && ($nTransactionCountWithdrawals >= $nThresholdTransactionCount))
								{
									$arrayRedFlag['START_DATE'] = $strPivotTransactionDate;
									$arrayRedFlag['END_DATE'] = $strTransactionDate;	
									$arrayRedFlag['DIRECTION'] = 'WITHDRAWALS';
									$arrayRedFlag['TRANSACTION_COUNT'] = $nTransactionCountWithdrawals;
									$arrayRedFlag['TOTAL_AMOUNT'] = $fTotalAmountWithdrawals;
									$arrayRedFlag['TRANSACTIONS'] = $arrayRedFlagTransactionsWithdrawals;
									$arrayRedFlags[] = $arrayRedFlag;
									$strPivotDate = RenderDate(GetTimeAddDays($strTransactionDate,1));
									$bContinue = false;	
								}
								
							}
						}
						$nIndex++;
					}
				}
			}
		}
		
		
		// Remove overlapping red flags
		
		$arrayRedFlagsFiltered = array();
		$nRedFlagCount = ArrayCount($arrayRedFlags);
		for ($nIndexPivot = 0; $nIndexPivot < $nRedFlagCount; $nIndexPivot++)
		{
			$bKeep = true;
			$strStartDatePivot = ArrayGetValue($arrayRedFlags,$nIndexPivot,'START_DATE');
			$strEndDatePivot = ArrayGetValue($arrayRedFlags,$nIndexPivot,'END_DATE');
			$strDirectionPivot =  ArrayGetValue($arrayRedFlags,$nIndexPivot,'DIRECTION');
			$nThresholdDaysPivot = ArrayGetValue($arrayRedFlags,$nIndexPivot,'THRESHOLD_DAYS');
			
			for ($nIndexSecond = 0; $nIndexSecond < $nRedFlagCount; $nIndexSecond++)
			{
				if ($nIndexPivot != $nIndexSecond)
				{
					$strDirectionSecond =  ArrayGetValue($arrayRedFlags,$nIndexSecond,'DIRECTION');
					if ($strDirectionPivot == $strDirectionSecond)
					{
						$strStartDateSecond = ArrayGetValue($arrayRedFlags,$nIndexSecond,'START_DATE');
						$strEndDateSecond = ArrayGetValue($arrayRedFlags,$nIndexSecond,'END_DATE');
						if (
								!(
									($strEndDatePivot < $strStartDateSecond)
									||
									($strEndDateSecond < $strStartDatePivot)
								)
							)
						{
							// there IS an overlap
							
							$nThresholdDaysSecond = ArrayGetValue($arrayRedFlags,$nIndexSecond,'THRESHOLD_DAYS');
							if ($nThresholdDaysPivot < $nThresholdDaysSecond)
							{
								$bKeep = false;
								continue;
							}
						}
					}
				}					
			}
			if ($bKeep == true)
			{
				$arrayRedFlagsFiltered[] = $arrayRedFlags[$nIndexPivot];
			}
		}

		return $arrayRedFlagsFiltered;
	}


	function AML_RaiseRedFlags_InternationalWires($arrayAccount,$arrayDefinition,$strDateStart = '', $strDateEnd = '')
	{
		$arrayTransactions = ArrayGetValue($arrayAccount,'TRANSACTIONS');
		
		$strAccountCurrency = ArrayGetValue($arrayAccount,'CURRENCY');
		if (IsValidCurrencyID($strAccountCurrency) == false)
		{
			return false;	
		}

		$strAccountCountry = ArrayGetValue($arrayAccount,'COUNTRY');
		if (IsValidCountryID($strAccountCountry) == false)
		{
			return false;	
		}

		
		$fThresholdAmount	= GetFloatValue(ArrayGetValue($arrayDefinition,'THRESHOLD_AMOUNT',$strAccountCurrency));
		if ($fThresholdAmount == 0)
		{
			return false;	
		}
		
		$arrayRedFlags = array();
		
		foreach ($arrayTransactions as $arrayTransaction)
		{
			$strTransactionCountry = ArrayGetValue($arrayTransaction,'COUNTRY');

			// Only of country information is given...
			if ($strTransactionCountry != '')
			{
				// Only if the transaction counterpart is in a different country 
				if ($strTransactionCountry != $strAccountCountry)
				{
					$strTransactionType = ArrayGetValue($arrayTransaction,'TYPE');
					
					// Only wires
					if ($strTransactionType == 'WIRE')
					{
						$fTransactionAmount = GetFloatValue(ArrayGetValue($arrayTransaction,'AMOUNT'));
						if (abs($fTransactionAmount) >= $fThresholdAmount)
						{
							$arrayRedFlag = array();
							$arrayRedFlag['TYPOLOGY'] = 'INTERNATIONALWIRES';
							$arrayRedFlag['THRESHOLD_AMOUNT'] = $fThresholdAmount;
							$arrayRedFlag['START_DATE'] = ArrayGetValue($arrayTransaction,'DATE');
							$arrayRedFlag['END_DATE'] = ArrayGetValue($arrayTransaction,'DATE');
							$arrayRedFlag['AMOUNT'] = GetFloatValue(abs($fTransactionAmount));
							$arrayRedFlag['DIRECTION'] = ($fTransactionAmount > 0)?('DEPOSITS'):('WITHDRAWALS');
							$arrayRedFlag['COUNTRY'] = $strTransactionCountry;
							$arrayRedFlag['TRANSACTIONS'][] = $arrayTransaction;
							$arrayRedFlags[] = $arrayRedFlag;
						}
					}
				}
			}
		}
		return $arrayRedFlags;
	}

	function AML_RaiseRedFlags_RapidMovement($arrayAccount,$arrayDefinition,$strDateStart = '', $strDateEnd = '')
	{
		$bCalculated = GetBoolValue(ArrayGetValue($arrayAccount,'CALCULATED'));
		if (!$bCalculated)
		{
			$arrayAccount = AML_CalculateAccount($arrayAccount);
		}
		$arrayTransactions = ArrayGetValue($arrayAccount,'TRANSACTIONS');
		$nTransactions = ArrayCount($arrayTransactions);
		
		$arrayDefinitonVariants = ArrayGetValue($arrayDefinition,'VARIANTS');
		if (ArrayCount($arrayDefinitonVariants) == 0)
		{
			return false;	
		}
		
		$strAccountCurrency = ArrayGetValue($arrayAccount,'CURRENCY');
		if (IsValidCurrencyID($strAccountCurrency) == false)
		{
			return false;	
		}

		$arrayRedFlags = array();
		
		foreach ($arrayDefinitonVariants as $arrayVariant)
		{
			$fThresholdRatio	= GetFloatValue(ArrayGetValue($arrayVariant,'THRESHOLD_RATIO'));
			if ($fThresholdRatio < 0.0)
			{
				return false;	
			}
			if ($fThresholdRatio > 1.0)
			{
				return false;	
			}
			$nThresholdDays = GetIntegerValue(ArrayGetValue($arrayVariant,'THRESHOLD_DAYS'));
			if ($nThresholdDays == 0)
			{
				return false;	
			}
			$arrayIgnoreTypes = ArrayGetValue($arrayVariant,'IGNORE_TYPES');
			if (!is_array($arrayIgnoreTypes))
			{
				return false;	
			}			
			
			$fThresholdTransactionAmountMin = GetFloatValue(ArrayGetValue($arrayVariant,'THRESHOLD_TRANSACTION_AMOUNT_MIN',$strAccountCurrency));

			$arrayRedFlag = array();
			$arrayRedFlag['TYPOLOGY'] = 'RAPIDMOVEMENT';
			$arrayRedFlag['THRESHOLD_DAYS'] = $nThresholdDays;
			$arrayRedFlag['THRESHOLD_RATIO'] = $fThresholdRatio;

			$strPivotDate = '';

			for ($nIndexPivot = 0; $nIndexPivot < $nTransactions; $nIndexPivot++)
			{
				$arrayRedFlagTransactions = array();
				$strPivotTransactionDate = ArrayGetValue($arrayTransactions,$nIndexPivot,'DATE');
				if ($strPivotDate == '')
				{
					$strPivotDate = $strPivotTransactionDate;
				}
				if (CompareString($strPivotTransactionDate,$strPivotDate) >= 0)
				{
					$strThresholdDate = RenderDate(GetTimeAddDays($strPivotTransactionDate,$nThresholdDays));
					
					$fTotalAmountWithdrawals = 0.0;
					$fTotalAmountDeposits = 0.0;
					
					$nWithdrawalsCount = 0;
					$nDepositsCount = 0;
					
					$nIndex = $nIndexPivot;
					$strTransactionDate = $strPivotTransactionDate;
					
					$bContinue = true;
					$strTransactionDatePrevious = '';
					while (	($bContinue == true) &&	($nIndex < $nTransactions) )
					{
						$strTransactionDate = ArrayGetValue($arrayTransactions,$nIndex,'DATE');
						if ( CompareString($strTransactionDate,$strThresholdDate) >= 0)
						{
							$bContinue = false;
						}
						else
						{
							$strTransactionDatePrevious = $strTransactionDate;	

							$strTransactionType = ArrayGetValue($arrayTransactions,$nIndex,'TYPE');
							if (ArrayValueExists($arrayIgnoreTypes,$strTransactionType) == false)
							{
								$fTransactionAmount = GetFloatValue(ArrayGetValue($arrayTransactions,$nIndex,'AMOUNT'));
								if (abs($fTransactionAmount) >= $fThresholdTransactionAmountMin)
								{
									$arrayRedFlagTransactions[] = ArrayGetValue($arrayTransactions,$nIndex);
									if ($fTransactionAmount > 0)
									{
											$fTotalAmountDeposits += $fTransactionAmount;
											$nDepositsCount++;

									}
									else if ($fTransactionAmount < 0)
									{
											$fTotalAmountWithdrawals += abs($fTransactionAmount);
											$nWithdrawalsCount++;
									}
								}
							}
						}
						$nIndex++;
					}
					
					
					$fAmountHigher = max($fTotalAmountDeposits,$fTotalAmountWithdrawals);
					$fAmountLower = min($fTotalAmountDeposits,$fTotalAmountWithdrawals);
					
					if ($fAmountHigher > 0 && $fAmountLower > 0)
					{
						$fRatio = $fAmountLower / $fAmountHigher;
						if ($fRatio >= $fThresholdRatio)
						{
							$arrayRedFlag['START_DATE'] = $strPivotTransactionDate;
							$arrayRedFlag['END_DATE'] = $strTransactionDatePrevious;
							$arrayRedFlag['RATIO'] = $fRatio;
							$arrayRedFlag['DEPOSITS_AMOUNT'] = $fTotalAmountDeposits;
							$arrayRedFlag['DEPOSITS_COUNT'] = $nDepositsCount;
							$arrayRedFlag['WITHDRAWALS_AMOUNT'] = $fTotalAmountWithdrawals;
							$arrayRedFlag['WITHDRAWALS_COUNT'] = $nWithdrawalsCount;
							$arrayRedFlag['NETFLOW'] = $fTotalAmountDeposits - $fTotalAmountWithdrawals;
							$arrayRedFlag['TRANSACTIONS'] = $arrayRedFlagTransactions;
							$arrayRedFlags[] = $arrayRedFlag;
							//$strPivotDate = RenderDate(GetTimeAddDays($strTransactionDate,1));
							$strPivotDate = RenderDate(GetTimeAddDays($strTransactionDatePrevious,1));

						}
					}
				}
			}
		}

		// Remove overlapping red flags
		
		$arrayRedFlagsFiltered = array();
		$nRedFlagCount = ArrayCount($arrayRedFlags);
		for ($nIndexPivot = 0; $nIndexPivot < $nRedFlagCount; $nIndexPivot++)
		{
			$bKeep = true;
			$strStartDatePivot = ArrayGetValue($arrayRedFlags,$nIndexPivot,'START_DATE');
			$strEndDatePivot = ArrayGetValue($arrayRedFlags,$nIndexPivot,'END_DATE');
			$strDirectionPivot =  ArrayGetValue($arrayRedFlags,$nIndexPivot,'DIRECTION');
			$nThresholdDaysPivot = ArrayGetValue($arrayRedFlags,$nIndexPivot,'THRESHOLD_DAYS');
			
			for ($nIndexSecond = 0; $nIndexSecond < $nRedFlagCount; $nIndexSecond++)
			{
				if ($nIndexPivot != $nIndexSecond)
				{
					$strStartDateSecond = ArrayGetValue($arrayRedFlags,$nIndexSecond,'START_DATE');
					$strEndDateSecond = ArrayGetValue($arrayRedFlags,$nIndexSecond,'END_DATE');
					if (
							!(
								($strEndDatePivot < $strStartDateSecond)
								||
								($strEndDateSecond < $strStartDatePivot)
							)
						)
					{
						// there is an overlap
						
						$nThresholdDaysSecond = ArrayGetValue($arrayRedFlags,$nIndexSecond,'THRESHOLD_DAYS');
						if ($nThresholdDaysPivot < $nThresholdDaysSecond)
						{
							$bKeep = false;
							continue;
						}
					}
				}					
			}
			if ($bKeep == true)
			{
				$arrayRedFlagsFiltered[] = $arrayRedFlags[$nIndexPivot];
			}
		}

		return $arrayRedFlagsFiltered;
	}

	function AML_IsRoundAmount($fAmount,$arrayDefinition)
	{
		if (ArrayCount($arrayDefinition) == 0)
		{
			return false;	
		}
		$fAmount = abs($fAmount);
		if ($fAmount == 0)
		{
			return false;	
		}
		foreach ($arrayDefinition as $strThreshold => $vDigits)
		{
			$nThreshold = GetIntegerValue($strThreshold);
			$nDigits = GetIntegerValue($vDigits);
			if ($nThreshold > 0 && $nDigits > 0)
			{
				if ($fAmount >= $nThreshold)
				{
					$fRounded = round($fAmount,-$nDigits);
					if ($fRounded == $fAmount)
					{
						return true;	
					}
				}
			}		
		}
		return false;
	}

	function AML_RaiseRedFlags_RoundAmounts($arrayAccount,$arrayDefinition,$strDateStart = '', $strDateEnd = '')
	{
		$arrayTransactions = ArrayGetValue($arrayAccount,'TRANSACTIONS');
		
		$strAccountCurrency = ArrayGetValue($arrayAccount,'CURRENCY');
		if (IsValidCurrencyID($strAccountCurrency) == false)
		{
			return false;	
		}
		
		$arrayCurrencyDefinitions	= ArrayGetValue($arrayDefinition,$strAccountCurrency);
		if (ArrayCount($arrayCurrencyDefinitions) == 0)
		{
			return false;	
		}
		
		$arrayRedFlags = array();
		
		foreach ($arrayTransactions as $arrayTransaction)
		{
			$fTransactionAmount = GetFloatValue(ArrayGetValue($arrayTransaction,'AMOUNT'));
			if (AML_IsRoundAmount($fTransactionAmount,$arrayCurrencyDefinitions) == true)
			{
				$arrayRedFlag = array();
				$arrayRedFlag['TYPOLOGY'] = 'ROUNDAMOUNTS';
				$arrayRedFlag['START_DATE'] = ArrayGetValue($arrayTransaction,'DATE');
				$arrayRedFlag['END_DATE'] = ArrayGetValue($arrayTransaction,'DATE');
				$arrayRedFlag['AMOUNT'] = GetFloatValue(abs($fTransactionAmount));
				$arrayRedFlag['DIRECTION'] = ($fTransactionAmount > 0)?('DEPOSITS'):('WITHDRAWALS');
				$arrayRedFlag['TRANSACTIONS'][] = $arrayTransaction;
				$arrayRedFlags[] = $arrayRedFlag;
			}
		}
		return $arrayRedFlags;
	}




	function AML_RaiseRedFlags_IntensiveCash($arrayAccount,$arrayDefinition,$strDateStart = '', $strDateEnd = '')
	{
		$bCalculated = GetBoolValue(ArrayGetValue($arrayAccount,'CALCULATED'));
		if (!$bCalculated)
		{
			$arrayAccount = AML_CalculateAccount($arrayAccount);
		}
		$arrayTransactions = ArrayGetValue($arrayAccount,'TRANSACTIONS');
		$nTransactions = ArrayCount($arrayTransactions);

		$strAccountCurrency = ArrayGetValue($arrayAccount,'CURRENCY');
		if (IsValidCurrencyID($strAccountCurrency) == false)
		{
			return false;	
		}

		$arrayDefinitonVariants = ArrayGetValue($arrayDefinition,'VARIANTS');
		if (ArrayCount($arrayDefinitonVariants) == 0)
		{
			return false;	
		}
		
		$arrayRedFlags = array();

		
		foreach ($arrayDefinitonVariants as $arrayVariant)
		{
			$fThresholdAmountTotal	= GetFloatValue(ArrayGetValue($arrayVariant,'THRESHOLD_AMOUNT_TOTAL',$strAccountCurrency));
			if ($fThresholdAmountTotal == 0)
			{
				return false;	
			}
			$nThresholdDays = GetIntegerValue(ArrayGetValue($arrayVariant,'THRESHOLD_DAYS'));
			if ($nThresholdDays == 0)
			{
				return false;	
			}
			$nThresholdTransactionCount = GetIntegerValue(ArrayGetValue($arrayVariant,'THRESHOLD_TRANSACTION_COUNT'));
			if ($nThresholdTransactionCount == 0)
			{
				return false;
			}
			
			$fThresholdTransactionAmountMax = GetFloatValue(ArrayGetValue($arrayVariant,'THRESHOLD_TRANSACTION_AMOUNT_MAX',$strAccountCurrency));
			$fThresholdTransactionAmountMin = GetFloatValue(ArrayGetValue($arrayVariant,'THRESHOLD_TRANSACTION_AMOUNT_MIN',$strAccountCurrency));
			if ($fThresholdTransactionAmountMax > 0)
			{
				if ($fThresholdTransactionAmountMax < $fThresholdTransactionAmountMin)
				{
					return false;	
				}
			}

			$arrayRedFlag = array();
			$arrayRedFlag['TYPOLOGY'] = 'INTENSIVECASH';
			$arrayRedFlag['THRESHOLD_DAYS'] = $nThresholdDays;
			$arrayRedFlag['THRESHOLD_AMOUNT'] = $fThresholdAmountTotal;

			$strPivotDate = '';

			for ($nIndexPivot = 0; $nIndexPivot < $nTransactions; $nIndexPivot++)
			{
				$arrayRedFlagTransactionsWithdrawals = array();
				$arrayRedFlagTransactionsDeposits = array();
				$strPivotTransactionDate = ArrayGetValue($arrayTransactions,$nIndexPivot,'DATE');
				if ($strPivotDate == '')
				{
					$strPivotDate = $strPivotTransactionDate;
				}
				if (CompareString($strPivotTransactionDate,$strPivotDate) >= 0)
				{
					$strThresholdDate = RenderDate(GetTimeAddDays($strPivotTransactionDate,$nThresholdDays));
					
					$fTotalAmountWithdrawals = 0.0;
					$fTotalAmountDeposits = 0.0;
					$nTransactionCountWithdrawals = 0;
					$nTransactionCountDeposits = 0;

					$nIndex = $nIndexPivot;
					$strTransactionDate = $strPivotTransactionDate;
					
					$bContinue = true;
					while (	($bContinue == true) &&	($nIndex < $nTransactions) )
					{
						$strTransactionDate = ArrayGetValue($arrayTransactions,$nIndex,'DATE');
						if ( CompareString($strTransactionDate,$strThresholdDate) >= 0)
						{
							$bContinue = false;
						}
						else
						{
							$fTransactionAmount = GetFloatValue(ArrayGetValue($arrayTransactions,$nIndex,'AMOUNT'));
							$strTransactionType = ArrayGetValue($arrayTransactions,$nIndex,'TYPE');
							if ($strTransactionType == 'CASH')
							{
								if (abs($fTransactionAmount) >= $fThresholdTransactionAmountMin)
								{
									if	(
												($fThresholdTransactionAmountMax == 0)
												||
												(abs($fTransactionAmount) <= $fThresholdTransactionAmountMax)
											)
									{
										if ($fTransactionAmount > 0)
										{
											$arrayRedFlagTransactionsDeposits[] = ArrayGetValue($arrayTransactions,$nIndex);
											$fTotalAmountDeposits += $fTransactionAmount;
											$nTransactionCountDeposits++;
										}
										else if ($fTransactionAmount < 0)
										{
											$arrayRedFlagTransactionsWithdrawals[] = ArrayGetValue($arrayTransactions,$nIndex);
											$fTotalAmountWithdrawals += abs($fTransactionAmount);
											$nTransactionCountWithdrawals++;
										}
									}
								}
								if ( 		($fTotalAmountDeposits >= $fThresholdAmountTotal)
										&& 	($nTransactionCountDeposits >= $nThresholdTransactionCount))
								{
									$arrayRedFlag['START_DATE'] = $strPivotTransactionDate;
									$arrayRedFlag['END_DATE'] = $strTransactionDate;	
									$arrayRedFlag['DIRECTION'] = 'DEPOSITS';
									$arrayRedFlag['TRANSACTION_COUNT'] = $nTransactionCountDeposits;
									$arrayRedFlag['TOTAL_AMOUNT'] = $fTotalAmountDeposits;
									$arrayRedFlag['TRANSACTIONS'] = $arrayRedFlagTransactionsDeposits;
									$arrayRedFlags[] = $arrayRedFlag;
									$strPivotDate = RenderDate(GetTimeAddDays($strTransactionDate,1));
									$bContinue = false;
								}
								if (		($fTotalAmountWithdrawals >= $fThresholdAmountTotal)
										&&	($nTransactionCountWithdrawals >= $nThresholdTransactionCount))
								{
									$arrayRedFlag['START_DATE'] = $strPivotTransactionDate;
									$arrayRedFlag['END_DATE'] = $strTransactionDate;	
									$arrayRedFlag['DIRECTION'] = 'WITHDRAWALS';
									$arrayRedFlag['TRANSACTION_COUNT'] = $nTransactionCountWithdrawals;
									$arrayRedFlag['TOTAL_AMOUNT'] = $fTotalAmountWithdrawals;
									$arrayRedFlag['TRANSACTIONS'] = $arrayRedFlagTransactionsWithdrawals;
									$arrayRedFlags[] = $arrayRedFlag;
									$strPivotDate = RenderDate(GetTimeAddDays($strTransactionDate,1));
									$bContinue = false;
								}
							}
						}
						$nIndex++;
					}
				}
			}
		}

		// Remove overlapping red flags
		
		$arrayRedFlagsFiltered = array();
		$nRedFlagCount = ArrayCount($arrayRedFlags);
		for ($nIndexPivot = 0; $nIndexPivot < $nRedFlagCount; $nIndexPivot++)
		{
			$bKeep = true;
			$strStartDatePivot = ArrayGetValue($arrayRedFlags,$nIndexPivot,'START_DATE');
			$strEndDatePivot = ArrayGetValue($arrayRedFlags,$nIndexPivot,'END_DATE');
			$strDirectionPivot =  ArrayGetValue($arrayRedFlags,$nIndexPivot,'DIRECTION');
			$nThresholdDaysPivot = ArrayGetValue($arrayRedFlags,$nIndexPivot,'THRESHOLD_DAYS');
			
			for ($nIndexSecond = 0; $nIndexSecond < $nRedFlagCount; $nIndexSecond++)
			{
				if ($nIndexPivot != $nIndexSecond)
				{
					$strStartDateSecond = ArrayGetValue($arrayRedFlags,$nIndexSecond,'START_DATE');
					$strEndDateSecond = ArrayGetValue($arrayRedFlags,$nIndexSecond,'END_DATE');
					if (
							!(
								($strEndDatePivot < $strStartDateSecond)
								||
								($strEndDateSecond < $strStartDatePivot)
							)
						)
					{
						// there IS an overlap
						
						$nThresholdDaysSecond = ArrayGetValue($arrayRedFlags,$nIndexSecond,'THRESHOLD_DAYS');
						if ($nThresholdDaysPivot < $nThresholdDaysSecond)
						{
							$bKeep = false;
							continue;
						}
					}
				}					
			}
			if ($bKeep == true)
			{
				$arrayRedFlagsFiltered[] = $arrayRedFlags[$nIndexPivot];
			}
		}

		return $arrayRedFlagsFiltered;   
	}

	function AML_RaiseRedFlags_HighRiskGeographies($arrayAccount,$arrayDefinition,$strDateStart = '', $strDateEnd = '')
	{
		$arrayTransactions = ArrayGetValue($arrayAccount,'TRANSACTIONS');
		
		$strAccountCountry = ArrayGetValue($arrayAccount,'COUNTRY');
		if ($strAccountCountry != '')
		{
			if (IsValidCountryID($strAccountCountry) == false)
			{
				return false;	
			}
		}

		$fThresholdScore	= GetFloatValue(ArrayGetValue($arrayDefinition,'THRESHOLD','SCORE'));
		if ($fThresholdScore < 0 || $fThresholdScore > 1.0)
		{
			return false;	
		}
		
		$arrayCountries = ArrayGetValue($arrayDefinition,'COUNTRIES');
		if (ArrayCount($arrayCountries) == 0)
		{
			return false;
		}
		
		$arrayRedFlags = array();
		
		if (ArrayCount($arrayTransactions) == 0)
		{
			return $arrayRedFlags;
		}
		
		foreach ($arrayTransactions as $arrayTransaction)
		{
			$strTransactionCountry = ArrayGetValue($arrayTransaction,'COUNTRY');

			// Only of country information is given...
			if ($strTransactionCountry != '')
			{
				// Only if the transaction counterpart is in a different country 
				if ($strTransactionCountry != $strAccountCountry)
				{
					$fTransactionAmount = GetFloatValue(ArrayGetValue($arrayTransaction,'AMOUNT'));
					$fScore = GetFloatValue(ArrayGetValue($arrayCountries,$strTransactionCountry,'SCORE'));
					if ($fScore >= $fThresholdScore)
					{
						$arrayRedFlag = array();
						$arrayRedFlag['TYPOLOGY'] = 'HIGHRISKGEOGRAPHIES';
						$arrayRedFlag['COUNTRY'] = $strTransactionCountry;
						$arrayRedFlag['SCORE'] = $fScore;
						$arrayRedFlag['THRESHOLD_SCORE'] = $fThresholdScore;
						$arrayRedFlag['START_DATE'] = ArrayGetValue($arrayTransaction,'DATE');
						$arrayRedFlag['END_DATE'] = ArrayGetValue($arrayTransaction,'DATE');
						$arrayRedFlag['AMOUNT'] = GetFloatValue(abs($fTransactionAmount));
						$arrayRedFlag['DIRECTION'] = ($fTransactionAmount > 0)?('DEPOSITS'):('WITHDRAWALS');
						$arrayRedFlag['TRANSACTIONS'][] = $arrayTransaction;
						$arrayRedFlags[] = $arrayRedFlag;
					}
				}
			}
		}
		return $arrayRedFlags;
	}
	
	
	function AML_ReadFileAccount($strFile)
	{
		$arrayContents = FileReadCSV(
				$strFile,
				true,
				false,
				';',
				array(),
				''',
				'''');
				
		if ($arrayContents == false)
		{
			return false;	
		}

		$arrayTransactions = array();
		$arrayAccount = array();
		foreach ($arrayContents as $arrayRow)
		{
			$strType = ArrayGetValue($arrayRow,'TYPE');
			$strDate = ArrayGetValue($arrayRow,'DATE');
			
			if ($strDate == '')
			{
				if ($strType == 'ID')
				{
					$arrayAccount['ID'] = ArrayGetValue($arrayRow,'DESCRIPTION');
				}
				else if ($strType == 'BALANCE_BEGIN')
				{
					$arrayAccount['BALANCE_BEGIN'] = GetFloatValue(ArrayGetValue($arrayRow,'DESCRIPTION'));
				}	
				else if ($strType == 'CURRENCY')
				{
					$arrayAccount['CURRENCY'] = ArrayGetValue($arrayRow,'DESCRIPTION');
				}
				else if ($strType == 'COUNTRY')
				{
					$arrayAccount['COUNTRY'] = ArrayGetValue($arrayRow,'DESCRIPTION');
				}
			}
			else
			{
				$arrayTransactions[] = $arrayRow;	
			}
		}
		$arrayAccount['TRANSACTIONS'] = array();
		foreach ($arrayTransactions as $arrayTransaction)
		{
			$arrayTransaction['AMOUNT'] = GetFloatValue(ArrayGetValue($arrayTransaction,'AMOUNT'));
			$arrayTransaction['DATE'] = ConvertToDate(ArrayGetValue($arrayTransaction,'DATE'));
			
			$arrayAccount['TRANSACTIONS'][] = $arrayTransaction;
		}
		return $arrayAccount;
	}


	function AML_GetRiskCountries()
	{
		$strPath = GetDocumentRootDir();
		$strPath .= '/_source/assemblies/aml/data/risks/countries.csv';
		$arrayRiskCountries = AML_ReadFileRiskCountries($strPath);
		return $arrayRiskCountries;
	}


	function AML_ReadFileRiskCountries($strFile) 
	{
		$arrayContents = FileReadCSV(
				$strFile,
				true,
				false,
				';',
				array(),
				''',
				'''');
				
		if ($arrayContents == false)
		{
			return false;	
		}

		$arrayCountries = array();
		
		foreach ($arrayContents as $arrayRow)
		{
			$strCountryID = ArrayGetValue($arrayRow,'COUNTRY');
			$fScore = GetFloatValue(ArrayGetValue($arrayRow,'SCORE'));
			//$strDate = ArrayGetValue($arrayRow,'DATE');
			if (IsValidCountryID($strCountryID) == true)
			{
				if ($fScore >= 0.0 && $fScore <= 1.0)
				{
					$arrayCountries[$strCountryID]['SCORE'] = $fScore;
				}
			}
		}
		return $arrayCountries;
	}


	