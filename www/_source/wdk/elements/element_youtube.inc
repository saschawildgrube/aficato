<?php

	class CYouTubeElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				"youtube",
				$strStyleID,
				true,
				true);
		}
		
		function RenderYouTube($strID)
		{
			if ($strID == "")
			{
				return "";
			}
			$strOutput = $this->GetLayoutHTML(); 
			
			$arrayItems = array();
			$arrayItems["ID"] = $strID;
							
			$nWidth = $this->GetLayoutDef("LAYOUT_WIDTH");
			$nHeight = intval($nWidth / 1.674509803921569);  
			$arrayItems["WIDTH"] = "".$nWidth;	
			$arrayItems["HEIGHT"] = "".$nHeight;	
			$arrayItems["WIDTH_HALF"] = "".intval($nWidth/2);	
			$arrayItems["HEIGHT_HALF"] = "".intval($nHeight/2);	

				
			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strOutput);
			return $strOutput;
		}
	}

