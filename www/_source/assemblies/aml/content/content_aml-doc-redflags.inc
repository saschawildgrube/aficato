<?php

	class CMyContent extends CContent
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$arrayRiskCountries = AML_GetRiskCountries();

			$this->GetWebsite()->LoadStringResources('local');
			
			$this->IncludeElement('icon');
			$icon = new CIconElement($this->GetWebsite(),"default");

			$arrayTable = array();
			/*
			array_push(
				$arrayTable,
				array(
					'?TID_LOCALENAVIGATOR_HEADER_COUNTRYID?',
					'?TID_LOCALENAVIGATOR_HEADER_FLAG?',
					'?TID_LOCALENAVIGATOR_HEADER_COUNTRYNAME?'));
			*/
			foreach ($arrayRiskCountries as $strCountryID => $arrayCountry)
			{
				$fRiskScore = GetFloatValue(ArrayGetValue($arrayCountry,'SCORE'));
				if (IsValidCountryID($strCountryID))
				{
					if ($fRiskScore > 0)
					{
						array_push(
							$arrayTable,
							array(
								$strCountryID,
								$icon->RenderIcon('country-'.StringLowerCase($strCountryID),'?TID_COUNTRY_'.$strCountryID.'?'),
								'?TID_COUNTRY_'.$strCountryID.'?'));
					}
				}
			}

			$table = new CTableElement($this->GetWebsite(),'default');
			$strTable = $table->RenderTable($arrayTable,false,false);			
			
			$arrayItems['HIGHRISKCOUNTRIES'] = $strTable;
		}
	}
	
