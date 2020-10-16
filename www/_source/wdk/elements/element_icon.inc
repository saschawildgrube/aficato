<?php

	class CIconElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				"icon",
				$strStyleID,
				true,
				false);
		}
		
		function RenderIcon($strIconID, $strToolTip)
		{
			$strLayoutDef = $this->GetLayoutDef("LAYOUT_ICON_".StringUppercase($strIconID));
			if ($strLayoutDef == "")
			{
				$strLayoutDef = $this->GetLayoutDef("LAYOUT_ICON_DEFAULT");
			}
			
			$arrayItems = array();
			$arrayItems["ICONID"] = $strIconID;
			if (FindString($strLayoutDef,"IMAGEURL") != -1)
			{
				$arrayItems["IMAGEURL"] = HtmlEncode($this->GetWebsite()->GetLayoutImageURL("icon_".$strIconID,"png"));
			}
			if (FindString($strLayoutDef,"TOOLTIP") != -1)
			{
				$arrayItems["TOOLTIP"] = HtmlEncode($strToolTip);
			}
			
			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strLayoutDef);
			return $strOutput;			
		}
	}

