<?php

	class CPasswordPolicyElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			$website->LoadStringResources("passwordpolicy");
			parent::__construct(
				$website,
				"passwordpolicy",
				$strStyleID,
				true,
				false);
		}
		
		function RenderPasswordPolicyInfo($passwordpolicy)
		{
			$arrayList = array();
			$arrayItems = array();
			
			$arrayList[] = $this->LoadString("TID_INFO_PASSWORDPOLICY_MINIMUM_LENGTH");
			$arrayItems["MINIMUM_LENGTH"] = $passwordpolicy->GetMinimumLength();
			
			$bRequiredAlpha = false;
			if ($passwordpolicy->GetRequiredCharsAlphaUppercase() == true)
			{
				$bRequiredAlpha = true;
				$arrayList[] = $this->LoadString("TID_INFO_PASSWORDPOLICY_REQUIRED_CHARS_ALPHA_UPPERCASE");
			}
			if ($passwordpolicy->GetRequiredCharsAlphaLowercase() == true)
			{
				$bRequiredAlpha = true;
				$arrayList[] = $this->LoadString("TID_INFO_PASSWORDPOLICY_REQUIRED_CHARS_ALPHA_LOWERCASE");
			}

			if ($bRequiredAlpha == false)
			{
				if ($passwordpolicy->GetRequiredCharsAlpha() == true)
				{
					$arrayList[] = $this->LoadString("TID_INFO_PASSWORDPOLICY_REQUIRED_CHARS_ALPHA");
				}
			}
			if ($passwordpolicy->GetRequiredCharsNumeric() == true)
			{
				$arrayList[] = $this->LoadString("TID_INFO_PASSWORDPOLICY_REQUIRED_CHARS_NUMERIC");
			}
			
			
			//$strOutput = $this->GetLayoutHTML(); 
			
			$list = new CListElement($this->GetWebsite(),"default");
			$strOutput = $list->RenderList($arrayList);
			$strOutput = ReplaceTagsWDK($strOutput, $arrayItems);
			return $strOutput;
		}
		
		function RenderPasswordPolicyErrors($arrayErrors)
		{
			$arrayList = array();
			foreach ($arrayErrors as $strError)
			{
				$arrayList[] = $this->LoadString("TID_ERROR_".$strError);
			}
			
			$list = new CListElement(
				$this->GetWebsite(),
				$this->GetLayoutDef("LAYOUT_PASSWORDPOLICY_LISTSTYLE"));
			$strOutput = $list->RenderList($arrayList);
			return $strOutput;
		}		
	}

