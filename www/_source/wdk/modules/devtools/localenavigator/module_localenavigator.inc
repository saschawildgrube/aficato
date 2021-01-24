<?php

	require_once(GetWDKDir().'wdk_module.inc');
	
	class CLocaleNavigatorModule extends CModule
	{

		function __construct(
			CWebSite $website,
			$strInstanceID = '',
			$arrayConfig = array())
		{
			parent::__construct(
				$website,
				'devtools/localenavigator',
				$strInstanceID,
				$arrayConfig);
		}

	
		function OnRenderModule($strState)
		{
			$this->Trace('OnRenderModule("'.$strState.'")');
			switch ($strState)
			{
				case 'languages':
					return $this->StateLanguages();
				case 'locales':
					return $this->StateLocales();
				case 'currencies':
					return $this->StateCurrencies();
				case 'timezones':
					return $this->StateTimezones();				
				case '':
					$this->SetState('countries');
				default:
					return $this->StateCountries();
			}
		}
		
		function RenderTabNavigation()
		{
			$this->IncludeElement('tabnavigation');

			$tabnavigationDef = new CTabNavigationDefinition();
   
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = 'countries';
   		$tabnavigationDef->AddTab(
	      'countries',
      	'?TID_LOCALENAVIGATOR_TAB_COUNTRIES?',
      	$this->GetWebsite()->MakeURL($arrayParams));

			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = 'languages';
   		$tabnavigationDef->AddTab(
	      'languages',
      	'?TID_LOCALENAVIGATOR_TAB_LANGUAGES?',
      	$this->GetWebsite()->MakeURL($arrayParams));

			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = 'locales';
   		$tabnavigationDef->AddTab(
	      'locales',
      	'?TID_LOCALENAVIGATOR_TAB_LOCALES?',
      	$this->GetWebsite()->MakeURL($arrayParams));
      	
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = 'currencies';
   		$tabnavigationDef->AddTab(
	      'currencies',
      	'?TID_LOCALENAVIGATOR_TAB_CURRENCIES?',
      	$this->GetWebsite()->MakeURL($arrayParams));      	
      	
      $arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = 'timezones';
   		$tabnavigationDef->AddTab(
	      'timezones',
      	'?TID_LOCALENAVIGATOR_TAB_TIMEZONES?',
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

		
		
		function StateCountries() 
		{
			$this->RenderTabNavigation();

			$this->IncludeElement('flag');
			$flag = new CFlagElement($this->GetWebsite(),'default');
			$arrayIDs = GetAllCountryIDs();
			$arrayList = array();
			array_push(
				$arrayList,
				array(
					'?TID_LOCALENAVIGATOR_HEADER_COUNTRYID?',
					'?TID_LOCALENAVIGATOR_HEADER_FLAG?',
					'?TID_LOCALENAVIGATOR_HEADER_COUNTRYNAME?'));
			foreach ($arrayIDs as $strCountryID)
			{
				array_push(
					$arrayList,
					array(
						$strCountryID,
						$flag->RenderFlag($strCountryID),
						'?TID_COUNTRY_'.$strCountryID.'?'));				
			}
			$table = new CTableElement($this->GetWebsite(),'default');
			$this->SetOutputItem('LIST',$table->RenderTable($arrayList,true,false));
			
			return 'countries';
		}
		
		function StateLanguages() 
		{
			$this->RenderTabNavigation();

			$arrayIDs = GetAllLanguageIDs();
			$arrayList = array();
			array_push(
				$arrayList,
				array(
					'?TID_LOCALENAVIGATOR_HEADER_LANGUAGEID?',
					'?TID_LOCALENAVIGATOR_HEADER_LANGUAGENAME?'));
			foreach ($arrayIDs as $strLanguage)
			{
				array_push(
					$arrayList,
					array(
						$strLanguage,
						'?TID_LANGUAGE_'.$strLanguage.'?'));				
			}
			$table = new CTableElement($this->GetWebsite(),'default');
			$this->SetOutputItem('LIST',$table->RenderTable($arrayList,true,false));
			
			return 'languages';
		}

		function StateLocales() 
		{
			$this->RenderTabNavigation();


			$arrayLocales = GetAllLocales();
			$arrayList = array();
			array_push(
				$arrayList,
				array(
					'?TID_LOCALENAVIGATOR_HEADER_COUNTRYID?',
					'?TID_LOCALENAVIGATOR_HEADER_LANGUAGEID?',
					'?TID_LOCALENAVIGATOR_HEADER_LOCALEID?',
					'?TID_LOCALENAVIGATOR_HEADER_NUMBERFORMAT?'));
			foreach ($arrayLocales as $strCountry => $arrayLanguages)
			{
				foreach ($arrayLanguages as $strLanguage => $strLocale)
				{
					$arraySettings = GetLocaleSettings($strCountry,$strLanguage);
					$strNumber = RenderNumber(1234.12,2,'',$arraySettings);
					array_push($arrayList,
						array(
							$strCountry,
							$strLanguage,
							$strLocale,
							$strNumber));				
				}
			}
			$table = new CTableElement($this->GetWebsite(),'default');
			$this->SetOutputItem('LIST',$table->RenderTable($arrayList,true,false));
			
			return 'locales';
		}

		function StateCurrencies() 
		{
			$this->RenderTabNavigation();

			$arrayCurrencyIDs = GetAllCurrencyIDs();
			$arrayList = array();
			array_push(
				$arrayList,
				array(
					'?TID_LOCALENAVIGATOR_HEADER_CURRENCYID?',
					'?TID_LOCALENAVIGATOR_HEADER_CURRENCYNAME?',
					'?TID_LOCALENAVIGATOR_HEADER_CURRENCYSYMBOL?', 
					'?TID_LOCALENAVIGATOR_HEADER_NUMBERFORMAT?'));
			foreach ($arrayCurrencyIDs as $strCurrencyID)
			{
				array_push($arrayList,
					array(
						$strCurrencyID,
						'?TID_CURRENCY_'.$strCurrencyID.'?',
						GetCurrencySymbol($strCurrencyID),
						$this->GetWebsite()->RenderCurrency(123.45,$strCurrencyID)));
			}

			$table = new CTableElement($this->GetWebsite(),'default');
			$this->SetOutputItem('LIST',$table->RenderTable($arrayList,true,false));
			
			return 'currencies';
		}


		function StateTimezones() 
		{
			$this->RenderTabNavigation();

			$arrayList = GetAllTimezoneIDs();
			
			$list = new CListElement($this->GetWebsite(),'default');
			$this->SetOutputItem('LIST',$list->RenderList($arrayList));
			
			//$table = new CTableElement($this->GetWebsite(),'default');
			//$this->SetOutputItem('LIST',$table->RenderTable($arrayList,true,false));
			
			return 'timezones';
		}

	}

