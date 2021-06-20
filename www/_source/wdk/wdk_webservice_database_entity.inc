<?php

	require_once("wdk_webservice_database.inc");  
	require_once("wdk_entitydefinitions.inc");
	require_once("wdk_entity.inc");

	abstract class CDatabaseEntityWebService extends CDatabaseWebService
	{
		protected $m_entityDef;
		public $m_bLegacyHack;
	
		function __construct(
			$strName,
			$strSourceDir,
			$strServiceDir,
			$arrayMeta,
			$arrayConfig,
			$arrayParams,
			$arrayEntityDefinitions,
			$arrayObfuscationItems = array()
			)
		{
			$this->m_bLegacyHack = false;
			$this->m_entityDef = new CEntityDefinitions();
			$arrayErrors = array();
			$this->m_entityDef->SetEntityDefinitions($arrayEntityDefinitions);
			
			parent::__construct(
				$strName,
				$strSourceDir,
				$strServiceDir,
				$arrayMeta,
				$arrayConfig,
				$arrayParams,
				$arrayObfuscationItems);
		}
		
		function OnInit()
		{
			$bResult = parent::OnInit();
			if ($bResult == false)
			{
				return false;
			}
			$arrayErrors = array();
			if ($this->m_entityDef->CheckEntityDefinitions($arrayErrors,ENTITYDEF_WEBSERVICE) == false)
			{
				$this->Trace("CheckEntityDefinition failed!");
				$this->Trace($arrayErrors);
				$this->SetError("ENTITYDEFINITION"); 
				return false;	
			}
			return true;
		}
		
		final
		function GetEntityDefinitions()
		{
			return $this->m_entityDef->GetEntityDefinitions();
		}
		
		final
		function GetEntityDefinition($strEntity)
		{
			return $this->m_entityDef->GetEntityDefinition($strEntity);
		}
		
		final
		function GetFilterOptionsFromEntityDefinition($strEntity)
		{
			$arrayFilterOptions = array();
			$arrayEntityDefinition = $this->GetEntityDefinition($strEntity);
			$arrayAttributes = ArrayGetValue($arrayEntityDefinition,"attributes");
			if (!is_array($arrayAttributes))
			{
				return array();
			}
			foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
			{
				$bFilterOption = ArrayGetValue($arrayAttribute,"filteroption");
				if ($bFilterOption)
				{
					$arrayFilterOptions[$strAttribute] = $this->GetFieldName($strEntity,$strAttribute);
				}
			}
			return $arrayFilterOptions;			
		}

		final
		function GetSortOptionsFromEntityDefinition($strEntity)
		{
			$arraySortOptions = array();
			$arrayEntityDefinition = $this->GetEntityDefinition($strEntity);
			$arrayAttributes = ArrayGetValue($arrayEntityDefinition,"attributes");
			if (!is_array($arrayAttributes))
			{
				return array();
			}
			foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
			{
				$bSortOption = GetBoolValue(ArrayGetValue($arrayAttribute,"sortoption"));
				if ($bSortOption)
				{
					$arraySortOptions[$strAttribute] = $this->GetFieldName($strEntity,$strAttribute);
				}
			}
			return $arraySortOptions;			
		}
		
		final
		function GetDatabaseTableName($strEntity)
		{
			return $this->m_entityDef->GetDatabaseTableName($strEntity);
		}
		
		final
		function GetWebserviceParamName($strEntity,$strAttribute)
		{
			return $this->m_entityDef->GetWebserviceParamName($strEntity,$strAttribute);
		}
		
		final
		function GetFieldName($strEntity,$strAttribute)
		{
			return $this->m_entityDef->GetFieldName($strEntity,$strAttribute);
		}
		
		function OnInitParameterDefinition(&$paramDef)
		{
			parent::OnInitParameterDefinition($paramDef);
			$arrayEntityDefinitions = $this->GetEntityDefinitions();
			if (is_array($arrayEntityDefinitions))
			{
				if (ArrayCount($arrayEntityDefinitions) > 0)
				{
					$paramDef->AddParameterDefinition_blocksize();
					$paramDef->AddParameterDefinition_offset();
				}
				foreach ($arrayEntityDefinitions as $strEntity => $arrayEntityDefinition)
				{
					$bDefault = GetBoolValue(ArrayGetValue($arrayEntityDefinition,"default"));
					if ($bDefault == true)
					{
						$strPrefix = "";	
					}
					else
					{
						$strPrefix = $strEntity;	
					}
					$arrayAttributes = ArrayGetValue($arrayEntityDefinition,"attributes");
					$arrayFilterOptions = array();
					$arraySortOptions = array();
					foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
					{
						$bWebserviceParam = GetBoolValue(ArrayGetValue($arrayAttribute,"webserviceparam"));
						if ($bWebserviceParam == true)
						{
							$strWebserviceParam = $this->GetWebserviceParamName($strEntity,$strAttribute);
							$strType = ArrayGetValue($arrayAttribute,"type");
							switch ($strType)
							{
								case "numericid": 
									$bResult = $paramDef->AddParameterDefinitionNumericID(
										$strWebserviceParam);
									break;
									
								case "string": 
									$nMaxLength = intval(ArrayGetValue($arrayAttribute,"maxlen"));
									$nMinLength = intval(ArrayGetValue($arrayAttribute,"minlen"));
									$strCharset = ArrayGetValue($arrayAttribute,"charset");
									$bResult = $paramDef->AddParameterDefinitionString(
										$strWebserviceParam,
										$nMinLength,
										$nMaxLength,
										$strCharset);
									break;

								case "password": 
									$bResult = $paramDef->AddParameterDefinitionPassword(
										$strWebserviceParam);
									break;

									
								case "bool":
									$bResult = $paramDef->AddParameterDefinitionBoolean(
										$strWebserviceParam);
									break;
									
								case "float": 
									$fMax = floatval(ArrayGetValue($arrayAttribute,"max"));
									$fMin = floatval(ArrayGetValue($arrayAttribute,"min"));
									$bResult = $paramDef->AddParameterDefinitionFloat(
										$strWebserviceParam,
										$fMin,
										$fMax,
										true);
									break;
									
								case "integer":
									$nMax = floatval(ArrayGetValue($arrayAttribute,"max"));
									$nMin = floatval(ArrayGetValue($arrayAttribute,"min"));
									$bResult = $paramDef->AddParameterDefinitionInteger(
										$strWebserviceParam,
										$nMin,
										$nMax,
										true);
									break;								

								case "datetime": 
									$bResult = $paramDef->AddParameterDefinitionDateTime(
										$strWebserviceParam,
										"",
										"",
										true);
									break;

								case "date": 
									$bResult = $paramDef->AddParameterDefinitionDate(
										$strWebserviceParam,
										"",
										"",
										true);
									break;

								case "enum": 
									$bResult = $arrayEnum = ArrayGetValue($arrayAttribute,"enum");
									if (!is_array($arrayEnum))
									{
										$arrayEnum = array();
									}
									$paramDef->AddParameterDefinitionEnum(
										$strWebserviceParam,
										$arrayEnum);
									break;

								case "sortorder": 
									$bResult = $paramDef->AddParameterDefinitionInteger(
										$strWebserviceParam,
										0,
										INTEGER_MAX,
										true);
									break;
									
								case "language": 
									$bResult = $paramDef->AddParameterDefinitionLanguage(
										$strWebserviceParam,
										true);
									break;

								case "currency": 
									$bResult = $paramDef->AddParameterDefinitionCurrency(
										$strWebserviceParam,
										true);
									break;

								case "email": 
									$bResult = $paramDef->AddParameterDefinitionEmail(
										$strWebserviceParam,
										true);
									break;
									
								default:
									$this->Trace("Attribute \"$strAttribute\" uses unknown or unsupported type \"$strType\"");
									$this->SetError("IMPLEMENTATION");
									break;
							}
							if ($bResult == false)
							{
								$this->Trace("Failed to add parameter definition for attribute \"$strAttribute\". The parameter definition differs from a previous parameter definition.");
								$this->SetError("IMPLEMENTATION");
								break;
							}
							$bFilterOption = ArrayGetValue($arrayAttribute,"filteroption");
							if ($bFilterOption)
							{
								$arrayFilterOptions[] = $strWebserviceParam;
							}
							$bSortOption = ArrayGetValue($arrayAttribute,"sortoption");
							if ($bSortOption)
							{
								$arraySortOptions[] = $strWebserviceParam;
							}
						}
					}
					 
					$paramDef->AddParameterDefinitions_Filter($arrayFilterOptions,$strPrefix,ArrayCount($arrayFilterOptions)*2);
					$paramDef->AddParameterDefinitions_Sort($arraySortOptions,$strPrefix);
				}
			}
		}
		
		function OnInitCommandDefinition(&$commandDef)
		{
			parent::OnInitCommandDefinition($commandDef);
			
			$arrayEntityDefinitions = $this->GetEntityDefinitions();
			foreach ($arrayEntityDefinitions as $strEntity => $arrayEntityDefinition)
			{
				$strIndexAttribute = ArrayGetValue($arrayEntityDefinition,"indexattribute");
				$bDefault = GetBoolValue(ArrayGetValue($arrayEntityDefinition,"default"));
				if ($bDefault == true)
				{
					$strPrefix = "";	
				}
				else
				{
					$strPrefix = $strEntity;	
				}

				$arrayAttributes = ArrayGetValue($arrayEntityDefinition,"attributes");
				$nFilterOptions = 0;
				$nSortOptions = 0;
				$arrayParamsAddSet = array();
				$arrayParamsIndex = array();
				foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
				{
					$bFilterOption = GetBoolValue(ArrayGetValue($arrayAttribute,"filteroption"));
					if ($bFilterOption)
					{
						$nFilterOptions++;
					}
					$bSortOption = GetBoolValue(ArrayGetValue($arrayAttribute,"sortoption"));
					if ($bSortOption)
					{
						$nSortOptions++;
					}
					$bWebserviceParam = GetBoolValue(ArrayGetValue($arrayAttribute,"webserviceparam"));
					if ($bWebserviceParam != false)
					{
						if ($this->IsAttributeReadOnly($arrayAttribute) == false)
						{
							$arrayParamsAddSet[] = $this->GetWebserviceParamName($strEntity,$strAttribute);
						}
					}
					if ($strAttribute == $strIndexAttribute)
					{
						$arrayParamsIndex[] = $this->GetWebserviceParamName($strEntity,$strAttribute);
					}
				}
				
				$arrayCommands = ArrayGetValue($arrayEntityDefinition,"webservice","commands");
				if (is_array($arrayCommands) != true)
				{
					$arrayCommands = array();
				}
				foreach ($arrayCommands as $strCommand => $arrayCommand)
				{
					$arrayParams = ArrayAppend(array(),ArrayGetValue($arrayCommand,"additionalparams"));
					if ($strCommand == "list")
					{
	
						$commandDef->AddCommandDefinition_list(
							$nFilterOptions*2,
							$nSortOptions,
							$strPrefix,
							$arrayParams);
					}
					else
					{
						$strCommandName = ArrayGetValue($arrayCommand,"name");
						if ($strCommandName == "")
						{
							$strCommandName = $strCommand.$strPrefix;
						}
						if ($strCommand == "add")
						{
							$arrayParams = ArrayAppend($arrayParams,$arrayParamsAddSet);
							$commandDef->AddCommandDefinition(
								$strCommandName,
								$arrayParams);	
						}
						if ($strCommand == "set")
						{
							$arrayParams = ArrayAppend($arrayParams,$arrayParamsIndex);
							$arrayParams = ArrayAppend($arrayParams,$arrayParamsAddSet);
							$commandDef->AddCommandDefinition(
								$strCommandName,
								$arrayParams);	
						}
						if ($strCommand == "get")
						{
							$arrayParams = ArrayAppend($arrayParams,$arrayParamsIndex);
							$commandDef->AddCommandDefinition(
								$strCommandName,
								$arrayParams);	
						}
						if ($strCommand == "delete")
						{
							$arrayParams = ArrayAppend($arrayParams,$arrayParamsIndex);
							$commandDef->AddCommandDefinition(
								$strCommandName,
								$arrayParams);	
						}
					}
				}
			}
		}
		
		function OnCommand($strCommand)
		{
			$this->Trace("CDatabaseEntityWebService::OnCommand('$strCommand')");
			$arrayResult = $this->GetGenericCommandAndEntity();
			if (is_array($arrayResult))
			{
				$strGenericCommand = ArrayGetValue($arrayResult,"genericcommand");
				$strEntity = ArrayGetValue($arrayResult,"entity");
				switch ($strGenericCommand)
				{
				case "list":
					$this->CommandList($strEntity);	
					return;
				case "add":
					$this->CommandAdd($strEntity);	
					return;
				case "set":
					$this->CommandSet($strEntity);	
					return;
				case "get":
					$this->CommandGet($strEntity);	
					return;
				case "delete":
					$this->CommandDelete($strEntity);	
					return;
				default:
					break;
				}			
			}
			parent::OnCommand($strCommand);	
		}
		
		final
		function GetGenericCommandAndEntity()
		{
			$strCommand = $this->GetCommand(); 
			$arrayEntityDefinitions = $this->GetEntityDefinitions();
			$arrayGenericCommands = array(
				"list",
				"add",
				"set",
				"get",
				"delete"
				);
			foreach ($arrayEntityDefinitions as $strEntity => $arrayEntityDefinition)
			{
				foreach ($arrayGenericCommands as $strGenericCommand)
				{
					if ($this->IsCommandSupported($strEntity,$strGenericCommand))
					{
						if ($this->GetCommandName($strEntity,$strGenericCommand) == $strCommand)
						{
							return array(
								"entity" => $strEntity,
								"genericcommand" => $strGenericCommand);
						}
					}
				}
			}
			return false;
		}
			
		final
		function IsCommandSupported($strEntity,$strCommand)
		{
			$arrayCommands = ArrayGetValue(
				$this->GetEntityDefinition($strEntity),
				"webservice","commands");
			if (!is_array($arrayCommands))
			{
				return false;	
			}
			return ArrayKeyExists($arrayCommands,$strCommand);
		}
		
		final
		function GetCommandName($strEntity,$strCommandGeneric)
		{
			$arrayEntityDefinition = $this->GetEntityDefinition($strEntity);
			if (!is_array($arrayEntityDefinition))
			{
				return "";	
			}
			$bDefault = GetBoolValue(ArrayGetValue($arrayEntityDefinition,"default"));
			if ($bDefault == true)
			{
				return $strCommandGeneric;
			}
			else
			{
				if ($strCommandGeneric == "list")
				{
					return $strEntity."list";	
				}
				else
				{
					return $strCommandGeneric.$strEntity;	
				}
			}
		}
		
		function OnGetDefaultCsvPath()
		{
			$arrayResult = $this->GetGenericCommandAndEntity();
			if (is_array($arrayResult))
			{
				$strGenericCommand = ArrayGetValue($arrayResult,"genericcommand");
				$strEntity = ArrayGetValue($arrayResult,"entity");
				$arrayEntityDefinition = $this->GetEntityDefinition($strEntity);
				$strDefaultCsvPath = ArrayGetValue($arrayEntityDefinition,"webservice","commands",$strGenericCommand,"defaultcsvpath");
				if ($strDefaultCsvPath == "")
				{
					switch ($strGenericCommand)	
					{
					case "list":
						return "LIST";
					case "add":
						$strItemTag = ArrayGetValue($arrayEntityDefinition,"webservice","newitemidtag");
						return $strItemTag;					
					case "set":
						return "";
					case "get":
						$strItemTag = ArrayGetValue($arrayEntityDefinition,"webservice","itemtag");
						return $strItemTag;
					case "delete":
						return "";
					default:
						return "";
					}
				}
			}
			return parent::OnGetDefaultCsvPath();	
		}

		function CommandSelfCheck()
		{
			parent::CommandSelfCheck();
			if ($this->GetError() != "")
			{
				return;	
			}
			$this->Trace("CDatabaseEntityWebservice::CommandSelfCheck()");
			// No specific checks at this time...			
		}

		function CommandInstall()
		{
			$this->Trace("CDatabaseEntityWebService::CommandInstall()");
			$arrayEntityDefinitions = $this->GetEntityDefinitions();
			foreach ($arrayEntityDefinitions as $strEntity => $arrayEntityDefinition)
			{
				$strTableName = ArrayGetValue($arrayEntityDefinition,"dbtablename");
				if ($strTableName == "")
				{
					$this->Trace("Entity ".$strEntity." does not require installation.");
				}
				else
				{
					$this->Trace("Install entity ".$strEntity);
					$entity = new CEntity($this,$strEntity,$arrayEntityDefinition);
					
					$strSQL = $entity->MakeSQL_CreateTable();
					if ($strSQL == false)
					{
						$this->Trace("CEntity::MakeSQL_CreateTable() returned false");
						$this->SetError("IMPLEMENTATION");
						return;
					}
					$this->Trace("SQL:\n".$strSQL);
					if ($this->m_database->Query($strSQL) != true)
					{
						$this->TraceDatabaseError();
						if ($this->m_database->GetErrorCode() != 1050)
						{
							$this->SetError("DATABASE_QUERY_FAILED");
							return;
							
						}
					}				
				}
			}
			parent::CommandInstall();	
		}

		function CommandList(
			$strEntity)
		{
			$this->Trace("CDatabaseEntityWebService::CommandList('$strEntity')");
			if ($strEntity == "")
			{
				$this->SetError("IMPLEMENTATION");
				return;	
			}
			$arrayEntityDefinition = $this->GetEntityDefinition($strEntity);
			$arrayAttributes = ArrayGetValue($arrayEntityDefinition,"attributes");
			$strTable = ArrayGetValue($arrayEntityDefinition,"dbtablename");
			
			// GET FILTER OPTIONS FROM PARAMS
			$arrayFilter = array();
			$arrayFilterConfig = $this->GetFilterOptionsFromEntityDefinition($strEntity);
			
			$bDefault = GetBoolValue(ArrayGetValue($arrayEntityDefinition,"default"));
			$strPrefix = "";
			if ($bDefault == false)
			{
				$strPrefix = $strEntity."_";	
			}
			
			$nFilterCount = ArrayCount($arrayFilterConfig) * 2;
			for ($nIndex = 1; $nIndex <= $nFilterCount; $nIndex++)
			{
				$strFilterName = $this->GetParam($strPrefix."filter".$nIndex);
				$strFilterValue = $this->GetParam($strPrefix."filter".$nIndex."_value");
				$strFilterOperator = $this->GetParam($strPrefix."filter".$nIndex."_operator");
				if ($strFilterOperator == "")
				{
					$strFilterOperator = "=";
				}
				if ($strFilterName != "")
				{
					foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
					{
						if ($strFilterName == $strAttribute)
						{
							$arrayForeignKey = ArrayGetValue($arrayAttribute,"foreignkey");

							$arrayFilterOption = array();
							$arrayFilterOption["value"] = $strFilterValue;
							$arrayFilterOption["operator"] = $strFilterOperator;
							if (is_array($arrayForeignKey) == true)
							{
								$arrayFilterOption["field"] = ArrayGetValue($arrayForeignKey,"dbfield");
								$arrayFilterOption["table"] = ArrayGetValue($arrayForeignKey,"dbtable");
								$arrayFilterOption["localattribute"] = ArrayGetValue($arrayForeignKey,"attribute");
								$arrayFilterOption["foreignkey"] = true;
								//$arrayFilterOption["table"] = "t1";
							}
							else
							{
								$arrayFilterOption["field"] = $this->GetFieldName($strEntity,$strAttribute);
								$arrayFilterOption["table"] = $strTable;
								$arrayFilterOption["foreignkey"] = false;
							}
							$arrayFilter[] = $arrayFilterOption;

						}
					}
				}
			}
			
			// GET SORT OPTIONS FROM PARAMS
			$arraySort = array();
			$arraySortConfig = $this->GetSortOptionsFromEntityDefinition($strEntity);
						
			$nSortCount = ArrayCount($arraySortConfig);
			for ($nIndex = 1; $nIndex <= $nSortCount; $nIndex++)
			{
				$strSortOption = $this->GetParam($strPrefix."sort".$nIndex);
				$strSortOrder = $this->GetParam($strPrefix."sort".$nIndex."_order");
				if ($strSortOption != "")
				{
					foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
					{
						if ($strSortOption == $strAttribute)
						{
/*							$strSortField = $this->GetFieldName($strEntity,$strAttribute);
							if ($strSortField != "")
							{*/
								$arraySort[$strAttribute] = ($strSortOrder != "")?(StringUpperCase($strSortOrder)):("ASC");
							//} 
						}
					}
				}
			}
			
			// Get the fields which should be part of the result array
			$arrayAttributes = ArrayGetValue($arrayEntityDefinition,"attributes");
			
	
			$nMaxBlockSize = intval(ArrayGetValue($arrayEntityDefinition,"maxblocksize"));
			if ($nMaxBlockSize == 0)
			{
				$nMaxBlockSize = ENTITY_MAXBLOCKSIZE_DEFAULT;
			}
			
			$strIndexAttribute = ArrayGetValue($arrayEntityDefinition,"indexattribute");
			$strDefaultSortAttribute = ArrayGetValue($arrayEntityDefinition,"sorting","defaultattribute");
			if ($strDefaultSortAttribute == "")
			{
				$strDefaultSortAttribute = $strIndexAttribute;
			}
			$strDefaultSortOrder = StringUpperCase(ArrayGetValue($arrayEntityDefinition,"sorting","defaultorder"));
			if ($strDefaultSortOrder == "")
			{
				$strDefaultSortOrder = "DESC";
			}
			//$strSQLSortByField = $this->GetFieldName($strEntity,$strDefaultSortAttribute);
			$nBlockSize = $this->GetParam("blocksize");
			$nOffset = $this->GetParam("offset");
			
			if ($nBlockSize == "")
			{
				$this->SetError("PARAMETER_BLOCKSIZE_MISSING");
				return;
			}
			$nBlockSize = intval($nBlockSize);
			if ($nBlockSize > $nMaxBlockSize)
			{
				$this->SetError("PARAMETER_BLOCKSIZE_TOOHIGH");
				return;
			}
			if ($nBlockSize < 0)
			{
				$this->SetError("PARAMETER_BLOCKSIZE_TOOLOW");
				return;
			}


			if ($nOffset == "")
			{
				$this->SetError("PARAMETER_OFFSET_MISSING");
				return;
			}
			$nOffset = intval($nOffset);
			if ($nOffset < 0)
			{
				$this->SetError("PARAMETER_OFFSET_TOOLOW");
				return;
			}

			$nLowerLimit = $nOffset * $nBlockSize;
			$nUpperLimit = (($nOffset+1) * $nBlockSize);



			$arraySQL = array();
			$arraySQL["command"] = "SELECT";
			$arraySQL["calculate_found_rows"] = true;
			$arraySQL["tables"][] = array(
				"table" => $strTable,
				"as" => "t0");

			$strTable = "t0";



			// SORT OPTIONS

			$nSortOptions = 0;
			$bDefaultSortFieldContained = false;
			
			foreach ($arraySort as $strAttribute => $strSortOrder)
			{
				if ($strAttribute == $strDefaultSortAttribute)
				{
					$bDefaultSortFieldContained = true;
				}
			}
			if ($bDefaultSortFieldContained == false)
			{
				$arraySort[$strDefaultSortAttribute] = $strDefaultSortOrder;
			}


			foreach ($arraySort as $strAttribute => $strSortOrder)
			{
				$strTable2 = $strTable;
				$strField = $this->GetFieldName($strEntity,$strAttribute);
				$arrayForeignKey = ArrayGetValue($arrayAttributes,$strAttribute,"foreignkey");
				
				// In case of foreign keys the table reference must be withheld.
				if (is_array($arrayForeignKey) == true)
				{
					$strTable2 = "";
				}
				
				$arraySQL["orderby"][] = array(
					"table" => $strTable2,
					"field" => $strField,
					"order" => $strSortOrder);

				$nSortOptions++;
			}

			
	
			// ATTRIBUTES
						
			$nForeignTables = 0;
			foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
			{
				$bNoStorage = GetBoolValue(ArrayGetValue($arrayAttribute,"nostorage"));
				if ($bNoStorage == false)
				{
					$arrayForeignKey = ArrayGetValue($arrayAttribute,"foreignkey");
					if (is_array($arrayForeignKey))
					{
						$strForeignTable = ArrayGetValue($arrayForeignKey,"dbtable");
						$strForeignField = ArrayGetValue($arrayForeignKey,"dbfield");
						$strLocalAttribute = ArrayGetValue($arrayForeignKey,"attribute");
						$strForeignKey = ArrayGetValue($arrayForeignKey,"dbkey");
						
						$arrayFrom = ArrayGetValue($arraySQL,"tables","$strForeignTable.$strLocalAttribute");
						if (is_array($arrayFrom))
						{
							$strForeignTableAlias = ArrayGetValue($arrayFrom,"as");
						}
						else
						{
							$nForeignTables++;
							$strForeignTableAlias = "t$nForeignTables";
						}

						$arraySQL["fields"][] = array(
							"table" => $strForeignTableAlias,
							"field" => $strForeignField,
							"as" => $this->GetFieldName($strEntity,$strAttribute)
							);
						$arraySQL["tables"]["$strForeignTable.$strLocalAttribute"] = array(
							"join" => "LEFT JOIN",
							"table" => $strForeignTable,
							"as" => $strForeignTableAlias,
							"on" => array(
								"left" => array(
									"table" => $strTable,
									"field" => $this->GetFieldName($strEntity,$strLocalAttribute)
									),
								"operator" => "=",
								"right" => array(
									"table" => $strForeignTableAlias,
									"field" => $strForeignKey
									)
								)
							);
					}
					else
					{
						$bExcludeFromGet = GetBoolValue(ArrayGetValue($arrayAttribute,"excludefromget"));
						$bExcludeFromList = GetBoolValue(ArrayGetValue($arrayAttribute,"excludefromlist"));
						if (($bExcludeFromList == false) && ($bExcludeFromGet == false))
						{
							$arraySQL["fields"][] = array(
								"table" => $strTable,
								"field" => $this->GetFieldName($strEntity,$strAttribute)
								);
						}
					}
				}
			}
			
			
			
			// FILTER OPTIONS
			$nFilterOptions = ArrayCount($arrayFilter);
			if ($nFilterOptions > 0)
			{
				foreach ($arrayFilter as $arrayFilterOption)
				{
					$vValue = ArrayGetValue($arrayFilterOption,"value");
					$strOperator = ArrayGetValue($arrayFilterOption,"operator");
					$strField = ArrayGetValue($arrayFilterOption,"field");
					$strTable = ArrayGetValue($arrayFilterOption,"table");
					$bForeignKey = ArrayGetValue($arrayFilterOption,"foreignkey");
					if ($bForeignKey == true)
					{
						$strTable = ArrayGetValue($arraySQL,"tables","$strTable.".ArrayGetValue($arrayFilterOption,"localattribute"),"as");
					}
					else
					{
						$strTable = "t0";
					}
					
					if ($strOperator == "")
					{
						$strOperatorSQL = "=";
					}
					else if ($strOperator == "contains")
					{
						$strOperatorSQL = "LIKE";
						$vValue = "%".$vValue."%";
					}
					else if ($strOperator == "like")
					{
						$strOperatorSQL = "LIKE";
					}
					else if ($strOperator == "in")
					{ 
	      		$strOperatorSQL = "IN";
	      		$vValue = array("values" => explode(",",$vValue));
	    		}
	    		else
	    		{
	    			$strOperatorSQL = $strOperator;
	    		}
	    		
	    		$arraySQL["where"]["and"][] = array(
	    			"left" => array("table" => $strTable, "field" => $strField),
	    			"operator" => $strOperatorSQL,
	    			"right" => $vValue);
					$nFilterOptions--;
				}
			}

			
			
			
			
			
			

			$arraySQL["limit"]["offset"] = $nLowerLimit;
			$arraySQL["limit"]["rowcount"] = $nBlockSize;

			
			$this->OnCommandListPrepareSQL($strEntity,$arraySQL);
			$strError = "";
			$strSQL = MakeSQL($arraySQL,$strError);
			if ($strSQL == false)
			{
				$this->Trace($arraySQL);
				$this->Trace("MakeSQL() returned false");
				$this->Trace($strError);
				$this->SetError("IMPLEMENTATION");
				return;
			}

			
			
			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}

	
			$arrayList = $this->DatabaseGetDataFromLastQuery();
			
			$nRows = ArrayCount($arrayList);
			
			$nTotalRows = $this->m_database->QueryFoundRows();
			$this->SetResultValue($nTotalRows,"TOTALENTRIES");
			$this->SetResultValue("$nOffset","OFFSET");
			$this->SetResultValue(max($nBlockSize,$nRows),"BLOCKSIZE");
			$this->SetResultValue("".max((intval(ceil($nTotalRows / $nBlockSize))-1),0)."","MAXOFFSET");
	
			
			foreach ($arrayList as &$arrayItem)
			{
				$bResult = $this->OnCommandGetComplete($strEntity,$arrayItem,"list");
				if ($bResult == false)
				{
					return;	
				}
			}

			//$this->Trace($arrayList);

			$arrayFields = $this->GetPublicFieldsList($strEntity);
			$arrayList = ArrayFilterListByKeys($arrayList,$arrayFields);
			
			
			$this->SetResultValue($arrayList,"LIST");
		}
		
		
		function CommandAdd($strEntity)
		{
			$this->Trace("CommandAdd");
			
			if ($strEntity == "")
			{
				$this->Trace("Entity has not been specified.");
				$this->SetError("IMPLEMENTATION");
				return;	
			}
			$arrayEntityDefinition = $this->GetEntityDefinition($strEntity);
			$arrayAttributes = ArrayGetValue($arrayEntityDefinition,"attributes");
			$strTable = ArrayGetValue($arrayEntityDefinition,"dbtablename");
			if ($strTable == "")
			{
				$this->Trace("Table name is not set.");
				$this->SetError("IMPLEMENTATION");
				return;	
			}
			
			$arrayFields = array();
			
			$strIndexAttribute = ArrayGetValue($arrayEntityDefinition,"indexattribute");
			
			foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
			{
				if ($strIndexAttribute != $strAttribute)
				{
					$strWebserviceParam = $this->GetWebserviceParamName($strEntity,$strAttribute);
					$strFieldName = $this->GetFieldName($strEntity,$strAttribute);
					if ($this->IsParam($strWebserviceParam))
					{
						$strValue = $this->GetParam($strWebserviceParam);
						$arrayFields[$strFieldName] = $strValue;
					}
				}				
			}
			
			$bResult = $this->OnCommandAddPrepare($strEntity,$arrayFields);
			if ($bResult == false)
			{
				return;	
			}
			
			$arrayFields = $this->UpdateSortOrder("add",$strEntity,array(),$arrayFields);
			if ($arrayFields == false)
			{
				return;	
			}
					
			$strSQL = MakeSQL_Insert(
				$strTable,
				$arrayFields);

			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}
			$strNewItemIDTag = ArrayGetValue($arrayEntityDefinition,"webservice","newitemidtag");
			$strNewItemID = $this->m_database->GetLastInsertID();
			$this->SetResultValue("".$strNewItemID,$strNewItemIDTag);
			
			$this->OnCommandAddComplete($strEntity,$arrayFields,$strNewItemID);

			return;
		}
		
		function OnCommandAddPrepare($strEntity,&$arrayItem)
		{
			return true;				
		}
		function OnCommandAddComplete($strEntity,$arrayItem,$strNewItemID)
		{
			return true;				
		}

		function MakeSQL_Helper_Field($strField)
		{
			if ($this->m_bLegacyHack == true)
			{
				return "`$strField`";
			}
			return "/*TablePrefix*/`t0`.`$strField`";
		}

		function GetIndexAttribute($strEntity) 
		{
			$arrayEntityDefinition = $this->GetEntityDefinition($strEntity);
			return ArrayGetValue($arrayEntityDefinition,"indexattribute");
		}
		
		function GetIndexValueFromParam($strEntity)  
		{
			$strIndexAttribute = $this->GetIndexAttribute($strEntity);
			$strWebserviceParam = $this->GetWebserviceParamName($strEntity,$strIndexAttribute);
			$strValue = $this->GetParam($strWebserviceParam);
			return $strValue;
		}
		
		function MakeIndexSQLCondition($strEntity)
		{
			$arrayEntityDefinition = $this->GetEntityDefinition($strEntity);
			$strIndexAttribute = ArrayGetValue($arrayEntityDefinition,"indexattribute");
			$strWebserviceParam = $this->GetWebserviceParamName($strEntity,$strIndexAttribute);
			$strDBFieldName = $this->GetFieldName($strEntity,$strIndexAttribute);
			$strValue = $this->GetParam($strWebserviceParam);
			if ($strValue == "")
			{
				return false;	
			}
			$strSQLCondition = $this->MakeSQL_Helper_Field($strDBFieldName)." = ".MakeSQL_Helper_Value($strValue);	
			return $strSQLCondition;
		}

		function CommandGet($strEntity)
		{
			$this->Trace("CommandGet('$strEntity')");

			if ($strEntity == "")
			{
				$this->SetError("IMPLEMENTATION");
				return;	
			}
			$arrayEntityDefinition = $this->GetEntityDefinition($strEntity);
			$arrayAttributes = ArrayGetValue($arrayEntityDefinition,"attributes");
			
			$strSQLCondition = $this->MakeIndexSQLCondition($strEntity);
			if ($strSQLCondition == "")
			{
				return;	
			}

			$strTable = $this->GetDatabaseTableName($strEntity);
			
			$bResult = $this->OnCommandGetPrepare($strEntity);
			if ($bResult == false)
			{
				return;	
			}

			$arraySQL = array();
			$arraySQL["command"] = "SELECT";
			$arraySQL["tables"][] = array(
				"table" => $strTable,
				"as" => "t0"
				);
			$strTable = "t0";
			$arraySQL["where"]["sql"] = $strSQLCondition;
		
			$nForeignTables = 0;

			foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
			{
				$bNoStorage = GetBoolValue(ArrayGetValue($arrayAttribute,"nostorage"));
				if ($bNoStorage == false)
				{
					$arrayForeignKey = ArrayGetValue($arrayAttribute,"foreignkey");
					if (is_array($arrayForeignKey))
					{
						
						/*
						$nForeignTables++;
						
						$strForeignTable = ArrayGetValue($arrayForeignKey,"dbtable");
						$strForeignField = ArrayGetValue($arrayForeignKey,"dbfield");
						$strLocalAttribute = ArrayGetValue($arrayForeignKey,"attribute");
						$strForeignKey = ArrayGetValue($arrayForeignKey,"dbkey");
						
						$arraySQL["fields"][] = array(
							"table" => "t$nForeignTables",
							"field" => $strForeignField,
							"as" => $this->GetFieldName($strEntity,$strAttribute)
							);
						$arraySQL["tables"]["$strForeignTable.$strLocalAttribute"] = array(
							"join" => "LEFT JOIN",
							"table" => $strForeignTable,
							"as" => "t$nForeignTables",
							"on" => array(
								"left" => array(
									"table" => $strTable,
									"field" => $this->GetFieldName($strEntity,$strLocalAttribute)
									),
								"operator" => "=",
								"right" => array(
									"table" => "t$nForeignTables",
									"field" => $strForeignKey
									)
								)
							);
							*/
							
						$strForeignTable = ArrayGetValue($arrayForeignKey,"dbtable");
						$strForeignField = ArrayGetValue($arrayForeignKey,"dbfield");
						$strLocalAttribute = ArrayGetValue($arrayForeignKey,"attribute");
						$strForeignKey = ArrayGetValue($arrayForeignKey,"dbkey");
						
						$arrayFrom = ArrayGetValue($arraySQL,"tables","$strForeignTable.$strLocalAttribute");
						if (is_array($arrayFrom))
						{
							$strForeignTableAlias = ArrayGetValue($arrayFrom,"as");
						}
						else
						{
							$nForeignTables++;
							$strForeignTableAlias = "t$nForeignTables";
						}

						$arraySQL["fields"][] = array(
							"table" => $strForeignTableAlias,
							"field" => $strForeignField,
							"as" => $this->GetFieldName($strEntity,$strAttribute)
							);
						$arraySQL["tables"]["$strForeignTable.$strLocalAttribute"] = array(
							"join" => "LEFT JOIN",
							"table" => $strForeignTable,
							"as" => $strForeignTableAlias,
							"on" => array(
								"left" => array(
									"table" => $strTable,
									"field" => $this->GetFieldName($strEntity,$strLocalAttribute)
									),
								"operator" => "=",
								"right" => array(
									"table" => $strForeignTableAlias,
									"field" => $strForeignKey
									)
								)
							);
					}
					else
					{
						$bExcludeFromGet = GetBoolValue(ArrayGetValue($arrayAttribute,"excludefromget"));
						if ($bExcludeFromGet == false)
						{
							$arraySQL["fields"][] = array(
								"table" => $strTable,
								"field" => $this->GetFieldName($strEntity,$strAttribute)
								);
						}
					}
				}
			}
			
			


			



			$this->OnCommandGetPrepareSQL($strEntity,$arraySQL);
			$strSQL = MakeSQL($arraySQL);

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
			if ($nRows > 1)
			{
				$this->SetError("ITEM_AMBIGUOUS");
				return;
			}

			$arrayItem = ArrayGetValue($arrayData,0);
			
			$bResult = $this->OnCommandGetComplete($strEntity,$arrayItem,"get");
			if ($bResult == false)
			{
				return;	
			}
			
			$strEntityTag = ArrayGetValue($arrayEntityDefinition,"webservice","itemtag");
			$this->SetResultValue($arrayItem,$strEntityTag);
			return;
		}
		function OnCommandGetPrepare($strEntity)
		{
			return true;				
		}
		function OnCommandGetComplete($strEntity,&$arrayItem,$strContext)
		{
			$arrayEntityDefinition = $this->GetEntityDefinition($strEntity);
			$arrayAttributes = ArrayGetValue($arrayEntityDefinition,"attributes");
			
			foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
			{
				$strType = $arrayAttribute["type"];
				$strField = $this->GetFieldName($strEntity,$strAttribute);
				$bZeroUnSet = GetBoolValue(ArrayGetValue($arrayAttribute,"zerounset"));
				switch ($strType)
				{
					case "datetime":
						$strValue = ArrayGetValue($arrayItem,$strField);
						if ($strValue == "0000-00-00 00:00:00")
						{
							$arrayItem[$strField] = "";	
						}
						break;

					case "date":
						$strValue = ArrayGetValue($arrayItem,$strField);
						if ($strValue == "0000-00-00")
						{
							$arrayItem[$strField] = "";	
						}
						break;
						
					case "integer":
					case "numbericid":
						$nValue = ArrayGetValue($arrayItem,$strField);
						if ($bZeroUnSet == true && $nValue == 0)
						{
							$arrayItem[$strField] = "";	
						}
						break;
						
					case "float":
						$strValue = ArrayGetValue($arrayItem,$strField);
						$fValue = floatval($strValue);
						if ($bZeroUnSet == true && $fValue == 0)
						{
							$arrayItem[$strField] = "";
						}	
						else
						{
							$nPrecision = intval(ArrayGetValue($arrayAttribute,"floatprecision"));
							if ($nPrecision == 0)
							{
								$nPrecision = ENTITY_FLOATPRECISION_DEFAULT;	
							}
							$arrayItem[$strField] = RenderNumber($fValue,$nPrecision);
						}
						break;

					default:
						break;
					
				}
			}
			return true;
		}

		function CommandSet($strEntity)
		{
			$this->Trace("CommandSet('$strEntity')");
			
			if ($strEntity == "")
			{
				$this->SetError("IMPLEMENTATION");
				return;	
			}
			$arrayEntityDefinition = $this->GetEntityDefinition($strEntity);
			$arrayAttributes = ArrayGetValue($arrayEntityDefinition,"attributes");
			
			$this->m_bLegacyHack = true;
			$strSQLCondition = $this->MakeIndexSQLCondition($strEntity);
			if ($strSQLCondition == "")
			{
				$this->SetError("ITEM_NOT_SPECIFIED");
				return;	
			}

			$strTable = ArrayGetValue($arrayEntityDefinition,"dbtablename");
			
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
			if ($nRows > 1)
			{
				$this->SetError("ITEM_AMBIGUOUS");
				return;
			}
			$arrayRow = ArrayGetValue($arrayData,0);

			
			$strIndexAttribute = ArrayGetValue($arrayEntityDefinition,"indexattribute");
			
			$arrayItem = array();
			foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
			{
				if ($strIndexAttribute != $strAttribute)
				{
					$bReadOnly = $this->IsAttributeReadOnly($arrayAttribute);
					$bNoStorage = ArrayGetValue($arrayAttribute,"nostorage");
					if (!$bReadOnly && !$bNoStorage)
					{
						$strWebserviceParam = $this->GetWebserviceParamName($strEntity,$strAttribute);
						$strDBFieldName = $this->GetFieldName($strEntity,$strAttribute);
						if ($this->IsParam($strWebserviceParam))
						{
							$strValue = $this->GetParam($strWebserviceParam);
							$arrayItem[$strDBFieldName] = $strValue;
						}
						else
						{
							$arrayItem[$strDBFieldName] = ArrayGetValue($arrayRow,$strDBFieldName);
						}
					}
				}				
			}
			
			$bResult = $this->OnCommandSetPrepare($strEntity,$arrayItem);
			if ($bResult == false)
			{
				return;	
			}

			$arrayItem = $this->UpdateSortOrder("set",$strEntity,$arrayRow,$arrayItem);
			if ($arrayItem == false)
			{
				return;	
			}
							
			$strSQL = MakeSQL_Update(
				$strTable,
				$arrayItem,
				$strSQLCondition);

			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}
						
			$this->OnCommandSetComplete($strEntity,$arrayItem);

			return;
		}

		function OnCommandSetPrepare($strEntity,&$arrayItem)
		{
			return true;				
		}
		function OnCommandSetComplete($strEntity,$arrayItem)
		{
			return true;
		}
		function OnCommandListPrepareSQL($strEntity,&$arraySQL)
		{
			return;	
		}
		function OnCommandGetPrepareSQL($strEntity,&$arraySQL)
		{
			return;	
		}
		

		function CommandDelete($strEntity)
		{
			$this->Trace("CommandDelete");
			
			if ($strEntity == "")
			{
				$this->SetError("IMPLEMENTATION");
				return;	
			}
		
			$arrayEntityDefinition = $this->GetEntityDefinition($strEntity);

			$this->m_bLegacyHack = true;
			$strSQLCondition = $this->MakeIndexSQLCondition($strEntity);
			if ($strSQLCondition == false)
			{
				$this->SetError("ITEM_NOT_SPECIFIED");
				return;
			}
			if ($strSQLCondition == "")
			{
				return;	
			}
						
			$strTable = ArrayGetValue($arrayEntityDefinition,"dbtablename");
			
			$strSQL = MakeSQL_Select(
				$strTable,
				"*",
				$strSQLCondition . " LIMIT 1");

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
			if ($nRows > 1)
			{
				$this->SetError("ITEM_AMBIGUOUS");
				return;
			}
			$arrayItem = ArrayGetValue($arrayData,0);
			

			$strSQL = MakeSQL_Delete(
				$strTable,
				$strSQLCondition);

			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}

			$this->UpdateSortOrder("delete",$strEntity,$arrayItem,array());

			return;	
		}
		
		/*
			The UpdateSortOrder method prepares the room for a new or changed item.
			It changes the order of all other affected items and leaves the free spot for the
			new or changed item. It does not actually modify the item in focus.
		
		*/
		function UpdateSortOrder($strCommand,$strEntity,$arrayItemOld,$arrayItemNew)
		{
			if ($strEntity == "")
			{
				$this->SetError("IMPLEMENTATION");
				return false;	
			}
			
			$bResult = true;
			
			$this->Trace("UpdateSortOrder: $strCommand");
			$arrayEntityDefinition = $this->GetEntityDefinition($strEntity);
			$arrayAttributes = ArrayGetValue($arrayEntityDefinition,"attributes");
			$strIndexAttribute = ArrayGetValue($arrayEntityDefinition,"indexattribute");
			$strTable = $this->GetDatabaseTableName($strEntity);
			$arrayItem = $arrayItemNew;
			foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
			{
				$strType = ArrayGetValue($arrayAttribute,"type");
				$strFieldName = $this->GetFieldName($strEntity,$strAttribute);
				if ($strType == "sortorder")
				{
					$nSortOrderOld = intval(ArrayGetValue($arrayItemOld,$strFieldName));
					$strIndex = ArrayGetValue($arrayItem,$this->GetFieldName($strEntity,$strIndexAttribute));
					
					switch ($strCommand)
					{
						
					case "add":
						$nSortOrderNew = ArrayGetValue($arrayItem,$strFieldName);
						$this->Trace("UpdateSortOrder: $strAttribute: New: ".$nSortOrderNew);
					
						// Find out what the highest rank is...
						$arraySQL = array(
							"command" => "SELECT",
							"fields" => array(
								array(
									"field" => $strFieldName)
								),
							"tables" => $strTable,
							"orderby" => array(
								array(
									"field" => $strFieldName,
									"order" => "DESC"
									)
								),
								"limit" => array(
									"offset" => 0,
									"rowcount" => 1)
							);	
						$strError = "";
						$strSQL = MakeSQL($arraySQL,$strError);
						if ($this->DatabaseQuery($strSQL) != true)
						{
							$this->Trace("DatabaseQuery: returned false");
						}
						$arrayData = $this->m_database->GetData();
						if (ArrayCount($arrayData) == 0)
						{
							$this->Trace("UpdateSortOrder: $strAttribute: Highest value: List is yet empty. Setting order to 0");
							$arrayItem[$strFieldName] = 0;
							break;
						}
						$nSortOrderMaxOld = intval(ArrayGetValue($arrayData,0,$strFieldName));
						$this->Trace("UpdateSortOrder: $strAttribute: Highest value: $nSortOrderMaxOld");
						
						if ($nSortOrderNew == null)
						{
							$arrayItem[$strFieldName] = $nSortOrderMaxOld+1;
							break;						
						}
						$nSortOrderNew = intval($nSortOrderNew);
						if ($nSortOrderNew > $nSortOrderMaxOld)
						{
							$arrayItem[$strFieldName] = $nSortOrderMaxOld+1;
							break;
						}

						if ($nSortOrderNew < 0)
						{
							$nSortOrderNew = 0;	
						}
						
						// Move all higher values to the "right" by one
						$arraySQL = array();
						$arraySQL["command"] = "UPDATE";
						$arraySQL["calculate_found_rows"] = true;
						$arraySQL["tables"] = $strTable;
		    		$arraySQL["where"]["and"][] = array(
							"left" => array(
								"field" => $strFieldName),
							"operator" => ">=",
							"right" => $nSortOrderNew);
						$arraySQL["values"][] = array(
							"field" => $this->GetFieldName($strEntity,$strAttribute),
							"value" => "++" 
							);
						$strSQL = MakeSQL($arraySQL,$strError);
						//$this->Trace("MakeSQL: Error: ".$strError);
						if ($this->DatabaseQuery($strSQL) != true)
						{
							$this->Trace("The process of moving all values to the \"right\" failed!");
							$bResult = false;
							break;
						}
						$arrayItem[$strFieldName] = $nSortOrderNew;
						break;
						
					
					case "set":
					
						$nSortOrderNew = ArrayGetValue($arrayItem,$strFieldName);
						$this->Trace("UpdateSortOrder: $strAttribute: New: ".$nSortOrderNew);
						$this->Trace("UpdateSortOrder: $strAttribute: Old: ".$nSortOrderOld);
						if ($nSortOrderNew == null)
						{
							break;						
						}
						$nSortOrderNew = intval($nSortOrderNew);
						if ($nSortOrderNew == $nSortOrderOld)
						{
							break;	
						}
						
						
						// Find out what the highest rank is...
						$arraySQL = array(
							"command" => "SELECT",
							"fields" => array(
								array(
									"field" => $strFieldName)
								),
							"tables" => $strTable,
							"orderby" => array(
								array(
									"field" => $strFieldName,
									"order" => "DESC"
									)
								),
								"limit" => array(
									"offset" => 0,
									"rowcount" => 1)
							);	
						$strError = "";
						$strSQL = MakeSQL($arraySQL,$strError);
						if ($this->DatabaseQuery($strSQL) != true)
						{
							$this->Trace("DatabaseQuery: returned false");
						}
						$arrayData = $this->m_database->GetData();
						if (ArrayCount($arrayData) == 0)
						{
							$this->Trace("UpdateSortOrder: $strAttribute: Highest value: List is yet empty. Setting order to 0. BUT THIS IS INCONSISTENT IN A SET COMMAND!");
							$arrayItem[$strFieldName] = 0;
							break;
						}
						$nSortOrderMaxOld = intval(ArrayGetValue($arrayData,0,$strFieldName));
						$this->Trace("UpdateSortOrder: $strAttribute: Highest value: $nSortOrderMaxOld");
						
						if ($nSortOrderNew > $nSortOrderMaxOld)
						{
							$nSortOrderNew = $nSortOrderMaxOld;
						}
						
						
						// shift "left" or shift "right" ?
						
						if ($nSortOrderNew < $nSortOrderOld)
						{
							// the new value is "left" off the old position, so we must move some items to the "right"

							// Move all higher values to the "right" by one
							$arraySQL = array();
							$arraySQL["command"] = "UPDATE";
							$arraySQL["calculate_found_rows"] = true;
							$arraySQL["tables"] = $strTable;
			    		$arraySQL["where"]["and"][] = array(
								"left" => array(
									"field" => $strFieldName),
								"operator" => ">=",
								"right" => $nSortOrderNew);
			    		$arraySQL["where"]["and"][] = array(
								"left" => array(
									"field" => $strFieldName),
								"operator" => "<=",
								"right" => $nSortOrderOld);
							$arraySQL["values"][] = array(
								"field" => $this->GetFieldName($strEntity,$strAttribute),
								"value" => "++" 
								);
							$strSQL = MakeSQL($arraySQL,$strError);
							if ($this->DatabaseQuery($strSQL) != true)
							{
								$this->Trace("The process of moving all values to the \"right\" failed!");
								$bResult = false;
								break;
							}
							$arrayItem[$strFieldName] = $nSortOrderNew;
							
						}
						else
						{
							// the new value is "right" off the old position, so we must move some items to the "left"

							// Move all lower values to the "left" by one
							$arraySQL = array();
							$arraySQL["command"] = "UPDATE";
							$arraySQL["calculate_found_rows"] = true;
							$arraySQL["tables"] = $strTable;
			    		$arraySQL["where"]["and"][] = array(
								"left" => array(
									"field" => $strFieldName),
								"operator" => ">",
								"right" => $nSortOrderOld);
			    		$arraySQL["where"]["and"][] = array(
								"left" => array(
									"field" => $strFieldName),
								"operator" => "<=",
								"right" => $nSortOrderNew);
							$arraySQL["values"][] = array(
								"field" => $this->GetFieldName($strEntity,$strAttribute),
								"value" => "--" 
								);
							$strSQL = MakeSQL($arraySQL,$strError);
							//$this->Trace($strSQL);
							$this->Trace("MakeSQL: Error: ".$strError);
							if ($this->DatabaseQuery($strSQL) != true)
							{
								$this->Trace("The process of moving all values to the \"left\" failed!");
								$bResult = false;
								break;
							}
							$arrayItem[$strFieldName] = $nSortOrderNew;
								
							
						}
						break;

						
					case "delete":
					
						$this->Trace("UpdateSortOrder: $strAttribute: decrease all higher items by 1.");
					
						$arraySQL = array();
						$arraySQL["command"] = "UPDATE";
						$arraySQL["calculate_found_rows"] = true;
						$arraySQL["tables"] = $strTable;
		    		$arraySQL["where"]["and"][] = array(
							"left" => array(
								"field" => $strFieldName),
							"operator" => ">=",
							"right" => $nSortOrderOld);
						$arraySQL["values"][] = array(
							"field" => $this->GetFieldName($strEntity,$strAttribute),
							"value" => "--" 
							);
						$strSQL = MakeSQL($arraySQL,$strError);
						$this->Trace("MakeSQL: Error: ".$strError);
						if ($this->DatabaseQuery($strSQL) != true)
						{
							$this->Trace("The process of moving all values to the \"left\" failed!");
							$bResult = false;
							break;
						}
						break;
					}
					
				}
			}
			if ($bResult == false)
			{
				return false;
			}
			return $arrayItem;				
			
		}


		function GetEntityItem($strEntity,$strIndex,&$arrayItem)
		{
			if ($strEntity == "")
			{
				$this->SetError("IMPLEMENTATION");
				return false;	
			}
			$arrayEntityDefinition = $this->GetEntityDefinition($strEntity);
			$arrayAttributes = ArrayGetValue($arrayEntityDefinition,"attributes");
			$strIndexAttribute = ArrayGetValue($arrayEntityDefinition,"indexattribute");
			$strDBFieldName = $this->GetFieldName($strEntity,$strIndexAttribute);
			$strSQLCondition = "`".$strDBFieldName."` = ".MakeSQL_Helper_Value($strIndex);	
			
			
			$strTable = ArrayGetValue($arrayEntityDefinition,"dbtablename");

			$strSQL = MakeSQL_Select(
				$strTable,
				"*",
				$strSQLCondition);

			if ($this->DatabaseQuery($strSQL) != true)
			{
				return false;
			}
			
			$arrayData = $this->m_database->GetData();
			
			$nRows = ArrayCount($arrayData);
			if ($nRows == 0)
			{
				return false;
			}
			if ($nRows > 1)
			{
				return false;
			}

			$arrayItem = ArrayGetValue($arrayData,0);
			return true;
		}


		function GetPublicFieldsGet($strEntity)
		{
			$arrayEntityDefinition = $this->GetEntityDefinition($strEntity);
			if (!is_array($arrayEntityDefinition))
			{
				return false;	
			}
			$arrayAttributes = ArrayGetValue($arrayEntityDefinition,"attributes");
			if (!is_array($arrayAttributes))
			{
				return false;	
			}
			$arrayFields = array();
			foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
			{
				$bHide = GetBoolValue(ArrayGetValue($arrayAttribute,"excludefromget"));
				if ($bHide == false)
				{
					$arrayFields[] = $this->GetFieldName($strEntity,$strAttribute);
				}
			}
			return $arrayFields;
		}

	

		function GetPublicFieldsList($strEntity)
		{
			$arrayEntityDefinition = $this->GetEntityDefinition($strEntity);
			if (!is_array($arrayEntityDefinition))
			{
				return false;	
			}
			$arrayAttributes = ArrayGetValue($arrayEntityDefinition,"attributes");
			if (!is_array($arrayAttributes))
			{
				return false;	
			}
			$arrayFields = array();
			foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
			{
				$bHide = GetBoolValue(ArrayGetValue($arrayAttribute,"excludefromlist")) || GetBoolValue(ArrayGetValue($arrayAttribute,"excludefromget"));
				if ($bHide == false)
				{
					$arrayFields[] = $this->GetFieldName($strEntity,$strAttribute);
				}
			}
			return $arrayFields;
		}



		function GetStorageFields($strEntity)
		{
			$arrayEntityDefinition = $this->GetEntityDefinition($strEntity);
			if (!is_array($arrayEntityDefinition))
			{
				return false;	
			}
			$arrayAttributes = ArrayGetValue($arrayEntityDefinition,"attributes");
			if (!is_array($arrayAttributes))
			{
				return false;	
			}
			$arrayFields = array();
			foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
			{
				$bNoStorage = GetBoolValue(ArrayGetValue($arrayAttribute,"nostorage"));
				$bIsForeignKey = ArrayKeyExists($arrayAttribute,"foreignkey");
				if ($bNoStorage == false && $bIsForeignKey == false)
				{
					$arrayFields[] = $this->GetFieldName($strEntity,$strAttribute);
				}
			}
			return $arrayFields;
		}		

		function IsAttributeReadOnly($arrayAttribute)
		{
			if (GetBoolValue(ArrayGetValue($arrayAttribute,"readonly")) == true)
			{
				return true;
			}
			if (is_array(ArrayGetValue($arrayAttribute,"foreignkey")) == true)
			{
				return true;
			}
			return false;
		}
		
		
		
	}
	
