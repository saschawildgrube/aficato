<?php

	require_once(GetWDKDir()."wdk_module_entitymanager.inc");
	require_once(GetWDKDir()."wdk_entity_article.inc");
	require_once(GetWDKDir()."wdk_bbcode.inc");
	
	class CArticleManagerModule extends CEntityManagerModule
	{
	
		function __construct(
			CWebSite $website,
			$strInstanceID = "",
			$arrayConfig = array())
		{
			parent::__construct(
				$website,
				"cms/articlemanager",
				$strInstanceID,
				$arrayConfig);
		}
		
		function CallbackInit()
		{
			$arrayEntityDefinitions = array(
				"article" => GetEntityDefinitionArticle()
			);
			$this->SetEntityDefinitions($arrayEntityDefinitions);
			$this->GetWebsite()->LoadStringResources("local");
			return true;
		}
		
	
		function CallbackRenderModule($strState)
		{
			$this->Trace("CallbackRenderModule(\"$strState\")");
			switch ($strState)
			{
				case "help":
					return $this->StateHelp();
				case "":
					return $this->StateList("article");
				default:
					return parent::CallbackRenderModule($strState);
			}
		}
		
		function CallbackPrepareWebserviceParams($strEntity,&$arrayParams)
		{
			if ($strEntity == "article")
			{
				$strCommand = ArrayGetValue($arrayParams,"command");
				if (	$strCommand == "add"
					||	$strCommand == "set")
				{
					$strAuthUserID = $this->GetWebsite()->GetAuthUserID();
					if ($strAuthUserID != "")
					{
						$arrayParams["author_user_id"] = $strAuthUserID;	
					}
				}
			}
		}
		
		
		function CallbackPrepareFilterSelectOptions($strEntity,$strFilter)
		{
			$arrayOptions = array();
			if ($strEntity == "article")
			{
/*				if ($strFilter == "filterbystatus")
				{
					$arrayOptions = array();
					$arrayOptions[""] = "?TID_OPTION_ALL?";
					$arrayStatus = GetArticleStatusEnum();
					foreach ($arrayStatus as $strStatus)
					{
						$arrayOptions[$strStatus] = $strStatus;
					}
					return $arrayOptions;
				}
				else if ($strFilter == "filterbytextformat")
				{
					$arrayOptions = array();
					$arrayOptions[""] = "?TID_OPTION_ALL?";
					$arrayTextFormat = GetArticleTextFormatEnum();
					foreach ($arrayTextFormat as $strTextFormat)
					{
						$arrayOptions[$strTextFormat] = $strTextFormat;
					}
					return $arrayOptions;
				}
				else if ($strFilter == "filterbytype")
				{
					$arrayOptions = array();
					$arrayOptions[""] = "?TID_OPTION_ALL?";
					$arrayType = GetArticleTypeEnum();
					foreach ($arrayType as $strType)
					{
						$arrayOptions[$strType] = $strType;
					}
					return $arrayOptions;
				}*/
				/*
				else if ($strFilter == "filterbylanguage")
				{
					$arrayOptions = array();
					$arrayOptions[""] = "?TID_OPTION_ALL?";
					$arrayLanguages = $this->GetWebsite()->GetSupportedLanguages();
					foreach ($arrayLanguages as $strLanguage)
					{
						$arrayOptions[$strLanguage] = "?TID_LANGUAGE_".StringUpperCase($strLanguage)."?";
					}
				}
				else */
				if ($strFilter == "filterbycountry")
				{
					$arrayOptions = array();
					$arrayOptions[""] = "?TID_OPTION_ALL?";
					$arrayCountries = $this->GetWebsite()->GetSupportedCountries();
					foreach ($arrayCountries as $strCountry)
					{
						$arrayOptions[$strCountry] = "?TID_COUNTRY_".StringUpperCase($strCountry)."?";
					}
					return $arrayOptions;
				}				
			}
			
			return parent::CallbackPrepareFilterSelectOptions($strEntity,$strFilter);
		}


		function CallbackPrepareSelectOptions($strEntity,$strAttributeID,$strIndex)
		{
			$arrayOptions = array();
			if ($strEntity == "article")
			{
				if ($strAttributeID == "country")
				{
					$arrayOptions = array();
					$arrayOptions[""] = "?TID_OPTION_ALL?";
					$arrayCountries = $this->GetWebsite()->GetSupportedCountries();
					foreach ($arrayCountries as $strCountry)
					{
						$arrayOptions[$strCountry] = "?TID_COUNTRY_".StringUpperCase($strCountry)."?";
					}
					return $arrayOptions;
				}
				
			}
			return parent::CallbackPrepareSelectOptions($strEntity,$strAttributeID,$strIndex);
		}
		
		
		function CallbackPrepareItemDataView($strEntity,$strIndex,&$arrayItem)
		{
			if ($strEntity == "article")
			{
				$strCountry = ArrayGetValue($arrayItem,"country");
				if ($strCountry != "")
				{
					$arrayItem["country"] = "?TID_COUNTRY_".StringUpperCase($strCountry)."?";
				}
				
				$strAuthorUserID = ArrayGetValue($arrayItem,"author_user_id");
				$arrayItem["author_user_id"] = "";
				if (intval($strAuthorUserID) != 0)
				{
					$webservice = new CWebserviceConsumerWebApplication($this->GetWebsite());
					$arrayParams["command"] = "get";
					$arrayParams["user_id"] = $strAuthorUserID;
					$webservice->ConsumeWebService("system/user",$arrayParams);
					$strError = $webservice->GetError();
					if ($strError == "")
					{
						$arrayItem["author_user_id"] = $webservice->GetResultValue("USER","USER_NAME");
					}
				}
				
				$strReferenceArticleVersionID = ArrayGetValue($arrayItem,"reference_articleversion_id");
				if (intval($strReferenceArticleVersionID) != 0)
				{
					$arrayParams = array();
					$arrayParams[$this->GetModuleVarNameState()] = $strEntity."_view";
					$arrayParams[$this->GetItemIndexPropertyModuleVarName($strEntity)] = $strReferenceArticleVersionID;
					$strLink = $this->GetWebsite()->MakeURL($arrayParams);
					$link = new CLinkElement($this->GetWebsite(),"default");
					$arrayItem["reference_articleversion_id"] = $link->RenderLink($strLink,$strReferenceArticleVersionID);
				}
				else
				{
					$arrayItem["reference_articleversion_id"] = "?TID_ARTICLE_NOREFERENCEARTICLE?";
				}
				
				$strPublicationStartDateTime = $arrayItem["publication_start_datetime"];
				if ($strPublicationStartDateTime == "")
				{
					$arrayItem["publication_start_datetime"] = "?TID_ARTICLE_PUBLICATION_START_NOTSET?";
				}
				$strPublicationEndDateTime = $arrayItem["publication_end_datetime"];
				if ($strPublicationEndDateTime == "")
				{
					$arrayItem["publication_end_datetime"] = "?TID_ARTICLE_PUBLICATION_END_NOTSET?";
				}
				$strLastAccessDateTime = ArrayGetValue($arrayItem,"lastaccess_datetime");
				if ($strLastAccessDateTime == "")
				{
					$arrayItem["lastaccess_datetime"] = "?TID_ARTICLE_LASTACCESS_NOTSET?";
				}

				/*
					parent::CallbackPrepareItemDataView will replace line breaks by "<br/>" tags.
					But this is not what we want, as the content will be rendered depending on
					article format using the article element.
				*/
				$strContent = $arrayItem["content"];    
				parent::CallbackPrepareItemDataView($strEntity,$strIndex,$arrayItem);
				$arrayItem["content"] = $strContent;
				
			}
			else
			{
				parent::CallbackPrepareItemDataView($strEntity,$strIndex,$arrayItem);
			}
		}
		
		function CallbackItemView($strEntity,$strEntityIndex,$arrayItem)
		{
			$this->IncludeElement("tabnavigation");
			$this->IncludeElement("article");
			
			
			$strContentViewMode = $this->GetModuleVar("contentviewmode");
			if ($strContentViewMode == "")
			{
				$strContentViewMode = "formatted";
			}
			
			$tabnavigationDef = new CTabNavigationDefinition();
					
		   $arrayParams = array();
		   $arrayParams[$this->GetModuleVarName("contentviewmode")] = "formatted";
		   $tabnavigationDef->AddTab(
		      "formatted",
		      "?TID_ARTICLE_CONTENTVIEWMODE_FORMATTED?",
		      $this->GetWebsite()->MakeURL($arrayParams));

		   $arrayParams = array();
		   $arrayParams[$this->GetModuleVarName("contentviewmode")] = "raw";
		   $tabnavigationDef->AddTab(
		      "raw",
		      "?TID_ARTICLE_CONTENTVIEWMODE_RAW?",
		      $this->GetWebsite()->MakeURL($arrayParams));


			$tabnavigation = new CTabNavigationElement(
			   $this->GetWebsite(),
			   "default");
			   
			$this->SetOutputItem(
				"TABNAV_CONTENTVIEWMODE",
				$tabnavigation->RenderTabNavigation(
   				$tabnavigationDef,
   				$strContentViewMode)
   			); 
   			
   		$strContent = $arrayItem["content"];
   		$strFormat = ($strContentViewMode == "raw")?("raw"):($arrayItem["textformat"]);
   		
   		$article = new CArticleElement($this->GetWebsite(),"default");
   		$strContent = $article->RenderArticle($strContent,$strFormat);
   		
  			$this->SetOutputItem(
				"ARTICLECONTENT",
				$strContent);


		}
	}

