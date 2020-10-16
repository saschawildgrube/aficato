<?php

	class CImageElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				"image",
				$strStyleID,
				false,
				true);
		}
		
		function RenderImage($strImageURL, $strText = "")
		{
			$strOutput = $this->GetLayoutHTML();  
			
			$arrayItems = array();
			
			$arrayItems["IMAGEURL"] = htmlentities($strImageURL);
			$arrayItems["TEXT"] = $strText;
			
			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strOutput);
			return $strOutput;			

		}
	}

