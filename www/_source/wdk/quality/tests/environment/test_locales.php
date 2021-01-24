<?php

	require_once(GetWDKDir()."wdk_locale.inc");
	require_once(GetWDKDir()."wdk_country.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Check installed locales");
		}
		  


 
		function OnTest()
		{
			parent::OnTest();
			$this->SetResult(true);

			$arrayLocales = GetAllLocales();
			
			$strInstallShellCommand = "";

			foreach ($arrayLocales as $strCountryID => $arrayLanguages)
			{
				if (!IsValidCountryID($strCountryID))
				{
					$this->SetResult(false);
					$this->Trace("Failed! IsValidCountryID(\"$strCountryID\") returned false");
				}
				foreach ($arrayLanguages as $strLanguageID => $strLocale)
				{
					if (!IsValidLanguageID($strLanguageID))
					{
						$this->SetResult(false);
						$this->Trace("Failed! IsValidLanguageID(\"$strLanguageID\") returned false");
					}
					
					$strLocale = GetLocale($strCountryID,$strLanguageID);		
			
					$this->Trace("GetLocale(\"$strCountryID\",\"$strLanguageID\") returned \"$strLocale\"");
					if ($strLocale == "")
					{
						$this->Trace("Failed!");
						$this->SetResult(false);
					}
					$strLocaleNew = setlocale(LC_ALL,$strLocale);
					if ($strLocaleNew === false)
					{
						$this->Trace("Failed: setlocale(LC_ALL,\"$strLocale\") returned false");
						$this->SetResult(false);
						$strInstallShellCommand .= "locale-gen ".$strLocale."\n";
					}
				}
			}
			if ($strInstallShellCommand != "")
			{
				$this->Trace("\n\nSome are not installed. Use this command to verify:\n"); 
				$this->Trace("locale -a\n\n");
				$this->Trace("Use this shell command to fix it:\n");
				$this->Trace($strInstallShellCommand);
				$this->Trace("\nThen restart the server!\n");
				$this->Trace("\n");
			}
		}
	}
	
	

		
