<?php

	require_once(GetWDKDir()."wdk_webservice_database_entity.inc");  
	require_once(GetWDKDir()."wdk_entity_article.inc");  
	require_once(GetWDKDir()."wdk_language.inc");  
	require_once(GetWDKDir()."wdk_country.inc");  
		
   class CArticleWebService extends CDatabaseEntityWebService
	{
		function __construct(
			$strSourceDir,
			$strServiceDir,
			$arrayConfig,
			$arrayParams)
		{
			$arrayMeta = array();
			$arrayMeta["version"] = WDK_VERSION; 
			$arrayMeta["author"] = "Sascha Wildgrube";

			$arrayEntityDefinition = array(
				"article" => GetEntityDefinitionArticle()
				);
				
				
			parent::__construct(
				"cms/article",
				$strSourceDir,
				$strServiceDir,
				$arrayMeta,
				$arrayConfig,
				$arrayParams,
				$arrayEntityDefinition);
				
		}
		
		function OnInitParameterDefinition(&$paramDef)
		{
			$paramDef->AddParameterDefinitionString("new_name",1,128,CHARSET_TEXTID);
			$paramDef->AddParameterDefinitionBoolean("update_lastaccess");
			//$paramDef->AddParameterDefinitionInteger("threshold_versions",0,1000,true);
			//$paramDef->AddParameterDefinitionDateTime("threshold_datetime","",RenderDateTimeNow(),true);
			parent::OnInitParameterDefinition($paramDef);
			
		}
		
		function OnInitCommandDefinition(&$commandDef)
		{
			$commandDef->AddCommandDefinition(
				"rename",
				array(
					"name",
					"new_name"
					)
				);
			/*
			$commandDef->AddCommandDefinition(
				"cleanup",
				array(
					"name",
					"language",
					"country",
					"type",
					"threshold_datetime",
					"threshold_versions"
					)
				);
			*/
			parent::OnInitCommandDefinition($commandDef);
		}
		
		function OnInit()
		{
			if ($this->DatabaseConnect() != true)
			{
				return false;
			}			
			return parent::OnInit();
		}
		
		function OnCommand($strCommand)
		{
			switch ($strCommand)
			{

			case "rename":
				$this->CommandRename();
				return;

			case "cleanup":
				$this->CommandCleanUp();
				return;
				
			default:
				parent::OnCommand($strCommand);
				return;
			}
		}

		function CommandAdd($strEntity)
		{
			if ($strEntity == "article")
			{
				parent::CommandAdd("article");
				return;
			}
		}
		
		function OnCheckParameter($strCommand,$strParamName,$strParamValue)
		{
			if ($strParamName == "language")
			{
				if (IsValidLanguageID($strParamValue) == false)
				{
					return false;
				}
			}
			else if ($strParamName == "country")
			{
				if ($strParamValue != "")
				{
					if (IsValidCountryID($strParamValue) == false)
					{
						return false;
					}
				}
			}
			/*
			else if ($strParamName == "author_user_id")
			{
				$nUserID = intval($strParamValue);
				if ($nUserID == 0)
				{
					return false;	
				}
			}
			*/
			$bResult = parent::OnCheckParameter($strCommand,$strParamName,$strParamValue);	
			if ($bResult == true)
			{
				if ($strParamName == "publication_end_datetime")
				{
					$strPublicationStartDatetime = $this->GetParam("publication_start_datetime");
					$strPublicationEndDatetime = $strParamValue;
					if ($strPublicationStartDatetime != "" && $strPublicationEndDatetime != "")
					{
						if ($strPublicationStartDatetime > $strPublicationEndDatetime)
						{
							return false;	
						}
					}
				}
			}
			return $bResult;
		}		
		
		
		function MakeIndexSQLCondition($strEntity)
		{
			if ($strEntity != "article")
			{
				return parent::MakeIndexSQLCondition($strEntity);
			}
			else
			{
				$strNameOrTitleField = "NAME";
				$strArticleVersionID = $this->GetParam("articleversion_id");
				if ($strArticleVersionID != "")
				{
					return parent::MakeIndexSQLCondition($strEntity);
				}
				$strArticleName = $this->GetParam("name");				
				$strArticleTitle = $this->GetParam("title");
				if ($strArticleName == "")
				{
					if ($strArticleTitle == "")
					{
						$this->SetError("PARAMETER_NAME_MISSING");
						return false;
					}
					$strNameOrTitleField = "TITLE";
					$strArticleNameOrTitle = $strArticleTitle;
				}
				else
				{
					if ($strArticleTitle != "")
					{
						$this->SetError("PARAMETER_AMBIGUOUS");
						return false;
					}
					$strArticleNameOrTitle = $strArticleName;
				}
				$strLanguage = $this->GetParam("language");
				if ($strLanguage == "")
				{
					$this->SetError("PARAMETER_LANGUAGE_MISSING");
					return false;	
				}
				$strCountry = $this->GetParam("country");
				
				if ($strCountry == "")
				{
					$strCountrySQL = " AND ".$this->MakeSQL_Helper_Field("COUNTRY")." = ".MakeSQL_Helper_Value("");
				}
				else
				{
					$strCountrySQL = " AND (".$this->MakeSQL_Helper_Field("COUNTRY")." = ".MakeSQL_Helper_Value($strCountry)." OR ".$this->MakeSQL_Helper_Field("COUNTRY")." = ".MakeSQL_Helper_Value("").")";
				}

				$strType = $this->GetParam("type");
				if ($strType == "")   
				{
					$strType = "STATIC";
				}
				
				$strDateTimeNow = RenderDateTimeNow();
				
				$strSQL = "`".$strNameOrTitleField."` = ".MakeSQL_Helper_Value($strArticleNameOrTitle)
							." AND ".$this->MakeSQL_Helper_Field("LANGUAGE")." = ".MakeSQL_Helper_Value($strLanguage)
							.$strCountrySQL
							." AND ".$this->MakeSQL_Helper_Field("STATUS")." = ".MakeSQL_Helper_Value("ACTIVE") 
							." AND ".$this->MakeSQL_Helper_Field("PUBLICATION_START_DATETIME")." <= ".MakeSQL_Helper_Value($strDateTimeNow)
							." AND (".$this->MakeSQL_Helper_Field("PUBLICATION_END_DATETIME")."  >= ".MakeSQL_Helper_Value($strDateTimeNow)." OR ".$this->MakeSQL_Helper_Field("PUBLICATION_END_DATETIME")." = 0)"
							." AND ".$this->MakeSQL_Helper_Field("TYPE")." = ".MakeSQL_Helper_Value($strType);    
							
				$strSQL .= " ORDER BY "	.$this->MakeSQL_Helper_Field("COUNTRY")." DESC, "
																.$this->MakeSQL_Helper_Field("CREATION_DATETIME")." DESC, "
																.$this->MakeSQL_Helper_Field("ARTICLEVERSION_ID")." DESC LIMIT 0, 1";
				
				return $strSQL;
			}
		}
		
		function CommandSet($strEntity)
		{
			if ($strEntity == "article")
			{
				$strArticleVersionID = $this->GetParam("articleversion_id");
				if ($strArticleVersionID == "")
				{
					$this->CommandAdd("article");
					return;
				}

				parent::CommandSet("article");
				$strError = $this->GetError();
				if (	$strError == "ITEM_NOT_FOUND"
					|| $strError == "ITEM_NOT_SPECIFIED"
					|| $strError == "PARAMETER_NAME_MISSING")
				{
					$this->SetError("",true);
					$this->CommandAdd("article");
				}
			}
		}
		
		function CommandDelete($strEntity)
		{
			if ($strEntity == "article")
			{
				$strArticleVersionID = $this->GetParam("articleversion_id");
				$strName = $this->GetParam("name");
				$strTitle = $this->GetParam("title");
				if ($strArticleVersionID != "")
				{
					if ($strName != "" || $strTitle != "")
					{
						$this->SetError("PARAMETER_AMBIGUOUS");
						return;
					}
					parent::CommandDelete("article");
					return;
				}
				$arrayEntityDefinition = $this->GetEntityDefinition($strEntity);
				$strTable = ArrayGetValue($arrayEntityDefinition,"dbtablename");
				$strSQLCondition = "";
				if ($strName != "")
				{
					if ($strTitle != "")
					{
						$this->SetError("PARAMETER_AMBIGUOUS");
						return;
					}
					$strSQLCondition = "`NAME` = ".MakeSQL_Helper_Value($strName);
				}
				else if ($strTitle != "")
				{
					$strSQLCondition = "`NAME` = ".MakeSQL_Helper_Value(MakeID($strTitle));			
				}
				if ($strSQLCondition == "")
				{
					$this->SetError("ITEM_NOT_SPECIFIED");
					return;
				}
				$strSQL = MakeSQL_Delete(
					$strTable,
					$strSQLCondition);
				if ($this->DatabaseQuery($strSQL) != true)
				{
					return;
				}
			}
		}
		
		function CommandRename()
		{
			$strNameOld = $this->GetParam("name");
			$strNameNew = $this->GetParam("new_name");

			$strTable = $this->GetDatabaseTableName("article");
			
			
			$strSQLCondition = "`NAME` = ".MakeSQL_Helper_Value($strNameOld);
			$strSQL = MakeSQL_Select(
				$strTable,
				"*",
				$strSQLCondition);
			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}
			$arrayData = $this->m_database->GetData();
			$nRows = ArrayCount($arrayData);
			if ($nRows == 0)
			{
				$this->SetError("ITEM_NOT_FOUND");
				return;
			}			
			
			
			$strSQLCondition = "`NAME` = ".MakeSQL_Helper_Value($strNameNew);
			$strSQL = MakeSQL_Select(
				$strTable,
				"*",
				$strSQLCondition);
			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}
			$arrayData = $this->m_database->GetData();
			$nRows = ArrayCount($arrayData);
			if ($nRows > 1)
			{
				$this->SetError("ARTICLE_EXISTS");
				return;
			}


			$strSQLCondition = "`NAME` = ".MakeSQL_Helper_Value($strNameOld);
			$arrayFields = array();
			$arrayFields["LASTCHANGE_DATETIME"] = RenderDateTimeNow();
			$arrayFields["NAME"] = $strNameNew;
			$strSQL = MakeSQL_Update(
				$strTable,
				$arrayFields,
				$strSQLCondition);
			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}
		}
		
		
		function CommandCleanUp()
		{
			$this->Trace("CommandCleanUp");

			$strTable = $this->GetDatabaseTableName("article");
			
			$strName = $this->GetParam("name");
			if ($strName != "")
			{
				
				
				/*
				$strThresholdDateTime = $this->GetParam("threshold_datetime");
								
				$strThresholdVersions = $this->GetParam("threshold_versions");
				$nThresholdVersions = intval($strThresholdVersions);
				
				$strLanguage = $this->GetParam("language");
				if ($strLanguage == "")
				{
					$this->SetError("PARAMETER_LANGUAGE_MISSING");
					return;	
				}
				
				$strCountry = $this->GetParam("country");
				$strType = $this->GetParam("type");
				if ($strType == "")   
				{
					$strType = "STATIC";
				}

				$strDateTimeNow = RenderDateTimeNow();				
				
				*/
				
				/*
				
				what should be deleted?
				
				* versionen, die nicht mehr benötigt werden
				
				PUBLICATION_START_DATETIME noch nicht erreicht? NIE löschen!
				
				PUBLICATION_END_DATETIME vorbei? -> kann weg
				ACTIVE und es gibt etwas aktuelleres  vorbei? -> kann weg
				
				LIMIT 95,18446744073709551615
				
				how should it be sorted?
				
				*/

				/*

				$strSQL = "`NAME` = ".MakeSQL_Helper_Value($strName)
							." AND `LANGUAGE` = ".MakeSQL_Helper_Value($strLanguage)
							." AND `COUNTRY` = ".MakeSQL_Helper_Value($strCountry)
							." AND `STATUS` = ".MakeSQL_Helper_Value("ACTIVE") 
							." AND `TYPE` = ".MakeSQL_Helper_Value($strType)
							//." AND `PUBLICATION_START_DATETIME` < ".MakeSQL_Helper_Value($strDateTimeNow)
							//." AND (`PUBLICATION_END_DATETIME` <= ".MakeSQL_Helper_Value($strDateTimeNow)." OR `PUBLICATION_END_DATETIME` = 0)"
							;
							
				$strSQL .= " ORDER BY `CREATION_DATETIME` DESC, `ARTICLEVERSION_ID` DESC LIMIT 0, 10000";
			
				$strSQL = MakeSQL_Select(
					$strTable,
					$this->GetStorageFields("article"),
					$strCondition)
				*/

				/*
				$strSQL = MakeSQL_Delete(
					$strTable,
					""
					"`END_DATETIME` < '".RenderDateTimeNow()."'");
	
				if ($this->DatabaseQuery($strSQL) != true)
				{
					return;
				}
				
				*/

			
				$this->SetError("COMMAND_NOT_IMPLEMENTED");				
				return;
			}
			else
			{
				// 
				$this->SetError("COMMAND_NOT_IMPLEMENTED");
				return;	
			}

			return;
		}		
		
		function OnCommandAddPrepare($strEntity,&$arrayItem)
		{
			if ($strEntity == "article")
			{
				$bResult = $this->OnCommandSetPrepare($strEntity,$arrayItem);
				if ($bResult == false)
				{
					return false;	
				}
				$arrayItem["CREATION_DATETIME"] = RenderDateTimeNow();				
				$strName = ArrayGetValue($arrayItem,"NAME");
				if ($strName == "")
				{
					$arrayItem["NAME"] = MakeID(ArrayGetValue($arrayItem,"TITLE"));
				}
			}
			return parent::OnCommandAddPrepare($strEntity,$arrayItem);		
		}

		function OnCommandSetPrepare($strEntity,&$arrayItem)
		{
			if ($strEntity == "article")
			{			
				$arrayItem["LASTCHANGE_DATETIME"] = RenderDateTimeNow();
				if (ArrayGetValue($arrayItem,"PUBLICATION_END_DATETIME") == "")
				{
					$arrayItem["PUBLICATION_END_DATETIME"] = DATETIME_INFINITE;
				}
			}
			return parent::OnCommandSetPrepare($strEntity,$arrayItem);
		}  
		
		function OnCommandGetComplete($strEntity,&$arrayItem,$strContext)
		{
			if ($strEntity == "article")
			{
				if ($strContext == "get")
				{
					if ($this->GetParam("update_lastaccess") == true)
					{
						$strArticleVersionID = $arrayItem["ARTICLEVERSION_ID"];
						$strSQL = MakeSQL_Update(
							$this->GetDatabaseTableName("article"),
							array(
								"LASTACCESS_DATETIME" => RenderDateTimeNow()
								),
							"`ARTICLEVERSION_ID` = ".MakeSQL_Helper_Value($strArticleVersionID)
							);
						$this->DatabaseQuery($strSQL);
					}
				}
				if ($arrayItem["PUBLICATION_END_DATETIME"] >= DATETIME_INFINITE)
				{
					$arrayItem["PUBLICATION_END_DATETIME"] = "";
				}				
				if ($arrayItem["REFERENCE_ARTICLEVERSION_ID"] == "0")
				{
					$arrayItem["REFERENCE_ARTICLEVERSION_ID"] = "";
				}
				if ($arrayItem["AUTHOR_USER_ID"] == "0")
				{
					$arrayItem["AUTHOR_USER_ID"] = "";
				}				
			}
			return parent::OnCommandGetComplete($strEntity,$arrayItem,$strContext);
		}		


	}

