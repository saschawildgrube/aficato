<?php

	function IsValidContentID($strContentID)
	{
		if ($strContentID == "")
		{
			return false;
		}
		if (StringCheckCharSet($strContentID,CHARSET_ID) == false)
		{
			return false;	
		}	
		return true;
	}

	class CContent extends CWebsiteSatellite
	{
		private $m_strContentID;
		
		function __construct(
			CWebSite $website,
			$strContentID)
		{
			parent::__construct($website, $strContentID);
			$this->m_strContentID = $strContentID;
		}
		
		function GetSatelliteType()
		{
			return 'Content';	
		}

		
		function RenderSpecialContent()
		{
			return $this->OnRenderSpecialContent();
		}
		
		function RenderContent()
		{
			$arrayRequiredConditions = $this->OnGetRequiredConditions();
			if (is_array($arrayRequiredConditions))
			{
				foreach ($arrayRequiredConditions as $strCondition => $strError)
				{
					if ($this->GetWebsite()->CheckCondition($strCondition) != true)
					{
						$this->Trace("Condition check for \"$strCondition\" has failed.");
						$this->GetWebsite()->SetErrorMessageCode($strError);
						return "";
					}
					else
					{
						$this->Trace("Condition check passed for \"$strCondition\".");
					}
				}	
			}
						
      $strSpecialContent = $this->OnRenderSpecialContent();
      if ($strSpecialContent != "")
      {
      	return $strSpecialContent;
      }
			
			$strHTML="";


			$strFile = $this->GetAssemblyLine()->GetContentTxtFilePath(
				$this->m_strContentID,
				$this->GetWebsite()->GetLanguage());

			if ($strFile != false)
      {
				$strTxt = FileRead($strFile);
				$strTxt = $this->GetWebsite()->ReplaceAllOutputItems($strTxt);
				$this->IncludeElement("wiki");
				$wiki = new CWikiElement($this->GetWebsite(),"default");
				$strHTML = $wiki->RenderWiki($strTxt);
      }
      else
      { 
				$strFile = $this->GetAssemblyLine()->GetContentHtmlFilePath(
					$this->m_strContentID,
					$this->GetWebsite()->GetLanguage());
				if ($strFile != false)
	      {
					$strHTML = FileRead($strFile);
	      }
	      else
	      {
	      	if (get_class($this) == "CMyContent")
	      	{
	      		FatalError(
	      			"A CMyContent class is present but there is no htm or txt file for the contents \"$this->m_strContentID\"!",
	      			$this->GetWebsite()->GetTraceLog());	
	      	} 
	      	return false;
	      }		
			}			
		
			$arrayItems = array();
			$this->OnRenderContent($arrayItems);
			$this->SetOutputItems($arrayItems);
			$strHTML = $this->ReplaceOutputItems($strHTML);
			return $strHTML;
		}
		
		function OnInit()
		{
			$this->Error("Content \"".$this->m_strContentID."\": OnInit not implemented.");	
		}

		function OnRenderSpecialContent()
		{
			$this->Trace("Content \"".$this->m_strContentID."\" does not implement OnRenderSpecialContent");
			return "";
		}
		
		function OnRenderContent(&$arrayItems)
		{
			$this->Trace("Content \"".$this->m_strContentID."\" uses CContent");
		}
		
		function OnGetRequiredConditions()
		{
			return array();
		}

	}

