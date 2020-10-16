<?php
	
	require_once(GetWDKDir()."wdk_locale.inc");
	require_once(GetWDKDir()."wdk_country.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK GetAllLocales");
		}
		  
		function CallbackInit()
		{
			$this->SetActive(false);
		}

 
		function CallbackTest()
		{
			parent::CallbackTest();
			$this->SetResult(true);

			$arrayLocales = GetAllLocales();

			
			$arrayAllCountries = GetAllCountryIDs();
			foreach ($arrayAllCountries as $strCountry)
			{
				$arrayLanguageLocales = ArrayGetValue($arrayLocales,$strCountry);
				if (!is_array($arrayLanguageLocales))
				{
					$this->Trace("$strCountry is not supported by GetAllLocales()");
					$this->SetResult(false);	
				}
			}		
		}
	}
	
	

		
