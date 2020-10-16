<?php

	require_once(GetWDKDir()."wdk_list.inc");

	// CPC codes are taken from: http://unstats.un.org/unsd/cr/registry/cpc-2.asp
	// Tax rates are taken from: http://en.wikipedia.org/wiki/Value-added_tax

	function GetVAT($strCountry,$strArea="",$strDate="",$strCPC="")
	{
		if ($strCountry != "TEST")
		{
			if (IsValidCountryID($strCountry) == false)
			{
				return false;	
			}
		}
		
		$strVATFile = GetWDKDir()."data/vat.csv";
		if (!IsFile($strVATFile))
		{
			return false;	
		}
		$arrayVAT = FileReadCSV_RFC4180($strVATFile);
		if ($arrayVAT == false)
		{
			return false;	
		}
		if ($strDate == "")
		{
			$strDate = RenderDateNow();	
		}
		if (IsValidDate($strDate) == false)
		{
			return false;	
		}
		
		foreach ($arrayVAT as &$arrayRow)
		{
			if (ArrayGetValue($arrayRow,"DATE_END") == "")
			{
				$arrayRow["DATE_END"] = "Z";	
			}
		}
		
		$listVAT = new CList();
		$listVAT->SetListArray($arrayVAT);
		
		
		// Filter by Country
		
		$listVAT->Filter(
			array(array(
				"field" => "COUNTRY",
				"operator" => "=",
				"value" => $strCountry
				))
			);

	
		if ($listVAT->GetCount() == 0)
		{
			return false;	
		}
		
		// Filter by Area
			
		$listVAT->Filter(
			array(array(
				"field" => "AREA",
				"operator" => "=",
				"value" => $strArea
				))
			);


		if ($listVAT->GetCount() == 0)
		{
			return false;	
		}

		
		// Filter by start date
				
		$listVAT->Filter(
			array(array(
				"field" => "DATE_START",
				"operator" => "<",
				"value" => $strDate
				))
			);

		// Filter by end date
				
		$listVAT->Filter(
			array(array(
				"field" => "DATE_END",
				"operator" => ">",
				"value" => $strDate
				))
			);

		if ($listVAT->GetCount() == 0)
		{
			return false;	
		}


		// Filter by CPC
		
		do 
		{
			$listVAT2 = new CList();
			$listVAT2->SetListArray($listVAT->GetListArray());
			$listVAT2->Filter(
				array(array(
					"field" => "CPC",
					"operator" => "=",
					"value" => $strCPC
					))
				);

			if ($listVAT2->GetCount() > 0)
			{
				$listVAT->SetListArray($listVAT2->GetListArray());
				break;
			}
			$strCPC = StringSection($strCPC,0,-1);
			
		} while ($strCPC != "");
		
		
		
		if ($listVAT->GetCount() == 0)
		{
			return false;	
		}


		// Sort by date
				
		if ($listVAT->GetCount() > 1)
		{
			$listVAT->Sort(
				array(
					"DATE_START" => "desc",
					)
				);
		}
		
		$arrayVAT = $listVAT->GetListArray();
		$arrayVATrow = ArrayGetValue($arrayVAT,0);
		
		$fVAT = floatval(ArrayGetValue($arrayVATrow,"RATE"));

		return $fVAT;
	}


