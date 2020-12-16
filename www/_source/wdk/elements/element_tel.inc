<?php

	// http://www.ietf.org/rfc/rfc3966.txt
	
	class CTelElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				'tel',
				$strStyleID,
				false,
				true);
		}
		
		function RenderTel($strTel, $strText = '')
		{
			
			$strOutput = $this->GetLayoutHTML(); 
			
			$arrayItems = array();
			
			if ($strText == '')
			{
				$strText = $strTel;
			}
			if ($strTel == '')
			{
				$strTel = $strText;	
			}
			
			$strTel = MakeTel($strTel);
			
			$arrayItems['TEL'] = $strTel;
			$arrayItems['TEXT'] = $strText;
			
			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strOutput);
			return $strOutput;			
		}
	}

