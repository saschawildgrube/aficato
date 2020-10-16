<?php

	require_once(GetWDKDir()."wdk_list.inc");

	class CContentBrowserModule extends CModule
	{
		private $m_listContentItems;
		private $m_arrayTags;
		
		function __construct(
			CWebSite $website,
			$strInstanceID = "",
			$arrayConfig = array())
		{
			$this->m_listContentItems = new CList();
			$this->m_arrayTags = array();
			
			parent::__construct(
				$website,
				"cms/contentbrowser",
				$strInstanceID,
				$arrayConfig);
		}
		
		function CallbackInit()
		{
			$this->IncludeElement("tagcloud");
			
			$strDataID = $this->GetModuleConfig("dataid");
			if ($strDataID == "")
			{
				$this->Error("No Data ID specified.");
				return false;
			}
		
			$strDataFile = $this->GetDataFilePath($strDataID.".csv");
			
			$arrayContentItems = FileReadCSV_Format($this->GetConfig("csvformat"),$strDataFile);
			if ($arrayContentItems == false)
			{
				$this->Error("Failed to read data from file: $strDataFile");
				return false;
			}
			$this->Trace("Loaded file: $strDataFile");
			 
			$this->m_listContentItems->SetListArray($arrayContentItems);

/*
			// Ignore inactive items
			$arrayFilter = array(
				array(
					"field" => "ACTIVE",
					"operator" => "=",
					"value" => "1"));
			$this->m_listContentItems->Filter($arrayFilter);
*/
			
			
			$strLanguage = StringUpperCase($this->GetWebsite()->GetLanguage());
			
			// Create Content IDs
			$arrayContentItems = $this->m_listContentItems->GetListArray();
			foreach ($arrayContentItems as &$arrayContentItem)
			{
				$arrayContentItem["ID"] =
					MakeID(
						ArrayGetValue(
							$arrayContentItem,
							"TITLE"));
			}
			$this->m_listContentItems->SetListArray($arrayContentItems);
						
			
			// Get Tag Config
			$strConfigFile = $this->GetDataFilePath($strDataID.".cfg");
			$arrayConfig = LoadNamedArrayFromTextFile($strConfigFile);
			if ($arrayConfig == false)
			{
				$arrayConfig["TAG_FACTOR_A"] = "4";
				$arrayConfig["TAG_FACTOR_B"] = "3";
				$arrayConfig["TAG_FACTOR_C"] = "2";
				$arrayConfig["TAG_FACTOR_D"] = "1";
			}
			else
			{
				$arrayConfig["TAG_FACTOR_A"] = floatval($arrayConfig["TAG_FACTOR_A"]);
				$arrayConfig["TAG_FACTOR_B"] = floatval($arrayConfig["TAG_FACTOR_B"]);
				$arrayConfig["TAG_FACTOR_C"] = floatval($arrayConfig["TAG_FACTOR_C"]);
				$arrayConfig["TAG_FACTOR_D"] = floatval($arrayConfig["TAG_FACTOR_D"]);
			}			
			//$this->m_arrayConfig = $arrayConfig;
			
			
			
			
			// Get Tags
			$this->m_arrayTags = array();
			$arrayContentItems = $this->m_listContentItems->GetListArray();
			foreach ($arrayContentItems as &$arrayContentItemIterator)
			{
				$arrayAllTags = array();
				for ($nTagPrio = 0; $nTagPrio < 4; $nTagPrio++)
				{
					$strPrio = chr(ord('A') + $nTagPrio);
					$strTags = ArrayGetValue($arrayContentItemIterator,"TAGS_".$strPrio);
					$arrayTags = explode(",",$strTags);
					$fTagFactor = $arrayConfig["TAG_FACTOR_".$strPrio];
					foreach ($arrayTags as $strTag)
					{
						$strTagID = MakeID($strTag);
						if ($strTagID != "")
						{
							$fWeight = ArrayGetValue($this->m_arrayTags,$strTagID,"weight");
							$fWeight += $fTagFactor * 1;
							$this->m_arrayTags[$strTagID]["weight"] = $fWeight;
							
							$nCount = ArrayGetValue($this->m_arrayTags,$strTagID,"count");
							$nCount++;							
							$this->m_arrayTags[$strTagID]["count"] = $nCount;
							
							$this->m_arrayTags[$strTagID]["name"] = $strTag;
							
							$arrayAllTags[] = $strTagID;
						}
					}
				}
				$arrayContentItemIterator["TAGS"] = implode(",",$arrayAllTags);
			}
			$this->m_listContentItems->SetListArray($arrayContentItems);

			
			// Debug output
			$this->Trace($this->m_listContentItems->GetListArray());
			$this->Trace($this->m_arrayTags);


			
			return true;
		}

			
		function CallbackRenderModule($strState)
		{
			switch ($strState)
			{
				case "tagcloud":
					return $this->DoStateTagCloud();

				case "details":
					return $this->DoStateDetails();

				case "list":
				default:
					return $this->DoStateList();
			}
		}

		function GetContentItemByID()
		{
			$strContentItemID = $this->GetModuleVar("contentitemid");
			$arrayContentItems = $this->m_listContentItems->GetListArray();
			foreach ($arrayContentItems as $arrayContentItemIterator)
			{
				if ($arrayContentItemIterator["ID"] == $strContentItemID)
				{
					return $arrayContentItemIterator;
				}
			}
			return false;
		}
		
	
		
		function RenderTagCloud()
		{
			$tagcloudDef = new CTagCloudDefinition();
			
			$strFilter = $this->GetModuleVar("filter");
			$strContent = $this->GetModuleConfig("content");
						
			foreach ($this->m_arrayTags as $strTagID => $arrayTag)
			{
				$fWeight = $arrayTag["weight"];
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "list";
				if ($strContent != "")
				{
					$arrayParams["content"] = $strContent;
				}

				if ($strFilter == $strTagID && $this->GetState() != "tagcloud")
				{
					//$tagcloudDef->AddTag($strTag,$fWeight,"","selected");
					$arrayParams[$this->GetModuleVarName("filter")] = "";
					$strLink = $this->GetWebsite()->MakeURL($arrayParams);
					$tagcloudDef->AddTag($arrayTag["name"],$fWeight,$strLink,"selected");
					
				}
				else
				{
					$arrayParams[$this->GetModuleVarName("filter")] = $strTagID;
					$strLink = $this->GetWebsite()->MakeURL($arrayParams);
					$tagcloudDef->AddTag($arrayTag["name"],$fWeight,$strLink,"");
				}
			}
			
			$tagcloud = new CTagCloudElement($this->GetWebsite(),"default");
			$this->SetOutputItem(
				"TAGCLOUD",
				$tagcloud->RenderTagCloud($tagcloudDef)
				);

		}
		
		function DoStateTagCloud()
		{
			$this->Trace("DoStateTagCloud");
			$this->RenderTagCloud();
			$this->SetState("");
			return "tagcloud";
		}

		function DoStateList()
		{
			$this->Trace("DoStateList");
			$this->RenderTagCloud();
			
			
			$strFilter = $this->GetModuleVar("filter");
		
			$list = $this->m_listContentItems;

			$strLinkShowAll = "";
			if ($strFilter != "")
			{
				$arrayFilterOptions = array(
					array(
						"field" => "TAGS",
						"operator" => "contains",
						"value" => $strFilter
						)
					);
				$list->Filter($arrayFilterOptions);
				
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "list";
				$arrayParams[$this->GetModuleVarName("filter")] = "";
				$strURL = $this->GetWebsite()->MakeURL($arrayParams);
				$link = new CLinkElement($this->GetWebsite(),"default");
				$strLinkShowAll = $link->RenderLink($strURL,"?TID_CONTENTBROWSER_LINK_SHOWALL?");
			}

			$this->SetOutputItem(
				"LINKSHOWALL",
				$strLinkShowAll);
			
						
			$arrayContentItems = $list->GetListArray();
			
			if (ArrayCount($arrayContentItems) == 1)
			{
				if ($this->GetModuleVar("backlink") == "1")
				{
					$this->SetModuleVar("backlink","");
					$this->SetModuleVar("filter","");
					return $this->DoStateList();
				}
				$this->SetModuleVar("contentitemid",$arrayContentItems[0]["ID"]);
				return $this->DoStateDetails();
			}
			

			$strContentTeasers = "";
			
			$teaser = new CTeaserElement($this->GetWebsite(),"default");
			foreach ($arrayContentItems as $arrayContentItemIterator)
			{
				$this->Trace($arrayContentItemIterator);
				$strTitle = $arrayContentItemIterator["TITLE"];
				$strTeaser = chop($arrayContentItemIterator["TEASER"]);
				if ($strTeaser == "")
				{
					$strTeaser = $arrayContentItemIterator["DETAILS"];
					$strTeaser = StringCutOff($strTeaser, 200);
				}
				
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "details";
				$arrayParams[$this->GetModuleVarName("contentitemid")] = $arrayContentItemIterator["ID"];
				$strURL = $this->GetWebsite()->MakeURL($arrayParams);
												
				$strContentTeasers .= $teaser->RenderTeaser(
					$strTitle,
					$strTeaser,
					$strURL,
					"?TID_CONTENTBROWSER_LINK_DETAILS?");

			}
	
			$this->SetOutputItem(
				"LIST",
				$strContentTeasers);

			return "list";
		}


		function DoStateDetails()
		{
			$this->Trace("DoStateDetails");
			$strContentItemID = $this->GetModuleVar("contentitemid");
			if ($strContentItemID == "")
			{
				return $this->DoStateList();	
			}
			
			$arrayContentItem = $this->GetContentItemByID();
			if ($arrayContentItem == false)
			{
				return $this->DoStateList();	
			}
	
			$this->RenderTagCloud();
			
			
			
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = "list";
			$arrayParams[$this->GetModuleVarName("contentitemid")] = "";
			$arrayParams[$this->GetModuleVarName("backlink")] = "1";
			$strURL = $this->GetWebsite()->MakeURL($arrayParams);
			$link = new CLinkElement($this->GetWebsite(),"default");
			$this->SetOutputItem(
				"LINKBACK",
				$link->RenderLink($strURL,"?TID_CONTENTBROWSER_LINK_BACK?")
				);
			
				
			$strTitle = $arrayContentItem["TITLE"];
			$strDetails = $arrayContentItem["DETAILS"];
			
			$text = new CTextElement($this->GetWebsite(),"default");
			$this->SetOutputItem(
				"TEXT",
				$text->RenderText(
					$strTitle,
					$strDetails)
				);
				
				
			$arrayLinks = array();


			$strContent = $arrayContentItem["CONTENT"];
			if ($strContent != "")
			{
				$linkcontent = new CLinkElement($this->GetWebsite(),"default");
				$arrayLinks[] = $linkcontent->RenderLink(
					$this->GetWebsite()->MakeContentURL($strContent),
					"?TID_CONTENTBROWSER_LINK_MORE?");
			}
			
			
			
			$strPDFFile = $arrayContentItem["PDF_FILE"];
			if ($strPDFFile != "")
			{
				$strPDFName = $arrayContentItem["PDF_NAME"];
				if ($strPDFName == "")
				{
					$strPDFName = $strPDFFile;
				}
				$downloadlink = new CDownloadLinkElement($this->GetWebsite(),"default");
				$arrayLinks[] = $downloadlink->RenderDownloadLink(
					"{DOWNLOADPATH}".$strPDFFile,
					$strPDFName,
					"pdf");
			}
			
			
			

			$strLinkURL = $arrayContentItem["LINK_URL"];
			if ($strLinkURL != "")
			{
				$strLinkName = $arrayContentItem["LINK_NAME"];
				if ($strLinkName == "")
				{
					$strLinkName = $strLinkURL;
				}
				$link = new CLinkElement($this->GetWebsite(),"default");
				$arrayLinks[] = $link->RenderLink(
					$strLinkURL,
					$strLinkName,
					true);
			}
			
			if ($this->GetModuleConfig("showrelatedtaglinks") == true)
			{
				$strTags = ArrayGetValue($arrayContentItem,"TAGS");
				$arrayTags = explode(",",$strTags);
				foreach ($arrayTags as $strTagID)
				{
					$linktag = new CLinkElement($this->GetWebsite(),"default");
					$nCount = intval(ArrayGetValue($this->m_arrayTags,$strTagID,"count"));
					if ($nCount > 1)
					{
						$arrayParams = array();
						$arrayParams[$this->GetModuleVarNameState()] = "list";
						$arrayParams[$this->GetModuleVarName("filter")] = $strTagID;
						
						$arrayLinks[] = $linktag->RenderLink(
							$this->GetWebsite()->MakeURL($arrayParams),
							ArrayGetValue($this->m_arrayTags,$strTagID,"name"));
					}
				}
			}
			
			$list = new CListElement($this->GetWebsite(),"default");
			$this->SetOutputItem(
				"LINKLIST",
				$list->RenderList($arrayLinks)
				);
				
			return "details";
			
		}
		
		
	}

