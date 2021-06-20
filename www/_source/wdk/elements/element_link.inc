<?php

	class CLinkElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				"link",
				$strStyleID,
				false,
				true);
		}
		
		function RenderLink($strURL, $strText = "", $bExternal = false)
		{
			$strOutput = $this->GetLayoutHTML(); 
			
			$arrayItems = array();
			
			if (strstr($strURL,"&amp;") == false )
			{
				$strURL = HtmlEncode($strURL);
			}
			
			if ($strText == "")
			{
				$strText = $strURL;
				if (strpos($strText,"mailto:") === 0)
				{
					$strText = substr($strText,7);
				}
			}

			
			$arrayItems["URL"] = $strURL;
			$arrayItems["TEXT"] = $strText;
			$arrayItems["EXTERNAL"] = ($bExternal == true)?(' target="_blank"'):('');
			
			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strOutput);
			return $strOutput;			

		}
	}

