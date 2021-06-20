<?php

	require_once(GetWDKDir().'wdk_module.inc');
	
	class CAccountAnalysisModule extends CModule
	{
		
		private $m_arrayAccount;
		private $m_arrayRiskCountries;
		private $m_arrayTypologyIDs;
		private $m_arrayTypologyDefinitions;
		private $m_accountchartElement;
		private $m_amlredflagsElement;
		private $m_accountchartlegendElement;
	
		function __construct(
			CWebSite $website,
			$strInstanceID = '',
			$arrayConfig = array())
		{
			parent::__construct(
				$website,
				'aml/accountanalysis',
				$strInstanceID,
				$arrayConfig);
		}
		
		function OnInit()
		{
			$this->m_arrayErrors = array();
	
			$this->m_arrayRiskCountries = AML_GetRiskCountries();
			
			$this->m_arrayTypologyIDs = AML_GetSupportedTypologyIDs();
			$this->m_arrayTypologyDefinitions = AML_GetDefaultTypologyDefinitions($this->m_arrayTypologyIDs);
			if (ArrayCount($this->m_arrayRiskCountries) > 0)
			{
				$this->m_arrayTypologyDefinitions['HIGHRISKGEOGRAPHIES']['COUNTRIES'] = $this->m_arrayRiskCountries;
			}
			
			$this->GetWebsite()->IncludeElement('amlaccountchart');
			$this->m_accountchartElement = new CAMLAccountChartElement($this->GetWebsite(),'default');

			$this->GetWebsite()->IncludeElement('parallelsets');
			$this->m_parallelsetsElement = new CParallelSetsElement($this->GetWebsite(),'default');
			
			$this->GetWebsite()->IncludeElement('amlaccountchartlegend');
			$this->m_accountchartlegendElement = new CAMLAccountChartLegendElement($this->GetWebsite(),'default');

			$this->GetWebsite()->IncludeElement('amlredflagslegend');
			$this->m_amlredflagsElement = new CAMLRedFlagsLegendElement($this->GetWebsite(),'default');
			
			return parent::OnInit();
		}
		
		
		function SetAccount($arrayAccount)
		{
			$this->m_arrayAccount = $arrayAccount;	
		}
		
		function IsDemo()
		{
			return $this->GetModuleConfigBoolean('demo');	
		}
	
		function OnRenderModule($strState)
		{
/*			if ($this->IsDemo() == true)
			{
				return $this->StateChart();
			}
			*/
			$this->Trace('OnRenderModule('.$strState.')');
			switch ($strState)
			{
				case 'data':
					return $this->StateData();
				case 'redflags':
					return $this->StateRedFlags();
				case 'flows':
					return $this->StateFlows();
				case '':
				default:
					$this->SetState('chart');
					return $this->StateChart();
			}
		}

		function RenderTabNavigation()
		{
			if ($this->IsDemo() == true)
			{
				$this->SetOutputItem('TABNAV','');
   			return;
			}


			$this->IncludeElement('tabnavigation');

			$tabnavigationDef = new CTabNavigationDefinition();
   
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = 'chart';
   		$tabnavigationDef->AddTab(
	      'chart',
      	'?TID_ACCOUNTANALYSIS_TAB_CHART?',
      	$this->GetWebsite()->MakeURL($arrayParams));

			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = 'flows';
   		$tabnavigationDef->AddTab(
	      'flows',
      	'?TID_ACCOUNTANALYSIS_TAB_FLOWS?',
      	$this->GetWebsite()->MakeURL($arrayParams));

			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = 'redflags';
   		$tabnavigationDef->AddTab(
	      'redflags',
      	'?TID_ACCOUNTANALYSIS_TAB_REDFLAGS?',
      	$this->GetWebsite()->MakeURL($arrayParams));

			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = 'data';
   		$tabnavigationDef->AddTab(
	      'data',
      	'?TID_ACCOUNTANALYSIS_TAB_DATA?',
      	$this->GetWebsite()->MakeURL($arrayParams));

			$tabnavigation = new CTabNavigationElement(
			   $this->GetWebsite(),
			   'default');
			   
			$this->SetOutputItem(
				'TABNAV',
				$tabnavigation->RenderTabNavigation(
   				$tabnavigationDef,
   				$this->GetState()));	
		}
		
		function RenderTitle($strID)
		{
			if ($this->IsDemo() == true)
			{
				$this->SetOutputItem('TITLE','');
			}
			else
			{
				$this->SetOutputItem('TITLE',$strID);
			}
		}


		
		function StateChart()
		{
			$this->SetState('chart');
			$this->RenderTabNavigation();
	
			$arrayAccount = $this->m_arrayAccount;
			
	
			$strID = ArrayGetValue($arrayAccount,'ID');
			$strUniqueID = StringFilter($strID,CHARSET_ALPHANUMERIC).'_'.MakeNumericToken(5);
					
			$arrayAccount = AML_CalculateAccount($arrayAccount);
			$this->Trace($strID.' account:');
			$this->Trace($arrayAccount);
			$strTransactionFirstDate = ArrayGetValue($arrayAccount,'TRANSACTION_FIRST_DATE');
			$strTransactionLastDate = ArrayGetValue($arrayAccount,'TRANSACTION_LAST_DATE');
			$strCurrency = ArrayGetValue($arrayAccount,'CURRENCY');
			$strCurrencySymbol = GetCurrencySymbol($strCurrency);
	
	
			$strTimePeriod = AML_GetAccountChartTimePeriod($strTransactionFirstDate,$strTransactionLastDate,14);
			
			$arrayAccountChart = AML_TransformAccountToAccountChart($arrayAccount,$strTransactionFirstDate,$strTransactionLastDate,$strTimePeriod,AML_GetDefaultTransactionTypes()); 


			$this->RenderTitle($strID);
			

			$arrayRedFlags = AML_RaiseRedFLags($arrayAccount,$this->m_arrayTypologyDefinitions);
			
			$this->Trace($strID.' red flags:');
			$this->Trace($arrayRedFlags);
			$arrayDetectedTypologyIDs = array();

			if (is_array($arrayRedFlags) == true)
			{
				foreach ($arrayRedFlags as $arrayRedFlag)
				{
					$arrayDetectedTypologyIDs[] = ArrayGetValue($arrayRedFlag,'TYPOLOGY');
				}
			}
			$arrayDetectedTypologyIDs = ArrayUnique($arrayDetectedTypologyIDs);
			$this->SetOutputItem(
				'REDFLAGS',
				$this->m_amlredflagsElement->RenderAMLRedFlagsLegend($this->m_arrayTypologyIDs,$arrayDetectedTypologyIDs));
			
			
			$this->SetOutputItem(
				'CHART',
				$this->m_accountchartElement->RenderAMLAccountChart($arrayAccountChart));


			$arraySettings = GetLocaleSettings('USA','EN');


			$arrayTable = array();
			$arrayHeaders = array();
			$arrayCellStyle = array();

			$arrayTable[] = array('?TID_AML_ITEM_FROM?',$strTransactionFirstDate);
			$arrayCellStyle[] = array('left','right');
			$arrayHeaders[] = array(true,false);

			$arrayTable[] = array('?TID_AML_ITEM_TO?',$strTransactionLastDate);
			$arrayCellStyle[] = array('left','right');
			$arrayHeaders[] = array(true,false);

			
			$arrayTable[] = array(
				'?TID_AML_ITEM_DEPOSITS?',
				RenderNumber(GetFloatValue(ArrayGetValue($arrayAccount,'TRANSACTION_AMOUNT_IN')),2,$strCurrencySymbol,$arraySettings));
			$arrayCellStyle[] = array('left','right');
			$arrayHeaders[] = array(true,false);

			$arrayTable[] = array(
				'?TID_AML_ITEM_DEPOSITS? #',
				GetFloatValue(ArrayGetValue($arrayAccount,'TRANSACTION_COUNT_IN')));
			$arrayCellStyle[] = array('left','right');
			$arrayHeaders[] = array(true,false);


			$arrayTable[] = array(
				'?TID_AML_ITEM_WITHDRAWALS?',
				RenderNumber(GetFloatValue(ArrayGetValue($arrayAccount,'TRANSACTION_AMOUNT_OUT')),2,$strCurrencySymbol,$arraySettings));
			$arrayCellStyle[] = array('left','right');
			$arrayHeaders[] = array(true,false);

			$arrayTable[] = array(
				'?TID_AML_ITEM_WITHDRAWALS? #',
				GetFloatValue(ArrayGetValue($arrayAccount,'TRANSACTION_COUNT_OUT')));
			$arrayCellStyle[] = array('left','right');
			$arrayHeaders[] = array(true,false);

			$arrayTable[] = array(
				'?TID_AML_ITEM_BEGINBALANCE?', 
				RenderNumber(GetFloatValue(ArrayGetValue($arrayAccount,'BALANCE_BEGIN')),2,$strCurrencySymbol,$arraySettings));
			$arrayCellStyle[] = array('left','right');
			$arrayHeaders[] = array(true,false);


			$arrayTable[] = array(
				'?TID_AML_ITEM_NETFLOW?', 
				RenderNumber(GetFloatValue(ArrayGetValue($arrayAccount,'TRANSACTION_NETFLOW')),2,$strCurrencySymbol,$arraySettings));
			$arrayCellStyle[] = array('left','right');
			$arrayHeaders[] = array(true,false);

			$tableElement = new CTableElement($this->GetWebsite(),'default');
			
			$this->SetOutputItem(
				'SUMMARY',
				$tableElement->RenderTableEx($arrayTable,$arrayHeaders,$arrayCellStyle));
	
			
			return 'chart';
		}


		function StateFlows()
		{
			$this->SetState('flows');
			$this->RenderTabNavigation();
	
			$arrayAccount = $this->m_arrayAccount;
			
	
			$strID = ArrayGetValue($arrayAccount,'ID');
			$strUniqueID = StringFilter($strID,CHARSET_ALPHANUMERIC).'_'.MakeNumericToken(5);
					
			$arrayAccount = AML_CalculateAccount($arrayAccount);
			$this->Trace($strID.' account:');
			$this->Trace($arrayAccount);
			$strTransactionFirstDate = ArrayGetValue($arrayAccount,'TRANSACTION_FIRST_DATE');
			$strTransactionLastDate = ArrayGetValue($arrayAccount,'TRANSACTION_LAST_DATE');
			$strCurrency = ArrayGetValue($arrayAccount,'CURRENCY');
			$strCurrencySymbol = GetCurrencySymbol($strCurrency);
	
			$this->RenderTitle($strID);

			$arrayRedFlags = AML_RaiseRedFLags($arrayAccount,$this->m_arrayTypologyDefinitions);
			$arrayDetectedTypologyIDs = array();

			if (is_array($arrayRedFlags) == true)
			{
				foreach ($arrayRedFlags as $arrayRedFlag)
				{
					$arrayDetectedTypologyIDs[] = ArrayGetValue($arrayRedFlag,'TYPOLOGY');
				}
			}
			$arrayDetectedTypologyIDs = ArrayUnique($arrayDetectedTypologyIDs);
			$this->SetOutputItem(
				'REDFLAGS',
				$this->m_amlredflagsElement->RenderAMLRedFlagsLegend($this->m_arrayTypologyIDs,$arrayDetectedTypologyIDs));
						


			$arrayAccountFlows = AML_TransformAccountToAccountFlows($arrayAccount,$strTransactionFirstDate,$strTransactionLastDate); 
			$arrayDimensions = ArrayGetValue($arrayAccountFlows,'DIMENSIONS');
			$arrayData = ArrayGetValue($arrayAccountFlows,'DATA');
			$this->SetOutputItem(
				'FLOWS',
				$this->m_parallelsetsElement->RenderParallelSets($arrayDimensions, $arrayData, 'Amount', $strCurrencySymbol,2));	
			
			return 'flows';
		}
		

		function StateData()
		{
			$this->SetState('data');
			$this->RenderTabNavigation();
			
			$this->GetWebsite()->LoadStringResources('local');
	
			$arrayAccount = $this->m_arrayAccount;
			
	
			$strID = ArrayGetValue($arrayAccount,'ID');
			$arrayAccount = AML_CalculateAccount($arrayAccount);

			$strTransactionFirstDate = ArrayGetValue($arrayAccount,'TRANSACTION_FIRST_DATE');
			$strTransactionLastDate = ArrayGetValue($arrayAccount,'TRANSACTION_LAST_DATE');
			$strCurrency = ArrayGetValue($arrayAccount,'CURRENCY');
			$strCurrencySymbol = GetCurrencySymbol($strCurrency);

			$this->RenderTitle($strID);


			// Move this into a separate function!
			$arrayRedFlags = AML_RaiseRedFLags($arrayAccount,$this->m_arrayTypologyDefinitions);
			$arrayDetectedTypologyIDs = array();
			if (is_array($arrayRedFlags) == true)
			{
				foreach ($arrayRedFlags as $arrayRedFlag)
				{
					$arrayDetectedTypologyIDs[] = ArrayGetValue($arrayRedFlag,'TYPOLOGY');
				}
			}
			$arrayDetectedTypologyIDs = ArrayUnique($arrayDetectedTypologyIDs);
			$this->SetOutputItem(
				'REDFLAGS',
				$this->m_amlredflagsElement->RenderAMLRedFlagsLegend($this->m_arrayTypologyIDs,$arrayDetectedTypologyIDs));
	
	
			
			
			$table = new CTableElement($this->GetWebsite(),'default');
			
			$arrayTransactions = ArrayGetValue($arrayAccount,'TRANSACTIONS');
			
			
			$arrayTable = array();
			$arrayHeaders = array();
			$arrayCellStyle = array();

			$arrayTable[] = array(
				'?TID_ACCOUNTANALYSIS_TABLEHEADER_DATE?',
				'?TID_ACCOUNTANALYSIS_TABLEHEADER_AMOUNT?',
				'?TID_ACCOUNTANALYSIS_TABLEHEADER_TYPE?',
				'?TID_ACCOUNTANALYSIS_TABLEHEADER_COUNTRY?',
				'?TID_ACCOUNTANALYSIS_TABLEHEADER_DESCRIPTION?');
			$arrayCellStyle[] = array('left','left','left','left','left');
			$arrayHeaders[] = array(true,true,true,true,true);

			$arrayLocaleSettings = GetLocaleSettings('USA','EN');

			foreach ($arrayTransactions as $arrayTransaction)
			{
				$strCountry = ArrayGetValue($arrayTransaction,'COUNTRY');
				if (IsValidCountryID($strCountry))
				{
					$strCountry = '?TID_COUNTRY_'.$strCountry.'?';
				}
				$arrayTable[] = array(
					$this->GetWebsite()->RenderDate(ArrayGetValue($arrayTransaction,'DATE')),
					RenderNumber(ArrayGetValue($arrayTransaction,'AMOUNT'),2,$strCurrencySymbol,$arrayLocaleSettings),
					ArrayGetValue($arrayTransaction,'TYPE'),
					$strCountry,
					ArrayGetValue($arrayTransaction,'DESCRIPTION'));					
				$arrayCellStyle[] = array('left','right','left','left','left');
				$arrayHeaders[] = array(false,false,false,false,false);
			}
			
			$this->SetOutputItem(
				'TABLE',
				$table->RenderTableEx($arrayTable,$arrayHeaders,$arrayCellStyle));
			
			return 'data';
		}

		function StateRedFlags()
		{
			$this->SetState('redflags');
			$this->RenderTabNavigation();
			
			$this->GetWebsite()->LoadStringResources('local');
	
			$arrayAccount = $this->m_arrayAccount;
			
	
			$strID = ArrayGetValue($arrayAccount,'ID');
			$arrayAccount = AML_CalculateAccount($arrayAccount);

			$strTransactionFirstDate = ArrayGetValue($arrayAccount,'TRANSACTION_FIRST_DATE');
			$strTransactionLastDate = ArrayGetValue($arrayAccount,'TRANSACTION_LAST_DATE');
			$strCurrency = ArrayGetValue($arrayAccount,'CURRENCY');
			$strCurrencySymbol = GetCurrencySymbol($strCurrency);

			$this->RenderTitle($strID);


			// Move this into a separate function!
			$arrayRedFlags = AML_RaiseRedFLags($arrayAccount,$this->m_arrayTypologyDefinitions);
			$arrayDetectedTypologyIDs = array();
			if (is_array($arrayRedFlags) == true)
			{
				foreach ($arrayRedFlags as $arrayRedFlag)
				{
					$arrayDetectedTypologyIDs[] = ArrayGetValue($arrayRedFlag,'TYPOLOGY');
				}
			}
			$arrayDetectedTypologyIDs = ArrayUnique($arrayDetectedTypologyIDs);
			$this->SetOutputItem(
				'REDFLAGS',
				$this->m_amlredflagsElement->RenderAMLRedFlagsLegend($this->m_arrayTypologyIDs,$arrayDetectedTypologyIDs));
	
	
			
			
			$table = new CTableElement($this->GetWebsite(),'default');
			
			$arrayTable = array();
			$arrayHeaders = array();
			$arrayCellStyle = array();

			$arrayTable[] = array(
				'?TID_ACCOUNTANALYSIS_TABLEHEADER_TYPOLOGY?',
				'?TID_ACCOUNTANALYSIS_TABLEHEADER_FROM?',
				'?TID_ACCOUNTANALYSIS_TABLEHEADER_TO?',
				'?TID_ACCOUNTANALYSIS_TABLEHEADER_DETAILS?',
				'?TID_ACCOUNTANALYSIS_TABLEHEADER_TRANSACTIONS?');
			$arrayCellStyle[] = array('left','left','left','left','left');
			$arrayHeaders[] = array(true,true,true,true,true);

			foreach ($arrayRedFlags as $arrayRedFlag)
			{ 
				$strTypology = ArrayGetValue($arrayRedFlag,'TYPOLOGY');
				$strStartDate = ArrayGetValue($arrayRedFlag,'START_DATE');
				$strEndDate = ArrayGetValue($arrayRedFlag,'END_DATE');
				$arrayTransactions = ArrayGetValue($arrayRedFlag,'TRANSACTIONS');
				
				/*
				$strCountry = ArrayGetValue($arrayTransaction,'COUNTRY');
				if (IsValidCountryID($strCountry))
				{
					$strCountry = '?TID_COUNTRY_'.$strCountry.'?';
				}
				*/
				
				$arrayRedFlag = ArrayRemoveKey($arrayRedFlag,'TYPOLOGY');
				$arrayRedFlag = ArrayRemoveKey($arrayRedFlag,'START_DATE');
				$arrayRedFlag = ArrayRemoveKey($arrayRedFlag,'END_DATE');
				$arrayRedFlag = ArrayRemoveKey($arrayRedFlag,'TRANSACTIONS');
				
				$arrayTableRedFlag = array();
				$arrayTableRedFlagCellStyle = array();
				$arrayTableRedFlagHeaders = array();
				foreach ($arrayRedFlag as $strKey => $strValue)
				{
					switch ($strKey)
					{
					case 'AMOUNT':
					case 'TOTAL_AMOUNT':
					case 'THRESHOLD_AMOUNT':
						$arrayTableRedFlag[] = array($strKey,$this->GetWebsite()->RenderNumber($strValue,2,$strCurrencySymbol));
						break;
					default:	
						$arrayTableRedFlag[] = array($strKey,$strValue);
						break;
					}
					$arrayTableRedFlagCellStyle[] = array('left','right');
					$arrayTableRedFlagHeaders[] = array(true,false);
				}
				$strRedFlagDetails = $table->RenderTableEx($arrayTableRedFlag,$arrayTableRedFlagHeaders,$arrayTableRedFlagCellStyle);
				
				$strTransactions = '';
				if (ArrayCount($arrayTransactions) > 0)
				{
					$arrayTransactionsCellStyle = array();
					foreach ($arrayTransactions as &$arrayTransaction)
					{
						$arrayTransaction['DATE'] = $this->GetWebsite()->RenderDate(ArrayGetValue($arrayTransaction,'DATE'));
						$arrayTransaction['AMOUNT'] = $this->GetWebsite()->RenderNumber(ArrayGetValue($arrayTransaction,'AMOUNT'),2,$strCurrencySymbol);
						$arrayTransactionsCellStyle[] = array('left','left','right','left','left');
					}
					$strTransactions = $table->RenderTableEx($arrayTransactions,false,$arrayTransactionsCellStyle);
				}
				
				$arrayTable[] = array(
					'?TID_AML_TYPOLOGY_'.$strTypology.'?',
					$this->GetWebsite()->RenderDate($strStartDate),
					$this->GetWebsite()->RenderDate($strEndDate), 
					$strRedFlagDetails,
					$strTransactions);					
				$arrayCellStyle[] = array('left','left','left','left','left');
				$arrayHeaders[] = array(false,false,false,false,false);
			}
			
			$this->SetOutputItem(
				'TABLE',
				$table->RenderTableEx($arrayTable,$arrayHeaders,$arrayCellStyle));
			
			return 'redflags';
		}


	}

