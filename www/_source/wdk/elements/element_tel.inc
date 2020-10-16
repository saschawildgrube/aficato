<?php

	// http://www.ietf.org/rfc/rfc3966.txt
	
	function MakeTel($strTel)
	{
		$strTel = StringFilter($strTel,CHARSET_TEL);
		if (StringSection($strTel,0,4) != "tel:")
		{
			$strTel = "tel:".$strTel;
		}
		return $strTel;	
	}

	class CTelElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				"tel",
				$strStyleID,
				false,
				true);
		}
		
		function RenderTel($strTel, $strText = "")
		{
			
			$strOutput = $this->GetLayoutHTML(); 
			
			$arrayItems = array();
			
			if ($strText == "")
			{
				$strText = $strTel;
			}
			if ($strTel == "")
			{
				$strTel = $strText;	
			}
			
			if (IsDeviceMobile() == false)
			{
				return $strText;
			}
			else
			{
				$strTel = MakeTel($strTel);
				
				$arrayItems["TEL"] = $strTel;
				$arrayItems["TEXT"] = $strText;
				
				$this->SetOutputItems($arrayItems);
				$strOutput = $this->ReplaceOutputItems($strOutput);
				return $strOutput;			
			}

		}
	}

